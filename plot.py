import pandas as pd
import matplotlib.pyplot as plt

# Load CSV
df = pd.read_csv("benchmark_file.csv")

# Plot
plt.figure(figsize=(10, 6))
plt.plot(df['ArraySize'], df['SingleThreadedTime'], label='Single-Threaded', marker='o')
plt.plot(df['ArraySize'], df['MultiThreadedTime'], label='Multi-Threaded', marker='o')

# Add labels and legend
plt.title("Merge Sort Benchmarking")
plt.xlabel("Array Size")
plt.ylabel("Time (seconds)")
plt.legend()
plt.grid(True)

# Show plot
plt.tight_layout()
plt.show()
