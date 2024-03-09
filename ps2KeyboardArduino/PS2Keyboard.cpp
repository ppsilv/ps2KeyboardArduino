#include"PS2Keyboard.h"

#define TransistorIRQPin  6
#define TransistorDataPin 7
#define DataPin 4
#define IRQPin 3

const char chrsNS[]={
    0,    249,  0,    245,  243,  241,  242,  252,  0,    250,  248,  246,  244,  '\t', '`',  0,
    0,    0,    0,    0,    0,    'q',  '1',  0,    0,    0,    'z',   's',  'a',  'w',  '2',  0,
    0,    'c',  'x',  'd',  'e',  '4',  '3',  0,    0,    ' ',  'v',  'f',  't',  'r',  '5',  0,
    0,    'n',  'b',  'h',  'g',  'y',  '6',  0,    0,    0,    'm',  'j',  'u',  '7',  '8',  0,
    0,    ',',  'k',  'i',  'o',  '0',  '9',  0,    0,    '.',  '/',  'l',  ';',  'p',  '-',  0,
    0,    0,    '\'', 0,    '[',  '=',  0,    0,    0,    0,    '\n', ']',  0,    '\\', 0,    0,
    0,    0,    0,    0,    0,    0,    '\b', 0,    0,    '1',  0,    '4',  '7',  0,    0,    0,
    '0',  '.',  '2',  '5',  '6',  '8',  '\033',0,   251,  '+',  '3',  '-',  '*',  '9',  0,    0,
    0,    0,    0,    247,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0};

const char chrsSH[]={
    0,    249,  0,    245,  243,  241,  242,  252,  0,    250,  248,  246,  244,  '\t', '~',  0,
    0,    0,    0,    0,    0,    'Q',  '!',  0,    0,    0,    'Z',  'S',  'A',  'W',  '@',  0,
    0,    'C',  'X',  'D',  'E',  '$',  '#',  0,    0,    ' ',  'V',  'F',  'T',  'R',  '%',  0,
    0,    'N',  'B',  'H',  'G',  'Y',  '^',  0,    0,    0,    'M',  'J',  'U',  '&',  '*',  0,
    0,    '<',  'K',  'I',  'O',  ')',  '(',  0,    0,    '>',  '?',  'L',  ':',  'P',  '_',  0,
    0,    0,    '\"', 0,    '{',  '+',  0,    0,    0,    0,    '\n', '}',  0,    '|',  0,    0,
    0,    0,    0,    0,    0,    0,    '\b', 0,    0,    '1',  0,    '4',  '7',  0,    0,    0,
    '0',  '.',  '2',  '5',  '6',  '8',  '\033',0,   0,    '+',  '3',  '-',  '*',  '9',  0,    0,
    0,    0,    0,    247,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0};

const char chrsSH1[]={
    0,    249,  0,    245,  243,  241,  242,  252,  0,    250,  248,  246,  244,  '\t', '~',  0,
    0,    0,    0,    0,    0,    'q',  '!',  0,    0,    0,    'z',  's',  'a',  'w',  '@',  0,
    0,    'c',  'x',  'd',  'e',  '$',  '#',  0,    0,    ' ',  'v',  'f',  't',  'r',  '%',  0,
    0,    'n',  'b',  'h',  'g',  'y',  '^',  0,    0,    0,    'm',  'j',  'u',  '&',  '*',  0,
    0,    '<',  'k',  'i',  'O',  ')',  '(',  0,    0,    '>',  '?',  'l',  ':',  'p',  '_',  0,
    0,    0,    '\"', 0,    '{',  '+',  0,    0,    0,    0,    '\n', '}',  0,    '|',  0,    0,
    0,    0,    0,    0,    0,    0,    '\b', 0,    0,    '1',  0,    '4',  '7',  0,    0,    0,
    '0',  '.',  '2',  '5',  '6',  '8',  '\033',0,   0,    '+',  '3',  '-',  '*',  '9',  0,    0,
    0,    0,    0,    247,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0};


#define BUFFER_SIZE 12
static volatile uint8_t buffer[BUFFER_SIZE];
static volatile uint8_t head, tail;
static volatile uint8_t teclaStatus = 0;

