#ifndef ALGORITHME_ORDONNANCEMENT_HPP
#define ALGORITHME_ORDONNANCEMENT_HPP

#include <string>

namespace AutoMed {

/**
 * Algorithmes d'ordonnancement disponibles
 */
enum class AlgorithmeOrdonnancement {
    FCFS,          // First Come First Serve
    PRIORITE,      // Par priorité (Urgence > Elective > Ambulatoire)
    SJF            // Shortest Job First
};

/**
 * Conversion AlgorithmeOrdonnancement vers string
 */
inline std::string algorithmeToString(AlgorithmeOrdonnancement algo) {
    switch (algo) {
        case AlgorithmeOrdonnancement::FCFS: return "FCFS";
        case AlgorithmeOrdonnancement::PRIORITE: return "PRIORITE";
        case AlgorithmeOrdonnancement::SJF: return "SJF";
        default: return "INCONNU";
    }
}

/**
 * Conversion string vers AlgorithmeOrdonnancement
 */
inline AlgorithmeOrdonnancement stringToAlgorithme(const std::string& str) {
    if (str == "FCFS") return AlgorithmeOrdonnancement::FCFS;
    if (str == "PRIORITE") return AlgorithmeOrdonnancement::PRIORITE;
    if (str == "SJF") return AlgorithmeOrdonnancement::SJF;
    return AlgorithmeOrdonnancement::FCFS; // Par défaut
}

} // namespace AutoMed

#endif // ALGORITHME_ORDONNANCEMENT_HPP
