#ifndef AnalogKeyboard_h
#define AnalogKeyboard_h

#include "IKeyboard.h"
#include "WiringCustom.h"

class IKeyboardProcessor;

const uint16_t KEY_BOUNDS[16] {
  900,  // 1
  1050, // 2
  1200, // 3
  1400, // 4
  1700, // 5
  1700, // 6
  1900, // 7
  2100, // 8
  2200, // 9
  2200, // 10
  2500, // 11
  2650, // 12
  3100, // 13
  3400, // 14
  3650, // 15
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