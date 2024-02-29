#ifndef MAIN_H
#define MAIN_H

// completed | checked 3/11/23
#include <Arduino.h>

#define PIN_BUTTON1 A0
#define PIN_BUTTON2 A1
#define PIN_BUTTON3 A2
#define PIN_BUZZER  A3

//7
#define PIN_LCD_RS  5
#define PIN_LCD_RW  6
#define PIN_LCD_EN  7
#define PIN_LCD_D4  8
#define PIN_LCD_D5  9
#define PIN_LCD_D6  10
#define PIN_LCD_D7  11
//
#define BTN_PAD_VALUE_INC_TIME  4
#define BTN_PAD_VALUE_DEC_TIME  2
#define BTN_PAD_VALUE_ABRT      6
#define BTN_PAD_VALUE_SP        1

typedef enum{
    NOT_PRESSED,
    BOUNCED,
    PRESSED
}button_state_t;

typedef enum{
    INC_TIME,
    DEC_TIME,
    TIME_TICK,
    START_PAUSE,
    ABRT,

    ENTRY,
    EXIT
}protimer_signal_t;

typedef enum{
    IDLE,
    TIME_SET,
    COUNTDOWN,
    PAUSE,
    STAT
}protimer_state_t;

typedef struct{
    uint32_t curr_time;
    uint32_t elapsed_time;
    uint32_t pro_time;
    protimer_state_t active_state;
}protimer_t;

typedef struct{
    uint8_t sig;
}event_t;

typedef struct{
    event_t super;
}protimer_user_event_t;

typedef struct{
    event_t super;
    uint8_t ss;
}protimer_tick_event;

typedef enum{
    EVENT_HANDLED,
    EVENT_IGNORED,
    EVENT_TRANSITION
}event_status_t;

void protimer_init(protimer_t *mobj);
event_status_t protimer_state_machine(protimer_t *const mobj, event_t const *const e);

#endif
