#ifndef PATIENT_HPP
#define PATIENT_HPP

#include <string>
#include <ctime>
#include <sstream>
#include <nlohmann/json.hpp>
#include "../enums/PrioritePatient.hpp"
#include "../enums/TypeOperation.hpp"

namespace AutoMed {

/**
 * Représente un patient dans le système hospitalier
 */
class Patient {
private:
    int id;
    std::string nom;
    std::string prenom;
    PrioritePatient priorite;
    TypeOperation typeOperation;
    int dureeEstimeeMinutes;  // Durée estimée de l'opération en minutes
    time_t horodatageArrivee; // Timestamp d'arrivée dans le système
    time_t horodatageDebutOperation; // Timestamp de début d'opération (0 si pas encore commencé)
    time_t horodatageFinOperation;   // Timestamp de fin d'opération (0 si pas encore terminé)

public:
    /**
     * Constructeur complet
     */
    Patient(int id, 
            const std::string& nom, 
            const std::string& prenom,
            PrioritePatient priorite,
            TypeOperation typeOperation,
            int dureeEstimeeMinutes)
        : id(id), 
          nom(nom), 
          prenom(prenom),
          priorite(priorite),
          typeOperation(typeOperation),
          dureeEstimeeMinutes(dureeEstimeeMinutes),
          horodatageArrivee(std::time(nullptr)),
          horodatageDebutOperation(0),
          horodatageFinOperation(0) {}

    /**
     * Constructeur par défaut
     */
    Patient() 
        : id(0), 
          nom(""), 
          prenom(""),
          priorite(PrioritePatient::ELECTIVE),
          typeOperation(TypeOperation::CARDIAQUE),
          dureeEstimeeMinutes(0),
          horodatageArrivee(std::time(nullptr)),
          horodatageDebutOperation(0),
          horodatageFinOperation(0) {}

    // Getters
    int getId() const { return id; }
    std::string getNom() const { return nom; }
    std::string getPrenom() const { return prenom; }
    std::string getNomComplet() const { return prenom + " " + nom; }
    PrioritePatient getPriorite() const { return priorite; }
    TypeOperation getTypeOperation() const { return typeOperation; }
    int getDureeEstimeeMinutes() const { return dureeEstimeeMinutes; }
    time_t getHorodatageArrivee() const { return horodatageArrivee; }
    time_t getHorodatageDebutOperation() const { return horodatageDebutOperation; }
    time_t getHorodatageFinOperation() const { return horodatageFinOperation; }

    // Setters
    void setId(int newId) { id = newId; }
    void setNom(const std::string& newNom) { nom = newNom; }
    void setPrenom(const std::string& newPrenom) { prenom = newPrenom; }
    void setPriorite(PrioritePatient newPriorite) { priorite = newPriorite; }
    void setTypeOperation(TypeOperation newType) { typeOperation = newType; }
    void setDureeEstimeeMinutes(int duree) { dureeEstimeeMinutes = duree; }
    void setHorodatageDebutOperation(time_t timestamp) { horodatageDebutOperation = timestamp; }
    void setHorodatageFinOperation(time_t timestamp) { horodatageFinOperation = timestamp; }

    /**
     * Marque le début de l'opération
     */
    void commencerOperation() {
        horodatageDebutOperation = std::time(nullptr);
    }

    /**
     * Marque la fin de l'opération
     */
    void terminerOperation() {
        horodatageFinOperation = std::time(nullptr);
    }

    /**
     * Calcule le temps d'attente en minutes
     */
    int getTempsAttenteMinutes() const {
        if (horodatageDebutOperation == 0) {
            return static_cast<int>(std::difftime(std::time(nullptr), horodatageArrivee) / 60);
        }
        return static_cast<int>(std::difftime(horodatageDebutOperation, horodatageArrivee) / 60);
    }

    /**
     * Calcule la durée réelle de l'opération en minutes
     */
    int getDureeReelleMinutes() const {
        if (horodatageDebutOperation == 0 || horodatageFinOperation == 0) {
            return 0;
        }
        return static_cast<int>(std::difftime(horodatageFinOperation, horodatageDebutOperation) / 60);
    }

    /**
     * Vérifie si le patient est en cours d'opération
     */
    bool estEnOperation() const {
        return horodatageDebutOperation > 0 && horodatageFinOperation == 0;
    }

    /**
     * Vérifie si l'opération est terminée
     */
    bool operationTerminee() const {
        return horodatageFinOperation > 0;
    }

    /**
     * Conversion vers JSON pour l'API
     */
    nlohmann::json toJson() const {
        return nlohmann::json{
            {"id", id},
            {"nom", nom},
            {"prenom", prenom},
            {"nomComplet", getNomComplet()},
            {"priorite", prioriteToString(priorite)},
            {"typeOperation", typeOperationToString(typeOperation)},
            {"dureeEstimeeMinutes", dureeEstimeeMinutes},
            {"horodatageArrivee", horodatageArrivee},
            {"horodatageDebutOperation", horodatageDebutOperation},
            {"horodatageFinOperation", horodatageFinOperation},
            {"tempsAttenteMinutes", getTempsAttenteMinutes()},
            {"dureeReelleMinutes", getDureeReelleMinutes()},
            {"estEnOperation", estEnOperation()},
            {"operationTerminee", operationTerminee()}
        };
    }

    /**
     * Conversion vers string pour debug
     */
    std::string toString() const {
        std::ostringstream oss;
        oss << "Patient #" << id << ": " << getNomComplet()
            << " | Priorité: " << prioriteToString(priorite)
            << " | Type: " << typeOperationToString(typeOperation)
            << " | Durée estimée: " << dureeEstimeeMinutes << "min"
            << " | Temps d'attente: " << getTempsAttenteMinutes() << "min";
        return oss.str();
    }
};

} // namespace AutoMed

#endif // PATIENT_HPP
