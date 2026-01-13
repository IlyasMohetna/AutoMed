#ifndef EVENEMENT_HPP
#define EVENEMENT_HPP

#include <ctime>
#include <string>
#include <nlohmann/json.hpp>

namespace AutoMed {

/**
 * Types d'événements dans la simulation
 */
enum class TypeEvenement {
    ARRIVEE_PATIENT,          // Nouveau patient arrive
    DEBUT_OPERATION,          // Opération commence dans un bloc
    FIN_OPERATION,            // Opération se termine
    FIN_NETTOYAGE_BLOC,       // Nettoyage du bloc terminé
    ENTREE_SALLE_REVEIL,      // Patient entre en salle de réveil
    SORTIE_SALLE_REVEIL,      // Patient quitte l'hôpital
    FIN_SIMULATION            // Fin de la simulation
};

/**
 * Conversion TypeEvenement vers string
 */
inline std::string typeEvenementToString(TypeEvenement type) {
    switch (type) {
        case TypeEvenement::ARRIVEE_PATIENT: return "ARRIVEE_PATIENT";
        case TypeEvenement::DEBUT_OPERATION: return "DEBUT_OPERATION";
        case TypeEvenement::FIN_OPERATION: return "FIN_OPERATION";
        case TypeEvenement::FIN_NETTOYAGE_BLOC: return "FIN_NETTOYAGE_BLOC";
        case TypeEvenement::ENTREE_SALLE_REVEIL: return "ENTREE_SALLE_REVEIL";
        case TypeEvenement::SORTIE_SALLE_REVEIL: return "SORTIE_SALLE_REVEIL";
        case TypeEvenement::FIN_SIMULATION: return "FIN_SIMULATION";
        default: return "INCONNU";
    }
}

/**
 * Représente un événement discret dans la simulation
 */
struct Evenement {
    TypeEvenement type;
    time_t horodatage;              // Quand l'événement se produit (timestamp)
    int patientId;                  // ID du patient concerné (-1 si non applicable)
    int blocOperatoireId;           // ID du bloc concerné (-1 si non applicable)
    int equipeId;                   // ID de l'équipe concernée (-1 si non applicable)
    nlohmann::json metadata;        // Données supplémentaires (optionnel)

    /**
     * Constructeur
     */
    Evenement(TypeEvenement t, time_t ts, int pId = -1, int bId = -1, int eId = -1)
        : type(t), 
          horodatage(ts), 
          patientId(pId), 
          blocOperatoireId(bId), 
          equipeId(eId),
          metadata(nlohmann::json::object()) {}

    /**
     * Opérateur de comparaison pour priority_queue
     * Note: Inversion pour avoir un min-heap (événements les plus anciens en premier)
     */
    bool operator<(const Evenement& other) const {
        return horodatage > other.horodatage;
    }

    /**
     * Opérateur de comparaison pour égalité
     */
    bool operator==(const Evenement& other) const {
        return horodatage == other.horodatage && 
               type == other.type && 
               patientId == other.patientId;
    }

    /**
     * Conversion vers JSON
     */
    nlohmann::json toJson() const {
        return nlohmann::json{
            {"type", typeEvenementToString(type)},
            {"horodatage", horodatage},
            {"patientId", patientId},
            {"blocOperatoireId", blocOperatoireId},
            {"equipeId", equipeId},
            {"metadata", metadata}
        };
    }

    /**
     * Conversion vers string pour affichage console
     */
    std::string toString() const {
        std::string str = "[" + std::to_string(horodatage) + "] " + typeEvenementToString(type);
        if (patientId >= 0) str += " | Patient #" + std::to_string(patientId);
        if (blocOperatoireId >= 0) str += " | Bloc #" + std::to_string(blocOperatoireId);
        if (equipeId >= 0) str += " | Équipe #" + std::to_string(equipeId);
        return str;
    }
};

} // namespace AutoMed

#endif // EVENEMENT_HPP
