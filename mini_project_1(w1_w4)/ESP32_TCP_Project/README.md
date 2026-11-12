# ESP32 TCP Server Project

## 1. Overview

This project implements TCP communication between an ESP32 device (or a simulated client) and a computer acting as a TCP server.

Two server implementations are provided:
- Python TCP Server
- Go TCP Server

Both servers receive sensor data in JSON format and save the data into CSV files for further analysis.

## 2. Project Structure

```text
ESP32_TCP_Project/
├── .gitignore
├── Analysis/
│   ├── analysis.py
│   └── requirements.txt
├── ESP32/
│   └── main.c
├── Golang_version/
│   ├── go.mod
│   ├── main.go
│   ├── test.py
│   └── sensor_log.csv
└── Python_version/
    ├── main.py
    ├── test.py
    └── sensor_log.csv
```

## 3. Data Format

The TCP servers receive newline-delimited JSON messages.

Example:

```json
{"device_id":"ESP32_01","temperature":28.5,"humidity":72.3,"timestamp_ms":12500}
```

## 4. Technologies

- Python
- Go (Golang)
- TCP/IP Socket Programming
- JSON
- CSV
- Pandas
- Matplotlib
- ESP32 / Embedded C

## 5. Data Analysis

The Analysis module uses Pandas and Matplotlib to:
- Read sensor data from CSV files.
- Calculate basic statistical values.
- Visualize temperature and humidity data.

Install Python dependencies:

```bash
pip install -r requirements.txt
```

## 6. Testing

Python test clients are provided to simulate ESP32 sensor data transmission.

**Note:** The TCP communication has been tested using simulated Python clients. Physical ESP32 hardware testing has not yet been completed.

## 7. Future Improvements

- Test communication with a physical ESP32 device.
- Compare Python and Go TCP server performance.
- Extend sensor data collection and visualization.
