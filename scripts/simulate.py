import os
import subprocess
import argparse
import re
from .set_env import EnvConfigurator
from pathlib import Path

class Simulator:
    def __init__(self, config_path):
        EnvConfigurator.setup_env_from_yaml(config_path)
        self.ysyx_home = os.environ.get("YSYX_HOME")
        self.am_kernels_home = os.environ.get("AM_KERNELS_HOME")
        self.npc_home = os.environ.get("NPC_HOME")
        self.top_name = os.environ.get("TOP_NAME")
        self.host_rtl_dir = os.environ.get("HOST_RTL_DIR")

    def copy_rtl_files(self):
        vsrc_dir = os.path.join(self.ysyx_home, "npc/vsrc")
        os.makedirs(vsrc_dir, exist_ok=True)
        exts = ('.v', '.sv', '.svh', '.vh')
        for root, _, files in os.walk(self.host_rtl_dir):
            for file in files:
                if file.endswith(exts):
                    src_file = os.path.join(root, file)
                    subprocess.run(["cp", src_file, vsrc_dir], check=True)

    def patch_soc_full_v(self):
        src_v = os.path.join(self.ysyx_home, "ysyxSoC/build/ysyxSoCFull.v")
        dst_v = os.path.join(self.npc_home, "vsrc/ysyxSoCFull.v")
        os.makedirs(os.path.dirname(dst_v), exist_ok=True)
        subprocess.run(["cp", src_v, dst_v], check=True)
        with open(dst_v, "r", encoding="utf-8") as f:
            content = f.read()
        if f"module {self.top_name}" not in content:
            new_content = re.sub(r"ysyx_00000000", self.top_name, content)
            with open(dst_v, "w", encoding="utf-8") as f:
                f.write(new_content)

    def run_tests(self, mainargs="train", tests=None):
        if tests is None:
            tests = ["cpu-tests", "coremark", "dhrystone", "microbench"]
        test_cmds = {
            "cpu-tests": ["make", "-C", f"{self.am_kernels_home}/tests/cpu-tests", "ARCH=riscv32e-ysyxsoc", "run"],
            "coremark": ["make", "-C", f"{self.am_kernels_home}/benchmarks/coremark", "ARCH=riscv32e-ysyxsoc", "run"],
            "dhrystone": ["make", "-C", f"{self.am_kernels_home}/benchmarks/dhrystone", "ARCH=riscv32e-ysyxsoc", "run"],
            "microbench": ["make", "-C", f"{self.am_kernels_home}/benchmarks/microbench", "ARCH=riscv32e-ysyxsoc", f"mainargs={mainargs}", "run"],
        }
        for test in tests:
            cmd = test_cmds[test]
            print(f"Running: {' '.join(cmd)}")
            result = subprocess.run(cmd)
            if result.returncode != 0:
                print(f"Command failed: {' '.join(cmd)}")
                break

    @classmethod
    def run(cls):
        parser = argparse.ArgumentParser(description="Run am-kernels simulation tests.")
        parser.add_argument('--mainargs', default='train', help='mainargs for microbench (default: train)')
        parser.add_argument('--tests', nargs='*', choices=['cpu-tests', 'coremark', 'dhrystone', 'microbench', 'all'],
                            default=['all'], help='Specify which tests to run')
        args = parser.parse_args()
        config_path = Path(__file__).parent / "config" / "config.yaml"
        sim = cls(config_path)
        sim.copy_rtl_files()
        sim.patch_soc_full_v()
        selected_tests = ["cpu-tests", "coremark", "dhrystone", "microbench"] if 'all' in args.tests else args.tests
        sim.run_tests(mainargs=args.mainargs, tests=selected_tests)

if __name__ == "__main__":
    Simulator.run()