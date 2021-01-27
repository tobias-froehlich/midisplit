#ifndef CONST_H_
#define CONST_H_

#include <vector>
#include <string>

const unsigned char cNoteForExit = 108;

const char cCommentTag = '%';

const float cFloatDelta = 0.0000000001; // For testing equality of floats.

enum StatusByteType {
  unknown,
  note_on, note_off,
  pedal_pressed, pedal_released,
  bending
};

#endif
