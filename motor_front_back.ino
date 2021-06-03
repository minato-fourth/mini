// motor one
int enA = 6;
int in1 = 4;
int in2 = 3;
// motor two
int enB = 5;
int in3 = 1;
int in4 = 0;
int a;
int JoystickLeft_var=200;
void setup()
{
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void motion_front(int JoystickLeft_var)
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
  analogWrite(enA, JoystickLeft_var); 
  analogWrite(enB, JoystickLeft_var); 
  delay(2000); 

}

void motion_back(int JoystickLeft_var)
{
  a=abs(JoystickLeft_var);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);  
  analogWrite(enA, a); 
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW); 
  analogWrite(enB, a); 
  delay(2000); 

}

void motion_stop()
{
    // now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);  

}

void loop()
{

  if(JoystickLeft_var>3)
  {
    motion_front(JoystickLeft_var);
    delay(1000);
  } 
  else if(JoystickLeft_var<-3)
  {
    motion_back(JoystickLeft_var);

  }
  else 
  {
    motion_stop();
  } 
  
}
