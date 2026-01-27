# Guide de Tests Manuels avec Postman - AutoMed API

## Configuration Initiale

### Informations de Base
- **URL de base**: `http://localhost:8080`
- **Format**: JSON
- **Headers requis**: `Content-Type: application/json`

---

## 1. Tests de Base (Health & Info)

### 1.1 Health Check
Vérifie que le serveur est opérationnel.

**Requête:**
```
GET http://localhost:8080/api/health
```

**Réponse attendue (200 OK):**
```json
{
  "status": "ok",
  "service": "AutoMed Backend",
  "timestamp": 1738000000
}
```

---

### 1.2 Informations du Serveur
Récupère les informations sur l'API.

**Requête:**
```
GET http://localhost:8080/api/info
```

**Réponse attendue (200 OK):**
```json
{
  "name": "AutoMed - Simulateur de Blocs Opératoires",
  "version": "1.0.0",
  "type": "Backend C++ REST API"
}
```

---

### 1.3 Test Echo
Teste l'envoi et la réception de données JSON.

**Requête:**
```
POST http://localhost:8080/api/echo
Content-Type: application/json
```

**Body:**
```json
{
  "message": "Test echo",
  "data": {
    "test": true,
    "value": 123
  }
}
```

**Réponse attendue (200 OK):**
```json
{
  "received": {
    "message": "Test echo",
    "data": {
      "test": true,
      "value": 123
    }
  },
  "timestamp": 1738000000
}
```

---

## 2. Gestion des Simulations

### 2.1 Créer une Simulation
Crée une nouvelle simulation avec une configuration personnalisée.

**Requête:**
```
POST http://localhost:8080/api/simulation/create
Content-Type: application/json
```

**Body (exemple minimal - mode instantané):**
```json
{
  "nom": "Simulation Test 1",
  "dureeSimulationMinutes": 480,
  "algorithme": "FCFS",
  "nombreBlocs": 3,
  "nombreEquipes": 3,
  "capaciteSalleAttente": 50,
  "capaciteSalleReveil": 20,
  "tauxArriveeHoraireUrgences": 2.0,
  "nombrePatientsElectifs": 10,
  "facteurVitesse": 0.0
}
```

**Body (exemple mode rapide - 1 min virtuel = 1 sec réel):**
```json
{
  "nom": "Simulation Rapide",
  "dureeSimulationMinutes": 480,
  "algorithme": "FCFS",
  "nombreBlocs": 3,
  "nombreEquipes": 3,
  "capaciteSalleAttente": 50,
  "capaciteSalleReveil": 20,
  "tauxArriveeHoraireUrgences": 2.0,
  "nombrePatientsElectifs": 10,
  "facteurVitesse": 60.0
}
```

**Body (exemple complet avec tous les paramètres):**
```json
{
  "nom": "Simulation Avancée",
  "dureeSimulationMinutes": 720,
  "algorithme": "SJF",
  "nombreBlocs": 5,
  "nombreEquipes": 4,
  "capaciteSalleAttente": 100,
  "capaciteSalleReveil": 30,
  "tauxArriveeHoraireUrgences": 3.5,
  "nombrePatientsElectifs": 20,
  "facteurVitesse": 600.0
}
```

**Algorithmes disponibles:**
- `"FCFS"` - First Come First Served
- `"SJF"` - Shortest Job First
- `"PRIORITY"` - Par priorité
- `"RR"` - Round Robin

**Facteur de vitesse (facteurVitesse):**
- `0.0` - **Mode INSTANTANÉ** (défaut) : Simulation complète en quelques secondes
- `1.0` - **Mode TEMPS RÉEL** : 1 minute virtuelle = 1 minute réelle (480 min = 8h réelles)
- `60.0` - **Mode RAPIDE** : 1 minute virtuelle = 1 seconde réelle (480 min = 8 min réelles)
- `600.0` - **Mode TRÈS RAPIDE** : 1 minute virtuelle = 0.1 seconde réelle (480 min = 48 sec réelles)
- Valeur personnalisée : Utilisez n'importe quel nombre > 0

**Note:** Conservez le `simulationId` retourné pour les requêtes suivantes!
```json
{
  "success": true,
  "simulationId": 1,
  "message": "Simulation créée avec succès"
}
```

**Réponse en cas d'erreur (400 Bad Request):**
```json
{
  "success": false,
  "error": "Erreur lors de la création",
  "message": "Description de l'erreur"
}
```

**Note:** Conservez le `simulationId` retourné pour les requêtes suivantes!

---

### 2.2 Lister Toutes les Simulations
Récupère la liste de toutes les simulations existantes.

**Requête:**
```
GET http://localhost:8080/api/simulations
```

**Réponse attendue (200 OK):**
```json
{
  "simulations": [1, 2, 3],
  "count": 3
}
```

---

### 2.3 Démarrer une Simulation
Démarre l'exécution d'une simulation créée.

**Requête:**
```
POST http://localhost:8080/api/simulation/1/start
```

*Remplacez `1` par l'ID de votre simulation*

**Réponse attendue (200 OK):**
```json
{
  "success": true,
  "message": "Simulation démarrée",
  "simulationId": 1
}
```

