# Integración de Flow con Apache Airflow
from airflow import DAG
from airflow.operators.bash import BashOperator
from airflow.operators.python import PythonOperator
from datetime import datetime, timedelta
import json

default_args = {
    'owner': 'data-team',
    'depends_on_past': False,
    'start_date': datetime(2025, 1, 1),
    'email_on_failure': True,
    'email_on_retry': False,
    'retries': 1,
    'retry_delay': timedelta(minutes=5),
}

dag = DAG(
    'flow_pipeline',
    default_args=default_args,
    description='Flow multi-language pipeline',
    schedule_interval=timedelta(days=1),
    catchup=False
)

# Tarea 1: Preparar datos para Flow
def prepare_data(**context):
    """Preparar datos que Flow consumirá"""
    data = {
        'input_files': ['data1.csv', 'data2.csv', 'data3.csv'],
        'config': {'batch_size': 1000, 'threshold': 0.95}
    }
    
    # Guardar para que Flow lo lea
    with open('/tmp/flow_input.json', 'w') as f:
        json.dump(data, f)
    
    # Push a XCom para Airflow
    context['task_instance'].xcom_push(key='input_files', value=len(data['input_files']))
    
    print(f"✓ Preparados {len(data['input_files'])} archivos para Flow")

prepare_task = PythonOperator(
    task_id='prepare_data',
    python_callable=prepare_data,
    dag=dag,
)

# Tarea 2: Ejecutar Flow pipeline
run_flow = BashOperator(
    task_id='run_flow_pipeline',
    bash_command='flow /path/to/pipeline.fl',
    dag=dag,
)

# Tarea 3: Extraer resultados de Flow
def extract_flow_results(**context):
    """Extraer resultados de Flow para Airflow"""
    
    # Leer métricas de Flow
    with open('__flow_metrics__.json', 'r') as f:
        metrics = [json.loads(line) for line in f]
    
    total_duration = sum(m['duration'] for m in metrics)
    all_passed = all(m['exit_code'] == 0 for m in metrics)
    
    # Leer resultados de negocio (si Flow los exportó)
    try:
        with open('__flow_xcom__.json', 'r') as f:
            results = json.load(f)
            records_processed = results.get('records_processed', 0)
    except:
        records_processed = 0
    
    # Push a XCom para siguiente tarea
    context['task_instance'].xcom_push(key='flow_duration', value=total_duration)
    context['task_instance'].xcom_push(key='flow_success', value=all_passed)
    context['task_instance'].xcom_push(key='records_processed', value=records_processed)
    
    print(f"✓ Flow completado en {total_duration:.2f}s")
    print(f"✓ Registros procesados: {records_processed}")
    
    if not all_passed:
        raise Exception("Flow pipeline falló")

extract_task = PythonOperator(
    task_id='extract_flow_results',
    python_callable=extract_flow_results,
    dag=dag,
)

# Tarea 4: Validar y notificar
def validate_and_notify(**context):
    """Validar resultados y notificar"""
    ti = context['task_instance']
    
    records = ti.xcom_pull(task_ids='extract_flow_results', key='records_processed')
    duration = ti.xcom_pull(task_ids='extract_flow_results', key='flow_duration')
    
    print(f"Pipeline completado:")
    print(f"  - Registros: {records}")
    print(f"  - Duración: {duration:.2f}s")
    print(f"  - Throughput: {records/duration:.2f} records/s")
    
    # Aquí podrías enviar a Slack, email, etc.

notify_task = PythonOperator(
    task_id='validate_and_notify',
    python_callable=validate_and_notify,
    dag=dag,
)

# Definir dependencias
prepare_task >> run_flow >> extract_task >> notify_task
