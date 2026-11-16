from sensor import Sensor


def test_sensor_creation():
    sensor = Sensor("ESP32_01", 28.5, 72.3, 12500)

    assert sensor.device_id == "ESP32_01"
    assert sensor.temperature == 28.5
    assert sensor.humidity == 72.3
    assert sensor.timestamp_ms == 12500


def test_sensor_valid():
    sensor = Sensor("ESP32_01", 28.5, 72.3, 12500)

    assert sensor.is_valid() == True


def test_sensor_invalid_humidity():
    sensor = Sensor("ESP32_01", 28.5, 150, 12500)

    assert sensor.is_valid() == False


def test_sensor_invalid_temperature():
    sensor = Sensor("ESP32_01", 100, 72.3, 12500)

    assert sensor.is_valid() == False


def test_sensor_to_dict():
    sensor = Sensor("ESP32_01", 28.5, 72.3, 12500)

    data = sensor.to_dict()

    assert data["device_id"] == "ESP32_01"
    assert data["temperature"] == 28.5
    assert data["humidity"] == 72.3
