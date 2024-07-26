import json
import matplotlib.pyplot as plt

def load_packet_counts_from_json(filename):
    with open(filename, 'r') as json_file:
        packet_counts = json.load(json_file)
    return packet_counts

def plot_packet_counts(filenames, labels, duration):
    plt.figure(figsize=(10, 6))
    
    for filename, label in zip(filenames, labels):
        packet_counts = load_packet_counts_from_json(filename)
        time_intervals = list(range(duration))
        packets_per_second = [packet_counts.get(str(sec), 0) for sec in time_intervals]

        plt.plot(time_intervals, packets_per_second, marker='o', linestyle='-', label=label)

    plt.title('Packets Received Per Second')
    plt.xlabel('Time (seconds)')
    plt.ylabel('Number of Packets')
    plt.legend()
    plt.grid(True)
    plt.show()

if __name__ == '__main__':
    duration = 60  # Duration in seconds
    filenames = ['packet_counts_1_bots.json', 'packet_counts_2_bots.json', 'packet_counts_3_bots.json']
    labels = ['1 bot', '2 bots', '3 bots']

    plot_packet_counts(filenames, labels, duration)
