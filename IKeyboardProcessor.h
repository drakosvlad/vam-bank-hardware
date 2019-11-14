#ifndef KeyboardProcessor_h
#define KeyboardProcessor_h

#include <stdint.h>

class IKeyboardProcessor
{
public:
  virtual ~IKeyboardProcessor() {  };
  virtual void press(const uint8_t buttonNum) = 0;
};

#endif