#ifndef ALGORITHM_COMPARISON_HPP
#define ALGORITHM_COMPARISON_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <cmath>
#include <nlohmann/json.hpp>
#include "../simulation/SimulationEngine.hpp"
#include "../enums/AlgorithmeOrdonnancement.hpp"

using json = nlohmann::json;
using namespace AutoMed;

/**
 * Résultats d'une simulation pour analyse comparative
 */
struct ResultatSimulation {
    std::string nomAlgorithme;
    AlgorithmeOrdonnancement algorithme;
    
    // Métriques principales
    int nombrePatientsTotal;
    int nombrePatientsTraites;
    double tempsAttenteMoyen;
    int tempsAttenteMax;
    double dureeOperationMoyenne;
    double debitPatients;
    
    // Métriques par priorité
    double tempsAttenteUrgence;
    double tempsAttenteElective;
    double tempsAttenteAmbulatoire;
    
    int nombreUrgences;
    int nombreElectifs;
    int nombreAmbulatoires;
    
    // Calcul du score global (plus bas = meilleur)
    double calculerScore() const {
        // Pondération: urgences plus importantes que électifs
        double scoreAttente = (tempsAttenteUrgence * 3.0) + 
                             (tempsAttenteElective * 1.5) + 
                             (tempsAttenteAmbulatoire * 1.0);
        
        // Pénalité pour patients non traités
        double tauxTraitement = nombrePatientsTotal > 0 ? 
            static_cast<double>(nombrePatientsTraites) / nombrePatientsTotal : 0.0;
        double penaliteNonTraites = (1.0 - tauxTraitement) * 1000.0;
        
        return scoreAttente + penaliteNonTraites;
    }
};

/**
 * Classe pour comparer les performances des algorithmes d'ordonnancement
 */
class AlgorithmComparison {
private:
    std::vector<ResultatSimulation> resultats;
    ConfigSimulation configBase;
    
public:
    /**
     * Exécute une simulation et collecte les statistiques
     */
    ResultatSimulation executerSimulation(const ConfigSimulation& config) {
        std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        std::cout << "🔬 Test: " << algorithmeToString(config.algorithme) << std::endl;
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        
        SimulationEngine engine(1, config);
        engine.demarrer();
        
        auto stats = engine.getStatistiques();
        
        ResultatSimulation resultat;
        resultat.nomAlgorithme = algorithmeToString(config.algorithme);
        resultat.algorithme = config.algorithme;
        resultat.nombrePatientsTotal = stats["nombrePatientsTotal"];
        resultat.nombrePatientsTraites = stats["nombrePatientsTraites"];
        resultat.tempsAttenteMoyen = stats["tempsAttenteMoyen"];
        resultat.tempsAttenteMax = stats["tempsAttenteMax"];
        resultat.dureeOperationMoyenne = stats["dureeOperationMoyenne"];
        resultat.debitPatients = stats["debitPatients"];
        
        // Stats par priorité
        resultat.tempsAttenteUrgence = stats["parPriorite"]["URGENCE"]["tempsAttenteMoyen"];
        resultat.tempsAttenteElective = stats["parPriorite"]["ELECTIVE"]["tempsAttenteMoyen"];
        resultat.tempsAttenteAmbulatoire = stats["parPriorite"]["AMBULATOIRE"]["tempsAttenteMoyen"];
        
        resultat.nombreUrgences = stats["parPriorite"]["URGENCE"]["nombre"];
        resultat.nombreElectifs = stats["parPriorite"]["ELECTIVE"]["nombre"];
        resultat.nombreAmbulatoires = stats["parPriorite"]["AMBULATOIRE"]["nombre"];
        
        return resultat;
    }
    
