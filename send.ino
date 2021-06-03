#include <Servo.h>
#include <Wire.h>

Servo servo_1;
Servo servo_2;

//mg996r servo motor initalizations
int servo_position=90;
int servo_1_pin=9;
int servo_2_pin=10;
int mapped_position;
int b;

//l298n motor driver initializations
int enA = 6;
int in1 = 4;
int in2 = 3;
int enB = 5;
int in3 = 1;
int in4 = 0;
int mapped_value;
int a;


//nrf24l01 variables initializations
int JoystickLeft_var=0;               //these variables vary from -1023 to 1023
int JoystickRight_var=10;           

void setup()
{

  Serial.begin(9600);
  //mg996r setup
  servo_1.attach(servo_1_pin);
  servo_2.attach(servo_2_pin);
  
  // l298 setup
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void motion_front(int JoystickLeft_var)
{
  // enabling pins 
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  // printing the values in serial monitor
  Serial.print("front raw= ");
  Serial.print(JoystickLeft_var); 
  mapped_value=map(JoystickLeft_var,0,1023,0,255);                  // changing values from (0,1023) to (0,255)
  Serial.print("front= ");
  Serial.print(mapped_value);
  
  //setting the speed of motors based on mapped values as motor max input is 255 
  analogWrite(enA, mapped_value); 
  analogWrite(enB, mapped_value); 
  delay(20); 

}

void motion_back(int JoystickLeft_var)
{
    // enabling pins but in opposite direction
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
   
  Serial.print("back raw= ");
  Serial.print(JoystickLeft_var);  
  a=abs(JoystickLeft_var);                          // to get positive values
  Serial.print("back abs raw= ");
  Serial.print(JoystickLeft_var);
  mapped_value=map(a,0,1023,0,255);                      //changing values from (0,1023) to (0,255)
  
  Serial.print("back= ");
  Serial.print(mapped_value);    

    //setting the speed of motors based on mapped values as motor max input is 255 
  analogWrite(enA, mapped_value); 
  analogWrite(enB, mapped_value); 
  delay(20); 
}

void motor_motion_stop()
{
  // now turn off motors
 digitalWrite(in1, LOW);
 digitalWrite(in2, LOW);  
 digitalWrite(in3, LOW);
 digitalWrite(in4, LOW); 
}

void motion_right(int JoystickRight_var)
{
  mapped_position=map(JoystickRight_var,0,1024,90,180);        // mapping as i need servo position to be in 90,180 degrees        
  servo_1.write(mapped_position);
  servo_2.write(mapped_position); 
  delay(50);

}
void motion_left(int JoystickRight_var)
{
  ///check this code
  b=abs(JoystickRight_var);                               // to get positive values
  mapped_position=map(b,0,1023,90,0);                     // mapping to get servo position to be in 90 to 0 degrees
  servo_1.write(mapped_position);
  servo_2.write(mapped_position);  
}

void servo_motion_stop()
{
  // reset servo position back to normal i.e 90 degrees
    servo_1.write(servo_position);
  servo_2.write(servo_position);

}
void loop()
{  


    if(JoystickRight_var>3)                       // ignore values from -3 to 3 for both JoystickRight_var and JoystickLeft_var
    {
      motion_right(JoystickRight_var);
      delay(100);
      Serial.println("motion right");
    }
    else if(JoystickRight_var<-3)
    {
      motion_left(JoystickRight_var);
      delay(100);
      Serial.println("motion left");
    }
    else 
    {
      servo_motion_stop();
      delay(100);
      Serial.println("servo motion stop");
    }


    if(JoystickLeft_var>3)
    {
      motion_front(JoystickLeft_var);
      delay(1000);
      Serial.println("motion front");
    }
    else if(JoystickLeft_var<-3)
    {
      motion_back(JoystickLeft_var);
      delay(100);
      Serial.println("motion back");
    }
    else 
    {
      motor_motion_stop();
      Serial.println("motor motion stop");
    }
}
