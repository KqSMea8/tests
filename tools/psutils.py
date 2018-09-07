import psutil
import time
while True:
    print("CPU:", psutil.cpu_times_percent(interval=None, percpu=False))
    print("MEM:", psutil.virtual_memory())
    #print(psutil.cpu_times())
    time.sleep(1)