    /**
     * Compare tous les algorithmes d'ordonnancement
     */
    void comparerAlgorithmes(const ConfigSimulation& configBase) {
        // Sauvegarder la config pour l'export
        this->configBase = configBase;
        
        std::vector<AlgorithmeOrdonnancement> algorithmes = {
            AlgorithmeOrdonnancement::FCFS,
            AlgorithmeOrdonnancement::SJF,
            AlgorithmeOrdonnancement::PRIORITE
        };
        
        std::cout << "\n";
        std::cout << "╔═══════════════════════════════════════════════════════════════╗\n";
        std::cout << "║     BENCHMARK - COMPARAISON DES ALGORITHMES D'ORDONNANCEMENT  ║\n";
        std::cout << "╚═══════════════════════════════════════════════════════════════╝\n";
        std::cout << "\n📊 Configuration de test:\n";
        std::cout << "   • Durée: " << configBase.dureeSimulationMinutes << " minutes\n";
        std::cout << "   • Blocs opératoires: " << configBase.nombreBlocs << "\n";
        std::cout << "   • Équipes médicales: " << configBase.nombreEquipes << "\n";
        std::cout << "   • Taux urgences: " << configBase.tauxArriveeHoraireUrgences << " patients/h\n";
        std::cout << "   • Patients électifs: " << configBase.nombrePatientsElectifs << "\n";
        std::cout << "\n";
        
        for (auto algo : algorithmes) {
            ConfigSimulation config = configBase;
            config.algorithme = algo;
            config.nom = "Test " + algorithmeToString(algo);
            
            ResultatSimulation resultat = executerSimulation(config);
            resultats.push_back(resultat);
            
            // Petit délai entre les simulations pour la lisibilité
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
    
    /**
     * Affiche un tableau comparatif dans la console
     */
    void afficherTableauComparatif() {
        std::cout << "\n\n";
        std::cout << "╔══════════════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                    📊 RÉSULTATS COMPARATIFS                              ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n\n";
        
        // En-tête du tableau
        std::cout << std::left;
        std::cout << "┌─────────────┬──────────┬──────────┬──────────┬──────────┬──────────┐\n";
        std::cout << "│ " << std::setw(11) << "Algorithme"
                  << " │ " << std::setw(8) << "Patients"
                  << " │ " << std::setw(8) << "Traités"
                  << " │ " << std::setw(8) << "Attente"
                  << " │ " << std::setw(8) << "Max"
                  << " │ " << std::setw(8) << "Débit" << " │\n";
        std::cout << "│             │  Total   │    (%)   │   moy.   │  (min)   │  (pat/h) │\n";
        std::cout << "├─────────────┼──────────┼──────────┼──────────┼──────────┼──────────┤\n";
        
        for (const auto& r : resultats) {
            double tauxTraitement = r.nombrePatientsTotal > 0 ? 
                (static_cast<double>(r.nombrePatientsTraites) / r.nombrePatientsTotal * 100.0) : 0.0;
            
            std::cout << "│ " << std::setw(11) << r.nomAlgorithme
                      << " │ " << std::setw(8) << r.nombrePatientsTotal
                      << " │ " << std::setw(7) << std::fixed << std::setprecision(1) << tauxTraitement << "%"
                      << " │ " << std::setw(7) << std::fixed << std::setprecision(1) << r.tempsAttenteMoyen << "m"
                      << " │ " << std::setw(7) << r.tempsAttenteMax << "m"
                      << " │ " << std::setw(7) << std::fixed << std::setprecision(2) << r.debitPatients << " │\n";
        }
        
        std::cout << "└─────────────┴──────────┴──────────┴──────────┴──────────┴──────────┘\n\n";
        
        // Tableau détaillé par priorité
        std::cout << "╔══════════════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║              📋 TEMPS D'ATTENTE MOYEN PAR PRIORITÉ (minutes)             ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n\n";
        
        std::cout << "┌─────────────┬──────────────────┬──────────────────┬──────────────────┐\n";
        std::cout << "│ " << std::setw(11) << "Algorithme"
                  << " │ " << std::setw(16) << "🚨 Urgences"
                  << " │ " << std::setw(16) << "📅 Électifs"
                  << " │ " << std::setw(16) << "🚶 Ambulatoire" << " │\n";
        std::cout << "├─────────────┼──────────────────┼──────────────────┼──────────────────┤\n";
        
        for (const auto& r : resultats) {
            std::cout << "│ " << std::setw(11) << r.nomAlgorithme
                      << " │ " << std::setw(15) << std::fixed << std::setprecision(1) << r.tempsAttenteUrgence << "m"
                      << " │ " << std::setw(15) << std::fixed << std::setprecision(1) << r.tempsAttenteElective << "m"
                      << " │ " << std::setw(15) << std::fixed << std::setprecision(1) << r.tempsAttenteAmbulatoire << "m │\n";
        }
        
        std::cout << "└─────────────┴──────────────────┴──────────────────┴──────────────────┘\n";
    }
    
    /**
     * Génère une analyse détaillée avec recommandations
     */
    void genererAnalyse() {
        std::cout << "\n\n";
        std::cout << "╔══════════════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                    🎯 ANALYSE ET RECOMMANDATIONS                         ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n\n";
        
        // Trouver le meilleur algorithme pour chaque métrique
        auto meilleurAttenteMoyenne = std::min_element(resultats.begin(), resultats.end(),
            [](const ResultatSimulation& a, const ResultatSimulation& b) {
                return a.tempsAttenteMoyen < b.tempsAttenteMoyen;
            });
        
        auto meilleurAttenteUrgence = std::min_element(resultats.begin(), resultats.end(),
            [](const ResultatSimulation& a, const ResultatSimulation& b) {
                return a.tempsAttenteUrgence < b.tempsAttenteUrgence;
            });
        
        auto meilleurDebit = std::max_element(resultats.begin(), resultats.end(),
            [](const ResultatSimulation& a, const ResultatSimulation& b) {
                return a.debitPatients < b.debitPatients;
            });
        
        auto meilleurScore = std::min_element(resultats.begin(), resultats.end(),
            [](const ResultatSimulation& a, const ResultatSimulation& b) {
                return a.calculerScore() < b.calculerScore();
            });
        
        std::cout << "🏆 MEILLEURS ALGORITHMES PAR CRITÈRE:\n\n";
        
        std::cout << "   1️⃣  Temps d'attente moyen le plus bas:\n";
        std::cout << "       → " << meilleurAttenteMoyenne->nomAlgorithme 
                  << " (" << std::fixed << std::setprecision(1) 
                  << meilleurAttenteMoyenne->tempsAttenteMoyen << " minutes)\n\n";
        
        std::cout << "   2️⃣  Temps d'attente urgences le plus bas:\n";
        std::cout << "       → " << meilleurAttenteUrgence->nomAlgorithme 
                  << " (" << std::fixed << std::setprecision(1) 
                  << meilleurAttenteUrgence->tempsAttenteUrgence << " minutes)\n\n";
        
        std::cout << "   3️⃣  Débit de patients le plus élevé:\n";
        std::cout << "       → " << meilleurDebit->nomAlgorithme 
                  << " (" << std::fixed << std::setprecision(2) 
                  << meilleurDebit->debitPatients << " patients/heure)\n\n";
        
        std::cout << "   🎯  ALGORITHME RECOMMANDÉ (score global):\n";
        std::cout << "       → " << meilleurScore->nomAlgorithme 
                  << " (score: " << std::fixed << std::setprecision(2) 
                  << meilleurScore->calculerScore() << ")\n\n";
        
        // Analyse détaillée par algorithme
        std::cout << "\n📝 ANALYSE DÉTAILLÉE:\n\n";
        
        for (const auto& r : resultats) {
            std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
            std::cout << "▶ " << r.nomAlgorithme << ":\n\n";
            
            // Forces
            std::cout << "  ✅ FORCES:\n";
            if (r.nomAlgorithme == "FCFS") {
                std::cout << "     • Équitable: tous les patients traités dans l'ordre d'arrivée\n";
                std::cout << "     • Simple à implémenter et à comprendre\n";
                std::cout << "     • Prévisible pour les patients\n";
            } else if (r.nomAlgorithme == "SJF") {
                std::cout << "     • Optimise le temps d'attente moyen global\n";
                std::cout << "     • Maximise le débit de patients\n";
                std::cout << "     • Efficace pour opérations rapides\n";
            } else if (r.nomAlgorithme == "PRIORITY") {
                std::cout << "     • Priorise les urgences médicales\n";
                std::cout << "     • Temps d'attente minimal pour cas critiques\n";
                std::cout << "     • Conforme aux protocoles médicaux\n";
            }
            
            // Faiblesses
            std::cout << "\n  ⚠️  FAIBLESSES:\n";
            if (r.nomAlgorithme == "FCFS") {
                std::cout << "     • Ne prend pas en compte l'urgence médicale\n";
                std::cout << "     • Peut retarder les cas urgents\n";
                std::cout << "     • Opérations longues bloquent les courtes\n";
            } else if (r.nomAlgorithme == "SJF") {
                std::cout << "     • Risque de famine pour opérations longues\n";
                std::cout << "     • Ne respecte pas les priorités médicales\n";
                std::cout << "     • Peut retarder dangereusement des urgences\n";
            } else if (r.nomAlgorithme == "PRIORITY") {
                std::cout << "     • Temps d'attente plus long pour électifs\n";
                std::cout << "     • Peut créer de la frustration chez patients non-urgents\n";
                std::cout << "     • Débit global potentiellement réduit\n";
            }
            
            // Cas d'usage recommandé
            std::cout << "\n  💡 CAS D'USAGE RECOMMANDÉ:\n";
            if (r.nomAlgorithme == "FCFS") {
                std::cout << "     • Centres ambulatoires avec peu d'urgences\n";
                std::cout << "     • Opérations programmées uniquement\n";
                std::cout << "     • Environnement à faible criticité\n";
            } else if (r.nomAlgorithme == "SJF") {
                std::cout << "     • Optimisation du débit dans centres spécialisés\n";
                std::cout << "     • Opérations courtes et standardisées\n";
                std::cout << "     • Contexte avec urgences gérées séparément\n";
            } else if (r.nomAlgorithme == "PRIORITY") {
                std::cout << "     • ⭐ Hôpitaux avec service d'urgences actif\n";
                std::cout << "     • Environnement mixte urgences/électifs\n";
                std::cout << "     • Respect strict des protocoles médicaux\n";
            }
            
            std::cout << "\n";
        }
        
        // Recommandation finale
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        std::cout << "\n🎯 RECOMMANDATION FINALE:\n\n";
        std::cout << "Pour un environnement hospitalier standard avec urgences:\n";
        std::cout << "→ Utilisez l'algorithme PRIORITY\n\n";
        std::cout << "Raisons:\n";
        std::cout << "  • Sécurité des patients: les urgences sont traitées rapidement\n";
        std::cout << "  • Conformité médicale: respect de la hiérarchie des soins\n";
        std::cout << "  • Équilibre: les électifs sont traités après les urgences\n\n";
        
        std::cout << "Pour optimiser le débit dans un contexte contrôlé:\n";
        std::cout << "→ Utilisez SJF avec un système de priorité d'urgence séparé\n\n";
    }
    
    /**
     * Exporte les résultats en JSON
     */
    void exporterJSON(const std::string& fichier) {
        json j;
        j["benchmark"] = {
            {"timestamp", std::time(nullptr)},
            {"algorithmes_testes", resultats.size()}
        };
        
        json resultatsJson = json::array();
        for (const auto& r : resultats) {
            resultatsJson.push_back({
                {"algorithme", r.nomAlgorithme},
                {"nombrePatientsTotal", r.nombrePatientsTotal},
                {"nombrePatientsTraites", r.nombrePatientsTraites},
                {"tauxTraitement", r.nombrePatientsTotal > 0 ? 
                    (static_cast<double>(r.nombrePatientsTraites) / r.nombrePatientsTotal * 100.0) : 0.0},
                {"tempsAttenteMoyen", r.tempsAttenteMoyen},
                {"tempsAttenteMax", r.tempsAttenteMax},
                {"dureeOperationMoyenne", r.dureeOperationMoyenne},
                {"debitPatients", r.debitPatients},
                {"parPriorite", {
                    {"urgences", {
                        {"nombre", r.nombreUrgences},
                        {"tempsAttenteMoyen", r.tempsAttenteUrgence}
                    }},
                    {"electifs", {
                        {"nombre", r.nombreElectifs},
                        {"tempsAttenteMoyen", r.tempsAttenteElective}
                    }},
                    {"ambulatoires", {
                        {"nombre", r.nombreAmbulatoires},
                        {"tempsAttenteMoyen", r.tempsAttenteAmbulatoire}
                    }}
                }},
                {"score", r.calculerScore()}
            });
        }
        j["resultats"] = resultatsJson;
        
        std::ofstream file(fichier);
        file << std::setw(4) << j << std::endl;
        
        std::cout << "\n✅ Résultats exportés vers: " << fichier << "\n";
    }
    
    /**
     * Exporte un rapport Markdown
     */
    void exporterMarkdown(const std::string& fichier, const ConfigSimulation& config) {
        std::ofstream md(fichier);
        
        md << "# 📊 Rapport de Benchmark - Algorithmes d'Ordonnancement AutoMed\n\n";
        
        time_t now = std::time(nullptr);
        md << "**Date de génération:** " << std::ctime(&now) << "\n";
        
        md << "---\n\n";
        md << "## ⚙️ Prérequis et Configuration du Test\n\n";
        md << "### 📋 Paramètres de Simulation\n\n";
        md << "| Paramètre | Valeur |\n";
        md << "|-----------|--------|\n";
        md << "| **Nom du scénario** | " << config.nom << " |\n";
        md << "| **Durée de simulation** | " << config.dureeSimulationMinutes << " minutes (" 
           << (config.dureeSimulationMinutes / 60.0) << " heures) |\n";
        md << "| **Nombre de blocs opératoires** | " << config.nombreBlocs << " |\n";
        md << "| **Nombre d'équipes médicales** | " << config.nombreEquipes << " |\n";
        md << "| **Capacité salle d'attente** | " << config.capaciteSalleAttente << " patients |\n";
        md << "| **Capacité salle de réveil** | " << config.capaciteSalleReveil << " patients |\n";
        md << "| **Taux d'arrivée urgences** | " << std::fixed << std::setprecision(1) 
           << config.tauxArriveeHoraireUrgences << " patients/heure |\n";
        md << "| **Patients électifs programmés** | " << config.nombrePatientsElectifs << " |\n";
        md << "| **Mode de simulation** | ";
        if (config.facteurVitesse == 0.0) {
            md << "Instantané (temps réel désactivé) |\n";
        } else if (config.facteurVitesse == 1.0) {
            md << "Temps réel (1:1) |\n";
        } else {
            md << "Accéléré (facteur " << config.facteurVitesse << "x) |\n";
        }
        
        md << "\n### 🎯 Objectif du Test\n\n";
        if (config.nom.find("Normal") != std::string::npos) {
            md << "Ce test simule une **journée standard** dans un hôpital avec une charge de travail normale.\n";
            md << "Il permet d'évaluer le comportement des algorithmes dans des conditions typiques.\n";
        } else if (config.nom.find("Forte") != std::string::npos) {
            md << "Ce test simule une **période de forte activité** avec beaucoup de patients.\n";
            md << "Il évalue la résilience des algorithmes sous pression et leur capacité à gérer les pics de charge.\n";
        } else if (config.nom.find("Urgences") != std::string::npos) {
            md << "Ce test simule un **service d'urgences très actif** avec un flux important de cas urgents.\n";
            md << "Il évalue la capacité des algorithmes à prioriser correctement les urgences médicales.\n";
        } else if (config.nom.find("Limitées") != std::string::npos || config.nom.find("Limités") != std::string::npos) {
            md << "Ce test simule un **contexte avec peu de ressources** (blocs et équipes limités).\n";
            md << "Il évalue l'efficacité des algorithmes dans l'optimisation de ressources rares.\n";
        } else {
            md << "Ce test évalue les performances des algorithmes d'ordonnancement dans un contexte personnalisé.\n";
        }
        
        md << "\n### 📊 Métriques Évaluées\n\n";
        md << "- **Taux de traitement**: Pourcentage de patients ayant terminé leur parcours complet\n";
        md << "- **Temps d'attente moyen**: Durée moyenne avant le début de l'opération\n";
        md << "- **Temps d'attente maximum**: Attente la plus longue observée\n";
        md << "- **Débit de patients**: Nombre de patients traités par heure\n";
        md << "- **Performance par priorité**: Temps d'attente selon l'urgence médicale\n\n";
        
        md << "---\n\n";
        md << "## 📈 Résultats Comparatifs\n\n";
        md << "| Algorithme | Patients Total | Traités (%) | Attente Moy. | Attente Max | Débit (pat/h) |\n";
        md << "|------------|----------------|-------------|--------------|-------------|---------------|\n";
        
        for (const auto& r : resultats) {
            double tauxTraitement = r.nombrePatientsTotal > 0 ? 
                (static_cast<double>(r.nombrePatientsTraites) / r.nombrePatientsTotal * 100.0) : 0.0;
            
            md << "| " << r.nomAlgorithme 
               << " | " << r.nombrePatientsTotal
               << " | " << std::fixed << std::setprecision(1) << tauxTraitement << "%"
               << " | " << std::fixed << std::setprecision(1) << r.tempsAttenteMoyen << " min"
               << " | " << r.tempsAttenteMax << " min"
               << " | " << std::fixed << std::setprecision(2) << r.debitPatients << " |\n";
        }
        
        md << "\n## 🎯 Temps d'Attente par Priorité\n\n";
        md << "| Algorithme | 🚨 Urgences | 📅 Électifs | 🚶 Ambulatoire |\n";
        md << "|------------|-------------|-------------|----------------|\n";
        
        for (const auto& r : resultats) {
            md << "| " << r.nomAlgorithme
               << " | " << std::fixed << std::setprecision(1) << r.tempsAttenteUrgence << " min"
               << " | " << std::fixed << std::setprecision(1) << r.tempsAttenteElective << " min"
               << " | " << std::fixed << std::setprecision(1) << r.tempsAttenteAmbulatoire << " min |\n";
        }
        
        md << "\n## 🏆 Recommandation\n\n";
        md << "Pour un environnement hospitalier avec urgences: **PRIORITY**\n\n";
        md << "Pour optimisation du débit: **SJF** (avec gestion urgences séparée)\n";
        
        md.close();
        std::cout << "✅ Rapport Markdown exporté vers: " << fichier << "\n";
    }
};

#endif // ALGORITHM_COMPARISON_HPP
