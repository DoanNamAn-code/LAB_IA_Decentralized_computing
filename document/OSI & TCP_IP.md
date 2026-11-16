# Mạng máy tính

## 1. Các lý thuyết cơ bản

### 1.1. Mạng máy tính là gì?
- Là hệ thống kết nối hai hoặc nhiều máy tính và các thiết bị độc lập nhằm chia sẻ dữ liệu và tài nguyên với nhau.
- **Các thành phần cơ bản của mạng máy tính:**
  - **Nút mạng (Nodes):** Là các thiết bị xử lý hoặc chuyển tiếp dữ liệu (máy tính, router, switch, modem, firewall).
  - **Liên kết (Links):** Môi trường truyền dẫn kết nối các nút, bao gồm kết nối có dây và kết nối không dây.
- Mạng máy tính sử dụng các **giao thức (Protocol)** – tập hợp các quy tắc chung chuẩn hóa về định dạng, cú pháp và cơ chế truyền dữ liệu để các thiết bị có thể hiểu và giao tiếp với nhau.

### 1.2. Kiến trúc của mạng máy tính
- Để quản lý quá trình truyền dữ liệu phức tạp, công việc được chia thành các tầng riêng biệt.
- **Có 02 mô hình tiêu chuẩn:**
  - Mô hình **OSI** (7 tầng).
  - Bộ giao thức **TCP/IP** (4 tầng).

---

## 2. Mô hình OSI

### 2.1. Nguyên tắc thiết kế mô hình OSI
- Mỗi tầng đảm nhận một nhóm chức năng chuyên biệt và cung cấp dịch vụ cho tầng liền trên, đồng thời sử dụng dịch vụ của tầng liền dưới.
- Giảm thiểu sự phụ thuộc và rò rỉ thông tin qua ranh giới các tầng.

### 2.2. Cấu trúc của mô hình OSI

- **Tầng 7: Ứng dụng (Application)**
  - **Đơn vị dữ liệu:** User Data.
  - Cung cấp giao diện trực tiếp cho người dùng và ứng dụng tương tác với dịch vụ mạng.
  - Nơi hoạt động của các giao thức ứng dụng phổ biến: HTTP, DNS, DHCP, SMTP, SSH, FTP.

- **Tầng 6: Biểu diễn (Presentation)**
  - **Đơn vị dữ liệu:** Formatted Data.
  - Đảm nhận 03 tác vụ chính: Chuyển đổi chuẩn hóa cú pháp, Nén dữ liệu và Mã hóa / Giải mã.

- **Tầng 5: Phiên (Session)**
  - **Đơn vị dữ liệu:** Formatted Data.
  - Thiết lập, duy trì, đồng bộ và kết thúc các phiên giao tiếp giữa các ứng dụng. Cụ thể là quản lý các chế độ truyền dữ liệu và gắn Session ID để theo dõi trạng thái các phiên làm việc.

- **Tầng 4: Giao vận (Transport)**
  - **Đơn vị dữ liệu:** Segment (đoạn dữ liệu).
  - Quản lý truyền dữ liệu tin cậy end-to-end giữa hai host. Phân chia dữ liệu thành các segment nhỏ khi gửi và tái lắp ráp khi nhận.
  - Cung cấp 02 giao thức cốt lõi: **TCP** và **UDP**.

- **Tầng 3: Mạng (Network)**
  - **Đơn vị dữ liệu:** Packet (gói tin).
  - Sử dụng địa chỉ IP để xác định vị trí thiết bị và tìm đường đi tối ưu cho gói tin băng qua các mạng khác nhau.
  - **Thiết bị phần cứng đặc trưng:** Router.

- **Tầng 2: Liên kết dữ liệu (Data Link)**
  - **Đơn vị dữ liệu:** Frame (khung dữ liệu).
  - Truyền các khung dữ liệu không bị lỗi giữa các nút kết nối trực tiếp trong cùng một mạng.

- **Tầng 1: Vật lý (Physical)**
  - **Đơn vị dữ liệu:** Bit.
  - Truyền chuỗi bit thô qua môi trường truyền dẫn vật lý (có dây và không dây). Đảm bảo dữ liệu bit đầu cuối không sai sót.

---

## 3. Bộ giao thức TCP/IP

