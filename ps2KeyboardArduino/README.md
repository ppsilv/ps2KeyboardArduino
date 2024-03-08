# Arduino PS/2 Keyboard Library

A small Library to allow bi-directional communication with any AT-Keyboard (includes keyboards with a DIN-Connector and PS/2 Connectors). XT keyboards will not work.  
With this library, you can play with LEDs on the keyboard.  
The PDF contians the schematic how to connect the Keyboard to the Arduino. You also need 2 NPN-Transistors.  
  
Note: this library does not convert any scancodes.  
  
to connect to a PS/2 or DIN Connector, use this Pinout diagram:  
[PS/2 Connector](https://pinouts.ru/InputCables/KeyboardPC6_pinout.shtml)  
[DIN Connector](https://old.pinouts.ru/InputCables/KeyboardPC5_pinout.shtml)  

### Prerequisites

* Arduino Leonardo (for others, adapatation in the source may needed)
* Arduino compiler

### Note

All these tools are provided as-is from [Vulpes](https://vulpes.lu).
If you've some questions, contact me [here](https://go.vulpes.lu/contact).
