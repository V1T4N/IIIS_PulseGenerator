#define SigIn A0
#define SW 8
#define TTLPin 13
#define LEDPinE 5 //LED E
#define LEDPinW 4
#define LEDPinN 3
#define LEDPinR 2
#define JpN 7
#define JpR 6
//#######################ユーザーが変更するところ#######################
float threthord =  0.1;//閾値 (0~1)
int PulseLen = 10;   //パルス長[ms]
//##############################################
int cnt = 0;//設定用カウンタ


void setup() {
  pinMode(SigIn,INPUT);
  pinMode(SW,INPUT_PULLUP);
  pinMode(JpN,INPUT_PULLUP);
  pinMode(JpR,INPUT_PULLUP);
  
  pinMode(TTLPin,OUTPUT);
  pinMode(LEDPinE,OUTPUT);
  pinMode(LEDPinW,OUTPUT);
  pinMode(LEDPinN,OUTPUT);
  pinMode(LEDPinR,OUTPUT);
  pinMode(A1,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int setuplabel = 0;
  String tempstr;
  
  label: //ジャンプ用ラベル
  
   while(true){ //閾値設定する用
    if(digitalRead(JpN) == 0){
      cnt++;
      threthord = led(cnt);
      delay(500);
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

  
  while(setuplabel == 0){ //パルス長設定する用
     if(digitalRead(JpN) == 0){
      cnt++;
      PulseLen = 100 * led(cnt);
      delay(500);
    }else if(digitalRead(SW) == 1){
     setuplabel == 1;
     break;
    }else if(digitalRead(JpR) == 0){
    break;
    }
  }

  led(0);

  while(digitalRead(SW) == 0 && setuplabel == 0){ //開始スイッチ待ち
    digitalWrite(LEDPinE,HIGH);
    delay(50);
    digitalWrite(LEDPinE,LOW);
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
    //Serial.print(data_f);
    //Serial.print("|");
    //Serial.println(maxdata);
    
    if(data_f < threthord &&  millis() - before_time > 100){ //ノイズが来た時に連続して反応しないように7[Hz]の T = 140[ms],少なめに見積もって100[ms]経過しないと次のパルスを出さない
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
      //digitalWrite(LEDPinE,LOW);
      digitalWrite(LEDPinW,LOW);
      digitalWrite(LEDPinN,LOW);
      digitalWrite(LEDPinR,LOW);
      break;

    case 1:
    //digitalWrite(LEDPinE,LOW);
      digitalWrite(LEDPinW,LOW);
      digitalWrite(LEDPinN,LOW);
      digitalWrite(LEDPinR,HIGH);
      return 0.1;
      break;
      
    case 2:
    //digitalWrite(LEDPinE,LOW);
      digitalWrite(LEDPinW,LOW);
      digitalWrite(LEDPinN,HIGH);
      digitalWrite(LEDPinR,LOW);
      return 0.2;
      break;

    case 3:
    //digitalWrite(LEDPinE,LOW);
      digitalWrite(LEDPinW,LOW);
      digitalWrite(LEDPinN,HIGH);
      digitalWrite(LEDPinR,HIGH);
      return 0.3;
      break;

   case 4:
    //digitalWrite(LEDPinE,LOW);
      digitalWrite(LEDPinW,HIGH);
      digitalWrite(LEDPinN,LOW);
      digitalWrite(LEDPinR,LOW);
      return  0.4;
      break;

   case 5:
    //digitalWrite(LEDPinE,LOW);
      digitalWrite(LEDPinW,HIGH);
      digitalWrite(LEDPinN,LOW);
      digitalWrite(LEDPinR,HIGH);
      return 0.5;
      break;

   case 6:
    //digitalWrite(LEDPinE,LOW);
      digitalWrite(LEDPinW,HIGH);
      digitalWrite(LEDPinN,HIGH);
      digitalWrite(LEDPinR,LOW);
      return 0.6;
      break;

    case 7:
    //digitalWrite(LEDPinE,LOW);
      digitalWrite(LEDPinW,HIGH);
      digitalWrite(LEDPinN,HIGH);
      digitalWrite(LEDPinR,HIGH);
      return 0.7;
      break;

    default:
      digitalWrite(LEDPinW,LOW);
      digitalWrite(LEDPinN,LOW);
      digitalWrite(LEDPinR,LOW);
      cnt = 0;
      break;
    
  }
  
}

