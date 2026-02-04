/**
 * Programme de benchmark pour comparer les algorithmes d'ordonnancement
 * AutoMed - Simulateur de Blocs Opératoires
 */

#include <iostream>
#include <string>
#include "simulation/SimulationEngine.hpp"
#include "benchmark/AlgorithmComparison.hpp"

using namespace AutoMed;

void afficherBanniere() {
    std::cout << R"(
╔══════════════════════════════════════════════════════════════════════════╗
║                                                                          ║
║     █████╗ ██╗   ██╗████████╗ ██████╗ ███╗   ███╗███████╗██████╗       ║
║    ██╔══██╗██║   ██║╚══██╔══╝██╔═══██╗████╗ ████║██╔════╝██╔══██╗      ║
║    ███████║██║   ██║   ██║   ██║   ██║██╔████╔██║█████╗  ██║  ██║      ║
║    ██╔══██║██║   ██║   ██║   ██║   ██║██║╚██╔╝██║██╔══╝  ██║  ██║      ║
║    ██║  ██║╚██████╔╝   ██║   ╚██████╔╝██║ ╚═╝ ██║███████╗██████╔╝      ║
║    ╚═╝  ╚═╝ ╚═════╝    ╚═╝    ╚═════╝ ╚═╝     ╚═╝╚══════╝╚═════╝       ║
║                                                                          ║
║              BENCHMARK - COMPARAISON DES ALGORITHMES                    ║
║                                                                          ║
╚══════════════════════════════════════════════════════════════════════════╝
)" << std::endl;
}

void afficherMenu() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                      SCÉNARIOS DE TEST DISPONIBLES                       ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n\n";
    std::cout << "  1️⃣   Scénario CHARGE NORMALE\n";
    std::cout << "       • 8 heures de simulation (480 min)\n";
    std::cout << "       • 3 blocs opératoires, 3 équipes\n";
    std::cout << "       • 2 urgences/heure, 10 patients électifs\n";
    std::cout << "       → Représente une journée standard\n\n";
    
    std::cout << "  2️⃣   Scénario FORTE CHARGE\n";
    std::cout << "       • 12 heures de simulation (720 min)\n";
    std::cout << "       • 3 blocs opératoires, 3 équipes\n";
    std::cout << "       • 4 urgences/heure, 20 patients électifs\n";
    std::cout << "       → Teste la résilience sous pression\n\n";
    
    std::cout << "  3️⃣   Scénario URGENCES INTENSIVES\n";
    std::cout << "       • 8 heures de simulation (480 min)\n";
    std::cout << "       • 4 blocs opératoires, 4 équipes\n";
    std::cout << "       • 6 urgences/heure, 5 patients électifs\n";
    std::cout << "       → Priorité aux urgences\n\n";
    
    std::cout << "  4️⃣   Scénario RESSOURCES LIMITÉES\n";
    std::cout << "       • 8 heures de simulation (480 min)\n";
    std::cout << "       • 2 blocs opératoires, 2 équipes\n";
    std::cout << "       • 2 urgences/heure, 15 patients électifs\n";
    std::cout << "       → Teste l'efficacité avec peu de ressources\n\n";
    
    std::cout << "  5️⃣   Scénario PERSONNALISÉ\n";
    std::cout << "       → Définissez vos propres paramètres\n\n";
    
    std::cout << "  0️⃣   Quitter\n\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
}

