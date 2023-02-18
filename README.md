# FOK-GYEM
Unofficial FOK-GYEM screen driver for Arduino and Arduino-like devices. Requires a special PCB controller module.

## PCB
Version 1 of the PCB design is available as a KiCad Project and Gerber zip on [mega.nz](https://mega.nz/file/3mAA2AxQ#uEkXPkIazQjaFEwh2zbjOequC6g8-KIUfY65_SY9eXo)

> The PCB design is currently being revised due to limited availability and high price of the used components.

## Usage - Software

> :warning: **SERIAL COMMUNICATION BRANCH**: This branch contains code mainly for serial communication based operation! Use branch `static/master` for static (self-opeating) mode.

> :information_source: See `main.cpp` for working example code.

> :information_source: Full protocol description: [FOK-GYEM_Ultimate-Serial/ProtocolDescription.md](https://github.com/zsotroav/FOK-GYEM_Ultimate-Serial/blob/main/ProtocolDescription.md)

### void setup()
Where your code is executed

### void loop()
After setup, the program automatically enters this function and loops it indefinitely.

### For more info see the comments and briefs in the code.

## Usage - Physical devices
1. Connect screen cables to FOK-GYEM
   - Ensure that they are connected in order and that the jumpers are in the correct location on the modules
1. Connect Arduino 5V 
2. Upload the image to Arduino
3. Connect 12-15V DC to the controller

## Support
Join the [Discord support server](https://discord.gg/dMrwaAkEcv) (English or Hungarian)
