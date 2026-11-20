## Giao thức I2C

### 1. Khái niệm

Là giao thức truyền thông nối tiếp, đồng bộ, bán song công, hoạt động theo mô hình **đa chủ - đa tớ** (*Multi-Master, Multi-Slave*). Cho phép kết nối hàng chục vi mạch với vi điều khiển chỉ bằng **2 đường dây tín hiệu** duy nhất.

---

### 2. Kiến trúc vật lý & cơ chế hoạt động

#### a) Kiến trúc vật lý cơ bản

Gồm hai đường dây cơ bản:

* **`SCL` (Serial Clock):** Đường truyền xung nhịp đồng bộ do thiết bị Master tạo ra để điều phối tốc độ truyền.
* **`SDA` (Serial Data):** Đường truyền dữ liệu và địa chỉ hai chiều giữa Master và Slave.

#### b) Cơ chế hoạt động

* Bên trong mỗi chân `SDA` và `SCL` chỉ có một transistor nối xuống đất (`GND`) nên chỉ có thể kéo đường dây xuống mức `0` (cấu trúc *Open-Drain*). Để bus có thể đạt mức logic `1`, bắt buộc phải mắc hai điện trở kéo lên ($R_p$) từ đường `SDA` và `SCL` lên nguồn dương $V_{cc}$ ($3.3\text{V}$ hoặc $5\text{V}$).
* Cơ chế trên cho phép nếu một thiết bị xuất mức `1` (thả nổi) trong khi thiết bị khác xuất mức `0` (kéo xuống đất) thì **không gây ngắn mạch**.
* **Lưu ý chọn trở:** Điện trở kéo lên quá lớn sẽ làm sườn xung bị bo tròn, gây lỗi ở tốc độ cao; ngược lại điện trở quá nhỏ sẽ làm dòng rò lớn khi chân tín hiệu kéo đường dây xuống mức thấp.

#### c) Các chế độ tốc độ

| Chế độ | Tốc độ tối đa | Đặc điểm kỹ thuật |
| :--- | :--- | :--- |
| **Standard-mode (Sm)** | $100\text{ kbps}$ | Phổ biến nhất, tương thích với hầu hết cảm biến và module cơ bản. |
| **Fast-mode (Fm)** | $400\text{ kbps}$ | Chuẩn mặc định trên ESP32, STM32 cho màn hình OLED, cảm biến IMU tốc độ cao. |
| **Fast-mode Plus (Fm+)** | $1\text{ Mbps}$ | Yêu cầu chân giao tiếp có dòng kéo xuống lớn hơn ($20\text{ mA}$) và điện trở kéo lên nhỏ hơn. |
| **High-speed mode (Hs)** | $3.4\text{ Mbps}$ | Cần một mã lệnh khởi đầu đặc biệt ở tốc độ thấp trước khi chuyển sang tần số cao. |
| **Ultra-fast mode (UFm)** | $5\text{ Mbps}$ | Chỉ truyền một chiều (*Unidirectional*) và dùng mạch *Push-Pull* thay vì *Open-Drain*. |

#### d) Trạng thái rảnh và điều kiện START/STOP

* **Bus Idle (Trạng thái rảnh):** Khi không có giao dịch, cả `SDA` và `SCL` đều được điện trở kéo lên giữ ở mức CAO (`1`).
* **Điều kiện `START` (`S`):** Ngoại lệ duy nhất phá vỡ quy tắc giữ ổn định dữ liệu khi `SCL` cao — `SDA` chuyển từ CAO xuống THẤP ($1 \rightarrow 0$) trong khi `SCL` đang ở mức CAO (`1`). Tín hiệu này đánh thức toàn bộ Slave trên bus lắng nghe byte địa chỉ tiếp theo.
* **Điều kiện `STOP` (`P`):** `SDA` chuyển từ THẤP lên CAO ($0 \rightarrow 1$) trong khi `SCL` đang ở mức CAO (`1`), báo hiệu kết thúc phiên truyền và giải phóng bus về trạng thái rảnh.
* **Repeated START (`Sr`):** Master phát lại điều kiện `START` mà không gửi `STOP` trước đó. Kỹ thuật này dùng khi Master muốn đổi hướng từ Ghi sang Đọc (ví dụ: ghi số hiệu thanh ghi cần đọc rồi đọc ngay giá trị của thanh ghi đó) mà không bị Master khác chen ngang chiếm mất bus.

---
#### e) Tổng quát quy trình giao tiếp một giao thức I2C

Mỗi lần **Master** (vi điều khiển) muốn ra lệnh hoặc hỏi dữ liệu từ một **Slave** (cảm biến, màn hình...), quá trình giao tiếp luôn diễn ra tuần tự qua **5 bước** sau:

