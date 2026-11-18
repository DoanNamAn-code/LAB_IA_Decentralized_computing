# Đơn công - Song công - Bán song công

Trong truyền thông dữ liệu, Có ba chế độ cơ bản là: **song công** (*Full-Duplex*) và **bán song công** (*Half-Duplex*), **đơn công** (*Simplex*) — bộ ba này phân loại **chiều di chuyển của dòng dữ liệu** giữa hai thiết bị.

---

## 1. Bản chất của 3 chế độ truyền dữ liệu

### a) Đơn công (Simplex — Truyền 1 chiều cố định)
* **Cơ chế:** Dữ liệu chỉ đi duy nhất một chiều từ bên Phát sang bên Nhận; bên Nhận không có khả năng gửi ngược lại thông tin trên đường truyền đó *(giống như đài phát thanh FM hoặc truyền hình)*.
* **Ví dụ phần cứng:** 8 đường dữ liệu ảnh `D0–D7` của camera OV7670 hoạt động theo chế độ đơn công — camera chỉ đẩy điểm ảnh sang ESP32, còn ESP32 không bao giờ gửi ngược dữ liệu qua 8 chân này.

### b) Bán song công (Half-Duplex — Truyền 2 chiều luân phiên từng lượt)
* **Cơ chế:** Hai bên dùng chung **một đường truyền vật lý** nên đều có thể gửi và nhận, nhưng tại một thời điểm chỉ một bên được phép phát, bên còn lại phải nghe rồi mới đổi vai *(giống như bộ đàm cầm tay hoặc cây cầu hẹp chỉ có 1 làn xe)*.
* **Ví dụ phần cứng:** Chính **giao thức I2C (dây `SDA`)** là bán song công — lúc ESP32 gửi địa chỉ và lệnh cấu hình thì cảm biến nghe, lúc cảm biến trả lời bit `ACK` hoặc đẩy dữ liệu thanh ghi về thì ESP32 phải thả dây `SDA` ra để nghe.

### c) Song công toàn phần (Full-Duplex — Truyền 2 chiều đồng thời cùng lúc)
* **Cơ chế:** Hai bên có thể vừa gửi vừa nhận dữ liệu **trong cùng một tích tắc** nhờ sở hữu **hai đường truyền vật lý tách biệt** — một đường chuyên đi và một đường chuyên về *(giống như cuộc gọi điện thoại hoặc đường cao tốc hai chiều riêng làn)*.
* **Ví dụ phần cứng & mạng:** **UART** (dây `TX` gửi riêng, dây `RX` nhận riêng), **SPI** (dây `MOSI` gửi riêng, dây `MISO` nhận riêng), hay ở tầng mạng là **TCP** và **WebSockets** đều là song công toàn phần.

---

## 2. Bảng so sánh nhanh

| Tiêu chí | Đơn công (Simplex) | Bán song công (Half-Duplex) | Song công toàn phần (Full-Duplex) |
| :--- | :--- | :--- | :--- |
| **Chiều dữ liệu** | 1 chiều duy nhất | 2 chiều, lần lượt từng bên | 2 chiều, đồng thời cùng lúc |
| **Số đường dữ liệu** | 1 đường (hoặc 1 bus 1 chiều) | 1 đường dùng chung cho cả 2 chiều | 2 đường độc lập (1 đi, 1 về) |
| **Xung đột tín hiệu** | Không xảy ra | Có thể xảy ra nếu hai bên cùng phát | Không xảy ra (tách riêng làn) |
| **Giao thức tiêu biểu** | DVP Camera (`D0–D7`), VGA | I2C (`SDA`), RS-485, Wi-Fi | SPI, UART, Ethernet, TCP |

---

## 3. Ý nghĩa thực tế khi thiết kế mạch và lập trình

Việc lựa chọn giữa 3 chế độ này quyết định trực tiếp đến **chi phí phần cứng (số chân GPIO)** và **tốc độ hệ thống**:

* **Tiết kiệm chân cắm với bán song công (I2C):** Khi cần nối nhiều cảm biến nhỏ (đo nhiệt độ, gia tốc, màn hình OLED) mà dữ liệu ít, dùng giao thức bán song công như I2C giúp tiết kiệm tối đa chân vi điều khiển vì chỉ cần 1 dây `SDA` chung. Đổi lại, tốc độ bị giới hạn do phải mất thời gian đổi lượt và xác nhận.
* **Tối đa hóa tốc độ với song công (SPI, UART):** Khi cần truyền dữ liệu liên tục hoặc yêu cầu phản hồi tức thì (như đọc thẻ nhớ SD, điều khiển màn hình tốc độ cao, hay stream dữ liệu qua TCP), giao thức song công tuy tốn gấp đôi số dây dữ liệu (`MOSI`/`MISO`) nhưng loại bỏ hoàn toàn thời gian chờ đổi chiều và không bao giờ bị treo do tranh chấp đường dây.
* **Tối ưu luồng dữ liệu một chiều với đơn công:** Với các thiết bị chỉ có nhiệm vụ thu thập và xả lượng dữ liệu khổng lồ như camera OV7670, dùng bus đơn công (`D0–D7`) giúp mạch điện đơn giản, không cần bộ chuyển hướng tín hiệu hai chiều mà vẫn đạt băng thông hàng chục Megabit/giây.
---


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
