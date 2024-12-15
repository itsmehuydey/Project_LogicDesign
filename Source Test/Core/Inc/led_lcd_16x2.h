/*
 * led_lcd_16x2.h
 *
 *  Created on: Nov 18, 2024
 *      Author: pc
 */


#ifndef INC_LED_LCD_16X2_H_
#define INC_LED_LCD_16X2_H_

#include "main.h"
#include "I2C_LCD.h"
#include <stdio.h>
#include <string.h>
extern I2C_HandleTypeDef hi2c1;
#define MyI2C_LCD I2C_LCD_1
//extern uint8_t HeartChar[];
//extern uint8_t SmileyFaceChar[];
//void LCD_InitializeAndDisplay();
//void LCD_DisplayTestSequence();
//void readSensorData();
//void lcd_init (void);   // initialize lcd
//
//void lcd_send_cmd (char cmd);  // send command to the lcd
//
//void lcd_send_data (char data);  // send data to the lcd
//
//void lcd_send_string (char *str);  // send string to the lcd
//
//void lcd_put_cur(int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);
//
//void lcd_clear (void);
//
//void lcd_send_float(int integer_part, int fractional_part);
#endif /* INC_LED_LCD_16X2_H_ */
