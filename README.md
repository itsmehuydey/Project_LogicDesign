# Project_LogicDesign STM32 & ESP32
# Hệ thống thu thập và hiển thị dữ liệu cảm biến trên
## 1. Mục tiêu dự án  
Dự án nhằm phát triển **device driver** để giao tiếp giữa vi điều khiển STM32 và các module ngoại vi, bao gồm:  
- **Cảm biến DHT20**: Đo nhiệt độ và độ ẩm.  
- **LCD 16x2**: Hiển thị dữ liệu đo được từ cảm biến.  
- **LED RGB**: Biểu thị trạng thái nhiệt độ/độ ẩm.  
- **Wi-Fi ESP32**: Gửi dữ liệu cảm biến đến máy chủ hoặc hiển thị trên một trang web cơ bản.  

Hệ thống hỗ trợ:  
- Thu thập và xử lý dữ liệu từ cảm biến.  
- Hiển thị thông tin trên màn hình LCD.  
- Báo hiệu trạng thái bằng LED RGB.  
- Truyền dữ liệu không dây qua module ESP32.  

## 2. Hướng dẫn sử dụng  
### 2.1. Chuẩn bị  
- **Phần mềm**:  
  - STM32CubeMX để cấu hình phần cứng.  
  - STM32CubeIDE hoặc Keil để biên dịch và tải mã lên STM32.  
  - Arduino IDE để lập trình và tải mã lên ESP32.  

- **Phần cứng**:  
  - Nucleo-F103RB (STM32) & Yolo Uno (ESP32)
  - Các module DHT20, LCD 16x2, LED RGB, ESP32, STM32 , Led đơn, Light sensor.
  - Dây kết nối và linh kiện cần thiết.  

### 2.2. Cấu hình và lập trình  
#### **1. Cảm biến DHT20**  
- **Mục tiêu**: Đọc nhiệt độ và độ ẩm qua giao tiếp I2C.  
- **Thực hiện**:  
  1. Import file `Module Test.ioc` vào **STM32CubeMX**.  
  2. Kích hoạt giao tiếp **I2C** cho cảm biến DHT20.  
  3. Generate code và phát triển driver để đọc dữ liệu từ DHT20.  

#### **2. Màn hình LCD 16x2**  
- **Mục tiêu**: Hiển thị dữ liệu nhiệt độ và độ ẩm.  
- **Thực hiện**:  
  1. Kết nối màn hình LCD 16x2 với STM32 qua giao tiếp I2C.    
  2. Sử dụng hàm hiển thị để xuất dữ liệu từ cảm biến.  

#### **3. LED RGB**  
- **Mục tiêu**: Biểu thị trạng thái hệ thống bằng màu sắc.  
- **Thực hiện**:
  1. Kết nối esp32 trên Yolo Uno qua UART
  2. Kết nối LED RGB với ESP32 qua PWM
  3. Phát triển driver điều khiển LED RGB để thay đổi màu sắc tùy thuộc vào ngưỡng:  
     - Xanh, Vàng: Bình thường.  
     - Đỏ: Nhiệt độ ngoài ngưỡng.  

#### **4. Module ESP32**  
- **Mục tiêu**: Truyền dữ liệu không dây qua Wi-Fi.  
- **Thực hiện**:  
  1. Mở file `ESP32.ino` bằng **Arduino IDE**.  
  2. Cấu hình ESP32 để kết nối Wi-Fi và truyền dữ liệu cảm biến qua UART hoặc SPI.  
  3. Lập trình để gửi dữ liệu lên máy chủ hoặc hiển thị qua **index.html**.  

### 2.3. Kiểm tra giao diện web  
- Mở file `index.html` bằng trình duyệt để xem dữ liệu cảm biến được hiển thị.  
- Đảm bảo ESP32 đã gửi đúng dữ liệu qua mạng Wi-Fi.  

## 3. Tính năng mở rộng  
- **Tích hợp module mới**:  
  - Ví dụ: Cảm biến ánh sáng, module điều khiển từ xa hoặc Bluetooth.  
- **Điều khiển từ xa qua mạng Wi-Fi**:  
  - Điều chỉnh ngưỡng nhiệt độ, độ ẩm hoặc bật/tắt thiết bị.  
- **Ứng dụng thực tế**:  
  - Quản lý môi trường, giám sát nhà thông minh.  

## 4. Ghi chú  
- Đảm bảo các driver được phát triển tương thích với STM32.  
- Kiểm tra kết nối phần cứng trước khi chạy.  
- Cập nhật đầy đủ các thư viện và phần mềm liên quan.  

## 5. Thông tin bổ sung  
- Tài liệu chi tiết và mã nguồn nằm trong thư mục `Source Test/`.  
- Liên hệ nếu cần hỗ trợ: **(https://ohstem.vn/product-all/)**, **(https://ohstem.vn/product-all/)](https://www.st.com/en/development-tools/stm32cubeide.html)**
