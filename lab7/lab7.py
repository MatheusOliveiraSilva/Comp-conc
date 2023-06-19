import math
import threading
import time

N = 1000000
THREADS = 4
pi_est = 0

class CalculatorThread(threading.Thread):
    def __init__(self, tid):
        super().__init__()
        self.tid = tid
        self.val = 0

    def run(self):
        for i in range(self.tid, N, THREADS):
            self.val += (-1) ** i / (2 * i + 1)

threads = [CalculatorThread(tid) for tid in range(THREADS)]
start_time = time.time()

for thread in threads:
    thread.start()
    # thread.run()

for thread in threads:
    thread.join()

for thread in threads:
    print("Thread {0.tid} = {0.val}".format(thread))
    pi_est += thread.val

print("\nRESULTS")
print(f"Elapsed time: {time.time() - start_time}")
print(f"Estimation: {4 * pi_est:.8f}")
print(f"Actual Value: {math.pi:.8f}")

difference = abs(math.pi - 4 * pi_est)
next_term = 4 * ((-1) ** N / (2 * N + 1))

if difference <= next_term:
    print("\nThe estimation is correct")
