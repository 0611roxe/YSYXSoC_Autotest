import os
from pathlib import Path
import yaml

class EnvConfigurator:
    @staticmethod
    def resolve_vars(value, env):
        if isinstance(value, str):
            while "${" in value:
                start = value.find("${")
                end = value.find("}", start)
                if start != -1 and end != -1:
                    var_name = value[start+2:end]
                    var_value = env.get(var_name, "")
                    value = value[:start] + var_value + value[end+1:]
                else:
                    break
        return value

    @classmethod
    def setup_env_from_yaml(cls, config_yaml: str | Path) -> dict:
        config_yaml = Path(config_yaml)
        if not config_yaml.exists():
            raise FileNotFoundError(f"Config file {config_yaml} does not exist.")
        with open(config_yaml, "r", encoding="utf-8") as f:
            config_dict = yaml.safe_load(f)
        env = {}
        for k, v in config_dict.items():
            env[k] = cls.resolve_vars(v, {**os.environ, **env})
            os.environ[k] = str(env[k])
        host_log_dir = os.path.expandvars(env.get("HOST_LOG_DIR", ""))
        npc_home = os.path.expandvars(env.get("NPC_HOME", ""))
        if os.path.isdir(host_log_dir) and os.listdir(host_log_dir):
            log_dir = host_log_dir
        else:
            log_dir = os.path.join(npc_home, "Log")
        env["LOG_DIR"] = log_dir
        os.environ["LOG_DIR"] = log_dir
        return env

if __name__ == "__main__":
    config_yaml = Path(__file__).parent / "config" / "config.yaml"
    env = EnvConfigurator.setup_env_from_yaml(config_yaml)
    for k, v in env.items():
        print(f"{k}={v}")