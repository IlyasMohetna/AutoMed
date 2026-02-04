#ifndef GENERATEUR_PATIENTS_HPP
#define GENERATEUR_PATIENTS_HPP

#include <random>
#include <vector>
#include <ctime>
#include "../models/Patient.hpp"
#include "../enums/PrioritePatient.hpp"
#include "../enums/TypeOperation.hpp"

namespace AutoMed {

/**
 * Générateur de patients selon distributions statistiques
 */
class GenerateurPatients {
private:
    int prochainId;
    double tauxArriveeHoraireUrgences;    // λ pour loi de Poisson (urgences/heure)
    int nombrePatientsElectifs;            // Nombre d'opérations programmées
    
    // Générateurs aléatoires
    std::mt19937 randomEngine;
    std::uniform_real_distribution<> uniform01;
    std::exponential_distribution<> expDist;

public:
    /**
     * Constructeur
     */
    GenerateurPatients(double tauxUrgences = 2.0, int nbElectifs = 10)
        : prochainId(1),
          tauxArriveeHoraireUrgences(tauxUrgences),
          nombrePatientsElectifs(nbElectifs),
          randomEngine(std::random_device{}()),
          uniform01(0.0, 1.0),
          expDist(tauxUrgences) {}

    /**
     * Génère un patient avec priorité et type d'opération aléatoires
     */
    Patient* genererPatient(PrioritePatient priorite) {
        TypeOperation type = genererTypeOperation();
        int duree = genererDureeOperation(type);
        
        std::string nom = "Patient_" + std::to_string(prochainId);
        std::string prenom = genererPrenom();
        
        Patient* patient = new Patient(prochainId++, nom, prenom, priorite, type, duree);
        
        return patient;
    }

    /**
     * Génère un patient urgence
     */
    Patient* genererPatientUrgence() {
        return genererPatient(PrioritePatient::URGENCE);
    }

    /**
     * Génère un patient électif (programmé)
     */
    Patient* genererPatientElectif() {
        return genererPatient(PrioritePatient::ELECTIVE);
    }

    /**
     * Génère un patient ambulatoire
     */
    Patient* genererPatientAmbulatoire() {
        return genererPatient(PrioritePatient::AMBULATOIRE);
    }

    /**
     * Calcule le prochain délai d'arrivée en minutes (loi exponentielle)
     * Utilisé pour les urgences (processus de Poisson)
     */
    int calculerProchainDelaiArriveeMinutes() {
        // Distribution exponentielle: délai entre 2 arrivées
        // λ = taux par heure → délai moyen = 60/λ minutes
        double delaiHeures = expDist(randomEngine);
        int delaiMinutes = static_cast<int>(delaiHeures * 60.0);
        
        // Minimum 1 minute pour éviter les événements simultanés
        return std::max(1, delaiMinutes);
    }

    /**
     * Génère une liste de patients électifs pour la journée
     * Répartis uniformément sur les heures de travail (8h-18h = 600 minutes)
     */
    std::vector<Patient*> genererPatientsElectifs(time_t dateDebut, int dureeJourneeMinutes = 600) {
        std::vector<Patient*> patients;
        
        if (nombrePatientsElectifs <= 0) {
            return patients;
        }
        
        // Intervalle entre chaque patient électif
        int intervalle = dureeJourneeMinutes / nombrePatientsElectifs;
        
        for (int i = 0; i < nombrePatientsElectifs; i++) {
            Patient* patient = genererPatientElectif();
            
            // Ajuster l'horodatage d'arrivée avec le temps virtuel
            time_t heureArrivee = dateDebut + (i * intervalle * 60);
            patient->setHorodatageArrivee(heureArrivee);
            
            patients.push_back(patient);
        }
        
        return patients;
    }

