/**
 * Open Hand Transmit Handler
 * This program is responsible for handling the transmit end of Open Hand.
 * The program should receive data from the force sensors and transmit them with the Xbee device.
 */

// Pin declarations
const int thumbPin = A3;
const int fingerPin = A2;

// Constant declarations
const int sampleSize = 5;
const int sampleRate = 10; // in milliseconds

// Public variables
int thumbForce = 0;
int fingerForce = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  // Sum repeated samples a finite amount of times to be averaged later
  int thumbSum = 0;
  int fingerSum = 0;
  for (int i = 0; i < sampleSize; i++) {
    thumbSum += analogRead(thumbPin);
    fingerSum += analogRead(fingerPin);
    delay(sampleRate);
  }

  // Calculate average thumb force over time
  thumbForce = thumbSum / sampleSize;
  fingerForce = fingerSum / sampleSize;

 /**
  * Send Packet
  * Packet conventions '< _ _ _ _ E _ _ _ _ >'
  * < - start of packet
  * First digit block - Thumb sensor value, can be from 1 to 4 digits
  * E - signal end of thumb value
  * Second digit block - Finger sensor value, can be from 1 to 4 digits
  * > - end of packet
  */
  Serial.print("<");
  Serial.print(thumbForce);
  Serial.print("E");
  Serial.print(fingerForce);
  Serial.print(">");
}
