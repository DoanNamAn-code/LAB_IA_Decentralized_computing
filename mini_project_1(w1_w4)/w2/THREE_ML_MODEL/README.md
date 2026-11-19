# THREE_ML_MODEL
## Huấn luyện mô hình Machine Learning và mô phỏng hệ thống ba nút

### 1. Giới thiệu

Project thực hiện việc huấn luyện ba mô hình Machine Learning cơ bản gồm K-Nearest Neighbors (KNN), Decision Tree và Support Vector Machine (SVM) để phân loại trạng thái môi trường dựa trên dữ liệu cảm biến nhiệt độ và độ ẩm.

Bên cạnh đó, project xây dựng chương trình mô phỏng ba nút trao đổi thông điệp, tích hợp kết quả dự đoán từ các mô hình Machine Learning và kiểm tra khả năng chịu lỗi khi một nút gặp sự cố.

### 2. Mục tiêu

- Tìm hiểu và thực hành ba thuật toán Machine Learning cơ bản.
- Xây dựng và xử lý dữ liệu cảm biến.
- Huấn luyện, đánh giá và lưu các mô hình bằng Scikit-learn.
- Dự đoán trạng thái môi trường từ dữ liệu cảm biến mới.
- Mô phỏng ba nút trao đổi thông điệp.
- Kiểm tra khả năng giao tiếp khi một nút gặp lỗi và khôi phục sau sự cố.

### 3. Công nghệ sử dụng

| Công nghệ | Mục đích |
|---|---|
| Python | Ngôn ngữ lập trình |
| Pandas | Đọc và xử lý dữ liệu |
| NumPy | Tính toán số học |
| Scikit-learn | Huấn luyện và đánh giá mô hình |
| Joblib | Lưu và tải mô hình |
| StandardScaler | Chuẩn hóa dữ liệu |

### 4. Cấu trúc project

```text
THREE_ML_MODEL/
│
├── dataset/
│   └── sensor_data.csv
│
├── models/
│   ├── __init__.py
│   ├── knn.py
│   ├── decision_tree.py
│   ├── svm.py
│   └── train_all.py
│
├── distributed/
│   ├── __init__.py
│   ├── node.py
│   ├── network.py
│   ├── ml_predictor.py
│   └── main.py
│
├── results/
│   ├── knn.pkl
│   ├── decision_tree.pkl
│   ├── svm.pkl
│   ├── scaler.pkl
│   └── label_encoder.pkl
│
├── main.py
├── predict.py
└── README.md
```

### 5. Nội dung thực hiện

#### 5.1. Xây dựng dữ liệu cảm biến

Dữ liệu được lưu trong `sensor_data.csv`, bao gồm:

- `Temperature`: Nhiệt độ.
- `Humidity`: Độ ẩm.
- `Status`: Trạng thái môi trường (`Normal` hoặc `Abnormal`).

#### 5.2. Huấn luyện mô hình Machine Learning

Ba thuật toán được sử dụng:

- **KNN:** Phân loại dữ liệu dựa trên các điểm dữ liệu lân cận.
- **Decision Tree:** Phân loại dựa trên cấu trúc cây quyết định.
- **SVM:** Xác định ranh giới phân chia các nhóm dữ liệu.

Dữ liệu được chia thành tập huấn luyện và tập kiểm tra theo tỷ lệ 75% và 25%. StandardScaler được sử dụng để chuẩn hóa dữ liệu trước khi huấn luyện.

Các mô hình sau khi huấn luyện được lưu trong thư mục `results/` bằng Joblib.

#### 5.3. Dự đoán dữ liệu cảm biến

Chương trình tải các mô hình đã huấn luyện, sử dụng bộ chuẩn hóa dữ liệu đã lưu và thực hiện dự đoán trạng thái môi trường từ dữ liệu nhiệt độ và độ ẩm mới.

#### 5.4. Mô phỏng hệ thống ba nút

Hệ thống gồm ba nút:

- `Node_1`
- `Node_2`
- `Node_3`

Các nút trao đổi thông điệp thông qua lớp `Network`. Thông điệp có thể chứa dữ liệu cảm biến và kết quả dự đoán từ ba mô hình Machine Learning.

#### 5.5. Mô phỏng khả năng chịu lỗi

Project kiểm tra ba tình huống:

1. **Hoạt động bình thường:** Các nút trao đổi và xử lý thông điệp.
2. **Một nút gặp sự cố:** Node_2 chuyển sang trạng thái FAILED. Node_1 và Node_3 vẫn tiếp tục giao tiếp.
3. **Khôi phục:** Node_2 chuyển lại trạng thái ACTIVE và có thể tiếp tục nhận, xử lý thông điệp.

### 6. Hướng dẫn cài đặt và chạy chương trình

**Yêu cầu:** Đã cài đặt Python và pip.

#### Bước 1. Cài đặt thư viện

```bash
pip install pandas numpy scikit-learn joblib matplotlib
```

#### Bước 2. Huấn luyện các mô hình

Đứng tại thư mục gốc `THREE_ML_MODEL`, chạy:

```bash
python models/train_all.py
```

Các mô hình và bộ xử lý dữ liệu sẽ được lưu trong thư mục `results/`.

#### Bước 3. Chạy chương trình dự đoán và mô phỏng

```bash
python distributed/main.py
```

Chương trình thực hiện dự đoán dữ liệu cảm biến, trao đổi thông điệp, mô phỏng sự cố và khôi phục nút.

### 7. Kết quả đạt được

- Huấn luyện thành công ba mô hình KNN, Decision Tree và SVM.
- Lưu và tải lại các mô hình đã huấn luyện.
- Dự đoán trạng thái môi trường từ dữ liệu cảm biến.
- Tích hợp kết quả dự đoán vào thông điệp trao đổi giữa các nút.
- Mô phỏng tình huống một nút gặp sự cố nhưng hai nút còn lại vẫn giao tiếp.
- Mô phỏng quá trình khôi phục và tiếp tục hoạt động của nút gặp sự cố.

### 8. Hạn chế và hướng phát triển

Dữ liệu hiện tại có quy mô nhỏ và được xây dựng phục vụ mục đích thực hành. Vì vậy, kết quả dự đoán chưa đủ để khẳng định khả năng tổng quát hóa trên dữ liệu thực tế.

Ngoài ra, ba nút hiện được mô phỏng bằng các đối tượng Python trong cùng một chương trình, chưa phải các thiết bị độc lập giao tiếp qua mạng thực tế.

Trong tương lai, project có thể được mở rộng bằng cách:

- Thu thập dữ liệu từ cảm biến thực tế.
- Tăng kích thước và độ đa dạng của tập dữ liệu.
- Tối ưu tham số của các mô hình Machine Learning.
- Triển khai giao tiếp giữa các nút thông qua TCP/IP.
- Xây dựng cơ chế phát hiện lỗi và khôi phục tự động.

### 9. Kết luận

Project giúp thực hành quy trình xây dựng ứng dụng Machine Learning cơ bản và tìm hiểu cách tích hợp các mô hình dự đoán vào chương trình mô phỏng hệ thống nhiều nút.

Qua đó, project cung cấp nền tảng thực hành về Machine Learning, trao đổi thông điệp và khả năng chịu lỗi trong hệ thống phân tán.