```text
[START] ──> [7-bit Địa chỉ + 1-bit R/W] ──> [ACK/NACK] ──> [8-bit Data + ACK/NACK] ──> [STOP / Repeated START]
```

##### Bước 1: Phát tín hiệu Bắt đầu (`START`)

* Khi bus rảnh rỗi, cả hai dây `SDA` và `SCL` đều nằm yên ở mức cao (`1`).
* Để mở đầu, Master giữ nguyên dây `SCL` ở mức `1` và kéo riêng dây `SDA` từ mức `1` xuống `0`.
* Tín hiệu này giống như tiếng gõ bảng *"Cả lớp chú ý!"* để đánh thức toàn bộ các Slave trên đường dây.

##### Bước 2: Gọi tên Slave và báo mục đích (`7-bit Địa chỉ` + `1-bit R/W`)

Master gửi ra một gói tin gồm **8 bit**:

* **7 bit đầu tiên:** Là số báo danh (địa chỉ) của Slave muốn gọi (tối đa 112 thiết bị trên cùng một bus).
* **Bit thứ 8 (`R/W`):** Báo mục đích giao tiếp:
  * **`0` (Write):** Master sắp gửi lệnh hoặc ghi dữ liệu xuống Slave.
  * **`1` (Read):** Master muốn đọc dữ liệu từ Slave gửi lên.

##### Bước 3: Xác nhận có mặt (`ACK` / `NACK`)

* Ở nhịp xung thứ 9 trên dây `SCL`, Master thả nổi dây `SDA` để chờ phản hồi.
* **`ACK` (SDA mức `0`):** Xác nhận đã sẵn sàng nhận dữ liệu.
* **`NACK` (SDA mức `1`):** Xác nhận chưa sẵn sàng nhận dữ liệu.

##### Bước 4: Truyền từng gói dữ liệu 8-bit

* Dữ liệu được gửi đi theo từng gói **8 bit**.
* Cứ nhận xong 1 gói 8-bit, bên nhận lại kéo tín hiệu **`ACK`** (mức `0`) để xác nhận đã nhận đủ.
* **Lưu ý khi Master đọc dữ liệu:** Ở gói dữ liệu cuối cùng muốn nhận từ Slave, chính Master sẽ gửi tín hiệu **`NACK`** (mức `1`) để báo hiệu *"Đủ rồi, dừng gửi nhé"*.

##### Bước 5: Kết thúc (`STOP`) hoặc Đổi chiều ngay (`Repeated START`)

* **Kết thúc (`STOP`):** Master kéo dây `SDA` từ `0` lên `1` trong khi `SCL` đang ở mức cao (`1`) để báo *"Hết giờ!"* và giải phóng đường dây.
* **Đổi chiều ngay (`Repeated START`):** Nếu Master vừa ghi tên thanh ghi cần hỏi xong và muốn đọc ngay câu trả lời mà không bị thiết bị khác chen ngang chiếm mất đường dây, nó sẽ phát lại tín hiệu `START` ngay lập tức mà bỏ qua bước `STOP`.


### 3. Cấu trúc khung truyền & quy tắc tín hiệu

```text
[S] │ A6 A5 A4 A3 A2 A1 A0 │ R/W │ ACK │ D7 D6 D5 D4 D3 D2 D1 D0 │ ACK/NACK │ [P]
     └──── 7-bit Addr ───┘  1b    1b    └───── 8-bit Data ─────┘     1b
```

#### a) Đánh địa chỉ (Addressing) và Bit R/W

Ngay sau tín hiệu `START`, Master gửi ra 1 byte (8 bit) theo thứ tự bit trọng số cao trước (**MSB first**):

* **7 bit đầu (`A6..A0`):** Địa chỉ định danh của Slave. Trong $2^7 = 128$ tổ hợp, có 16 địa chỉ dành riêng cho hệ thống (như `0000 000` cho lệnh gọi chung *General Call*), để lại 112 địa chỉ khả dụng cho các thiết bị trên cùng một bus. *(I2C cũng hỗ trợ chuẩn mở rộng địa chỉ 10-bit khi cần nhiều thiết bị hơn).*
* **Bit thứ 8 (`R/W`):** Xác định chiều truyền dữ liệu của các byte tiếp theo:
  * **`0` (Write):** Master sẽ ghi dữ liệu xuống Slave.
  * **`1` (Read):** Master sẽ đọc dữ liệu từ Slave gửi lên.

#### b) Cơ chế xác nhận (ACK / NACK)

