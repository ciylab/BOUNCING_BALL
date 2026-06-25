#ifndef ENCODER_H
#define ENCODER_H
#include <Versatile_RotaryEncoder.h>
#include <Arduino.h>
#define NUM_ENC 2

/** Marge max.*/
#define MARGIN_MAX 20

void handleRotate0(int8_t rotation);
void handleLongPress0();
void handleRotate1(int8_t rotation);
#endif
