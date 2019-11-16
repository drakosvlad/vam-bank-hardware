#ifndef DisplayKeyboardProcessor_h
#define DisplayKeyboardProcessor_h

#include "IKeyboardProcessor.h"
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>


const uint32_t BUZZ_TIME = 100;

/**
 * === STATES ===
 * 1: Enter amount
 * 2: Read card
 * 3: Verify card
 * 4: Enter pin
 * 5: Wait for transaction
 * 6: Transaction success
 * 7:
 */
class DisplayKeyboardProcessor : public IKeyboardProcessor
{
private:
  LiquidCrystal_I2C& _display;
  MFRC522& _cardReader;
  const uint8_t _buzzerPin;
  uint32_t _buzzerStart = 0;

  uint8_t _state = 1;
  int32_t _value = 0;
  int16_t _pin = 0;

  char * _cid = nullptr;

  void state1();
  void processState1(uint8_t buttonNum);
  void state2();
  void processState2(uint8_t buttonNum);
  void loopState2();
  void state3();
  void state4();
  void processState4(uint8_t buttonNum);
  void state5();
  void state6();
  void processState6(uint8_t buttonNum);
  void state7();

  void renderValueF();
  void renderPin();

  void buzz();
  void blockingBuzz();
public:
  DisplayKeyboardProcessor(LiquidCrystal_I2C& display, MFRC522& cardReader, uint8_t buzzerPin);
  ~DisplayKeyboardProcessor() override {  }
  DisplayKeyboardProcessor(const DisplayKeyboardProcessor &dkp) : _display(dkp._display), _cardReader(dkp._cardReader), _buzzerPin(dkp._buzzerPin) {  }
  DisplayKeyboardProcessor& operator=(const DisplayKeyboardProcessor &dkp)
  {
    _display = dkp._display;
  }

  void press(const uint8_t buttonNum) override;
  void init();
  void loop();
};

#endif