ConfigSimulation obtenirConfigScenario(int choix) {
    ConfigSimulation config;
    config.facteurVitesse = 0.0;  // Mode instantané pour les benchmarks
    
    switch (choix) {
        case 1: // Charge normale
            config.nom = "Scénario Charge Normale";
            config.dureeSimulationMinutes = 480;
            config.nombreBlocs = 3;
            config.nombreEquipes = 3;
            config.capaciteSalleAttente = 50;
            config.capaciteSalleReveil = 20;
            config.tauxArriveeHoraireUrgences = 2.0;
            config.nombrePatientsElectifs = 10;
            break;
            
        case 2: // Forte charge
            config.nom = "Scénario Forte Charge";
            config.dureeSimulationMinutes = 720;
            config.nombreBlocs = 3;
            config.nombreEquipes = 3;
            config.capaciteSalleAttente = 80;
            config.capaciteSalleReveil = 30;
            config.tauxArriveeHoraireUrgences = 4.0;
            config.nombrePatientsElectifs = 20;
            break;
            
        case 3: // Urgences intensives
            config.nom = "Scénario Urgences Intensives";
            config.dureeSimulationMinutes = 480;
            config.nombreBlocs = 4;
            config.nombreEquipes = 4;
            config.capaciteSalleAttente = 60;
            config.capaciteSalleReveil = 25;
            config.tauxArriveeHoraireUrgences = 6.0;
            config.nombrePatientsElectifs = 5;
            break;
            
        case 4: // Ressources limitées
            config.nom = "Scénario Ressources Limitées";
            config.dureeSimulationMinutes = 480;
            config.nombreBlocs = 2;
            config.nombreEquipes = 2;
            config.capaciteSalleAttente = 40;
            config.capaciteSalleReveil = 15;
            config.tauxArriveeHoraireUrgences = 2.0;
            config.nombrePatientsElectifs = 15;
            break;
            
        case 5: // Personnalisé
            std::cout << "\n📝 Configuration personnalisée:\n\n";
            
            std::cout << "Durée simulation (minutes): ";
            std::cin >> config.dureeSimulationMinutes;
            
            std::cout << "Nombre de blocs opératoires: ";
            std::cin >> config.nombreBlocs;
            
            std::cout << "Nombre d'équipes médicales: ";
            std::cin >> config.nombreEquipes;
            
            std::cout << "Capacité salle d'attente: ";
            std::cin >> config.capaciteSalleAttente;
            
            std::cout << "Capacité salle de réveil: ";
            std::cin >> config.capaciteSalleReveil;
            
            std::cout << "Taux d'arrivée urgences (par heure): ";
            std::cin >> config.tauxArriveeHoraireUrgences;
            
            std::cout << "Nombre de patients électifs: ";
            std::cin >> config.nombrePatientsElectifs;
            
            config.nom = "Scénario Personnalisé";
            break;
            
        default:
            config.nom = "Scénario par Défaut";
            config.dureeSimulationMinutes = 480;
            config.nombreBlocs = 3;
            config.nombreEquipes = 3;
            config.capaciteSalleAttente = 50;
            config.capaciteSalleReveil = 20;
            config.tauxArriveeHoraireUrgences = 2.0;
            config.nombrePatientsElectifs = 10;
    }
    
    return config;
}

int main(int argc, char* argv[]) {
    afficherBanniere();
    
    // Mode non-interactif si arguments fournis
    if (argc > 1) {
        int scenario = std::stoi(argv[1]);
        if (scenario >= 1 && scenario <= 5) {
            std::cout << "\n🚀 Exécution du scénario " << scenario << " (mode non-interactif)\n";
            
            ConfigSimulation config = obtenirConfigScenario(scenario);
            AlgorithmComparison comparison;
            
            comparison.comparerAlgorithmes(config);
            comparison.afficherTableauComparatif();
            comparison.genererAnalyse();
            
            // Export des résultats
            comparison.exporterJSON("/app/results/benchmark_results.json");
            comparison.exporterMarkdown("/app/results/benchmark_report.md", config);
            
            std::cout << "\n✅ Benchmark terminé avec succès!\n";
            return 0;
        }
    }
    
    // Mode interactif
    while (true) {
        afficherMenu();
        
        std::cout << "Votre choix: ";
        int choix;
        std::cin >> choix;
        
        if (choix == 0) {
            std::cout << "\n👋 Au revoir!\n\n";
            break;
        }
        
        if (choix < 1 || choix > 5) {
            std::cout << "\n❌ Choix invalide. Veuillez réessayer.\n";
            continue;
        }
        
        ConfigSimulation config = obtenirConfigScenario(choix);
        AlgorithmComparison comparison;
        
        std::cout << "\n⏳ Lancement du benchmark...\n";
        
        comparison.comparerAlgorithmes(config);
        comparison.afficherTableauComparatif();
        comparison.genererAnalyse();
        
        // Export des résultats
        std::string timestamp = std::to_string(std::time(nullptr));
        comparison.exporterJSON("/app/results/benchmark_" + timestamp + ".json");
        comparison.exporterMarkdown("/app/results/benchmark_" + timestamp + ".md", config);
        
        std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        std::cout << "\nAppuyez sur Entrée pour revenir au menu...";
        std::cin.ignore();
        std::cin.get();
    }
    
    return 0;
}
