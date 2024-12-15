#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <FirebaseESP32.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

// WiFi và Firebase cấu hình
#define WIFI_SSID "iPhone"
#define WIFI_PASSWORD "777********"
#define API_KEY "AIzaSyBDgWra86uFe0ma4fZlYewS9JEiHmYQN0A"
#define DATABASE_URL "aiot-bcb93-default-rtdb.asia-southeast1.firebasedatabase.app"
#define USER_EMAIL "*********@gmail.com"
#define USER_PASSWORD "*********"

// GPIO cấu hình
#define LED_PIN D13
#define FAN_PIN D5
#define INPUT_ADC A0
#define OUTPUT_ADC A1
#define RX D0
#define TX D1
#define NUM_PIXELS 4

// Cấu hình NeoPixel
#define LED_RGB_PIN D3
Adafruit_NeoPixel rgb(NUM_PIXELS, LED_RGB_PIN, NEO_GRB + NEO_KHZ800);

// Biến toàn cục
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
float temperature = 0.0;
float humidity = 0.0;

// Server WiFi
WiFiServer server(80);

// Hàm đọc dữ liệu từ STM32 qua UART2
void readSensorData() {
    static String data = "";
    while (Serial1.available()) {
        char c = Serial1.read();
        data += c;

        if (c == '\n') {
            Serial.print("Received line: ");
            Serial.print(data);

            if (data.startsWith("Humidity:")) {
                humidity = data.substring(9).toFloat();
            } else if (data.startsWith("Temperature:")) {
                temperature = data.substring(12).toFloat();
            }

            Serial.print("Temperature: ");
            Serial.print(temperature);
            Serial.print(" °C, Humidity: ");
            Serial.print(humidity);
            Serial.println(" %");

            data = "";
        }
    }
}

void updateLED(float temperature) {
    if (temperature < 20) {
        rgb.fill(rgb.Color(0, 0, 255)); // Xanh
    } else if (temperature >= 20 && temperature < 30) {
        rgb.fill(rgb.Color(255, 255, 0)); // Vàng
    } else {
        analogWrite(FAN_PIN, 250);
        rgb.fill(rgb.Color(255, 0, 0)); // Đỏ
    }
    rgb.show();
}




void setup() {
    // Cấu hình LED
    rgb.begin();
    rgb.show();
    pinMode(LED_PIN, OUTPUT);

    // Khởi tạo serial và WiFi
    Serial.begin(115200);
    WiFi.mode(WIFI_AP_STA);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("Connected to WiFi. IP Address: ");
    Serial.println(WiFi.localIP());

    WiFi.softAP("ESP32-AP", "123456789");
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());

    // Firebase khởi tạo
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.database_url = DATABASE_URL;
    config.token_status_callback = tokenStatusCallback;
    Firebase.reconnectNetwork(true);
    Firebase.begin(&config, &auth);

    Serial1.begin(9600, SERIAL_8N1, RX, TX);

    pinMode(OUTPUT_ADC, OUTPUT);
    pinMode(INPUT_ADC, INPUT);
}

    
void loop() {
    // Đọc giá trị cảm biến ánh sáng
    int sensorValue = analogRead(INPUT_ADC);
    digitalWrite(OUTPUT_ADC, sensorValue < 900 ? HIGH : LOW);

    // Đọc dữ liệu từ STM32 qua UART2
    readSensorData();

    // Gửi dữ liệu nhiệt độ và độ ẩm lên Firebase
    if (Firebase.setFloat(fbdo, "/sensorData/temperature", temperature)) {
        Serial.println("Temperature sent successfully");
    } else {
        Serial.print("Error sending temperature: ");
        Serial.println(fbdo.errorReason());
    }

    if (Firebase.setFloat(fbdo, "/sensorData/humidity", humidity)) {
        Serial.println("Humidity sent successfully");
    } else {
        Serial.print("Error sending humidity: ");
        Serial.println(fbdo.errorReason());
    }

    // Lấy trạng thái buttonState từ Firebase
    String buttonState = "";
    if (Firebase.getString(fbdo, "/sensorData/buttonState/state")) {
        buttonState = fbdo.stringData();
        Serial.print("Button state: ");
        Serial.println(buttonState);

        // Điều khiển chân D5 dựa trên trạng thái buttonState
        if (buttonState == "ON") {
            analogWrite(FAN_PIN, 250);  // Bật quạt (D5)
        } else if (buttonState == "OFF") {
            analogWrite(FAN_PIN, 0);   // Tắt quạt (D5)
        }
    } else {
        Serial.print("Error reading buttonState: ");
        Serial.println(fbdo.errorReason());
    }

    // Cập nhật trạng thái LED RGB
    updateLED(temperature);
    
    // Đợi 5 giây trước khi thực hiện lại
    delay(5000);
}

