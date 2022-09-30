/*
SPDX-license-identifier: Apache-2.0

Copyright (C) 2021 SmartMe.IO

Authors:  Marco Calapristi <marco.calapristi@smartme.io>

Licensed under the Apache License, Version 2.0 (the "License"); you may
not use this file except in compliance with the License. You may obtain
a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
License for the specific language governing permissions and limitations
under the License
*/

/*
This library can be considered as a reshaped version of the ArduinoUniqueID library by ricaun:
https://github.com/ricaun/ArduinoUniqueID
*/

#ifndef MICROCONTROLLERID_H_
#define MICROCONTROLLERID_H_

#ifndef ARDUINO_H_
#define ARDUINO_H_
#include <Arduino.h>
#endif

#if defined(ARDUINO_ARCH_RP2040)
  #if defined(ARDUINO_ARCH_MBED)
    extern "C" {
    #include "hardware/flash.h"
    #include "pico/bootrom.h"
    }
  #else
    #include <pico/unique_id.h>
  #endif
#endif

#if defined(ARDUINO_ARCH_AVR)
#include <avr/boot.h>
#ifndef SIGRD
#define SIGRD 5
#endif
#endif

#define MAX_BUFFER_SIZE 16

#if defined (ARDUINO_ARCH_RP2040)
#define IDSIZE 8

#elif defined (ARDUINO_ARCH_AVR)
#if defined (__AVR_ATmega328PB__)
#define IDSIZE 10
#else
#define IDSIZE 9
#endif

#elif defined (ARDUINO_ARCH_ESP8266)
#define IDSIZE 8

#elif defined (ARDUINO_ARCH_ESP32)
#define IDSIZE 8

#elif defined (ARDUINO_ARCH_SAM)
#define IDSIZE 16

#elif defined (ARDUINO_ARCH_SAMD)
#define IDSIZE 16

#elif defined(ARDUINO_ARCH_STM32)
#define IDSIZE 12
#elif defined(ARDUINO_ARCH_NRF52)
#define IDSIZE 8
#endif




class MicrocontrollerID {
    public:
    void getUniqueID(uint8_t* id_out, int size);
    void getUniqueIDString(char* id_out, int size=0);

    private:
    void _zeroPad(uint8_t* buf, int start, int stop);
    void _array_to_string(byte array[], unsigned int len, char buffer[]);
};

extern MicrocontrollerID MicroID;

#endif /* MICROCONTROLLERID_H_ */
