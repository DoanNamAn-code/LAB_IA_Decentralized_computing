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

