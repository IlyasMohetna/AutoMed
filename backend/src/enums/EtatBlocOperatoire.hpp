#ifndef ETAT_BLOC_OPERATOIRE_HPP
#define ETAT_BLOC_OPERATOIRE_HPP

#include <string>

namespace AutoMed {

/**
 * État d'un bloc opératoire
 */
enum class EtatBlocOperatoire {
    LIBRE,          // Disponible pour une opération
    OCCUPE,         // Opération en cours
    NETTOYAGE,      // En cours de nettoyage
    MAINTENANCE     // En maintenance
};

/**
 * Conversion EtatBlocOperatoire vers string
 */
inline std::string etatBlocToString(EtatBlocOperatoire etat) {
    switch (etat) {
        case EtatBlocOperatoire::LIBRE: return "LIBRE";
        case EtatBlocOperatoire::OCCUPE: return "OCCUPE";
        case EtatBlocOperatoire::NETTOYAGE: return "NETTOYAGE";
        case EtatBlocOperatoire::MAINTENANCE: return "MAINTENANCE";
        default: return "INCONNU";
    }
}

} // namespace AutoMed

#endif // ETAT_BLOC_OPERATOIRE_HPP
