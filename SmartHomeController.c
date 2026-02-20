#include <stdio.h>
#include <stdint.h>

/* Device bits */
#define Light       0
#define Fan         1
#define TV          2
#define AC          3
#define Headphones  4
#define Speaker     5
#define Curtain     6
#define Dim_LED     7

#define LONG_PRESS_TIME 1000

typedef enum {
    MOVIE,
    MUSIC,
    WORK,
    REGULAR
} MODES;

typedef enum {
    EVENT_NONE,
    EVENT_LONG_PRESS,
    EVENT_SHORT_PRESS
} EVENT;

/* Globals */
uint8_t devices = 0;
MODES current_mode = REGULAR;
EVENT event = EVENT_NONE;

unsigned long timer = 0;
unsigned long press_start = 0;

uint8_t button_now = 0;
uint8_t button_prev = 0;

/* Device actions */
void toggle_light(void) {
    devices ^= (1U << Light);
    printf("Light toggled\n");
}

void curtain_open(void) {
    devices ^= (1U << Curtain);
    printf("Curtain toggled\n");
}

void pause_music(void) {
    devices ^= (1U << Headphones);
    printf("Music paused\n");
}

void pause_movie(void) {
    devices ^= (1U << TV);
    printf("Movie paused\n");
}

void all_off(void) {
    devices = 0x00;
}

/* Mode setup (called ONLY on mode entry) */
void movie(void) {
    all_off();
    devices |= (1U << TV) | (1U << Curtain);
    printf("MOVIE mode\n");
}

void music(void) {
    all_off();
    devices |= (1U << Headphones) | (1U << Curtain);
    printf("MUSIC mode\n");
}

void regular(void) {
    all_off();
    devices |= (1U << Light) | (1U << Fan);
    printf("REGULAR mode\n");
}

void work(void) {
    all_off();
    devices |= (1U << Curtain);
    printf("WORK mode\n");
}

/* Input */
void read_user_input(void) {
    int b;
    printf("Enter 1 (press) or 0 (release): ");
    scanf("%d", &b);
    button_now = (b != 0);
}

/* Button timing */
void button_handling(void) {
    event = EVENT_NONE;

    if (button_now == 1 && button_prev == 0) {
        press_start = timer;
    }

    if (button_now == 0 && button_prev == 1) {
        unsigned long duration = timer - press_start;
        if (duration >= LONG_PRESS_TIME)
            event = EVENT_LONG_PRESS;
        else
            event = EVENT_SHORT_PRESS;
    }

    button_prev = button_now;
}

/* FSM */
void smart_controller(void) {
    static MODES last_mode = -1;

    if (current_mode != last_mode) {
        switch (current_mode) {
            case REGULAR: regular(); break;
            case WORK:    work();    break;
            case MUSIC:   music();   break;
            case MOVIE:   movie();   break;
        }
        last_mode = current_mode;
    }

    if (event == EVENT_LONG_PRESS) {
        switch (current_mode) {
            case REGULAR: current_mode = WORK;  break;
            case WORK:    current_mode = MUSIC; break;
            case MUSIC:   current_mode = MOVIE; break;
            case MOVIE:   current_mode = REGULAR; break;
        }
    }
}

void short_press(void) {
    if (event != EVENT_SHORT_PRESS) return;

    switch (current_mode) {
        case REGULAR: toggle_light(); break;
        case WORK:    curtain_open(); break;
        case MUSIC:   pause_music();  break;
        case MOVIE:   pause_movie();  break;
    }
}

int main(void) {
    printf("Starting Smart Home Controller\n");

    while (1) {
        timer++;

        read_user_input();
        button_handling();
        smart_controller();
        short_press();
    }
}
