#include <LiquidCrystal.h>
#include <MsTimer2.h>

int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

#define SW 2
#define IN A1
#define TTLPin 3

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

float th = 0.8;
int len = 10;
int phase = 0;

int read_LCD_buttons(){
  adc_key_in = analogRead(0);      // read the value from the sensor 
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  // For V1.1 us this threshold
  if (adc_key_in < 50)   return btnRIGHT;  
  if (adc_key_in < 250)  return btnUP; 
  if (adc_key_in < 450)  return btnDOWN; 
  if (adc_key_in < 650)  return btnLEFT; 
  if (adc_key_in < 850)  return btnSELECT;  

  return btnNONE;  // when all others fail, return this...
}

int change_th(){
  while(true){
    display_lcd();
    lcd.setCursor(0,0);
    lcd.print("*");
    delay(50);
     switch(read_LCD_buttons()){

        case 0://RIGHT
          return 1;
        case 1://up
          th = th + 0.01;
          break;
        case 2:
          th = th - 0.01;
          break;//down
        case 4: //select
          return 0;
          break;
        default:
          break;
      
    }
    if(th == 9.9 || th < 0 ) {
      th = 0;
      lcd.clear();
    }
  }
}
int change_len(){
  while(true){
    display_lcd();
    lcd.setCursor(0,1);
    lcd.print("*");
    delay(50);
     switch(read_LCD_buttons()){
        case 0://RIGHT
          return 1;
        case 1://up
          len++;
          break;
        case 2:
          len--;
          break;//down
        case 4://select
          return 0;
          break;
        default:
          break;
    }
    if(len== 150 || th < 0 ) {
      th = 0;
      lcd.clear();
    }
  }
}
int change_phase(){
  while(true){
    display_lcd();
    lcd.setCursor(7
    ,1);
    lcd.print("*");
    delay(50);
     switch(read_LCD_buttons()){

        case 0://RIGHT
          return 1;
        case 1://up
          phase++;
          break;
        case 2:
          phase--;
          break;//down
        case 4: //select
          return 0;
          break;
        default:
          break;
      
    }
    if(phase == 181 || phase < 0 ) {
      phase = 0;
      lcd.clear();
    }
  }
}

void display_lcd(){
lcd.clear();
lcd.setCursor(0,0); 
lcd.print(" Th =");
lcd.print(th);
lcd.setCursor(0,1); 
lcd.print(" Len=");
lcd.print(len);
//lcd.setCursor(0,1); 
lcd.print(" ph=");
lcd.print(phase);
}

void setup() {
  lcd.begin(16, 2);
  pinMode(SW,INPUT_PULLUP);
  pinMode(TTLPin,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  while(true){ //設定用ループ
    if(change_th() == 1) break;
    delay(200);
    if(change_len() == 1) break;
    delay(200);
    if(change_phase() == 1) break;
    delay(200);
  }
 
  label1: //ジャンプ用ラベル
  
  lcd.clear(); 
  lcd.print("waiting for");
  lcd.setCursor(0,1);
  lcd.print("wave learning");
  delay(500);

  float ave = 0;
  float sd = 0;
  while(read_LCD_buttons() != 0){//波形学習ボタン待ち
  }
  long i = 0;
  int data[700];
  long sum = 0;
  long sum2 = 0;

  lcd.clear();
  
  for(int i = 0; i< 700;i++){
    lcd.setCursor(0,0);
    lcd.print("setting...");
    data[i] = analogRead(IN);
    sum = sum + data[i];
    ave = sum / i;
    delay(7); //128Hzだとして700回でおよそ５秒
  }
  for(int j = 0; j < 700; j++){
    sum2 = sum2 + pow((data[j] - ave),2);
  }
  sd = pow(sum2/700,0.5); 
  

  lcd.setCursor(0,0); 
  lcd.print("ave=");
  lcd.print(ave);
  lcd.setCursor(0,1);
  lcd.print("sd=");
  lcd.print(sd);
  lcd.print("waiting");

  byte flag = 0;
  byte flag_d = 1; //ダミーパルス用フラグ
  byte TTL_flag = 0;
  byte Phase_Flag = 0;
  unsigned long TTL_time = 0;
  unsigned long Phase_time = 0;

  /*float out_1;
  float out_2;
  float out_3;*/

  float out[5];
  
  unsigned long before_time = millis();
  unsigned long dynamic_freq;
  byte SW_flag = 0;
  
  
  while(true){ //測定時メインループ
    while(true){
      if(digitalRead(SW) == 1) {
         delay(500);
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("running...");
         break;       
      }
    }
    while(true){
      /*
      out_1 = (analogRead(IN) - ave) / sd;
      out_2 = (analogRead(IN) - ave) / sd;
      out_3 = (analogRead(IN) - ave) / sd;
      */
      
      out[0] = out[1]; //スライドして更新
      out[1] = out[2];
      out[2] = out[3];
      out[3] = out[4];
      out[4] = (analogRead(IN) - ave) / sd;  
      
      if(out[2] > th  && out[0]<out[1] && out[1]<out[2] && out[2]>out[3] && out[3]>out[4] &&  millis() - before_time> 100 &&TTL_flag == 0){ //ノイズが来た時に連続して反応しないように7[Hz]の T = 140[ms],少なめに見積もって100[ms]経過しないと次のパルスを出さない
        dynamic_freq = millis() - before_time;
        before_time = millis();
        flag_d = 0;
        TTL_flag = 1;
        TTL_time = millis();
        
      }/*else if(millis() - before_time > 160 && flag_d == 0){
        before_time = millis();
        flag_d = 1; //2回連続ではダミーパルスを出さないようにする
        TTL_flag = 1;
        TTL_time = millis();
      }*/
      if(TTL_flag == 1 && millis() - TTL_time > (phase/360.0)*dynamic_freq){ //TTLフラグONからphase分立ったら実行される
        digitalWrite(TTLPin,HIGH);
        Phase_time = millis();
        Phase_Flag = 1;   
        TTL_flag = 0;
      }if(Phase_Flag == 1 && millis() - Phase_time > len){ //パルスの発生時間がlenを超えたらLOWにする
        digitalWrite(TTLPin,LOW);
        Phase_Flag = 0;
      }
      if(read_LCD_buttons() == 0){ //RIGHTが押されたら
        goto label1;
      }
      if(digitalRead(SW) == 1){
         delay(500);
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("waiting for");
         lcd.setCursor(0,1);
         lcd.print("re start");
         break;
      }
    }
  }
}
