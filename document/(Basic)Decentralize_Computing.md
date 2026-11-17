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
---
## 3. Định lý CAP và Nhất quán cuối cùng
- **Định lý CAP**:
  - **Consistency**: Mọi thiết bị biên đều đọc được cùng một dữ liệu tại cùng một thời điểm cụ thể
  - **Availability**: Hệ thống luôn tiếp nhận và phản hồi yêu cầu kể cả quá tải
  - **Partition Tolerance**: Hệ thống vẫn vận hành bình thường kể cả khi đường truyền mạng giữa các thiết bị bị đứt gãy
  
Định lý phát biểu rằng chỉ có thể hiện thực tối đa 2 trong 3 yếu tố kể trên \
**Lưu ý**: Trong thực tế, kết nối mạng không ổn định nên chữ P được ưu tiên, vậy nên ta phải chọn giữa C và A. Để giải quyết hiện tượng này người ta đề ra giải pháp: **Nhất quán cuối cùng**

- **Nhất quán cuối cùng**:
  Chấp nhận tại một thời điểm dữ liệu lệch nhau, nhưng sau một khoảng thời gian truyền tải tất cả các nút sẽ hội tụ về cùng một trạng thái đồng nhất
- **Đồng bộ thời gian và xử lý lỗi**:
  - Các kỹ sư sử dụng thuật toán thứ tự sự kiện (như Lamport Timestamps hay Vector Clocks) thay vì dùng giờ hệ thống
  - Cơ chế lỗi và phục hồi: Liên tục bắn tín hiệu nhịp tim để phát hiện một nút biên đã chết hay sống để biết đường định tuyến sang các nút khác

