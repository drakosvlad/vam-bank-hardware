#include "DisplayKeyboardProcessor.h"

char * byteArrayToHex(byte arr[], uint32_t len) {
  const char * digits = "01234567890ABCDEF";
  char * res = new char[len * 2 + 1];

  for (uint32_t i = 0; i < len; i++)
  {
    res[i * 2] = digits[arr[i] & 15];
    res[i * 2 + 1] = digits[(arr[i] & 240) >> 4];
  }
  res[len * 2] = 0;

  return res;
}

char * readLineFromSerial()
{
  char * res = new char[100];
  int i = 0;

  while (true) {
    while (!Serial1.available());
    res[i] = Serial1.read();

    if (res[i] == '\r')
    {
      res[i] = 0;
      while (!Serial1.available());
      Serial1.read();
      return res;
    }

    i++;
  }
}

DisplayKeyboardProcessor::DisplayKeyboardProcessor(LiquidCrystal_I2C& display, MFRC522& cardReader, uint8_t buzzerPin)
  : _display(display), _cardReader(cardReader), _buzzerPin(buzzerPin)
{

}

void DisplayKeyboardProcessor::renderValueF()
{
  int i = 0;
  int32_t value = _value;

  _display.clear();
  _display.setCursor(0, 0);
  _display.print("Enter amount:");
  _display.setCursor(12, 1);
  _display.print("0.00");

  _display.setCursor(15, 1);
  
  while (value > 0)
  {
    _display.print(static_cast<char>((value % 10) + '0'));
    value /= 10;
    
    if (++i == 2) // Skip the . character 
      i = 3;
    
    _display.setCursor(15 - i, 1);
  }
}

void DisplayKeyboardProcessor::renderPin()
{
  int32_t pin = _pin;
  int i = 0;

  _display.clear();
  _display.setCursor(0, 0);
  _display.print("Enter PIN:");
  _display.setCursor(15, 1);
  
  while (pin > 0)
  {
    _display.print('X');
    pin /= 10;
    
    _display.setCursor(15 - ++i, 1);
  }
}

void DisplayKeyboardProcessor::init()
{
  state1();
  pinMode(_buzzerPin, OUTPUT);
}

void DisplayKeyboardProcessor::state1()
{
  _state = 1;
  _value = 0;
  _display.clear();
  _display.setCursor(0, 0);
  _display.print("Enter amount:");
  _display.setCursor(12, 1);
  _display.print("0.00");
}

void DisplayKeyboardProcessor::processState1(uint8_t buttonNum)
{
  if (buttonNum >= 0 && buttonNum <= 9)
  {
    // Input number
    if (_value < 100000000)
    {
      _value *= 10;
      _value += buttonNum;
    }
  }

  if (buttonNum == 12)
  {
    // Backspace
    if (_value == 0)
      buzz();
    else
      _value /= 10;
  }

  if (buttonNum == 11)
  {
    // Cancel
    _value = 0;
  }

  renderValueF();

  if (buttonNum == 10)
  {
    // Accept
    if (_value > 0)
      state2();
    else
      buzz();
  }
}

void DisplayKeyboardProcessor::state2()
{
  _state = 2;
  _display.clear();
  _display.setCursor(0, 0);
  _display.print("Place your card");
}

void DisplayKeyboardProcessor::processState2(uint8_t buttonNum)
{
  if (buttonNum == 11)
    state1();
  else
    blockingBuzz();
}

void DisplayKeyboardProcessor::state3()
{
  _state = 3;
  _display.clear();
  _display.setCursor(0, 0);
  _display.print("Verifying card");
  Serial1.print("CC/");
  Serial1.println(_cid);

  char * res = readLineFromSerial();
  if (strcmp(res, "S") == 0)
  {
    state4();
  }
  else
  {
    buzz();
    state2();
  }
  delete [] res;
}

void DisplayKeyboardProcessor::state4()
{
  _state = 4;
  _pin = 0;
  _display.clear();
  _display.setCursor(0, 0);
  _display.print("Enter PIN:");
}

void DisplayKeyboardProcessor::processState4(uint8_t buttonNum)
{
  if (buttonNum >= 0 && buttonNum <= 9)
  {
    // Input number
    if (_pin < 1000)
    {
      _pin *= 10;
      _pin += buttonNum;
    }
    else
    {
      buzz();
    }
  }

  if (buttonNum == 12)
  {
    // Backspace
    if (_pin == 0)
      buzz();
    else
      _pin /= 10;
  }

  if (buttonNum == 11)
  {
    // Cancel
    state1();
  }

  renderPin();

  if (buttonNum == 10)
  {
    // Accept
    if (_pin >= 1000)
      state5();
    else
      buzz();
  }
}

void DisplayKeyboardProcessor::state5()
{
  _state = 5;
  _display.clear();
  _display.setCursor(0, 0);
  _display.print("Transaction");
  _display.setCursor(0, 1);
  _display.print("pending...");

  Serial1.print("TR/");
  Serial1.print(_cid);
  Serial1.print("/");
  Serial1.print(_value, 10);
  Serial1.print('/');
  Serial1.println(_pin, 10);

  char * tid = readLineFromSerial(); // Transaction ID

  while (true)
  {
    Serial1.print("CH/");
    Serial1.println(tid);

    char * res = readLineFromSerial();
    if (strcmp(res, "S") == 0)
    {
      // Success
      state6();
      break;
    }
    else if (strcmp(res, "F") == 0)
    {
      buzz();
      state7();
      break;
    }
    delete [] res;
  }
}

void DisplayKeyboardProcessor::state6()
{
  _state = 6;
  _pin = 0;
  _display.clear();
  _display.setCursor(0, 0);
  _display.print("Transaction");
  _display.setCursor(0, 1);
  _display.print("succeeded!");
}

void DisplayKeyboardProcessor::processState6(uint8_t buttonNum)
{
  state1();
}

void DisplayKeyboardProcessor::state7()
{
  _state = 6;
  _pin = 0;
  _display.clear();
  _display.setCursor(0, 0);
  _display.print("Transaction");
  _display.setCursor(0, 1);
  _display.print("rejected!");
}

void DisplayKeyboardProcessor::press(uint8_t buttonNum)
{
  switch (_state)
  {
    case 1:
      processState1(buttonNum);
      break;
    case 2:
      processState2(buttonNum);
      break;
    case 4:
      processState4(buttonNum);
      break;
    case 6:
    case 7:
      processState6(buttonNum);
      break;
  }
}

void DisplayKeyboardProcessor::loop()
{
  if (millis() - _buzzerStart > BUZZ_TIME)
    digitalWrite(_buzzerPin, 0);
  else
    digitalWrite(_buzzerPin, 1);

  if (_state == 2)
    loopState2();
}

void DisplayKeyboardProcessor::loopState2()
{
  if (!_cardReader.PICC_IsNewCardPresent()) {
    return;
  }

  if (!_cardReader.PICC_ReadCardSerial()) {
    return;
  }

  _cid = byteArrayToHex(_cardReader.uid.uidByte, _cardReader.uid.size);
  delay(BUZZ_TIME);
  state3();
}

void DisplayKeyboardProcessor::buzz()
{
  _buzzerStart = millis();
}

void DisplayKeyboardProcessor::blockingBuzz()
{
  digitalWrite(_buzzerPin, 1);
  delay(BUZZ_TIME);
  digitalWrite(_buzzerPin, 0);
}