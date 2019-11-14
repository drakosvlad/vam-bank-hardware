#ifndef AnalogKeyboard_h
#define AnalogKeyboard_h

#include "IKeyboard.h"
#include "WiringCustom.h"

class IKeyboardProcessor;

const uint16_t KEY_BOUNDS[16] {
  920,  // 1
  1050, // 2
  1210, // 3
  1550, // 4
  1680, // 5
  1800, // 6
  1900, // 7
  2000, // 8
  2100, // 9
  2300, // 10
  2400, // 11
  2600, // 12
  2900, // 13
  3200, // 14
  3500, // 15
  3800  // 16
};

uint8_t getKeyByLevel(const uint16_t level)
{
  for (uint8_t i = 0; i < 16; ++i)
  {
    if (level < KEY_BOUNDS[i])
      return i;
  }

  return 16;
}

uint16_t getMedian(const uint16_t* median)
{
  uint16_t max = 0;
  uint16_t maxi = 0;
  for (int i = 0; i < 810; ++i)
  {
    if (median[i] > max)
    {
      max = median[i];
      maxi = i;
    }
  }

  return maxi * 5;
}

const uint32_t DEBOUNCE_TIME = 20;

class AnalogKeyboard : public IKeyboard
{
private:
  uint8_t _pin;
  uint16_t _value;
  uint32_t _debounce;
  IKeyboardProcessor* _processor = nullptr;

public:
  AnalogKeyboard(const uint8_t pin);
  ~AnalogKeyboard() override;
  AnalogKeyboard(const AnalogKeyboard &) = delete;

  void loop();
  void connectTo(IKeyboardProcessor& processor) override;
};

#endif