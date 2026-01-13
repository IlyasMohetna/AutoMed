#ifndef ETAT_SIMULATION_HPP
#define ETAT_SIMULATION_HPP

#include <string>

namespace AutoMed {

/**
 * État d'une simulation
 */
enum class EtatSimulation {
    CREATED,      // Simulation créée mais pas démarrée
    RUNNING,      // Simulation en cours d'exécution
    PAUSED,       // Simulation en pause
    STOPPED,      // Simulation arrêtée manuellement
    FINISHED      // Simulation terminée (durée atteinte)
};

/**
 * Conversion EtatSimulation vers string
 */
inline std::string etatSimulationToString(EtatSimulation etat) {
    switch (etat) {
        case EtatSimulation::CREATED: return "CREATED";
        case EtatSimulation::RUNNING: return "RUNNING";
        case EtatSimulation::PAUSED: return "PAUSED";
        case EtatSimulation::STOPPED: return "STOPPED";
        case EtatSimulation::FINISHED: return "FINISHED";
        default: return "INCONNU";
    }
}

} // namespace AutoMed

#endif // ETAT_SIMULATION_HPP
