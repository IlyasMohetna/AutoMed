#ifndef SIMULATION_ENGINE_HPP
#define SIMULATION_ENGINE_HPP

#include <string>
#include <vector>
#include <queue>
#include <map>
#include <ctime>
#include <iostream>
#include <nlohmann/json.hpp>

#include "../models/Patient.hpp"
#include "../models/BlocOperatoire.hpp"
#include "../models/EquipeMedicale.hpp"
#include "../models/SalleAttente.hpp"
#include "../models/SalleReveil.hpp"
#include "../enums/EtatSimulation.hpp"
#include "../enums/AlgorithmeOrdonnancement.hpp"
#include "Evenement.hpp"
#include "GenerateurPatients.hpp"
#include "Scheduler.hpp"
#include "Statistics.hpp"

namespace AutoMed {

/**
 * Configuration pour créer une simulation
 */
struct ConfigSimulation {
    std::string nom;
    int dureeSimulationMinutes;
    AlgorithmeOrdonnancement algorithme;
    int nombreBlocs;
    int nombreEquipes;
    int capaciteSalleAttente;
    int capaciteSalleReveil;
    double tauxArriveeHoraireUrgences;
    int nombrePatientsElectifs;

    ConfigSimulation()
        : nom("Simulation"),
          dureeSimulationMinutes(480),
          algorithme(AlgorithmeOrdonnancement::FCFS),
          nombreBlocs(3),
          nombreEquipes(3),
          capaciteSalleAttente(50),
          capaciteSalleReveil(20),
          tauxArriveeHoraireUrgences(2.0),
          nombrePatientsElectifs(10) {}
};

/**
 * Moteur de simulation à événements discrets
 */
class SimulationEngine {
private:
    int id;
    std::string nom;
    time_t tempsSimulation;                           // Horloge virtuelle
    time_t tempsDebutReel;                            // Timestamp réel du démarrage
    EtatSimulation etat;
    AlgorithmeOrdonnancement algorithme;
    int dureeSimulationMinutes;
    
    // File d'événements (priority_queue)
    std::priority_queue<Evenement> fileEvenements;
    
    // Composants hospitaliers
    SalleAttente* salleAttente;
    std::vector<BlocOperatoire*> blocsOperatoires;
    SalleReveil* salleReveil;
    std::vector<EquipeMedicale*> equipesDisponibles;
    
    // Tous les patients (pour gestion mémoire)
    std::map<int, Patient*> tousLesPatients;
    std::map<int, MembreMedical*> tousLesMembres;
    
    // Générateur et stats
    GenerateurPatients* generateur;
    Statistics* stats;
    
    // Historique d'événements récents (pour affichage)
    std::vector<Evenement> historiqueEvenements;
    const size_t MAX_HISTORIQUE = 50;

public:
    /**
     * Constructeur
     */
    SimulationEngine(int simId, const ConfigSimulation& config)
        : id(simId),
          nom(config.nom),
          tempsSimulation(std::time(nullptr)),
          tempsDebutReel(0),
          etat(EtatSimulation::CREATED),
          algorithme(config.algorithme),
          dureeSimulationMinutes(config.dureeSimulationMinutes) {
        
        // Créer les composants
        salleAttente = new SalleAttente(1, "Salle d'attente principale", config.capaciteSalleAttente);
        salleReveil = new SalleReveil(1, "Salle de réveil", config.capaciteSalleReveil, 60);
        
        // Créer les blocs opératoires
        for (int i = 0; i < config.nombreBlocs; i++) {
            blocsOperatoires.push_back(
                new BlocOperatoire(i + 1, "Bloc Opératoire " + std::to_string(i + 1), 15)
            );
        }
        
        // Créer les équipes médicales
        for (int i = 0; i < config.nombreEquipes; i++) {
            creerEquipeMedicale(i + 1);
        }
        
        // Créer le générateur de patients
        generateur = new GenerateurPatients(
            config.tauxArriveeHoraireUrgences,
            config.nombrePatientsElectifs
        );
        
        // Créer les statistiques
        stats = new Statistics();
        
        std::cout << "[SIMULATION] Simulation #" << id << " créée: " << nom << std::endl;
    }

