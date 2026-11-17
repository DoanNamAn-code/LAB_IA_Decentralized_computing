# ESP32 TCP Server Project

## 1. Tổng quan

Dự án triển khai giao tiếp TCP giữa thiết bị ESP32 (hoặc một client mô phỏng) và máy tính đóng vai trò là TCP Server.

Dự án cung cấp hai phiên bản TCP Server:
- Python TCP Server
- Go (Golang) TCP Server

Cả hai Server đều có chức năng nhận dữ liệu cảm biến dưới định dạng JSON và lưu dữ liệu vào file CSV để phục vụ quá trình phân tích.

## 2. Cấu trúc dự án

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

## 3. Định dạng dữ liệu

Các TCP Server nhận dữ liệu theo định dạng JSON, mỗi thông điệp kết thúc bằng ký tự xuống dòng (newline-delimited JSON).

Ví dụ:

```json
{"device_id":"ESP32_01","temperature":28.5,"humidity":72.3,"timestamp_ms":12500}
```

Trong đó:
- `device_id`: Mã định danh thiết bị.
- `temperature`: Nhiệt độ đo được.
- `humidity`: Độ ẩm đo được.
- `timestamp_ms`: Thời gian ghi nhận dữ liệu (milliseconds).

## 4. Công nghệ sử dụng

- Python
- Go (Golang)
- TCP/IP Socket Programming
- JSON
- CSV
- Pandas
- Matplotlib
- ESP32 / Embedded C

## 5. Phân tích dữ liệu

Module `Analysis` sử dụng Pandas và Matplotlib để:

- Đọc dữ liệu cảm biến từ các file CSV.
- Tính toán các giá trị thống kê cơ bản.
- Trực quan hóa dữ liệu nhiệt độ và độ ẩm thông qua biểu đồ.

Cài đặt các thư viện Python cần thiết:

```bash
pip install -r requirements.txt
```

## 6. Kiểm thử hệ thống

Dự án cung cấp các Python Test Client nhằm mô phỏng ESP32 gửi dữ liệu cảm biến đến TCP Server.

**Lưu ý:** Giao tiếp TCP hiện đã được kiểm thử bằng Python Client mô phỏng. Việc kiểm thử trên phần cứng ESP32 thực tế vẫn chưa được thực hiện.

## 7. Định hướng phát triển

- Kiểm thử giao tiếp với phần cứng ESP32 thực tế.
- So sánh hiệu năng giữa Python TCP Server và Go TCP Server.
- Mở rộng khả năng thu thập và trực quan hóa dữ liệu cảm biến.
