/**
 * @file display.cpp
 * @brief Gestion de l'affichage.
 *
 * La variable screen représente ce qui est affiché.
 * La variable buffer est ce qui doit être affiché.
 * Pour des questions de latence, screen est mis à jour à partir
 * de buffer tant qu'elles sont différentes et un caractère
 * par boucle de loop(). 
 */


#include "display.h"

/**
 * Oled SSD1306 ou SH1106.
 */
#ifdef SSD1306
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
#elif SH1106
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
#endif

extern int ballX;
extern int ballY;
extern int margin;

/*
 * Pour illustrer un exemple d'utilisation.
 */ 
void welcome() {
    char temp[16];
    u8g2.begin();
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

void first_page() {
    u8g2.firstPage();
}

void draw_ball() {
    u8g2.drawDisc(ballX, ballY, BALL_SIZE / 2);
}

void draw_frame() {
    u8g2.drawFrame(2 * margin, margin, 
            SCREEN_WIDTH - 4 * margin, SCREEN_HEIGHT - 2 * margin);
}

bool next_page() {
    return u8g2.nextPage();
}

