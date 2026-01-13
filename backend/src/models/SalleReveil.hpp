#ifndef SALLE_REVEIL_HPP
#define SALLE_REVEIL_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <nlohmann/json.hpp>
#include "Patient.hpp"

namespace AutoMed {

/**
 * Représente un patient en salle de réveil avec son heure d'arrivée
 */
struct PatientReveil {
    Patient* patient;
    time_t horodatageEntree;
    int dureeReveilEstimeeMinutes;

    PatientReveil(Patient* p, int dureeReveil)
        : patient(p),
          horodatageEntree(std::time(nullptr)),
          dureeReveilEstimeeMinutes(dureeReveil) {}

    /**
     * Calcule le temps passé en salle de réveil en minutes
     */
    int getTempsReveilMinutes() const {
        return static_cast<int>(std::difftime(std::time(nullptr), horodatageEntree) / 60);
    }

    /**
     * Vérifie si le patient peut sortir de la salle de réveil
     */
    bool peutSortir() const {
        return getTempsReveilMinutes() >= dureeReveilEstimeeMinutes;
    }

    nlohmann::json toJson() const {
        return nlohmann::json{
            {"patient", patient ? patient->toJson() : nullptr},
            {"horodatageEntree", horodatageEntree},
            {"dureeReveilEstimeeMinutes", dureeReveilEstimeeMinutes},
            {"tempsReveilMinutes", getTempsReveilMinutes()},
            {"peutSortir", peutSortir()}
        };
    }
};

/**
 * Représente une salle de réveil post-opératoire
 */
class SalleReveil {
private:
    int id;
    std::string nom;
    std::vector<PatientReveil> patientsEnReveil;
    int capaciteMax;
    int dureeReveilDefaultMinutes;  // Durée par défaut si non spécifiée

public:
    /**
     * Constructeur
     */
    SalleReveil(int id, const std::string& nom, int capaciteMax = 20, int dureeReveilDefault = 60)
        : id(id),
          nom(nom),
          capaciteMax(capaciteMax),
          dureeReveilDefaultMinutes(dureeReveilDefault) {}

    // Getters
    int getId() const { return id; }
    std::string getNom() const { return nom; }
    int getCapaciteMax() const { return capaciteMax; }
    int getNombrePatients() const { return patientsEnReveil.size(); }
    int getDureeReveilDefaultMinutes() const { return dureeReveilDefaultMinutes; }
    const std::vector<PatientReveil>& getPatientsEnReveil() const { return patientsEnReveil; }

    // Setters
    void setNom(const std::string& newNom) { nom = newNom; }
    void setCapaciteMax(int capacite) { capaciteMax = capacite; }
    void setDureeReveilDefaultMinutes(int duree) { dureeReveilDefaultMinutes = duree; }

    /**
     * Vérifie si la salle est pleine
     */
    bool estPleine() const {
        return patientsEnReveil.size() >= static_cast<size_t>(capaciteMax);
    }

    /**
     * Vérifie si la salle est vide
     */
    bool estVide() const {
        return patientsEnReveil.empty();
    }

    /**
     * Ajoute un patient en salle de réveil
     */
    bool ajouterPatient(Patient* patient, int dureeReveilMinutes = -1) {
        if (!patient || estPleine()) {
            return false;
        }

        // Utiliser la durée par défaut si non spécifiée
        if (dureeReveilMinutes < 0) {
            dureeReveilMinutes = dureeReveilDefaultMinutes;
        }

        patientsEnReveil.emplace_back(patient, dureeReveilMinutes);
        return true;
    }

    /**
     * Retire un patient par ID
     */
    Patient* retirerPatient(int patientId) {
        auto it = std::find_if(patientsEnReveil.begin(), patientsEnReveil.end(),
            [patientId](const PatientReveil& pr) { 
                return pr.patient && pr.patient->getId() == patientId; 
            });

        if (it != patientsEnReveil.end()) {
            Patient* patient = it->patient;
            patientsEnReveil.erase(it);
            return patient;
        }

        return nullptr;
    }

