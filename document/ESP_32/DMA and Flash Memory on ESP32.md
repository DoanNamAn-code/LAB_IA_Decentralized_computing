## DMA và Bộ nhớ Flash trên ESP32

### 1. DMA (Direct Memory Access — Truy cập bộ nhớ trực tiếp)

#### Khái niệm
**DMA** là một bộ điều khiển phần cứng chuyên biệt tích hợp bên trong vi điều khiển ESP32, đóng vai trò như một "trợ lý vận chuyển" độc lập giúp **chuyển dữ liệu trực tiếp giữa các ngoại vi (`Camera I2S`, `SPI`, `UART`, `ADC`...) và bộ nhớ `RAM` mà không cần sự can thiệp của `CPU`**.
```text
Không dùng DMA:  [Camera OV7670] ──► [CPU đọc từng byte] ──► [RAM]  (CPU bị chiếm 100%)
Có dùng DMA:     [Camera OV7670] ──► [Bộ điều khiển DMA] ──► [RAM]  (CPU rảnh tay làm việc khác)
```

#### Cơ chế hoạt động thực tế
* **Nếu không có DMA:** Cứ mỗi khi OV7670 bắn ra 1 byte ảnh theo nhịp `PCLK`, CPU phải dừng chương trình, đọc chân GPIO rồi ghi vào RAM. Với tốc độ hàng trăm nghìn byte mỗi giây, CPU sẽ bị vắt kiệt và không kịp xử lý các tác vụ khác.
* **Khi bật DMA:** CPU chỉ cần chỉ định sẵn một vùng đệm (Buffer) trên RAM rồi giao quyền cho DMA. Bộ DMA tự động hứng từng byte từ bộ ngoại vi I2S (dựa theo tín hiệu `PCLK`, `HREF`, `VSYNC`) và xếp thẳng vào RAM; chỉ khi đầy trọn vẹn một bức ảnh, DMA mới phát tín hiệu báo cho CPU biết để mang ảnh đi xử lý.

#### Ý nghĩa của DMA
* **Giải phóng hoàn toàn CPU:** Trong lúc DMA âm thầm hút dữ liệu ảnh vào RAM, hai nhân xử lý của ESP32 vẫn rảnh rỗi để duy trì kết nối Wi-Fi, đóng gói giao thức HTTP POST hoặc chạy mô hình AI.
* **Đảm bảo tốc độ cao và không rớt dữ liệu:** DMA vận hành bằng mạch cứng nên bắt kịp các xung nhịp tốc độ hàng chục MHz mà không bao giờ bị trễ nhịp như khi đọc bằng code phần mềm.

---

### 2. Bộ nhớ Flash của ESP32

#### Khái niệm
**Flash** là loại **bộ nhớ không bay hơi (Non-volatile Memory)** của ESP32 — toàn bộ dữ liệu lưu trên đây vẫn được giữ nguyên vĩnh viễn ngay cả khi rút nguồn điện hoặc nhấn nút Reset (tương tự ổ cứng SSD trên máy tính). Trên các module như ESP32-WROOM-32 (ESP-32S), Flash là một chip nhớ dung lượng thường từ `4 MB` đến `16 MB` gắn ngay dưới nắp kim loại và nối với chip chính qua bus SPI nội (`GPIO 6` đến `GPIO 11`).

#### Các phân vùng phổ biến (Partition Table)
Khi nạp chương trình xuống ESP32, bộ nhớ Flash thường được chia thành nhiều phân vùng đảm nhiệm các nhiệm vụ riêng:
* **Bootloader:** Đoạn mã khởi động chạy đầu tiên ngay khi cấp điện để kiểm tra phần cứng và gọi chương trình chính.
* **Application (Firmware):** Nơi chứa toàn bộ mã nguồn C/C++ sau khi đã được biên dịch thành mã máy.
* **NVS (Non-Volatile Storage):** Vùng nhớ chuyên lưu các thông số cấu hình nhỏ dạng cặp khóa – giá trị (tên Wi-Fi, mật khẩu, địa chỉ IP server, ngưỡng hiệu chuẩn cảm biến).
* **Hệ thống tệp (SPIFFS / LittleFS):** Hoạt động như một ổ đĩa thu nhỏ để lưu các file tĩnh như giao diện web HTML/CSS/JS, chứng chỉ bảo mật, hoặc lưu tạm file ảnh.
* **Phân vùng OTA (Over-The-Air):** Vùng nhớ dự phòng cho phép ESP32 tải và cập nhật bản firmware mới qua mạng Wi-Fi từ xa mà không cần cắm cáp USB.

#### Ý nghĩa của Flash (và lý do phải đi cùng RAM)
* **Lưu giữ chương trình và trạng thái hệ thống:** Giúp ESP32 tự động chạy lại đúng nhiệm vụ và kết nối lại Wi-Fi mỗi khi khởi động lại mà không cần nạp lại code.
* **Bù trừ nhược điểm cho RAM:** Flash có dung lượng lớn (hàng Megabyte) và không mất dữ liệu khi mất điện, nhưng **tốc độ ghi chậm** và bị giới hạn tuổi thọ khoảng `100.000` lần ghi/xóa. Ngược lại, **RAM (SRAM)** mất sạch dữ liệu khi mất điện và dung lượng nhỏ (`~520 KB`), nhưng tốc độ đọc/ghi cực nhanh và ghi bao nhiêu lần cũng không hỏng. Vì vậy, code chương trình được cất trên **Flash**, còn luồng dữ liệu ảnh thay đổi liên tục từ DMA bắt buộc phải đổ vào **RAM**.

---

### 3. Bảng tổng hợp vai trò trong hệ thống Camera ESP32

| Thành phần | Bản chất | Mất dữ liệu khi tắt nguồn? | Vai trò cụ thể trong dự án OV7670 |
| :--- | :--- | :--- | :--- |
| **Flash** | Bộ nhớ lưu trữ cố định (`4–16 MB`) | **Không** | Chứa code khởi tạo camera, driver Wi-Fi, mật khẩu mạng (NVS). |
| **RAM (SRAM)** | Bộ nhớ làm việc tốc độ cao (`~520 KB`) | **Có** | Chứa mảng đệm khung hình (`153.600 byte`) và các biến đang chạy. |
| **DMA** | Bộ điều khiển phần cứng truyền dữ liệu | Không lưu trữ (chỉ vận chuyển) | Hút trực tiếp từng byte ảnh từ OV7670 đổ thẳng vào mảng trên RAM. |
