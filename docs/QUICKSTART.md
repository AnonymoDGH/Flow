# Flow v2.0 - Quick Start Guide

## 5 Minutos para tu Primer Pipeline

### 1. Instala Flow (30 segundos)

```bash
# Compila Flow
g++ -o flow.exe flow.cpp -std=c++17

# Verifica instalación
flow --version
# Output: Flow v2.0 - Multi-language programming
```

---

### 2. Crea tu Primer Proyecto (30 segundos)

```bash
flow init mi_primer_pipeline
cd mi_primer_pipeline
```

**Archivos creados:**
```
mi_primer_pipeline/
├── main.fl       # Tu código aquí
├── flow.json     # Configuración
├── README.md     # Documentación
└── .gitignore    # Git ignore
```

---

### 3. Escribe tu Primer Pipeline (2 minutos)

Edita `main.fl`:

```python
# main.fl - Mi primer pipeline Flow

@data

print("=" * 50)
print("MI PRIMER PIPELINE FLOW")
print("=" * 50)

# ETAPA 1: Python - Procesa datos
def process_numbers():
    numbers = [1, 2, 3, 4, 5]
    total = sum(numbers)
    
    print(f"\n[Python] Números: {numbers}")
    print(f"[Python] Total: {total}")
    
    # Guardar en memoria compartida
    flow_set('numbers', numbers)
    flow_set('total', total)

process_numbers()

# ETAPA 2: JavaScript - Transforma
fn transform():
    const numbers = flowGet('numbers');
    const total = flowGet('total');
    
    console.log(`\n[JavaScript] Recibido: ${numbers}`);
    console.log(`[JavaScript] Total: ${total}`);
    
    // Duplicar números
    const doubled = numbers.map(n => n * 2);
    const newTotal = doubled.reduce((a, b) => a + b, 0);
    
    console.log(`[JavaScript] Duplicados: ${doubled}`);
    console.log(`[JavaScript] Nuevo total: ${newTotal}`);
    
    flowSet('doubled_total', newTotal);

transform()

# ETAPA 3: C++ - Calcula
cpp
int doubled_total = std::stoi(flowGet("doubled_total", "0"));

std::cout << "\n[C++] Total duplicado: " << doubled_total << std::endl;

// Calcular factorial
long long factorial = 1;
for (int i = 1; i <= 5; ++i) {
    factorial *= i;
}

std::cout << "[C++] Factorial de 5: " << factorial << std::endl;

flowSet("factorial", std::to_string(factorial));
end

# ETAPA 4: Python - Reporte final
def final_report():
    doubled = flow_get('doubled_total')
    factorial = flow_get('factorial')
    
    print("\n" + "=" * 50)
    print("REPORTE FINAL")
    print("=" * 50)
    print(f"Total duplicado: {doubled}")
    print(f"Factorial: {factorial}")
    print("=" * 50)

final_report()

# CLEANUP
# CLEANUP
print("\n[Cleanup] Pipeline completado exitosamente!")
```

---

### 4. Ejecuta tu Pipeline (30 segundos)

```bash
flow main.fl
```

**Output esperado:**
```
→ Running main.fl

[Python] Executing...
==================================================
MI PRIMER PIPELINE FLOW
==================================================

[Python] Números: [1, 2, 3, 4, 5]
[Python] Total: 15

[JavaScript] Executing...

[JavaScript] Recibido: 1,2,3,4,5
[JavaScript] Total: 15
[JavaScript] Duplicados: 2,4,6,8,10
[JavaScript] Nuevo total: 30

[C++] Compiling...
[C++] Executing...

[C++] Total duplicado: 30
[C++] Factorial de 5: 120

[Python] Executing...

==================================================
REPORTE FINAL
==================================================
Total duplicado: 30
Factorial: 120
==================================================

[Cleanup] Executing...

[Cleanup] Pipeline completado exitosamente!

✓ Execution completed
```

---

### 5. Experimenta (2 minutos)

#### Prueba el Fail-Fast

Edita `main.fl` y agrega un error intencional:

```python
def process_numbers():
    numbers = [1, 2, 3, 4, 5]
    total = sum(numbers)
    
    # Error intencional
    raise ValueError("¡Error de prueba!")
    
    flow_set('numbers', numbers)
```

