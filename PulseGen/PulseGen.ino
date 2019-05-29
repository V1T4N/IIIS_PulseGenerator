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
int cnt = threthord*10 ;//設定用カウンタ



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
      delay(100);
      led(1);
      delay(100);
      led(2);
      delay(100);
      led(4);
      delay(100);
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
      led(0);
      delay(200);
      led(7);
      delay(200);
      led(4);
      delay(100);
      led(2);
      delay(100);
      led(1);
      delay(1000);
      led(0);
      break;
    }
  }

  led(0);
  
  float maxdata = 0;
  int data = 0;
  
  label: //ジャンプ用ラベル
  delay(500);
  while(digitalRead(SW) == 0 && setuplabel == 0){ //開始スイッチ待ち
    if(maxdata == 0){
      digitalWrite(LEDPin4,HIGH);
      delay(50);
      digitalWrite(LEDPin4,LOW);
      delay(50);
     }else{
      digitalWrite(LEDPin4,HIGH);
      digitalWrite(LEDPin3,LOW);
      delay(50);
     }
    while(digitalRead(JpR) == 0){
      digitalWrite(LEDPin3,HIGH);
      data = analogRead(SigIn);
      if(data > maxdata){
        maxdata = data;
      }
    }
  }

  
  /*
  while(maxdata < 10){ //1000msの間の最大値を取る．一定以下の場合失敗とみなして再度
    for(int i = 0; i<1000; i++){
      data = analogRead(SigIn);
      delay(1);
      if(data > maxdata){
        maxdata = data;
      }
    }
  }
*/
  int flag = 1; //ダミーパルス用フラグ
  float data_1;
  float data_2;
  float data_3;
  unsigned long before_time = millis();
  unsigned long dynamic_freq;
  
  while(true){
    data_1 = analogRead(SigIn) / maxdata;
    data_2 = analogRead(SigIn) / maxdata;
    data_3 = analogRead(SigIn) / maxdata;
    
    dynamic_freq = millis() - before_time //前回のshockからの経過時間
    
    //2019_4_2 ピーク値をグラフのを使って検出する
    if(data_2 > threthord && data_1 - data_2 < 0 && data_2 - data_3 > 0 &&  dynamic_freq > 100){ //ノイズが来た時に連続して反応しないように7[Hz]の T = 140[ms],少なめに見積もって100[ms]経過しないと次のパルスを出さない
      before_time = millis();
      flag = 0;
      digitalWrite(TTLPin,HIGH);
      delay(PulseLen);
    }else if(millis() - before_time > 160 && flag == 0){
      before_time = millis();
      flag = 1; //2回連続ではダミーパルスを出さないようにする
      digitalWrite(TTLPin,HIGH);
      led(7);
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
      led(0);
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
