/*
   Copyright (c) 2015 Intel Corporation.  All rights reserved.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/

/*
   This sketch example demonstrates how the BMI160 on the
   Intel(R) Curie(TM) module can be used to read accelerometer data
*/

#include "CurieIMU.h"
#include "bleaccel.h"
void setup() 
{
  Serial.begin(9600); // initialize Serial communication
  while (!Serial);    // wait for the serial port to open

  // initialize device
  Serial.println("Initializing IMU device...");
  CurieIMU.begin();
  
  initAccelBLE();
}

void loop() 
{
  int axRaw, ayRaw, azRaw;         // raw accelerometer values
  float ax, ay, az;
   
  if( getCongfig() && isConnected())
  {
     
      CurieIMU.setAccelerometerRange(getCongfig());
      
      CurieIMU.readAccelerometer(axRaw, ayRaw, azRaw);
      // convert the raw accelerometer data to G's
      ax = convertRawAcceleration(axRaw);
      ay = convertRawAcceleration(ayRaw);
      az = convertRawAcceleration(azRaw);
     
      Serial.print("a:\t");
      Serial.print(axRaw);
      Serial.print("\t");
      Serial.print(ayRaw);
      Serial.print("\t");
      Serial.print(azRaw);
      Serial.println();
      // Send to BLE 
      setAccelData(axRaw, ayRaw, azRaw);
      
      /*
      Serial.print("a:\t");
      Serial.print(ax);
      Serial.print("\t");
      Serial.print(ay);
      Serial.print("\t");
      Serial.print(az);
      Serial.println();
      */
      delay(getPeriod());
  }
}

float convertRawAcceleration(int aRaw) {
  // since we are using 2G range
  // -2g maps to a raw value of -32768
  // +2g maps to a raw value of 32767
  
  float a = (aRaw * 2.0) / 32768.0;

  return a;
}



