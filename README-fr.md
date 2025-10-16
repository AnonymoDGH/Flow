# Flow - Langage de Programmation

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![GitHub](https://img.shields.io/badge/GitHub-AnonymoDGH%2FFlow-blue)](https://github.com/AnonymoDGH/Flow)

**Lire dans d'autres langues: [English](README.md) | [Español](README-es.md) | [中文](README-zh.md) | [日本語](README-ja.md)**

---

Flow est un langage de programmation multi-paradigme qui vous permet d'écrire du code Python, JavaScript et C++ dans un seul fichier, en les exécutant automatiquement dans le bon ordre.

## 🚀 Démarrage Rapide

```bash
# Compiler Flow
make

# Exécuter un exemple
./flow examples/test.fl

# Voir les métriques
./flow metrics
```

## 📦 Installation

### Depuis les Sources

```bash
git clone https://github.com/AnonymoDGH/Flow.git
cd Flow
make
make install  # Optionnel: installation système
```

### Prérequis

- g++ avec support C++17
- Python 3.x
- Node.js

## 🎯 Pourquoi Flow?

Flow résout un problème réel: **orchestrer plusieurs langages dans des pipelines de données**.

### Le Problème
```bash
# Avant: Scripts séparés, difficiles à maintenir
python extract.py
node transform.js
./analyze.exe
python cleanup.py
```

### La Solution: Flow
```python
# Un seul fichier, une seule commande
@data

# Python: Extraire les données
data = extract_from_db()
flow_set('data', data)

# JavaScript: Transformer
const data = flowGet('data');
const transformed = await transform(data);
flowSet('result', transformed);

# C++: Analyser (haute performance)
analyze_fast(data);
```

```bash
flow pipeline.fl  # C'est tout!
```

## ✨ Fonctionnalités

- **Multi-langage**: Python, JavaScript et C++ dans un seul fichier
- **Mémoire partagée**: Communication rapide avec `flow_set()` / `flowGet()`
- **Exécution parallèle**: `@parallel` pour des performances maximales
- **Fail-fast**: Arrête automatiquement le pipeline en cas d'erreur
- **Multi-fichier**: Organisez le code avec `import "fichier.fl"`
- **Intégration CI/CD**: JUnit XML pour GitHub Actions, Jenkins
- **Observabilité**: Métriques pour Prometheus, Datadog, Grafana
- **Orchestration**: XCom pour Airflow, Prefect, Dagster
- **Système de paquets**: Comme npm, mais pour plusieurs langages

## 📖 Documentation

- [Guide de Démarrage Rapide](docs/QUICKSTART.md) - Commencez en 5 minutes
- [Pourquoi Flow?](docs/WHY_FLOW.md) - Cas d'usage et comparaisons
- [Intégration d'Écosystème](docs/ECOSYSTEM_INTEGRATION.md) - CI/CD, Airflow, Prometheus
- [Changelog](docs/CHANGELOG.md) - Historique des versions
- [Contribuer](CONTRIBUTING.md) - Comment contribuer

## 🎓 Exemples

### Exemple de Base

```python
# hello.fl
@data

# Python
def greet(name):
    print(f"Bonjour de Python, {name}!")
    flow_set('name', name)

greet("Monde")

# JavaScript
fn greet_js():
    const name = flowGet('name');
    console.log(`Bonjour de JavaScript, ${name}!`);

greet_js()

# C++
cpp
std::string name = flowGet("name", "Monde");
std::cout << "Bonjour de C++, " << name << "!" << std::endl;
end
```

### Exécution Parallèle

```python
@parallel

# Ces trois blocs s'exécutent SIMULTANÉMENT

# Python: I/O (lire des fichiers)
for file in files:
    data = read_file(file)

# JavaScript: CPU (transformer)
fn transform():
    compute_heavy_task();

# C++: CPU (calculer)
cpp
calculate_intensive();
end
```

## 🛠️ Commandes

```bash
# Exécution
flow <fichier.fl>           # Exécuter un fichier
flow init [nom]             # Créer un nouveau projet

# Gestion des paquets
flow install <paquet>       # Installer un paquet
flow install                # Installer toutes les dépendances
flow uninstall <paquet>     # Désinstaller un paquet
flow list                   # Lister les paquets installés

# Utilitaires
flow metrics                # Afficher les métriques d'exécution
flow run <script>           # Exécuter un script depuis flow.json
flow version                # Afficher la version
flow --help                 # Afficher l'aide
```

## 📊 Performance

### Mode Série (Par Défaut)
- Python → JavaScript → C++
- Temps: Somme de toutes les étapes

### Mode Parallèle (`@parallel`)
- Python || JavaScript || C++
- Temps: Maximum des trois étapes
- **Accélération: Jusqu'à 3x plus rapide**

## 🤝 Contribuer

Les contributions sont les bienvenues! Voir [CONTRIBUTING.md](CONTRIBUTING.md) pour plus de détails.

## 📝 Licence

Licence MIT - Voir [LICENSE](LICENSE) pour plus de détails.

## 📞 Contact

- **GitHub**: [@AnonymoDGH](https://github.com/AnonymoDGH)
- **Dépôt**: [Flow](https://github.com/AnonymoDGH/Flow)
- **Issues**: [GitHub Issues](https://github.com/AnonymoDGH/Flow/issues)

---

**Flow v2.0** - Un langage, trois mondes, zéro limite. 🚀
