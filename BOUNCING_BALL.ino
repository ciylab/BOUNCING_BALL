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
#include <U8g2lib.h>
#include <Versatile_RotaryEncoder.h>
#include <MIDI.h>

/** pin de la led. */
#define CLOCK_OUT 2

/** Marge max.*/
#define MARGIN_MAX 20

/** Duére d'un pas en millis.*/
#define STEP 30

#define BALL_SIZE 4 /**<diamètre de la balle.*/

#define SCREEN_WIDTH 128 /**<largeur de l'écran*/
#define SCREEN_HEIGHT 64 /**<hauteur de l'écran*/

#define LOW_PITCH 24
#define HIGH_PITCH 48

#ifdef SSD1306
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
#elif SH1106
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
#endif

/** Pour envoyer une note à chaque imact.*/
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

/** Encodeur PARAMETER */
Versatile_RotaryEncoder *versatile_encoder_parameter;
/** Encodeur VALUE */
Versatile_RotaryEncoder *versatile_encoder_value;

/**Le tableau des deux encodeurs SW2 et SW1 dans l'ordre.*/
Versatile_RotaryEncoder *versatile_encoder[2];

/* Pour le module MIDI->MIDI */
int sws[2]  = {10, 20}; /**<pin des switchs */
int dts[2]  = {11, 21}; /**<pin des datas */
int clks[2] = {12, 17}; /**<pin des clocks */

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
 * @brief Direction par défaut.
 */
int idx = 4;

/**
 * La direction change au moment de l'impact.
 */
int bufferedIdx = 4;

/**
 * @brief Marge variable du cadre.
 */

int margin;

byte pitch; /**<pitch de la note jouée*/
bool open; /**<Gate*/

unsigned long now; /**<L'instant présent en millisecondes.*/

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
        MIDI.sendNoteOff(pitch, 127, 1);
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
            MIDI.sendNoteOn(pitch, 127, 1);
        } else if (ballX > SCREEN_WIDTH - 2 - 2 * margin) {
            idx = bufferedIdx;
            ballX = SCREEN_WIDTH - 2 - 2 * margin;
            direction[idx][0] = -direction[idx][0];
            digitalWrite(CLOCK_OUT, HIGH);
            open = true;
            pitch = HIGH_PITCH;
            MIDI.sendNoteOn(pitch, 127, 1);
        } else if (ballY < 2 + margin) {
            idx = bufferedIdx;
            ballY = 2 + margin;
            direction[idx][1] = -direction[idx][1];
            digitalWrite(CLOCK_OUT, HIGH);
            open = true;
            pitch = LOW_PITCH;
            MIDI.sendNoteOn(pitch, 127, 1);
        } else if (ballY > SCREEN_HEIGHT - 2 - margin) {
            idx = bufferedIdx;
            ballY = SCREEN_HEIGHT - 2 - margin;
            direction[idx][1] = -direction[idx][1];
            digitalWrite(CLOCK_OUT, HIGH);
            open = true;
            pitch = LOW_PITCH;
            MIDI.sendNoteOn(pitch, 127, 1);
        }
        now = millis();
    }
}

/**
 * L'encodeur PARAMETER fait varier la direction.
 */
void handleRotate_parameter(int8_t rotation) {
    if(0 < rotation && bufferedIdx < 9) {
        bufferedIdx++;
    } else if (rotation < 0 && 0 < bufferedIdx) {
        bufferedIdx--;
    }
}

/**
 * L'encodeur VALUE fait varier la margin du cadre.
 */
void handleRotate_value(int8_t rotation) {
    if(0 < rotation && margin < MARGIN_MAX) {
        margin++;
    } else if (rotation < 0 && 1 < margin) {
        margin--;
    }
}

/**
 * Pour illustrer un exemple d'utilisation.
 */ 
void welcome() {
    char temp[16];
    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_10x20_tr);
        u8g2.drawStr(0, 16, " BOUNCING");
        u8g2.drawStr(0, 32, " BALL");
        u8g2.drawStr(0, 48, " BY CIYLAB");
        sprintf(temp, " %s",  VERSION);
        u8g2.drawStr(0, 64, temp);
    } while (u8g2.nextPage());
}

/**
 * @brief Fonction d'initialisation.
 *
 */
void setup() {
    margin = 0;
    /* rotary encoders */
    versatile_encoder_parameter = 
        new Versatile_RotaryEncoder(clks[0], dts[0], sws[0]);
    versatile_encoder_value = 
        new Versatile_RotaryEncoder(clks[1], dts[1], sws[1]);
    pinMode(CLOCK_OUT, OUTPUT);
    versatile_encoder_parameter->setHandleRotate(handleRotate_parameter);
    versatile_encoder_value->setHandleRotate(handleRotate_value);
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.turnThruOff();
    now = millis();
    u8g2.begin();
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
    u8g2.firstPage();
    do {
        versatile_encoder_parameter->ReadEncoder();
        versatile_encoder_value->ReadEncoder();
        move();
        u8g2.drawDisc(ballX, ballY, BALL_SIZE / 2);
        u8g2.drawFrame(2 * margin, margin, 
                SCREEN_WIDTH - 4 * margin, SCREEN_HEIGHT - 2 * margin);
    } while(u8g2.nextPage());
}
