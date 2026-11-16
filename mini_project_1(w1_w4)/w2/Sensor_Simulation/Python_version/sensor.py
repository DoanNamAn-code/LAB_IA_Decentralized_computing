class Sensor:

    def __init__(self, device_id, temperature, humidity, timestamp_ms):
        self.device_id = device_id
        self.temperature = temperature
        self.humidity = humidity
        self.timestamp_ms = timestamp_ms

    def is_valid(self):
        return (
            -40 <= self.temperature <= 85
            and 0 <= self.humidity <= 100
            and self.timestamp_ms >= 0
            and self.device_id != ""
        )

    def to_dict(self):
        return {
            "device_id": self.device_id,
            "temperature": self.temperature,
            "humidity": self.humidity,
            "timestamp_ms": self.timestamp_ms
        }
