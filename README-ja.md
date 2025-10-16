# Flow プログラミング言語

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![GitHub](https://img.shields.io/badge/GitHub-AnonymoDGH%2FFlow-blue)](https://github.com/AnonymoDGH/Flow)

**他の言語で読む: [English](README.md) | [Español](README-es.md) | [中文](README-zh.md) | [Français](README-fr.md)**

---

Flow は、Python、JavaScript、C++ のコードを 1 つのファイルに記述し、自動的に正しい順序で実行できるマルチパラダイムプログラミング言語です。

## 🚀 クイックスタート

```bash
# Flow をビルド
make

# サンプルを実行
./flow examples/test.fl

# メトリクスを表示
./flow metrics
```

## 📦 インストール

### ソースからインストール

```bash
git clone https://github.com/AnonymoDGH/Flow.git
cd Flow
make
make install  # オプション：システム全体にインストール
```

### 必要要件

- C++17 対応の g++
- Python 3.x
- Node.js

## 🎯 なぜ Flow？

Flow は実際の問題を解決します：**データパイプラインでの複数言語のオーケストレーション**。

### 問題点
```bash
# 以前：分散したスクリプト、保守が困難
python extract.py
node transform.js
./analyze.exe
python cleanup.py
```

### Flow の解決策
```python
# 1 つのファイル、1 つのコマンド
@data

# Python：データ抽出
data = extract_from_db()
flow_set('data', data)

# JavaScript：変換
const data = flowGet('data');
const transformed = await transform(data);
flowSet('result', transformed);

# C++：分析（高性能）
analyze_fast(data);
```

```bash
flow pipeline.fl  # これだけ！
```

## ✨ 機能

- **マルチ言語**: 1 つのファイルで Python、JavaScript、C++ を使用
- **共有メモリ**: `flow_set()` / `flowGet()` による高速通信
- **並列実行**: `@parallel` で最大のパフォーマンス
- **フェイルファスト**: エラー時にパイプラインを自動停止
- **マルチファイル**: `import "file.fl"` でコードを整理
- **CI/CD 統合**: GitHub Actions、Jenkins 用の JUnit XML
- **可観測性**: Prometheus、Datadog、Grafana 用のメトリクス
- **オーケストレーション**: Airflow、Prefect、Dagster 用の XCom
- **パッケージシステム**: npm のような、複数言語対応

## 📖 ドキュメント

- [クイックスタートガイド](docs/QUICKSTART.md) - 5 分で始める
- [なぜ Flow?](docs/WHY_FLOW.md) - ユースケースと比較
- [エコシステム統合](docs/ECOSYSTEM_INTEGRATION.md) - CI/CD、Airflow、Prometheus
- [変更履歴](docs/CHANGELOG.md) - バージョン履歴
- [貢献](CONTRIBUTING.md) - 貢献方法

## 🎓 例

### 基本的な例

```python
# hello.fl
@data

# Python
def greet(name):
    print(f"Python からこんにちは、{name}！")
    flow_set('name', name)

greet("世界")

# JavaScript
fn greet_js():
    const name = flowGet('name');
    console.log(`JavaScript からこんにちは、${name}！`);

greet_js()

# C++
cpp
std::string name = flowGet("name", "世界");
std::cout << "C++ からこんにちは、" << name << "！" << std::endl;
end
```

### 並列実行

```python
@parallel

# これら 3 つのブロックは同時に実行されます

# Python：I/O（ファイル読み込み）
for file in files:
    data = read_file(file)

# JavaScript：CPU（変換）
fn transform():
    compute_heavy_task();

# C++：CPU（計算）
cpp
calculate_intensive();
end
```

## 🛠️ コマンド

```bash
# 実行
flow <file.fl>              # ファイルを実行
flow init [name]            # 新しいプロジェクトを作成

# パッケージ管理
flow install <package>      # パッケージをインストール
flow install                # すべての依存関係をインストール
flow uninstall <package>    # パッケージをアンインストール
flow list                   # インストール済みパッケージを一覧表示

# ユーティリティ
flow metrics                # 実行メトリクスを表示
flow run <script>           # flow.json からスクリプトを実行
flow version                # バージョンを表示
flow --help                 # ヘルプを表示
```

## 📊 パフォーマンス

### シリアルモード（デフォルト）
- Python → JavaScript → C++
- 時間：すべてのステージの合計

### パラレルモード（`@parallel`）
- Python || JavaScript || C++
- 時間：3 つのステージの最大値
- **高速化：最大 3 倍**

## 🤝 貢献

貢献を歓迎します！詳細は [CONTRIBUTING.md](CONTRIBUTING.md) をご覧ください。

## 📝 ライセンス

MIT ライセンス - 詳細は [LICENSE](LICENSE) をご覧ください。

## 📞 連絡先

- **GitHub**: [@AnonymoDGH](https://github.com/AnonymoDGH)
- **リポジトリ**: [Flow](https://github.com/AnonymoDGH/Flow)
- **Issues**: [GitHub Issues](https://github.com/AnonymoDGH/Flow/issues)

---

**Flow v2.0** - 1 つの言語、3 つの世界、制限なし。🚀
