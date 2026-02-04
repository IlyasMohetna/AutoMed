# 🧪 Benchmark des Algorithmes d'Ordonnancement - AutoMed

Ce dossier contient un outil de benchmark pour comparer les performances des différents algorithmes d'ordonnancement des blocs opératoires.

## 📊 Objectif

Comparer quantitativement les algorithmes suivants:

- **FCFS** (First Come First Served) - Premier arrivé, premier servi
- **SJF** (Shortest Job First) - Plus courte opération en premier
- **PRIORITY** - Ordonnancement par priorité médicale

## 🚀 Utilisation Rapide

### Lancer un benchmark automatique:

```bash
./run_benchmark.sh
```

Ou avec un scénario spécifique:

```bash
./run_benchmark.sh 1  # Charge normale
./run_benchmark.sh 2  # Forte charge
./run_benchmark.sh 3  # Urgences intensives
./run_benchmark.sh 4  # Ressources limitées
./run_benchmark.sh 5  # Personnalisé (interactif)
```

## 📈 Scénarios de Test

### 1️⃣ Charge Normale

- **Contexte**: Journée standard dans un hôpital
- **Paramètres**:
  - Durée: 8 heures (480 min)
  - Blocs opératoires: 3
  - Équipes médicales: 3
  - Urgences: 2 par heure
  - Patients électifs: 10
- **Objectif**: Évaluer le comportement normal

### 2️⃣ Forte Charge

- **Contexte**: Pic d'activité prolongé
- **Paramètres**:
  - Durée: 12 heures (720 min)
  - Blocs opératoires: 3
  - Équipes médicales: 3
  - Urgences: 4 par heure
  - Patients électifs: 20
- **Objectif**: Tester la résilience sous pression

### 3️⃣ Urgences Intensives

- **Contexte**: Service d'urgences très actif
- **Paramètres**:
  - Durée: 8 heures (480 min)
  - Blocs opératoires: 4
  - Équipes médicales: 4
  - Urgences: 6 par heure
  - Patients électifs: 5
- **Objectif**: Évaluer la gestion des urgences

### 4️⃣ Ressources Limitées

- **Contexte**: Hôpital avec peu de ressources
- **Paramètres**:
  - Durée: 8 heures (480 min)
  - Blocs opératoires: 2
  - Équipes médicales: 2
  - Urgences: 2 par heure
  - Patients électifs: 15
- **Objectif**: Optimiser l'utilisation des ressources

### 5️⃣ Personnalisé

- **Contexte**: Configuration libre
- **Objectif**: Tester un cas spécifique

## 📊 Métriques Analysées

### Métriques Globales

- **Nombre de patients traités**: Efficacité globale
- **Taux de traitement**: Pourcentage de patients traités
- **Temps d'attente moyen**: Moyenne tous patients confondus
- **Temps d'attente maximum**: Pic d'attente observé
- **Débit de patients**: Patients traités par heure
- **Durée opération moyenne**: Temps moyen en bloc

### Métriques par Priorité

- **🚨 Urgences**: Temps d'attente des cas critiques
- **📅 Électifs**: Temps d'attente des opérations programmées
- **🚶 Ambulatoires**: Temps d'attente des cas légers

### Score Global

Un score pondéré calculé comme:

```
Score = (Urgences × 3.0) + (Électifs × 1.5) + (Ambulatoires × 1.0) + Pénalité
```

_Plus le score est bas, meilleure est la performance_

## 📁 Fichiers Générés

Après l'exécution, les résultats sont disponibles dans `./results/`:

### `benchmark_results.json`

Format JSON structuré avec toutes les métriques:

```json
{
  "benchmark": {
    "timestamp": 1738000000,
    "algorithmes_testes": 3
  },
  "resultats": [
    {
      "algorithme": "FCFS",
      "nombrePatientsTotal": 26,
      "nombrePatientsTraites": 24,
      "tauxTraitement": 92.3,
      "tempsAttenteMoyen": 45.2,
      ...
    }
  ]
}
```

### `benchmark_report.md`

Rapport Markdown avec:

- Tableaux comparatifs
- Graphiques de performance
- Recommandations d'utilisation

## 🎯 Interprétation des Résultats

### Algorithme FCFS (First Come First Served)

**✅ Forces:**

- Équitable: tous traités dans l'ordre
- Simple et prévisible
- Bon pour contextes non-urgents

**⚠️ Faiblesses:**

- Ignore l'urgence médicale
- Opérations longues bloquent les courtes
- Peut retarder des cas critiques

**💡 Utilisation recommandée:**

- Centres ambulatoires
- Opérations programmées uniquement
- Environnement à faible criticité

### Algorithme SJF (Shortest Job First)

**✅ Forces:**

- Optimise le temps d'attente moyen
- Maximise le débit de patients
- Efficace pour opérations rapides

**⚠️ Faiblesses:**

- Risque de famine pour opérations longues
- Ne respecte pas les priorités médicales
- Peut retarder dangereusement des urgences

