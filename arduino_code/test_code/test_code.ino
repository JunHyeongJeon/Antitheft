#include <SoftwareSerial.h>

// init value 
SoftwareSerial BTSerial(8, 9);
const int led_red = 2;  // red led 
const int led_white = 3; // white led 
const int sona = 7;

const int button_start = A1; // start button
const int sensor_p = A0; // presure sensor
const int gyro_x = A3; // gyro sensor input
const int gyro_y = A4; // gyro sensor input

int val_w=0;
int val_x=0;
int val_y=0;

char A,B;

void setup() 
{
  // init serial 
  Serial.begin(9600);
  BTSerial.begin(9600);
  
  // init IO
  pinMode(button_start,INPUT);
  pinMode(led_red,OUTPUT);
  pinMode(led_white,OUTPUT);
  pinMode(sona, OUTPUT);

  
  // print serial 
  Serial.println("PROGRAM START");
  BTSerial.println("PROGRAM START");
  
  // set IO
  digitalWrite(led_white,HIGH); 
  digitalWrite(led_red,LOW);
}

void loop(){

  if( isActive() ){
    Detecting_Mode();
  }
  else{
    Standby_Mode();
  }
  

}

void Detecting_Mode(){
  long duration, sona_cm;

  while(1)  {
//    BTSerial.print('A');
    // if status is standby red led is on
    digitalWrite(led_red,HIGH);
    digitalWrite(led_white,LOW);
    
    val_w = analogRead(sensor_p);
    val_x = analogRead(gyro_x);
    val_y = analogRead(gyro_y);
 
    // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    pinMode(sona, OUTPUT);
    digitalWrite(sona, LOW);
    delayMicroseconds(2);
    digitalWrite(sona, HIGH);
    delayMicroseconds(5);
    digitalWrite(sona, LOW);
  
    // The same pin is used to read the signal from the PING))): a HIGH
    // pulse whose duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
    pinMode(sona, INPUT);
    duration = pulseIn(sona, HIGH);
  
    // convert the time into a distance
    sona_cm = microsecondsToCentimeters(duration);
  

    
    
    delay(40);
    
    if( isTilt ( val_x, val_y ) ){
        BTSerial.println('T');
        Serial.println('T');
      
      }
    else if (isMoved(sona_cm)){
        BTSerial.println('T');
        Serial.println('T');
      
    }
    else{ 
        BTSerial.println('A');
        Serial.println('A');
      
    }
    if(  BTSerial.read() == 'B')
      break; // 블루투스로 'B'문자 받을 경우 루프 탈출
  }
}

void Standby_Mode()
{
  // if status is standby white led is on
  BTSerial.print("S");
  digitalWrite(led_white,HIGH);
  digitalWrite(led_red,LOW);
  delay(100);  
}

bool isButtonClick(){
  return analogRead(button_start) <= 512;
}

bool isAndroidButtonClick(){
  if ( BTSerial.read() == 'B' )
    return true;
  else 
    return false;
}

bool isActive(){
  if( isButtonClick() || isAndroidButtonClick() ) 
    return true;
    
  else 
    return false;
}

bool isTilt(int x, int y){
  if ((x > 390 || x < 310) ||(y > 390 || y < 310))
    return true;
    
  else 
    return false;
}

bool isMoved(int val){
  Serial.println(val);
  if (val > 10)
    return true;
  else
    return false;
}


long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
