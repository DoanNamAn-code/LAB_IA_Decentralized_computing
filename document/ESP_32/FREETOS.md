# ESP32 – FreeRTOS

Khi lập trình ESP32 cơ bản, mã nguồn chạy tuần tự trong một vòng lặp `loop()`, lệnh trước chặn lệnh sau. Trong môi trường chuyên nghiệp, ESP32 sử dụng hệ điều hành thời gian thực **FreeRTOS** để chia chương trình thành nhiều tác vụ độc lập chạy song song.

---

## 1. Task (Tác vụ)

- **Task (Tác vụ)** là đơn vị thực thi nhỏ nhất trong FreeRTOS.
- Mỗi task là một hàm có vòng lặp vô tận riêng (`while(1)`), có bộ nhớ **Stack** riêng.
- Task tồn tại ở một trong 4 trạng thái:
  - **Running:** Đang chạy.
  - **Ready:** Sẵn sàng chờ CPU.
  - **Blocked:** Đang ngủ chờ thời gian hoặc dữ liệu.
  - **Suspended:** Bị tạm khóa.
- Để các Task có thể phối hợp với nhau mà không gây xung đột, FreeRTOS cung cấp 4 công cụ quản lý chính:

| Cơ chế | Bản chất | Mục đích sử dụng chính | Ví dụ thực tế |
| :--- | :--- | :--- | :--- |
| **Queue** *(Hàng đợi)* | Bộ đệm truyền dữ liệu theo nguyên tắc **FIFO** (vào trước ra trước), sao chép theo giá trị. | Truyền gói dữ liệu an toàn từ task này sang task khác. | Task đọc cảm biến gửi `struct {nhiệt_độ, độ_ẩm}` sang Task xử lý. |
| **Semaphore** *(Cờ hiệu)* | Biến đếm tín hiệu (*Binary*: 0/1 hoặc *Counting*: 0..N), không chứa dữ liệu. | Đồng bộ hóa thứ tự thực thi (báo hiệu sự kiện đã xảy ra). | Ngắt (ISR) báo hiệu cho Task biết dữ liệu từ cảm biến đã sẵn sàng. |
| **Mutex** *(Khóa loại trừ)* | Giống Binary Semaphore nhưng có quyền sở hữu và cơ chế *Priority Inheritance* (kế thừa ưu tiên). | Bảo vệ tài nguyên dùng chung, tránh 2 task truy cập cùng lúc. | Khóa bus I2C hoặc cổng UART khi nhiều task cùng muốn ghi dữ liệu. |
| **Event Group** *(Nhóm sự kiện)* | Tập hợp các bit cờ (24-bit trên ESP32). | Chờ nhiều điều kiện cùng lúc theo logic **AND** (đủ tất cả) hoặc **OR** (một trong số đó). | Task gửi dữ liệu chỉ chạy khi *Bit 0 (có Wi-Fi)* **AND** *Bit 1 (đã đọc cảm biến)* cùng bật. |

---

## 2. Lập lịch hai lõi & Ưu tiên tác vụ

### 2.1. Kiến trúc hai lõi (Dual-Core SMP)
- Chip ESP32 sở hữu hai nhân xử lý vật lý giống nhau:
  - **Core 0 (`PRO_CPU`):** Mặc định xử lý các tác vụ hệ thống và giao thức mạng (Wi-Fi, Bluetooth).
  - **Core 1 (`APP_CPU`):** Thường dùng để chạy mã ứng dụng của người dùng.
- Khi khởi tạo bằng hàm `xTaskCreatePinnedToCore()`, bạn có thể ghim task chạy cố định trên Core `0`, Core `1`, hoặc chọn `tskNO_AFFINITY` để bộ lập lịch tự điều phối linh hoạt giữa hai lõi *(lưu ý: theo tài liệu ESP-IDF, các task có phép tính số thực `float` nên được ghim cố định vào một lõi)*.

