#include "lcd.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static lcd_config_t lcd_cfg;

static void lcd_enable_pulse() {
    gpio_set_level(lcd_cfg.en_pin, 1);
    ets_delay_us(1);
    gpio_set_level(lcd_cfg.en_pin, 0);
    ets_delay_us(100);
}

static void lcd_send_nibble(uint8_t nibble) {
    for (int i = 0; i < 4; i++) {
        gpio_set_level(lcd_cfg.data_pins[i], (nibble >> i) & 0x01);
    }
    lcd_enable_pulse();
}

static void lcd_send_cmd(uint8_t cmd) {
    gpio_set_level(lcd_cfg.rs_pin, 0);
    lcd_send_nibble(cmd >> 4);
    lcd_send_nibble(cmd & 0x0F);
    vTaskDelay(pdMS_TO_TICKS(2));
}

static void lcd_send_data(uint8_t data) {
    gpio_set_level(lcd_cfg.rs_pin, 1);
    lcd_send_nibble(data >> 4);
    lcd_send_nibble(data & 0x0F);
    vTaskDelay(pdMS_TO_TICKS(2));
}

void lcd_init(lcd_config_t* config) {
    lcd_cfg = *config;
    gpio_set_direction(lcd_cfg.rs_pin, GPIO_MODE_OUTPUT);
    gpio_set_direction(lcd_cfg.en_pin, GPIO_MODE_OUTPUT);
    for (int i = 0; i < 4; i++) {
        gpio_set_direction(lcd_cfg.data_pins[i], GPIO_MODE_OUTPUT);
    }

    vTaskDelay(pdMS_TO_TICKS(50));
    lcd_send_nibble(0x03); vTaskDelay(pdMS_TO_TICKS(5));
    lcd_send_nibble(0x03); vTaskDelay(pdMS_TO_TICKS(5));
    lcd_send_nibble(0x03); vTaskDelay(pdMS_TO_TICKS(1));
    lcd_send_nibble(0x02);

    lcd_send_cmd(0x28); 
    lcd_send_cmd(0x0C); 
    lcd_send_cmd(0x06); 
    lcd_send_cmd(0x01); 
}

void lcd_clear() {
    lcd_send_cmd(0x01);
}

void lcd_display(const char* text) {
    lcd_clear();
    while (*text) {
        lcd_send_data(*text++);
    }
}

void lcd_scroll(const char* text, int speed_ms) {
    int len = strlen(text);
    char buffer[17] = {0};
    for (int i = 0; i <= len; i++) {
        strncpy(buffer, text + i, 16);
        lcd_display(buffer);
        vTaskDelay(pdMS_TO_TICKS(speed_ms));
    }
}
