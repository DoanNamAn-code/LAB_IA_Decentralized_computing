# Tổng kết kiến thức tuần 2

## 1. Python -- Hướng đối tượng và kiểm thử

### 1.1. Lập trình hướng đối tượng với `class`

-   Sử dụng `class` để mô hình hóa thiết bị cảm biến.
-   Lưu trữ các thông tin: `device_id`, `temperature`, `humidity` và
    `timestamp_ms`.
-   Xây dựng các phương thức để kiểm tra và xử lý dữ liệu cảm biến.

### 1.2. Kiểm thử bằng `pytest`

-   Sử dụng `pytest` để kiểm tra hoạt động của lớp cảm biến.
-   Kiểm tra dữ liệu và kết quả xử lý.

## 2. Go -- Interface và kiểm thử

### 2.1. Method và cấu trúc dữ liệu

-   Sử dụng `struct` để biểu diễn thông tin của thiết bị cảm biến.
-   Xây dựng các method để kiểm tra và xử lý dữ liệu cảm biến.

### 2.2. Kiểm thử bằng `go test`

-   Viết và chạy kiểm thử cho chương trình bằng lệnh `go test`.
-   Đối chiếu kết quả tạo dữ liệu giữa phiên bản Go và Python.

## 3. ML và CV -- Thuật toán nền tảng

### 3.1. Các thuật toán học máy

-   Thực hành ba thuật toán phân loại:
    -   K-Nearest Neighbors (KNN).
    -   Decision Tree (Cây quyết định).
    -   Support Vector Machine (SVM).
-   Sử dụng dữ liệu nhiệt độ và độ ẩm để dự đoán trạng thái cảm biến
    (`Normal` hoặc `Abnormal`).

### 3.2. Quy trình huấn luyện và đánh giá mô hình

-   Sử dụng `pandas` để đọc dữ liệu từ tệp CSV.
-   Chia dữ liệu thành tập huấn luyện và tập kiểm tra theo tỷ lệ 75/25.
-   Sử dụng `StandardScaler` để chuẩn hóa dữ liệu.
-   Sử dụng `LabelEncoder` để mã hóa nhãn.
-   Đánh giá mô hình bằng Accuracy Score.

### 3.3. Lưu trữ và sử dụng mô hình

-   Sử dụng `joblib` để lưu các mô hình đã huấn luyện thành tệp `.pkl`.
-   Nạp lại mô hình đã lưu để dự đoán dữ liệu cảm biến mới.

## 4. ML/CV trên thiết bị biên -- Dữ liệu cảm biến

### 4.1. Cửa sổ trượt (Sliding Window)

-   Chia chuỗi dữ liệu cảm biến thành các cửa sổ có kích thước cố định.
-   Sử dụng cửa sổ kích thước 5 mẫu và bước trượt 1 mẫu.
-   Với 10 bản ghi đầu vào, tạo được 6 cửa sổ dữ liệu.

### 4.2. Trích xuất đặc trưng thống kê

-   Tính các đặc trưng thống kê cho nhiệt độ và độ ẩm:
    -   Giá trị trung bình (`mean`).
    -   Giá trị nhỏ nhất (`min`).
    -   Giá trị lớn nhất (`max`).
    -   Độ lệch chuẩn (`std`).

### 4.3. Gán nhãn và lưu dữ liệu

-   Tạo nhãn cho từng cửa sổ dựa trên nhiệt độ trung bình.
-   Lưu dữ liệu đã xử lý thành tệp CSV để phục vụ huấn luyện mô hình.

## 5. Tính toán phi tập trung -- Hệ phân tán

### 5.1. Mô phỏng hệ thống nhiều nút

-   Xây dựng mô hình gồm 3 nút Python (Node1, Node2, Node3).
-   Mô phỏng quá trình các nút trao đổi và xử lý bản tin chứa dữ liệu
    cảm biến cùng kết quả dự đoán.

### 5.2. Xử lý lỗi và phục hồi

-   Mô phỏng tình huống Node2 gặp sự cố và ngừng hoạt động. Đồng thời kiểm tra khả năng tiếp tục trao đổi dữ liệu giữa các nút còn hoạt động hong.
-   Khôi phục Node2 và kiểm tra khả năng tiếp nhận bản tin trở lại.

### 5.3. Tích hợp mô hình học máy

-   Xây dựng `MLPredictor` để nạp và sử dụng ba mô hình đã huấn luyện.
-   Tích hợp kết quả dự đoán vào bản tin trao đổi giữa các nút.

**Lưu ý:** Mô hình ba nút hiện được mô phỏng trong cùng một chương trình
Python, chưa phải hệ thống phân tán thực tế qua mạng. Các mô hình học
máy được huấn luyện trên máy tính, chưa triển khai trực tiếp lên ESP32.
