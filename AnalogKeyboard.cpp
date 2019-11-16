#include "AnalogKeyboard.h"

#include "IKeyboardProcessor.h"

AnalogKeyboard::AnalogKeyboard(const uint8_t pin) : _pin(pin)
{
  analogReadResolution(12);
}

AnalogKeyboard::~AnalogKeyboard()
{

}

void AnalogKeyboard::loop()
{
  //static int i(0);
  const uint16_t newValue = analogRead(_pin);
  //if (i++ % 10 == 0)
  //  Serial1.println(newValue);
  
  if (newValue < KEY_BOUNDS[0] && 
      _value > KEY_BOUNDS[0] && 
      (millis() - _debounce) >= DEBOUNCE_TIME)
  {
    // Key press finished, registering key press
    uint8_t key = getKeyByLevel(_value);
    if (_processor != nullptr)
      _processor->press(key);
  }
  
  if (getKeyByLevel(_value) > getKeyByLevel(newValue))
  {
    _debounce = millis();
  }

  _value = newValue;
}

void AnalogKeyboard::connectTo(IKeyboardProcessor& processor)
{
  _processor = &processor;
}