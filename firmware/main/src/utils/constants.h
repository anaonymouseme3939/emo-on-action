// firmware/main/src/utils/constants.h
#pragma once

#include <stdint.h>

/*═══════════════════════════════════════════════════════════════════════════
                    ENUMERATIONS & CONSTANTS
  ═══════════════════════════════════════════════════════════════════════════*/

// ============================================================================
// System Modes
// ============================================================================
typedef enum {
    MODE_VOICE_CHAT = 0,
    MODE_WEATHER = 1,
    MODE_SETTINGS = 2,
    MODE_IDLE = 3,
    MODE_ALARM = 4
} system_mode_t;

// ============================================================================
// Emotions / States
// ============================================================================
typedef enum {
    EMOTION_NEUTRAL = 0,
    EMOTION_HAPPY = 1,
    EMOTION_SAD = 2,
    EMOTION_ANGRY = 3,
    EMOTION_THINKING = 4,
    EMOTION_LISTENING = 5,
    EMOTION_SPEAKING = 6,
    EMOTION_EXCITED = 7,
    EMOTION_CONFUSED = 8,
    EMOTION_SHOCKED = 9,
    EMOTION_BLUSHING = 10
} emotion_t;

// ============================================================================
// Button Events
// ============================================================================
typedef enum {
    BUTTON_NONE = 0,
    BUTTON_SINGLE_CLICK = 1,
    BUTTON_DOUBLE_CLICK = 2,
    BUTTON_TRIPLE_CLICK = 3,
    BUTTON_LONG_PRESS = 4,
    BUTTON_VERY_LONG_PRESS = 5  // 5+ seconds
} button_event_t;

typedef enum {
    BUTTON_MODE = 0,
    BUTTON_PLUS = 1,
    BUTTON_MINUS = 2
} button_id_t;

// ============================================================================
// Touch Events (Capacitive Touch)
// ============================================================================
typedef enum {
    TOUCH_NONE = 0,
    TOUCH_SINGLE_TAP = 1,
    TOUCH_DOUBLE_TAP = 2,
    TOUCH_LONG_PRESS_1S = 3,      // 1 second hold
    TOUCH_LONG_PRESS_3S = 4       // 3 seconds hold (reboot)
} touch_event_t;

// ============================================================================
// WiFi & Bluetooth Status
// ============================================================================
typedef enum {
    STATUS_DISCONNECTED = 0,
    STATUS_CONNECTING = 1,
    STATUS_CONNECTED = 2,
    STATUS_FAILED = 3,
    STATUS_UNKNOWN = 4
} connection_status_t;

// ============================================================================
// Voice Chat States
// ============================================================================
typedef enum {
    VOICE_IDLE = 0,
    VOICE_LISTENING = 1,
    VOICE_PROCESSING = 2,
    VOICE_SPEAKING = 3,
    VOICE_ERROR = 4
} voice_state_t;

// ============================================================================
// RGB Color Definitions (24-bit)
// ============================================================================
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_color_t;

#define COLOR_BLACK         {0, 0, 0}
#define COLOR_WHITE         {255, 255, 255}
#define COLOR_RED           {255, 0, 0}
#define COLOR_GREEN         {0, 255, 0}
#define COLOR_BLUE          {0, 0, 255}
#define COLOR_CYAN          {0, 255, 255}
#define COLOR_MAGENTA       {255, 0, 255}
#define COLOR_YELLOW        {255, 255, 0}

// Futuristic Theme Colors
#define COLOR_THEME_PRIMARY   {0, 255, 200}    // Cyan
#define COLOR_THEME_ACCENT    {100, 150, 255}  // Light blue
#define COLOR_THEME_WARNING   {255, 150, 0}    // Orange
#define COLOR_THEME_ERROR     {255, 50, 50}    // Red
#define COLOR_THEME_SUCCESS   {50, 200, 100}   // Green

// ============================================================================
// Error Codes
// ============================================================================
typedef enum {
    ERR_OK = 0,
    ERR_WIFI_FAILED = -1,
    ERR_BT_FAILED = -2,
    ERR_API_TIMEOUT = -3,
    ERR_API_ERROR = -4,
    ERR_SD_FAILED = -5,
    ERR_NVS_FAILED = -6,
    ERR_MEMORY_ALLOC = -7,
    ERR_INVALID_PARAM = -8,
    ERR_TIMEOUT = -9,
    ERR_UNKNOWN = -99
} error_code_t;

// ============================================================================
// Alarm Configuration
// ============================================================================
typedef enum {
    ALARM_DISABLED = 0,
    ALARM_ENABLED = 1,
    ALARM_TRIGGERED = 2,
    ALARM_SNOOZED = 3
} alarm_state_t;

#define SNOOZE_DURATION_MIN 9  // Snooze for 9 minutes
#define MAX_ALARMS 5
#define ALARM_NAME_MAX 32
#define RINGTONE_PATH_MAX 128

// ============================================================================
// String Constants
// ============================================================================

// Mode Names
#define STR_MODE_VOICE "Voice Chat"
#define STR_MODE_WEATHER "Weather"
#define STR_MODE_SETTINGS "Settings"
#define STR_MODE_IDLE "Idle"
#define STR_MODE_ALARM "Alarm"

// Emotion Names
#define STR_EMO_HAPPY "Happy"
#define STR_EMO_SAD "Sad"
#define STR_EMO_ANGRY "Angry"
#define STR_EMO_THINKING "Thinking"
#define STR_EMO_LISTENING "Listening"
#define STR_EMO_SPEAKING "Speaking"

// Status Messages
#define STR_STATUS_CONNECTING "Connecting..."
#define STR_STATUS_CONNECTED "Connected"
#define STR_STATUS_FAILED "Failed"
#define STR_STATUS_LISTENING "🎤 LISTENING..."
#define STR_STATUS_PROCESSING "⏳ PROCESSING..."
#define STR_STATUS_SPEAKING "🗣️ SPEAKING..."

// ============================================================================
// Time Constants
// ============================================================================
#define SECONDS_PER_MINUTE 60
#define SECONDS_PER_HOUR 3600
#define SECONDS_PER_DAY 86400

// ============================================================================
// Memory Limits
// ============================================================================
#define MAX_LOG_LINE_LENGTH 256
#define MAX_JSON_BUFFER 2048
#define MAX_AUDIO_BUFFER 32000
#define MAX_TEXT_BUFFER 512

// ============================================================================
// Animation Timing
// ============================================================================
#define ANIMATION_FRAME_DELAY_MS 50   // 20 FPS
#define ANIMATION_BLINK_DURATION 150   // ms
#define ANIMATION_SMILE_DURATION 1000  // ms

// ============================================================================
// Task Stack Sizes (bytes)
// ============================================================================
#define STACK_SIZE_LARGE 4096
#define STACK_SIZE_MEDIUM 2048
#define STACK_SIZE_SMALL 1024

// ============================================================================
// Task Priorities (FreeRTOS)
// ============================================================================
#define PRIORITY_CRITICAL 5
#define PRIORITY_HIGH 4
#define PRIORITY_NORMAL 3
#define PRIORITY_LOW 2
#define PRIORITY_IDLE 1

#endif // CONSTANTS_H
