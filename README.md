# Flow Programming Language

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![GitHub](https://img.shields.io/badge/GitHub-AnonymoDGH%2FFlow-blue)](https://github.com/AnonymoDGH/Flow)

**[English](#english) | [Español](#español) | [中文](#中文) | [日本語](#日本語) | [Français](#français)**

---

## English

Flow is a multi-paradigm programming language that allows you to write Python, JavaScript, and C++ code in a single file, executing them in the correct order automatically.

### 🚀 Quick Start

```bash
# Build Flow
make

# Run example
./flow examples/test.fl

# View metrics
./flow metrics
```

### ✨ Features

- **Multi-language**: Python, JavaScript, and C++ in one file
- **Shared memory**: Fast communication with `flow_set()` / `flowGet()`
- **Parallel execution**: `@parallel` for maximum performance
- **Fail-fast**: Automatically stops pipeline on errors
- **Multi-file**: Organize code with `import "file.fl"`
- **CI/CD integration**: JUnit XML for GitHub Actions, Jenkins
- **Observability**: Metrics for Prometheus, Datadog, Grafana
- **Orchestration**: XCom for Airflow, Prefect, Dagster

### 📖 Documentation

- [Quick Start Guide](docs/QUICKSTART.md)
- [Why Flow?](docs/WHY_FLOW.md)
- [Ecosystem Integration](docs/ECOSYSTEM_INTEGRATION.md)
- [Contributing](CONTRIBUTING.md)

### 📦 Installation

```bash
git clone https://github.com/AnonymoDGH/Flow.git
cd Flow
make
make install  # Optional: system-wide install
```

---

## Español

Flow es un lenguaje de programación multi-paradigma que permite escribir código en Python, JavaScript y C++ en un mismo archivo, ejecutándolos en el orden correcto automáticamente.

### 🚀 Inicio Rápido

```bash
# Compilar Flow
make

# Ejecutar ejemplo
./flow examples/test.fl

# Ver métricas
./flow metrics
```

### ✨ Características

- **Multi-lenguaje**: Python, JavaScript y C++ en un solo archivo
- **Memoria compartida**: Comunicación rápida con `flow_set()` / `flowGet()`
- **Ejecución paralela**: `@parallel` para máximo rendimiento
- **Fail-fast**: Detiene el pipeline automáticamente en errores
- **Multi-archivo**: Organiza código con `import "archivo.fl"`
- **Integración CI/CD**: JUnit XML para GitHub Actions, Jenkins
- **Observabilidad**: Métricas para Prometheus, Datadog, Grafana
- **Orquestación**: XCom para Airflow, Prefect, Dagster

### 📖 Documentación

- [Guía de Inicio Rápido](docs/QUICKSTART.md)
- [¿Por Qué Flow?](docs/WHY_FLOW.md)
- [Integración con Ecosistemas](docs/ECOSYSTEM_INTEGRATION.md)
- [Contribuir](CONTRIBUTING.md)

### 📦 Instalación

```bash
git clone https://github.com/AnonymoDGH/Flow.git
cd Flow
make
make install  # Opcional: instalación global
```

---

## 中文

Flow 是一种多范式编程语言，允许您在单个文件中编写 Python、JavaScript 和 C++ 代码，并自动按正确顺序执行它们。

### 🚀 快速开始

```bash
# 构建 Flow
make

# 运行示例
./flow examples/test.fl

# 查看指标
./flow metrics
```

### ✨ 特性

- **多语言**: 在一个文件中使用 Python、JavaScript 和 C++
- **共享内存**: 使用 `flow_set()` / `flowGet()` 快速通信
- **并行执行**: 使用 `@parallel` 获得最大性能
- **快速失败**: 出错时自动停止管道
- **多文件**: 使用 `import "file.fl"` 组织代码
- **CI/CD 集成**: 为 GitHub Actions、Jenkins 提供 JUnit XML
- **可观测性**: 为 Prometheus、Datadog、Grafana 提供指标
- **编排**: 为 Airflow、Prefect、Dagster 提供 XCom

### 📖 文档

- [快速入门指南](docs/QUICKSTART.md)
- [为什么选择 Flow?](docs/WHY_FLOW.md)
- [生态系统集成](docs/ECOSYSTEM_INTEGRATION.md)
- [贡献指南](CONTRIBUTING.md)

### 📦 安装

```bash
git clone https://github.com/AnonymoDGH/Flow.git
cd Flow
make
make install  # 可选：系统级安装
```

---

## 日本語

Flow は、Python、JavaScript、C++ のコードを 1 つのファイルに記述し、自動的に正しい順序で実行できるマルチパラダイムプログラミング言語です。

### 🚀 クイックスタート

```bash
# Flow をビルド
make

# サンプルを実行
./flow examples/test.fl

# メトリクスを表示
./flow metrics
```

### ✨ 機能

- **マルチ言語**: 1 つのファイルで Python、JavaScript、C++ を使用
- **共有メモリ**: `flow_set()` / `flowGet()` による高速通信
- **並列実行**: `@parallel` で最大のパフォーマンス
- **フェイルファスト**: エラー時にパイプラインを自動停止
- **マルチファイル**: `import "file.fl"` でコードを整理
- **CI/CD 統合**: GitHub Actions、Jenkins 用の JUnit XML
- **可観測性**: Prometheus、Datadog、Grafana 用のメトリクス
- **オーケストレーション**: Airflow、Prefect、Dagster 用の XCom

### 📖 ドキュメント

- [クイックスタートガイド](docs/QUICKSTART.md)
- [なぜ Flow?](docs/WHY_FLOW.md)
- [エコシステム統合](docs/ECOSYSTEM_INTEGRATION.md)
- [貢献](CONTRIBUTING.md)

### 📦 インストール

```bash
git clone https://github.com/AnonymoDGH/Flow.git
cd Flow
make
make install  # オプション: システム全体にインストール
```

---

## Français

Flow est un langage de programmation multi-paradigme qui vous permet d'écrire du code Python, JavaScript et C++ dans un seul fichier, en les exécutant automatiquement dans le bon ordre.

### 🚀 Démarrage Rapide

```bash
# Compiler Flow
make

# Exécuter un exemple
./flow examples/test.fl

# Voir les métriques
./flow metrics
```

### ✨ Fonctionnalités

- **Multi-langage**: Python, JavaScript et C++ dans un seul fichier
- **Mémoire partagée**: Communication rapide avec `flow_set()` / `flowGet()`
- **Exécution parallèle**: `@parallel` pour des performances maximales
- **Fail-fast**: Arrête automatiquement le pipeline en cas d'erreur
- **Multi-fichier**: Organisez le code avec `import "fichier.fl"`
- **Intégration CI/CD**: JUnit XML pour GitHub Actions, Jenkins
- **Observabilité**: Métriques pour Prometheus, Datadog, Grafana
- **Orchestration**: XCom pour Airflow, Prefect, Dagster

### 📖 Documentation

- [Guide de Démarrage Rapide](docs/QUICKSTART.md)
- [Pourquoi Flow?](docs/WHY_FLOW.md)
- [Intégration d'Écosystème](docs/ECOSYSTEM_INTEGRATION.md)
- [Contribuer](CONTRIBUTING.md)

### 📦 Installation

```bash
git clone https://github.com/AnonymoDGH/Flow.git
cd Flow
make
make install  # Optionnel: installation système
```

---

## 📚 Complete Documentation

### Core Concepts
- **Multi-language execution**: Write Python, JavaScript, and C++ in one file
- **Shared memory**: Fast inter-language communication
- **Parallel mode**: Execute languages simultaneously
- **Fail-fast**: Stop on first error

### Example

```python
# pipeline.fl
@data

# Python: Extract data
data = extract_from_db()
flow_set('data', data)

# JavaScript: Transform
const data = flowGet('data');
const transformed = await transform(data);
flowSet('result', transformed);

# C++: Analyze (high performance)
cpp
analyze_fast(data);
end
```

```bash
flow pipeline.fl  # That's it!
```

## 🛠️ Commands

```bash
# Execution
flow <file.fl>              # Run file
flow init [name]            # Create new project

# Package management
flow install <package>      # Install package
flow install                # Install all dependencies
flow uninstall <package>    # Uninstall package
flow list                   # List installed packages

# Utilities
flow metrics                # Show execution metrics
flow run <script>           # Run script from flow.json
flow version                # Show version
flow --help                 # Show help
```

## 🤝 Contributing

Contributions are welcome! See [CONTRIBUTING.md](CONTRIBUTING.md) for details.

## 📝 License

MIT License - See [LICENSE](LICENSE) for details.

## 📞 Contact

- **GitHub**: [@AnonymoDGH](https://github.com/AnonymoDGH)
- **Repository**: [Flow](https://github.com/AnonymoDGH/Flow)
- **Issues**: [GitHub Issues](https://github.com/AnonymoDGH/Flow/issues)

---

**Flow v2.0** - One language, three worlds, zero limits. 🚀
