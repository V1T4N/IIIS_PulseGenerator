
#include <LiquidCrystal.h>
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int th = 0;
int len = 0;
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
    //lcd.blink();
    delay(100);
     switch(read_LCD_buttons()){

        case 0://RIGHT
          return 1;
        case 1://up
          th++;
          break;
        case 2:
          th--;
          break;//down
        case 4: //select
          return 0;
          break;
        default:
          break;
      
    }
    if(th == 99 || th < 0 ) th = 0;
  }
}
int change_len(){
  while(true){
    display_lcd();
    delay(100);
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
    if(len== 99 || th < 0 ) th = 0;
  }
}
int change_phase(){
  while(true){
    display_lcd();
    delay(100);
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
    if(th == 99 || th < 0 ) th = 0;
  }
}

void display_lcd(){
lcd.setCursor(0,0); 
lcd.print("Th = 0.");
lcd.print(th);
lcd.setCursor(0,1); 
lcd.print("Len=");
lcd.print(len);
//lcd.setCursor(0,1); 
lcd.print(" phase=");
lcd.print(phase);
}

void setup() {
  lcd.begin(16, 2);
  
}

void loop() {
  if(change_th() == 1) digitalWrite(13,HIGH);
  delay(50);
  if(change_len() == 1) digitalWrite(13,HIGH);
  delay(50);
  if(change_phase() == 1) digitalWrite(13,HIGH);
  delay(50);
  
}