    /**
     * Destructeur
     */
    ~SimulationEngine() {
        // Libérer la mémoire
        delete salleAttente;
        delete salleReveil;
        delete generateur;
        delete stats;
        
        for (auto* bloc : blocsOperatoires) delete bloc;
        for (auto* equipe : equipesDisponibles) delete equipe;
        for (auto& pair : tousLesPatients) delete pair.second;
        for (auto& pair : tousLesMembres) delete pair.second;
    }

    /**
     * Initialise la simulation avec les événements de départ
     */
    void initialiser() {
        std::cout << "[SIMULATION] Initialisation de la simulation..." << std::endl;
        
        // Générer les patients électifs
        auto patientsElectifs = generateur->genererPatientsElectifs(tempsSimulation, dureeSimulationMinutes);
        for (auto* patient : patientsElectifs) {
            tousLesPatients[patient->getId()] = patient;
            
            // Planifier leur arrivée
            planifierEvenement(Evenement(
                TypeEvenement::ARRIVEE_PATIENT,
                patient->getHorodatageArrivee(),
                patient->getId()
            ));
        }
        
        // Planifier la première arrivée d'urgence
        planifierProchaineArriveeUrgence();
        
        // Planifier la fin de simulation
        planifierEvenement(Evenement(
            TypeEvenement::FIN_SIMULATION,
            tempsSimulation + (dureeSimulationMinutes * 60)
        ));
        
        std::cout << "[SIMULATION] " << patientsElectifs.size() << " patients électifs programmés" << std::endl;
        std::cout << "[SIMULATION] Durée: " << dureeSimulationMinutes << " minutes" << std::endl;
        std::cout << "[SIMULATION] Algorithme: " << algorithmeToString(algorithme) << std::endl;
    }

    /**
     * Démarre la simulation
     */
    void demarrer() {
        if (etat == EtatSimulation::CREATED) {
            initialiser();
        }
        
        etat = EtatSimulation::RUNNING;
        tempsDebutReel = std::time(nullptr);
        stats->demarrer(tempsSimulation);
        
        std::cout << "\n[SIMULATION] ===== DÉMARRAGE DE LA SIMULATION =====" << std::endl;
        std::cout << "[SIMULATION] Horloge virtuelle: " << tempsSimulation << std::endl;
        
        // Traiter tous les événements
        while (etat == EtatSimulation::RUNNING && !fileEvenements.empty()) {
            step();
        }
        
        if (fileEvenements.empty() && etat == EtatSimulation::RUNNING) {
            etat = EtatSimulation::FINISHED;
            stats->terminer(tempsSimulation);
            std::cout << "\n[SIMULATION] ===== SIMULATION TERMINÉE =====" << std::endl;
            std::cout << stats->toString() << std::endl;
        }
    }

    /**
     * Avance d'un événement (pour exécution pas à pas)
     */
    void step() {
        if (fileEvenements.empty()) {
            return;
        }
        
        // Récupérer le prochain événement
        Evenement evt = fileEvenements.top();
        fileEvenements.pop();
        
        // Avancer l'horloge virtuelle
        tempsSimulation = evt.horodatage;
        
        // Afficher l'événement
        std::cout << "[" << getTempsEcouleMinutes() << "min] " << evt.toString() << std::endl;
        
        // Sauvegarder dans l'historique
        ajouterAHistorique(evt);
        
        // Traiter l'événement
        traiterEvenement(evt);
        
        // Tenter d'assigner des patients aux blocs
        tentativeAssignation();
    }

    /**
     * Met en pause la simulation
     */
    void pause() {
        if (etat == EtatSimulation::RUNNING) {
            etat = EtatSimulation::PAUSED;
            std::cout << "[SIMULATION] Simulation mise en pause" << std::endl;
        }
    }

