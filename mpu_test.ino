#include <Servo.h>
#include <Wire.h>

Servo servo_1;
Servo servo_2;

// mpu6050 accelerometer gyroscope 
int gyro_x, gyro_y, gyro_z;
long acc_x, acc_y, acc_z, acc_total_vector;
int temperature;
long gyro_x_cal, gyro_y_cal, gyro_z_cal;
//long loop_timer;
//int lcd_loop_counter;
float angle_pitch, angle_roll;
int angle_pitch_buffer, angle_roll_buffer;
boolean set_gyro_angles;
float angle_roll_acc, angle_pitch_acc;
float angle_pitch_output, angle_roll_output;
  int pitch_angle,roll_angle;

//mg996r servo motor initalizations
int servo_position=90;
int servo_1_pin=9;
int servo_2_pin=10;
int mapped_position;
int d=2;
int var;

//l298n motor driver initializations
int enA = 6;
int in1 = 4;
int in2 = 3;
int enB = 5;
int in3 = 8;
int in4 = 7;
int mapped_value;

//nrf24l01 variables initializations
//motor speed and direction
int JoystickLeft_var=00;               //these variables vary from -1023 to 1023
//servo angle
int JoystickRight_var=0;                   

void setup()
{
  Wire.begin();
  Serial.begin(115200);
 
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
//  Serial.println("call initialize");
//  initialize();
//  Serial.println("call mpu setup register");
  setup_mpu_6050_registers();
  for(int cal_int =0; cal_int<2000;cal_int++)
  {
    read_mpu_6050_data();                                              //Read the raw acc and gyro data from the MPU-6050
    gyro_x_cal += gyro_x;                                              //Add the gyro x-axis offset to the gyro_x_cal variable
    gyro_y_cal += gyro_y;                                              //Add the gyro y-axis offset to the gyro_y_cal variable
    gyro_z_cal += gyro_z;                                              //Add the gyro z-axis offset to the gyro_z_cal variable
    delay(3);
  }
  gyro_x_cal /= 2000;                                                  //Divide the gyro_x_cal variable by 2000 to get the avarage offset
  gyro_y_cal /= 2000;                                                  //Divide the gyro_y_cal variable by 2000 to get the avarage offset
  gyro_z_cal /= 2000;                                                  //Divide the gyro_z_cal variable by 2000 to get the avarage offset
// loop_timer=micros();
//  Serial.println("end setup");
// initialize();
}

void motion_front(int Left_var)
{
  // enabling pins 
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  mapped_value=map(Left_var,0,1023,0,255);                  // changing values from (0,1023) to (0,255)
  //setting the speed of motors based on mapped values as motor max input is 255 
  analogWrite(enA, mapped_value); 
  analogWrite(enB, mapped_value); 
  delay(20); 

}

