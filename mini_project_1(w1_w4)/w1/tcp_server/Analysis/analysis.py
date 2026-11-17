import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

# Get project root directory
BASE_DIR = Path(__file__).resolve().parent.parent

# CSV file paths
PYTHON_CSV = BASE_DIR / "Python_version" / "sensor_log.csv"
GOLANG_CSV = BASE_DIR / "Golang_version" / "sensor_log.csv"

def analyze_data(file_path, server_name):

    print("\n================================")
    print(f"      {server_name} ANALYSIS")
    print("================================")

    # Read CSV
    if not file_path.exists():
        print(f"[ERROR] CSV file not found: {file_path}")
        return
    df = pd.read_csv(file_path)

    # Display raw data
    print("\n[1] SENSOR DATA")
    print(df.to_string(index=False))

    # Check missing values
    print("\n[2] MISSING VALUES")
    print(df.isnull().sum())

    # Statistical analysis
    print("\n[3] STATISTICAL SUMMARY")
    print(df[["temperature", "humidity"]].describe())

    # Average values
    print("\n[4] AVERAGE VALUES")
    print("Average temperature:", df["temperature"].mean())
    print("Average humidity:", df["humidity"].mean())

    # Maximum and minimum
    print("\n[5] MAXIMUM AND MINIMUM")

    print("Maximum temperature:", df["temperature"].max())
    print("Minimum temperature:", df["temperature"].min())

    print("Maximum humidity:", df["humidity"].max())
    print("Minimum humidity:", df["humidity"].min())

    # Plot temperature
    plt.figure(figsize=(10, 5))

    plt.plot(
        df["timestamp_ms"],
        df["temperature"],
        marker="o",
        label="Temperature"
    )

    plt.xlabel("Time (ms)")
    plt.ylabel("Temperature (C)")
    plt.title(f"Temperature - {server_name}")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()

    plt.show()

    # Plot humidity
    plt.figure(figsize=(10, 5))

    plt.plot(
        df["timestamp_ms"],
        df["humidity"],
        marker="o",
        label="Humidity"
    )

    plt.xlabel("Time (ms)")
    plt.ylabel("Humidity (%)")
    plt.title(f"Humidity - {server_name}")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()

    plt.show()

def compare_servers():
    print("\n================================")
    print("       SERVER COMPARISON")
    print("================================")

    python_df = pd.read_csv(PYTHON_CSV)
    golang_df = pd.read_csv(GOLANG_CSV)

    columns = [
        "device_id",
        "temperature",
        "humidity",
        "timestamp_ms"
    ]

    print("\n[1] NUMBER OF RECORDS")
    print("Python Server:", len(python_df))
    print("Golang Server:", len(golang_df))

    print("\n[2] AVERAGE TEMPERATURE")
    print("Python:", python_df["temperature"].mean())
    print("Golang:", golang_df["temperature"].mean())

    print("\n[3] AVERAGE HUMIDITY")
    print("Python:", python_df["humidity"].mean())
    print("Golang:", golang_df["humidity"].mean())

    print("\n[4] DATA COMPARISON")
    same_data = python_df[columns].equals(golang_df[columns])

    if same_data:
        print("[SUCCESS] Both servers recorded identical sensor data.")
    else:
        print("[WARNING] Sensor data is different.")

def main():

    analyze_data(PYTHON_CSV, "PYTHON SERVER")

    analyze_data(GOLANG_CSV, "GOLANG SERVER")

    compare_servers()

if __name__ == "__main__":
    main()