    /**
     * Reprend la simulation
     */
    void reprendre() {
        if (etat == EtatSimulation::PAUSED) {
            etat = EtatSimulation::RUNNING;
            std::cout << "[SIMULATION] Simulation reprise" << std::endl;
        }
    }

    /**
     * Arrête la simulation
     */
    void arreter() {
        etat = EtatSimulation::STOPPED;
        stats->terminer(tempsSimulation);
        std::cout << "[SIMULATION] Simulation arrêtée" << std::endl;
        std::cout << stats->toString() << std::endl;
    }

private:
    /**
     * Traite un événement
     */
    void traiterEvenement(const Evenement& evt) {
        switch (evt.type) {
            case TypeEvenement::ARRIVEE_PATIENT: {
                Patient* patient = tousLesPatients[evt.patientId];
                if (patient) {
                    arriveePatient(patient);
                }
                
                // Si urgence, planifier la prochaine
                if (patient && patient->getPriorite() == PrioritePatient::URGENCE) {
                    planifierProchaineArriveeUrgence();
                }
                break;
            }
            
            case TypeEvenement::FIN_OPERATION: {
                BlocOperatoire* bloc = trouverBloc(evt.blocOperatoireId);
                if (bloc && bloc->estOccupe()) {
                    Patient* patient = bloc->getPatientActuel();
                    terminerOperation(bloc);
                    
                    // Planifier fin de nettoyage
                    planifierEvenement(Evenement(
                        TypeEvenement::FIN_NETTOYAGE_BLOC,
                        tempsSimulation + (bloc->getDureeNettoyageMinutes() * 60),
                        -1,
                        bloc->getId()
                    ));
                    
                    // Transférer en salle de réveil
                    if (patient) {
                        transfererEnSalleReveil(patient);
                    }
                }
                break;
            }
            
            case TypeEvenement::FIN_NETTOYAGE_BLOC: {
                BlocOperatoire* bloc = trouverBloc(evt.blocOperatoireId);
                if (bloc) {
                    bloc->terminerNettoyage();
                    std::cout << "    → " << bloc->getNom() << " est maintenant LIBRE" << std::endl;
                }
                break;
            }
            
            case TypeEvenement::SORTIE_SALLE_REVEIL: {
                Patient* patient = tousLesPatients[evt.patientId];
                if (patient) {
                    libererPatientReveil(patient);
                }
                break;
            }
            
            case TypeEvenement::FIN_SIMULATION: {
                arreter();
                break;
            }
            
            default:
                break;
        }
    }

    /**
     * Arrivée d'un patient
     */
    void arriveePatient(Patient* patient) {
        if (!patient) return;
        
        salleAttente->ajouterPatient(patient);
        stats->enregistrerArrivee(patient);
        
        std::cout << "    → " << patient->getNomComplet() 
                  << " (Priorité: " << prioriteToString(patient->getPriorite())
                  << ", Type: " << typeOperationToString(patient->getTypeOperation())
                  << ", Durée estimée: " << patient->getDureeEstimeeMinutes() << "min)" << std::endl;
    }

    /**
     * Tente d'assigner des patients aux blocs disponibles
     */
    void tentativeAssignation() {
        while (Scheduler::assignationPossible(blocsOperatoires, equipesDisponibles, salleAttente)) {
            // Trouver un bloc disponible
            BlocOperatoire* bloc = Scheduler::trouverBlocDisponible(blocsOperatoires);
            if (!bloc) break;
            
            // Sélectionner le prochain patient
            Patient* patient = Scheduler::selectionnerProchainPatient(salleAttente, algorithme);
            if (!patient) break;
            
            // Trouver une équipe disponible
            EquipeMedicale* equipe = Scheduler::trouverEquipeDisponible(
                equipesDisponibles, 
                patient->getTypeOperation()
            );
            if (!equipe) {
                // Remettre le patient en attente
                salleAttente->ajouterPatient(patient);
                break;
            }
            
            // Démarrer l'opération
            demarrerOperation(bloc, patient, equipe);
        }
    }

