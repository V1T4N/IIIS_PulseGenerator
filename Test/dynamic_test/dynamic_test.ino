#include <MsTimer2.h>

#define IN A1
#define TTLPin 13

float th = 0.9;
int len = 10;
int phase = 90;


void setup() {
  pinMode(TTLPin,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  float ave = 0;
  float sd = 0;

  long i = 0;
  int data[700];
  long sum = 0;
  long sum2 = 0;

  
  for(int i = 0; i< 700;i++){
    data[i] = analogRead(IN);
    sum = sum + data[i];
    ave = sum / i;
    delay(7); //128Hzだとして700回でおよそ５秒
  }
  for(int j = 0; j < 700; j++){
    sum2 = sum2 + pow((data[j] - ave),2);
  }
  sd = pow(sum2/700,0.5); 
  Serial.println(sd);
  Serial.println(ave);
  

  byte flag = 0;
  byte flag_d = 1; //ダミーパルス用フラグ
  byte TTL_flag = 0;
  byte Phase_Flag = 0;
  unsigned long TTL_time = 0;
  unsigned long Phase_time = 0;

  float out[5];
  
  unsigned long before_time = millis();
  unsigned long dynamic_freq;
  
  
  while(true){ //測定時メインループ
    /*
    out_1 = (analogRead(IN) - ave) / sd;
    out_2 = (analogRead(IN) - ave) / sd;
    out_3 = (analogRead(IN) - ave) / sd;
    out_4 = (analogRead(IN) - ave) / sd;
    */
    out[0] = out[1]; //スライドして更新
    out[1] = out[2];
    out[2] = out[3];
    out[3] = out[4];
    out[4] = (analogRead(IN) - ave) / sd;
/*
    Serial.print(out[0]);
    Serial.print(",");
    Serial.print(out[1]);
    Serial.print(",");
    Serial.print(out[2]);
    Serial.print(",");
    Serial.print(out[3]);
    Serial.print(",");
    Serial.print(out[4]);
    Serial.println("");
       */ 
    if(out[2] > th  && out[0]<=out[1] && out[1]<out[2] && out[2]>out[3] && out[3]>=out[4] &&  millis() - before_time> 100 &&TTL_flag == 0 ){ //ノイズが来た時に連続して反応しないように7[Hz]の T = 140[ms],少なめに見積もって100[ms]経過しないと次のパルスを出さない
      dynamic_freq = millis() - before_time;
      before_time = millis();
      flag_d = 0;
      TTL_flag = 1;
      TTL_time = millis();
      
    }/*else if(millis() - before_time > 160 && flag_d == 0){//ダミーパルスを出すところ
      before_time = millis();
      flag_d = 1; //2回連続ではダミーパルスを出さないようにする
      TTL_flag = 1;
      TTL_time = millis();
    }*/
    if(TTL_flag == 1 && millis() - TTL_time > float(phase/180.0)*dynamic_freq){ //TTLフラグONからphase分立ったら実行される
      digitalWrite(TTLPin,HIGH);
      Phase_time = millis();
      Phase_Flag = 1;   
      TTL_flag = 0;
    }if(Phase_Flag == 1 && millis() - Phase_time > len){ //パルスの発生時間がlenを超えたらLOWにする
      digitalWrite(TTLPin,LOW);
      Phase_Flag = 0;
      
    }
  }
}
