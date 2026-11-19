# Hoạt động của Đoàn Nam An
## 09/11/2026
Tạo repo cho nhóm; viết file README.md
## 09/11/2026 và 10/11/2026
Hoàn thành week 1, chi tiết: \
Khởi tạo ESP/IDF qua PLATFORMIO thành công (mcu_firmware) \
Khởi tạo cấu trúc dữ liệu của ring_buffer và hiện thực một số hàm làm việc của nó (ring_buffer.c và ring_buffer.h) \
Khởi tạo ngẫu nhiên một bộ dữ liệu mẫu, kết nối wifi và truyền dữ liệu bằng giao thức TCP (main.c), cụ thể: \
- Khởi tạo dữ liệu mẫu (main.c/get_sample_data)
- kết nối wifi: Xử lý sự kiện (main.c/wifi_event_handler) + thiết lập STA (main.c/wifi_init_sta)
- Truyền dữ liệu qua TCP (main.c/tcp_client_task) 

Cùng với đó là tiến hành khởi tạo nvs_flash và tạo đa luồng chạy 

