# FOK-GYEM
Unofficial FOK-GYEM screen driver for Arduino and Arduino-like devices. Requires a special PCB controller module.

## PCB
Version 1 of the PCB design is available as a KiCad Project and Gerber zip on [mega.nz](https://mega.nz/file/3mAA2AxQ#uEkXPkIazQjaFEwh2zbjOequC6g8-KIUfY65_SY9eXo)

> The PCB design is currently being revised due to limited availability and high price of the used components.

## Usage - Software

1. Set CFG_PANEL_COUNT and other settings in `config.h`
2. Call `driver_init();` in `void setup()` in `main.cpp`
3. Write data to the screen using:
```cpp
uint8_t buff[DRV_DATABUFF_SIZE];
driver_setBuffer(buff, DRV_DATABUFF_SIZE);
driver_writeScreen();
```

### void setup()
Where your code is executed

### void loop()
After setup, the program automatically enters this function and loops it indefinitely.

### For more info see the comments in the code.

## Usage - Physical devices
1. Connect screen cables to FOK-GYEM
   - Ensure that they are connected in order and that the jumpers are in the correct location on the modules
1. Connect Arduino 5V 
2. Upload the image to Arduino
3. Connect 12-15V DC to the controller

## Support
Join the [Discord support server](https://discord.gg/D7EgcpuUdG) (English or Hungarian)
