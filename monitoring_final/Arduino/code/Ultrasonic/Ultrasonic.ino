#define VCC 2
#define TRIG 3
#define ECHO 4
#define GND 5
#define WAIT_TIME 500

void setup() {
  pinMode (VCC,OUTPUT);//attach pin 2 to vcc
  digitalWrite(VCC, HIGH);
  pinMode (TRIG, OUTPUT);// attach pin 3 to Trig
  pinMode (ECHO, INPUT);//attach pin 4 to Echo  
  pinMode (GND,OUTPUT);//attach pin 5 to GND
  // initialize serial communication:
  Serial.begin(9600);
}

void loop()
{
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(5);
  digitalWrite(TRIG, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration = pulseIn(ECHO, HIGH);

  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);
  //The resolution of the sensor is from 0 to 4 m 
  if(cm>=400||cm<=2){//Out of range value
    Serial.println("Out of range");
  }
  else{///Real measure
    Serial.print(cm);
    Serial.println("cm");
  }

  //Distance between measures 
  delay(WAIT_TIME);
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
