/**
 * @file midi.cpp
 * @brief Gestion des messages MIDI.
 */

#include "midi.h"

/** Pour envoyer une note à chaque imact.*/
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

void midi_init(void) {
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.turnThruOff();
}

void play_note_on(byte pitch) {
    MIDI.sendNoteOn(pitch, 127, 1);
}