static volatile uint8_t SendByteState;
static volatile byte SendBits[10];
static volatile uint8_t disableisr;
static volatile uint8_t fulldisableisr;
static volatile uint16_t waitcounter;

bool scrollk = false;
bool future_scrollk = true;
bool cpslk = false;
bool future_cpslk = true;
bool numlk = false;
bool future_numlk = true;
volatile uint16_t shift=0;
volatile uint16_t modifs=0;
volatile uint8_t  ledflags=0;

//#pragma GCC push_options
//#pragma GCC optimize ("O2")



byte FoxPS2Keyboard::GetParity(byte b)
{
	byte count = 0;
	for (byte i = 0; i < 8; i++)
	{
		if (((b >> i) & 0x1) == 1)
			count++;
	}
	return ((count % 2) == 1 ? 0 : 1);
}

void FoxPS2Keyboard::SendByte(byte b)
{
	SendBits[0] = 1;

	for (byte j = 0; j < 8; j++)
		SendBits[j + 1] = ((b >> j) & 0x1) == 0x1 ? 0x0 : 0x1;

	SendBits[9] = GetParity(b) == 0x1 ? 0x0 : 0x1;

	SendByteState = 0;
	fulldisableisr = 1;
	digitalWrite(TransistorIRQPin, 1);
	delayMicroseconds(100);
	digitalWrite(TransistorDataPin, 1);
	delayMicroseconds(100);
	digitalWrite(TransistorIRQPin, 0);
	disableisr = 1;
	fulldisableisr = 0;

	while (disableisr != 0);

	delay(10);
}



bool FoxPS2Keyboard::SendByteWithConfirm(byte b, byte ACK, byte Tries, unsigned short Wait)
{
	unsigned long wait = millis();
	byte Test = 0;
	byte dat = 0;

	do
	{
		SendByte(b);
		while (1)
		{
			dat = GetScancode();
			if (dat == ACK)
				return(true);

			if (millis() - wait > Wait) //Sek
			{
				Test++;
				break;
			}
		}
	} while (Test < Tries);

	return(false);
}

void FoxPS2Keyboard::setNumLock(bool flag)
{
  if( flag ){
    ledflags |= NUM_LOCK;
  }else{
    ledflags &= !NUM_LOCK;
  }

}

bool FoxPS2Keyboard::SetKeyboardLights() //byte NumLock, byte CapsLock, byte ScrollLock)
{
	byte b = 0;
	if (ledflags & CAPS_LOCK)
		b |= 0x4;
	if (ledflags & NUM_LOCK)
		b |= 0x2;
	if (ledflags & SCROLL_LOCK)
		b |= 0x1;

	if (SendByteWithConfirm(0xED, 0xFA, 1, 500) == false) //Change Lights
		return(false);

	if (SendByteWithConfirm(b, 0xFA, 1, 500) == false) //Lights data
		return(false);

	return(true);
}

bool FoxPS2Keyboard::WaitForKeyboard()
{
	unsigned long wait = millis();
	byte Test = 0;

	if (SendByteWithConfirm(0xFF, 0xAA, 5, 2000) == false) //Reset Keyboard
		return(false);

	if (SendByteWithConfirm(0xEE, 0xEE, 1, 500) == false) //Echo Test (connectivity test)
		return(false);

	if (SendByteWithConfirm(0xF4, 0xFA, 1, 500) == false) //Enable Keyboard
		return(false);

	return(true);
}
//#pragma GCC pop_options

byte FoxPS2Keyboard::GetScancode()
{
	uint8_t c, i;

	i = tail;
	if (i == head) return 0;
	i++;

	if (i >= BUFFER_SIZE) i = 0;
	c = buffer[i];
	tail = i;
	return (c);
}

byte FoxPS2Keyboard::GetCharcode()
{
	uint8_t c, i;

	i = tail;
	if (i == head) return 0;
	i++;

	if (i >= BUFFER_SIZE) i = 0;
	c = buffer[i];
	tail = i;

  if (cpslk && (modifs&SHIFT) && (chrsNS[c] < 127) ){
    //Serial.println(" caps modifs ");
    return(chrsSH1[c]);
  }else if(modifs&SHIFT) {
    //Serial.print(" modifs ");
    return( chrsSH[c]);
  }else if (cpslk && chrsNS[c] < 127){
    //Serial.println(" caps ");
    return(toUpperCase(chrsNS[c]));
  }else {
    return(chrsNS[c]);
  }

	return (c);
}

