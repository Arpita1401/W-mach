/*
 * written by Sayansree Paria
 */
 
#define front_top_left 3
#define front_top_right 5
#define front_bottom_left 6
#define front_bottom_right 9
#define back_top 10
#define back_bottom 11

long timer;
bool timerFlag;

void setup() {
  Serial.begin(115200);
  Serial.write("device on\n"); 
  
  pinMode(front_top_left,OUTPUT);
  pinMode(front_top_right,OUTPUT);
  pinMode(front_bottom_left,OUTPUT);
  pinMode(front_bottom_right,OUTPUT);
  pinMode(back_top,OUTPUT);
  pinMode(back_bottom,OUTPUT);
  
  analogWrite(front_top_left,0);
  analogWrite(front_top_right,0);
  analogWrite(front_bottom_left,0);
  analogWrite(front_bottom_right,0);
  analogWrite(back_top,0);
  analogWrite(back_bottom,0);

  timer=millis();
  timerFlag=false;
}

void loop() {
  if (Serial.available()){
      byte b=Serial.read();
      byte val =(b&0b11)*255/3; // extract power level  0% 33% 67% 100% from last two bits
      b=b>>2;                   
      if(b==63){ //when all heating elements are selected
        if (val==0)// turn all off when value is zero
          allOff();
        else if (val==255) //all on  turn all elements when value is maximum
          allOn();
        else //or turn on reset timer
          ResetTimer();
      }else if (b&1)    //or when one heating element is selected 
        analogWrite(front_top_left , val);
      else if ((b>>1)&1)
        analogWrite(front_top_right , val);
      else if ((b>>2)&1)
        analogWrite(front_bottom_left , val);
      else if ((b>>3)&1)
        analogWrite(front_bottom_right , val);
      else if ((b>>4)&1)
        analogWrite(back_top , val);
      else if ((b>>5)&1)
        analogWrite(back_bottom , val);
        
      Serial.print(val);
    }
    if(timerFlag&&timer<millis()){
      timerFlag=false;
      allOff();
    }
}
void allOff(){
  analogWrite(front_top_left,0);
  analogWrite(front_top_right,0);
  analogWrite(front_bottom_left,0);
  analogWrite(front_bottom_right,0);
  analogWrite(back_top,0);
  analogWrite(back_bottom,0);
}
void allOn(){
  analogWrite(front_top_left,255);
  analogWrite(front_top_right,255);
  analogWrite(front_bottom_left,255);
  analogWrite(front_bottom_right,255);
  analogWrite(back_top,255);
  analogWrite(back_bottom,255);
}
void ResetTimer(){
  timer=millis()+18e5;//add 30 minutes timer 
  timerFlag=true;// turn on timer
}
