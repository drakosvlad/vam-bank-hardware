#include "AnalogKeyboard.h"

#include "IKeyboardProcessor.h"

AnalogKeyboard::AnalogKeyboard(const uint8_t pin) : _pin(pin)
{

}

AnalogKeyboard::~AnalogKeyboard()
{

}

void AnalogKeyboard::loop()
{
  analogReadResolution(12);
  const uint16_t newValue = analogRead(_pin);
  Serial1.println(newValue);
  
  if (newValue < KEY_BOUNDS[0] && 
      _value > KEY_BOUNDS[0] && 
      (millis() - _debounce) >= DEBOUNCE_TIME)
  {
    // Key press finished, registering key press
    uint8_t key = getKeyByLevel(_value);
    Serial1.println(key);
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