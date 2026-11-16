package main

import "testing"

//Kiem tra sensor hop le
func TestSensorValid(t *testing.T) {
	sensor := Sensor{
		DeviceID:    "ESP32_01",
		Temperature: 28.5,
		Humidity:    72.3,
		TimestampMs: 12500,
	}

	if !sensor.IsValid() {
		t.Error("Sensor not valid") //Neu hop le thi khong thong bao loi
	}
}

//Kiem tra khong hop le
func TestSensorInvalidHumidity(t *testing.T) {
	sensor := Sensor{
		DeviceID:    "ESP32_01",
		Temperature: 28.5,
		Humidity:    150,
		TimestampMs: 12500,
	}

	if sensor.IsValid() {
		t.Error("Sensor not valid") //Neu khong hop le thi thong bao loi
	}
}

func TestSensorInvalidTemperature(t *testing.T) {
	sensor := Sensor{
		DeviceID:    "ESP32_01",
		Temperature: 100,
		Humidity:    72.3,
		TimestampMs: 12500,
	}

	if sensor.IsValid() {
		t.Error("Sensor not valid")
	}
}

func TestSensorEmptyID(t *testing.T) {
	sensor := Sensor{
		DeviceID:    "",
		Temperature: 28.5,
		Humidity:    72.3,
		TimestampMs: 12500,
	}

	if sensor.IsValid() {
		t.Error("Sensor not valid")
	}
}
