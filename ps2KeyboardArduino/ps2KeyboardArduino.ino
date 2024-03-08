#include "FoxPS2Keyboard.h"

//http://www-ug.eecg.utoronto.ca/desl/nios_devices_SoC/datasheets/PS2%20Protocol.htm
//https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
//https://www.win.tue.nl/~aeb/linux/kbd/scancodes-12.html


FoxPS2Keyboard keyboard;
uint8_t dat;

extern bool cpslk;
extern bool future_cpslk;

void setup()
{
	Serial.begin(9600);
	delay(1000);

	Serial.write("\n\n\n\nKeyboard test Init\n");

	if (keyboard.WaitForKeyboard() == false)
	{
		Serial.write("Keyboard not found... Press F1 to continue\n");
		while (1);
	}

	Serial.write("Lights\n");

	keyboard.SetKeyboardLights(1, 0, 0);

	Serial.write("Completed\n");
}
static volatile bool caps = false;
void loop()
{
	dat = keyboard.GetScancode();
	if (dat != 0){
    Serial.print(" ");
		Serial.print(dat, 16);
  }

  if ( cpslk == future_cpslk ){
    if ( future_cpslk )
      keyboard.SetKeyboardLights(0, 1, 0);
    else  
      keyboard.SetKeyboardLights(0, 0, 0);
    future_cpslk = !future_cpslk;
  }
      
  if ( dat == 0x42 ){
    keyboard.SetKeyboardLights(0, 0, 1);
  }  

}