    /**
     * Cherche un patient par ID
     */
    PatientReveil* trouverPatient(int patientId) {
        auto it = std::find_if(patientsEnReveil.begin(), patientsEnReveil.end(),
            [patientId](const PatientReveil& pr) { 
                return pr.patient && pr.patient->getId() == patientId; 
            });

        return (it != patientsEnReveil.end()) ? &(*it) : nullptr;
    }

    /**
     * Récupère tous les patients qui peuvent sortir
     */
    std::vector<Patient*> getPatientsPretsASortir() {
        std::vector<Patient*> patientsPrets;

        for (const auto& pr : patientsEnReveil) {
            if (pr.peutSortir() && pr.patient) {
                patientsPrets.push_back(pr.patient);
            }
        }

        return patientsPrets;
    }

    /**
     * Retire tous les patients qui ont terminé leur réveil
     */
    std::vector<Patient*> libererPatientsPrets() {
        std::vector<Patient*> patientsLiberes;

        auto it = patientsEnReveil.begin();
        while (it != patientsEnReveil.end()) {
            if (it->peutSortir()) {
                patientsLiberes.push_back(it->patient);
                it = patientsEnReveil.erase(it);
            } else {
                ++it;
            }
        }

        return patientsLiberes;
    }

    /**
     * Calcule le temps de réveil moyen en minutes
     */
    double getTempsReveilMoyenMinutes() const {
        if (estVide()) {
            return 0.0;
        }

        int totalTempsReveil = 0;
        for (const auto& pr : patientsEnReveil) {
            totalTempsReveil += pr.getTempsReveilMinutes();
        }

        return static_cast<double>(totalTempsReveil) / patientsEnReveil.size();
    }

    /**
     * Calcule le taux d'occupation (%)
     */
    double getTauxOccupation() const {
        if (capaciteMax == 0) {
            return 0.0;
        }
        return (static_cast<double>(patientsEnReveil.size()) / capaciteMax) * 100.0;
    }

    /**
     * Compte le nombre de patients prêts à sortir
     */
    int compterPatientsPretsASortir() const {
        return std::count_if(patientsEnReveil.begin(), patientsEnReveil.end(),
            [](const PatientReveil& pr) { return pr.peutSortir(); });
    }

    /**
     * Vide complètement la salle de réveil
     */
    void vider() {
        patientsEnReveil.clear();
    }

    /**
     * Conversion vers JSON pour l'API
     */
    nlohmann::json toJson() const {
        nlohmann::json patientsJson = nlohmann::json::array();
        for (const auto& pr : patientsEnReveil) {
            patientsJson.push_back(pr.toJson());
        }

        return nlohmann::json{
            {"id", id},
            {"nom", nom},
            {"capaciteMax", capaciteMax},
            {"nombrePatients", getNombrePatients()},
            {"dureeReveilDefaultMinutes", dureeReveilDefaultMinutes},
            {"estPleine", estPleine()},
            {"estVide", estVide()},
            {"patientsEnReveil", patientsJson},
            {"tempsReveilMoyenMinutes", getTempsReveilMoyenMinutes()},
            {"tauxOccupation", getTauxOccupation()},
            {"nombrePatientsPretsASortir", compterPatientsPretsASortir()}
        };
    }

    /**
     * Conversion vers string pour debug
     */
    std::string toString() const {
        std::ostringstream oss;
        oss << "SalleReveil #" << id << ": " << nom
            << " | Patients: " << getNombrePatients() << "/" << capaciteMax
            << " | Taux occupation: " << getTauxOccupation() << "%"
            << " | Prêts à sortir: " << compterPatientsPretsASortir();
        return oss.str();
    }
};

} // namespace AutoMed

#endif // SALLE_REVEIL_HPP
