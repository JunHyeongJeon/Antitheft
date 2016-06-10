#include <SoftwareSerial.h>

// init value 
SoftwareSerial BTSerial(8, 9);
int led_red = 2;  // red led 
int led_white = 3; // white led 

int button_start = A1; // start button
int sensor_p = A0; // presure sensor
int gyro_x = A3; // gyro sensor input
int gyro_y = A4; // gyro sensor input

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
  while(1)  {
    // if status is standby red led is on
    digitalWrite(led_red,HIGH);
    digitalWrite(led_white,LOW);
    
    val_w = analogRead(sensor_p);
    val_x = analogRead(gyro_x);
    val_y = analogRead(gyro_y);
 
    delay(100);
    
    Serial.print("w:");
    Serial.print(val_w);
    Serial.print("x:");
    Serial.print(val_x);
    Serial.print("y:");
    Serial.println(val_y);
    
    if(  BTSerial.read() == 'B')
      break; // 블루투스로 'B'문자 받을 경우 루프 탈출
  }
}

void Standby_Mode()
{
  // if status is standby white led is on
  // BTSerial.println("STAND_BY");
  digitalWrite(led_white,HIGH);
  // delay(100);
  digitalWrite(led_red,LOW);
  // delay(100);  
}

bool isButtonClick(){
  return analogRead(button_start) <= 512;
}

bool isAndroidButtonClick(){
  if ( BTSerial.read() == 'S' )
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

