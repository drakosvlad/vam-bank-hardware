#include "DisplayKeyboardProcessor.h"

DisplayKeyboardProcessor::DisplayKeyboardProcessor(LiquidCrystal_I2C& display)
  : _display(display)
{
  
}

void DisplayKeyboardProcessor::init()
{
  _display.clear();
  _display.setCursor(0, 0);
  _display.cursor_on();
  //_display.blink_on();
}

void DisplayKeyboardProcessor::press(uint8_t buttonNum)
{
  _display.print(buttonNum);
}