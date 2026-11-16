package main

type Sensor struct {
	DeviceID    string  `json:"device_id"`
	Temperature float64 `json:"temperature"`
	Humidity    float64 `json:"humidity"`
	TimestampMs int64   `json:"timestamp_ms"`
}

func (s Sensor) IsValid() bool {
	return s.DeviceID != "" &&
		s.Temperature >= -40 &&
		s.Temperature <= 85 &&
		s.Humidity >= 0 &&
		s.Humidity <= 100 &&
		s.TimestampMs >= 0
}