    /**
     * Génère un type d'opération aléatoire
     */
    TypeOperation genererTypeOperation() {
        int rand = static_cast<int>(uniform01(randomEngine) * 10);
        
        switch (rand) {
            case 0: return TypeOperation::CARDIAQUE;
            case 1: return TypeOperation::ORTHOPEDIQUE;
            case 2: return TypeOperation::NEUROCHIRURGIE;
            case 3: return TypeOperation::DIGESTIVE;
            case 4: return TypeOperation::THORACIQUE;
            case 5: return TypeOperation::VASCULAIRE;
            case 6: return TypeOperation::UROLOGIQUE;
            case 7: return TypeOperation::ORL;
            case 8: return TypeOperation::OPHTALMOLOGIQUE;
            case 9: return TypeOperation::GYNECOLOGIQUE;
            default: return TypeOperation::CARDIAQUE;
        }
    }

    /**
     * Génère une durée d'opération réaliste selon le type
     */
    int genererDureeOperation(TypeOperation type) {
        int dureeBase = 0;
        int variabilite = 0;
        
        switch (type) {
            case TypeOperation::CARDIAQUE:
                dureeBase = 240;      // 4h en moyenne
                variabilite = 60;
                break;
            case TypeOperation::NEUROCHIRURGIE:
                dureeBase = 300;      // 5h en moyenne
                variabilite = 90;
                break;
            case TypeOperation::ORTHOPEDIQUE:
                dureeBase = 120;      // 2h en moyenne
                variabilite = 30;
                break;
            case TypeOperation::DIGESTIVE:
                dureeBase = 180;      // 3h en moyenne
                variabilite = 45;
                break;
            case TypeOperation::THORACIQUE:
                dureeBase = 210;      // 3h30 en moyenne
                variabilite = 60;
                break;
            case TypeOperation::VASCULAIRE:
                dureeBase = 150;      // 2h30 en moyenne
                variabilite = 40;
                break;
            case TypeOperation::UROLOGIQUE:
                dureeBase = 90;       // 1h30 en moyenne
                variabilite = 20;
                break;
            case TypeOperation::ORL:
                dureeBase = 60;       // 1h en moyenne
                variabilite = 15;
                break;
            case TypeOperation::OPHTALMOLOGIQUE:
                dureeBase = 45;       // 45min en moyenne
                variabilite = 10;
                break;
            case TypeOperation::GYNECOLOGIQUE:
                dureeBase = 120;      // 2h en moyenne
                variabilite = 30;
                break;
            default:
                dureeBase = 120;
                variabilite = 30;
        }
        
        // Ajouter une variabilité aléatoire
        int variation = static_cast<int>((uniform01(randomEngine) - 0.5) * 2 * variabilite);
        int duree = dureeBase + variation;
        
        // Minimum 15 minutes
        return std::max(15, duree);
    }

    /**
     * Génère un prénom aléatoire
     */
    std::string genererPrenom() {
        static const std::vector<std::string> prenoms = {
            "Jean", "Marie", "Pierre", "Sophie", "Luc", "Anne",
            "Marc", "Julie", "Paul", "Claire", "Jacques", "Nathalie",
            "François", "Isabelle", "Michel", "Catherine", "Philippe", "Sylvie"
        };
        
        int index = static_cast<int>(uniform01(randomEngine) * prenoms.size());
        return prenoms[index % prenoms.size()];
    }

    // Getters
    double getTauxArriveeHoraireUrgences() const { return tauxArriveeHoraireUrgences; }
    int getNombrePatientsElectifs() const { return nombrePatientsElectifs; }
    int getProchainId() const { return prochainId; }

    // Setters
    void setTauxArriveeHoraireUrgences(double taux) { 
        tauxArriveeHoraireUrgences = taux;
        expDist = std::exponential_distribution<>(taux);
    }
    void setNombrePatientsElectifs(int nombre) { nombrePatientsElectifs = nombre; }
};

} // namespace AutoMed

#endif // GENERATEUR_PATIENTS_HPP
