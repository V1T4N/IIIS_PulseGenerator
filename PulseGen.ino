#define SigIn A0
#define SW 12
#define TTLPin 13

#define threthord 0.5 //閾値 (0~1)
#define PulseLen 30 //パルス長[ms]

void setup() {
  pinMode(SigIn,INPUT);
  pinMode(SW,INPUT_PULLUP);
  pinMode(TTLPin,OUTPUT);
  Serial.begin(9600);
}

void loop() {

  while(digitalRead(SW) == 0){ //開始スイッチ待ち
    digitalWrite(TTLPin,HIGH);
    delay(50);
    digitalWrite(TTLPin,LOW);
    delay(50);
  }

  float maxdata = 0;
  int data = 0;

  while(maxdata < 10){ //1024msの間の最大値を取る．一定以下の場合失敗とみなして再度
    for(int i = 0; i<1024; i++){
      data = analogRead(SigIn);
      delay(1);
      if(data > maxdata){
        maxdata = data;
      }
    }
  }
  
  float data_f;

  while(true){
    data_f = analogRead(SigIn) / maxdata;
    Serial.print(data_f);
    Serial.print("|");
    Serial.println(maxdata);
    if(data_f > threthord){
      digitalWrite(TTLPin,HIGH);
      delay(PulseLen);
    }else{
      digitalWrite(TTLPin,LOW);
    }
  }



}
