import socket
import json
import time

HOST = "127.0.0.1"
PORT = 5000

device_id = "ESP32_01"

try:
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((HOST, PORT))

    print("[CLIENT] Connected to Golang Server")

    for i in range(10):
        data = {
            "device_id": device_id,
            "temperature": 25.0 + i * 0.5,
            "humidity": 60.0 + i,
            "timestamp_ms": i * 2000
        }

        message = json.dumps(data) + "\n"

        client.sendall(message.encode("utf-8"))

        print("[SENT]", data)

        time.sleep(2)

    client.close()
    print("[CLIENT] Connection closed")

except ConnectionRefusedError:
    print("[ERROR] Cannot connect to Golang Server")

except Exception as error:
    print("[ERROR]", error)
