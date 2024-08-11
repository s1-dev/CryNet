import json
import matplotlib.pyplot as plt
import numpy as np

# Function to read data from a JSON file
def read_data_from_json(file_path):
    with open(file_path, 'r') as f:
        data = json.load(f)
    return data

# Function to plot the data
def plot_encryption_times(data):
    num_files = [entry['num_files'] for entry in data]
    encryption_times = [entry['encryption_time'] for entry in data]

    plt.figure(figsize=(10, 6))
    plt.scatter(num_files, encryption_times, color='b', label='Data points')

    # Add trend line
    z = np.polyfit(num_files, encryption_times, 1)
    p = np.poly1d(z)
    plt.plot(num_files, p(num_files), "r--", label='Trend line')

    plt.title('Encryption Time vs Number of Files')
    plt.xlabel('Number of Files')
    plt.ylabel('Encryption Time (seconds)')
    plt.grid(True)
    plt.legend()

    plt.show()

# Main function
def main():
    file_path = 'enc_times.json'  # Update with your JSON file path
    data = read_data_from_json(file_path)
    plot_encryption_times(data)

if __name__ == "__main__":
    main()
