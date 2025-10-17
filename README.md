# Flow Programming Language

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![GitHub](https://img.shields.io/badge/GitHub-AnonymoDGH%2FFlow-blue)](https://github.com/AnonymoDGH/Flow)
[![Status: Prototype](https://img.shields.io/badge/Status-Educational%20Prototype-orange)](https://github.com/AnonymoDGH/Flow)

**Read this in other languages: [Español](README-es.md) | [中文](README-zh.md) | [日本語](README-ja.md) | [Français](README-fr.md)**

---

Flow is an **educational prototype** that demonstrates multi-language orchestration by allowing you to write Python, JavaScript, and C++ code in a single file.

> ⚠️ **IMPORTANT**: Flow is a proof-of-concept for learning and experimentation. It is **NOT production-ready**. See [Production Readiness](#-production-readiness) section below.

## 🚀 Quick Start

```bash
# Build Flow
make

# Run example (sequential mode - safe)
./flow examples/test.fl

# ⚠️ IMPORTANT: Read this first
# See QUICK_REFERENCE.md for safe usage patterns
```

**⚠️ Before using Flow, read**: [QUICK_REFERENCE.md](QUICK_REFERENCE.md) - What to use and what to avoid

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

Flow is an **educational project** that explores multi-language orchestration.

### The Concept
```bash
# Traditional approach: Multiple scripts
python extract.py
node transform.js
./analyze.exe
python cleanup.py
```

### Flow's Approach
```python
# One file, sequential execution
@data

# Python: Extract data
data = extract_from_db()
print("Data extracted")

# JavaScript: Transform
fn transform():
    console.log('Data transformed');

transform()

# C++: Analyze
cpp
std::cout << "Data analyzed" << std::endl;
end
```

```bash
flow pipeline.fl  # Simple execution
```

### Reality Check

**What Flow does well**:
- Demonstrates multi-language integration concept
- Simplifies sequential script execution
- Good for learning and prototyping

**What Flow doesn't do**:
- ❌ Reliable shared memory (race conditions)
- ❌ Safe parallel execution (data corruption)
- ❌ Production-grade error handling
- ❌ Security hardening for untrusted code

**For production use**, consider:
- **Airflow/Prefect**: Mature workflow orchestration
- **Docker Compose**: Container-based multi-language pipelines
- **Polyglot notebooks**: Jupyter with multiple kernels
- **gRPC/ZeroMQ**: Proper inter-process communication

## ✨ Features

### ✅ Working Features
- **Multi-language execution**: Python, JavaScript, and C++ in one file
- **Sequential execution**: Reliable execution order
- **Basic IPC**: File-based communication between languages
- **Error handling**: Pipeline stops on first error
- **Multi-file support**: Organize code with `import "file.fl"`
- **Command injection protection**: Basic sanitization implemented

### ⚠️ Experimental Features (Known Issues)
- **Shared memory** (`flow_set()`/`flowGet()`): Race conditions cause data loss/corruption
- **Parallel execution** (`@parallel`): Not thread-safe, causes unpredictable behavior
- **Package system**: Basic implementation, not tested at scale
- **CI/CD integration**: Works but limited error reporting

### ❌ Not Implemented
- **Transaction support**: No rollback on failures
- **Proper IPC**: Uses JSON files instead of message queues
- **Security hardening**: Basic protections only
- **Production monitoring**: Metrics are basic

## 📖 Documentation

### Getting Started
- [Quick Start Guide](docs/QUICKSTART.md) - Get started in 5 minutes
- [Why Flow?](docs/WHY_FLOW.md) - Use cases and comparisons

### Critical Information ⚠️
- [Testing Results](TESTING_RESULTS.md) - **READ THIS FIRST** - Test results and failure rates
- [Race Condition Explained](RACE_CONDITION_EXPLAINED.md) - Why shared memory fails (visual guide)
- [Security & Limitations](SECURITY_AND_LIMITATIONS.md) - Security analysis and known issues
- [Production Readiness](PRODUCTION_READINESS.md) - Honest assessment of production readiness

### Integration & Development
- [Ecosystem Integration](docs/ECOSYSTEM_INTEGRATION.md) - CI/CD, Airflow, Prometheus
- [Changelog](docs/CHANGELOG.md) - Version history
- [Contributing](CONTRIBUTING.md) - How to contribute

## 🎓 Examples

### Basic Example (✅ Reliable)

```python
# hello.fl
@data

# Python
def greet(name):
    print(f"Hello from Python, {name}!")

greet("World")

# JavaScript
fn greet_js():
    console.log('Hello from JavaScript!');

greet_js()

# C++
cpp
std::cout << "Hello from C++!" << std::endl;
end
```

### Shared Memory Example (⚠️ Has Race Conditions)

```python
# memory_test.fl
@data

# Python
flow_set('counter', 0)
print(f"Python set: {flow_get('counter')}")

# JavaScript
fn test():
    let val = flowGet('counter');
    flowSet('counter', val + 1);
    console.log('JS incremented:', flowGet('counter'));

test()

# C++
cpp
int val = std::stoi(flowGet("counter", "0"));
flowSet("counter", std::to_string(val + 1));
std::cout << "C++ incremented: " << flowGet("counter", "0") << std::endl;
end

# ⚠️ WARNING: Results may be inconsistent due to race conditions
# Expected: 0, 1, 2
# Actual: May show 0, 0, 1 or other incorrect values
```

### Parallel Execution (⚠️ Experimental)

```python
@parallel

# ⚠️ WARNING: Parallel mode has known race conditions
# Use only for independent tasks that don't share data

# Python: Independent task
print("Python task")

# JavaScript: Independent task
fn task():
    console.log('JavaScript task');

task()

# C++: Independent task
cpp
std::cout << "C++ task" << std::endl;
end
```

### More Examples

See [`examples/`](examples/) folder for more:
- `test.fl` - Basic sequential execution (✅ Reliable)
- `memory_test.fl` - Shared memory (⚠️ Race conditions)
- `parallel_test.fl` - Parallel execution (⚠️ Not thread-safe)
- `security_test.fl` - Security protections test
- `advanced_demo.fl` - Complete pipeline example

## 🧪 Testing & Known Issues

### Test Results

```bash
# Sequential execution: ✅ PASS
./flow examples/test.fl
# Result: Executes in correct order

# Shared memory: ❌ FAIL (Race conditions)
./flow examples/memory_test.fl
# Expected: counter = 0, 1, 2
# Actual: counter = 0, 0, 1 (data loss)

# Parallel execution: ❌ FAIL (Data corruption)
./flow examples/parallel_test.fl
# Result: Unpredictable, data corruption

# Security: ✅ PASS (Basic protection)
./flow examples/security_test.fl
# Result: Command injection blocked
```

### Root Cause Analysis

The shared memory system uses JSON files without proper locking:

```cpp
// Current implementation (UNSAFE)
void flowSet(key, value) {
    json data = read_json("__flow_shared__.json");  // Read
    data[key] = value;                               // Modify
    write_json("__flow_shared__.json", data);        // Write
}
// Problem: Another process can read/write between these steps
```

**Why it fails**:
1. Process A reads file: `{counter: 0}`
2. Process B reads file: `{counter: 0}` (same value!)
3. Process A writes: `{counter: 1}`
4. Process B writes: `{counter: 1}` (overwrites A's change!)
5. Result: Lost update, counter should be 2 but is 1

**What's needed**:
- File locking (flock/fcntl)
- Atomic operations
- Or better: Replace with proper IPC (ZeroMQ, Redis, etc.)

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

### Serial Mode (Default) - ✅ Reliable
- Python → JavaScript → C++
- Time: Sum of all stages
- **Use this mode for actual work**

### Parallel Mode (`@parallel`) - ⚠️ Experimental
- Python || JavaScript || C++
- **Known Issues**:
  - Race conditions in shared memory
  - Data corruption possible
  - Unpredictable results
- **Not recommended for production use**

## 🚨 Production Readiness

**Current Status**: Educational Prototype

### ✅ Appropriate For:
- Learning about multi-language orchestration
- Personal research scripts
- Algorithm prototyping
- Educational demonstrations

### ❌ NOT Appropriate For:
- Production applications
- Multi-user systems
- Sensitive data processing
- Mission-critical pipelines
- Financial or healthcare systems

### Known Critical Issues:

1. **Race Conditions in IPC**
   - File-based shared memory is not thread-safe
   - Parallel execution causes data corruption
   - No atomic operations or locks
   - **Impact**: Data loss, incorrect results

2. **Security Limitations**
   - Basic command injection protection only
   - No sandboxing or isolation
   - Executes code with full system privileges
   - **Impact**: Potential security vulnerabilities

3. **No Transaction Support**
   - No rollback on failures
   - Partial execution leaves inconsistent state
   - **Impact**: Data integrity issues

4. **Limited Error Handling**
   - Basic error propagation
   - No retry mechanisms
   - Limited debugging information
   - **Impact**: Hard to troubleshoot failures

### Path to Production:

To make Flow production-ready would require:
- **Parser**: Replace regex with ANTLR/Bison (~3-4 months)
- **IPC**: Implement ZeroMQ or gRPC (~2-3 months)
- **Security**: Add sandboxing, proper validation (~2-3 months)
- **Transactions**: ACID guarantees, rollback support (~3-4 months)
- **Testing**: Comprehensive test suite (~2-3 months)
- **Documentation**: Production-grade docs (~1-2 months)

**Estimated time**: 10-18 months of full-time development

For detailed analysis, see:
- [SECURITY_AND_LIMITATIONS.md](SECURITY_AND_LIMITATIONS.md)
- [PRODUCTION_READINESS.md](PRODUCTION_READINESS.md)

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

## ❓ FAQ

### Can I use Flow in production?

**No, not recommended.** Flow is an educational prototype with known critical issues (race conditions, limited security, no transactions). For production, use mature tools like Airflow, Prefect, or Docker Compose.

### Why does shared memory fail?

Flow uses JSON files without proper locking. Multiple processes can read/write simultaneously, causing race conditions and data loss. This is a fundamental architectural limitation.

### Is parallel mode safe?

**No.** Parallel execution has race conditions that cause data corruption. Use sequential mode (`@data`) for reliable results.

### Will these issues be fixed?

Fixing them requires a complete rewrite:
- Replace file-based IPC with ZeroMQ/gRPC
- Add proper locking mechanisms
- Implement transaction support
- Add security sandboxing

This would take 10-18 months of full-time development.

### What should I use Flow for?

Flow is great for:
- Learning about multi-language orchestration
- Quick prototypes and experiments
- Educational demonstrations
- Personal research scripts

### What are the alternatives?

For production use:
- **Airflow/Prefect/Dagster**: Workflow orchestration
- **Docker Compose**: Multi-container pipelines
- **Polyglot notebooks**: Jupyter with multiple kernels
- **Shell scripts + proper IPC**: ZeroMQ, Redis, RabbitMQ

### Can I contribute?

Yes! See [CONTRIBUTING.md](CONTRIBUTING.md). Focus areas:
- Improving documentation
- Adding test cases
- Fixing bugs in sequential mode
- Creating educational examples

## 📝 License

MIT License - See [LICENSE](LICENSE) for details.

## 📞 Contact

- **GitHub**: [@AnonymoDGH](https://github.com/AnonymoDGH)
- **Repository**: [Flow](https://github.com/AnonymoDGH/Flow)
- **Issues**: [GitHub Issues](https://github.com/AnonymoDGH/Flow/issues)

---

**Flow v2.0** - An educational prototype for multi-language orchestration. 🎓

**Status**: Proof of concept - Not production ready - Use at your own risk
