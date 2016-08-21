//motor driver pins
int pwm1= 10;// M1 - Top right
int dir1= 13;
int pwm2= 11;// M2 - Top Left
int dir2= 12;
int pwm3= 9;// M3 - Bottom Right
int dir3= 8;
int pwm4= 6;// M4 - Bottom Left
int dir4= 7;

//cytron pins
const byte rx = 0;    // Defining pin 0 as Rx
const byte tx = 1;    // Defining pin 1 as Tx
const byte serialEn = 2;    // Connect UART output enable of LSA08 to pin 2
const byte junctionPulse = 4;   // Connect JPULSE of LSA08 to pin 4 

int count =0;
unsigned int junctionCount = 0;   // Variable to store junction count value

byte dummy = 0;   // Declare a dummy variable to store incoming data
byte ma=0;

void forward()
{
   Serial.println("forward!!!");
  digitalWrite(dir1,LOW);
  digitalWrite(dir2,LOW);
  digitalWrite(dir3,HIGH);
  digitalWrite(dir4,HIGH);
    
  analogWrite(pwm1,40);
  analogWrite(pwm2,40);
  analogWrite(pwm3,40);
  analogWrite(pwm4,40);
}

void reverse()
{
  digitalWrite(dir1,HIGH);
  digitalWrite(dir2,HIGH);
  digitalWrite(dir3,LOW);
  digitalWrite(dir4,LOW);
    
  analogWrite(pwm1,70);
  analogWrite(pwm2,70);
  analogWrite(pwm3,70);
  analogWrite(pwm4,70);
}

void newright()
{ 
  digitalWrite(dir1,LOW);
  digitalWrite(dir2,LOW);
  digitalWrite(dir3,LOW);
  digitalWrite(dir4,LOW);
    
  analogWrite(pwm1,50);
  analogWrite(pwm2,50);
  analogWrite(pwm3,50);
  analogWrite(pwm4,50);
 
  delay(50);
  
  digitalWrite(serialEn,LOW);   // Set serialEN to LOW to request UART data
  while(Serial.available() <= 0);   // Wait for data to be available
  dummy = Serial.read();    // Read incoming data and store in dummy
  digitalWrite(serialEn,HIGH);    // Stop requesting for UART data  
Serial.println(dummy);

  if(bitRead(dummy,3))
  {
    brake();
  }
  else
  {
    newright();
    }
}
void brake(){
  
  analogWrite(pwm1,0);
  analogWrite(pwm2,0);
  analogWrite(pwm3,0);
  analogWrite(pwm4,0);
  
}


void newleft()
{
   Serial.println("taking left"); 
   
  digitalWrite(dir1,HIGH);
  digitalWrite(dir2,HIGH);
  digitalWrite(dir3,HIGH);
  digitalWrite(dir4,HIGH);
    
  analogWrite(pwm1,30);
  analogWrite(pwm2,30);
  analogWrite(pwm3,30);
  analogWrite(pwm4,30);

  delay(50);
  
  digitalWrite(serialEn,LOW);   // Set serialEN to LOW to request UART data
  while(Serial.available() <= 0);   // Wait for data to be available
  dummy = Serial.read();    // Read incoming data and store in dummy
  digitalWrite(serialEn,HIGH);    // Stop requesting for UART data
  
  if(bitRead(dummy,3))
  {
    brake();
  }
   else
  {
    newleft();
    }
}

/*void readdata(){
Serial.println("reding dataatatatta");


digitalWrite(dir1,HIGH);
  digitalWrite(dir2,HIGH);
  digitalWrite(dir3,HIGH);
  digitalWrite(dir4,HIGH);
    
  analogWrite(pwm1,40);
  analogWrite(pwm2,40);
  analogWrite(pwm3,40);
  analogWrite(pwm4,40);

digitalWrite(serialEn,LOW);   // Set serialEN to LOW to request UART data
  while(Serial.available() <= 0);   // Wait for data to be available
  ma = Serial.read();    // Read incoming data and store in dummy
  digitalWrite(serialEn,HIGH);    // Stop requesting for UART data

Serial.print("this is what i read: ");
Serial.println(ma);

if(bitRead(ma,3)){
  forward();
}
else{
  readdata();
}
  
}
*/
void junctionnewleft()
{

  forward();
  delay(1000);
  Serial.println("I tell you junction!!!!");
  
  digitalWrite(dir1,HIGH);
  digitalWrite(dir2,HIGH);
  digitalWrite(dir3,HIGH);
  digitalWrite(dir4,HIGH);
    
  analogWrite(pwm1,50);
  analogWrite(pwm2,50);
  analogWrite(pwm3,50);
  analogWrite(pwm4,50);

  delay(3250);
  
  Serial.println("I set it to left bithches!!!!!!");
  }


void setup() {
  
  pinMode(serialEn,OUTPUT);   // Setting serialEn as digital output pin
  pinMode(junctionPulse,INPUT);   // Setting  junctionPulse as digital input pin
  // Setting initial condition of serialEn pin to HIGH
  digitalWrite(serialEn,HIGH);
  // Begin serial communication with baudrate 9600
  Serial.begin(9600);
  // Clear internal junction counter of LSA08
  clearJunction();
  
  pinMode(pwm1,OUTPUT);
  pinMode(dir1,OUTPUT);
  pinMode(pwm2,OUTPUT);
  pinMode(dir2,OUTPUT);
  pinMode(pwm3,OUTPUT);
  pinMode(dir3,OUTPUT);
  pinMode(pwm4,OUTPUT);
  pinMode(dir4,OUTPUT);
}


void loop() {
  // put your main code here, to run repeatedly:

// Checking for junction crossing
int a=digitalRead(junctionPulse);
count=count+a;
delay(10);
  digitalWrite(serialEn,LOW);   // Set serialEN to LOW to request UART data
  while(Serial.available() <= 0);   // Wait for data to be available
  dummy = Serial.read();    // Read incoming data and store in dummy
  digitalWrite(serialEn,HIGH);    // Stop requesting for UART data

if(a==1){
  
Serial.println("junction!!");
junctionnewleft();
  delay(10);
}
else{
   // Checking for sensor number 1 and 2, if line detected, move left
  if(bitRead(dummy,0)||bitRead(dummy,1))
       {
        newleft();
        delay(500);
        }
        
  // Checking for sensor number 5 and 6, if line detected, move right
  else if(bitRead(dummy,3) || bitRead(dummy,4))
      {
        Serial.println("forward"); 
        forward();
       delay(500);
        }
        
  else if(bitRead(dummy,5)||bitRead(dummy,6) || bitRead(dummy,7))
 // moveRight();
      {
        Serial.println("Taking right");
       newright();
       delay(500);
      }
      
  // If no line is detected, stay at the position
  else
  //wait();
        {
          Serial.println("brake");
          brake();
          } 
}

}

// Function to clear internal junction counter of LSA08
void clearJunction() {
  char address = 0x01;
  char command = 'X';
  char data = 0x00;
  char checksum = address + command + data;

  Serial.write(address);
  Serial.write(command);
  Serial.write(data);
  Serial.write(checksum);
}

// Function to retrieve junction count from LSA08
int getJunction() {
  char address = 0x01;
  char command = 'X';
  char data = 0x01;
  char checksum = address + command + data;

  Serial.write(address);
  Serial.write(command);
  Serial.write(data);
  Serial.write(checksum);

  while(Serial.available() <= 0);
  return (int(Serial.read()));
}
