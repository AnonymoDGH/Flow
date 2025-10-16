# Flow 编程语言

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![GitHub](https://img.shields.io/badge/GitHub-AnonymoDGH%2FFlow-blue)](https://github.com/AnonymoDGH/Flow)

**其他语言版本: [English](README.md) | [Español](README-es.md) | [日本語](README-ja.md) | [Français](README-fr.md)**

---

Flow 是一种多范式编程语言，允许您在单个文件中编写 Python、JavaScript 和 C++ 代码，并自动按正确顺序执行它们。

## 🚀 快速开始

```bash
# 构建 Flow
make

# 运行示例
./flow examples/test.fl

# 查看指标
./flow metrics
```

## 📦 安装

### 从源代码安装

```bash
git clone https://github.com/AnonymoDGH/Flow.git
cd Flow
make
make install  # 可选：系统级安装
```

### 系统要求

- 支持 C++17 的 g++
- Python 3.x
- Node.js

## 🎯 为什么选择 Flow？

Flow 解决了一个实际问题：**在数据管道中编排多种语言**。

### 问题所在
```bash
# 之前：分散的脚本，难以维护
python extract.py
node transform.js
./analyze.exe
python cleanup.py
```

### Flow 的解决方案
```python
# 一个文件，一条命令
@data

# Python：提取数据
data = extract_from_db()
flow_set('data', data)

# JavaScript：转换数据
const data = flowGet('data');
const transformed = await transform(data);
flowSet('result', transformed);

# C++：分析（高性能）
analyze_fast(data);
```

```bash
flow pipeline.fl  # 就这么简单！
```

## ✨ 特性

- **多语言**: 在一个文件中使用 Python、JavaScript 和 C++
- **共享内存**: 使用 `flow_set()` / `flowGet()` 快速通信
- **并行执行**: 使用 `@parallel` 获得最大性能
- **快速失败**: 出错时自动停止管道
- **多文件**: 使用 `import "file.fl"` 组织代码
- **CI/CD 集成**: 为 GitHub Actions、Jenkins 提供 JUnit XML
- **可观测性**: 为 Prometheus、Datadog、Grafana 提供指标
- **编排**: 为 Airflow、Prefect、Dagster 提供 XCom
- **包管理系统**: 类似 npm，但支持多种语言

## 📖 文档

- [快速入门指南](docs/QUICKSTART.md) - 5 分钟入门
- [为什么选择 Flow?](docs/WHY_FLOW.md) - 用例和比较
- [生态系统集成](docs/ECOSYSTEM_INTEGRATION.md) - CI/CD、Airflow、Prometheus
- [更新日志](docs/CHANGELOG.md) - 版本历史
- [贡献指南](CONTRIBUTING.md) - 如何贡献

## 🎓 示例

### 基础示例

```python
# hello.fl
@data

# Python
def greet(name):
    print(f"来自 Python 的问候，{name}！")
    flow_set('name', name)

greet("世界")

# JavaScript
fn greet_js():
    const name = flowGet('name');
    console.log(`来自 JavaScript 的问候，${name}！`);

greet_js()

# C++
cpp
std::string name = flowGet("name", "世界");
std::cout << "来自 C++ 的问候，" << name << "！" << std::endl;
end
```

### 并行执行

```python
@parallel

# 这三个代码块同时执行

# Python：I/O（读取文件）
for file in files:
    data = read_file(file)

# JavaScript：CPU（转换）
fn transform():
    compute_heavy_task();

# C++：CPU（计算）
cpp
calculate_intensive();
end
```

## 🛠️ 命令

```bash
# 执行
flow <file.fl>              # 运行文件
flow init [name]            # 创建新项目

# 包管理
flow install <package>      # 安装包
flow install                # 安装所有依赖
flow uninstall <package>    # 卸载包
flow list                   # 列出已安装的包

# 实用工具
flow metrics                # 显示执行指标
flow run <script>           # 从 flow.json 运行脚本
flow version                # 显示版本
flow --help                 # 显示帮助
```

## 📊 性能

### 串行模式（默认）
- Python → JavaScript → C++
- 时间：所有阶段的总和

### 并行模式（`@parallel`）
- Python || JavaScript || C++
- 时间：三个阶段的最大值
- **加速：最高可达 3 倍**

## 🤝 贡献

欢迎贡献！详情请参阅 [CONTRIBUTING.md](CONTRIBUTING.md)。

## 📝 许可证

MIT 许可证 - 详情请参阅 [LICENSE](LICENSE)。

## 📞 联系方式

- **GitHub**: [@AnonymoDGH](https://github.com/AnonymoDGH)
- **仓库**: [Flow](https://github.com/AnonymoDGH/Flow)
- **Issues**: [GitHub Issues](https://github.com/AnonymoDGH/Flow/issues)

---

**Flow v2.0** - 一种语言，三个世界，零限制。🚀
