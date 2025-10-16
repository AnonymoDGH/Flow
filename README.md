# Flow Programming Language

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![GitHub](https://img.shields.io/badge/GitHub-AnonymoDGH%2FFlow-blue)](https://github.com/AnonymoDGH/Flow)

**Read this in other languages: [Español](README-es.md) | [中文](README-zh.md) | [日本語](README-ja.md) | [Français](README-fr.md)**

---

Flow is a multi-paradigm programming language that allows you to write Python, JavaScript, and C++ code in a single file, executing them in the correct order automatically.

## 🚀 Quick Start

```bash
# Build Flow
make

# Run example
./flow examples/test.fl

# View metrics
./flow metrics
```

## 📦 Installation

### From Source

```bash
git clone https://github.com/AnonymoDGH/Flow.git
cd Flow
make
make install  # Optional: system-wide install
```

### Requirements

- g++ with C++17 support
- Python 3.x
- Node.js

## 🎯 Why Flow?

Flow solves a real problem: **orchestrating multiple languages in data pipelines**.

### The Problem
```bash
# Before: Separate scripts, hard to maintain
python extract.py
node transform.js
./analyze.exe
python cleanup.py
```

### The Solution: Flow
```python
# One file, one command
@data

# Python: Extract data
data = extract_from_db()
flow_set('data', data)

# JavaScript: Transform
const data = flowGet('data');
const transformed = await transform(data);
flowSet('result', transformed);

# C++: Analyze (high performance)
analyze_fast(data);
```

```bash
flow pipeline.fl  # That's it!
```

## ✨ Features

- **Multi-language**: Python, JavaScript, and C++ in one file
- **Shared memory**: Fast communication with `flow_set()` / `flowGet()`
- **Parallel execution**: `@parallel` for maximum performance
- **Fail-fast**: Automatically stops pipeline on errors
- **Multi-file**: Organize code with `import "file.fl"`
- **CI/CD integration**: JUnit XML for GitHub Actions, Jenkins
- **Observability**: Metrics for Prometheus, Datadog, Grafana
- **Orchestration**: XCom for Airflow, Prefect, Dagster
- **Package system**: Like npm, but for multiple languages

## 📖 Documentation

- [Quick Start Guide](docs/QUICKSTART.md) - Get started in 5 minutes
- [Why Flow?](docs/WHY_FLOW.md) - Use cases and comparisons
- [Ecosystem Integration](docs/ECOSYSTEM_INTEGRATION.md) - CI/CD, Airflow, Prometheus
- [Changelog](docs/CHANGELOG.md) - Version history
- [Contributing](CONTRIBUTING.md) - How to contribute

## 🎓 Examples

### Basic Example

```python
# hello.fl
@data

# Python
def greet(name):
    print(f"Hello from Python, {name}!")
    flow_set('name', name)

greet("World")

# JavaScript
fn greet_js():
    const name = flowGet('name');
    console.log(`Hello from JavaScript, ${name}!`);

greet_js()

# C++
cpp
std::string name = flowGet("name", "World");
std::cout << "Hello from C++, " << name << "!" << std::endl;
end
```

### Parallel Execution

```python
@parallel

# These three blocks execute SIMULTANEOUSLY

# Python: I/O (read files)
for file in files:
    data = read_file(file)

# JavaScript: CPU (transform)
fn transform():
    compute_heavy_task();

# C++: CPU (calculate)
cpp
calculate_intensive();
end
```

### More Examples

See [`examples/`](examples/) folder for more:
- `test.fl` - Basic example
- `memory_test.fl` - Shared memory
- `parallel_test.fl` - Parallel execution
- `advanced_demo.fl` - Complete pipeline
- `neural_pipeline.fl` - ML pipeline

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

## 🌉 Ecosystem Integration

### CI/CD (GitHub Actions)

```yaml
- name: Run Flow tests
  run: flow test.fl

- name: Publish test results
  uses: EnricoMi/publish-unit-test-result-action@v2
  with:
    files: __flow_junit__.xml
```

### Orchestration (Airflow)

```python
run_flow = BashOperator(
    task_id='run_flow',
    bash_command='flow pipeline.fl'
)
```

### Observability (Prometheus)

```bash
python integrations/prometheus_exporter.py &
flow pipeline.fl
```

## 📊 Performance

### Serial Mode (Default)
- Python → JavaScript → C++
- Time: Sum of all stages

### Parallel Mode (`@parallel`)
- Python || JavaScript || C++
- Time: Maximum of the three stages
- **Speedup: Up to 3x faster**

## 🤝 Contributing

Contributions are welcome! See [CONTRIBUTING.md](CONTRIBUTING.md) for details.

```bash
# Fork the repository
git clone https://github.com/AnonymoDGH/Flow.git
cd Flow

# Create branch
git checkout -b feature/my-feature

# Make changes and test
make clean && make && make test

# Commit and push
git commit -m "Add: my feature"
git push origin feature/my-feature
```

## 📝 License

MIT License - See [LICENSE](LICENSE) for details.

## 📞 Contact

- **GitHub**: [@AnonymoDGH](https://github.com/AnonymoDGH)
- **Repository**: [Flow](https://github.com/AnonymoDGH/Flow)
- **Issues**: [GitHub Issues](https://github.com/AnonymoDGH/Flow/issues)

---

**Flow v2.0** - One language, three worlds, zero limits. 🚀
