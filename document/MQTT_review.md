# Tổng Quan Lý Thuyết Giao Thức MQTT

## 1. Khái niệm cơ bản
* **Định nghĩa:** MQTT (*Message Queuing Telemetry Transport*) là giao thức truyền tin nhắn siêu nhẹ hoạt động ở **tầng Ứng dụng (Application Layer)** trên nền TCP/IP (ngang hàng với HTTP), sử dụng cổng mặc định `1883` (thường) hoặc `8883` (mã hóa TLS/SSL).
* **Mục tiêu thiết kế:** Tối ưu hóa cho các thiết bị phần cứng hạn chế tài nguyên RAM/CPU/pin (như ESP32), mạng băng thông thấp hoặc đường truyền không dây chập chờn.

---

## 2. Kiến trúc tổng quát (Publish / Subscribe)
Khác với mô hình hỏi – đáp trực tiếp (Client – Server) của HTTP, MQTT vận hành theo mô hình **Publish / Subscribe** thông qua một máy chủ trung chuyển ở giữa:

```text
[ESP32 (Publisher)]   ── Publish ("lab/cam/status") ──► ┌─────────────────────────┐
                                                        │       MQTT BROKER       │
[ESP32 (Subscriber)]  ◄─ Push ("lab/cam/cmd") ──────────┤  (Mosquitto / EMQX...)  │
                                                        └────────────┬────────────┘
[Server Python]       ◄─ Push ("lab/cam/status") ────────────────────┘
                      ── Publish ("lab/cam/cmd") ────────────────────┘
```

* **MQTT Broker (Trạm trung chuyển — VD: Eclipse Mosquitto, EMQX):** Đóng vai trò như "bưu điện trung tâm", tiếp nhận toàn bộ bản tin từ bên phát, lọc theo chủ đề (Topic) và phân phối ngay lập tức đến đúng những bên đã đăng ký nhận.
* **Publisher (Bên phát tin):** Gửi dữ liệu lên Broker kèm theo nhãn Topic mà không cần biết địa chỉ IP hay số lượng người nhận.
* **Subscriber (Bên nhận tin):** Đăng ký với Broker các Topic quan tâm. Mỗi khi có dữ liệu mới đổ vào Topic đó, Broker chủ động đẩy (Push) tin nhắn xuống cho Subscriber.
* *(Lưu ý: Một thiết bị có thể đồng thời vừa làm **Publisher** vừa làm **Subscriber** trên cùng một kết nối TCP thông qua các Topic khác nhau).*

---

## 3. Ba tính chất tách rời (Decoupling)
* **Tách rời không gian (Space Decoupling):** Các thiết bị không cần biết địa chỉ IP của nhau; tất cả chỉ cần biết duy nhất địa chỉ của Broker.
* **Tách rời thời gian (Time Decoupling):** Bên gửi và bên nhận không bắt buộc phải trực tuyến (online) cùng một thời điểm nhờ cơ chế lưu giữ tin nhắn của Broker.
* **Tách rời đồng bộ (Synchronization Decoupling):** Quá trình gửi và nhận diễn ra bất đồng bộ hoàn toàn (thông qua hàng đợi và sự kiện ngắt/callback), không làm khóa cứng (block) luồng xử lý chính của vi điều khiển.

---

## 4. Hệ thống định tuyến: Topic & Wildcards

### 4.1. Quy tắc đặt tên Topic
* MQTT không dùng địa chỉ IP hay URL endpoint để định tuyến mà dùng **Topic** — một chuỗi văn bản UTF-8 phân cấp bằng dấu gạch chéo `/` tương tự đường dẫn thư mục (ví dụ: `lab/esp32_01/temp`).
* Phân biệt chữ hoa và chữ thường (`lab/Sensor` khác với `lab/sensor`).
* Không nên bắt đầu Topic bằng dấu `/` (nên dùng `lab/sensor` thay vì `/lab/sensor` để tránh tạo ra một cấp rỗng ở đầu chuỗi).
* Không cần khai báo tạo trước trên Broker: hễ có Client Publish hoặc Subscribe một chuỗi Topic bất kỳ, Broker tự động khởi tạo nhánh định tuyến đó trên cây quản lý trong RAM.

### 4.2. Ký tự đại diện (Wildcards)
> **Quy tắc bắt buộc:** Chỉ được phép dùng Wildcards khi **Subscribe**, tuyệt đối cấm dùng khi **Publish**.

* **Single-level (`+`):** Thay thế cho **đúng 1 cấp** bất kỳ tại vị trí đặt dấu `+`.
  * *Ví dụ:* Đăng ký `lab/+/temp` sẽ nhận được tin từ `lab/esp01/temp` và `lab/esp02/temp`, nhưng **không** nhận được `lab/tang1/esp01/temp`.
