#ifndef SIMULATION_MANAGER_HPP
#define SIMULATION_MANAGER_HPP

#include <map>
#include <vector>
#include <mutex>
#include <thread>
#include <nlohmann/json.hpp>
#include "SimulationEngine.hpp"

namespace AutoMed {

/**
 * Gestionnaire de multiples simulations
 * Thread-safe pour utilisation avec l'API REST
 */
class SimulationManager {
private:
    std::map<int, SimulationEngine*> simulations;
    std::map<int, std::thread*> threads;
    int prochainId;
    mutable std::mutex mutex;

public:
    /**
     * Constructeur
     */
    SimulationManager() : prochainId(1) {}

    /**
     * Destructeur
     */
    ~SimulationManager() {
        std::lock_guard<std::mutex> lock(mutex);
        
        // Arrêter toutes les simulations
        for (auto& pair : simulations) {
            if (pair.second) {
                pair.second->arreter();
                delete pair.second;
            }
        }
        
        // Attendre que tous les threads se terminent
        for (auto& pair : threads) {
            if (pair.second && pair.second->joinable()) {
                pair.second->join();
            }
            delete pair.second;
        }
    }

    /**
     * Crée une nouvelle simulation
     * Retourne l'ID de la simulation créée
     */
    int creerSimulation(const ConfigSimulation& config) {
        std::lock_guard<std::mutex> lock(mutex);
        
        int simId = prochainId++;
        SimulationEngine* sim = new SimulationEngine(simId, config);
        simulations[simId] = sim;
        threads[simId] = nullptr;
        
        std::cout << "[MANAGER] Simulation #" << simId << " créée: " << config.nom << std::endl;
        
        return simId;
    }

    /**
     * Démarre une simulation dans un thread séparé
     */
    bool demarrerSimulation(int simId) {
        std::lock_guard<std::mutex> lock(mutex);
        
        auto it = simulations.find(simId);
        if (it == simulations.end() || !it->second) {
            return false;
        }
        
        SimulationEngine* sim = it->second;
        
        // Vérifier si déjà démarrée
        if (threads[simId] != nullptr && threads[simId]->joinable()) {
            return false;
        }
        
        // Démarrer dans un nouveau thread
        threads[simId] = new std::thread([sim]() {
            sim->demarrer();
        });
        
        std::cout << "[MANAGER] Simulation #" << simId << " démarrée" << std::endl;
        
        return true;
    }

    /**
     * Met en pause une simulation
     */
    bool pauserSimulation(int simId) {
        std::lock_guard<std::mutex> lock(mutex);
        
        auto it = simulations.find(simId);
        if (it == simulations.end() || !it->second) {
            return false;
        }
        
        it->second->pause();
        return true;
    }

    /**
     * Reprend une simulation en pause
     */
    bool reprendreSimulation(int simId) {
        std::lock_guard<std::mutex> lock(mutex);
        
        auto it = simulations.find(simId);
        if (it == simulations.end() || !it->second) {
            return false;
        }
        
        it->second->reprendre();
        return true;
    }

    /**
     * Arrête une simulation
     */
    bool arreterSimulation(int simId) {
        std::lock_guard<std::mutex> lock(mutex);
        
        auto it = simulations.find(simId);
        if (it == simulations.end() || !it->second) {
            return false;
        }
        
        it->second->arreter();
        
        // Attendre que le thread se termine
        if (threads[simId] && threads[simId]->joinable()) {
            threads[simId]->join();
        }
        
        return true;
    }

    /**
     * Récupère une simulation par ID
     */
    SimulationEngine* getSimulation(int simId) {
        std::lock_guard<std::mutex> lock(mutex);
        
        auto it = simulations.find(simId);
        if (it == simulations.end()) {
            return nullptr;
        }
        
        return it->second;
    }

    /**
     * Supprime une simulation
     */
    bool supprimerSimulation(int simId) {
        std::lock_guard<std::mutex> lock(mutex);
        
        auto it = simulations.find(simId);
        if (it == simulations.end()) {
            return false;
        }
        
        // Arrêter la simulation
        if (it->second) {
            it->second->arreter();
            
            // Attendre que le thread se termine
            if (threads[simId] && threads[simId]->joinable()) {
                threads[simId]->join();
            }
            
            delete threads[simId];
            threads.erase(simId);
            
            delete it->second;
        }
        
        simulations.erase(it);
        
        std::cout << "[MANAGER] Simulation #" << simId << " supprimée" << std::endl;
        
        return true;
    }

    /**
     * Liste toutes les simulations
     */
    std::vector<int> listerSimulations() const {
        std::lock_guard<std::mutex> lock(mutex);
        
        std::vector<int> ids;
        for (const auto& pair : simulations) {
            ids.push_back(pair.first);
        }
        
        return ids;
    }

    /**
     * Retourne le nombre de simulations actives
     */
    int getNombreSimulations() const {
        std::lock_guard<std::mutex> lock(mutex);
        return simulations.size();
    }
};

} // namespace AutoMed

#endif // SIMULATION_MANAGER_HPP
