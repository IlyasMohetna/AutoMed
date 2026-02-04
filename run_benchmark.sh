#!/bin/bash

# Script pour exécuter le benchmark des algorithmes d'ordonnancement
# AutoMed - Simulateur de Blocs Opératoires

echo "╔══════════════════════════════════════════════════════════════════════════╗"
echo "║              AutoMed - Benchmark des Algorithmes                         ║"
echo "╚══════════════════════════════════════════════════════════════════════════╝"
echo ""

# Vérifier si Docker est lancé
if ! docker ps > /dev/null 2>&1; then
    echo "❌ Docker n'est pas lancé. Veuillez démarrer Docker."
    exit 1
fi

# Créer le dossier results s'il n'existe pas
mkdir -p results

# Demander le scénario si non fourni en argument
if [ -z "$1" ]; then
    echo "Scénarios disponibles:"
    echo "  1 - Charge normale (8h, 3 blocs, 2 urgences/h)"
    echo "  2 - Forte charge (12h, 3 blocs, 4 urgences/h)"
    echo "  3 - Urgences intensives (8h, 4 blocs, 6 urgences/h)"
    echo "  4 - Ressources limitées (8h, 2 blocs, 2 urgences/h)"
    echo "  5 - Personnalisé (configuration interactive)"
    echo ""
    read -p "Choisissez un scénario (1-5): " SCENARIO
else
    SCENARIO=$1
fi

# Validation du scénario
if ! [[ "$SCENARIO" =~ ^[1-5]$ ]]; then
    echo "❌ Scénario invalide. Veuillez choisir entre 1 et 5."
    exit 1
fi

echo ""
echo "🚀 Lancement du benchmark dans Docker..."
echo ""

# Exécuter le benchmark dans le container
docker compose exec backend /bin/bash -c "
    cd /app && \
    mkdir -p results && \
    make clean && \
    make benchmark && \
    ./bin/automed_benchmark $SCENARIO
"

# Vérifier si le benchmark a réussi
if [ $? -eq 0 ]; then
    echo ""
    echo "╔══════════════════════════════════════════════════════════════════════════╗"
    echo "║                    ✅ BENCHMARK TERMINÉ AVEC SUCCÈS                      ║"
    echo "╚══════════════════════════════════════════════════════════════════════════╝"
    echo ""
    echo "📁 Résultats disponibles dans:"
    echo "   • backend/results/benchmark_results.json"
    echo "   • backend/results/benchmark_report.md"
    echo ""
    
    # Copier les résultats localement si possible
    if docker compose exec backend test -f /app/results/benchmark_results.json; then
        echo "📥 Copie des résultats en local..."
        docker compose cp backend:/app/results/. ./results/
        echo "✅ Fichiers copiés dans ./results/"
        echo ""
        
        # Afficher un aperçu du rapport si disponible
        if [ -f "./results/benchmark_report.md" ]; then
            echo "📄 Aperçu du rapport:"
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            head -n 30 ./results/benchmark_report.md
            echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
            echo ""
            echo "📖 Pour voir le rapport complet: cat results/benchmark_report.md"
        fi
    fi
else
    echo ""
    echo "❌ Le benchmark a échoué. Vérifiez les logs ci-dessus."
    exit 1
fi