* **Multi-level (`#`):** Thay thế cho **tất cả các cấp con trở về sau** và bắt buộc phải đứng ở ký tự cuối cùng của chuỗi.
  * *Ví dụ:* Đăng ký `lab/esp32_01/#` sẽ nhận toàn bộ bản tin của thiết bị này như `lab/esp32_01/temp`, `lab/esp32_01/camera/fps`, `lab/esp32_01/status`...

---

## 5. Ba cấp độ Chất lượng dịch vụ (QoS)

| Cấp độ | Tên gọi | Cơ chế bắt tay gói tin | Đặc điểm & Đánh đổi |
| :--- | :--- | :--- | :--- |
| **QoS 0** | *At most once* (Tối đa 1 lần — Gửi rồi quên) | Publisher gửi 1 gói `PUBLISH` duy nhất, không chờ phản hồi. | Nhanh nhất, tốn ít RAM/CPU nhất, nhưng **có thể mất tin** nếu rớt mạng đúng lúc gửi. Phù hợp với dữ liệu cảm biến gửi liên tục. |
| **QoS 1** | *At least once* (Ít nhất 1 lần) | `PUBLISH` $\rightarrow$ Bên nhận trả về **`PUBACK`**. Nếu quá hạn chưa nhận `PUBACK`, bên gửi lưu bản sao và gửi lại (bật cờ `DUP = 1`). | Đảm bảo **không mất tin**, nhưng **có thể nhận trùng** nếu gói `PUBACK` bị thất lạc trên đường về. Mức chuẩn phổ biến nhất thực tế. |
| **QoS 2** | *Exactly once* (Chính xác 1 lần) | Bắt tay **4 bước**: `PUBLISH` $\rightarrow$ `PUBREC` (đã nhận) $\rightarrow$ `PUBREL` (xác nhận xả) $\rightarrow$ `PUBCOMP` (hoàn tất). | Đảm bảo tin tới đích **đúng 1 lần duy nhất**, không mất và không trùng. Độ trễ cao nhất và tốn tài nguyên nhất (dùng cho lệnh kích hoạt quan trọng). |

* **Quy tắc hạ cấp QoS:** Quá trình truyền tin được tách làm 2 chặng độc lập (Chặng 1: `Publisher` $\rightarrow$ `Broker`; Chặng 2: `Broker` $\rightarrow$ `Subscriber`). Mức QoS thực tế áp dụng ở chặng 2 sẽ là giá trị nhỏ hơn giữa QoS lúc gửi lên và QoS tối đa lúc đăng ký:
  $$\text{QoS}_{\text{thực tế}} = \min(\text{QoS}_{\text{Publish}}, \text{QoS}_{\text{Subscribe}})$$

---

## 6. Bốn cơ chế quản lý trạng thái & Độ tin cậy nâng cao

### 6.1. Keep-Alive & `PINGREQ` / `PINGRESP`
* Khi kết nối, Client thỏa thuận với Broker một khoảng thời gian `Keep-Alive` (ví dụ `60` giây). Nếu trong khoảng thời gian này Client không gửi dữ liệu nào, nó tự động gửi gói `PINGREQ` (nặng đúng 2 byte) và Broker phản hồi bằng `PINGRESP`.
* Nếu quá $1.5 \times \text{Keep-Alive}$ (`90` giây) mà Broker không nhận được bất kỳ gói tin hay tín hiệu ping nào, Broker sẽ kết luận Client đã mất kết nối đột ngột và đóng phiên làm việc.

### 6.2. Retained Message (Tin nhắn lưu giữ)
* Khi Publisher gửi một bản tin kèm cờ **`Retain = true`**, Broker sẽ lưu giữ lại **duy nhất bản tin mới nhất** của Topic đó trong bộ nhớ.
* Bất kỳ Subscriber nào kết nối sau đó (kể cả sau nhiều giờ) đều nhận được ngay lập tức bản tin này mà không phải chờ Publisher gửi lại lần kế tiếp.
* *Cách xóa tin lưu giữ:* Publish một bản tin rỗng (`0` byte) kèm cờ `Retain = true` vào đúng Topic đó.

### 6.3. Last Will and Testament — LWT ("Di chúc")
* Ngay trong gói tin `CONNECT` đầu tiên, Client có thể gửi kèm một "bản di chúc" (gồm `Will Topic`, `Will Message`, `Will QoS`, `Will Retain` — ví dụ: Topic `lab/esp32_01/status`, nội dung `"offline"`).
* Nếu Client chủ động ngắt kết nối hợp lệ bằng gói `DISCONNECT`, Broker sẽ hủy bản di chúc này. Ngược lại, nếu thiết bị bị mất nguồn, treo chip hoặc rớt Wi-Fi đột ngột (quá hạn Keep-Alive), Broker sẽ **tự động phát bản tin di chúc** này cho các bên liên quan biết thiết bị đã gặp sự cố.

