#ifndef DisplayKeyboardProcessor_h
#define DisplayKeyboardProcessor_h

#include "IKeyboardProcessor.h"
#include <LiquidCrystal_I2C.h>

class DisplayKeyboardProcessor : public IKeyboardProcessor
{
private:
  LiquidCrystal_I2C& _display;
public:
  DisplayKeyboardProcessor(LiquidCrystal_I2C& display);
  ~DisplayKeyboardProcessor() override {  }
  DisplayKeyboardProcessor(const DisplayKeyboardProcessor &dkp) : _display(dkp._display) {  }
  DisplayKeyboardProcessor& operator=(const DisplayKeyboardProcessor &dkp)
  {
    _display = dkp._display;
  }

  void press(const uint8_t buttonNum) override;
  void init();
};

#endif