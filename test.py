import subprocess
import concurrent.futures
import multiprocessing
from tqdm import tqdm

START = 1
END = 1000

BASE_CMD = ["./Game.exe", "{num}", "n", "debug", "new", "./config.cnf", "Template", "Dummy"]

def run_test(n):
    cmd = [arg.format(num=str(n)) for arg in BASE_CMD]
    try:
        result = subprocess.run(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        return n if result.returncode != 0 else None
    except Exception:
        return n 

def main():
    crashed = []
    max_workers = multiprocessing.cpu_count()
    with concurrent.futures.ThreadPoolExecutor(max_workers=max_workers) as executor:
        numbers = list(range(START, END + 1))
        futures = {executor.submit(run_test, n): n for n in numbers}
        
        with tqdm(total=len(futures), desc="Running tests") as pbar:
            for future in concurrent.futures.as_completed(futures):
                result = future.result()
                if result is not None:
                    crashed.append(result)
                pbar.update(1)

    if crashed:
        print("Crashed inputs:", crashed)
    else:
        print("Tests passed")

if __name__ == "__main__":
    main()