### 6.4. Clean Session (`clean_session = false` / Persistent Session)
* **Mặc định (`clean_session = true`):** Mỗi lần thiết bị rớt mạng và kết nối lại, Broker coi như một phiên làm việc mới hoàn toàn.
* **Phiên bền vững (`clean_session = false` kèm một `ClientID` cố định, duy nhất):** Broker lưu giữ hồ sơ của thiết bị ngay cả khi offline — ghi nhớ toàn bộ các Topic đã Subscribe và xếp hàng lưu hộ các bản tin QoS 1 / QoS 2 gửi tới trong lúc mất mạng, sau đó tự động xả toàn bộ xuống ngay khi thiết bị kết nối lại.

---

## 7. Cấu trúc gói tin & Lý do MQTT phù hợp cho giao tiếp hai chiều

### 7.1. Cấu trúc gói tin nhị phân (3 phần)
1. **Fixed Header (Tối thiểu 2 byte):** Byte đầu tiên chứa mã loại lệnh (`CONNECT`, `PUBLISH`, `SUBSCRIBE`, `PINGREQ`...) và các bit cờ (`DUP`, `QoS`, `RETAIN`); các byte tiếp theo ghi độ dài phần dữ liệu phía sau (`Remaining Length`).
2. **Variable Header (Tùy chọn):** Chứa tên Topic, mã định danh gói tin `Packet ID` (bắt buộc với QoS 1 và 2 để đối chiếu ACK).
3. **Payload (Dữ liệu thực):** Không ràng buộc định dạng — có thể chứa chuỗi văn bản, JSON hoặc mảng byte nhị phân thô (như dữ liệu điểm ảnh `uint8_t *`).

### 7.2. Tại sao MQTT tối ưu cho giao tiếp hai chiều?
* Các vi điều khiển như ESP32 thường nằm sau Router Wi-Fi (bị chặn bởi tường lửa NAT), khiến Server bên ngoài không thể tự ý mở kết nối tới địa chỉ IP nội bộ của mạch.
* Với MQTT, chính ESP32 chủ động mở một đường hầm TCP đi ra Broker và duy trì liên tục 24/7 bằng cơ chế `Keep-Alive`. Nhờ đường truyền hai chiều đã mở sẵn này, Server có thể đẩy lệnh điều khiển xuống ESP32 bất cứ lúc nào với độ trễ chỉ vài mili-giây mà ESP32 không cần liên tục gửi yêu cầu thăm dò.

---

## 8. Tổng hợp giải đáp: MQTT vs HTTP

### 8.1. Tại sao MQTT tối ưu cho phần cứng yếu, băng thông thấp và mạng chập chờn?
* **Tiết kiệm tài nguyên & băng thông:** Header của MQTT chỉ từ `2 byte` (nhẹ hơn rất nhiều so với hàng trăm byte văn bản của HTTP) và chỉ tốn `1` lần bắt tay TCP duy nhất cho toàn bộ vòng đời kết nối.
* **Chống chịu mạng chập chờn:** Giao thức tích hợp sẵn ở tầng ứng dụng cơ chế gửi lại khi mất xác nhận (`QoS 1/2`), tự phát hiện đứt kết nối ngầm (`Keep-Alive`), tự báo rớt mạng đột ngột (`Last Will`) và lưu tin hộ lúc mất sóng (`Persistent Session`) mà không cần lập trình thủ công từ đầu.

### 8.2. Tại sao trong nhiều trường hợp vẫn phải chọn HTTP thay vì MQTT?
* **Truyền dữ liệu khối lượng lớn (Ảnh thô, video, file lớn):** HTTP truyền thẳng 1 chặng tới Server; nếu đẩy dữ liệu lớn qua MQTT sẽ phải đi vòng qua trạm trung chuyển Broker, dễ gây nghẽn hàng đợi gói tin trên cùng một ống TCP (*Head-of-Line Blocking*) và làm đứt kết nối.
* **Giao tiếp Hỏi – Đáp đồng bộ (Request – Response):** HTTP trả về mã trạng thái (`200 OK`) kèm kết quả ngay trên cùng một luồng gọi, phù hợp cho các tác vụ xác thực hoặc gọi API lấy dữ liệu tức thì.
* **Tương thích trực tiếp với Web/Cloud & Tối giản hạ tầng:** Mọi trình duyệt Web, Cloud API và Webhook đều hỗ trợ sẵn HTTP REST; dùng HTTP giúp kết nối thẳng tới Server ứng dụng mà không cần cài đặt và vận hành thêm máy chủ Broker trung gian chạy 24/7.