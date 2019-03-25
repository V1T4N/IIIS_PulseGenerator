/*
#define SigIn A0
#define SW 8
#define TTLPin 13
#define LEDPinE 5 //LED E
#define LEDPinW 4
#define LEDPinN 3
#define LEDPinR 2
#define JpN 7
#define JpR 6
*/


#define SigIn A0
#define SW 8
#define TTLPin 13
#define LEDPin4 6 //LED E
#define LEDPin3 5 //LED E
#define LEDPin2 4
#define LEDPin1 3
#define LEDPin0 2
#define JpN 10
#define JpR 9

//#######################ユーザーが変更するところ#######################
float threthord =  0.7;//閾値 (0~1)
int PulseLen = 10;   //パルス長[ms]
//##############################################
int cnt = 0;//設定用カウンタ



void setup() {
  pinMode(SigIn,INPUT);
  pinMode(SW,INPUT_PULLUP);
  pinMode(JpN,INPUT_PULLUP);
  pinMode(JpR,INPUT_PULLUP);
  
  pinMode(TTLPin,OUTPUT);
  pinMode(LEDPin0,OUTPUT);
  pinMode(LEDPin1,OUTPUT);
  pinMode(LEDPin2,OUTPUT);
  pinMode(LEDPin3,OUTPUT);
  pinMode(LEDPin4,OUTPUT);
  pinMode(A1,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int setuplabel = 0;
  String tempstr;
  
  label: //ジャンプ用ラベル
  led(threthord * 10);
  
  
   while(true){ //閾値設定する用
    if(digitalRead(JpN) == 0){
      cnt++;
      threthord = led(cnt);
      delay(200);
    }else if(digitalRead(SW) == 1){
     setuplabel == 1;
     break; 
   }else if(digitalRead(JpR) == 0){
      led(0);
      delay(200);
      led(1);
      delay(200);
      led(2);
      delay(200);
      led(4);
      delay(200);
      led(7);
      delay(1000);
      led(0);
      cnt = 0;
      break;
   }
  }

  led(PulseLen / 10);
  while(setuplabel == 0){ //パルス長設定する用
     if(digitalRead(JpN) == 0){
      cnt++;
      PulseLen = 100 * led(cnt);
      delay(200);
    }else if(digitalRead(SW) == 1){
     setuplabel == 1;
     break;
    }else if(digitalRead(JpR) == 0){
    break;
    }
  }

  led(0);

  while(digitalRead(SW) == 0 && setuplabel == 0){ //開始スイッチ待ち
    digitalWrite(LEDPin4,HIGH);
    delay(50);
    digitalWrite(LEDPin4,LOW);
    delay(50);
  }

  float maxdata = 0;
  int data = 0;

  while(maxdata < 10){ //1000msの間の最大値を取る．一定以下の場合失敗とみなして再度
    for(int i = 0; i<1000; i++){
      data = analogRead(SigIn);
      delay(1);
      if(data > maxdata){
        maxdata = data;
      }
    }
  }
  
  float data_f;
  unsigned long before_time = millis();

  while(true){
    data_f = analogRead(SigIn) / maxdata;
    Serial.println(data_f);
    //Serial.print("|");
    //Serial.println(maxdata);
    
    if(data_f > threthord &&  millis() - before_time > 100){ //ノイズが来た時に連続して反応しないように7[Hz]の T = 140[ms],少なめに見積もって100[ms]経過しないと次のパルスを出さない
              //ここの不等号を入れ替えて上ピーク・下ピーク切り替え可能
      before_time = millis();
      digitalWrite(TTLPin,HIGH);
      delay(PulseLen);
    }else if(digitalRead(SW) == 0){//フットスイッチが離されたら
      delay(10);
      if(digitalRead(SW) == 0){ //チャタリング防止のため10[ms]待つ
        setuplabel = 0;
        digitalWrite(TTLPin,LOW);
        goto label;
      }
    }else{
      digitalWrite(TTLPin,LOW);
    }
  }



}


float led (int num){

  switch(num){

    case 0:
      digitalWrite(LEDPin3,LOW);
      digitalWrite(LEDPin2,LOW);
      digitalWrite(LEDPin1,LOW);
      digitalWrite(LEDPin0,LOW);
      break;

    case 1:
      digitalWrite(LEDPin3,LOW);
      digitalWrite(LEDPin2,LOW);
      digitalWrite(LEDPin1,LOW);
      digitalWrite(LEDPin0,HIGH);
      return 0.1;
      break;
      
    case 2:
      digitalWrite(LEDPin3,LOW);
      digitalWrite(LEDPin2,LOW);
      digitalWrite(LEDPin1,HIGH);
      digitalWrite(LEDPin0,LOW);
      return 0.2;
      break;

    case 3:
      digitalWrite(LEDPin3,LOW);
      digitalWrite(LEDPin2,LOW);
      digitalWrite(LEDPin1,HIGH);
      digitalWrite(LEDPin0,HIGH);
      return 0.3;
      break;

   case 4:
      digitalWrite(LEDPin3,LOW);
      digitalWrite(LEDPin2,HIGH);
      digitalWrite(LEDPin1,LOW);
      digitalWrite(LEDPin0,LOW);
      return  0.4;
      break;

   case 5:
      digitalWrite(LEDPin3,LOW);
      digitalWrite(LEDPin2,HIGH);
      digitalWrite(LEDPin1,LOW);
      digitalWrite(LEDPin0,HIGH);
      return 0.5;
      break;

   case 6:
      digitalWrite(LEDPin3,LOW);
      digitalWrite(LEDPin2,HIGH);
      digitalWrite(LEDPin1,HIGH);
      digitalWrite(LEDPin0,LOW);
      return 0.6;
      break;

    case 7:
      digitalWrite(LEDPin3,LOW);
      digitalWrite(LEDPin2,HIGH);
      digitalWrite(LEDPin1,HIGH);
      digitalWrite(LEDPin0,HIGH);
      return 0.7;
      break;

     case 8:
      digitalWrite(LEDPin3,HIGH);
      digitalWrite(LEDPin2,LOW);
      digitalWrite(LEDPin1,LOW);
      digitalWrite(LEDPin0,LOW);
      return 0.8;
      break;

     case 9:
      digitalWrite(LEDPin3,HIGH);
      digitalWrite(LEDPin2,LOW);
      digitalWrite(LEDPin1,LOW);
      digitalWrite(LEDPin0,HIGH);
      return 0.9;
      break;

     case 10:
      digitalWrite(LEDPin3,HIGH);
      digitalWrite(LEDPin2,LOW);
      digitalWrite(LEDPin1,HIGH);
      digitalWrite(LEDPin0,LOW);
      return 1.0;
      break;




    default:
      digitalWrite(LEDPin3,LOW);
      digitalWrite(LEDPin2,LOW);
      digitalWrite(LEDPin1,LOW);
      digitalWrite(LEDPin0,LOW);
      cnt = 0;
      break;
    
  }
  
}
