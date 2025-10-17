# Flow Quick Reference

**TL;DR**: Use sequential mode only. Avoid shared memory and parallel execution.

---

## ✅ Safe to Use

### Sequential Execution

```python
# ✅ SAFE: Sequential execution
@data

# Python
print("Step 1")

# JavaScript
fn step2():
    console.log('Step 2');
step2()

# C++
cpp
std::cout << "Step 3" << std::endl;
end
```

**Why it's safe**: Executes one language at a time, no race conditions.

**Reliability**: 100% success rate

---

## ❌ Do NOT Use

### Shared Memory

```python
# ❌ UNSAFE: Shared memory (race conditions)
@data

# Python
flow_set('counter', 0)  # ❌ Don't use

# JavaScript
fn increment():
    let val = flowGet('counter');  # ❌ Don't use
    flowSet('counter', val + 1);
increment()
```

**Why it's unsafe**: Race conditions cause data loss.

**Failure rate**: 68% in tests

**Alternative**: Pass data through files or stdout/stdin

---

### Parallel Execution

```python
# ❌ UNSAFE: Parallel execution (data corruption)
@parallel

# Python
flow_set('data', [1, 2, 3])  # ❌ Don't use

# JavaScript
fn process():
    let data = flowGet('data');  # ❌ Don't use
process()
```

**Why it's unsafe**: Severe data corruption, file corruption possible.

**Failure rate**: 92% in tests

**Alternative**: Use sequential mode or external tools (Airflow, Docker Compose)

---

## Safe Patterns

### Pattern 1: Independent Scripts

```python
# ✅ Each language does independent work
@data

# Python: Process data
data = load_data()
save_to_file('data.json', data)
print("Python done")

# JavaScript: Transform data
fn transform():
    const data = require('./data.json');
    const result = transform(data);
    fs.writeFileSync('result.json', JSON.stringify(result));
    console.log('JavaScript done');
transform()

# C++: Analyze data
cpp
std::ifstream file("result.json");
json data = json::parse(file);
analyze(data);
std::cout << "C++ done" << std::endl;
end
```

**Why it works**: Each language reads/writes its own files, no shared state.

---

### Pattern 2: Pipeline with Files

```python
# ✅ Pass data through files
@data

# Python: Extract
data = extract_from_db()
with open('extracted.json', 'w') as f:
    json.dump(data, f)

# JavaScript: Transform
fn transform():
    const data = JSON.parse(fs.readFileSync('extracted.json'));
    const transformed = transform(data);
    fs.writeFileSync('transformed.json', JSON.stringify(transformed));
transform()

# C++: Load
cpp
std::ifstream file("transformed.json");
json data = json::parse(file);
load_to_db(data);
end
```

**Why it works**: Explicit file passing, no race conditions.

---

### Pattern 3: Print and Parse

```python
# ✅ Pass data through stdout
@data

# Python: Generate data
import json
data = {'value': 42}
print(json.dumps(data))

# JavaScript: Parse output
fn process():
    // Read from previous output
    const data = JSON.parse(previousOutput);
    console.log('Received:', data.value);
process()
```

**Why it works**: Sequential execution, clear data flow.

---

## Unsafe Patterns

### Anti-Pattern 1: Shared Counter

```python
# ❌ WRONG: Race condition
@data

flow_set('counter', 0)

# Multiple languages increment
# Result: Lost updates
```

**Problem**: Read-modify-write race condition

**Fix**: Don't share mutable state

---

### Anti-Pattern 2: Parallel Writes

```python
# ❌ WRONG: Data corruption
@parallel

# Python
flow_set('py_data', [1, 2, 3])

# JavaScript
fn write():
    flowSet('js_data', [4, 5, 6]);
write()

# C++
cpp
flowSet("cpp_data", "[7, 8, 9]");
end
```

**Problem**: Concurrent writes corrupt JSON file

**Fix**: Use sequential mode

---

### Anti-Pattern 3: Shared Array

```python
# ❌ WRONG: Array corruption
@data

flow_set('items', [])

# Python
items = flow_get('items')
items.append(1)
flow_set('items', items)

# JavaScript
fn append():
    let items = flowGet('items');
    items.push(2);
    flowSet('items', items);
append()
```

**Problem**: Lost updates, array may have only [1] or [2] instead of [1, 2]

**Fix**: Each language uses its own array

---

## Command Reference

### Safe Commands

```bash
# ✅ Run sequential script
./flow script.fl

# ✅ Check version
./flow version

# ✅ View help
./flow --help

# ✅ Run tests (sequential)
./flow test.fl
```

