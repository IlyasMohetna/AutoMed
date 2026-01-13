#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <nlohmann/json.hpp>
#include "../models/Patient.hpp"
#include "../enums/PrioritePatient.hpp"

namespace AutoMed {

/**
 * Collecteur de statistiques pour la simulation
 */
class Statistics {
private:
    // Compteurs généraux
    int nombrePatientsTotal;
    int nombrePatientsTraites;
    int nombrePatientsEnAttente;
    int nombrePatientsEnOperation;
    int nombrePatientsEnReveil;
    
    // Temps par patient (en minutes)
    std::vector<int> tempsAttenteParPatient;
    std::vector<int> dureeOperationParPatient;
    std::vector<int> tempsReveilParPatient;
    std::vector<int> tempsSejourTotalParPatient;
    
    // Par priorité
    std::map<PrioritePatient, int> nombrePatientParPriorite;
    std::map<PrioritePatient, std::vector<int>> tempsAttenteParPriorite;
    
    // Horodatages
    time_t tempsDebutSimulation;
    time_t tempsFinSimulation;

public:
    /**
     * Constructeur
     */
    Statistics()
        : nombrePatientsTotal(0),
          nombrePatientsTraites(0),
          nombrePatientsEnAttente(0),
          nombrePatientsEnOperation(0),
          nombrePatientsEnReveil(0),
          tempsDebutSimulation(0),
          tempsFinSimulation(0) {
        
        // Initialiser les compteurs par priorité
        nombrePatientParPriorite[PrioritePatient::URGENCE] = 0;
        nombrePatientParPriorite[PrioritePatient::ELECTIVE] = 0;
        nombrePatientParPriorite[PrioritePatient::AMBULATOIRE] = 0;
    }

    /**
     * Marque le début de la simulation
     */
    void demarrer(time_t temps) {
        tempsDebutSimulation = temps;
    }

    /**
     * Marque la fin de la simulation
     */
    void terminer(time_t temps) {
        tempsFinSimulation = temps;
    }

    /**
     * Enregistre l'arrivée d'un patient
     */
    void enregistrerArrivee(Patient* patient) {
        if (!patient) return;
        
        nombrePatientsTotal++;
        nombrePatientsEnAttente++;
        nombrePatientParPriorite[patient->getPriorite()]++;
    }

    /**
     * Enregistre le début d'une opération
     */
    void enregistrerDebutOperation(Patient* patient) {
        if (!patient) return;
        
        nombrePatientsEnAttente--;
        nombrePatientsEnOperation++;
        
        // Enregistrer le temps d'attente
        int tempsAttente = patient->getTempsAttenteMinutes();
        tempsAttenteParPatient.push_back(tempsAttente);
        tempsAttenteParPriorite[patient->getPriorite()].push_back(tempsAttente);
    }

    /**
     * Enregistre la fin d'une opération
     */
    void enregistrerFinOperation(Patient* patient) {
        if (!patient) return;
        
        nombrePatientsEnOperation--;
        nombrePatientsEnReveil++;
        
        // Enregistrer la durée d'opération
        int dureeOperation = patient->getDureeReelleMinutes();
        dureeOperationParPatient.push_back(dureeOperation);
    }

    /**
     * Enregistre la sortie d'un patient (fin de réveil)
     */
    void enregistrerSortie(Patient* patient) {
        if (!patient) return;
        
        nombrePatientsEnReveil--;
        nombrePatientsTraites++;
        
        // Calculer le temps de séjour total
        int tempsTotal = static_cast<int>(
            std::difftime(std::time(nullptr), patient->getHorodatageArrivee()) / 60
        );
        tempsSejourTotalParPatient.push_back(tempsTotal);
    }

    /**
     * Met à jour les compteurs en temps réel
     */
    void mettreAJourCompteurs(int attente, int operation, int reveil) {
        nombrePatientsEnAttente = attente;
        nombrePatientsEnOperation = operation;
        nombrePatientsEnReveil = reveil;
    }

    /**
     * Calcule le temps d'attente moyen (en minutes)
     */
    double getTempsAttenteMoyen() const {
        if (tempsAttenteParPatient.empty()) return 0.0;
        
        int total = std::accumulate(tempsAttenteParPatient.begin(), 
                                    tempsAttenteParPatient.end(), 0);
        return static_cast<double>(total) / tempsAttenteParPatient.size();
    }

    /**
     * Calcule le temps d'attente maximum (en minutes)
     */
    int getTempsAttenteMax() const {
        if (tempsAttenteParPatient.empty()) return 0;
        return *std::max_element(tempsAttenteParPatient.begin(), 
                                 tempsAttenteParPatient.end());
    }

    /**
     * Calcule le temps d'attente moyen par priorité
     */
    double getTempsAttenteMoyenPriorite(PrioritePatient priorite) const {
        auto it = tempsAttenteParPriorite.find(priorite);
        if (it == tempsAttenteParPriorite.end() || it->second.empty()) {
            return 0.0;
        }
        
        int total = std::accumulate(it->second.begin(), it->second.end(), 0);
        return static_cast<double>(total) / it->second.size();
    }

