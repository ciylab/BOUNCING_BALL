/**
 * @file display.h
 * @brief Champs de la classe d'affichage.
 */
 
#ifndef DISPLAY_H
#define DISPLAY_H

#include <U8g2lib.h>

#define BALL_SIZE 4 /**<diamètre de la balle.*/

#define SCREEN_WIDTH 128 /**<largeur de l'écran*/
#define SCREEN_HEIGHT 64 /**<hauteur de l'écran*/

void welcome();
void first_page();
void draw_ball();
void draw_frame();
bool next_page(); 

#endif
