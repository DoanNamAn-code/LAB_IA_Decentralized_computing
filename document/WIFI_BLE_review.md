## 1. Kết nối Wi-Fi

### a) Chế độ hoạt động
- **STA (Station):** Vi điều khiển đóng vai trò là một máy trạm (client), đi dò tìm và kết nối vào một Access Point (AP) hay Router có sẵn để ra Internet hoặc vào mạng LAN nội bộ.
- **AP (Access Point):** Vi điều khiển tự phát ra một mạng Wi-Fi độc lập (Soft-AP) để các thiết bị khác (laptop, điện thoại) kết nối vào. Thường dùng để thiết lập captive portal (giao diện web nội bộ) cho việc nạp SSID/Password cấu hình mạng lần đầu. 
- **ESP32 có khả năng chạy đồng thời cả hai (chế độ `STA_AP`).** 
### b) Quá trình kết nối
Việc một Node liên kết với Router không diễn ra tức thời mà trải qua một máy trạng thái (state machine) nghiêm ngặt:

1. **Scanning (Quét):** Chủ động gửi Probe Request ra mọi kênh sóng, hoặc thụ động lắng nghe Beacon frames từ Router phát ra định kỳ.
2. **Authentication (Xác thực ban đầu):** Thường là chuẩn Open System gửi đi để xác nhận thiết bị có quyền nói chuyện với Router.
3. **Association (Liên kết):** Node gửi thông số băng thông, tốc độ truyền tải hỗ trợ để chính thức gia nhập vào cell mạng (BSS).
   - *Cell mạng:* Là vùng phủ sóng không dây của một router duy nhất, tham gia vô là chấp nhận vô chung một kênh tần số vô tuyến và đồng bộ thời gian.
4. **4-Way Handshake (Bắt tay 4 bước):** Sinh khóa mã hóa phiên làm việc từ mật khẩu Wi-Fi. *(Nếu nhận được mã lỗi 15 hoặc 204 là lỗi ở đây)*
5. **DHCP:** Đàm phán để nhận địa chỉ IP động, Subnet Mask và Gateway từ Router. *(Trên không lỗi chờ mãi không chọn thì lỗi ở đây)*

- **WPA2 (Cơ chế giải đố PSK):** Router gửi một chuỗi ký tự ngẫu nhiên ra không khí. Node dùng mật khẩu Wi-Fi để mã hóa chuỗi đó và gửi đáp án lại. Nếu khớp, Router mở mạng.
- **WPA3 (Đàm phán toán học SAE):** Loại bỏ hoàn toàn trò giải đố. Node và Router trao đổi các mảnh mã hóa động. Cả hai tự dùng mật khẩu nội bộ làm chất xúc tác cùng với các mảnh dữ liệu này để tính ra một chìa khóa chung (thuật toán Dragonfly).

---

## 2. Kết nối BLE

BLE được thiết kế tối ưu điện năng (ngủ sâu, thức dậy mili-giây để phát/nhận rồi ngủ tiếp), hoàn toàn tách biệt với Bluetooth Classic. Nó được chia làm hai tầng logic cốt lõi: **GAP** và **GATT**.

### a) GAP
GAP quy định cách các thiết bị "phát hiện" và "kết nối" với nhau. Quá trình này xoay quanh cơ chế **Quảng bá (Advertising)**.

- **Advertising:** Thiết bị định kỳ bắn ra các gói tin nhỏ (tối đa 31 bytes) trên 3 kênh vô tuyến (kênh 37, 38, 39) để tránh dải tần Wi-Fi.
- **4 Vai trò GAP:**
  1. **Broadcaster:** Chỉ ném dữ liệu quảng bá ra không khí, không cho ai kết nối (ví dụ: Apple iBeacon theo dõi vị trí).
  2. **Observer:** Chỉ chực chờ quét sóng để nghe gói tin quảng bá (ứng với bài thực hành "quét thiết bị BLE" trên vi điều khiển).
  3. **Peripheral (Ngoại vi):** Phát quảng bá và mở cửa cho phép thiết bị khác kết nối vào. Đóng vai trò là nguồn dữ liệu (cảm biến, ESP32).
  4. **Central (Trung tâm):** Quét tìm Peripheral và chủ động gửi yêu cầu thiết lập kết nối (điện thoại, máy tính).

### b) GATT
GATT chỉ bắt đầu hoạt động sau khi hai thiết bị đã kết nối thành công thông qua GAP. Nó tổ chức cách lưu trữ và trao đổi dữ liệu dưới dạng cấu trúc phân tầng:

- **GATT Server:** Thiết bị nắm giữ dữ liệu (thường là Peripheral).
- **GATT Client:** Thiết bị gửi lệnh đi đọc/ghi dữ liệu (thường là Central).
- **Cấu trúc dữ liệu:**
  - **Profile:** Tập hợp nhiều Services.
  - **Service (Dịch vụ):** Nhóm các chức năng liên quan (VD: Battery Service, Heart Rate Service). Được định danh bằng một mã UUID (16-bit cho chuẩn hóa, 128-bit cho dịch vụ tự chế).
  - **Characteristic (Đặc tính):** Bản chất là một vùng nhớ (biến) lưu giá trị cụ thể nằm trong Service. Nó đi kèm các Quyền (Properties) như: `Read` (Đọc), `Write` (Ghi), và cực kỳ quan trọng là `Notify` (Thông báo - MCU tự động đẩy data lên điện thoại khi giá trị thay đổi mà không cần điện thoại đi polling hỏi liên tục).
