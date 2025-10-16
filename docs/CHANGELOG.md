# Flow Changelog

## v2.0 - Major Update (2025)

### 🎉 Nuevas Características

#### 1. Ejecución Paralela (`@parallel`)
- **Funcionalidad**: Python, JavaScript y C++ se ejecutan simultáneamente
- **Beneficio**: Hasta 3x más rápido para tareas independientes
- **Uso**: Ideal para pipelines productor-consumidor

**Antes:**
```python
# Serial: Python (3s) → JS (3s) → C++ (3s) = 9s total
```

**Ahora:**
```python
@parallel
# Paralelo: Python (3s) || JS (3s) || C++ (3s) = 3s total
# ⚡ 3x más rápido!
```

#### 2. Memoria Compartida
- **Funciones**: `flow_set()` / `flowGet()` / `flowSet()`
- **Beneficio**: Comunicación rápida entre lenguajes sin I/O de disco
- **Uso**: Ideal para datos pequeños/medianos (números, strings, arrays)

**Antes:**
```python
# Lento: Escribir a disco
with open('data.json', 'w') as f:
    json.dump(data, f)
```

**Ahora:**
```python
# Rápido: Memoria compartida
flow_set('data', data)
```

#### 2. Fail-Fast (Detención Automática en Errores)
- **Beneficio**: Pipeline se detiene inmediatamente si una etapa falla
- **Seguridad**: Evita procesar datos corruptos o incompletos
- **Debugging**: Errores claros con stack traces completos

**Ejemplo:**
```
[Python] Executing...
✗ Python Error: Division by zero
✗ Pipeline stopped: Python failed with exit code 1
```

#### 3. Multi-Archivo (Imports)
- **Sintaxis**: `import "archivo.fl"`
- **Beneficio**: Organiza código en módulos reutilizables
- **Uso**: Separa utilidades, configuración, modelos

**Ejemplo:**
```python
# main.fl
import "utils.fl"
import "config.fl"

# Usar funciones importadas
result = calculate_stats(data)
```

#### 4. Manejo de Errores Mejorado
- **Python**: Try-catch automático con traceback completo
- **JavaScript**: Captura errores con stack trace
- **C++**: Captura std::exception

**Beneficio**: No más errores silenciosos

#### 5. Colores en Terminal
- 🔵 Cyan: Acciones en progreso
- 🟢 Green: Operaciones exitosas
- 🔴 Red: Errores
- 🟡 Yellow: Información

#### 6. Sistema de Paquetes Completo
- `flow install <pkg>` - Instalar paquetes
- `flow uninstall <pkg>` - Desinstalar paquetes
- `flow list` - Listar instalados
- `flow run <script>` - Ejecutar scripts

#### 7. flow.json (Configuración de Proyecto)
- Similar a package.json de Node.js
- Define scripts personalizados
- Gestiona dependencias de ambos lenguajes

### 🔧 Mejoras

- **Mejor output**: Etiquetas claras `[Python]`, `[JavaScript]`, `[C++]`
- **Exit codes**: Códigos de salida apropiados para CI/CD
- **Validación**: Verifica que archivos existan antes de ejecutar
- **Limpieza automática**: Elimina archivos temporales

### 🐛 Bugs Corregidos

- Indentación incorrecta en bloques Python vacíos
- Docstrings de Python mezclándose con JavaScript
- Errores de C++ no detenían el pipeline
- Archivos temporales no se limpiaban en errores

### 📚 Documentación

- README completo con ejemplos
- Guía de mejores prácticas
- Comparación con Node.js/NPM
- Ejemplos avanzados incluidos

### ⚠️ Breaking Changes

Ninguno - v2.0 es compatible con código v1.0

### 🎯 Próximas Características (v3.0)

- [x] ~~Ejecución bidireccional~~ ✅ Implementado en v2.0
- [x] ~~Ejecución paralela~~ ✅ Implementado en v2.0
- [ ] Debugging interactivo
- [ ] Soporte para más lenguajes (Rust, Go)
- [ ] Memoria compartida optimizada (sin JSON)
- [ ] Hot reload en desarrollo
- [ ] Testing framework integrado
- [ ] Package registry (flow.dev)
- [ ] Streaming de datos (pipeline continuo)

---

## v1.0 - Initial Release

### Características Iniciales

- Ejecución multi-lenguaje (Python, JavaScript, C++)
- Macros (@data, @web, @ml, @api)
- Sintaxis básica (fn, def, cpp...end)
- Comunicación por archivos
- Comando `flow init`

### Limitaciones v1.0

- Sin variables compartidas
- Sin manejo de errores
- Sin fail-fast
- Sin multi-archivo
- Sin colores en terminal
- Sin sistema de paquetes

---

**Flow v2.0** - Un lenguaje, tres mundos, cero límites.
