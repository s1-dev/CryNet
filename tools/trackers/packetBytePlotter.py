import json
import matplotlib.pyplot as plt

def aggregate_data(file_path, interval=1):
    with open(file_path, 'r') as f:
        packet_data = json.load(f)

    bytes_per_second = []
    start_time = None
    current_bytes = 0
    last_time = None

    for entry in packet_data:
        timestamp = float(entry['time'])
        packet_size = entry['packet_size']
        
        if start_time is None:
            start_time = timestamp
            last_time = start_time

        # Accumulate packet sizes
        if timestamp - last_time < interval:
            current_bytes += packet_size
        else:
            # Store the accumulated bytes for the last interval
            bytes_per_second.append(current_bytes)
            # Reset for the next interval
            current_bytes = packet_size
            # Update last_time to the start of the new interval
            last_time += interval

    # Append the last interval data
    if current_bytes > 0:
        bytes_per_second.append(current_bytes)
    while len(bytes_per_second) < 60:
        bytes_per_second.append(0)

    return bytes_per_second

def main():
    file_paths = ["packet_data_1_bots.json", "packet_data_2_bots.json", "packet_data_3_bots.json"]

    labels = ["1 Bot", "2 Bots", "3 Bots"]

    aggregated_data = []
    for file_path in file_paths:
        aggregated_data.append(aggregate_data(file_path))

    time_intervals = list(range(len(aggregated_data[0])))

    plt.figure(figsize=(12, 8))
    for data, label in zip(aggregated_data, labels):
        plt.plot(time_intervals, data, marker='o', linestyle='-', label=label)

    plt.xlabel('Time (seconds)')
    plt.ylabel('Bytes Received')
    plt.title('Bytes Received Per Second for Different Number of Bots')
    plt.legend()
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    main()
