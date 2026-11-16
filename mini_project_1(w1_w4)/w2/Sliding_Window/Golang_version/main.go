package main

import (
	"bufio"
	"encoding/csv"
	"encoding/json"
	"fmt"
	"net"
	"os"
	"strconv"
	"sync"
	"time"
)

const (
	host    = "0.0.0.0"
	port    = "5000"
	logFile = "sensor_log.csv"
)

type SensorData struct {
	DeviceID    string  `json:"device_id"`
	Temperature float64 `json:"temperature"`
	Humidity    float64 `json:"humidity"`
	TimestampMS int64   `json:"timestamp_ms"`
}

var fileMutex sync.Mutex

// Initialize CSV log file
func initLog() error {
	file, err := os.OpenFile(
		logFile,
		os.O_CREATE|os.O_APPEND|os.O_WRONLY,
		0644,
	)

	if err != nil {
		return err
	}

	defer file.Close()

	info, err := file.Stat()
	if err != nil {
		return err
	}

	// Write header if file is empty
	if info.Size() == 0 {
		writer := csv.NewWriter(file)

		header := []string{
			"received_at",
			"device_id",
			"temperature",
			"humidity",
			"timestamp_ms",
		}

		if err := writer.Write(header); err != nil {
			return err
		}

		writer.Flush()

		if err := writer.Error(); err != nil {
			return err
		}

		fmt.Println("[SUCCESS] CSV file created:", logFile)
	} else {
		fmt.Println("[INFO] CSV file already exists:", logFile)
	}

	return nil
}

// Write sensor data to CSV
func writeLog(data SensorData) error {
	fileMutex.Lock()
	defer fileMutex.Unlock()

	file, err := os.OpenFile(
		logFile,
		os.O_APPEND|os.O_WRONLY,
		0644,
	)

	if err != nil {
		return err
	}

	defer file.Close()

	writer := csv.NewWriter(file)

	record := []string{
		time.Now().Format("2006-01-02 15:04:05"),
		data.DeviceID,
		strconv.FormatFloat(data.Temperature, 'f', 2, 64),
		strconv.FormatFloat(data.Humidity, 'f', 2, 64),
		strconv.FormatInt(data.TimestampMS, 10),
	}

	if err := writer.Write(record); err != nil {
		return err
	}

	writer.Flush()

	return writer.Error()
}

// Handle TCP client
func handleClient(conn net.Conn) {
	defer conn.Close()

	fmt.Println("[CONNECTED]", conn.RemoteAddr())

	scanner := bufio.NewScanner(conn)

	for scanner.Scan() {
		message := scanner.Bytes()

		var data SensorData

		if err := json.Unmarshal(message, &data); err != nil {
			fmt.Println("[ERROR] Invalid JSON:", err)
			continue
		}

		fmt.Printf(
			"[DATA] Device: %s | Temperature: %.2f | Humidity: %.2f\n",
			data.DeviceID,
			data.Temperature,
			data.Humidity,
		)

		if err := writeLog(data); err != nil {
			fmt.Println("[ERROR] Cannot write CSV:", err)
			continue
		}

		fmt.Println("[SUCCESS] Data saved to CSV")
	}

	if err := scanner.Err(); err != nil {
		fmt.Println("[ERROR] Connection:", err)
	}

	fmt.Println("[DISCONNECTED]", conn.RemoteAddr())
}

// Main function
func main() {
	fmt.Println("================================")
	fmt.Println("        GOLANG TCP SERVER")
	fmt.Println("================================")

	if err := initLog(); err != nil {
		fmt.Println("[FATAL] CSV initialization failed:", err)
		return
	}

	address := net.JoinHostPort(host, port)

	listener, err := net.Listen("tcp", address)

	if err != nil {
		fmt.Println("[FATAL] Cannot start server:", err)
		return
	}

	defer listener.Close()

	fmt.Println("[SERVER] Listening on", address)
	fmt.Println("[SERVER] Waiting for ESP32 connections...")

	for {
		conn, err := listener.Accept()

		if err != nil {
			fmt.Println("[ERROR] Accept connection:", err)
			continue
		}

		go handleClient(conn)
	}
}
