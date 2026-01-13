#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <vector>
#include <algorithm>
#include "../models/Patient.hpp"
#include "../models/BlocOperatoire.hpp"
#include "../models/EquipeMedicale.hpp"
#include "../models/SalleAttente.hpp"
#include "../enums/AlgorithmeOrdonnancement.hpp"

namespace AutoMed {

/**
 * Gestionnaire d'ordonnancement pour l'assignation des patients aux blocs
 */
class Scheduler {
public:
    /**
     * Sélectionne le prochain patient à opérer selon l'algorithme choisi
     * Retire le patient de la salle d'attente
     */
    static Patient* selectionnerProchainPatient(
        SalleAttente* salle, 
        AlgorithmeOrdonnancement algo
    ) {
        if (!salle || salle->estVide()) {
            return nullptr;
        }

        Patient* patient = nullptr;

        switch (algo) {
            case AlgorithmeOrdonnancement::FCFS:
                patient = salle->getProchainPatientFCFS();
                break;

            case AlgorithmeOrdonnancement::PRIORITE:
                patient = salle->getProchainPatientPriorite();
                break;

            case AlgorithmeOrdonnancement::SJF:
                patient = salle->getProchainPatientSJF();
                break;

            default:
                patient = salle->getProchainPatientFCFS();
                break;
        }

        return patient;
    }

    /**
     * Trouve un bloc opératoire disponible
     * Retourne nullptr si aucun bloc n'est libre
     */
    static BlocOperatoire* trouverBlocDisponible(
        const std::vector<BlocOperatoire*>& blocs
    ) {
        for (auto* bloc : blocs) {
            if (bloc && bloc->estDisponible()) {
                return bloc;
            }
        }
        return nullptr;
    }

    /**
     * Trouve une équipe médicale disponible
     * Pour l'instant, retourne simplement la première équipe disponible
     * Peut être étendu pour vérifier la compatibilité avec le type d'opération
     */
    static EquipeMedicale* trouverEquipeDisponible(
        const std::vector<EquipeMedicale*>& equipes,
        TypeOperation typeOp
    ) {
        // Version simple: première équipe disponible et complète
        for (auto* equipe : equipes) {
            if (equipe && equipe->estDisponible() && equipe->estComplete()) {
                return equipe;
            }
        }
        return nullptr;
    }

    /**
     * Compte le nombre de blocs disponibles
     */
    static int compterBlocsDisponibles(const std::vector<BlocOperatoire*>& blocs) {
        return std::count_if(blocs.begin(), blocs.end(),
            [](BlocOperatoire* b) { return b && b->estDisponible(); });
    }

    /**
     * Compte le nombre de blocs occupés
     */
    static int compterBlocsOccupes(const std::vector<BlocOperatoire*>& blocs) {
        return std::count_if(blocs.begin(), blocs.end(),
            [](BlocOperatoire* b) { return b && b->estOccupe(); });
    }

    /**
     * Compte le nombre d'équipes disponibles
     */
    static int compterEquipesDisponibles(const std::vector<EquipeMedicale*>& equipes) {
        return std::count_if(equipes.begin(), equipes.end(),
            [](EquipeMedicale* e) { return e && e->estDisponible() && e->estComplete(); });
    }

    /**
     * Vérifie si une assignation est possible
     * (au moins 1 bloc libre ET 1 équipe disponible ET 1 patient en attente)
     */
    static bool assignationPossible(
        const std::vector<BlocOperatoire*>& blocs,
        const std::vector<EquipeMedicale*>& equipes,
        SalleAttente* salle
    ) {
        return compterBlocsDisponibles(blocs) > 0 &&
               compterEquipesDisponibles(equipes) > 0 &&
               salle && !salle->estVide();
    }
};

} // namespace AutoMed

#endif // SCHEDULER_HPP
