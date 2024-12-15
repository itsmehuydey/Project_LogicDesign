/*
 * led_grb.h
 *
 *  Created on: Nov 18, 2024
 *      Author: pc
 */

#ifndef INC_LED_GRB_H_
#define INC_LED_GRB_H_
#include "main.h"
#include <math.h>
void Update_Color_By_Temperature(uint8_t temperature);
void WS2812_Update() ;
void Set_LED_Color(uint8_t led, uint8_t red, uint8_t green, uint8_t blue) ;
#endif /* INC_LED_GRB_H_ */
