#ifndef SALLE_ATTENTE_HPP
#define SALLE_ATTENTE_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <nlohmann/json.hpp>
#include "../enums/PrioritePatient.hpp"
#include "Patient.hpp"

namespace AutoMed {

/**
 * Représente une salle d'attente avec file de patients
 */
class SalleAttente {
private:
    int id;
    std::string nom;
    std::vector<Patient*> fileAttente;
    int capaciteMax;

public:
    /**
     * Constructeur
     */
    SalleAttente(int id, const std::string& nom, int capaciteMax = 100)
        : id(id),
          nom(nom),
          capaciteMax(capaciteMax) {}

    /**
     * Destructeur - Ne supprime pas les pointeurs (gestion externe)
     */
    ~SalleAttente() {
        fileAttente.clear();
    }

    // Getters
    int getId() const { return id; }
    std::string getNom() const { return nom; }
    int getCapaciteMax() const { return capaciteMax; }
    int getNombrePatients() const { return fileAttente.size(); }
    const std::vector<Patient*>& getFileAttente() const { return fileAttente; }

    // Setters
    void setNom(const std::string& newNom) { nom = newNom; }
    void setCapaciteMax(int capacite) { capaciteMax = capacite; }

    /**
     * Vérifie si la salle est pleine
     */
    bool estPleine() const {
        return fileAttente.size() >= static_cast<size_t>(capaciteMax);
    }

    /**
     * Vérifie si la salle est vide
     */
    bool estVide() const {
        return fileAttente.empty();
    }

    /**
     * Ajoute un patient à la file d'attente
     */
    bool ajouterPatient(Patient* patient) {
        if (!patient || estPleine()) {
            return false;
        }

        fileAttente.push_back(patient);
        return true;
    }

    /**
     * Retire un patient de la file d'attente par ID
     */
    Patient* retirerPatient(int patientId) {
        auto it = std::find_if(fileAttente.begin(), fileAttente.end(),
            [patientId](Patient* p) { return p->getId() == patientId; });

        if (it != fileAttente.end()) {
            Patient* patient = *it;
            fileAttente.erase(it);
            return patient;
        }

        return nullptr;
    }

    /**
     * Récupère le prochain patient selon l'algorithme FCFS (First Come First Serve)
     */
    Patient* getProchainPatientFCFS() {
        if (estVide()) {
            return nullptr;
        }

        Patient* patient = fileAttente.front();
        fileAttente.erase(fileAttente.begin());
        return patient;
    }

    /**
     * Récupère le prochain patient selon la priorité (Urgence > Elective > Ambulatoire)
     */
    Patient* getProchainPatientPriorite() {
        if (estVide()) {
            return nullptr;
        }

        // Trouver le patient avec la plus haute priorité (valeur la plus petite)
        auto it = std::min_element(fileAttente.begin(), fileAttente.end(),
            [](Patient* a, Patient* b) {
                return static_cast<int>(a->getPriorite()) < static_cast<int>(b->getPriorite());
            });

        Patient* patient = *it;
        fileAttente.erase(it);
        return patient;
    }

    /**
     * Récupère le prochain patient selon SJF (Shortest Job First)
     */
    Patient* getProchainPatientSJF() {
        if (estVide()) {
            return nullptr;
        }

        // Trouver le patient avec la durée estimée la plus courte
        auto it = std::min_element(fileAttente.begin(), fileAttente.end(),
            [](Patient* a, Patient* b) {
                return a->getDureeEstimeeMinutes() < b->getDureeEstimeeMinutes();
            });

        Patient* patient = *it;
        fileAttente.erase(it);
        return patient;
    }

    /**
     * Cherche un patient par ID
     */
    Patient* trouverPatient(int patientId) const {
        auto it = std::find_if(fileAttente.begin(), fileAttente.end(),
            [patientId](Patient* p) { return p->getId() == patientId; });

        return (it != fileAttente.end()) ? *it : nullptr;
    }

    /**
     * Compte le nombre de patients par priorité
     */
    int compterPatientsPriorite(PrioritePatient priorite) const {
        return std::count_if(fileAttente.begin(), fileAttente.end(),
            [priorite](Patient* p) { return p->getPriorite() == priorite; });
    }

    /**
     * Calcule le temps d'attente moyen en minutes
     */
    double getTempsAttenteMoyenMinutes() const {
        if (estVide()) {
            return 0.0;
        }

        int totalTempsAttente = 0;
        for (const auto& patient : fileAttente) {
            totalTempsAttente += patient->getTempsAttenteMinutes();
        }

        return static_cast<double>(totalTempsAttente) / fileAttente.size();
    }

    /**
     * Calcule le temps d'attente maximum en minutes
     */
    int getTempsAttenteMaxMinutes() const {
        if (estVide()) {
            return 0;
        }

        int maxTemps = 0;
        for (const auto& patient : fileAttente) {
            int temps = patient->getTempsAttenteMinutes();
            if (temps > maxTemps) {
                maxTemps = temps;
            }
        }

        return maxTemps;
    }

    /**
     * Vide complètement la file d'attente
     */
    void vider() {
        fileAttente.clear();
    }

    /**
     * Conversion vers JSON pour l'API
     */
    nlohmann::json toJson() const {
        nlohmann::json patientsJson = nlohmann::json::array();
        for (const auto& patient : fileAttente) {
            if (patient) {
                patientsJson.push_back(patient->toJson());
            }
        }

        return nlohmann::json{
            {"id", id},
            {"nom", nom},
            {"capaciteMax", capaciteMax},
            {"nombrePatients", getNombrePatients()},
            {"estPleine", estPleine()},
            {"estVide", estVide()},
            {"patients", patientsJson},
            {"tempsAttenteMoyenMinutes", getTempsAttenteMoyenMinutes()},
            {"tempsAttenteMaxMinutes", getTempsAttenteMaxMinutes()},
            {"nombreUrgences", compterPatientsPriorite(PrioritePatient::URGENCE)},
            {"nombreElectives", compterPatientsPriorite(PrioritePatient::ELECTIVE)},
            {"nombreAmbulatoires", compterPatientsPriorite(PrioritePatient::AMBULATOIRE)}
        };
    }

    /**
     * Conversion vers string pour debug
     */
    std::string toString() const {
        std::ostringstream oss;
        oss << "SalleAttente #" << id << ": " << nom
            << " | Patients: " << getNombrePatients() << "/" << capaciteMax
            << " | Temps attente moyen: " << getTempsAttenteMoyenMinutes() << "min";
        return oss.str();
    }
};

} // namespace AutoMed

#endif // SALLE_ATTENTE_HPP
