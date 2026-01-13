#ifndef PRIORITE_PATIENT_HPP
#define PRIORITE_PATIENT_HPP

#include <string>

namespace AutoMed {

/**
 * Priorité d'un patient
 */
enum class PrioritePatient {
    URGENCE = 1,        // Urgence vitale
    ELECTIVE = 2,       // Opération programmée
    AMBULATOIRE = 3     // Chirurgie ambulatoire (patient rentre le jour même)
};

/**
 * Conversion PrioritePatient vers string
 */
inline std::string prioriteToString(PrioritePatient priorite) {
    switch (priorite) {
        case PrioritePatient::URGENCE: return "URGENCE";
        case PrioritePatient::ELECTIVE: return "ELECTIVE";
        case PrioritePatient::AMBULATOIRE: return "AMBULATOIRE";
        default: return "INCONNU";
    }
}

} // namespace AutoMed

#endif // PRIORITE_PATIENT_HPP
