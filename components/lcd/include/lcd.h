#ifndef LCD_H
#define LCD_H

#include "driver/gpio.h"

// Adjust these GPIO numbers according to your wiring
#define LCD_RS GPIO_NUM_21
#define LCD_EN GPIO_NUM_22
#define LCD_D4 GPIO_NUM_16
#define LCD_D5 GPIO_NUM_17
#define LCD_D6 GPIO_NUM_18
#define LCD_D7 GPIO_NUM_19

void lcd_init(void);
void lcd_clear(void);
void lcd_send_cmd(uint8_t cmd);
void lcd_send_data(uint8_t data);
void lcd_send_string(const char *str);
void lcd_set_cursor(uint8_t row, uint8_t col);

#endif
