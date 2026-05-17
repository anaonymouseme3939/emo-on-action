// firmware/main/src/display/oled_display.cpp
#include "oled_display.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "../utils/config.h"
#include <string.h>
#include <stdlib.h>

static const char *TAG = "OLED Display";

// SSD1306 I2C commands
#define SSD1306_CMD_SETCONTRAST        0x81
#define SSD1306_CMD_DISPLAYALLON       0xA4
#define SSD1306_CMD_NORMALDISPLAY      0xA6
#define SSD1306_CMD_INVERTDISPLAY      0xA7
#define SSD1306_CMD_DISPLAYOFF         0xAE
#define SSD1306_CMD_DISPLAYON          0xAF
#define SSD1306_CMD_SETCOMPINS         0xDA
#define SSD1306_CMD_SETVCOMDESELECT    0xDB

// Framebuffer
static uint8_t framebuffer[OLED_WIDTH * OLED_HEIGHT / 8] = {0};

esp_err_t oled_init() {
    ESP_LOGI(TAG, "Initializing OLED display...");
    
    // Initialize I2C
    i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = OLED_SDA_PIN,
        .scl_io_num = OLED_SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000,  // 400 kHz
    };
    
    i2c_param_config(I2C_NUM_0, &i2c_config);
    i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
    
    // Initialize SSD1306
    uint8_t init_commands[] = {
        SSD1306_CMD_DISPLAYOFF,
        0x20, 0x00,  // Set addressing mode to Horizontal
        0x21, 0, 127,  // Column addresses
        0x22, 0, 7,    // Page addresses
        0x40,  // Set display start line to 0
        SSD1306_CMD_SETCONTRAST, 0x7F,  // Set contrast
        SSD1306_CMD_NORMALDISPLAY,
        0xA8, 0x3F,  // Set multiplex ratio
        0xD3, 0x00,  // Set display offset
        0xD5, 0x80,  // Set display clock divide
        0xD9, 0xF1,  // Set pre-charge period
        SSD1306_CMD_SETCOMPINS, 0x12,
        SSD1306_CMD_SETVCOMDESELECT, 0x40,
        0x8D, 0x14,  // Enable charge pump
        SSD1306_CMD_DISPLAYON
    };
    
    i2c_master_write_to_device(I2C_NUM_0, OLED_ADDRESS, init_commands, 
                              sizeof(init_commands), pdMS_TO_TICKS(100));
    
    oled_clear();
    
    ESP_LOGI(TAG, "✓ OLED display initialized");
    return ESP_OK;
}

void oled_clear() {
    memset(framebuffer, 0x00, sizeof(framebuffer));
    oled_update();
}

void oled_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT) return;
    
    uint16_t index = x + (y / 8) * OLED_WIDTH;
    if (color) {
        framebuffer[index] |= (1 << (y % 8));
    } else {
        framebuffer[index] &= ~(1 << (y % 8));
    }
}

void oled_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    int16_t dx = abs(x2 - x1);
    int16_t dy = abs(y2 - y1);
    int16_t sx = (x1 < x2) ? 1 : -1;
    int16_t sy = (y1 < y2) ? 1 : -1;
    int16_t err = dx - dy;
    
    int16_t x = x1, y = y1;
    
    while (1) {
        oled_draw_pixel(x, y, color);
        
        if (x == x2 && y == y2) break;
        
        int16_t e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

void oled_draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, 
                   uint16_t color, bool filled) {
    if (filled) {
        for (uint16_t i = 0; i < width; i++) {
            for (uint16_t j = 0; j < height; j++) {
                oled_draw_pixel(x + i, y + j, color);
            }
        }
    } else {
        oled_draw_line(x, y, x + width - 1, y, color);
        oled_draw_line(x + width - 1, y, x + width - 1, y + height - 1, color);
        oled_draw_line(x + width - 1, y + height - 1, x, y + height - 1, color);
        oled_draw_line(x, y + height - 1, x, y, color);
    }
}

void oled_draw_circle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color, bool filled) {
    int16_t f = 1 - radius;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * radius;
    int16_t px = 0;
    int16_t py = radius;
    
    if (filled) {
        oled_draw_line(x, y - radius, x, y + radius, color);
    }
    
    oled_draw_pixel(x, y - radius, color);
    oled_draw_pixel(x, y + radius, color);
    oled_draw_pixel(x - radius, y, color);
    oled_draw_pixel(x + radius, y, color);
    
    while (px < py) {
        if (f >= 0) {
            py--;
            ddF_y += 2;
            f += ddF_y;
        }
        px++;
        ddF_x += 2;
        f += ddF_x;
        
        if (filled) {
            oled_draw_line(x - px, y - py, x + px, y - py, color);
            oled_draw_line(x - px, y + py, x + px, y + py, color);
            oled_draw_line(x - py, y - px, x + py, y - px, color);
            oled_draw_line(x - py, y + px, x + py, y + px, color);
        } else {
            oled_draw_pixel(x + px, y - py, color);
            oled_draw_pixel(x - px, y - py, color);
            oled_draw_pixel(x + px, y + py, color);
            oled_draw_pixel(x - px, y + py, color);
            oled_draw_pixel(x + py, y - px, color);
            oled_draw_pixel(x - py, y - px, color);
            oled_draw_pixel(x + py, y + px, color);
            oled_draw_pixel(x - py, y + px, color);
        }
    }
}

