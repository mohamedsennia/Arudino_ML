
/*
  IMU Capture

  This example uses the on-board IMU to start reading acceleration and gyroscope
  data from on-board IMU and prints it to the Serial Monitor for one second
  when the significant motion is detected.

  You can also use the Serial Plotter to graph the data.

  The circuit:
  - Arduino Nano 33 BLE or Arduino Nano 33 BLE Sense board.

  Created by Don Coleman, Sandeep Mistry
  Modified by Dominic Pajak, Sandeep Mistry

  This example code is in the public domain.
*/
#include <stdio.h>
#include <stdlib.h>
#include <Arduino_LSM9DS1.h>
byte bytesToSend[sizeof(float)];
  const float accelerationThreshold = 2.5; // threshold of significant in G's
const int numSamples = 119;
float tableau[714];
int z=0;
int samplesRead = numSamples;

void setup() {
  Serial.begin(9600);
   Serial1.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // print the header
  Serial.println("aX,aY,aZ,gX,gY,gZ");
}

void loop() {
  float aX, aY, aZ, gX, gY, gZ;
  Serial.println("b");

  // wait for significant motion
  while (samplesRead == numSamples) {
    if (IMU.accelerationAvailable()) {
      // read the acceleration data
      IMU.readAcceleration(aX, aY, aZ);

      // sum up the absolutes
      float aSum = fabs(aX) + fabs(aY) + fabs(aZ);

      // check if it's above the threshold
      if (aSum >= accelerationThreshold) {
        // reset the sample read count
        samplesRead = 0;
        break;
      }
    }
  }

  // check if the all the required samples have been read since
  // the last time the significant motion was detected
  while (samplesRead < numSamples) {
    // check if both new acceleration and gyroscope data is
    // available
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
      // read the acceleration and gyroscope data
      IMU.readAcceleration(aX, aY, aZ);
      IMU.readGyroscope(gX, gY, gZ);

      samplesRead++;

      // print the data in CSV format
      //Serial.print(aX, 3);
      tableau[z]=aX;
      z=z+1;
     // Serial.print(',');
     // Serial.print(aY, 3);
      tableau[z]=aY;
      z=z+1;
      //Serial.print(',');
      //Serial.print(aZ, 3);
      tableau[z]=aZ;
      z=z+1;
     // Serial.print(',');
    //  Serial.print(gX, 3);
      tableau[z]=gX;
      z=z+1;
     // Serial.print(',');
     // Serial.print(gY, 3);
      tableau[z]=gY;
      z=z+1;
     // Serial.print(',');
     // Serial.print(gZ, 3);
      tableau[z]=gZ;
      z=z+1;
    //  Serial.println();
      //Serial.println("//");
      
     
 

      if (samplesRead == numSamples) {
        // add an empty line if it's the last sample
        int m;
        for(m=0;m<714;m++){
          char s[6];
          sprintf(s, "%f",tableau[m]);
          int i;
          Serial.println(s);
          
          for(i=0;i<6;i++){
            Serial1.print(s[i]);
            }
          }
          Serial1.println('f');
        Serial.println();
        Serial.println("------------------");
        
      }
    }
  }
  Serial.println("a");
}
