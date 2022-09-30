# Microcontroller-Id Arduino Library by smartme.IO


The Microcontroller-Id library allows to retrieve the manufacturer's serial number from the microprocessor.

## Credits

This library is based on [ArduinoUniqueID](https://github.com/ricaun/ArduinoUniqueID) by Luiz Henrique Cassettari (https://github.com/ricaun).

## Supported devices

Currently the following devices are supported:

 - Atmel AVR
 - Atmel SAM
 - Atmel SAMD
 - STM32
 - Espressif ESP
 - RP2040 (Raspberry Pi Pico)
 - nRF52

## Examples and usage

The library already comes with an example showing how to properly use the provided methods. The Id can be either retieved as a uint8_t array by calling the MicroID.getUniqueID function
```c
uint8_t  uniqueID [16];
MicroID.getUniqueID(uniqueID, 16);
```

or as a string via the MicroID.getUniqueIDString function. In both cases the Id will be zero padded until the given size is met however, if a size is not provided in the getUniqueIDString method, only the bare minimum Id will be returned.
```c
char  id [50];
MicroID.getUniqueIDString(id);
//This one will be padded until 20 bytes reached
MicroID.getUniqueIDString(id, 20); 
```
