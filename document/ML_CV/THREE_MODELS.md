# BA MÔ HÌNH MACHINE LEARNING

## 1. MÔ HÌNH DECISION TREE (CÂY QUYẾT ĐỊNH)

* **Nguyên lý:**
  * Mô hình chia nhỏ tập dữ liệu thành các nhánh dựa trên chuỗi các câu hỏi điều kiện `True / False` của từng đặc trưng.
  * Sử dụng một nút là nút gốc (**Root Node**), sau đó đi qua các nút nhánh và đi đến các nút lá (**Leaf Node**) chứa kết quả dự đoán cuối cùng.
* **Đặc điểm:**
  * **Ưu điểm:** Rất dễ đọc hiểu logic và không cần chuẩn hóa đơn vị đo dữ liệu.
  * **Nhược điểm:** Nếu có quá nhiều nút nhánh (phân nhánh quá sâu) rất dễ bị *overfitting* (dự đoán kém hiệu quả).

---

## 2. MÔ HÌNH K-NEAREST NEIGHBORS (LÁNG GIỀNG GẦN NHẤT)

* **Nguyên lý:**
  * KNN còn gọi là thuật toán học lười (**Lazy Learning**), nghĩa là nó không xây dựng công thức toán học trong lúc huấn luyện mà ghi nhớ toàn bộ dữ liệu gốc.
  * Khi xuất hiện một điểm dữ liệu mới, KNN sẽ tìm ra các điểm có khoảng cách gần nhất so với điểm vừa xuất hiện trong toàn bộ dữ liệu, sau đó lấy nhãn chiếm đa số (bài toán phân loại) hoặc tính trung bình cộng (bài toán hồi quy).
* **Đặc điểm:**
  * **Ưu điểm:** Nguyên lý trực quan và không cần giả định trước về phân phối dữ liệu.
  * **Nhược điểm:** Dự đoán chậm khi dữ liệu lớn và bắt buộc phải chuẩn hóa tỷ lệ dữ liệu.

---

## 3. MÔ HÌNH SUPPORT VECTOR MACHINE (MÁY VECTOR HỖ TRỢ)

* **Nguyên lý:**
  * Mô hình sẽ tìm một ranh giới phân chia tối ưu để tách riêng các nhóm dữ liệu.
  * **Phương pháp chọn đường phân cách:**
    * Mô hình sẽ chọn đường phân cách là đường nằm giữa dữ liệu sao cho khoảng cách từ đường đó đến các điểm dữ liệu nằm sát biên nhất của mỗi nhóm là rộng nhất có thể.
    * Các dữ liệu nằm sát đường ranh giới nhất gọi là **Support Vectors**.
    * Nếu dữ liệu đan xen nhau và không thể phân tách thành đường thẳng, mô hình sẽ dùng kỹ thuật **Kernel Trick** để chiếu dữ liệu lên không gian nhiều chiều hơn nhằm tìm mặt phẳng để phân tích.
* **Đặc điểm:**
  * **Ưu điểm:** Rất mạnh khi dữ liệu có nhiều chiều đặc trưng (như phân loại văn bản, nhận diện ảnh) và ít bị nhiễu bởi các điểm nằm xa biên.
  * **Nhược điểm:** Huấn luyện chậm trên tập dữ liệu lớn và cần chuẩn hóa dữ liệu.
