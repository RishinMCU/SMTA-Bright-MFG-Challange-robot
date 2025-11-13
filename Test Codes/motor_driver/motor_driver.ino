void setup() {
  motor_initiate();
}

void loop() {
  for(int i=0;i<=100;i++)
  {
   motor ('F', i , i);
   delay(20);
  }
  delay(2000);
  motor ('S', 0 , 0);
  delay(1000);
  for(int i=0;i<=100;i++)
  {
   motor ('B', i , i);
   delay(20);
  }
  delay(2000);
  motor ('S', 0 , 0);
  delay(1000);
  for(int i=0;i<=100;i++)
  {
   motor ('F', i , 0);
   delay(20);
  }
  delay(2000);
  motor ('S', 0 , 0);
  delay(1000);
  for(int i=0;i<=100;i++)
  {
   motor ('F', 0, i);
   delay(20);
  }
  delay(2000);
  motor ('S', 0 , 0);
  delay(10000);
}

void motor(char type, int left, int right)
{
  switch (type){
    case 'S': // break applied to left and right motor
      analogWrite(PA8,0);
      analogWrite(PA9,0);
      analogWrite(PA11,0);
      analogWrite(PA10,0);
      break;
    /*case 'S': // putting the motor in Hi-Z mode aka stoping the motor 
      digitalWrite(PA8,LOW);
      digitalWrite(PA9,LOW);
      digitalWrite(PA11,LOW);
      digitalWrite(PA10,LOW);
      break;*/
    case 'F': // putting the motor in forward rotation 
      analogWrite(PA8,left);
      analogWrite(PA9,0);
      analogWrite(PA11,right);
      analogWrite(PA10,0);
      break;
    case 'B': // putting the motor in reverse rotation 
      analogWrite(PA8,0);
      analogWrite(PA9,left);
      analogWrite(PA11,0);
      analogWrite(PA10,right);
      break;
    default :
      break;
  }

}
void motor_initiate()
{
  //setting the motor driver 1 and 2 pin to output
  pinMode(PA8, OUTPUT);  // motor A_1
  pinMode(PA9, OUTPUT);  // motor A_2
  pinMode(PA11, OUTPUT); // motor B_1
  pinMode(PA10, OUTPUT); // motor B_2
  // setting the motor drivers in Hi-Z
  digitalWrite(PA8,LOW);
  digitalWrite(PA9,LOW);
  digitalWrite(PA10,LOW);
  digitalWrite(PA11,LOW);

}