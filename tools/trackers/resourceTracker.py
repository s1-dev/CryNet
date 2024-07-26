import psutil
import time
import threading
import json

data_file = "system_resource_consumption.json"

data_points = []

def monitor_resources(interval=1):
    start_time = time.time()
    while time.time() - start_time < monitor_duration:
        cpu_usage = psutil.cpu_percent(interval=interval)
        memory_info = psutil.virtual_memory()
        memory_usage = memory_info.percent
        data_points.append({
            "time": time.time() - start_time,
            "cpu_usage": cpu_usage,
            "memory_usage": memory_usage
        })
        print(f"Time: {time.time() - start_time}, CPU Usage: {cpu_usage}%, Memory Usage: {memory_usage}%")

def save_data_to_file():
    with open(data_file, 'w') as f:
        json.dump(data_points, f, indent=4)

monitor_duration = 30 
monitor_thread = threading.Thread(target=monitor_resources, daemon=True)
monitor_thread.start()

time.sleep(monitor_duration)

save_data_to_file()
print(f"Data saved to {data_file}")
