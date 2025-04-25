<<<<<<< HEAD
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <string.h>

// ==== LCD Pin Definitions (Change as needed) ====
#define LCD_RS GPIO_NUM_21
#define LCD_EN GPIO_NUM_22
#define LCD_D4 GPIO_NUM_15
#define LCD_D5 GPIO_NUM_23
#define LCD_D6 GPIO_NUM_18
#define LCD_D7 GPIO_NUM_19

// ==== Delay Helpers ====
void lcd_delay_us(int us) {
    ets_delay_us(us);
}

// ==== Low-level GPIO helpers ====
void lcd_send_nibble(uint8_t nibble) {
    gpio_set_level(LCD_D4, (nibble >> 0) & 0x01);
    gpio_set_level(LCD_D5, (nibble >> 1) & 0x01);
    gpio_set_level(LCD_D6, (nibble >> 2) & 0x01);
    gpio_set_level(LCD_D7, (nibble >> 3) & 0x01);

    // Pulse EN
    gpio_set_level(LCD_EN, 1);
    lcd_delay_us(1);
    gpio_set_level(LCD_EN, 0);
    lcd_delay_us(100);
}

void lcd_send_byte(uint8_t byte, bool is_data) {
    gpio_set_level(LCD_RS, is_data);
    lcd_send_nibble(byte >> 4);   // High nibble
    lcd_send_nibble(byte & 0x0F); // Low nibble
    vTaskDelay(pdMS_TO_TICKS(2)); // Short wait
}

// ==== LCD Commands ====
void lcd_cmd(uint8_t cmd) {
    lcd_send_byte(cmd, false);
}

void lcd_char(char c) {
    lcd_send_byte(c, true);
}

void lcd_init() {
    // Configure pins
    gpio_set_direction(LCD_RS, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_EN, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_D4, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_D5, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_D6, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_D7, GPIO_MODE_OUTPUT);

    vTaskDelay(pdMS_TO_TICKS(50));

    // 4-bit mode init sequence
    lcd_send_nibble(0x03); vTaskDelay(pdMS_TO_TICKS(5));
    lcd_send_nibble(0x03); lcd_delay_us(150);
    lcd_send_nibble(0x03); lcd_send_nibble(0x02);

    lcd_cmd(0x28); // 4-bit mode, 2 lines
    lcd_cmd(0x0C); // Display ON, cursor OFF
    lcd_cmd(0x06); // Entry mode
    lcd_cmd(0x01); // Clear display
    vTaskDelay(pdMS_TO_TICKS(5));
}

void lcd_set_cursor(int row, int col) {
    const uint8_t row_offsets[] = {0x00, 0x40};
    lcd_cmd(0x80 | (col + row_offsets[row]));
}

void lcd_print(const char *str) {
    while (*str) {
        lcd_char(*str++);
    }
}

// ==== Main app ====
void app_main(void) {
    lcd_init();
    lcd_set_cursor(0, 3);  // Row 0, Column 3
    lcd_print("Hello Sneha!");
}
=======
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"


#define RS GPIO_NUM_21
#define EN GPIO_NUM_22
#define D4 GPIO_NUM_15
#define D5 GPIO_NUM_23
#define D6 GPIO_NUM_18
#define D7 GPIO_NUM_19

gpio_num_t data_pins[4] = {D4, D5, D6, D7};

// Delay helpers
void lcd_delay_us(uint32_t us) {
    esp_rom_delay_us(us); // Accurate short delays
}

void lcd_pulse_enable() {
    gpio_set_level(EN, 0);
    lcd_delay_us(1);
    gpio_set_level(EN, 1);
    lcd_delay_us(1);
    gpio_set_level(EN, 0);
    lcd_delay_us(100);
}

void lcd_send_nibble(uint8_t nibble) {
    for (int i = 0; i < 4; i++) {
        gpio_set_level(data_pins[i], (nibble >> i) & 0x01);
    }
    lcd_pulse_enable();
}

void lcd_send_byte(uint8_t byte, bool is_data) {
    gpio_set_level(RS, is_data);
    lcd_send_nibble(byte >> 4);
    lcd_send_nibble(byte & 0x0F);
    if (!is_data) vTaskDelay(pdMS_TO_TICKS(2)); // Command delay
}

void lcd_init() {
    gpio_set_direction(RS, GPIO_MODE_OUTPUT);
    gpio_set_direction(EN, GPIO_MODE_OUTPUT);
    for (int i = 0; i < 4; i++) {
        gpio_set_direction(data_pins[i], GPIO_MODE_OUTPUT);
    }

    vTaskDelay(pdMS_TO_TICKS(50)); // Wait after power-up

    // Init sequence for 4-bit mode
    lcd_send_nibble(0x03); vTaskDelay(pdMS_TO_TICKS(5));
    lcd_send_nibble(0x03); lcd_delay_us(100);
    lcd_send_nibble(0x03); lcd_send_nibble(0x02);

    // Function set: 4-bit, 2-line, 5x8 dots
    lcd_send_byte(0x28, false);
    // Display ON, Cursor OFF, Blink OFF
    lcd_send_byte(0x0C, false);
    // Entry mode: Increment cursor
    lcd_send_byte(0x06, false);
    // Clear display
    lcd_send_byte(0x01, false);
    vTaskDelay(pdMS_TO_TICKS(2));
}

void lcd_print(const char* str) {
    while (*str) {
        lcd_send_byte(*str++, true);
    }
}

void app_main() {
    lcd_init();
    lcd_print("Sneha");
}
>>>>>>> cf23f5418b3c4a1333c2f6ac7956db43f020d348
