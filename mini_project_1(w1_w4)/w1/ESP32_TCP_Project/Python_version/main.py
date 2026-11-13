import socket
import json
import csv
import os
from datetime import datetime

# TCP Server configuration
HOST = "0.0.0.0"
PORT = 5000

# CSV log file
LOG_FILE = "sensor_log.csv"


def init_log():
    """Create CSV file if it does not exist."""

    if not os.path.exists(LOG_FILE):
        with open(LOG_FILE, "w", newline="", encoding="utf-8") as file:
            writer = csv.writer(file)

            writer.writerow([
                "received_at",
                "device_id",
                "temperature",
                "humidity",
                "timestamp_ms"
            ])

        print(f"[SUCCESS] Created {LOG_FILE}")
    else:
        print(f"[INFO] {LOG_FILE} already exists")


def write_log(data):
    """Append sensor data to CSV."""

    with open(LOG_FILE, "a", newline="", encoding="utf-8") as file:
        writer = csv.writer(file)

        writer.writerow([
            datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
            data["device_id"],
            data["temperature"],
            data["humidity"],
            data["timestamp_ms"]
        ])


def handle_client(conn, addr):
    """Receive and process data from ESP32."""

    print(f"[CONNECTED] {addr}")

    try:
        buffer = ""

        while True:
            received = conn.recv(1024)

            if not received:
                break

            buffer += received.decode("utf-8")

            while "\n" in buffer:
                message, buffer = buffer.split("\n", 1)

                if not message.strip():
                    continue

                try:
                    data = json.loads(message)

                    print(
                        f"[DATA] Device: {data['device_id']} | "
                        f"Temperature: {data['temperature']} | "
                        f"Humidity: {data['humidity']}"
                    )

                    write_log(data)

                    print("[SUCCESS] Data saved to CSV")

                except (json.JSONDecodeError, KeyError, TypeError) as error:
                    print(f"[ERROR] Invalid data: {error}")

    except ConnectionError as error:
        print(f"[ERROR] Connection lost: {error}")

    finally:
        conn.close()
        print(f"[DISCONNECTED] {addr}")


def main():
    print("================================")
    print("       PYTHON TCP SERVER")
    print("================================")

    init_log()

    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    server.bind((HOST, PORT))
    server.listen()

    print(f"[SERVER] Listening on {HOST}:{PORT}")
    print("[SERVER] Waiting for ESP32 connections...")

    try:
        while True:
            conn, addr = server.accept()
            handle_client(conn, addr)

    except KeyboardInterrupt:
        print("\n[SERVER] Stopped")

    finally:
        server.close()


if __name__ == "__main__":
    main()
