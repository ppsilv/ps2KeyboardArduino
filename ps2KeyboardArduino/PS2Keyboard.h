#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <Arduino.h>

#define SHIFT 3

class FoxPS2Keyboard
{
public:
	FoxPS2Keyboard();
	byte GetScancode();
	void SendByte(byte b);
	bool WaitForKeyboard();
	bool FoxPS2Keyboard::SendByteWithConfirm(byte b, byte ACK, byte Tries, unsigned short Wait);
	bool FoxPS2Keyboard::SetKeyboardLights(byte NumLock, byte CapsLock, byte ScrollLock);
private:
	byte FoxPS2Keyboard::GetParity(byte b);
  //const char chrsNS[];
  //const char chrsSH[];
  //volatile uint16_t shift;
  //volatile uint16_t modifs;
  volatile bool cpslk;
  //void KeyboardISR();
};
