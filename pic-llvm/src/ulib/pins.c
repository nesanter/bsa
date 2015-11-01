/*
 *  Pin control routines
 */

#include "ulib/pins.h"
#include "proc/p32mx250f128b.h"

void pin_clear(Pin p) {
    switch (p.group) {
        case PIN_GROUP_A:
            PORTACLR |= p.pin;
            break;
        case PIN_GROUP_B:
            PORTBCLR |= p.pin;
            break;
    }
}

void pin_set(Pin p) {
    switch (p.group) {
        case PIN_GROUP_A:
            PORTASET |= p.pin;
            break;
        case PIN_GROUP_B:
            PORTBSET |= p.pin;
            break;
    }
}

void pin_toggle(Pin p) {
    switch (p.group) {
        case PIN_GROUP_A:
            PORTAINV |= p.pin;
            break;
        case PIN_GROUP_B:
            PORTBINV |= p.pin;
            break;
    }
}

int pin_read(Pin p) {
    switch (p.group) {
        case PIN_GROUP_A:
            return PORTA & p.pin;
        case PIN_GROUP_B:
            return PORTB & p.pin;
    }
}

int pin_test(Pin p) {
    switch (p.group) {
        case PIN_GROUP_A:
            return (PORTA & p.pin) ? 1 : 0;
        case PIN_GROUP_B:
            return (PORTB & p.pin) ? 1 : 0;
    }
}

void pin_mode_set(Pin p, int input, int analog) {
    switch (p.group) {
        case PIN_GROUP_A:
            if (input) {
                TRISASET |= p.pin;
            } else {
                TRISACLR |= p.pin;
            }
            if (analog) {
                ANSELASET |= p.pin;
            } else {
                ANSELACLR |= p.pin;
            }
            break;
        case PIN_GROUP_B:
            if (input) {
                TRISBSET |= p.pin;
            } else {
                TRISBCLR |= p.pin;
            }
            if (analog) {
                ANSELBSET |= p.pin;
            } else {
                ANSELBCLR |= p.pin;
            }
            break;
    }
}

int pin_mode_io_get(Pin p) {
    switch (p.group) {
        case PIN_GROUP_A:
            return TRISA & p.pin;
        case PIN_GROUP_B:
            return TRISB & p.pin;
    }
}

int pin_mode_ad_get(Pin p) {
    switch (p.group) {
        case PIN_GROUP_A:
            return ANSELA & p.pin;
        case PIN_GROUP_B:
            return ANSELB & p.pin;
    }
}
