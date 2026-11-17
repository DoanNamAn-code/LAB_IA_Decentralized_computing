## Mutex

### a) Khái niệm
- Là một chìa khóa (giống threading.Lock() trong Python) giúp việc làm việc nhiều task cùng lúc không bị **race condition**.

### b) Quy trình tạo dựng
- `xSemaphoreCreateMutex()`: Tạo ra chiếc chìa khóa trong RAM khi khởi động chương trình (trạng thái ban đầu là đang rảnh).
- `xSemaphoreTake(mutex, xTicksToWait)`: Xin cầm chìa khóa trước khi đụng vào biến chung.
- `xSemaphoreGive(mutex)`: Trả lại chìa khóa ngay sau khi cập nhật xong con trỏ để đánh thức Task đang ngủ chờ (nếu có).

### c) Tính chất
- **Kế thừa độ ưu tiên:** Task độ ưu tiên thấp đang giữ chìa khóa mà cái cao cần thì task độ ưu tiên thấp hơn được nâng lên ngang bằng task cao để nó chạy nhanh hơn.
- **Quyền sở hữu:** Task nào giữ chìa khóa thì chính task đó phải trả lại chìa khóa.
