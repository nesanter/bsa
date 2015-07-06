/* 
 * File:   pins.h
 * Author: noah
 *
 * Created on December 24, 2013, 8:49 AM
 */

#ifndef PINS_H
#define	PINS_H

typedef enum {
    PIN_GROUP_A,
    PIN_GROUP_B
} u_pin_group;

#define PIN_OUTPUT 0
#define PIN_INPUT 1
#define PIN_DIGITAL 0
#define PIN_ANALOG 1

typedef struct {
    u_pin_group group;
    int pin;
} Pin;

#define PIN_A_CHECK(P) (P >= 0 && P < 5)
#define PIN_B_CHECK(P) (P >= 0 && P < 15 && P != 6 && P != 12)

void pin_clear(Pin p);
void pin_set(Pin p);
void pin_toggle(Pin p);
int pin_read(Pin p);
void pin_mode_set(Pin p, int input, int analog);
int pin_test(Pin p);
int pin_mode_io_get(Pin p);
int pin_mode_ad_get(Pin p);

#endif	/* PINS_H */

