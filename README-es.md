# Flow - Lenguaje de Programación

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![GitHub](https://img.shields.io/badge/GitHub-AnonymoDGH%2FFlow-blue)](https://github.com/AnonymoDGH/Flow)

**Leer en otros idiomas: [English](README.md) | [中文](README-zh.md) | [日本語](README-ja.md) | [Français](README-fr.md)**

---

Flow es un lenguaje de programación multi-paradigma que permite escribir código en Python, JavaScript y C++ en un mismo archivo, ejecutándolos en el orden correcto automáticamente.

## 🚀 Inicio Rápido

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
git clone https://github.com/AnonymoDGH/Flow.git
cd Flow
make
make install  # Opcional: instalación global
```

### Requisitos

- g++ con soporte C++17
- Python 3.x
- Node.js

## 🎯 ¿Por Qué Flow?

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

# C++: Analiza (alto rendimiento)
analyze_fast(data);
```

```bash
flow pipeline.fl  # ¡Eso es todo!
```

## ✨ Características

- **Multi-lenguaje**: Python, JavaScript y C++ en un solo archivo
- **Memoria compartida**: Comunicación rápida con `flow_set()` / `flowGet()`
- **Ejecución paralela**: `@parallel` para máximo rendimiento
- **Fail-fast**: Detiene el pipeline automáticamente en errores
- **Multi-archivo**: Organiza código con `import "archivo.fl"`
- **Integración CI/CD**: JUnit XML para GitHub Actions, Jenkins
- **Observabilidad**: Métricas para Prometheus, Datadog, Grafana
- **Orquestación**: XCom para Airflow, Prefect, Dagster
- **Sistema de paquetes**: Como npm, pero para múltiples lenguajes

## 📖 Documentación

- [Guía de Inicio Rápido](docs/QUICKSTART.md) - Empieza en 5 minutos
- [¿Por Qué Flow?](docs/WHY_FLOW.md) - Casos de uso y comparaciones
- [Integración con Ecosistemas](docs/ECOSYSTEM_INTEGRATION.md) - CI/CD, Airflow, Prometheus
- [Changelog](docs/CHANGELOG.md) - Historial de versiones
- [Contribuir](CONTRIBUTING.md) - Cómo contribuir

## 🎓 Ejemplos

### Ejemplo Básico

```python
# hello.fl
@data

# Python
def greet(name):
    print(f"¡Hola desde Python, {name}!")
    flow_set('name', name)

greet("Mundo")

# JavaScript
fn greet_js():
    const name = flowGet('name');
    console.log(`¡Hola desde JavaScript, ${name}!`);

greet_js()

# C++
cpp
std::string name = flowGet("name", "Mundo");
std::cout << "¡Hola desde C++, " << name << "!" << std::endl;
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

## 📊 Rendimiento

### Modo Serial (Por Defecto)
- Python → JavaScript → C++
- Tiempo: Suma de todas las etapas

### Modo Paralelo (`@parallel`)
- Python || JavaScript || C++
- Tiempo: Máximo de las tres etapas
- **Aceleración: Hasta 3x más rápido**

## 🤝 Contribuir

¡Las contribuciones son bienvenidas! Ver [CONTRIBUTING.md](CONTRIBUTING.md) para más detalles.

## 📝 Licencia

Licencia MIT - Ver [LICENSE](LICENSE) para más detalles.

## 📞 Contacto

- **GitHub**: [@AnonymoDGH](https://github.com/AnonymoDGH)
- **Repositorio**: [Flow](https://github.com/AnonymoDGH/Flow)
- **Issues**: [GitHub Issues](https://github.com/AnonymoDGH/Flow/issues)

---

**Flow v2.0** - Un lenguaje, tres mundos, cero límites. 🚀
