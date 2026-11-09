# Edge IoT Anomaly Detection System

> **Dự án:** Hệ thống Giám sát & Chẩn đoán Bất thường Biên Phi tập trung  
> **Nhóm thực hiện:** Nhóm 2 — Hệ thống tính toán và trí tuệ phi tập trung  
> **Thời gian triển khai:** 4 tuần (Tuần 1 – Tuần 4)

---

## 1. Giới thiệu Dự án

Dự án hướng tới xây dựng một hệ thống giám sát và phát hiện bất thường kết hợp giữa trí tuệ nhân tạo tại thiết bị biên (**TinyML / Edge AI**) và hạ tầng trạm thu phân tán có khả năng tự phục hồi (**Fault-Tolerant Distributed Gateway**). 

Hệ thống hoạt động theo mô hình lũy tiến: thiết bị nhúng thu thập dữ liệu cảm biến, tự suy luận phát hiện sự cố tại chỗ qua mô hình mạng nơ-ron nén số nguyên (**INT8**) nhằm tiết kiệm điện năng và băng thông; đồng thời truyền tín hiệu cảnh báo về trạm Gateway và lan truyền thông tin qua mạng phân tán ngang hàng (**P2P Gossip & Raft Consensus**).

### Đội ngũ Thực hiện & Phân chia Chuyên môn

| Thành viên | Vai trò Kỹ thuật | Trách nhiệm & Công nghệ Chủ đạo |
| :--- | :--- | :--- |
| **Đoàn Nam An** | Kỹ sư Phần cứng Nhúng & Edge AI | • Ngôn ngữ: `C/C++`<br>• Nền tảng: Vi điều khiển `ESP32 / ESP32-S3`<br>• Hệ điều hành & Thư viện: `FreeRTOS`, `TFLite Micro`<br>• Giao thức & Phần cứng: `GPIO`, `ADC`, `I2C`, `UART`, `Ring Buffer`, `MQTT Client` |
| **Nguyễn Hải Đăng** | Kỹ sư Hạ tầng Gateway & Hệ phân tán | • Ngôn ngữ: `Python`, `Go`<br>• Nền tảng & Ảo hóa: `Linux Headless`, `Docker`, `Docker Compose`<br>• Dịch vụ & Dữ liệu: `FastAPI`, `Mosquitto MQTT Broker`, `SQLite/Postgres`, `pandas`<br>• Trí tuệ nhân tạo & Phân tán: `scikit-learn`, `PyTorch`, `P2P Gossip`, `Raft Consensus` |

---

## 2. Sơ đồ Luồng Kế thừa Lũy tiến (4-Week Pipeline)

Dự án được triển khai theo mô hình phát triển lũy tiến (Agile / Incremental): **kết quả đầu ra của tuần trước là điều kiện đầu vào bắt buộc để tuần sau nâng cấp và hoàn thiện.**

```text
TUẦN 1: Nền tảng Công nghệ & Thiết lập Kết nối Thô
  │
  ├──► [Ring Buffer C cơ bản & Luồng dữ liệu Socket TCP/UDP thô]
  │                               │
  ▼                               ▼
TUẦN 2: Lập trình Nâng cao, Baseline ML & Mô phỏng Phân tán
  │
  ├──► [Thư viện Ring Buffer hoàn thiện (Kiểm soát con trỏ, RAM)] ──┐
  ├──► [Tập dữ liệu chuỗi thời gian đã gán nhãn (Dataset)] ───────┼──────────────┐
  ├──► [Dockerfile chuẩn hóa dịch vụ] ──────────────────────────┐ │              │
  │                                                             │ │              │
  ▼                                                             ▼ ▼              │
TUẦN 3: Multithreading FreeRTOS, MQTT Broker & P2P Gossip       │ │              │
  │                                                             │ │              │
  ├──► [Firmware FreeRTOS 3 Task + Queue + FSM ổn định] ────────┼─┼──────┐       │
  ├──► [Hạ tầng Gateway Docker Compose Stack] ──────────────────┼─┼──────┼───┐   │
  │                                                             │ │      │   │   │
  ▼                                                             ▼ ▼      ▼   ▼   ▼
TUẦN 4: TinyML (TFLite Micro), Go Bridge & Tích hợp End-to-End Raft Consensus
  │
  └──► [SẢN PHẨM HOÀN CHỈNH TÍCH HỢP TOÀN DIỆN TỪ BIÊN ĐẾN MÁY CHỦ PHÂN TÁN]
```

---

## 3. Lộ trình Triển khai Chi tiết từng Tuần

### Tuần 1: Nền tảng Công nghệ & Thiết lập Môi trường Kết nối

