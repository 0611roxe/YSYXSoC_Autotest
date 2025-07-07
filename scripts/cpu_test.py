import os
import json
import re

class CpuTestLogParser:
    def __init__(self, log_dir):
        self.log_file = os.path.join(log_dir, "cpu-test-riscv32e-ysyxsoc.log")
        self.result_json = os.path.join(log_dir, "cpu_test_result.json")

    def parse(self):
        results = {}
        pass_count = 0
        total_count = 0
        all_pass = True
        with open(self.log_file, "r", encoding="utf-8") as f:
            for line in f:
                m = re.match(r"\[\s*([^\]]+)\]\s+(PASS|FAIL)", line)
                if m:
                    test_name = m.group(1).strip()
                    status = m.group(2)
                    results[test_name] = status
                    total_count += 1
                    if status == "PASS":
                        pass_count += 1
                    else:
                        all_pass = False
        summary = {
            "cpu-test": "pass" if all_pass and results else "fail",
            "details": results,
            "cpu-test_stat": f"{pass_count}/{total_count}"
        }
        with open(self.result_json, "w", encoding="utf-8") as f:
            json.dump(summary, f, indent=2, ensure_ascii=False)
        print(f"cpu-test: {summary['cpu-test']} ({summary['cpu-test_stat']})")

if __name__ == "__main__":
    log_dir = os.environ.get("LOG_DIR", "./log")
    CpuTestLogParser(log_dir).parse()