void motion_back(int Left_var)
{
    // enabling pins but in opposite direction
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  mapped_value=map(Left_var,0,-1023,0,255);                      //changing values from (0,1023) to (0,255)
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

void motion_right(int Right_var)
{
  mapped_position=map(Right_var,0,1024,90,180);               
  servo_1.write(mapped_position);
  servo_2.write(180-mapped_position); 
}

void motion_left(int Right_var)
{
  mapped_position=map(Right_var,0,-1023,90,0);
  servo_1.write(mapped_position);
  servo_2.write(180-mapped_position);  
}

void servo_motion_stop()
{
    servo_1.write(servo_position);
    servo_2.write(servo_position);
//    Serial.println("90 90 90");

}

void initialize()
{
  for(var=-1023;var<1023;var+=2)
{
    if(var>3)
    {
      motion_right(var);
      delay(d);
    }
    else if(var<-3)
    {
      motion_left(var);
      delay(d);
    }
    else 
    {
      servo_motion_stop();
      delay(d);
    }
//    int a=servo_1.read();
//    Serial.println(a);
    
}
for(var=1023;var>-1023;var-=2)
{
    if(var>3)
    {
      motion_right(var);
      delay(d);
    }
    else if(var<-3)
    {
      motion_left(var);
      delay(d);
    }
    else 
    {
      servo_motion_stop();
      delay(d);
    }
//    int a=servo_1.read();
//    Serial.println(a);

}

for(var=-1023;var<0;var+=2)
{
    if(var>3)
    {
      motion_right(var);
      delay(d);
    }
    else if(var<-3)
    {
      motion_left(var);
      delay(d);
    }
    else 
    {
      servo_motion_stop();
      delay(d);
    }
//    int a=servo_1.read();
//    Serial.println(a);
    
}
}

void servo_write(int t)
{
  servo_1.write(t);
  servo_2.write(180-t);
  delay(5);  
}

void servo_motion( int angle)
{
  int curr,temp;
  curr=servo_1.read();
  if (curr<angle)
  {
    for(temp=servo_1.read();temp<angle;temp++)
      servo_write(temp);
  }
  if (curr>angle)
  {
    for(temp=servo_1.read();temp>angle;temp--)
      servo_write(temp);          
  }
}

void servo_motion_right(int JoystickRight)
{
  int desti=map(JoystickRight,0,1023,90,180);
  servo_motion(desti);
}

void servo_motion_left( int JoystickRight)
{
  int desti=map(JoystickRight,0,-1023,90,0);
  servo_motion(desti);  
}

void read_mpu_6050_data()
{                                             //Subroutine for reading the raw gyro and accelerometer data
//  Serial.println("start read data");
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x3B);                                                    //Send the requested starting register
  Wire.endTransmission();                                              //End the transmission
  Wire.requestFrom(0x68,14);                                           //Request 14 bytes from the MPU-6050
  while(Wire.available() < 14);                                        //Wait until all the bytes are received
  acc_x = Wire.read()<<8|Wire.read();                                  //Add the low and high byte to the acc_x variable
  acc_y = Wire.read()<<8|Wire.read();                                  //Add the low and high byte to the acc_y variable
  acc_z = Wire.read()<<8|Wire.read();                                  //Add the low and high byte to the acc_z variable
  temperature = Wire.read()<<8|Wire.read();                            //Add the low and high byte to the temperature variable
  gyro_x = Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_x variable
  gyro_y = Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_y variable
  gyro_z = Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_z variable
//  Serial.println("end read data");
}

void setup_mpu_6050_registers()
{
//  Serial.println("start setup registers");
  //Activate the MPU-6050
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x6B);                                                    //Send the requested starting register
  Wire.write(0x00);                                                    //Set the requested starting register
  Wire.endTransmission();                                              //End the transmission
  //Configure the accelerometer (+/-8g)
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x1C);                                                    //Send the requested starting register
  Wire.write(0x10);                                                    //Set the requested starting register
  Wire.endTransmission();                                              //End the transmission
  //Configure the gyro (500dps full scale)
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x1B);                                                    //Send the requested starting register
  Wire.write(0x08);                                                    //Set the requested starting register
  Wire.endTransmission();                                              //End the transmission
//  Serial.println("end setup registers");
}