void FoxPS2Keyboard::handleLeds()
{   
  if ( scrollk == future_scrollk ){
    if ( future_scrollk )
      ledflags |= SCROLL_LOCK;
    else  
      ledflags &= !SCROLL_LOCK;

    future_scrollk = !future_scrollk;
    SetKeyboardLights();
  }
  if ( cpslk == future_cpslk ){
    if ( future_cpslk )
      ledflags |= CAPS_LOCK;
    else  
      ledflags &= !CAPS_LOCK;

    future_cpslk = !future_cpslk;
    SetKeyboardLights();
  }
  if ( numlk == future_numlk ){
    if ( future_numlk )
      ledflags |= NUM_LOCK;
    else  
      ledflags &= !NUM_LOCK;

    future_numlk = !future_numlk;
    SetKeyboardLights();
  }
}

void KeyboardISR() //FALLING EDGE
{
	if (fulldisableisr != 0)
		return;

	if (disableisr != 0){
		SendByteState++;
		switch (SendByteState){
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			digitalWrite(TransistorDataPin, SendBits[SendByteState]);
			break;                              

		case 10:
			digitalWrite(TransistorDataPin, 0);
			break;
		case 11:
			waitcounter = 0;
			while (digitalRead(DataPin) != 0){
				waitcounter++;                             
				if (waitcounter > 32767){
					disableisr = 0;
					return;                             
				}
			};
			waitcounter = 0;
			while (digitalRead(DataPin) != 1){
				waitcounter++;
				if (waitcounter > 32767){
					disableisr = 0;
					return;
				}
			};
			disableisr = 0;
			break;
		}
		return;
	}
  
	static uint8_t bitcount = 0;
	static uint8_t incoming = 0;
	static uint32_t prev_ms = 0;
	uint32_t now_ms;
	uint8_t n, val;

	val = digitalRead(DataPin);
	now_ms = millis();
	if (now_ms - prev_ms > 250){
		bitcount = 0;
		incoming = 0;
	}
	prev_ms = now_ms;
	n = bitcount - 1;
	if (n <= 7){
		incoming |= (val << n);
	}
	bitcount++;
	if (bitcount == 11){
    if (incoming == 0x12)
      modifs |=SHIFT;
    if ( (teclaStatus == 1) || (incoming == 0xAA) || (incoming == 0xEE || (incoming == 0xFA)) ){
      switch(incoming){
        case 0x12:
          modifs &= !SHIFT;
          break;
        case 0x58:
          cpslk = !cpslk;
          break;
        case 0x77:
          numlk = !numlk;
          break;
        case 0x7E:
          scrollk = !scrollk;
          break;
        default:  
          uint8_t i = head + 1;
          if (i >= BUFFER_SIZE) i = 0;
          if (i != tail){
            buffer[i] = incoming;
            head = i;
          }
      } 
      //char fix[26];
      //sprintf(fix,"h=%d t=%d \n",head,tail);
      Serial.println(" ");
      teclaStatus = 0;
    }
    Serial.print(" [");    Serial.print(incoming, 16);    Serial.print("] ");
    switch(incoming){
      case 0xF0:
        teclaStatus = 1;
        break;
    }
		bitcount = 0;
		incoming = 0;
	}
}

FoxPS2Keyboard::FoxPS2Keyboard()
{
	pinMode(IRQPin, INPUT_PULLUP);
	pinMode(DataPin, INPUT_PULLUP);
	pinMode(TransistorDataPin, OUTPUT);
	pinMode(TransistorIRQPin, OUTPUT);

	digitalWrite(TransistorDataPin, 0);
	digitalWrite(TransistorIRQPin, 0);

	disableisr = 0;
	head = 0;
	tail = 0;
	fulldisableisr = 0;
  ledflags = 0;
  scrollk = false;
  future_scrollk = true;
  cpslk = false;
  future_cpslk = true;
  numlk = false;
  future_numlk = true;
	attachInterrupt(digitalPinToInterrupt(IRQPin), KeyboardISR, FALLING);
}
