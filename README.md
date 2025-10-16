# Flow Programming Language

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![GitHub](https://img.shields.io/badge/GitHub-anonymodgh%2FFlow-blue)](https://github.com/anonymodgh/Flow)

Flow es un lenguaje de programación multi-paradigma que permite escribir código en Python, JavaScript y C++ en un mismo archivo, ejecutándolos en el orden correcto automáticamente.

## 🚀 Quick Start

```bash
# Compilar Flow
make

# Ejecutar ejemplo
./flow examples/test.fl

# Ver métricas
./flow metrics
```

## 📦 Instalación

### Desde Código Fuente

```bash
git clone https://github.com/anonymodgh/Flow.git
cd Flow
make
make install  # Opcional: instalar system-wide
```

### Requisitos

- g++ con soporte C++17
- Python 3.x
- Node.js

## 🎯 Por Qué Flow

Flow resuelve un problema real: **orquestar múltiples lenguajes en pipelines de datos**.

### El Problema
```bash
# Antes: Scripts separados, difíciles de mantener
python extract.py
node transform.js
./analyze.exe
python cleanup.py
```

### La Solución: Flow
```python
# Un solo archivo, un solo comando
@data

# Python: Extrae datos
data = extract_from_db()
flow_set('data', data)

# JavaScript: Transforma
const data = flowGet('data');
const transformed = await transform(data);
flowSet('result', transformed);

# C++: Analiza (performance crítico)
analyze_fast(data);
```

```bash
flow pipeline.fl  # ¡Eso es todo!
```

## ✨ Características

- **Multi-lenguaje**: Python, JavaScript y C++ en un solo archivo
- **Memoria compartida**: Comunicación rápida con `flow_set()` / `flowGet()`
- **Ejecución paralela**: `@parallel` para máximo performance
- **Fail-fast**: Detiene pipeline automáticamente en errores
- **Multi-archivo**: Organiza código con `import "archivo.fl"`
- **Integración CI/CD**: JUnit XML para GitHub Actions, Jenkins
- **Observabilidad**: Métricas para Prometheus, Datadog, Grafana
- **Orquestación**: XCom para Airflow, Prefect, Dagster
- **Sistema de paquetes**: Como npm, pero para múltiples lenguajes

## 📖 Documentación

- [Quick Start Guide](docs/QUICKSTART.md) - Empieza en 5 minutos
- [Why Flow?](docs/WHY_FLOW.md) - Casos de uso y comparaciones
- [Ecosystem Integration](docs/ECOSYSTEM_INTEGRATION.md) - CI/CD, Airflow, Prometheus
- [Changelog](docs/CHANGELOG.md) - Historial de cambios
- [Contributing](CONTRIBUTING.md) - Cómo contribuir

## 🎓 Ejemplos

### Ejemplo Básico

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

### Ejecución Paralela

```python
@parallel

# Estos tres bloques se ejecutan SIMULTÁNEAMENTE

# Python: I/O (leer archivos)
for file in files:
    data = read_file(file)

# JavaScript: CPU (transformar)
fn transform():
    compute_heavy_task();

# C++: CPU (calcular)
cpp
calculate_intensive();
end
```

### Más Ejemplos

Ver carpeta [`examples/`](examples/) para más ejemplos:
- `test.fl` - Ejemplo básico
- `memory_test.fl` - Memoria compartida
- `parallel_test.fl` - Ejecución paralela
- `advanced_demo.fl` - Pipeline completo
- `neural_pipeline.fl` - ML pipeline

## 🛠️ Comandos

```bash
# Ejecución
flow <archivo.fl>           # Ejecutar archivo
flow init [nombre]          # Crear proyecto nuevo

# Gestión de paquetes
flow install <paquete>      # Instalar paquete
flow install                # Instalar todas las dependencias
flow uninstall <paquete>    # Desinstalar paquete
flow list                   # Listar paquetes instalados

# Utilidades
flow metrics                # Mostrar métricas de ejecución
flow run <script>           # Ejecutar script de flow.json
flow version                # Mostrar versión
flow --help                 # Mostrar ayuda
```

## 🌉 Integración con Ecosistemas

### CI/CD (GitHub Actions)

```yaml
- name: Run Flow tests
  run: flow test.fl

- name: Publish test results
  uses: EnricoMi/publish-unit-test-result-action@v2
  with:
    files: __flow_junit__.xml
```

### Orquestación (Airflow)

```python
run_flow = BashOperator(
    task_id='run_flow',
    bash_command='flow pipeline.fl'
)
```

### Observabilidad (Prometheus)

```bash
python integrations/prometheus_exporter.py &
flow pipeline.fl
```

## 📊 Performance

### Modo Serial (Por Defecto)
- Python → JavaScript → C++
- Tiempo: Suma de todas las etapas

### Modo Paralelo (`@parallel`)
- Python || JavaScript || C++
- Tiempo: Máximo de las tres etapas
- **Speedup: Hasta 3x más rápido**

## 🤝 Contribuir

¡Las contribuciones son bienvenidas! Ver [CONTRIBUTING.md](CONTRIBUTING.md) para más detalles.

```bash
# Fork el repositorio
git clone https://github.com/anonymodgh/Flow.git
cd Flow

# Crear rama
git checkout -b feature/mi-feature

# Hacer cambios y probar
make clean && make && make test

# Commit y push
git commit -m "Add: mi feature"
git push origin feature/mi-feature
```

## 📝 Licencia

MIT License - Ver [LICENSE](LICENSE) para más detalles.

## 🙏 Agradecimientos

- **Gemini AI** por identificar limitaciones y desafiar a Flow a ser mejor
- **Comunidad Open Source** por las herramientas que hacen esto posible

## 📞 Contacto

- **GitHub**: [@anonymodgh](https://github.com/anonymodgh)
- **Repositorio**: [Flow](https://github.com/anonymodgh/Flow)
- **Issues**: [GitHub Issues](https://github.com/anonymodgh/Flow/issues)

---

**Flow v2.0** - Un lenguaje, tres mundos, cero límites. 🚀