    /**
     * Démarre une opération
     */
    void demarrerOperation(BlocOperatoire* bloc, Patient* patient, EquipeMedicale* equipe) {
        if (!bloc || !patient || !equipe) return;
        
        bloc->commencerOperation(patient, equipe);
        stats->enregistrerDebutOperation(patient);
        
        std::cout << "    → Opération démarrée: " << patient->getNomComplet()
                  << " | " << bloc->getNom() 
                  << " | " << equipe->getNom()
                  << " | Durée: " << patient->getDureeEstimeeMinutes() << "min" << std::endl;
        
        // Planifier la fin de l'opération
        planifierEvenement(Evenement(
            TypeEvenement::FIN_OPERATION,
            tempsSimulation + (patient->getDureeEstimeeMinutes() * 60),
            patient->getId(),
            bloc->getId(),
            equipe->getId()
        ));
    }

    /**
     * Termine une opération
     */
    void terminerOperation(BlocOperatoire* bloc) {
        if (!bloc) return;
        
        Patient* patient = bloc->getPatientActuel();
        bloc->terminerOperation();
        
        if (patient) {
            stats->enregistrerFinOperation(patient);
            std::cout << "    → Opération terminée: " << patient->getNomComplet() 
                      << " | Durée réelle: " << patient->getDureeReelleMinutes() << "min" << std::endl;
        }
    }

    /**
     * Transfère un patient en salle de réveil
     */
    void transfererEnSalleReveil(Patient* patient) {
        if (!patient || !salleReveil) return;
        
        if (salleReveil->ajouterPatient(patient)) {
            std::cout << "    → " << patient->getNomComplet() << " transféré en salle de réveil" << std::endl;
            
            // Planifier la sortie
            planifierEvenement(Evenement(
                TypeEvenement::SORTIE_SALLE_REVEIL,
                tempsSimulation + (60 * 60),  // 60 minutes de réveil
                patient->getId()
            ));
        }
    }

    /**
     * Libère un patient de la salle de réveil
     */
    void libererPatientReveil(Patient* patient) {
        if (!patient || !salleReveil) return;
        
        salleReveil->retirerPatient(patient->getId());
        stats->enregistrerSortie(patient);
        
        std::cout << "    → " << patient->getNomComplet() << " quitte l'hôpital" << std::endl;
    }

    /**
     * Planifie la prochaine arrivée d'urgence
     */
    void planifierProchaineArriveeUrgence() {
        int delaiMinutes = generateur->calculerProchainDelaiArriveeMinutes();
        time_t prochainTimestamp = tempsSimulation + (delaiMinutes * 60);
        
        // Ne pas dépasser la durée de simulation
        if (prochainTimestamp > tempsDebutReel + (dureeSimulationMinutes * 60)) {
            return;
        }
        
        // Créer le patient urgence
        Patient* patient = generateur->genererPatientUrgence();
        tousLesPatients[patient->getId()] = patient;
        
        // Planifier son arrivée
        planifierEvenement(Evenement(
            TypeEvenement::ARRIVEE_PATIENT,
            prochainTimestamp,
            patient->getId()
        ));
    }

    /**
     * Planifie un événement
     */
    void planifierEvenement(const Evenement& evt) {
        fileEvenements.push(evt);
    }

    /**
     * Ajoute un événement à l'historique
     */
    void ajouterAHistorique(const Evenement& evt) {
        historiqueEvenements.push_back(evt);
        if (historiqueEvenements.size() > MAX_HISTORIQUE) {
            historiqueEvenements.erase(historiqueEvenements.begin());
        }
    }

