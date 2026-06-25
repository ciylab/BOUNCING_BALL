/**
 * @file BOUNCING_BALL.ino
 * @brief Balle rebondissante dans un rectangle.
 * @author Pierrick MEIGNEN
 * @version 1
 * @date 28 janvier 2025
 * 
 * Demo fun pour illustrer la possibilité de coder
 * comme on veut le module MIDI TOOLS. Adaptation simple
 * aux autres modules.
 *
 * L'impact sur chaque mur génère une note (LOW_PITCH/HIGH_PITCH). 
 * Le tempo augmente comme la margin du cadre.
 */
 
#include "src/midi.h"
#include "src/encoder.h"
#include "src/ball.h"
#include "src/display.h"

/** Encodeur PARAMETER */
Versatile_RotaryEncoder *versatile_encoder_parameter;
/** Encodeur VALUE */
Versatile_RotaryEncoder *versatile_encoder_value;

/**Le tableau des deux encodeurs SW2 et SW1 dans l'ordre.*/
Versatile_RotaryEncoder *versatile_encoder[NUM_ENC];

/* Pour le module MIDI->MIDI */
int sws[2]  = {10, 20}; /**<pin des switchs */
int dts[2]  = {11, 21}; /**<pin des datas */
int clks[2] = {12, 17}; /**<pin des clocks */

/**
 * @brief Fonction d'initialisation.
 *
 */
void setup() {
    /* rotary encoders */
    versatile_encoder_parameter = 
        new Versatile_RotaryEncoder(clks[0], dts[0], sws[0]);
    versatile_encoder_value = 
        new Versatile_RotaryEncoder(clks[1], dts[1], sws[1]);
    pinMode(CLOCK_OUT, OUTPUT);
    versatile_encoder_parameter->setHandleRotate(handleRotate0);
    versatile_encoder_parameter->setHandleLongPress(handleLongPress0);
    versatile_encoder_value->setHandleRotate(handleRotate1);
    midi_init();
    welcome();
    delay(2000);
}

/**
 * @brief Boucle principale.
 *
 * Algorithme :
 *  - lecture des encodeurs
 *  - nouvelle position
 *  - affichage
 */
void loop() {
    first_page();
    do {
        versatile_encoder_parameter->ReadEncoder();
        versatile_encoder_value->ReadEncoder();
        move();
        draw_ball();
        draw_frame();
    } while(next_page());
}
