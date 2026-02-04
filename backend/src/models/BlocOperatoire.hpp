#ifndef BLOC_OPERATOIRE_HPP
#define BLOC_OPERATOIRE_HPP

#include <string>
#include <ctime>
#include <sstream>
#include <nlohmann/json.hpp>
#include "../enums/EtatBlocOperatoire.hpp"
#include "Patient.hpp"
#include "EquipeMedicale.hpp"

namespace AutoMed {

/**
 * Représente un bloc opératoire
 */
class BlocOperatoire {
private:
    int id;
    std::string nom;
    EtatBlocOperatoire etat;
    Patient* patientActuel;
    EquipeMedicale* equipeAssignee;
    time_t horodatageDebutOperation;
    time_t horodatageFinOperation;
    int dureeNettoyageMinutes;  // Durée nécessaire pour le nettoyage

public:
    /**
     * Constructeur
     */
    BlocOperatoire(int id, const std::string& nom, int dureeNettoyageMinutes = 15)
        : id(id),
          nom(nom),
          etat(EtatBlocOperatoire::LIBRE),
          patientActuel(nullptr),
          equipeAssignee(nullptr),
          horodatageDebutOperation(0),
          horodatageFinOperation(0),
          dureeNettoyageMinutes(dureeNettoyageMinutes) {}

    /**
     * Destructeur - Ne supprime pas les pointeurs (gestion externe)
     */
    ~BlocOperatoire() {
        patientActuel = nullptr;
        equipeAssignee = nullptr;
    }

    // Getters
    int getId() const { return id; }
    std::string getNom() const { return nom; }
    EtatBlocOperatoire getEtat() const { return etat; }
    Patient* getPatientActuel() const { return patientActuel; }
    EquipeMedicale* getEquipeAssignee() const { return equipeAssignee; }
    time_t getHorodatageDebutOperation() const { return horodatageDebutOperation; }
    time_t getHorodatageFinOperation() const { return horodatageFinOperation; }
    int getDureeNettoyageMinutes() const { return dureeNettoyageMinutes; }

    // Setters
    void setNom(const std::string& newNom) { nom = newNom; }
    void setDureeNettoyageMinutes(int duree) { dureeNettoyageMinutes = duree; }

    /**
     * Vérifie si le bloc est disponible pour une opération
     */
    bool estDisponible() const {
        return etat == EtatBlocOperatoire::LIBRE;
    }

    /**
     * Vérifie si une opération est en cours
     */
    bool estOccupe() const {
        return etat == EtatBlocOperatoire::OCCUPE;
    }

    /**
     * Commence une opération avec le temps virtuel
     */
    bool commencerOperation(Patient* patient, EquipeMedicale* equipe, time_t tempsVirtuel = 0) {
        if (!estDisponible() || !patient || !equipe) {
            return false;
        }

        patientActuel = patient;
        equipeAssignee = equipe;
        etat = EtatBlocOperatoire::OCCUPE;
        horodatageDebutOperation = (tempsVirtuel > 0) ? tempsVirtuel : std::time(nullptr);
        horodatageFinOperation = 0;

        // Marquer le patient comme en opération avec le temps virtuel
        patient->commencerOperation(tempsVirtuel);
        
        // Réserver l'équipe
        equipe->reserver();

        return true;
    }

    /**
     * Termine l'opération en cours avec le temps virtuel
     */
    bool terminerOperation(time_t tempsVirtuel = 0) {
        if (!estOccupe() || !patientActuel) {
            return false;
        }

        horodatageFinOperation = (tempsVirtuel > 0) ? tempsVirtuel : std::time(nullptr);
        
        // Marquer le patient comme opération terminée avec le temps virtuel
        patientActuel->terminerOperation(tempsVirtuel);
        
        // On garde les références pour l'historique mais on change l'état
        etat = EtatBlocOperatoire::NETTOYAGE;

        return true;
    }

    /**
     * Termine le nettoyage et libère le bloc
     */
    void terminerNettoyage() {
        // Libérer l'équipe
        if (equipeAssignee) {
            equipeAssignee->liberer();
            equipeAssignee = nullptr;
        }

        patientActuel = nullptr;
        etat = EtatBlocOperatoire::LIBRE;
        horodatageDebutOperation = 0;
        horodatageFinOperation = 0;
    }

    /**
     * Met le bloc en maintenance
     */
    void mettreEnMaintenance() {
        if (estDisponible()) {
            etat = EtatBlocOperatoire::MAINTENANCE;
        }
    }

    /**
     * Sort le bloc de la maintenance
     */
    void sortirDeMaintenance() {
        if (etat == EtatBlocOperatoire::MAINTENANCE) {
            etat = EtatBlocOperatoire::LIBRE;
        }
    }

    /**
     * Calcule la durée de l'opération en cours en minutes
     */
    int getDureeOperationEnCoursMinutes() const {
        if (!estOccupe() || horodatageDebutOperation == 0) {
            return 0;
        }
        return static_cast<int>(std::difftime(std::time(nullptr), horodatageDebutOperation) / 60);
    }

    /**
     * Calcule la durée totale de la dernière opération terminée
     */
    int getDureeDerniereOperationMinutes() const {
        if (horodatageDebutOperation == 0 || horodatageFinOperation == 0) {
            return 0;
        }
        return static_cast<int>(std::difftime(horodatageFinOperation, horodatageDebutOperation) / 60);
    }

    /**
     * Conversion vers JSON pour l'API
     */
    nlohmann::json toJson() const {
        return nlohmann::json{
            {"id", id},
            {"nom", nom},
            {"etat", etatBlocToString(etat)},
            {"patientActuel", patientActuel ? patientActuel->toJson() : nullptr},
            {"equipeAssignee", equipeAssignee ? equipeAssignee->toJson() : nullptr},
            {"horodatageDebutOperation", horodatageDebutOperation},
            {"horodatageFinOperation", horodatageFinOperation},
            {"dureeNettoyageMinutes", dureeNettoyageMinutes},
            {"dureeOperationEnCoursMinutes", getDureeOperationEnCoursMinutes()},
            {"dureeDerniereOperationMinutes", getDureeDerniereOperationMinutes()},
            {"estDisponible", estDisponible()},
            {"estOccupe", estOccupe()}
        };
    }

    /**
     * Conversion vers string pour debug
     */
    std::string toString() const {
        std::ostringstream oss;
        oss << "Bloc #" << id << ": " << nom
            << " | État: " << etatBlocToString(etat);
        
        if (patientActuel) {
            oss << " | Patient: " << patientActuel->getNomComplet();
        }
        
        if (estOccupe()) {
            oss << " | Durée: " << getDureeOperationEnCoursMinutes() << "min";
        }

        return oss.str();
    }
};

} // namespace AutoMed

#endif // BLOC_OPERATOIRE_HPP