    /**
     * Crée une équipe médicale complète
     */
    void creerEquipeMedicale(int equipeId) {
        EquipeMedicale* equipe = new EquipeMedicale(equipeId, "Équipe " + std::to_string(equipeId));
        
        // Créer les membres
        int baseId = equipeId * 10;
        
        MembreMedical* chirurgien = new MembreMedical(
            baseId + 1, 
            "Chirurgien", 
            "Dr", 
            "Chirurgie Générale"
        );
        tousLesMembres[chirurgien->id] = chirurgien;
        equipe->setChirurgien(chirurgien);
        
        MembreMedical* anesthesiste = new MembreMedical(
            baseId + 2, 
            "Anesthésiste", 
            "Dr", 
            "Anesthésie"
        );
        tousLesMembres[anesthesiste->id] = anesthesiste;
        equipe->setAnesthesiste(anesthesiste);
        
        MembreMedical* infirmier = new MembreMedical(
            baseId + 3, 
            "Infirmier", 
            "IDE", 
            "Bloc Opératoire"
        );
        tousLesMembres[infirmier->id] = infirmier;
        equipe->ajouterInfirmier(infirmier);
        
        equipesDisponibles.push_back(equipe);
    }

    /**
     * Trouve un bloc par ID
     */
    BlocOperatoire* trouverBloc(int blocId) {
        for (auto* bloc : blocsOperatoires) {
            if (bloc && bloc->getId() == blocId) {
                return bloc;
            }
        }
        return nullptr;
    }

    /**
     * Calcule le temps écoulé en minutes depuis le début
     */
    int getTempsEcouleMinutes() const {
        if (tempsDebutReel == 0) return 0;
        return static_cast<int>(std::difftime(tempsSimulation, tempsDebutReel) / 60);
    }

public:
    /**
     * Retourne l'état actuel de la simulation (JSON)
     */
    nlohmann::json getEtatActuel() const {
        // Mettre à jour les compteurs en temps réel
        int nbAttente = salleAttente->getNombrePatients();
        int nbOperation = Scheduler::compterBlocsOccupes(blocsOperatoires);
        int nbReveil = salleReveil->getNombrePatients();
        
        return nlohmann::json{
            {"simulationId", id},
            {"nom", nom},
            {"etat", etatSimulationToString(etat)},
            {"algorithme", algorithmeToString(algorithme)},
            {"tempsEcouleMinutes", getTempsEcouleMinutes()},
            {"dureeSimulationMinutes", dureeSimulationMinutes},
            {"progression", (getTempsEcouleMinutes() * 100.0) / dureeSimulationMinutes},
            {"nombrePatientsTotal", stats->getNombrePatientsTotal()},
            {"nombrePatientsTraites", stats->getNombrePatientsTraites()},
            {"nombrePatientsEnAttente", nbAttente},
            {"nombrePatientsEnOperation", nbOperation},
            {"nombrePatientsEnReveil", nbReveil},
            {"nombreBlocsLibres", Scheduler::compterBlocsDisponibles(blocsOperatoires)},
            {"nombreBlocsOccupes", Scheduler::compterBlocsOccupes(blocsOperatoires)},
            {"nombreEquipesDisponibles", Scheduler::compterEquipesDisponibles(equipesDisponibles)}
        };
    }

    /**
     * Retourne les statistiques complètes (JSON)
     */
    nlohmann::json getStatistiques() const {
        auto json = stats->toJson();
        json["simulationId"] = id;
        return json;
    }

    /**
     * Retourne les derniers événements (JSON)
     */
    nlohmann::json getEvenements() const {
        nlohmann::json events = nlohmann::json::array();
        for (const auto& evt : historiqueEvenements) {
            events.push_back(evt.toJson());
        }
        return nlohmann::json{
            {"simulationId", id},
            {"events", events}
        };
    }

    // Getters
    int getId() const { return id; }
    std::string getNom() const { return nom; }
    EtatSimulation getEtat() const { return etat; }
};

} // namespace AutoMed

#endif // SIMULATION_ENGINE_HPP
