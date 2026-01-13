#!/bin/bash

# Script de hot-reload pour C++
echo "🔄 Hot-reload activé pour le backend C++"
echo "📁 Surveillance du dossier /app/src"
echo ""

# Variable globale pour le PID
SERVER_PID=""
REBUILD_IN_PROGRESS=false

# Fonction pour arrêter proprement le serveur
stop_server() {
    if [ -n "$SERVER_PID" ] && kill -0 $SERVER_PID 2>/dev/null; then
        echo "⏹️  Arrêt du serveur (PID: $SERVER_PID)..."
        kill -15 $SERVER_PID 2>/dev/null
        wait $SERVER_PID 2>/dev/null
        
        # Attendre que le port soit complètement libéré
        sleep 2
    fi
    SERVER_PID=""
}

# Fonction de build et run
build_and_run() {
    if [ "$REBUILD_IN_PROGRESS" = true ]; then
        return
    fi
    
    REBUILD_IN_PROGRESS=true
    
    echo "⚙️  Compilation en cours..."
    make clean > /dev/null 2>&1
    
    if make; then
        echo "✅ Compilation réussie!"
        echo ""
        echo "🚀 Démarrage du serveur..."
        echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
        ./bin/automed_server &
        SERVER_PID=$!
        sleep 1
    else
        echo "❌ Erreur de compilation"
        SERVER_PID=""
    fi
    
    REBUILD_IN_PROGRESS=false
}

# Trap pour nettoyer à l'arrêt du script
trap 'stop_server; exit 0' SIGTERM SIGINT

# Build initial
build_and_run

# Surveillance des changements avec debounce
inotifywait -m -r -e close_write --exclude '(build|bin)' /app/src |
while read -r directory event filename; do
    # Filtrer uniquement les fichiers .cpp et .hpp
    if [[ "$filename" =~ \.(cpp|hpp)$ ]] && [ "$REBUILD_IN_PROGRESS" = false ]; then
        echo ""
        echo "🔔 Changement détecté: $filename"
        
        # Arrêter le serveur proprement
        stop_server
        
        # Rebuild et restart
        build_and_run
    fi
done