void balance()
{
//  Serial.println("--------balance start---------");
  
  read_mpu_6050_data();                                                //Read the raw acc and gyro data from the MPU-6050

  gyro_x -= gyro_x_cal;                                                //Subtract the offset calibration value from the raw gyro_x value
  gyro_y -= gyro_y_cal;                                                //Subtract the offset calibration value from the raw gyro_y value
  gyro_z -= gyro_z_cal;                                                //Subtract the offset calibration value from the raw gyro_z value
  
  //Gyro angle calculations
  //0.0000611 = 1 / (250Hz / 65.5)
  angle_pitch += gyro_x * 0.0000611;                                   //Calculate the traveled pitch angle and add this to the angle_pitch variable
  angle_roll += gyro_y * 0.0000611;                                    //Calculate the traveled roll angle and add this to the angle_roll variable
  
  //0.000001066 = 0.0000611 * (3.142(PI) / 180degr) The Arduino sin function is in radians
  angle_pitch += angle_roll * sin(gyro_z * 0.000001066);               //If the IMU has yawed transfer the roll angle to the pitch angel
  angle_roll -= angle_pitch * sin(gyro_z * 0.000001066);               //If the IMU has yawed transfer the pitch angle to the roll angel
  
  //Accelerometer angle calculations
  acc_total_vector = sqrt((acc_x*acc_x)+(acc_y*acc_y)+(acc_z*acc_z));  //Calculate the total accelerometer vector
  //57.296 = 1 / (3.142 / 180) The Arduino asin function is in radians
  angle_pitch_acc = asin((float)acc_y/acc_total_vector)* 57.296;       //Calculate the pitch angle
  angle_roll_acc = asin((float)acc_x/acc_total_vector)* -57.296;       //Calculate the roll angle
  
  //Place the MPU-6050 spirit level and note the values in the following two lines for calibration
  angle_pitch_acc -= 0.0;                                              //Accelerometer calibration value for pitch
  angle_roll_acc -= 0.0;                                               //Accelerometer calibration value for roll

  if(set_gyro_angles){                                                 //If the IMU is already started
    angle_pitch = angle_pitch * 0.9996 + angle_pitch_acc * 0.0004;     //Correct the drift of the gyro pitch angle with the accelerometer pitch angle
    angle_roll = angle_roll * 0.9996 + angle_roll_acc * 0.0004;        //Correct the drift of the gyro roll angle with the accelerometer roll angle
  }
  else{                                                                //At first start
    angle_pitch = angle_pitch_acc;                                     //Set the gyro pitch angle equal to the accelerometer pitch angle 
    angle_roll = angle_roll_acc;                                       //Set the gyro roll angle equal to the accelerometer roll angle 
    set_gyro_angles = true;                                            //Set the IMU started flag
  }
  
  //To dampen the pitch and roll angles a complementary filter is used
  angle_pitch_output = angle_pitch_output * 0.9 + angle_pitch * 0.1;   //Take 90% of the output pitch value and add 10% of the raw pitch value
  angle_roll_output = angle_roll_output * 0.9 + angle_roll * 0.1;      //Take 90% of the output roll value and add 10% of the raw roll value
  pitch_angle=round(angle_pitch_output);
  roll_angle=round(angle_roll_output);
  Serial.print(angle_pitch_output);
  Serial.print("   /////    " );
  Serial.println(angle_roll_output);
//  while(micros() - loop_timer < 4000);                                 //Wait until the loop_timer reaches 4000us (250Hz) before starting the next loop
//  loop_timer = micros();                                               //Reset the loop timer

//  while(angle_roll_output>3 && angle_roll_output<-3)
//  {
//    if(angle_roll_output>3)
//      motion_back(300);
//    else if (angle_roll_outpu<-3)
//      motion_front(300);
//  }
//  motor_motion_stop();

//  while(pitch_angle>8 && pitch_angle<-8)
//  {
    if(pitch_angle>8)
    {
      int val=map(pitch_angle,0,90,90,180);
      servo_motion(val);
    }
    else if(pitch_angle<-8)
    {
      int val=map(pitch_angle,0,-90,90,0);
      servo_motion(val);
    }
    else
    {
      servo_motion_stop();
    }
//  }
//  Serial.println("------balance end-----------");

}

void loop()
{  
//  Serial.println("-----------------loop being---------------");
//    
//    int a=servo_1.read();
//    Serial.println(a);
    if(JoystickRight_var>3)
      servo_motion_right(JoystickRight_var);
    else if(JoystickRight_var<-3)
      servo_motion_left(JoystickRight_var);
    else 
    {
      servo_motion_stop();
      delay(100);
    }
    
    if(JoystickLeft_var>3)
      motion_front(JoystickLeft_var);
    else if(JoystickLeft_var<-3)
      motion_back(JoystickLeft_var);
    else 
      motor_motion_stop();
    
    if(JoystickRight_var<3 && JoystickRight_var>-3)
      if(JoystickLeft_var<3 && JoystickLeft_var>-3)
       {
//        Serial.println("call balance");
        balance();
       }
//Serial.println("-----------------loop end-------------------");
}
