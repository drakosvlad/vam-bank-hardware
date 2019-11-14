#ifndef IKeyboard_h
#define IKeyboard_h

#include <stdint.h>

class IKeyboardProcessor;

class IKeyboard
{
public:
  virtual ~IKeyboard() {  };
  virtual void connectTo(IKeyboardProcessor& processor) = 0;
};

#endif