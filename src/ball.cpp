/**
 * @file ball.cpp
 * @brief Gestion de la trajectoire.
 */

#include "ball.h"
#include "midi.h"

byte pitch; /**<pitch de la note jouée*/
bool open; /**<Gate*/

/**
 * Coordonnées de la position de départ.
 */
int ballX = 64;
int ballY = 32;

/**
 * @brief Liste des directions en pixels.
 *
 * On divise un quart de tour de 0 à PI/2 sur un cercle de rayon 50 de sorte
 * que la vitesse est constante dans toutes les directions.
 *
 * Les valeurs sont ajoutées à chaque pas.
 */

int direction[10][2] = {
    { 6 , 0 }, 
    { 6 , 1 }, 
    { 6 , 2 }, 
    { 5 , 3 }, 
    { 5 , 4 }, 
    { 4 , 5 }, 
    { 3 , 5 }, 
    { 2 , 6 }, 
    { 1 , 6 }, 
    { 0 , 6 }
};

/**
 * @brief Marge variable du cadre.
 */

int margin = 0;

/** 
 * @brief Direction par défaut.
 */
int idx = 4;

/**
 * La direction change au moment de l'impact.
 */
int bufferedIdx = 4;

unsigned long now = millis(); /**<L'instant présent en millisecondes.*/

/**
 * @brief Déplacement de la balle et génération des signaux.
 *
 * La fonction calcule la future position de la balle et gère les 
 * rebonds et la génération des gates de 2 millisecondes.
 *
 * À chaque rebond, on envoie une note.
 */
void move(void) {
    if (open && millis() - now > 2) {
        digitalWrite(CLOCK_OUT, LOW);
        open = false;
        play_note_on(pitch);
    } else if (!open && millis() > now + STEP) {
        ballX += direction[idx][0];
        ballY += direction[idx][1];
        if (ballX < 2 + 2 * margin) {
            ballX = 2 + 2 * margin;
            idx = bufferedIdx;
            direction[idx][0] = -direction[idx][0];
            digitalWrite(CLOCK_OUT, HIGH);
            open = true;
            pitch = HIGH_PITCH;
            play_note_on(pitch);
        } else if (ballX > SCREEN_WIDTH - 2 - 2 * margin) {
            idx = bufferedIdx;
            ballX = SCREEN_WIDTH - 2 - 2 * margin;
            direction[idx][0] = -direction[idx][0];
            digitalWrite(CLOCK_OUT, HIGH);
            open = true;
            pitch = HIGH_PITCH;
            play_note_on(pitch);
        } else if (ballY < 2 + margin) {
            idx = bufferedIdx;
            ballY = 2 + margin;
            direction[idx][1] = -direction[idx][1];
            digitalWrite(CLOCK_OUT, HIGH);
            open = true;
            pitch = LOW_PITCH;
            play_note_on(pitch);
        } else if (ballY > SCREEN_HEIGHT - 2 - margin) {
            idx = bufferedIdx;
            ballY = SCREEN_HEIGHT - 2 - margin;
            direction[idx][1] = -direction[idx][1];
            digitalWrite(CLOCK_OUT, HIGH);
            open = true;
            pitch = LOW_PITCH;
            play_note_on(pitch);
        }
        now = millis();
    }
}
