/*
 * led_rgb.c
 *
 *  Created on: Nov 18, 2024
 *      Author: pc
 */

#include "led_grb.h"

#define LED_COUNT 4            // Số lượng LED
#define HIGH_TIME 12          // Tín hiệu PWM "1" (0.8µs)
#define LOW_TIME 6           // Tín hiệu PWM "0" (0.4µs)
#define RESET_PULSES 50        // Số xung "0" để reset (tương đương 50µs)

uint8_t LED_Data[LED_COUNT][3];        // Dữ liệu LED [LED][G, R, B]
uint16_t PWM_Data[24 * LED_COUNT + RESET_PULSES];  // Dữ liệu PWM

//TIM_HandleTypeDef htim2; // Timer cấu hình trước đó

// Hàm set màu cho từng LED
void Set_LED_Color(uint8_t led, uint8_t red, uint8_t green, uint8_t blue) {
    if (led < LED_COUNT) {
        LED_Data[led][0] = green; // WS2812 theo thứ tự G-R-B
        LED_Data[led][1] = red;
        LED_Data[led][2] = blue;
    }
}

// Chuyển đổi dữ liệu LED thành tín hiệu PWM
void WS2812_Update() {
    uint16_t pwm_index = 0;

    // Tạo dữ liệu PWM từ dữ liệu LED
    for (int i = 0; i < LED_COUNT; i++) {
        for (int j = 0; j < 24; j++) {
            if (LED_Data[i][j / 8] & (1 << (7 - (j % 8)))) {
                PWM_Data[pwm_index] = HIGH_TIME;  // Bit "1"
            } else {
                PWM_Data[pwm_index] = LOW_TIME;   // Bit "0"
            }
            pwm_index++;
        }
    }

    // Thêm tín hiệu "Reset" ở cuối
    for (int i = 0; i < RESET_PULSES; i++) {
        PWM_Data[pwm_index++] = 0;
    }

    // Kiểm tra trạng thái DMA trước khi bắt đầu
    HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *)PWM_Data, pwm_index);
}

// Hàm đổi màu theo nhiệt độ
void Update_Color_By_Temperature(uint8_t temperature) {
    uint8_t red = 0, green = 0, blue = 0;

    if (temperature <= 25) {
        // Lạnh -> màu xanh dương
        blue = 255 - (temperature * 10);
        green = temperature * 10;
    } else if (temperature <= 50) {
        // Nhiệt độ trung bình -> xanh lá chuyển sang đỏ
        green = 255 - ((temperature - 25) * 10);
        red = (temperature - 25) * 10;
    } else {
        // Nóng -> màu đỏ
        red = 255;
        blue = (temperature - 50) * 5;
    }

    // Cập nhật màu LED và gửi dữ liệu
    Set_LED_Color(0, red, green, blue);
    WS2812_Update();
}

// Main loop (giả lập)
/*
int main(void) {
    HAL_Init();
    MX_GPIO_Init();
    MX_TIM2_Init();

    // Bắt đầu Timer PWM
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

    uint8_t temperature = 0;

    while (1) {
        temperature = GetTemperature(); // Hàm giả lập hoặc đọc từ cảm biến
        Update_Color_By_Temperature(temperature);
        HAL_Delay(100); // Cập nhật mỗi 100ms
    }
}

// Hàm giả lập đọc nhiệt độ
uint8_t GetTemperature(void) {
    uint32_t adc_value = HAL_ADC_GetValue(&hadc1); // Đọc giá trị ADC
    return (adc_value * 75) / 4095;                // Chuyển đổi sang nhiệt độ
}
*/