Sau mỗi 8 bit dữ liệu (kể cả byte địa chỉ lẫn byte dữ liệu), bên phát thả nổi đường `SDA` và Master cấp xung nhịp thứ 9 trên `SCL` để bên nhận phản hồi:

* **`ACK` (Acknowledge — mức `0`):** Bên nhận chủ động kéo `SDA` xuống mức THẤP (`0`) trong xung thứ 9 để báo đã nhận đủ byte và sẵn sàng cho byte kế tiếp.
* **`NACK` (Not Acknowledge — mức `1`):** Đường `SDA` giữ nguyên ở mức CAO (`1`). `NACK` xảy ra trong 4 tình huống:
  1. Không có Slave nào trên bus trùng với địa chỉ Master vừa gọi (hoặc Slave bị lỗi/chưa cấp nguồn).
  2. Slave đang bận xử lý tác vụ nội bộ nên chưa thể nhận thêm dữ liệu.
  3. Slave nhận được byte dữ liệu hoặc mã lệnh không hợp lệ.
  4. **Trường hợp bình thường khi Master đọc dữ liệu:** Ở byte cuối cùng muốn đọc từ Slave, chính Master sẽ gửi `NACK` thay vì `ACK` để ra hiệu cho Slave ngừng gửi dữ liệu và đưa chân về trạng thái thả nổi, giúp Master phát được điều kiện `STOP` kết thúc phiên.

#### c) Các cơ chế điều phối nâng cao

* **Kéo giãn xung nhịp (Clock Stretching):**
  Nếu một Slave (ví dụ cảm biến nhiệt độ hoặc vi điều khiển đóng vai trò Slave) nhận được yêu cầu đọc nhưng chưa kịp chuyển đổi dữ liệu xong, Slave có quyền chủ động kéo và giữ chặt đường `SCL` ở mức THẤP (`0`) ngay sau khi nhận xong byte trước đó. Khi Master thả chân `SCL` ra nhưng đọc lại thấy `SCL` vẫn bị ghìm ở mức `0`, Master hiểu rằng Slave đang bận và tự động tạm dừng đếm xung nhịp cho tới khi Slave nhả `SCL` lên mức `1`.
* **Phân xử quyền điều khiển bus (Bus Arbitration):**
  Khi có từ 2 Master trở lên cùng gắn trên một bus và vô tình phát điều kiện `START` cùng một thời điểm:
  * Mỗi Master vừa đẩy từng bit của mình lên `SDA`, vừa liên tục đọc lại mức điện áp thực tế trên đường `SDA` mỗi khi `SCL` lên cao.
  * Nhờ nguyên lý **Wired-AND**, nếu Master A xuất bit `1` (thả nổi) nhưng đo thấy `SDA` đang ở mức `0` (do Master B kéo xuống), Master A lập tức biết mình đã thua cuộc phân xử, tự động tắt bộ phát và chuyển sang chế độ lắng nghe. Master B tiếp tục truyền trọn vẹn gói tin mà không hề bị nhiễu hay mất dữ liệu.

#### d) Ba lỗi kinh điển khi làm việc với I2C

* **Treo bus (`SDA` kẹt cứng ở mức thấp `0`):** Xảy ra khi ESP32 bị reset đột ngột đúng lúc Slave đang truyền bit `0` lên đường `SDA`. Sau khi khởi động lại, ESP32 thấy `SDA` đang thấp nên tưởng có Master khác đang chiếm bus và từ chối phát `START`.
  * *Cách khắc phục:* Trước khi khởi tạo driver I2C, cấu hình chân `SCL` làm GPIO Output và phát thủ công 9 xung nhịp vuông để ép Slave đẩy nốt các bit còn kẹt và nhả đường `SDA` về mức cao.
* **Quên điện trở kéo lên hoặc nối dài dây quá mức:** Khi cắm dây Jumper dài trên breadboard, điện dung ký sinh tăng cao làm xung vuông biến dạng. Nếu không có điện trở kéo lên ngoài ($2.2\text{ k}\Omega - 4.7\text{ k}\Omega$) mà chỉ dựa vào điện trở kéo lên nội yếu của vi điều khiển ($\sim 45\text{ k}\Omega$), giao tiếp rất dễ rớt gói.
* **Nhầm lẫn địa chỉ 7-bit và 8-bit:** Một số datasheet ghi địa chỉ dưới dạng 7-bit (ví dụ `0x21`), trong khi số khác dịch trái sẵn 1 bit kèm bit `R/W` (ví dụ `0x42` cho Write và `0x43` cho Read). Các hàm trong ESP-IDF luôn nhận đầu vào là địa chỉ 7-bit (`0x21`) và tự động dịch bit `R/W` bên dưới.
