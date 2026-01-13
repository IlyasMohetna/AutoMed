# 🏥 AutoMed - Simulateur de Blocs Opératoires

> Projet universitaire - FISA Informatique UTBM 2025  
> Simulateur de gestion de blocs opératoires hospitaliers

## 📋 Description

AutoMed est un simulateur d'unité de blocs opératoires qui modélise le parcours des patients depuis la salle d'attente pré-opératoire jusqu'à la salle de réveil, en gérant les ressources médicales et en prenant en compte les urgences.

**Architecture:**

- **Backend C++**: Moteur de simulation + serveur WebSocket (port 8080)
- **Frontend React**: Interface utilisateur moderne avec Tailwind CSS (port 3000)
- **Docker**: Containerisation complète avec hot-reload

## Démarrage Rapide

### Prérequis

- Docker
- Docker Compose

### Installation et Lancement

```bash
# Cloner le projet
git clone https://github.com/IlyasMohetna/AutoMed.git
cd AutoMed

# Démarrer l'application (build + run automatique)
docker-compose up

# Ou en mode détaché
docker-compose up -d
```

L'application sera accessible sur:

- **Frontend**: http://localhost:3000
- **Backend API**: ws://localhost:8080
- **Backend WebSocket**: ws://localhost:8080

### Autres Commandes

```bash
# Voir les logs en temps réel
docker-compose logs -f

# Arrêter les containers
docker-compose down

# Rebuild complet
docker-compose up --build

# Nettoyage complet
docker-compose down -v
```

## 🛠️ Technologies

### Backend

- **C++17** - Langage principal
- **websocketpp** - Communication WebSocket (header-only)
- **nlohmann/json** - Parsing JSON (header-only)
- **Boost** - Bibliothèques système

### Frontend

- **React 18** - Framework UI
- **Vite** - Build tool moderne
- **Tailwind CSS** - Styling
- **WebSocket API** - Communication temps réel

### DevOps

- **Docker** - Containerisation
- **Docker Compose** - Orchestration
- **Hot-reload** - Développement en temps réel

## 🔧 Développement

### Hot-Reload

Les deux containers supportent le hot-reload:

- **Backend C++**: Recompilation automatique avec `inotifywait`
- **Frontend React**: Hot Module Replacement (HMR) natif de Vite

Modifiez simplement les fichiers sources et les changements seront appliqués automatiquement.

### Compilation Backend (locale)

```bash
cd backend
make          # Compiler
make run      # Compiler et exécuter
make clean    # Nettoyer
```

## 📄 Licence

Projet universitaire - UTBM 2025