**Réponse si simulation non trouvée (404 Not Found):**
```json
{
  "success": false,
  "error": "Impossible de démarrer la simulation"
}
```

---

### 2.4 Mettre en Pause une Simulation
Met en pause une simulation en cours d'exécution.

**Requête:**
```
POST http://localhost:8080/api/simulation/1/pause
```

**Réponse attendue (200 OK):**
```json
{
  "success": true,
  "message": "Simulation mise en pause"
}
```

---

### 2.5 Reprendre une Simulation
Reprend une simulation mise en pause.

**Requête:**
```
POST http://localhost:8080/api/simulation/1/resume
```

**Réponse attendue (200 OK):**
```json
{
  "success": true,
  "message": "Simulation reprise"
}
```

---

### 2.6 Arrêter une Simulation
Arrête complètement une simulation.

**Requête:**
```
POST http://localhost:8080/api/simulation/1/stop
```

**Réponse attendue (200 OK):**
```json
{
  "success": true,
  "message": "Simulation arrêtée"
}
```

---

### 2.7 Obtenir le Statut d'une Simulation
Récupère l'état actuel d'une simulation.

**Requête:**
```
GET http://localhost:8080/api/simulation/1/status
```

**Réponse attendue (200 OK):**
```json
{
  "simulationId": 1,
  "nom": "Simulation Test 1",
  "etat": "EN_COURS",
  "tempsSimule": 120,
  "dureeMinutes": 480,
  "progressPourcentage": 25.0,
  "blocs": [...],
  "salleAttente": {...},
  "salleReveil": {...}
}
```

---

### 2.8 Obtenir les Statistiques
Récupère les statistiques détaillées d'une simulation.

**Requête:**
```
GET http://localhost:8080/api/simulation/1/stats
```

**Réponse attendue (200 OK):**
```json
{
  "patientsTraites": 45,
  "patientsEnAttente": 8,
  "tempsAttenteMovenne": 32.5,
  "tauxOccupationBlocs": 78.3,
  "nombreOperationsTerminees": 42,
  "nombreUrgencesTraitees": 15,
  "details": {...}
}
```

---

### 2.9 Obtenir les Événements
Récupère les derniers événements d'une simulation.

**Requête:**
```
GET http://localhost:8080/api/simulation/1/events
```

**Réponse attendue (200 OK):**
```json
{
  "events": [
    {
      "timestamp": 120,
      "type": "ARRIVEE_PATIENT",
      "description": "Patient P001 arrivé en salle d'attente"
    },
    {
      "timestamp": 125,
      "type": "DEBUT_OPERATION",
      "description": "Opération démarrée pour patient P001 dans bloc B1"
    }
  ],
  "count": 2
}
```

---

### 2.10 Supprimer une Simulation
Supprime définitivement une simulation.

**Requête:**
```
DELETE http://localhost:8080/api/simulation/1
```

**Réponse attendue (200 OK):**
```json
{
  "success": true,
  "message": "Simulation supprimée"
}
```

**Réponse si simulation non trouvée (404 Not Found):**
```json
{
  "success": false,
  "error": "Simulation non trouvée"
}
```

---

## 3. Scénarios de Test Complets

### 🚀 Comprendre le Facteur de Vitesse

Le paramètre `facteurVitesse` contrôle la relation entre le temps virtuel simulé et le temps réel d'exécution.

**Formule:** `Temps réel (secondes) = Temps virtuel (secondes) / facteurVitesse`
facteurVitesse = Temps Virtuel / Temps réel

**Exemples pratiques:**

| facteurVitesse | Mode | 480 min simulées = | Cas d'usage |
|---------------|------|-------------------|-------------|
| `0.0` | INSTANTANÉ | ~quelques secondes | Tests rapides, debugging |
| `1.0` | TEMPS RÉEL | 8 heures réelles | Simulation réaliste, démonstration |
| `60.0` | RAPIDE | 8 minutes réelles | Observation détaillée, formation |
| `600.0` | TRÈS RAPIDE | 48 secondes réelles | Tests de charge, validation |
| `3600.0` | ULTRA RAPIDE | 8 secondes réelles | Tests de performance |

**💡 Conseil:** Pour observer une simulation de 480 minutes de façon réaliste en ~5 minutes réelles, utilisez `facteurVitesse = 96.0`

---

### Scénario 1: Cycle de Vie Complet d'une Simulation

1. **Créer** une simulation → `POST /api/simulation/create`
2. **Vérifier** qu'elle existe → `GET /api/simulations`
3. **Démarrer** la simulation → `POST /api/simulation/{id}/start`
4. **Surveiller** le statut → `GET /api/simulation/{id}/status`
5. **Mettre en pause** → `POST /api/simulation/{id}/pause`
6. **Consulter** les stats → `GET /api/simulation/{id}/stats`
7. **Reprendre** → `POST /api/simulation/{id}/resume`
8. **Consulter** les événements → `GET /api/simulation/{id}/events`
9. **Arrêter** → `POST /api/simulation/{id}/stop`
10. **Supprimer** → `DELETE /api/simulation/{id}`

