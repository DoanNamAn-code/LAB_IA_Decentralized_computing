# Sliding Window - Xử lý dữ liệu cảm biến

## 1. Giới thiệu

Dự án triển khai kỹ thuật Sliding Window (cửa sổ trượt) nhằm xử lý dữ liệu cảm biến thu thập thông qua giao tiếp TCP.

Hệ thống sử dụng hai TCP Server được viết bằng Python và Golang để nhận dữ liệu từ ESP32 Client (hiện tại sử dụng chương trình mô phỏng). Dữ liệu sau khi thu thập sẽ được xử lý bằng các kỹ thuật Sliding Window, Feature Extraction (trích xuất đặc trưng) và Labeling (gán nhãn) để tạo ra các tập dữ liệu phục vụ cho quá trình phân tích và các ứng dụng Machine Learning.

**Mục tiêu chính:**
- Thu thập dữ liệu cảm biến thông qua giao tiếp TCP.
- Xử lý dữ liệu chuỗi thời gian bằng kỹ thuật Sliding Window.
- Trích xuất các đặc trưng thống kê từ từng cửa sổ dữ liệu.
- Gán nhãn cho dữ liệu sau khi xử lý.
- Xuất tập dữ liệu dưới định dạng CSV để phục vụ các bước phân tích tiếp theo.

## 2. Cấu trúc dự án

```text
Sliding_Window/
├── ESP32/
│   └── main.c
│
├── Python_version/
│   ├── main.py
│   ├── test.py
│   └── sensor_log.csv
│
├── Golang_version/
│   ├── main.go
│   ├── test.py
│   ├── go.mod
│   └── sensor_log.csv
│
├── Analysis/
│   ├── analysis.py
│   ├── sliding_window.py
│   ├── feature_extraction.py
│   ├── labeling.py
│   ├── dataset.py
│   ├── requirements.txt
│   └── dataset/
│       ├── python_dataset.csv
│       └── golang_dataset.csv
│
└── README.md
```

## 3. Các thành phần chính

### 3.1. TCP Server

Dự án triển khai hai phiên bản TCP Server:

- **Python TCP Server:** Tiếp nhận dữ liệu cảm biến và lưu vào file `sensor_log.csv`.
- **Golang TCP Server:** Thực hiện chức năng tương tự bằng ngôn ngữ Golang.

Cả hai Server đều nhận dữ liệu cảm biến theo định dạng JSON, mỗi bản ghi kết thúc bằng ký tự xuống dòng.

Ví dụ dữ liệu:

```json
{
    "device_id": "ESP32_01",
    "temperature": 28.5,
    "humidity": 72.3,
    "timestamp_ms": 12500
}
```

Các file `test.py` được sử dụng để mô phỏng Client gửi dữ liệu cảm biến đến TCP Server.

### 3.2. Sliding Window (Cửa sổ trượt)

Sliding Window là kỹ thuật chia dữ liệu liên tục thành các cửa sổ nhỏ có thể chồng lấn lên nhau.

Các thông số hiện tại:

- Window Size: 5 mẫu dữ liệu.
- Step Size: 1 mẫu dữ liệu.

Mỗi cửa sổ chứa 5 mẫu dữ liệu liên tiếp. Sau mỗi lần dịch chuyển, cửa sổ tiến lên 1 mẫu dữ liệu mới.

### 3.3. Feature Extraction (Trích xuất đặc trưng)

Sau khi chia dữ liệu thành các cửa sổ, chương trình tiến hành tính toán các đặc trưng thống kê cho từng cửa sổ.

Các đặc trưng được trích xuất bao gồm:

| Đặc trưng | Ý nghĩa |
|---|---|
| Mean | Giá trị trung bình |
| Min | Giá trị nhỏ nhất |
| Max | Giá trị lớn nhất |
| Std | Độ lệch chuẩn |

Các đặc trưng này được tính toán cho cả hai đại lượng nhiệt độ và độ ẩm.

### 3.4. Labeling (Gán nhãn)

Mỗi cửa sổ dữ liệu được gán một nhãn nhị phân dựa trên nhiệt độ trung bình của cửa sổ.

Quy tắc gán nhãn hiện tại:

- **Label 0:** Nhiệt độ trung bình nhỏ hơn 30°C.
- **Label 1:** Nhiệt độ trung bình lớn hơn hoặc bằng 30°C.

Ngưỡng 30°C hiện được sử dụng như một quy tắc minh họa cho dự án và có thể được điều chỉnh tùy theo yêu cầu ứng dụng thực tế.

## 4. Cài đặt môi trường

### Yêu cầu

- Python 3.
- Golang.
- Git.

Cài đặt các thư viện Python cần thiết bằng lệnh:

```bash
pip install -r Analysis/requirements.txt
```

## 5. Hướng dẫn chạy chương trình

### Bước 1. Khởi chạy TCP Server

Đối với Python:

```bash
cd Python_version
python main.py
```

Đối với Golang:

```bash
cd Golang_version
go run main.go
```

### Bước 2. Chạy chương trình mô phỏng Client

Mở một cửa sổ Terminal khác và chạy file `test.py` tương ứng để gửi dữ liệu cảm biến mô phỏng đến TCP Server.

### Bước 3. Tạo tập dữ liệu

Di chuyển đến thư mục `Analysis`:

```bash
cd Analysis
python analysis.py
```

Chương trình sẽ thực hiện phân tích, so sánh dữ liệu từ hai TCP Server và tạo các tập dữ liệu thông qua quy trình Sliding Window.

## 6. Kết quả đầu ra

Các tập dữ liệu sau khi xử lý được lưu trong thư mục:

```text
Analysis/dataset/
├── python_dataset.csv
└── golang_dataset.csv
```

Mỗi tập dữ liệu bao gồm:

- Mã định danh cửa sổ (`window_id`).
- Thời điểm bắt đầu và kết thúc cửa sổ.
- Các đặc trưng thống kê của nhiệt độ.
- Các đặc trưng thống kê của độ ẩm.
- Nhãn được gán cho từng cửa sổ (`label`).
