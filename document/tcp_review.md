# Tổng quan về TCP

## 1. Khái niệm
* **TCP (Transmission Control Protocol):** Đảm bảo dữ liệu đến đúng, đủ, và theo đúng thứ tự. Nếu rớt gói sẽ gửi lại.
* **Ưu điểm:** Tốt cho truyền văn bản, gửi file, tải web.
* **Nhược điểm:** Chậm hơn so với UDP.

---

## 2. Cấu trúc gói tin (TCP Header)
Trước khi được gửi đến tầng tiếp theo thì dữ liệu sẽ được gắn thêm **20 bytes** chứa các thông tin cốt lõi:

* **Source Port (2 bytes):** Cổng nguồn.
* **Destination Port (2 bytes):** Cổng đích.
* **Sequence Number (4 bytes):** Là tọa độ để bên nhận biết đường đặt cái data được gửi vô.
* **Acknowledgment Number (ACK) (4 bytes):** Số thứ tự của byte tiếp theo mà bên nhận đang mong đợi nhận.
* **Data Offset (4 bits):** Báo cho bên nhận header dài bao nhiêu.
* **Reserved (4 bits):** Dự phòng cho nâng cấp.
* **Flags (1 byte / 8 bits):** Thể hiện cờ trạng thái.
* **Window Size (2 bytes):** Dung lượng bộ đệm của mình còn trống bao nhiêu.
* **Checksum (2 bytes):** Mã kiểm tra toàn vẹn dữ liệu để phát hiện lỗi bit do nhiễu đường truyền.
* **Urgent Pointer (2 bytes):** Con trỏ dữ liệu khẩn cấp.

---

## 3. Các cờ trạng thái
* **SYN:** Xin thiết lập kết nối và đồng bộ chỉ số SEQ.
* **ACK:** Xác nhận đã nhận dữ liệu thành công.
* **FIN:** Thông báo gửi hết rồi nên đóng kết nối.
* **RST:** Ngắt kết nối ngay lập tức.
* **PSH:** Yêu cầu hệ điều hành đẩy thẳng lên ứng dụng không giữ lại chờ đầy bộ đệm.

---

## 4. Mở kết nối (Bắt tay 3 bước)
1. **Client đến Server (`SYN`, `SEQ = x`):** Client gửi yêu cầu kết nối kèm số thứ tự khởi tạo ngẫu nhiên `x`.
2. **Server đến Client (`SYN + ACK`, `SEQ = y`, `ACK = x + 1`):** Server đồng ý, xác nhận đã nhận `x` và gửi kèm số thứ tự khởi tạo `y` của riêng nó.
3. **Client đến Server (`ACK`, `SEQ = x + 1`, `ACK = y + 1`):** Client xác nhận lại `y`. Kết nối chính thức thông suốt hai chiều.

---

## 5. Đóng kết nối (Bắt tay 4 bước)
Vì hai chiều truyền dữ liệu độc lập, mỗi bên phải tự đóng chiều gửi của mình bằng cặp gói `FIN` và `ACK` (tổng cộng 4 gói tin).

---

## 6. Cơ chế bảo vệ dữ liệu của TCP
TCP có 3 cơ chế:
* **Reliability:** Lâu quá không nhận `ACK` suy ra rớt nên gửi lại.
* **Flow control:** Thấy `window size` kia = 0 thì ngừng đẩy dữ liệu.
* **Congestion control:** Phát hiện mất gói tin nên giảm tốc độ.

---

## 7. Những hạn chế của TCP và cách khắc phục
* **Tự bật thuật toán Nagle:** Gom đủ lớn ở bộ nhớ đệm, quá lâu cho các gói tin khẩn cấp.
  * **Giải pháp:** Tắt nó đi (`TCP_NODELAY`).
* **Kết nối nửa mở:** Cắt kết nối mạng client không gửi `FIN` cho server để server tắt thế là nó vẫn cứ bật.
  * **Giải pháp:** Định kỳ gửi gói ping (Keep-Alive / Heartbeat) đi để server biết client còn sống hay không.
* **Nghẽn cổ chai (Head-of-Line Blocking):** Gói tin 2, 3, 4 đến rồi mà 1 mãi chưa tới thế là nghẽn, bên nhận khóa lại không cho ứng dụng đọc dữ liệu lên cho tới khi gói 1 tới.
  * **Giải pháp:** Dùng UDP cho các tác vụ thời gian thực không chấp nhận trễ (như stream camera).

> **Chú ý:** Cả hai client và server đều chuẩn bị cho mình hai cái là `send buffer` và `receive buffer` cho riêng mình.