---

### Scénario 2: Test de Charge Basique

1. Créer 5 simulations différentes avec différentes configurations
2. Lister toutes les simulations pour vérifier qu'elles existent
3. Démarrer toutes les simulations
4. Monitorer le statut de chacune
5. Arrêter et supprimer toutes les simulations

---

### Scénario 3: Test des Algorithmes

Créer plusieurs simulations avec le même profil mais différents algorithmes:

**Test FCFS:**
```json
{
  "nom": "Test FCFS",
  "algorithme": "FCFS",
  "nombreBlocs": 3,
  "nombrePatientsElectifs": 15
}
```

**Test SJF:**
```json
{
  "nom": "Test SJF",
  "algorithme": "SJF",
  "nombreBlocs": 3,
  "nombrePatientsElectifs": 15
}
```

**Test PRIORITY:**
```json
{
  "nom": "Test Priority",
  "algorithme": "PRIORITY",
  "nombreBlocs": 3,
  "nombrePatientsElectifs": 15
}
```

Comparer les statistiques de performance de chaque algorithme.

---

## 4. Cas d'Erreur à Tester

### 4.1 ID de Simulation Invalide
```
GET http://localhost:8080/api/simulation/9999/status
```
Devrait retourner 404 avec message d'erreur.

### 4.2 JSON Invalide
```
POST http://localhost:8080/api/echo
Body: {invalid json}
```
Devrait retourner 400 avec message d'erreur.

### 4.3 Paramètres Manquants
```
POST http://localhost:8080/api/simulation/create
Body: {}
```
Devrait créer une simulation avec valeurs par défaut ou retourner une erreur.

### 4.4 Opération Invalide
Tenter de démarrer une simulation déjà en cours, ou mettre en pause une simulation arrêtée.

---

## 5. Collection Postman (Import)

Pour faciliter les tests, vous pouvez créer une collection Postman avec toutes ces requêtes.

### Variables d'environnement suggérées:
- `base_url`: `http://localhost:8080`
- `simulation_id`: `1` (à mettre à jour après création)

### Structure de collection suggérée:
```
AutoMed API
├── 1. Health Checks
│   ├── Health Check
│   ├── Server Info
│   └── Echo Test
├── 2. Simulation Lifecycle
│   ├── Create Simulation
│   ├── List Simulations
│   ├── Start Simulation
│   ├── Pause Simulation
│   ├── Resume Simulation
│   ├── Stop Simulation
│   └── Delete Simulation
└── 3. Monitoring
    ├── Get Status
    ├── Get Statistics
    └── Get Events
```

---

## 6. Notes Importantes

### CORS
L'API est configurée avec CORS pour accepter les requêtes depuis:
- `http://localhost:3000` (frontend)

Si vous testez depuis Postman, CORS ne devrait pas être un problème.

### Port par Défaut
Le serveur écoute sur le port **8080** par défaut.

### Format des Réponses
Toutes les réponses sont en JSON avec le header `Content-Type: application/json`.

### Codes de Statut HTTP
- **200 OK**: Requête réussie
- **400 Bad Request**: Erreur de format ou paramètres invalides
- **404 Not Found**: Ressource (simulation) non trouvée

---

## 7. Conseils de Test

1. **Commencez par les tests de base** (health, info, echo) pour vérifier que le serveur fonctionne.

2. **Créez toujours une simulation** avant de tester les autres endpoints qui nécessitent un ID.

3. **Notez les IDs** de simulation retournés pour les réutiliser dans les tests suivants.

4. **Testez le cycle de vie complet** avant de tester des cas limites.

5. **Surveillez les logs du serveur** pendant les tests pour voir les détails de traitement.

6. **Utilisez les variables Postman** pour éviter de modifier manuellement les IDs dans chaque requête.

7. **Créez des tests automatisés** dans Postman en utilisant l'onglet "Tests" pour valider automatiquement les réponses.

---

## 8. Exemple de Script de Test Postman

Pour automatiser la validation, ajoutez ce script dans l'onglet "Tests" de Postman:

```javascript
// Test pour /api/health
pm.test("Status code is 200", function () {
    pm.response.to.have.status(200);
});

pm.test("Response has status field", function () {
    var jsonData = pm.response.json();
    pm.expect(jsonData).to.have.property('status');
    pm.expect(jsonData.status).to.eql('ok');
});

// Test pour /api/simulation/create
pm.test("Simulation created successfully", function () {
    var jsonData = pm.response.json();
    pm.expect(jsonData.success).to.eql(true);
    pm.expect(jsonData).to.have.property('simulationId');
    
    // Sauvegarder l'ID pour les requêtes suivantes
    pm.environment.set("simulation_id", jsonData.simulationId);
});
```

---

## Démarrage Rapide

1. Démarrez le serveur AutoMed backend
2. Ouvrez Postman
3. Commencez par: `GET http://localhost:8080/api/health`
4. Si OK, créez une simulation: `POST http://localhost:8080/api/simulation/create`
5. Utilisez l'ID retourné pour tester les autres endpoints

**Bon test! 🚀**