* **Đoàn Nam An (Nhúng):**
  * Khởi tạo dự án trên `ESP-IDF/PlatformIO`.
  * Viết mã C cơ bản (con trỏ, struct) lưu dữ liệu cảm biến giả lập vào bộ đệm vòng (`Ring Buffer`).
  * Đọc dữ liệu thực qua giao tiếp `I2C/ADC`, kết nối Wi-Fi và truyền chuỗi byte qua `TCP/UDP Socket Client`.
* **Nguyễn Hải Đăng (Gateway):**
  * Cài đặt môi trường Linux Headless trên SBC/PC, cấu hình SSH, môi trường Conda và Docker cơ bản.
  * Viết chương trình `TCP Server` (Python/Go) tiếp nhận luồng dữ liệu từ MCU, ghi log và dùng `pandas` phân tích mẫu.
* **Phối hợp & Nghiệm thu (Joint DoD Tuần 1):**
  * Khởi tạo GitHub Repository chung với `.gitignore` và `environment.yml`.
  * Bắt gói tin bằng Wireshark (TCP/ping) và vẽ sơ đồ phân tích kiến trúc tập trung vs. phi tập trung.
  * **Điều kiện hoàn thành:** Luồng dữ liệu truyền thành công từ MCU qua Wi-Fi tới Socket Server; cả hai thành viên chạy được script phân tích dữ liệu trên máy cá nhân.

---

### Tuần 2: Lập trình Nâng cao, Baseline ML & Mô phỏng Phân tán

* **Đoàn Nam An (Nhúng):**
  * Hoàn thiện thư viện C Ring Buffer (tối ưu hóa con trỏ, struct, kiểm soát an toàn không để rò rỉ RAM).
  * Chuyển đổi cơ chế gửi dữ liệu chuỗi thời gian sang giao thức `REST/HTTP`.
  * Kiểm tra độ ổn định của Wi-Fi và lập trình cơ chế bắt lỗi mất kết nối.
* **Nguyễn Hải Đăng (Gateway & AI):**
  * Viết lớp OOP (Python/Go) mô hình hóa cảm biến kèm bộ kiểm thử tự động (`pytest` / `go test`).
  * Huấn luyện 3 mô hình học máy cổ điển (`k-NN`, `Decision Tree`, `SVM`) làm mốc so sánh (Baseline).
  * Viết script mô phỏng mạng 3 nút trao đổi bản tin P2P có khả năng chịu lỗi khi 1 nút sập.
* **Phối hợp & Nghiệm thu (Joint DoD Tuần 2):**
  * Sử dụng kỹ thuật cửa sổ trượt (`sliding window`) để trích xuất đặc trưng và gán nhãn cho tập dữ liệu thực tế.
  * Đóng gói mã nguồn thành Dockerfile và đẩy image lên GitHub Container Registry.
  * **Điều kiện hoàn thành:** Mô hình ML baseline phân loại được dữ liệu cảm biến thực; cả hai thành viên vận hành được kịch bản mô phỏng 3 nút chịu lỗi và duyệt (Approve) Pull Request chéo trên GitHub.

---

### Tuần 3: Multithreading FreeRTOS, MQTT Broker & P2P Gossip

* **Đoàn Nam An (Nhúng):**
  * Tái cấu trúc firmware sang hệ điều hành `FreeRTOS` với 3 Task độc lập: Task đọc cảm biến, Task xử lý bộ đệm, Task gửi tin qua `Queue`.
  * Xây dựng máy trạng thái hữu hạn (`FSM`) điều khiển luồng hoạt động nhúng an toàn.
  * Lập trình `MQTT Client` trên MCU để publish dữ liệu có cấu trúc lên trạm trung gian.
* **Nguyễn Hải Đăng (Gateway):**
  * Dựng máy chủ `Mosquitto MQTT Broker` và xây dựng ứng dụng web `FastAPI` kết nối cơ sở dữ liệu `SQLite/Postgres`.
  * Viết chương trình Go sử dụng `Goroutines` và `Channels` để xử lý song song luồng dữ liệu cảm biến.
  * Viết kịch bản mô phỏng thuật toán lan truyền thông tin `Gossip Protocol` trên mạng 10 nút P2P.
* **Phối hợp & Nghiệm thu (Joint DoD Tuần 3):**
  * Thống nhất chuẩn bản tin MQTT (Topic naming, Payload JSON, cấu hình QoS 0/1).
  * Đóng gói trọn bộ Gateway (Mosquitto + FastAPI + DB) thành tệp `docker-compose.yml`.
  * **Điều kiện hoàn thành:** MCU chạy FreeRTOS gửi bản tin MQTT ổn định vào trạm thu; khởi chạy toàn bộ hạ tầng Gateway chỉ bằng 1 lệnh `docker compose up`.

---

### Tuần 4: TinyML (TFLite Micro), Go Bridge & Đồng thuận Phi tập trung

