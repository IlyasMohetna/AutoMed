# Exemples de Configurations de Simulation AutoMed

## 📖 Guide Rapide du Facteur de Vitesse

Le paramètre `facteurVitesse` détermine la vitesse d'exécution de la simulation:

```
Temps réel (sec) = Temps virtuel (sec) ÷ facteurVitesse
```

### Valeurs Recommandées

| Valeur | Description | Durée réelle pour 480 min |
|--------|-------------|---------------------------|
| `0.0` | ⚡ **Instantané** | ~2-5 secondes |
| `60.0` | 🏃 **Rapide** | 8 minutes |
| `96.0` | 👁️ **Observable** | 5 minutes |
| `600.0` | ⚡ **Très Rapide** | 48 secondes |
| `1.0` | 🕐 **Temps Réel** | 8 heures |

---

## 1️⃣ Configuration de Base - Mode Instantané

**Cas d'usage:** Tests rapides, validation de l'API, debugging

```json
{
  "nom": "Test Rapide",
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

**Résultat:** Simulation complète en quelques secondes

---

## 2️⃣ Configuration Observation - Mode Rapide

**Cas d'usage:** Observer l'évolution de la simulation, formation, démonstration

```json
{
  "nom": "Simulation Observable",
  "dureeSimulationMinutes": 480,
  "algorithme": "PRIORITY",
  "nombreBlocs": 4,
  "nombreEquipes": 4,
  "capaciteSalleAttente": 50,
  "capaciteSalleReveil": 20,
  "tauxArriveeHoraireUrgences": 3.0,
  "nombrePatientsElectifs": 15,
  "facteurVitesse": 96.0
}
```

**Résultat:** 480 minutes simulées en ~5 minutes réelles

**Idéal pour:**
- Suivre les événements en temps quasi-réel
- Démonstrations aux clients
- Sessions de formation

---

## 3️⃣ Configuration Stress Test - Haute Charge

**Cas d'usage:** Tester les limites du système, validation de performance

```json
{
  "nom": "Test de Charge",
  "dureeSimulationMinutes": 720,
  "algorithme": "SJF",
  "nombreBlocs": 8,
  "nombreEquipes": 6,
  "capaciteSalleAttente": 100,
  "capaciteSalleReveil": 40,
  "tauxArriveeHoraireUrgences": 8.0,
  "nombrePatientsElectifs": 50,
  "facteurVitesse": 600.0
}
```

**Résultat:** 720 minutes (12h) en ~72 secondes

**Caractéristiques:**
- Beaucoup de blocs et d'équipes
- Taux d'urgences élevé (8/heure)
- Grande capacité d'accueil
- Vitesse rapide pour tests de charge

---

## 4️⃣ Configuration Réaliste - Hôpital Moyen

**Cas d'usage:** Simulation réaliste d'un hôpital de taille moyenne

```json
{
  "nom": "Hôpital Moyen - Journée Type",
  "dureeSimulationMinutes": 540,
  "algorithme": "PRIORITY",
  "nombreBlocs": 5,
  "nombreEquipes": 5,
  "capaciteSalleAttente": 60,
  "capaciteSalleReveil": 25,
  "tauxArriveeHoraireUrgences": 2.5,
  "nombrePatientsElectifs": 20,
  "facteurVitesse": 60.0
}
```

**Résultat:** 540 minutes (9h) en 9 minutes réelles

**Scénario:** Journée type de 7h à 16h

---

## 5️⃣ Configuration Temps Réel - Démonstration Live

**Cas d'usage:** Présentation en direct, synchronisation avec temps réel

```json
{
  "nom": "Démo Temps Réel",
  "dureeSimulationMinutes": 60,
  "algorithme": "FCFS",
  "nombreBlocs": 3,
  "nombreEquipes": 3,
  "capaciteSalleAttente": 30,
  "capaciteSalleReveil": 15,
  "tauxArriveeHoraireUrgences": 1.0,
  "nombrePatientsElectifs": 5,
  "facteurVitesse": 1.0
}
```

**Résultat:** 60 minutes simulées = 60 minutes réelles

**⚠️ Attention:** Mode temps réel = durée d'exécution longue!

---

## 6️⃣ Configuration Comparaison Algorithmes

**Cas d'usage:** Comparer l'efficacité des différents algorithmes d'ordonnancement

### Test FCFS
```json
{
  "nom": "Comparaison - FCFS",
  "dureeSimulationMinutes": 480,
  "algorithme": "FCFS",
  "nombreBlocs": 4,
  "nombreEquipes": 4,
  "capaciteSalleAttente": 50,
  "capaciteSalleReveil": 20,
  "tauxArriveeHoraireUrgences": 2.5,
  "nombrePatientsElectifs": 15,
  "facteurVitesse": 0.0
}
```

### Test SJF
```json
{
  "nom": "Comparaison - SJF",
  "dureeSimulationMinutes": 480,
  "algorithme": "SJF",
  "nombreBlocs": 4,
  "nombreEquipes": 4,
  "capaciteSalleAttente": 50,
  "capaciteSalleReveil": 20,
  "tauxArriveeHoraireUrgences": 2.5,
  "nombrePatientsElectifs": 15,
  "facteurVitesse": 0.0
}
```

### Test PRIORITY
```json
{
  "nom": "Comparaison - PRIORITY",
  "dureeSimulationMinutes": 480,
  "algorithme": "PRIORITY",
  "nombreBlocs": 4,
  "nombreEquipes": 4,
  "capaciteSalleAttente": 50,
  "capaciteSalleReveil": 20,
  "tauxArriveeHoraireUrgences": 2.5,
  "nombrePatientsElectifs": 15,
  "facteurVitesse": 0.0
}
```

**Méthode:**
1. Créer les 3 simulations avec la même configuration (sauf algorithme)
2. Lancer chacune en mode instantané
3. Comparer les statistiques (temps d'attente moyen, taux d'occupation, etc.)

---

## 7️⃣ Configuration Urgences Intensives

**Cas d'usage:** Simuler une situation d'afflux massif d'urgences

```json
{
  "nom": "Crise Urgences",
  "dureeSimulationMinutes": 240,
  "algorithme": "PRIORITY",
  "nombreBlocs": 6,
  "nombreEquipes": 6,
  "capaciteSalleAttente": 100,
  "capaciteSalleReveil": 30,
  "tauxArriveeHoraireUrgences": 10.0,
  "nombrePatientsElectifs": 5,
  "facteurVitesse": 120.0
}
```

**Caractéristiques:**
- Taux d'urgences très élevé: 10 par heure
- Peu de patients électifs
- Algorithme de priorité pour gérer les urgences
- Simulation de 4h en 2 minutes réelles

---

## 8️⃣ Configuration Nuit Calme

**Cas d'usage:** Simuler une période de faible activité

```json
{
  "nom": "Garde de Nuit",
  "dureeSimulationMinutes": 480,
  "algorithme": "FCFS",
  "nombreBlocs": 2,
  "nombreEquipes": 2,
  "capaciteSalleAttente": 20,
  "capaciteSalleReveil": 10,
  "tauxArriveeHoraireUrgences": 0.5,
  "nombrePatientsElectifs": 0,
  "facteurVitesse": 240.0
}
```

**Caractéristiques:**
- Peu de ressources (équipes de nuit réduites)
- Seulement des urgences (0.5 par heure)
- Pas d'électifs
- Simulation de 8h en 2 minutes

---

## 9️⃣ Configuration Debug - Événements Espacés

**Cas d'usage:** Débugger la simulation avec des événements bien espacés

```json
{
  "nom": "Debug Mode",
  "dureeSimulationMinutes": 120,
  "algorithme": "FCFS",
  "nombreBlocs": 2,
  "nombreEquipes": 2,
  "capaciteSalleAttente": 20,
  "capaciteSalleReveil": 10,
  "tauxArriveeHoraireUrgences": 0.5,
  "nombrePatientsElectifs": 3,
  "facteurVitesse": 30.0
}
```

**Résultat:** 120 minutes en 4 minutes réelles - temps de voir chaque événement

---

## 🔟 Configuration Performance - Mini Test

**Cas d'usage:** Test ultra-rapide pour validation continue

```json
{
  "nom": "Mini Test",
  "dureeSimulationMinutes": 60,
  "algorithme": "FCFS",
  "nombreBlocs": 2,
  "nombreEquipes": 2,
  "capaciteSalleAttente": 20,
  "capaciteSalleReveil": 10,
  "tauxArriveeHoraireUrgences": 1.0,
  "nombrePatientsElectifs": 3,
  "facteurVitesse": 0.0
}
```

**Résultat:** Test complet en ~1 seconde

**Idéal pour:** CI/CD, tests automatisés, validation rapide

---

## 📊 Tableau de Conversion Facteur Vitesse

Pour calculer votre propre facteur de vitesse:

```
facteurVitesse = (Durée virtuelle en secondes) / (Durée réelle souhaitée en secondes)
```

**Exemples:**

| Durée virtuelle | Durée réelle souhaitée | Calcul | facteurVitesse |
|-----------------|------------------------|--------|----------------|
| 480 min (28800s) | 8 min (480s) | 28800 / 480 | 60.0 |
| 480 min (28800s) | 5 min (300s) | 28800 / 300 | 96.0 |
| 480 min (28800s) | 2 min (120s) | 28800 / 120 | 240.0 |
| 480 min (28800s) | 1 min (60s) | 28800 / 60 | 480.0 |
| 720 min (43200s) | 10 min (600s) | 43200 / 600 | 72.0 |
| 60 min (3600s) | 1 min (60s) | 3600 / 60 | 60.0 |

---

## 🎯 Recommandations par Cas d'Usage

### Pour le Développement
- **Tests unitaires:** `facteurVitesse: 0.0` (instantané)
- **Debugging:** `facteurVitesse: 30.0 - 60.0` (observable)

### Pour les Démonstrations
- **Présentation courte:** `facteurVitesse: 96.0 - 120.0` (5-4 min pour 8h)
- **Présentation détaillée:** `facteurVitesse: 60.0` (8 min pour 8h)
- **Workshop interactif:** `facteurVitesse: 1.0` (temps réel sur courte durée)

### Pour l'Analyse
- **Comparaison d'algorithmes:** `facteurVitesse: 0.0` (rapide)
- **Analyse statistique:** `facteurVitesse: 600.0` (assez rapide mais observable)

### Pour la Formation
- **Formation initiale:** `facteurVitesse: 60.0 - 96.0` (bien pour suivre)
- **Pratique avancée:** `facteurVitesse: 1.0` (temps réel)

---

## 🚨 Notes Importantes

1. **Mode Instantané (0.0)** est le comportement par défaut si le paramètre est omis
2. **Mode Temps Réel (1.0)** peut être très long - réservé aux courtes durées
3. Plus le `facteurVitesse` est **élevé**, plus la simulation est **rapide**
4. Les logs console affichent toujours le temps virtuel, pas le temps réel

---

## 💻 Exemples de Commandes curl

### Simulation rapide pour tests
```bash
curl -X POST http://localhost:8080/api/simulation/create \
  -H "Content-Type: application/json" \
  -d '{
    "nom": "Test Rapide",
    "dureeSimulationMinutes": 480,
    "algorithme": "FCFS",
    "nombreBlocs": 3,
    "nombreEquipes": 3,
    "facteurVitesse": 0.0
  }'
```

### Simulation observable
```bash
curl -X POST http://localhost:8080/api/simulation/create \
  -H "Content-Type: application/json" \
  -d '{
    "nom": "Demo Observable",
    "dureeSimulationMinutes": 480,
    "algorithme": "PRIORITY",
    "nombreBlocs": 4,
    "nombreEquipes": 4,
    "facteurVitesse": 96.0
  }'
```

---

**Astuce:** Commencez toujours avec `facteurVitesse: 0.0` pour valider votre configuration, puis augmentez progressivement pour observer le comportement! 🎓
