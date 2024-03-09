#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <Arduino.h>

#define SHIFT 1
#define SCROLL_LOCK   1
#define NUM_LOCK      2
#define CAPS_LOCK     4

#define C_SCROLL_LOCK   ~SCROLL_LOCK
#define C_NUM_LOCK      ~NUM_LOCK
#define C_CAPS_LOCK     ~CAPS_LOCK

class FoxPS2Keyboard
{
public:
	FoxPS2Keyboard();
	byte GetScancode();
  byte GetCharcode();
	void SendByte(byte b);
	bool WaitForKeyboard();
  void FoxPS2Keyboard::setNumLock();
  void FoxPS2Keyboard::handleLeds();
	bool FoxPS2Keyboard::SendByteWithConfirm(byte b, byte ACK, byte Tries, unsigned short Wait);
	bool FoxPS2Keyboard::SetKeyboardLights( );
private:
	byte FoxPS2Keyboard::GetParity(byte b);
};
