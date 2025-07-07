# YSYXSoC Auto-Test Platform

## Directory Structure

```
auto-test/
├── Dockerfile
├── scripts/
│   ├── __init__.py
│   ├── __main__.py
│   ├── benchmark.py
│   ├── cpu_test.py
│   ├── log_parser.py
│   ├── set_env.py
│   ├── simulate.py
│   ├── config/
│   │   └── config.yaml
│   └── ...
├── log/
│   ├── cpu-test-riscv32e-ysyxsoc.log
│   ├── coremark-riscv32e-ysyxsoc.log
│   ├── dhrystone-riscv32e-ysyxsoc.log
│   ├── microbench-riscv32e-ysyxsoc.log
│   ├── cpu_test_result.json
│   ├── benchmark_result.json
│   └── ...
├── core/
│   └── ysyx_22050499.v
└── ysyx-local-autotest/
    └── test-framework/
        └── ysyx-workbench/
            ├── abstract-machine/
            ├── am-kernels/
            ├── npc/
            ├── ysyxSoC/
            └── ...
```

---

## Quick Start

### 1. Build Docker Image

```sh
docker build -t ysyx-local-autotest -f Dockerfile .
```

### 2. Run Container

```sh
docker run --rm -it \
  -v $(pwd)/core:/mnt/rtl \
  -v $(pwd)/log:/mnt/log \
  ysyx-auto-test /bin/bash
```

### 3. One-Click Auto Test

```sh
# cd /ysyx-local-autotest
python3 -m scripts
```

This command will automatically complete environment configuration, RTL file preparation, SoC simulation, test execution, log parsing, and JSON result output.