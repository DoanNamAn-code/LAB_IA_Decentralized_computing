# Biên bản tổng kết buổi họp (11/10/2026)

---

## I. Phần chia sẻ chuyên môn
* **Người trình bày:** Hải Đăng
* **Nội dung:** Hướng dẫn cài đặt, cấu hình và thao tác làm việc trên môi trường **WSL (Windows Subsystem for Linux)** kết hợp **Ubuntu**.

---

## II. Phần hỏi đáp & Phản biện

### 1. Nam An đặt câu hỏi cho Hải Đăng
* **Câu hỏi 1:** Ethernet là gì?
  * *Hải Đăng trả lời:* *(Chưa cập nhật)*
* **Câu hỏi 2:** Linux headless là gì?
  * *Hải Đăng trả lời:* *(Chưa cập nhật)*

---

### 2. Hải Đăng đặt câu hỏi cho Nam An
* **Câu hỏi 1:** Ring buffer là gì?
  * *Nam An trả lời:*
    * Là cấu trúc dữ liệu bộ đệm vòng (Circular Buffer) được xây dựng và quản lý dựa trên **hai con trỏ** (Read/Write pointer).
    * Hoạt động theo nguyên tắc **FIFO (First-In, First-Out)**: khi bộ đệm đầy, phần tử mới ghi vào sẽ tự động ghi đè lên dữ liệu cũ nhất.
    * Không gặp lỗi tràn chỉ số (`out of index`) khi truy cập vị trí lớn hơn độ dài mảng nhờ cơ chế xoay vòng chỉ số (modulo theo độ dài mảng).
* **Câu hỏi 2:** Hàm `get_sample_data` có tác dụng gì?
  * *Nam An trả lời:*
    * Sinh dữ liệu giả lập (mock data) thay thế cho luồng dữ liệu thực tế thu thập từ cảm biến.
    * Đưa toàn bộ dữ liệu giả lập này vào `ring_buffer` để phục vụ chạy kiểm thử và xử lý tiếp theo.