    /**
     * Calcule la durée moyenne des opérations
     */
    double getDureeOperationMoyenne() const {
        if (dureeOperationParPatient.empty()) return 0.0;
        
        int total = std::accumulate(dureeOperationParPatient.begin(),
                                    dureeOperationParPatient.end(), 0);
        return static_cast<double>(total) / dureeOperationParPatient.size();
    }

    /**
     * Calcule le temps de séjour moyen total
     */
    double getTempsSejourMoyen() const {
        if (tempsSejourTotalParPatient.empty()) return 0.0;
        
        int total = std::accumulate(tempsSejourTotalParPatient.begin(),
                                    tempsSejourTotalParPatient.end(), 0);
        return static_cast<double>(total) / tempsSejourTotalParPatient.size();
    }

    /**
     * Calcule le débit de patients (patients traités par heure)
     */
    double getDebitPatients() const {
        if (tempsDebutSimulation == 0 || nombrePatientsTraites == 0) {
            return 0.0;
        }
        
        time_t tempsActuel = (tempsFinSimulation > 0) ? tempsFinSimulation : std::time(nullptr);
        double heuresEcoulees = std::difftime(tempsActuel, tempsDebutSimulation) / 3600.0;
        
        if (heuresEcoulees <= 0) return 0.0;
        
        return nombrePatientsTraites / heuresEcoulees;
    }

    /**
     * Calcule le taux d'occupation des blocs (nécessite données externes)
     * Cette méthode sera appelée depuis SimulationEngine
     */
    double calculerTauxOccupationBlocs(int minutesUtilisation, int minutesTotales, int nombreBlocs) const {
        if (minutesTotales == 0 || nombreBlocs == 0) return 0.0;
        
        int capaciteTotale = minutesTotales * nombreBlocs;
        return (static_cast<double>(minutesUtilisation) / capaciteTotale) * 100.0;
    }

    // Getters
    int getNombrePatientsTotal() const { return nombrePatientsTotal; }
    int getNombrePatientsTraites() const { return nombrePatientsTraites; }
    int getNombrePatientsEnAttente() const { return nombrePatientsEnAttente; }
    int getNombrePatientsEnOperation() const { return nombrePatientsEnOperation; }
    int getNombrePatientsEnReveil() const { return nombrePatientsEnReveil; }
    
    int getNombrePatientsPriorite(PrioritePatient priorite) const {
        auto it = nombrePatientParPriorite.find(priorite);
        return (it != nombrePatientParPriorite.end()) ? it->second : 0;
    }

    /**
     * Conversion vers JSON
     */
    nlohmann::json toJson() const {
        return nlohmann::json{
            {"nombrePatientsTotal", nombrePatientsTotal},
            {"nombrePatientsTraites", nombrePatientsTraites},
            {"nombrePatientsEnAttente", nombrePatientsEnAttente},
            {"nombrePatientsEnOperation", nombrePatientsEnOperation},
            {"nombrePatientsEnReveil", nombrePatientsEnReveil},
            {"tempsAttenteMoyen", getTempsAttenteMoyen()},
            {"tempsAttenteMax", getTempsAttenteMax()},
            {"dureeOperationMoyenne", getDureeOperationMoyenne()},
            {"tempsSejourMoyen", getTempsSejourMoyen()},
            {"debitPatients", getDebitPatients()},
            {"parPriorite", {
                {"URGENCE", {
                    {"nombre", getNombrePatientsPriorite(PrioritePatient::URGENCE)},
                    {"tempsAttenteMoyen", getTempsAttenteMoyenPriorite(PrioritePatient::URGENCE)}
                }},
                {"ELECTIVE", {
                    {"nombre", getNombrePatientsPriorite(PrioritePatient::ELECTIVE)},
                    {"tempsAttenteMoyen", getTempsAttenteMoyenPriorite(PrioritePatient::ELECTIVE)}
                }},
                {"AMBULATOIRE", {
                    {"nombre", getNombrePatientsPriorite(PrioritePatient::AMBULATOIRE)},
                    {"tempsAttenteMoyen", getTempsAttenteMoyenPriorite(PrioritePatient::AMBULATOIRE)}
                }}
            }}
        };
    }

    /**
     * Conversion vers string pour affichage console
     */
    std::string toString() const {
        std::ostringstream oss;
        oss << "\n=== STATISTIQUES SIMULATION ===\n";
        oss << "Patients total: " << nombrePatientsTotal << "\n";
        oss << "Patients traités: " << nombrePatientsTraites << "\n";
        oss << "En attente: " << nombrePatientsEnAttente << "\n";
        oss << "En opération: " << nombrePatientsEnOperation << "\n";
        oss << "En réveil: " << nombrePatientsEnReveil << "\n";
        oss << "Temps attente moyen: " << getTempsAttenteMoyen() << " min\n";
        oss << "Temps attente max: " << getTempsAttenteMax() << " min\n";
        oss << "Durée opération moyenne: " << getDureeOperationMoyenne() << " min\n";
        oss << "Débit: " << getDebitPatients() << " patients/heure\n";
        oss << "==============================\n";
        return oss.str();
    }
};

} // namespace AutoMed

#endif // STATISTICS_HPP
