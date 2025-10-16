# Flow v2.0 - Integración con Ecosistemas Externos

## Resumen

Flow v2.0 incluye **puentes nativos** para integrarse con herramientas empresariales estándar:

- ✅ **CI/CD**: JUnit XML para GitHub Actions, Jenkins, GitLab CI
- ✅ **Orquestación**: XCom para Airflow, Prefect, Dagster
- ✅ **Observabilidad**: Métricas JSON para Prometheus, Datadog, Grafana

---

## 1. Integración con CI/CD

### GitHub Actions

Flow genera automáticamente `__flow_junit__.xml` con resultados de tests:

```yaml
# .github/workflows/flow-ci.yml
name: Flow CI

on: [push]

jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Run Flow tests
        run: ./flow test.fl
      
      - name: Publish test results
        uses: EnricoMi/publish-unit-test-result-action@v2
        with:
          files: __flow_junit__.xml
```

### Jenkins

```groovy
pipeline {
    agent any
    stages {
        stage('Test') {
            steps {
                sh 'flow test.fl'
            }
        }
        stage('Results') {
            steps {
                junit '__flow_junit__.xml'
            }
        }
    }
}
```

### GitLab CI

```yaml
test:
  script:
    - flow test.fl
  artifacts:
    reports:
      junit: __flow_junit__.xml
```

---

## 2. Integración con Orquestadores

### Apache Airflow

Flow exporta resultados en `__flow_xcom__.json` para XCom:

```python
from airflow import DAG
from airflow.operators.bash import BashOperator
from airflow.operators.python import PythonOperator

# Ejecutar Flow
run_flow = BashOperator(
    task_id='run_flow',
    bash_command='flow pipeline.fl'
)

# Extraer resultados
def extract_results(**context):
    with open('__flow_xcom__.json') as f:
        results = json.load(f)
    
    # Push a XCom
    context['ti'].xcom_push(
        key='records_processed',
        value=results['records_processed']
    )

extract = PythonOperator(
    task_id='extract',
    python_callable=extract_results
)

run_flow >> extract
```

**En tu archivo Flow:**

```python
# pipeline.fl

# Procesar datos
records = process_data()

# Exportar para Airflow
import json
with open('__flow_xcom__.json', 'w') as f:
    json.dump({
        'records_processed': len(records),
        'status': 'success'
    }, f)
```

### Prefect

```python
from prefect import flow, task
import subprocess
import json

@task
def run_flow_pipeline():
    subprocess.run(['flow', 'pipeline.fl'], check=True)
    
    with open('__flow_xcom__.json') as f:
        return json.load(f)

@flow
def main_flow():
    results = run_flow_pipeline()
    print(f"Processed {results['records_processed']} records")
```

---

## 3. Integración con Observabilidad

### Métricas Nativas

Flow genera `__flow_metrics__.json` con métricas de cada etapa:

```json
{"stage":"python","duration":2.5,"exit_code":0,"timestamp":1704067200}
{"stage":"javascript","duration":1.2,"exit_code":0,"timestamp":1704067203}
{"stage":"cpp","duration":0.8,"exit_code":0,"timestamp":1704067204}
```

**Ver métricas:**
```bash
flow metrics
```

**Output:**
```
Stage Performance:
─────────────────────────────────────
  python: 2.5s ✓
  javascript: 1.2s ✓
  cpp: 0.8s ✓
─────────────────────────────────────
Total: 4.5s
```

### Prometheus

Usa el exportador incluido:

```bash
python prometheus_exporter.py
```

Métricas disponibles en `http://localhost:8000/metrics`:

```
# HELP flow_stage_duration_seconds Duration of Flow stages
# TYPE flow_stage_duration_seconds histogram
flow_stage_duration_seconds_bucket{stage="python",le="1.0"} 0
flow_stage_duration_seconds_bucket{stage="python",le="2.5"} 1
flow_stage_duration_seconds_sum{stage="python"} 2.5
flow_stage_duration_seconds_count{stage="python"} 1

# HELP flow_pipeline_duration_seconds Total duration of Flow pipeline
# TYPE flow_pipeline_duration_seconds gauge
flow_pipeline_duration_seconds 4.5
```

**Grafana Dashboard:**

```json
{
  "dashboard": {
    "title": "Flow Pipeline Metrics",
    "panels": [
      {
        "title": "Stage Duration",
        "targets": [
          {
            "expr": "flow_stage_duration_seconds_sum"
          }
        ]
      },
      {
        "title": "Success Rate",
        "targets": [
          {
            "expr": "rate(flow_stage_success_total[5m])"
          }
        ]
      }
    ]
  }
}
```

### Datadog

```python
from datadog import initialize, statsd
import json

initialize(api_key='YOUR_API_KEY')

# Leer métricas de Flow
with open('__flow_metrics__.json') as f:
    for line in f:
        metric = json.loads(line)
        
        # Enviar a Datadog
        statsd.histogram(
            'flow.stage.duration',
            metric['duration'],
            tags=[f"stage:{metric['stage']}"]
        )
        
        if metric['exit_code'] == 0:
            statsd.increment('flow.stage.success', tags=[f"stage:{metric['stage']}"])
        else:
            statsd.increment('flow.stage.failure', tags=[f"stage:{metric['stage']}"])
```

