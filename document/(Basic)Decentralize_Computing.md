# Khái niệm tính toán và trí tuệ phi tập trung

## 1. Mô hình điều khiển / tính toán

### 1.1. Tính toán tập trung (Centralized Computing)
- **Ví dụ minh họa:** Giả sử có 10 chiếc máy tính, trong đó 1 chiếc chịu trách nhiệm chính xử lý toàn bộ công việc, các máy khác chỉ việc gửi yêu cầu đến máy chính và nhận kết quả → Máy chính gọi là **Central Server** (máy chủ trung tâm).
- **Đặc điểm:**
  - Dễ quản lý, dễ kiểm soát dữ liệu.
  - Chỉ có một điểm điều khiển trung tâm; nếu máy chủ gặp sự cố thì các máy khách cũng sẽ bị ảnh hưởng (**Single Point of Failure**).

### 1.2. Tính toán phân tán (Distributed Computing)
- **Ví dụ minh họa:** Giả sử cần xử lý 1.000 bức ảnh, ta chia nhỏ cho 04 máy khác nhau xử lý sau đó tổng hợp lại kết quả → Ý tưởng cơ bản của Distributed Computing.
- **Đặc điểm:**
  - Nhiều máy tính (ở nhiều địa điểm khác nhau) cùng tham gia vào quá trình tính toán.
  - Công việc được chia nhỏ cho nhiều máy khác nhau, và các máy cùng nhau trao đổi thông tin để xử lý.
  - Khả năng xử lý tỷ lệ thuận với quy mô máy tính.

### 1.3. Tính toán phi tập trung (Decentralized Computing)
- **Ví dụ minh họa:** Giả sử có 10 máy tính kết nối với nhau, tuy nhiên không có máy nào gọi là máy chủ trung tâm, mà các máy tính đều kết nối với nhau để cùng xử lý công việc.
- **Đặc điểm:**
  - Các **node (peer)** kết nối với nhau: chia sẻ trực tiếp tài nguyên, xử lý công việc và cùng tham gia phối hợp đưa ra các quyết định chung.
  - Quyền kiểm soát được phân bổ giữa nhiều thành phần, làm giảm sự phụ thuộc vào một trung tâm quản lý duy nhất.
  - Quản lý hệ thống phức tạp.

---

## 2. Mô hình giao tiếp

### 2.1. Client – Server
- Đây là mô hình mô tả vị trí và vai trò của máy chủ và máy khách. Trong đó:
  - **Máy chủ (Server):** Nhận yêu cầu, xử lý và trả kết quả.
  - **Máy khách (Client):** Gửi yêu cầu, nhận phản hồi.

### 2.2. Peer-to-Peer (P2P)
- Mỗi máy được gọi là một **Peer** (tương đương **Node**), có nghĩa là các máy đều ngang hàng nhau.
- Mỗi Peer đều có khả năng:
  - Gửi dữ liệu cho máy khác.
  - Nhận dữ liệu từ máy khác.
  - Cung cấp tài nguyên.
  - Tham gia xử lý công việc.
