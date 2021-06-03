#include <Servo.h>
Servo servo_1;
Servo servo_2;

//mg996r initalizations
int servo_position=90;
int servo_1_pin=9;
int servo_2_pin=10;
int mapped_position;
int b;


int JoystickRight_var;
void setup() {

//  Serial.begin(9600);
  //mg996r setup
  servo_1.attach(servo_1_pin);
  servo_2.attach(servo_2_pin);
 
}
void motion_right(int JoystickRight_var)
{
  ///check this code
  Serial.print(JoystickRight_var);
  mapped_position=map(JoystickRight_var,0,1024,90,180);
  servo_1.write(mapped_position);
  servo_2.write(mapped_position); 
}
void motion_left(int JoystickRight_var)
{
  ///check this code
//  Serial.print(JoystickRight_var);
  b=abs(JoystickRight_var);
//  Serial.print("---------");
//  Serial.print(b);
  mapped_position=map(b,0,1023,90,0);
//  Serial.print("---------");
//  Serial.print(mapped_position);
  servo_1.write(mapped_position);
  servo_2.write(mapped_position);  
}

void servo_motion_stop()
{
    servo_1.write(servo_position);
  servo_2.write(servo_position);

}
void loop() {
//servo_1.write(0);
//servo_2.write(0);
//delay(1000);
for(JoystickRight_var=-1023;JoystickRight_var<1023;JoystickRight_var+=30)
{
    if(JoystickRight_var>3)
    {
      motion_right(JoystickRight_var);
      delay(50);
//      Serial.println("motion right");
    }
    else if(JoystickRight_var<-3)
    {
      motion_left(JoystickRight_var);
      delay(50);
//      Serial.println("motion left");
    }
    else 
    {
      servo_motion_stop();
      delay(50);
//      Serial.println("servo motion stop");
    }
}
for(JoystickRight_var=1023;JoystickRight_var>-1023;JoystickRight_var-=30)
{
    if(JoystickRight_var>3)
    {
      motion_right(JoystickRight_var);
      delay(50);
//      Serial.println("motion right");
    }
    else if(JoystickRight_var<-3)
    {
      motion_left(JoystickRight_var);
      delay(50);
//      Serial.println("motion left");
    }
    else 
    {
      servo_motion_stop();
      delay(50);
//      Serial.println("servo motion stop");
    }
}

}
