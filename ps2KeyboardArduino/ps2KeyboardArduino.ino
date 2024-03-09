#include "PS2Keyboard.h"

//http://www-ug.eecg.utoronto.ca/desl/nios_devices_SoC/datasheets/PS2%20Protocol.htm
//https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
//https://www.win.tue.nl/~aeb/linux/kbd/scancodes-12.html


FoxPS2Keyboard keyboard;
char dat;

extern bool cpslk;
extern bool future_cpslk;

void setup()
{
	Serial.begin(9600);
	delay(1000);

	Serial.write("\n\n\n\nKeyboard test Init\n");

	if (keyboard.WaitForKeyboard() == false)
	{
		Serial.write("Keyboard not found... \nPress F1 to continue\n \nStoped......");
		while (1);
	}

	Serial.write("Lights\n");

  keyboard.setNumLock(true);
	keyboard.handleLeds();

	Serial.write("Completed\n");
}

void loop()
{
	dat = keyboard.GetCharcode();

	if (dat != 0){
		Serial.print(dat);
  }

  keyboard.handleLeds();


}