---

## 4. Patrones de Integración

### Patrón 1: Flow como Tarea en Pipeline Mayor

```yaml
# Airflow DAG
extract_data >> run_flow >> validate_results >> deploy

# Flow es UNA tarea en un pipeline más grande
```

**Ventajas:**
- ✅ Flow maneja complejidad multi-lenguaje
- ✅ Airflow maneja orquestación y scheduling
- ✅ Cada herramienta hace lo que mejor sabe

### Patrón 2: Flow con Monitoreo Externo

```python
# Flow ejecuta el pipeline
flow pipeline.fl

# Prometheus scrapes métricas
# Grafana visualiza
# Alertmanager notifica si falla
```

**Ventajas:**
- ✅ Observabilidad completa
- ✅ Alertas automáticas
- ✅ Dashboards en tiempo real

### Patrón 3: Flow en CI/CD

```yaml
# GitHub Actions
test:
  - flow test.fl
  - Publish JUnit results
  - Upload metrics
  - Deploy if passed
```

**Ventajas:**
- ✅ Tests automáticos
- ✅ Resultados estructurados
- ✅ Integración nativa con GitHub

---

## 5. Archivos de Integración

Flow genera estos archivos para integración:

| Archivo | Propósito | Formato | Consumidor |
|---------|-----------|---------|------------|
| `__flow_junit__.xml` | Resultados de tests | JUnit XML | CI/CD (GitHub, Jenkins) |
| `__flow_metrics__.json` | Métricas de performance | JSON Lines | Prometheus, Datadog |
| `__flow_xcom__.json` | Datos para orquestador | JSON | Airflow, Prefect |

**Estos archivos NO se eliminan automáticamente** para que herramientas externas puedan leerlos.

---

## 6. Ejemplo Completo: Pipeline Empresarial

```python
# pipeline.fl - Flow pipeline
@data

# Procesar datos
df = pd.read_csv('input.csv')
processed = process_data(df)

# Exportar métricas de negocio
import json
with open('__flow_xcom__.json', 'w') as f:
    json.dump({
        'records_processed': len(processed),
        'errors': 0,
        'status': 'success'
    }, f)

print(f"Processed {len(processed)} records")
```

```python
# airflow_dag.py - Orquestación
from airflow import DAG

with DAG('enterprise_pipeline') as dag:
    # Tarea 1: Extraer de DB
    extract = PostgresOperator(...)
    
    # Tarea 2: Flow procesa multi-lenguaje
    process = BashOperator(
        task_id='flow_process',
        bash_command='flow pipeline.fl'
    )
    
    # Tarea 3: Validar resultados
    validate = PythonOperator(...)
    
    # Tarea 4: Cargar a warehouse
    load = BigQueryOperator(...)
    
    extract >> process >> validate >> load
```

```python
# prometheus_exporter.py - Observabilidad
# Exporta métricas de Flow a Prometheus
# Grafana visualiza en tiempo real
# Alertmanager notifica si falla
```

**Resultado:**
- ✅ Flow maneja complejidad multi-lenguaje
- ✅ Airflow maneja orquestación
- ✅ Prometheus/Grafana manejan observabilidad
- ✅ Cada herramienta hace lo que mejor sabe

---

## 7. Mejores Prácticas

### ✅ DO: Usar Flow como Componente

```python
# BIEN: Flow es una pieza en un sistema mayor
airflow_task >> flow_pipeline >> validation >> deployment
```

### ❌ DON'T: Intentar que Flow haga todo

```python
# MAL: Flow no reemplaza a Airflow/Kubernetes
# No intentes hacer scheduling complejo en Flow
```

### ✅ DO: Exportar Métricas de Negocio

```python
# BIEN: Exporta datos que otros necesitan
with open('__flow_xcom__.json', 'w') as f:
    json.dump({'records': 1000, 'errors': 0}, f)
```

### ✅ DO: Monitorear con Herramientas Estándar

```bash
# BIEN: Usa Prometheus/Grafana para observabilidad
python prometheus_exporter.py &
flow pipeline.fl
```

---

## 8. Troubleshooting

### "No se generó __flow_junit__.xml"

Flow solo genera JUnit XML si hay un error o si el pipeline completa exitosamente. Verifica que el pipeline se ejecutó.

### "Prometheus no muestra métricas"

Asegúrate de que `prometheus_exporter.py` esté corriendo y que `__flow_metrics__.json` exista.

### "Airflow no puede leer __flow_xcom__.json"

Verifica que Flow tenga permisos de escritura en el directorio de trabajo de Airflow.

---

## Conclusión

Flow v2.0 ya no es una "isla". Con estas integraciones, Flow se convierte en un **ciudadano de primera clase** en el ecosistema empresarial:

- ✅ **CI/CD**: JUnit XML nativo
- ✅ **Orquestación**: XCom para Airflow
- ✅ **Observabilidad**: Métricas para Prometheus

**Flow v2.0: Simple por dentro, integrado por fuera.** 🌉
