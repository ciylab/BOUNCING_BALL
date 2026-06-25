/**
 * @file encoder.cpp
 * @brief Comportement général des deux encodeurs.
 * 
 * Les appels sont antérieurs aux autres appels.
 *
 */

#include "encoder.h"

extern int bufferedIdx;
extern int margin;

/**
 * L'encodeur PARAMETER fait varier la direction.
 */
void handleRotate0(int8_t rotation) {
    if(0 < rotation && bufferedIdx < 9) {
        bufferedIdx++;
    } else if (rotation < 0 && 0 < bufferedIdx) {
        bufferedIdx--;
    }
}

/**
 * @brief Pression longue de l'encodeur PARAMETER.
 *
 * Reboot.
 */

void handleLongPress0() {
    asm volatile("jmp 0x00");
}


/**
 * L'encodeur VALUE fait varier la marge du cadre.
 */
void handleRotate1(int8_t rotation) {
    if(0 < rotation && margin < MARGIN_MAX) {
        margin++;
    } else if (rotation < 0 && 1 < margin) {
        margin--;
    }
}

