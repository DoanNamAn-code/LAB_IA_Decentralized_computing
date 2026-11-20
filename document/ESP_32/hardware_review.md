## Tổng quan đầy đủ toàn bộ các chân trên ESP32 (ESP-32S / ESP32-WROOM-32)

### 1. Nhóm chân tự do & an toàn nhất (16 chân đa năng In/Out)
Đây là các chân sạch, có thể cấu hình tự do làm đầu vào/đầu ra số (`GPIO`), xung `PWM`, hoặc ánh xạ sang `I2C`, `SPI`, `I2S` thông qua **GPIO Matrix**:

* **`GPIO 21` & `GPIO 22`:** Hai chân tự do hoàn toàn; theo quy ước mặc định của các thư viện thường dùng làm **`I2C SDA` (`GPIO 21`)** và **`I2C SCL` (`GPIO 22`)** (có thể đổi sang chân khác khi cần nhường cho camera).
* **`GPIO 18`, `GPIO 19`, `GPIO 23`:** Ba chân tự do hoàn toàn, mặc định gắn với bộ giao tiếp tốc độ cao **`VSPI`** (`GPIO 18` = `SCK`, `GPIO 19` = `MISO`, `GPIO 23` = `MOSI`).
* **`GPIO 5`:** Chân tự do, mặc định là chân chọn chip **`VSPI CS`** *(thuộc nhóm strapping phụ xuất xung PWM cực ngắn lúc boot nhưng sử dụng bình thường rất ổn định)*.
* **`GPIO 16` & `GPIO 17`:** Hai chân tự do, mặc định là **`UART2 RX` (`GPIO 16`)** và **`UART2 TX` (`GPIO 17`)** *(lưu ý: hoàn toàn trống trên board ESP-32S tiêu chuẩn; chỉ bị chiếm dụng nếu dùng các board có chip PSRAM ngoài như ESP32-WROVER hoặc ESP32-CAM)*.
* **`GPIO 25` & `GPIO 26`:** Hai chân tự do hoàn toàn, ngoài In/Out thông thường còn tích hợp bộ xuất điện áp tương tự thật **`DAC1` (`GPIO 25`)** và **`DAC2` (`GPIO 26`)**, cùng kênh `ADC2`.
* **`GPIO 27`:** Chân tự do hoàn toàn, hỗ trợ cảm ứng chạm (`Touch`) và `ADC2`.
* **`GPIO 32` & `GPIO 33`:** Hai chân tự do hoàn toàn, rất giá trị vì hỗ trợ **`ADC1`** (đọc tín hiệu Analog chính xác ngay cả khi đang bật Wi-Fi) và cảm ứng chạm (`Touch`).
* **`GPIO 4`, `GPIO 13`, `GPIO 14`:** Ba chân tự do hỗ trợ `PWM`, `Touch`, `ADC2` (`GPIO 13` = `HSPI MOSI`, `GPIO 14` = `HSPI SCK`; riêng `GPIO 14` có xuất xung PWM ngắn lúc khởi động).

---

### 2. Nhóm chân chỉ đọc đầu vào — Input-Only (4 chân)
* **`GPIO 34`, `GPIO 35`, `GPIO 36 (VP)`, `GPIO 39 (VN)`:**
  * Chỉ có mạch nhận tín hiệu vào (**Input-Only**), **không** thể xuất tín hiệu ra (Output).
  * **Không** có điện trở kéo lên/kéo xuống nội bộ (`INPUT_PULLUP` / `INPUT_PULLDOWN` không có tác dụng).
  * Thuộc nhóm **`ADC1`** nên đọc Analog ổn định khi bật Wi-Fi, và rất tối ưu để nhận dữ liệu một chiều từ cảm biến (như các bit cao `D4–D7` của camera OV7670).

---

### 3. Nhóm chân khởi động hệ thống — Strapping Pins (4 chân)
ESP32 đọc mức điện áp của 4 chân này ngay khoảnh khắc cấp nguồn hoặc nhấn Reset để quyết định chế độ chạy; vẫn dùng làm GPIO được nhưng **tránh để ngoại vi kéo sai mức logic lúc khởi động**:

* **`GPIO 0`:** Quyết định chế độ nạp code (kéo xuống `GND` lúc Reset sẽ vào chế độ Bootloader chờ nạp firmware; thả nổi/kéo cao để chạy chương trình bình thường).
* **`GPIO 2`:** Nối sẵn với đèn LED xanh trên board; phải ở mức thấp hoặc thả nổi khi nạp code.
* **`GPIO 12` (`HSPI MISO`):** Chọn điện áp cấp cho bộ nhớ Flash nội ($3.3\text{V}$ nếu mức thấp, $1.8\text{V}$ nếu mức cao) — nếu bị ngoại vi kéo lên mức cao lúc bật nguồn sẽ làm ESP32 treo không boot được.
* **`GPIO 15` (`HSPI CS`):** Điều khiển xuất thông báo log khởi động qua `UART0` (mức cao = bật log, mức thấp = tắt log).

---