void oled_draw_text(uint16_t x, uint16_t y, const char *text, uint16_t fg, uint16_t bg) {
    // Simple text rendering (5x7 font assumed)
    // Implementation depends on available font library
    // Placeholder for now
    ESP_LOGI(TAG, "Text: %s at (%u, %u)", text, x, y);
}

void oled_display_face(emotion_t emotion) {
    oled_clear();
    
    uint16_t face_color = COLOR_WHITE;
    
    // Select emotion color
    switch (emotion) {
        case EMOTION_HAPPY:
            face_color = COLOR_HAPPY;
            break;
        case EMOTION_SAD:
            face_color = COLOR_SAD;
            break;
        case EMOTION_ANGRY:
            face_color = COLOR_ANGRY;
            break;
        case EMOTION_EXCITED:
            face_color = COLOR_EXCITED;
            break;
        default:
            face_color = COLOR_WHITE;
    }
    
    // Draw simple face
    uint16_t center_x = OLED_WIDTH / 2;
    uint16_t center_y = OLED_HEIGHT / 2;
    
    // Head (circle)
    oled_draw_circle(center_x, center_y, 15, face_color, false);
    
    // Eyes (two small circles)
    oled_draw_circle(center_x - 8, center_y - 5, 2, face_color, true);
    oled_draw_circle(center_x + 8, center_y - 5, 2, face_color, true);
    
    // Mouth
    if (emotion == EMOTION_HAPPY) {
        oled_draw_circle(center_x, center_y + 8, 5, face_color, false);
    } else if (emotion == EMOTION_SAD) {
        oled_draw_circle(center_x, center_y + 10, 5, face_color, false);
    } else if (emotion == EMOTION_ANGRY) {
        oled_draw_line(center_x - 5, center_y + 8, center_x + 5, center_y + 10, face_color);
    }
    
    oled_update();
}

void oled_display_weather() {
    oled_clear();
    oled_draw_text(0, 0, "Weather: 29C", COLOR_WHITE, COLOR_BLACK);
    oled_draw_text(0, 8, "Scattered T-storms", COLOR_WHITE, COLOR_BLACK);
    oled_update();
}

void oled_display_settings() {
    oled_clear();
    oled_draw_text(0, 0, "Settings Menu", COLOR_WHITE, COLOR_BLACK);
    oled_draw_text(0, 16, "Voice Chat: ON", COLOR_WHITE, COLOR_BLACK);
    oled_draw_text(0, 32, "Weather: ON", COLOR_WHITE, COLOR_BLACK);
    oled_update();
}

void oled_display_listening(uint32_t duration_ms) {
    oled_clear();
    oled_draw_text(0, 0, "LISTENING...", COLOR_HAPPY, COLOR_BLACK);
    char duration_str[32];
    sprintf(duration_str, "Duration: %lu ms", duration_ms);
    oled_draw_text(0, 16, duration_str, COLOR_WHITE, COLOR_BLACK);
    oled_update();
}

void oled_display_processing() {
    oled_clear();
    oled_draw_text(0, 0, "Processing...", COLOR_EXCITED, COLOR_BLACK);
    oled_draw_text(0, 16, "Please wait", COLOR_WHITE, COLOR_BLACK);
    oled_update();
}

void oled_display_speaking() {
    oled_clear();
    oled_draw_text(0, 0, "Speaking...", COLOR_HAPPY, COLOR_BLACK);
    oled_draw_text(0, 16, "AI Response", COLOR_WHITE, COLOR_BLACK);
    oled_update();
}

void oled_update() {
    // Send framebuffer to display via I2C
    uint8_t cmd[] = {0x21, 0, 127};  // Column address
    i2c_master_write_to_device(I2C_NUM_0, OLED_ADDRESS, cmd, sizeof(cmd), 
                              pdMS_TO_TICKS(100));
    
    cmd[0] = 0x22;  // Page address
    cmd[1] = 0;
    cmd[2] = 7;
    i2c_master_write_to_device(I2C_NUM_0, OLED_ADDRESS, cmd, sizeof(cmd), 
                              pdMS_TO_TICKS(100));
    
    uint8_t data_buffer[1 + sizeof(framebuffer)];
    data_buffer[0] = 0x40;  // Data byte
    memcpy(&data_buffer[1], framebuffer, sizeof(framebuffer));
    
    i2c_master_write_to_device(I2C_NUM_0, OLED_ADDRESS, data_buffer, 
                              sizeof(data_buffer), pdMS_TO_TICKS(100));
}

void oled_set_brightness(uint8_t level) {
    uint8_t cmd[] = {SSD1306_CMD_SETCONTRAST, level};
    i2c_master_write_to_device(I2C_NUM_0, OLED_ADDRESS, cmd, sizeof(cmd), 
                              pdMS_TO_TICKS(100));
}

void oled_invert_display(bool invert) {
    uint8_t cmd = invert ? SSD1306_CMD_INVERTDISPLAY : SSD1306_CMD_NORMALDISPLAY;
    i2c_master_write_to_device(I2C_NUM_0, OLED_ADDRESS, &cmd, 1, 
                              pdMS_TO_TICKS(100));
}
