/*
 * led_lcd_16x2.c
 *
 *  Created on: Nov 18, 2024
 *      Author: pc
 */

#include "led_lcd_16x2.h"
I2C_HandleTypeDef hi2c1;

uint8_t HeartChar[] = {0x00, 0x00, 0x0a, 0x15, 0x11, 0x0a, 0x04, 0x00};
uint8_t SmileyFaceChar[] = {0x00, 0x00, 0x0a, 0x00, 0x1f, 0x11, 0x0e, 0x00};

void LCD_InitializeAndDisplay() {
    I2C_LCD_Init(MyI2C_LCD);
    I2C_LCD_CreateCustomChar(MyI2C_LCD, 0, HeartChar);
    I2C_LCD_CreateCustomChar(MyI2C_LCD, 1, SmileyFaceChar);
    I2C_LCD_SetCursor(MyI2C_LCD, 0, 0);
    I2C_LCD_WriteString(MyI2C_LCD, "DeepBlueMbedded");
    I2C_LCD_SetCursor(MyI2C_LCD, 0, 1);
    I2C_LCD_WriteString(MyI2C_LCD, "I2C LCD ");
    I2C_LCD_PrintCustomChar(MyI2C_LCD, 1);
    I2C_LCD_PrintCustomChar(MyI2C_LCD, 0);
}


void LCD_DisplayTestSequence() {
       I2C_LCD_NoBacklight(MyI2C_LCD);
       HAL_Delay(1000);
       I2C_LCD_Backlight(MyI2C_LCD);
       HAL_Delay(1000);

       I2C_LCD_ShiftRight(MyI2C_LCD);
       HAL_Delay(500);
       I2C_LCD_ShiftRight(MyI2C_LCD);
       HAL_Delay(500);
       I2C_LCD_ShiftRight(MyI2C_LCD);
       HAL_Delay(500);

       I2C_LCD_ShiftLeft(MyI2C_LCD);
       HAL_Delay(500);
       I2C_LCD_ShiftLeft(MyI2C_LCD);
       HAL_Delay(500);
       I2C_LCD_ShiftLeft(MyI2C_LCD);
       HAL_Delay(500);

       I2C_LCD_Cursor(MyI2C_LCD);
       HAL_Delay(1000);
       I2C_LCD_Blink(MyI2C_LCD);
       HAL_Delay(2000);
       I2C_LCD_NoBlink(MyI2C_LCD);
       HAL_Delay(2000);

       I2C_LCD_NoCursor(MyI2C_LCD);
       HAL_Delay(1000);
       I2C_LCD_NoDisplay(MyI2C_LCD);
       HAL_Delay(1000);
       I2C_LCD_Display(MyI2C_LCD);
       HAL_Delay(1000);
   }












