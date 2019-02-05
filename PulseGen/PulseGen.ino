#define SigIn A0
#define SW 12
#define TTLPin 13
#define LEDPin 13
//#######################ユーザーが変更するところ#######################
float threthord =  0.9;//閾値 (0~1)
int PulseLen = 10;   //パルス長[ms]
//##############################################


void setup() {
  pinMode(SigIn,INPUT);
  pinMode(SW,INPUT_PULLUP);
  pinMode(TTLPin,OUTPUT);
  pinMode(LEDPin,OUTPUT);
  pinMode(A1,OUTPUT);
  Serial.begin(9600);
}

void loop() {

  int setuplabel = 0;
  String tempstr;

   while(true){ //シリアル通信で設定する用
    if (Serial.available() > 0){
      tempstr = Serial.readStringUntil('\0');
      threthord = tempstr.toFloat();
      break;
    }else if(digitalRead(SW) == 1){ //フットスイッチが押されたら受信待ちを終了
      setuplabel = 1;
      break;
    }
  }

  
  while(setuplabel == 0){
    if (Serial.available() > 0){
      tempstr = Serial.readStringUntil('\0');
      PulseLen = tempstr.toInt();
      break;
    }else if(digitalRead(SW) == 1){
      setuplabel = 1;
      break;
    }
  }

  label: //ジャンプ用ラベル

  while(digitalRead(SW) == 0 && setuplabel == 0){ //開始スイッチ待ち
    digitalWrite(LEDPin,HIGH);
    delay(50);
    digitalWrite(LEDPin,LOW);
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
    
    if(data_f > threthord &&  millis() - before_time > 100){ //ノイズが来た時に連続して反応しないように7[Hz]の T = 140[ms],少なめに見積もって100[ms]経過しないと次のパルスを出さない
      before_time = millis();
      digitalWrite(TTLPin,HIGH);
      delay(PulseLen);
    }else if(digitalRead(SW) == 0){//フットスイッチが離されたら
      delay(50);
      if(digitalRead(SW) == 0){ //チャタリング防止のため50[ms]待つ
        setuplabel = 0;
        goto label;
      }
    }else{
      digitalWrite(TTLPin,LOW);
    }
  }



}
