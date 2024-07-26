import json
import matplotlib.pyplot as plt

def load_data_from_file(filename):
    with open(filename, 'r') as f:
        data = json.load(f)
    return data

def plot_resources(data):
    times = [point['time'] for point in data]
    cpu_usages = [point['cpu_usage'] for point in data]
    memory_usages = [point['memory_usage'] for point in data]

    plt.figure(figsize=(12, 6))

    plt.subplot(2, 1, 1)
    plt.plot(times, cpu_usages, marker='o', linestyle='-', color='b', label='CPU Usage (%)')
    plt.xlabel('Time (seconds)')
    plt.ylabel('CPU Usage (%)')
    plt.title('CPU Usage Over Time During Bash ForkBomb')
    plt.legend()
    plt.grid(True)

    plt.subplot(2, 1, 2)
    plt.plot(times, memory_usages, marker='o', linestyle='-', color='r', label='Memory Usage (%)')
    plt.xlabel('Time (seconds)')
    plt.ylabel('Memory Usage (%)')
    plt.title('Memory Usage Over Time During C ForkBomb')
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.show()

if __name__ == '__main__':
    data_file = "system_resource_consumption.json"
    data = load_data_from_file(data_file)
    plot_resources(data)
