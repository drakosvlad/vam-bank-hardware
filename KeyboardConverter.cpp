#include "KeyboardConverter.h"

#include "IKeyboardProcessor.h"

uint8_t KEY_MAPPINGS[17] 
{
  255, // N/A
  255, // N/A
  10,  // Accept
  0,   // 0
  11,  // Decline
  255, // N/A
  9,   // 9
  8,   // 8
  7,   // 7
  255, // N/A
  6,   // 6
  5,   // 5
  4,   // 4
  12,  // Backspace
  3,   // 3
  2,   // 2
  1    // 1
};

void KeyboardConverter::connectTo(IKeyboardProcessor& processor)
{
  _processor = &processor;
}

void KeyboardConverter::press(const uint8_t buttonNum)
{
  if (_processor != nullptr)
    _processor->press(KEY_MAPPINGS[buttonNum]);
}