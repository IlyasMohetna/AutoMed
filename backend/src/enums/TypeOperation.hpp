#ifndef TYPE_OPERATION_HPP
#define TYPE_OPERATION_HPP

#include <string>

namespace AutoMed {

/**
 * Types d'opérations chirurgicales
 */
enum class TypeOperation {
    CARDIAQUE,          // Chirurgie cardiaque
    ORTHOPEDIQUE,       // Chirurgie orthopédique
    NEUROCHIRURGIE,     // Neurochirurgie
    DIGESTIVE,          // Chirurgie digestive
    THORACIQUE,         // Chirurgie thoracique
    VASCULAIRE,         // Chirurgie vasculaire
    UROLOGIQUE,         // Urologie
    ORL,                // ORL
    OPHTALMOLOGIQUE,    // Ophtalmologie
    GYNECOLOGIQUE       // Gynécologie
};

/**
 * Conversion TypeOperation vers string
 */
inline std::string typeOperationToString(TypeOperation type) {
    switch (type) {
        case TypeOperation::CARDIAQUE: return "CARDIAQUE";
        case TypeOperation::ORTHOPEDIQUE: return "ORTHOPEDIQUE";
        case TypeOperation::NEUROCHIRURGIE: return "NEUROCHIRURGIE";
        case TypeOperation::DIGESTIVE: return "DIGESTIVE";
        case TypeOperation::THORACIQUE: return "THORACIQUE";
        case TypeOperation::VASCULAIRE: return "VASCULAIRE";
        case TypeOperation::UROLOGIQUE: return "UROLOGIQUE";
        case TypeOperation::ORL: return "ORL";
        case TypeOperation::OPHTALMOLOGIQUE: return "OPHTALMOLOGIQUE";
        case TypeOperation::GYNECOLOGIQUE: return "GYNECOLOGIQUE";
        default: return "INCONNU";
    }
}

} // namespace AutoMed

#endif // TYPE_OPERATION_HPP