**💡 Utilisation recommandée:**

- Centres spécialisés (chirurgie ambulatoire)
- Opérations courtes et standardisées
- Contexte avec urgences gérées séparément

### Algorithme PRIORITY (Par Priorité)

**✅ Forces:**

- ⭐ Priorise les urgences médicales
- Temps d'attente minimal pour cas critiques
- Conforme aux protocoles médicaux
- Sécurité maximale pour les patients

**⚠️ Faiblesses:**

- Temps d'attente plus long pour électifs
- Débit global potentiellement réduit
- Peut créer frustration chez non-urgents

**💡 Utilisation recommandée:**

- **⭐ RECOMMANDÉ** pour hôpitaux avec urgences
- Environnement mixte urgences/électifs
- Respect strict des protocoles médicaux
- Contexte où la sécurité prime sur l'efficacité

## 🏆 Recommandation Générale

### Pour un hôpital standard:

```
→ Algorithme PRIORITY
```

**Raisons:**

1. **Sécurité**: Les urgences sont traitées immédiatement
2. **Conformité**: Respect de la hiérarchie médicale
3. **Équilibre**: Les électifs sont traités après les urgences
4. **Éthique**: Priorité à la vie et à la santé

### Pour optimiser le débit:

```
→ Algorithme SJF avec triage d'urgence séparé
```

**Raisons:**

1. **Efficacité**: Maximise le nombre de patients traités
2. **Flexibilité**: Bon pour opérations standardisées
3. **Coût**: Meilleure utilisation des ressources

## 🔧 Développement

### Ajouter un nouveau scénario

Modifiez `backend/src/benchmark_main.cpp`:

```cpp
case 6: // Nouveau scénario
    config.nom = "Mon Scénario";
    config.dureeSimulationMinutes = 600;
    config.nombreBlocs = 5;
    // ... autres paramètres
    break;
```

### Ajouter une métrique

Modifiez `backend/src/benchmark/AlgorithmComparison.hpp`:

```cpp
struct ResultatSimulation {
    // ... métriques existantes
    double nouvelleMétrique;  // Votre nouvelle métrique
};
```

### Personnaliser le calcul du score

Dans `AlgorithmComparison.hpp`, méthode `calculerScore()`:

```cpp
double calculerScore() const {
    // Votre logique de scoring
    return score;
}
```

## 📚 Documentation Technique

### Architecture

```
backend/
├── src/
│   ├── benchmark_main.cpp          # Point d'entrée du benchmark
│   ├── benchmark/
│   │   └── AlgorithmComparison.hpp # Logique de comparaison
│   ├── simulation/
│   │   ├── SimulationEngine.hpp    # Moteur de simulation
│   │   ├── Statistics.hpp          # Collecte des stats
│   │   └── Scheduler.hpp           # Algorithmes d'ordonnancement
│   └── ...
└── Makefile                        # Compilation
```

### Compilation manuelle

```bash
cd backend
make clean
make benchmark
./bin/automed_benchmark
```

### Exécution dans Docker

```bash
docker compose exec backend ./bin/automed_benchmark 1
```

## 🐛 Dépannage

### Le benchmark ne compile pas

```bash
docker compose exec backend make clean
docker compose exec backend make benchmark
```

### Les résultats ne s'exportent pas

Vérifiez que le dossier `results/` existe:

```bash
docker compose exec backend mkdir -p /app/results
```

### Permissions Docker

Si problème de permissions:

```bash
sudo chown -R $USER:$USER results/
```

## 📊 Exemples de Résultats

### Exemple - Charge Normale

```
╔══════════════════════════════════════════════════════════════════════════╗
║                    📊 RÉSULTATS COMPARATIFS                              ║
╚══════════════════════════════════════════════════════════════════════════╝

┌─────────────┬──────────┬──────────┬──────────┬──────────┬──────────┐
│ Algorithme  │ Patients │ Traités  │ Attente  │ Max      │ Débit    │
│             │  Total   │    (%)   │   moy.   │  (min)   │  (pat/h) │
├─────────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
│ FCFS        │ 26       │ 92.3%    │ 45.2m    │ 120m     │ 3.00     │
│ SJF         │ 26       │ 96.2%    │ 38.7m    │ 98m      │ 3.12     │
│ PRIORITY    │ 26       │ 88.5%    │ 52.1m    │ 145m     │ 2.88     │
└─────────────┴──────────┴──────────┴──────────┴──────────┴──────────┘

🎯 ALGORITHME RECOMMANDÉ: PRIORITY
   Meilleur pour la sécurité des urgences
```

## 🤝 Contribution

Pour améliorer le benchmark:

1. Fork le projet
2. Créez une branche (`git checkout -b feature/amélioration`)
3. Committez vos changements
4. Push et créez une Pull Request

## 📝 License

Projet AutoMed - Simulateur de Blocs Opératoires

---

**Développé avec ❤️ pour optimiser la gestion hospitalière**
