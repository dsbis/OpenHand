/**
 * OpenHand Receive Handler
 * This program is responsible for handling the receiving end of OpenHand.
 * The program should receive sensor values from OpenHand Transmit through the XBee device
 *    and adjust the motors' amplitude accordingly.
 */

// Pin declarations
const int frontMotorPin = 9;
const int backMotorPin = 11;

// Public variables
bool started = false;
bool ended = false;
bool isFinger = false;
char incomingByte;
char thumbMsg[4];
char fingerMsg[4];
byte thumbIndex;
byte fingerIndex;

void setup() { 
  pinMode(frontMotorPin, OUTPUT);
  pinMode(backMotorPin, OUTPUT);
  Serial.begin(9600);
} 
 
 
void loop() { 
    
    while(Serial.available() > 0) {
      incomingByte = Serial.read();
//      Serial.print(incomingByte);

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

        // Set the motor speeds
        setMotor(backMotorPin, thumbValue);
        setMotor(frontMotorPin, fingerValue);

        // Reset the packet flags
        started = false;
        ended = false;
        thumbIndex = 0;
        fingerIndex = 0;
        thumbMsg[thumbIndex] = '\0';
        fingerMsg[fingerIndex] = '\0';
      }
  }
} 

/**
 * Set the motor speeds
 * pin - motor pin
 * rawVal - raw sensor value assigned to motor
 */
void setMotor(int pin, int rawVal) {
  int newVal = 100;


  
  analogWrite(pin, newVal);
}
