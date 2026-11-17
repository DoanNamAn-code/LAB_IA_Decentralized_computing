# Quyết định 1 (11/11/2026)

* Hải Đăng bàn giao cho Nam An ESP32 và bộ cảm biến để Nam An thực hiện lấy dữ liệu thật từ \
môi trường thay cho dữ liệu mẫu tạo ngẫu nhiên.
* Tạo thêm thư mục document để các thành viên chia sẻ các nguồn tài liệu hữu ích hoặc các khái niệm \
  lý thuyết
* Hai thành viên tiếp tục thực hiện theo kế hoạch đã để ra
---
# Quyết định 2 (16/11/2026)

* Thay đổi từ sử dụng dữ liệu giả lập dể mô phỏng truyền tin từ server sang client sang \
  thu thập dữ liệu trực tiếp từ môi trường cụ thể sử dụng (Camera OV7670 + ESP32)
* Thay đổi từ truyền tin qua TCP thông thường sang truyền tin có sử dụng thêm giao thức HTTP
## Nhiệm vụ cụ thể sau quyết định:
* Nam An: Thu thập dữ liệu ảnh từ camera và xây dựng http client ở ESP32
* Hải Đăng: Xây dựng HTTP server tiếp nhận ảnh từ Nam An và hiện thị bức ảnh đó lên màn hình
