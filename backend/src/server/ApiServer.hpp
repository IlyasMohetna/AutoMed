#ifndef API_SERVER_HPP
#define API_SERVER_HPP

#include <crow.h>
#include <nlohmann/json.hpp>
#include <string>
#include <map>
#include "../simulation/SimulationManager.hpp"
#include "../simulation/SimulationEngine.hpp"

using json = nlohmann::json;
using namespace AutoMed;

class ApiServer {
private:
    crow::SimpleApp app;
    SimulationManager* simulationManager;
    
    // Fonction helper pour ajouter les headers CORS
    crow::response addCORS(crow::response&& res) {
        res.add_header("Access-Control-Allow-Origin", "http://localhost:3000");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
        return std::move(res);
    }
    
public:
    ApiServer() : simulationManager(new SimulationManager()) {
        setupRoutes();
    }
    
    ~ApiServer() {
        delete simulationManager;
    }
    
    void setupRoutes() {
        // Health check
        CROW_ROUTE(app, "/api/health")
        ([this]() {
            json response = {
                {"status", "ok"},
                {"service", "AutoMed Backend"},
                {"timestamp", std::time(nullptr)}
            };
            return addCORS(crow::response(200, response.dump()));
        });
        
        // Info du serveur
        CROW_ROUTE(app, "/api/info")
        ([this]() {
            json response = {
                {"name", "AutoMed - Simulateur de Blocs Opératoires"},
                {"version", "1.0.0"},
                {"type", "Backend C++ REST API"}
            };
            return addCORS(crow::response(200, response.dump()));
        });
        
        // Test echo
        CROW_ROUTE(app, "/api/echo")
            .methods("POST"_method)
        ([this](const crow::request& req) {
            try {
                auto body = json::parse(req.body);
                json response = {
                    {"received", body},
                    {"timestamp", std::time(nullptr)}
                };
                return addCORS(crow::response(200, response.dump()));
            } catch (const std::exception& e) {
                json error = {
                    {"error", "Invalid JSON"},
                    {"message", e.what()}
                };
                return addCORS(crow::response(400, error.dump()));
            }
        });
        
        // ===== ROUTES SIMULATION =====
        
        // POST /api/simulation/create - Créer une nouvelle simulation
        CROW_ROUTE(app, "/api/simulation/create")
            .methods("POST"_method)
        ([this](const crow::request& req) {
            try {
                auto body = json::parse(req.body);
                
                ConfigSimulation config;
                config.nom = body.value("nom", "Simulation");
                config.dureeSimulationMinutes = body.value("dureeSimulationMinutes", 480);
                config.algorithme = stringToAlgorithme(body.value("algorithme", "FCFS"));
                config.nombreBlocs = body.value("nombreBlocs", 3);
                config.nombreEquipes = body.value("nombreEquipes", 3);
                config.capaciteSalleAttente = body.value("capaciteSalleAttente", 50);
                config.capaciteSalleReveil = body.value("capaciteSalleReveil", 20);
                config.tauxArriveeHoraireUrgences = body.value("tauxArriveeHoraireUrgences", 2.0);
                config.nombrePatientsElectifs = body.value("nombrePatientsElectifs", 10);
                
                int simId = simulationManager->creerSimulation(config);
                
                json response = {
                    {"success", true},
                    {"simulationId", simId},
                    {"message", "Simulation créée avec succès"}
                };
                return addCORS(crow::response(200, response.dump()));
            } catch (const std::exception& e) {
                json error = {
                    {"success", false},
                    {"error", "Erreur lors de la création"},
                    {"message", e.what()}
                };
                return addCORS(crow::response(400, error.dump()));
            }
        });
        
        // POST /api/simulation/<id>/start - Démarrer une simulation
        CROW_ROUTE(app, "/api/simulation/<int>/start")
            .methods("POST"_method)
        ([this](int simId) {
            if (simulationManager->demarrerSimulation(simId)) {
                json response = {
                    {"success", true},
                    {"message", "Simulation démarrée"},
                    {"simulationId", simId}
                };
                return addCORS(crow::response(200, response.dump()));
            } else {
                json error = {
                    {"success", false},
                    {"error", "Impossible de démarrer la simulation"}
                };
                return addCORS(crow::response(404, error.dump()));
            }
        });
        
        // POST /api/simulation/<id>/pause - Mettre en pause
        CROW_ROUTE(app, "/api/simulation/<int>/pause")
            .methods("POST"_method)
        ([this](int simId) {
            if (simulationManager->pauserSimulation(simId)) {
                json response = {
                    {"success", true},
                    {"message", "Simulation mise en pause"}
                };
                return addCORS(crow::response(200, response.dump()));
            } else {
                json error = {
                    {"success", false},
                    {"error", "Simulation non trouvée"}
                };
                return addCORS(crow::response(404, error.dump()));
            }
        });
        
        // POST /api/simulation/<id>/resume - Reprendre
        CROW_ROUTE(app, "/api/simulation/<int>/resume")
            .methods("POST"_method)
        ([this](int simId) {
            if (simulationManager->reprendreSimulation(simId)) {
                json response = {
                    {"success", true},
                    {"message", "Simulation reprise"}
                };
                return addCORS(crow::response(200, response.dump()));
            } else {
                json error = {
                    {"success", false},
                    {"error", "Simulation non trouvée"}
                };
                return addCORS(crow::response(404, error.dump()));
            }
        });
        
        // POST /api/simulation/<id>/stop - Arrêter
        CROW_ROUTE(app, "/api/simulation/<int>/stop")
            .methods("POST"_method)
        ([this](int simId) {
            if (simulationManager->arreterSimulation(simId)) {
                json response = {
                    {"success", true},
                    {"message", "Simulation arrêtée"}
                };
                return addCORS(crow::response(200, response.dump()));
            } else {
                json error = {
                    {"success", false},
                    {"error", "Simulation non trouvée"}
                };
                return addCORS(crow::response(404, error.dump()));
            }
        });
        
        // GET /api/simulation/<id>/status - État de la simulation
        CROW_ROUTE(app, "/api/simulation/<int>/status")
        ([this](int simId) {
            SimulationEngine* sim = simulationManager->getSimulation(simId);
            if (!sim) {
                json error = {
                    {"success", false},
                    {"error", "Simulation non trouvée"}
                };
                return addCORS(crow::response(404, error.dump()));
            }
            
            json response = sim->getEtatActuel();
            return addCORS(crow::response(200, response.dump()));
        });
        
        // GET /api/simulation/<id>/stats - Statistiques
        CROW_ROUTE(app, "/api/simulation/<int>/stats")
        ([this](int simId) {
            SimulationEngine* sim = simulationManager->getSimulation(simId);
            if (!sim) {
                json error = {
                    {"success", false},
                    {"error", "Simulation non trouvée"}
                };
                return addCORS(crow::response(404, error.dump()));
            }
            
            json response = sim->getStatistiques();
            return addCORS(crow::response(200, response.dump()));
        });
        
        // GET /api/simulation/<id>/events - Derniers événements
        CROW_ROUTE(app, "/api/simulation/<int>/events")
        ([this](int simId) {
            SimulationEngine* sim = simulationManager->getSimulation(simId);
            if (!sim) {
                json error = {
                    {"success", false},
                    {"error", "Simulation non trouvée"}
                };
                return addCORS(crow::response(404, error.dump()));
            }
            
            json response = sim->getEvenements();
            return addCORS(crow::response(200, response.dump()));
        });
        
        // GET /api/simulations - Lister toutes les simulations
        CROW_ROUTE(app, "/api/simulations")
        ([this]() {
            auto ids = simulationManager->listerSimulations();
            json response = {
                {"simulations", ids},
                {"count", ids.size()}
            };
            return addCORS(crow::response(200, response.dump()));
        });
        
        // DELETE /api/simulation/<id> - Supprimer une simulation
        CROW_ROUTE(app, "/api/simulation/<int>")
            .methods("DELETE"_method)
        ([this](int simId) {
            if (simulationManager->supprimerSimulation(simId)) {
                json response = {
                    {"success", true},
                    {"message", "Simulation supprimée"}
                };
                return addCORS(crow::response(200, response.dump()));
            } else {
                json error = {
                    {"success", false},
                    {"error", "Simulation non trouvée"}
                };
                return addCORS(crow::response(404, error.dump()));
            }
        });
    }
    
