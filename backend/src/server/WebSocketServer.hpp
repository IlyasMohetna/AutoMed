#ifndef WEBSOCKET_SERVER_HPP
#define WEBSOCKET_SERVER_HPP

#include <iostream>
#include <set>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

class WebSocketServer {
private:
    server m_server;
    std::set<connection_hdl, std::owner_less<connection_hdl>> m_connections;

    void on_open(connection_hdl hdl) {
        m_connections.insert(hdl);
        std::cout << "[WS] Nouvelle connexion établie. Total: " << m_connections.size() << std::endl;
    }

    void on_close(connection_hdl hdl) {
        m_connections.erase(hdl);
        std::cout << "[WS] Connexion fermée. Total: " << m_connections.size() << std::endl;
    }

    void on_message(connection_hdl /* hdl */, server::message_ptr msg) {
        std::string payload = msg->get_payload();
        std::cout << "[WS] Message reçu: " << payload << std::endl;
        std::cout << "[WS] Processing messagesss..." << std::endl;

        // Echo du message à tous les clients connectés (broadcast)
        for (auto it : m_connections) {
            try {
                m_server.send(it, payload, msg->get_opcode());
            } catch (const std::exception& e) {
                std::cerr << "[WS] Erreur d'envoi: " << e.what() << std::endl;
            }
        }
    }

public:
    WebSocketServer() {
        // Configuration du serveur
        m_server.set_access_channels(websocketpp::log::alevel::all);
        m_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialisation
        m_server.init_asio();

        // Handlers
        m_server.set_open_handler(bind(&WebSocketServer::on_open, this, ::_1));
        m_server.set_close_handler(bind(&WebSocketServer::on_close, this, ::_1));
        m_server.set_message_handler(bind(&WebSocketServer::on_message, this, ::_1, ::_2));
    }

    void run(uint16_t port) {
        try {
            m_server.listen(port);
            m_server.start_accept();

            std::cout << "========================================" << std::endl;
            std::cout << "  Serveur WebSocket démarré" << std::endl;
            std::cout << "  Port: " << port << std::endl;
            std::cout << "  Prêt à recevoir des connexions..." << std::endl;
            std::cout << "========================================" << std::endl;

            m_server.run();
        } catch (const std::exception& e) {
            std::cerr << "[ERREUR] " << e.what() << std::endl;
        }
    }

    void broadcast(const std::string& message) {
        for (auto it : m_connections) {
            try {
                m_server.send(it, message, websocketpp::frame::opcode::text);
            } catch (const std::exception& e) {
                std::cerr << "[WS] Erreur broadcast: " << e.what() << std::endl;
            }
        }
    }
};

#endif // WEBSOCKET_SERVER_HPP