### 4. Nhóm chân nạp code & giao tiếp USB-UART (2 chân)
* **`GPIO 1 (TX0)` & `GPIO 3 (RX0)`:** Nối trực tiếp với chip chuyển đổi USB-UART trên board để nạp chương trình và in log `printf` lên máy tính. Chỉ nên tận dụng làm GPIO thường khi không dùng cổng USB Serial.

---

### 5. Nhóm chân kết nối bộ nhớ Flash nội (6 chân — Tuyệt đối không dùng)
* **`GPIO 6`, `GPIO 7`, `GPIO 8`, `GPIO 9`, `GPIO 10`, `GPIO 11`:** Nối trực tiếp với chip bộ nhớ SPI Flash chứa code chương trình bên trong module ESP32-WROOM-32. Cắm dây ngoại vi vào các chân này sẽ làm treo vi điều khiển ngay lập tức.

---

### 6. Nhóm chân nguồn & kích hoạt (4 chân)
* **`3V3`:** Xuất nguồn ổn áp $3.3\text{V}$ để nuôi cảm biến, camera *(dòng tối đa khoảng $500\text{–}600\text{ mA}$)*.
* **`VIN` (`5V`):** Cấp nguồn $5\text{V}$ vào board hoặc lấy nguồn $5\text{V}$ từ cổng USB *(lưu ý: mọi chân GPIO của ESP32 chỉ chịu mức logic $3.3\text{V}$, không chịu được $5\text{V}$)*.
* **`GND`:** Chân nối đất chung (Mass).
> **Ghi chú:** Trên module ESP32-WROOM-32 / ESP-32S (38 chân), các số **`GPIO 20, 24, 28, 29, 30, 31`** không tồn tại trên chân vật lý bên ngoài.
---
## Tổng quan các chân trên module camera OV7670

### 1. Nhóm điều khiển trạng thái & nguồn
* **`3.3V` & `GND`:** Cấp nguồn hoạt động $3.3\text{V}$ và nối đất chung (Mass) cho toàn bộ bo camera *(lưu ý: tuyệt đối không cấp nguồn 5V*.
* **`PWDN` (Power Down — tích cực mức cao):** Điều khiển chế độ tiết kiệm điện (`1` = ngủ đông / tắt cảm biến, `0` = hoạt động bình thường). Thường nối thẳng xuống `GND` để camera luôn bật.
* **`RESET` (tích cực mức thấp):** Khởi động lại phần cứng camera (`0V` = reset toàn bộ thanh ghi cấu hình về mặc định, `3.3V` = chạy bình thường).

### 2. Nhóm cấu hình thanh ghi (Chuẩn SCCB / I2C)
* **`SIOD` (SDA) & `SIOC` (SCL):** Hai đường truyền dữ liệu và xung nhịp giao tiếp I2C, dùng để ESP32 ghi các thông số cấu hình (độ phân giải, định dạng màu, độ sáng...) vào thanh ghi của OV7670 trước khi chụp.

### 3. Nhóm xung nhịp & đồng bộ khung hình (DVP)
* **`XCLK` (System Clock):** Xung nhịp hệ thống đầu vào **của OV7670** (do ESP32 tạo ra và cấp sang, thường $10\text{–}20\text{ MHz}$) để nuôi toàn bộ mạch số bên trong camera hoạt động *(không có xung này camera sẽ không phản hồi kể cả I2C)*.
* **`VSYNC` (Vertical Sync):** Tín hiệu đồng bộ khung hình. Khi `VSYNC` chuyển trạng thái, bộ DMA của ESP32 biết đã kết thúc một bức ảnh và đặt lại con trỏ bộ nhớ về vị trí pixel đầu tiên `(0, 0)`.
* **`HREF` (Horizontal Reference):** Tín hiệu báo hiệu một dòng ngang điểm ảnh hợp lệ:
  * `HREF = 1`: Dữ liệu trên bus `D0–D7` là điểm ảnh thật.
  * `HREF = 0`: Camera đang trong khoảng nghỉ chuyển dòng (Horizontal Blanking), ESP32 sẽ bỏ qua không ghi vào RAM.
* **`PCLK` (Pixel Clock):** Xung nhịp điểm ảnh **do OV7670 phát ra**. Cứ mỗi nhịp `PCLK`, OV7670 đặt sẵn $1\text{ byte}$ dữ liệu lên 8 chân `D0–D7` để báo cho ESP32 chụp lấy byte đó ngay lập tức.

### 4. Nhóm bus dữ liệu ảnh 8-bit song song
* **`D0 – D7` (8 chân):** Truyền đồng thời $8\text{ bit}$ ($1\text{ byte}$) dữ liệu ảnh trong mỗi nhịp `PCLK` (`D7` là bit trọng số cao nhất MSB, `D0` là bit thấp nhất LSB). Với chuẩn màu RGB565 ($16\text{ bit/pixel}$), camera truyền $2\text{ byte}$ qua $2\text{ nhịp PCLK}$ liên tiếp để ghép thành $1\text{ điểm ảnh}$.
* **`EN` (Enable):** Chân bật/reset toàn bộ chip ESP32 (mặc định kéo lên $3.3\text{V}$ để chạy; kéo xuống `GND` để khởi động lại).