    void run(uint16_t port) {
        std::cout << "========================================" << std::endl;
        std::cout << "  AutoMed - Serveur REST API" << std::endl;
        std::cout << "  Port: " << port << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Endpoints disponibles:" << std::endl;
        std::cout << "  Généraux:" << std::endl;
        std::cout << "    GET  /api/health" << std::endl;
        std::cout << "    GET  /api/info" << std::endl;
        std::cout << "    POST /api/echo" << std::endl;
        std::cout << "  Simulation:" << std::endl;
        std::cout << "    POST   /api/simulation/create" << std::endl;
        std::cout << "    POST   /api/simulation/<id>/start" << std::endl;
        std::cout << "    POST   /api/simulation/<id>/pause" << std::endl;
        std::cout << "    POST   /api/simulation/<id>/resume" << std::endl;
        std::cout << "    POST   /api/simulation/<id>/stop" << std::endl;
        std::cout << "    GET    /api/simulation/<id>/status" << std::endl;
        std::cout << "    GET    /api/simulation/<id>/stats" << std::endl;
        std::cout << "    GET    /api/simulation/<id>/events" << std::endl;
        std::cout << "    GET    /api/simulations" << std::endl;
        std::cout << "    DELETE /api/simulation/<id>" << std::endl;
        std::cout << "========================================" << std::endl;
        
        app.port(port)
            .multithreaded()
            .run();
    }
};

#endif // API_SERVER_HPP
