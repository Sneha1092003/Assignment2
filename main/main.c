//LCD_RS GPIO_NUM_21
//LCD_EN GPIO_NUM_22
//LCD_D4 GPIO_NUM_15
//LCD_D5 GPIO_NUM_23
//LCD_D6 GPIO_NUM_18
//LCD_D7 GPIO_NUM_19

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lcd.h"  // Include your custom LCD driver

void app_main(void)
{
    // Configure the LCD pins and 4-bit mode
    lcd_config_t config = {
        .rs_pin = GPIO_NUM_21,
        .en_pin = GPIO_NUM_22,
        .data_pins = {GPIO_NUM_15, GPIO_NUM_23, GPIO_NUM_18, GPIO_NUM_19},
        .is_4bit_mode = 1
    };

    // Initialize LCD
    lcd_init(&config);

    // Display your name
    lcd_display("Sneha");

    // Wait for 2 seconds
    vTaskDelay(pdMS_TO_TICKS(2000));

    // Scroll a message on LCD
    lcd_scroll("Hello Sneha, 300);
}