Ejecuta:
```bash
flow main.fl
```

**Output:**
```
[Python] Executing...
✗ Python Error: ¡Error de prueba!
Traceback (most recent call last):
  ...
ValueError: ¡Error de prueba!
✗ Pipeline stopped: Python failed with exit code 1
```

**¡El pipeline se detuvo automáticamente!** JavaScript y C++ no se ejecutaron.

---

## Próximos Pasos

### Aprende Más

1. **Memoria Compartida**: Lee `memory_test.fl`
2. **Multi-Archivo**: Lee `multi_file_test.fl`
3. **Pipeline Avanzado**: Lee `advanced_demo.fl`
4. **ML Pipeline**: Lee `neural_pipeline.fl`

### Instala Paquetes

```bash
# Python
flow install pandas numpy --py

# JavaScript
flow install express axios --js

# Ambos
flow install requests
```

### Usa Scripts

Edita `flow.json`:
```json
{
  "scripts": {
    "start": "flow main.fl",
    "dev": "flow main.fl",
    "test": "flow test.fl"
  }
}
```

Ejecuta:
```bash
flow run start
flow run dev
flow run test
```

---

## Comandos Esenciales

```bash
# Crear proyecto
flow init [nombre]

# Ejecutar archivo
flow archivo.fl

# Instalar paquetes
flow install <paquete>
flow install <paquete> --py
flow install <paquete> --js

# Listar paquetes
flow list

# Ejecutar scripts
flow run <script>

# Ayuda
flow --help

# Versión
flow --version
```

---

## Ejemplos Rápidos

### ETL Simple

```python
@data

# Extract
df = pd.read_csv('data.csv')
flow_set('rows', len(df))

# Transform
const rows = flowGet('rows');
console.log(`Processing ${rows} rows...`);

# Load
cpp
std::cout << "Data loaded successfully" << std::endl;
end
```

### API + Database

```python
@web

# Fetch from API
response = requests.get('https://api.example.com/data')
flow_set('data', response.json())

# Process
const data = flowGet('data');
const processed = data.map(item => transform(item));
flowSet('processed', processed);

# Save to DB
data = flow_get('processed')
save_to_database(data)
```

### ML Training

```python
@ml

# Train
model = train_model(X_train, y_train)
flow_set('accuracy', model.score(X_test, y_test))

# Deploy
const accuracy = flowGet('accuracy');
if (accuracy > 0.95) {
    await deployModel();
}
```

---

## Tips Rápidos

1. **Usa memoria compartida** para datos pequeños (números, strings, arrays)
2. **Usa archivos** para datasets grandes (CSV, JSON)
3. **Usa `# CLEANUP`** para código que debe ejecutarse al final
4. **Usa colores** para identificar errores rápidamente
5. **Usa `import "archivo.fl"`** para organizar código

---

## Troubleshooting

### Error: "flow: command not found"
```bash
# Agrega flow a PATH o usa ruta completa
./flow.exe main.fl
```

### Error: "ModuleNotFoundError: No module named 'pandas'"
```bash
flow install pandas --py
```

### Error: "Cannot find module 'express'"
```bash
flow install express --js
```

### Error: "g++: command not found"
```bash
# Instala MinGW (Windows) o gcc (Linux/Mac)
```

---

## ¿Necesitas Ayuda?

- 📖 **Documentación completa**: `README.md`
- 🎯 **Casos de uso**: `WHY_FLOW.md`
- 📝 **Changelog**: `CHANGELOG.md`
- 💡 **Ejemplos**: `*.fl` en el directorio

---

## ¡Felicidades!

Ya sabes lo básico de Flow v2.0. Ahora:

1. ✅ Crea pipelines multi-lenguaje
2. ✅ Usa memoria compartida
3. ✅ Maneja errores automáticamente
4. ✅ Organiza código en módulos

**¡Empieza a construir!**

```bash
flow init mi_proyecto_real
cd mi_proyecto_real
# ... tu código aquí ...
flow run start
```

---

**Flow v2.0** - De cero a pipeline en 5 minutos.
