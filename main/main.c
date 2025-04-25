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