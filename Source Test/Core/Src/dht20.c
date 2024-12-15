/*
 * dht20.c
 *
 *  Created on: Nov 8, 2024
 *      Author: HPVictus
 */

#include "DHT20.h"

// Định nghĩa các biến toàn cục


/* THỰC HIỆN ĐO VÀ TRUYỀN QUA LCD ở MAIN
Status_Trigger_DHT20 check_sensor  = trigger_measurement_data_dht20();
		char buffer[20];
		if (check_sensor == DHT20_OK)
		{
			sprintf (buffer, "Humidity: %0.1f", RH);
			lcd_put_cur(0,0);
			lcd_send_string (buffer);
			sprintf (buffer, "Temperature: %0.1f", Temp);
			lcd_put_cur(1,0);
			lcd_send_string (buffer);
			HAL_Delay(700);
		}
		else if (check_sensor == DHT20_BUSY)
		{
			sprintf (buffer, "Humidity:--BUSY--");
			lcd_put_cur(0,0);
			lcd_send_string (buffer);
			sprintf (buffer, "Temperature:--BUSY--");
			lcd_put_cur(1,0);
			lcd_send_string (buffer);
			HAL_Delay(1000);
			lcd_clear();
		}
		else
		{
			sprintf (buffer, "Humidity:--NONE--");
			lcd_put_cur(0,0);
			lcd_send_string (buffer);
			sprintf (buffer, "Temperature:--NONE--");
			lcd_put_cur(1,0);
			lcd_send_string (buffer);
			HAL_Delay(1000);
			lcd_clear();
		}
*/


//Status_Trigger_DHT20 check_sensor = trigger_measurement_data_dht20();
//	  char buffer[50];  // Tăng kích thước buffer để chứa toàn bộ chuỗi
//
//	  if (check_sensor == DHT20_OK)
//	  {
//	      snprintf(buffer, sizeof(buffer), "Humidity: %0.1f\r\n", RH);
//	      HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
//
//	      snprintf(buffer, sizeof(buffer), "Temperature: %0.1f\r\n", Temp);
//	      HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
//	  }
//	  else if (check_sensor == DHT20_BUSY)
//	  {
//	      snprintf(buffer, sizeof(buffer), "Humidity: --BUSY--\r\n");
//	      HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
//
//	      snprintf(buffer, sizeof(buffer), "Temperature: --BUSY--\r\n");
//	      HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
//	  }
//	  else
//	  {
//	      snprintf(buffer, sizeof(buffer), "Humidity: --NONE--\r\n");
//	      HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
//
//	      snprintf(buffer, sizeof(buffer), "Temperature: --NONE--\r\n");
//	      HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
//	  }
//	  HAL_Delay(1000);  // Th�?i gian ch�? giữa các lần gửi (tùy ch�?n)
unsigned char  CheckCrc8(unsigned char *pDat,unsigned char Lenth)
{
	unsigned char crc = 0xff, i, j;

	for (i = 0; i < Lenth ; i++)
	{
			crc = crc ^ *pDat;
			for (j = 0; j < 8; j++)
			{
					if (crc & 0x80) crc = (crc << 1) ^ 0x31;
					else crc <<= 1;
			}
			pDat++;
	}
	return crc;
}

uint8_t read_register(uint8_t register_pointer)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t return_value = 0;

	status = HAL_I2C_Mem_Read(&hi2c1, ADDRESS_SLAVE_DHT20 << 1, (uint16_t)register_pointer, I2C_MEMADD_SIZE_8BIT, &return_value, 1, 100);

	/* Check the communication status */
	if(status != HAL_OK)
	{

	}
	printf("return_value for %X : %X \n",register_pointer, return_value);
	return return_value;
}


//Trigger measurement data
Status_Trigger_DHT20 trigger_measurement_data_dht20()
{
	Status_Trigger_DHT20 status = DHT20_OK;

	HAL_Delay(100);
	uint8_t status_init = read_register(0x71);
	if ((status_init & 0x18) == 0x18) // et a byte of status word by sending 0x71. If the status word and 0x18 are not equal to 0x18
	{
		HAL_Delay(10);
		uint8_t data_t[3];
		data_t[0] = 0xAC; //address
		data_t[1] = 0x33; //data1
		data_t[2] = 0x00; //data2
		HAL_I2C_Master_Transmit (&hi2c1, ADDRESS_SLAVE_DHT20 << 1,(uint8_t *) data_t, 3, 100);
		HAL_Delay(80);

		uint8_t buffer[7];
		uint32_t data_read = 0;

    // receive the 2 x 8bit data into the receive buffer
    HAL_I2C_Master_Receive(&hi2c1, ADDRESS_SLAVE_DHT20 << 1, buffer, 7, 100);

		for (int i =0; i<7;i++)
		{
			printf("bufffer %d: %X \n",i, buffer[i]);
		}

		// Check Bit[7] at Byte0
		if ((buffer[0] & 0x80) == 0x00)
		{
			// Check CRC
			if(CheckCrc8(&buffer[0],6)==buffer[6])
			{
				printf("CRC OK \n");
				data_read=buffer[1]; //0x90
				data_read=data_read<<8;// 0x9000
				data_read+=buffer[2]; //0x9000 + 0x28 => 0x9028
				data_read=data_read<<8; //0x902800
				data_read+=buffer[3]; //0x902800 + 0x96 => 0x902896
				data_read=data_read>>4;				 // 0x902896 => 0x90289

				RH = (float)data_read*100/1048576;
				printf("RH %0.1f%% \n", RH);

				data_read=buffer[3]&0x0F;
				data_read=data_read<<8;
				data_read+=buffer[4];
				data_read=data_read<<8;
				data_read+=buffer[5];

				Temp = (float)data_read*200/1048576-50;
				printf("Temp %0.1f \n", Temp);
			}
			else
			{
				printf("CRC fail \n");
				status = DHT20_ERROR_CRC;
			}
		}
		else
		{
			printf("Check Bit[7] at Byte0: %x \n",buffer[0]&0x80);
			status = DHT20_BUSY;
		}

	}
	else //  initialize the 0x1B, 0x1C, 0x1E registers
	{
		status = DHT20_ERROR_INIT;
	}
	return status;
}