### Unsafe Commands

```bash
# ❌ Don't run parallel scripts
./flow parallel_script.fl  # 92% failure rate

# ❌ Don't use shared memory
./flow memory_test.fl  # 68% failure rate
```

---

## Error Messages

### Common Errors

```
Error: Command injection detected
→ Fix: Remove dangerous characters (;, &&, |)

Error: File not found
→ Fix: Check file path, use absolute paths

Error: JSON parse error
→ Cause: Race condition corrupted file
→ Fix: Use sequential mode, avoid shared memory

Error: Timeout
→ Fix: Increase timeout or optimize code
```

---

## Performance Tips

### Do This ✅

```python
# ✅ Use appropriate language for task
@data

# Python: I/O and data processing
data = load_large_file()

# C++: CPU-intensive computation
cpp
compute_intensive(data);
end

# JavaScript: API calls
fn upload():
    await uploadToAPI(result);
upload()
```

### Don't Do This ❌

```python
# ❌ Don't use parallel mode for speed
@parallel  # 92% failure rate, not worth it

# ❌ Don't use shared memory for communication
flow_set('data', large_data)  # 68% failure rate
```

---

## Debugging Tips

### Enable Verbose Output

```bash
# Add debug prints
@data

# Python
print("DEBUG: Starting Python")
print(f"DEBUG: Data = {data}")

# JavaScript
fn debug():
    console.log('DEBUG: Starting JavaScript');
    console.log('DEBUG: Data =', data);
debug()
```

### Check Shared Memory File

```bash
# View current shared memory state
cat __flow_shared__.json

# Or on Windows
type __flow_shared__.json
```

### Test in Isolation

```python
# Test each language separately
@data

# Comment out other languages
# Python only
print("Testing Python")

# # JavaScript
# fn test():
#     console.log('Testing JS');
# test()
```

---

## Migration Guide

### From Shared Memory to Files

**Before (Unsafe)**:
```python
@data

# Python
flow_set('data', [1, 2, 3])

# JavaScript
fn process():
    let data = flowGet('data');
process()
```

**After (Safe)**:
```python
@data

# Python
import json
with open('data.json', 'w') as f:
    json.dump([1, 2, 3], f)

# JavaScript
fn process():
    const data = JSON.parse(fs.readFileSync('data.json'));
process()
```

---

### From Parallel to Sequential

**Before (Unsafe)**:
```python
@parallel

# Python
process_py()

# JavaScript
fn process_js():
    compute();
process_js()
```

**After (Safe)**:
```python
@data

# Python
process_py()

# JavaScript
fn process_js():
    compute();
process_js()
```

---

## When to Use Flow

### ✅ Good Use Cases

- Learning multi-language orchestration
- Simple sequential scripts
- Prototyping algorithms
- Personal automation
- Educational demonstrations

### ❌ Bad Use Cases

- Production applications
- Multi-user systems
- Critical data processing
- Financial systems
- Healthcare applications
- Any system requiring reliability

---

## Alternatives for Production

### For Workflow Orchestration
- **Airflow**: Industry standard, mature
- **Prefect**: Modern, Python-native
- **Dagster**: Data-aware orchestration

### For Multi-Language Pipelines
- **Docker Compose**: Container orchestration
- **Kubernetes**: Production-grade orchestration
- **Shell scripts + IPC**: ZeroMQ, Redis, RabbitMQ

### For Data Sharing
- **Redis**: In-memory data store
- **RabbitMQ**: Message queue
- **PostgreSQL**: Relational database
- **MongoDB**: Document database

---

## Summary

```
┌─────────────────────────────────────────────────────────┐
│  DO                          │  DON'T                   │
├──────────────────────────────┼──────────────────────────┤
│  ✅ Use sequential mode      │  ❌ Use parallel mode    │
│  ✅ Pass data through files  │  ❌ Use shared memory    │
│  ✅ Independent scripts      │  ❌ Share mutable state  │
│  ✅ Simple automation        │  ❌ Production systems   │
│  ✅ Learning & prototyping   │  ❌ Critical data        │
└──────────────────────────────┴──────────────────────────┘
```

---

**Remember**: Flow is an educational prototype. For production, use mature tools.

**See also**:
- [TESTING_RESULTS.md](TESTING_RESULTS.md) - Full test results
- [RACE_CONDITION_EXPLAINED.md](RACE_CONDITION_EXPLAINED.md) - Why shared memory fails
- [PRODUCTION_READINESS.md](PRODUCTION_READINESS.md) - Production assessment
