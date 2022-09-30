/*
  SPDX-license-identifier: Apache-2.0

  Copyright (C) 2021 smartme.IO

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
  The library will use Microcontroller's built in method to retrieve his unique ID. This will be 0 padded until given size is met.
  Keep in mind that when calling the string method, the buffer should be at least double+1 of ID size. If a size will not provided as argument,
  only the bare minimum string will be returned.
*/

#include <MicrocontrollerID.h>

void setup() {
  Serial.begin(115200);
  while (!Serial);

  uint8_t uniqueID [16];
  MicroID.getUniqueID(uniqueID, 16);

  Serial.print("Device ID (bytes): ");
  for (int i = 0; i < 16; i++) {
    Serial.print(uniqueID[i], HEX);
  }
  Serial.println();
  
  char id [41];
  MicroID.getUniqueIDString(id);
  Serial.print("Device ID (string): ");
  Serial.println(id);

  MicroID.getUniqueIDString(id, 20);
  Serial.print("Device ID (fixed-length string): ");
  Serial.println(id);
}

void loop() {
}
