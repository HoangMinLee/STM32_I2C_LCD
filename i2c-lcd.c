/** 
Edit by modify: Hoang-Le 
**/

#include "i2c-lcd.h"
extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly

#define SLAVE_ADDRESS_LCD 0x4E // change this according to ur setup

void lcd_send_cmd (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_send_data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=1
	data_t[1] = data_u|0x09;  //en=0, rs=1
	data_t[2] = data_l|0x0D;  //en=1, rs=1
	data_t[3] = data_l|0x09;  //en=0, rs=1
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_init (void)
{   
	/*4 bit initialization */
	usleep(20000);
	lcd_send_cmd (0x30);
	usleep(50000)
	lcd_send_cmd (0x30);
	usleep(200);
	lcd_send_cmd (0x30); 
	usleep(200);
	lcd_send_cmd (0x02); /*set 4 bit mode */
	/* display intitialization */
	usleep(200);
	lcd_send_cmd (0x28);/*DL -> number data length (4 bit or 8 bit) 
                          Data sent or received in 8 bit lengths (DB7-DB0) when DL = 1
                          Data sent or received in 4 bit lengths (DB7-DB4) when DL = 0 */
    lcd_send_cmd (0x08) /*Display on/off control --> D=0,C=0, B=0  ---> display off*/
    usleep(1000);
	lcd_send_cmd(0x01); /* clear display */
 	usleep(2000);
	lcd_send_cmd(0x06); /*Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)*/
    usleep(1000);
	lcd_send_cmd(0x0C);/*Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)*/
	usleep(2000);



void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}

void lcd_clear_display (void)
{
	lcd_send_cmd (0x01); //clear display
}

void lcd_put_cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }

    lcd_send_cmd (col);
}
