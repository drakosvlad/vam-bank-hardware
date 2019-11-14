#ifndef KeyboardConverter_h
#define KeyboardConverter_h

#include <stdint.h>

#include "IKeyboard.h"
#include "IKeyboardProcessor.h"

class KeyboardConverter : public IKeyboard, public IKeyboardProcessor
{
private:
  IKeyboardProcessor* _processor = nullptr;
public:
  KeyboardConverter() {  }; 
  ~KeyboardConverter() {  };
  void press(const uint8_t buttonNum) override;
  void connectTo(IKeyboardProcessor& processor) override;
};

#endif