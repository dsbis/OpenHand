/**
 * Open Hand Receive Handler
 * This program is responsible for handling the receiving end of Open Hand.
 * The program should receive sensor values from Open Hand Transmit through the XBee device
 *    and adjust the motors' amplitude accordingly.
 */

// Pin declarations
const int frontMotorPin = A3;
const int backMotorPin = A0;

// Constant declarations
const int onTolerance = -60;
const int offTolerance = 40;

// Public variables
bool started = false;
bool ended = false;
bool isFinger = false;
char incomingByte;
char thumbMsg[4];
char fingerMsg[4];
byte thumbIndex;
byte fingerIndex;
int lastThumbValue;
int lastFingerValue;

void setup() { 
  pinMode(frontMotorPin, OUTPUT);
  pinMode(backMotorPin, OUTPUT);
  Serial.begin(9600);
} 
 
 
void loop() { 
//    Serial.println("waiting on data"); // For Debug Purposes
    while(Serial.available() > 0) {
      incomingByte = Serial.read();

     /**
      * Parse Packet
      * Packet conventions '< _ _ _ _ E _ _ _ _ >'
      * < - start of packet
      * First digit block - Thumb sensor value, can be from 1 to 4 digits
      * E - signal end of thumb value
      * Second digit block - Finger sensor value, can be from 1 to 4 digits
      * > - end of packet
      */
      if(incomingByte == '<') {
        started = true;
      } else if (incomingByte == '>') {
        ended = true;
        break;
      } else if (incomingByte == 'E') {
        isFinger = true;
      } else {
        if (isFinger) {
          if(fingerIndex < 5)
           {
             fingerMsg[fingerIndex] = incomingByte;
             fingerIndex++;
             fingerMsg[fingerIndex] = '\0';
           }
        } else {
          if(thumbIndex < 5)
           {
             thumbMsg[thumbIndex] = incomingByte;
             thumbIndex++;
             thumbMsg[thumbIndex] = '\0';
           }
        }
        
      }

      // End of packet routine
      if (started && ended) {
        
        // translate sensor values
        int thumbValue = atoi(thumbMsg);
        int fingerValue = atoi(fingerMsg);

        Serial.print(thumbValue);
        Serial.print(",");
        Serial.println(fingerValue);
        
        // Set the motor speeds
        setMotor(backMotorPin, thumbValue, isFinger);
        setMotor(frontMotorPin, fingerValue, isFinger);

        // Reset the packet flags and variables
        started = false;
        ended = false;
        isFinger = false;
        thumbIndex = 0;
        fingerIndex = 0;
        thumbMsg[thumbIndex] = '\0';
        fingerMsg[fingerIndex] = '\0';
        lastThumbValue = thumbValue;
        lastFingerValue = fingerValue;
      }
  }
} 

/**
 * Set the motor speeds
 * pin - motor pin
 * rawVal - raw sensor value assigned to motor
 */
void setMotor(int pin, int rawValue, bool isFinger) {
  int newVal;
  int lastValue;
  
  if (isFinger) {
    lastValue = lastFingerValue;
  } else {
    lastValue = lastThumbValue;
  }

  int difference = rawValue - lastValue;
  if (difference >= offTolerance) {
    // Motor OFF
    analogWrite(pin, 0);
  } else if (difference <= onTolerance && rawValue <= 1024 && rawValue != 0) {
    // Motor ON
    analogWrite(pin, 140);
  }
}