### 2.2. Ưu tiên tác vụ (Task Priority) & Lập lịch chiếm quyền (Preemptive)
- **Quy tắc số:** Số càng **lớn** thì độ ưu tiên càng **cao** (thấp nhất là `0` – dành cho *Idle Task*, cao nhất mặc định trong ESP-IDF là `24`).
- **Chiếm quyền (Preemption):** Tại mỗi thời điểm, mỗi lõi luôn chọn task ở trạng thái *Ready* có độ ưu tiên cao nhất để chạy. Nếu một task ưu tiên cao vừa thức dậy, nó sẽ lập tức ngắt ngang task ưu tiên thấp đang chạy.
- **Chia sẻ thời gian (Time Slicing):** Các task có cùng mức ưu tiên sẽ chạy luân phiên theo từng lát cắt thời gian (*Round-Robin*, mặc định mỗi nhịp tick là 10ms).
- **Nguyên tắc bắt buộc:** Mọi task đều phải có lệnh nhường CPU để rơi vào trạng thái *Blocked*. Nếu một task ưu tiên cao chạy vòng lặp kín không nghỉ, nó sẽ làm "chết đói" (starvation) các task khác và kích hoạt bộ giám sát Watchdog làm reset chip.

---

## 3. NVS và Phân vùng bộ nhớ Flash

Bộ nhớ RAM trên ESP32 sẽ mất sạch dữ liệu khi mất điện. Để lưu trữ lâu dài, dữ liệu được ghi xuống chip nhớ **Flash** ngoài thông qua hai cơ chế:

### 3.1. Phân vùng bộ nhớ Flash (Partition Table)
- Vì một chip Flash phải chứa nhiều loại dữ liệu khác nhau, ESP-IDF dùng một bảng phân vùng (mặc định nạp ở địa chỉ `0x8000`) để chia Flash thành các khu vực rạch ròi:
  - `nvs`: Lưu cấu hình và tham số nhỏ.
  - `phy_init`: Lưu dữ liệu hiệu chuẩn sóng RF/Wi-Fi.
  - `factory` / `ota_0`, `ota_1`: Lưu mã chương trình thực thi và hỗ trợ cập nhật từ xa (OTA).
  - `spiffs` / `fatfs`: Hệ thống tệp dùng để lưu file dung lượng lớn (trang web, âm thanh, file log).

### 3.2. NVS (Non-Volatile Storage)
- Là thư viện chuyên dùng để lưu các cặp dữ liệu nhỏ dạng **Key – Value** (số nguyên, chuỗi tối đa 4000 byte, hoặc mảng byte *blob*) vào phân vùng `nvs`.
- NVS phân chia dữ liệu theo các `namespace` (không gian tên) để tránh trùng lặp key và tích hợp sẵn thuật toán cân bằng hao mòn (*wear-leveling*) giúp tăng tuổi thọ bộ nhớ Flash.

---

## 4. Các khái niệm liên quan mật thiết bên cạnh

- **Ngắt (ISR) và các hàm `...FromISR`:** Khi dùng ngắt phần cứng (ví dụ: nút nhấn hoặc chân báo hiệu từ cảm biến), hàm xử lý ngắt (ISR) phải chạy cực nhanh và không được phép chờ đợi.
- **Task Notification (Thông báo trực tiếp):** Cho phép gửi tín hiệu hoặc một giá trị 32-bit trực tiếp từ Task/ISR tới thẳng một Task cụ thể. Cơ chế này nhanh hơn và tốn ít RAM hơn hẳn so với tạo Binary Semaphore hay Event Group trong trường hợp chỉ có 1 task nhận tín hiệu.
- **Software Timer (Bộ định thời phần mềm):** Cho phép gọi một hàm thực thi định kỳ hoặc sau một khoảng trễ mà không cần tốn RAM tạo thêm Task mới hay chiếm dụng bộ định thời phần cứng (Hardware Timer).
- **Task Watchdog Timer (TWDT):** Bộ đếm giám sát tích hợp sẵn. Nếu một task bị treo hoặc chiếm dụng CPU quá lâu mà không nhường quyền cho *Idle Task*, TWDT sẽ cảnh báo lỗi hoặc tự động khởi động lại ESP32 để phục hồi hệ thống.
