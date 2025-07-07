from pathlib import Path

def main():
    from .set_env import EnvConfigurator
    config_path = Path(__file__).parent / "config" / "config.yaml"
    EnvConfigurator.setup_env_from_yaml(config_path)

    from .simulate import Simulator
    Simulator.run()

    from .cpu_test import CpuTestLogParser
    from .benchmark import BenchmarkLogParser
    import os

    log_dir = os.environ.get("LOG_DIR", "./log")
    CpuTestLogParser(log_dir).parse()
    BenchmarkLogParser(log_dir).parse()

if __name__ == "__main__":
    main()