* **Đoàn Nam An (Nhúng):**
  * Tối ưu hóa đóng gói bản tin C (`struct packing`, kiểm tra lỗi `CRC`) và cấu hình phân bổ vùng nhớ SRAM/Flash.
  * Viết cơ chế Wi-Fi event loop tự động phục hồi kết nối có thuật toán backoff khi rớt mạng.
  * Tích hợp thư viện `TensorFlow Lite for Microcontrollers` (TFLite Micro), nạp mảng byte mô hình C (`.h`) và chạy suy luận AI trực tiếp tại biên.
* **Nguyễn Hải Đăng (Gateway & AI):**
  * Huấn luyện mạng nơ-ron sâu (`MLP/CNN`) bằng PyTorch dựa trên tập dữ liệu gán nhãn từ Tuần 2.
  * Thực hiện lượng tử hóa số nguyên `INT8` toàn phần và xuất mô hình thành file tiêu đề mảng byte C (`.h`) bàn giao cho Nam An.
  * Xây dựng dịch vụ `Go Bridge` chuyển tiếp dữ liệu tốc độ cao từ MQTT sang HTTP API.
  * Lập trình kịch bản mô phỏng thuật toán bầu chọn Leader (`Raft Consensus`) cho mạng 5 nút.
* **Phối hợp & Nghiệm thu Toàn diện (Joint DoD Tuần 4):**
  * **Kịch bản kiểm thử End-to-End:** Cố tình ngắt kết nối Wi-Fi của thiết bị $\rightarrow$ MCU vẫn tự phát hiện bất thường tại chỗ nhờ AI biên $\rightarrow$ Bật lại Wi-Fi $\rightarrow$ Thiết bị tự kết nối lại và bắn cảnh báo qua MQTT $\rightarrow$ Dịch vụ Go Bridge tiếp nhận và kích hoạt thuật toán đồng thuận lan truyền cảnh báo trong cụm.
  * **Nghiệm thu kiến thức chéo:** Nam An giải thích được luồng dữ liệu của Go Bridge & P2P Gossip; Hải Đăng giải thích được cơ chế cấp phát tĩnh Tensor Arena của TFLite Micro và FreeRTOS.

---

## 4. Tổng kết Sản phẩm Bàn giao Sau 4 Tuần

| STT | Tên Sản phẩm | Mô tả Kỹ thuật & Khả năng Vận hành | Thành viên Phụ trách Chính |
| :---: | :--- | :--- | :--- |
| **1** | **01 Thiết bị nhúng (ESP32)** | Thu thập dữ liệu cảm biến qua giao tiếp bus I2C, vận hành FreeRTOS đa nhiệm, nhúng mô hình TinyML (TFLite Micro INT8) tự suy luận tại chỗ và có cơ chế tự phục hồi kết nối mạng. | **Đoàn Nam An** |
| **2** | **01 Trạm thu Gateway Stack (Docker Compose)** | Cụm dịch vụ đóng gói hoàn chỉnh gồm Mosquitto MQTT Broker, Go Service Bridge chuyển tiếp song song, ứng dụng FastAPI và cơ sở dữ liệu chuỗi thời gian. | **Nguyễn Hải Đăng** |
| **3** | **01 Bộ Kịch bản Mô phỏng Phân tán** | Mã nguồn mô phỏng thuật toán lan truyền dữ liệu Gossip Protocol (10 nút P2P) và cơ chế bầu Leader theo thuật toán đồng thuận Raft (5 nút). | **Nguyễn Hải Đăng** |
| **4** | **01 GitHub Repository Chung** | Quản lý mã nguồn theo quy trình Git Flow (nhánh feature, pull request, code review), có cấu hình môi trường chuẩn hóa (`environment.yml`, `Dockerfile`, `docker-compose.yml`) và tài liệu hướng dẫn vận hành. | **Cả hai cùng phối hợp** |

---

## 5. Quy chuẩn Phối hợp Kỹ thuật

* **Mô hình Quản lý Mã nguồn (Git Flow):**
  * Nhánh `main`: Chứa mã nguồn ổn định sau khi đã nghiệm thu ở mỗi cột mốc cuối tuần.
  * Nhánh `develop`: Nhánh tích hợp mã nguồn chung của cả hai thành viên.
  * Nhánh `feature/<tên-tính-năng>`: Mỗi thành viên tạo nhánh riêng khi phát triển task độc lập và tạo Pull Request (PR) để người còn lại review trước khi merge.
* **Môi trường & Đóng gói:**
  * Mọi thư viện phụ thuộc phía máy chủ được khóa phiên bản trong `environment.yml` hoặc `requirements.txt`.
  * Các dịch vụ Gateway bắt buộc chạy được thông qua Docker Compose mà không phụ thuộc vào hệ điều hành của máy host.