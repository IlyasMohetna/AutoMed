#ifndef API_SERVER_HPP
#define API_SERVER_HPP

#include <crow.h>
#include <nlohmann/json.hpp>
#include <string>
#include <map>

using json = nlohmann::json;

class ApiServer {
private:
    crow::SimpleApp app;
    
    // Fonction helper pour ajouter les headers CORS
    crow::response addCORS(crow::response&& res) {
        res.add_header("Access-Control-Allow-Origin", "http://localhost:3000");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
        return std::move(res);
    }
    
public:
    ApiServer() {
        setupRoutes();
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
    }
    
    void run(uint16_t port) {
        std::cout << "========================================" << std::endl;
        std::cout << "  Serveur REST API démarré" << std::endl;
        std::cout << "  Port: " << port << std::endl;
        std::cout << "  Endpoints disponibles:" << std::endl;
        std::cout << "    GET  /api/health" << std::endl;
        std::cout << "    GET  /api/info" << std::endl;
        std::cout << "    POST /api/echo" << std::endl;
        std::cout << "========================================" << std::endl;
        
        app.port(port)
            .multithreaded()
            .run();
    }
};

#endif // API_SERVER_HPP
