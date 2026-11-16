# Tổng kết kiến thức tuần 2

## 1. Python – Hướng đối tượng và kiểm thử

### 1.1. Lập trình hướng đối tượng với `class`
- Tạo lớp mô hình hóa thiết bị cảm biến.
- Sử dụng đối tượng để lưu trữ các thông tin của cảm biến: `device_id`, `temperature`, `humidity` và `timestamp_ms`.

### 1.2. Kiểm thử bằng `pytest`
- Sử dụng `pytest` để kiểm tra hoạt động của lớp cảm biến.
- Kiểm tra dữ liệu và kết quả do chương trình tạo ra.

## 2. Go – Interface và kiểm thử

### 2.1. Method và cấu trúc dữ liệu
- Sử dụng `struct` để biểu diễn thông tin của thiết bị cảm biến.
- Xây dựng và sử dụng các method để xử lý dữ liệu cảm biến.

### 2.2. Kiểm thử bằng `go test`
- Viết và chạy kiểm thử cho phần mô hình hóa cảm biến bằng lệnh `go test`.
- Đối chiếu kết quả tạo dữ liệu giữa phiên bản Go và Python.

## 3. ML/CV trên thiết bị biên – Dữ liệu cảm biến

### 3.1. Cửa sổ trượt (Sliding Window)
- Chia chuỗi dữ liệu cảm biến thành các cửa sổ có kích thước cố định.
- Sử dụng cửa sổ có kích thước 5 mẫu và bước trượt 1 mẫu.
- Với 10 bản ghi đầu vào, tạo được 6 cửa sổ dữ liệu.

### 3.2. Trích xuất đặc trưng thống kê
- Tính các đặc trưng thống kê cho nhiệt độ và độ ẩm trong từng cửa sổ:
  - Giá trị trung bình (`mean`).
  - Giá trị nhỏ nhất (`min`).
  - Giá trị lớn nhất (`max`).
  - Độ lệch chuẩn (`std`).

### 3.3. Gán nhãn dữ liệu
- Tạo nhãn cho từng cửa sổ dựa trên giá trị nhiệt độ trung bình.
- Lưu tập dữ liệu sau khi xử lý thành các tệp CSV để phục vụ việc phân tích và sử dụng ở các bước tiếp theo.
