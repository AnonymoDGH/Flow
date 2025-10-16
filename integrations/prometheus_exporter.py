# Exportador de métricas de Flow para Prometheus
from prometheus_client import start_http_server, Gauge, Counter, Histogram
import json
import time
import os

# Definir métricas
flow_stage_duration = Histogram(
    'flow_stage_duration_seconds',
    'Duration of Flow stages',
    ['stage']
)

flow_stage_success = Counter(
    'flow_stage_success_total',
    'Total successful Flow stage executions',
    ['stage']
)

flow_stage_failure = Counter(
    'flow_stage_failure_total',
    'Total failed Flow stage executions',
    ['stage']
)

flow_pipeline_duration = Gauge(
    'flow_pipeline_duration_seconds',
    'Total duration of Flow pipeline'
)

def parse_flow_metrics():
    """Parse Flow metrics and export to Prometheus"""
    if not os.path.exists('__flow_metrics__.json'):
        return
    
    with open('__flow_metrics__.json', 'r') as f:
        metrics = [json.loads(line) for line in f]
    
    total_duration = 0
    
    for metric in metrics:
        stage = metric['stage']
        duration = metric['duration']
        exit_code = metric['exit_code']
        
        # Registrar duración
        flow_stage_duration.labels(stage=stage).observe(duration)
        total_duration += duration
        
        # Registrar éxito/fallo
        if exit_code == 0:
            flow_stage_success.labels(stage=stage).inc()
        else:
            flow_stage_failure.labels(stage=stage).inc()
    
    # Registrar duración total
    flow_pipeline_duration.set(total_duration)
    
    print(f"✓ Exported metrics for {len(metrics)} stages")
    print(f"  Total duration: {total_duration:.2f}s")

if __name__ == '__main__':
    # Iniciar servidor HTTP en puerto 8000
    start_http_server(8000)
    print("✓ Prometheus exporter started on :8000")
    print("  Metrics available at http://localhost:8000/metrics")
    
    # Loop para actualizar métricas
    while True:
        try:
            parse_flow_metrics()
        except Exception as e:
            print(f"✗ Error parsing metrics: {e}")
        
        time.sleep(10)  # Actualizar cada 10 segundos
