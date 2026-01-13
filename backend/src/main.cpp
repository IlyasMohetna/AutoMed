#include <iostream>
#include "server/ApiServer.hpp"

int main() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║   AutoMed - Simulateur de Blocs     ║" << std::endl;
    std::cout << "║      Backend C++ REST API            ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;

    ApiServer server;
    server.run(8080);

    return 0;
}