### 3.1. TCP/IP là gì?
- **TCP/IP (Transmission Control Protocol / Internet Protocol)** là bộ quy tắc và giao thức chuẩn hóa cho phép các máy tính và thiết bị trao đổi dữ liệu với nhau trong Internet.
- TCP/IP không chỉ là một mô hình lý thuyết mà chính là nền tảng thực tế đang vận hành toàn bộ mạng Internet toàn cầu.
- **Nguyên lý cốt lõi:**
  - **IP (Internet Protocol):** Gán địa chỉ đầu/cuối cho gói tin và xác định đường truyền tốt nhất.
  - **TCP (Transmission Control Protocol):** Chia message (file, email, text, image, …) thành từng đoạn nhỏ khi gửi và kiểm tra, lắp ráp lại các đoạn đó khi nhận.

### 3.2. Cấu trúc của mô hình TCP/IP

- **Tầng 4: Ứng dụng (Application)**
  - Gộp ba chức năng của tầng 7, 6, 5 trong mô hình OSI.
  - Giao tiếp trực tiếp với các chương trình phần mềm và người dùng.
  - **Giao thức phổ biến:**
    - `HTTP` (truy cập web) – Port 80
    - `HTTPS` (truy cập web có xác minh SSL/TLS) – Port 443
    - `FTP` (truyền file) – Port 20/21
    - `SMTP` (gửi email) – Port 25
    - `DNS` (phân giải tên miền thành địa chỉ IP) – Port 53
    - `DHCP` (cấp phát địa chỉ IP tự động) – Port 67/68 *(Lưu ý: Port 22 là của giao thức SSH)*

- **Tầng 3: Giao vận (Transport)**
  - Quản lý việc truyền dữ liệu đầu-cuối (end-to-end) giữa hai máy tính host.
  - Cung cấp 02 giao thức cốt lõi: **TCP** và **UDP**.

- **Tầng 2: Internet**
  - Tương đương với tầng 3 của mô hình OSI.
  - Đảm nhận việc đóng gói dữ liệu thành các gói tin IP (IP datagrams) và thực hiện định tuyến (routing) để truyền gói tin qua nhiều mạng khác nhau.
  - **Giao thức chính:**
    - `IP` (đánh địa chỉ và định tuyến)
    - `ICMP` (báo lỗi và chẩn đoán mạng)
    - `ARP` (chuyển đổi địa chỉ IP sang địa chỉ MAC)
    - `IGMP` (quản lý nhóm đa hướng)

- **Tầng 1: Truy cập Mạng (Network Interface)**
  - Đảm nhận việc đưa và nhận các gói tin TCP/IP từ môi trường truyền dẫn phần cứng.
  - Gộp tầng 1 và 2 của mô hình OSI.

---

## 4. So sánh TCP và UDP

- **Giống nhau:** Đều nằm ở tầng giao vận (Transport Layer) của cả hai mô hình OSI và TCP/IP.
- **Khác nhau:**

| Tiêu chí | TCP | UDP |
| :--- | :--- | :--- |
| **Cơ chế kết nối** | Giao thức có thiết lập kết nối trước khi truyền dữ liệu. | Giao thức không kết nối, truyền dữ liệu ngay lập tức. |
| **Cơ chế kiểm soát luồng** | Có. Dữ liệu được bảo vệ. | Không. Dữ liệu chấp nhận rủi ro không bảo vệ. |
| **Tốc độ truyền tải** | Chậm hơn. | Rất nhanh. |
| **Kích thước Header** | 20 bytes | 8 bytes |
| **Ứng dụng thực tế** | Các ứng dụng đòi hỏi độ chính xác tuyệt đối (duyệt web, gửi thư điện tử, truyền file). | Các ứng dụng ưu tiên tốc độ theo thời gian thực, chấp nhận mất mát lượng nhỏ dữ liệu (xem video streaming, gaming). |

---

## 5. Giao thức HTTP

### 5.1. HTTP là gì?
- **HTTP (HyperText Transfer Protocol)** là giao thức ứng dụng dùng để truyền tải các tài nguyên siêu văn bản giữa trình duyệt web (client) và máy chủ web (server) trên nền tảng WWW – World Wide Web.

### 5.2. Tại sao HTTP lại rộng rãi?
- HTTP là giao thức phổ biến và gần gũi nhất trên Internet.
- Dễ đọc và dễ hiểu do được thiết kế dưới dạng văn bản thuần túy.
- **Tổng hợp toàn bộ các căn bản:**
  - **DNS:** Để tìm ra địa chỉ IP từ tên miền trang web.
  - **TCP:** Để tạo kết nối truyền dữ liệu tin cậy.
  - **IP:** Để định hướng gói tin đi qua các mạng.
  - **Chứng chỉ TLS/SSL (HTTPS):** Mã hóa an toàn dữ liệu.
