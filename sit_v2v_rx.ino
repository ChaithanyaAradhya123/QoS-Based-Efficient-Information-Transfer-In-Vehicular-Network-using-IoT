#include <SoftwareSerial.h>
SoftwareSerial xbeeSerial(11,12); //RX, TX

#include<LiquidCrystal.h>
LiquidCrystal lcd(16,17,4,5,6,7);
int bz=15,m=3;


void setup() {
   Serial.begin(9600);
   xbeeSerial.begin(9600);
   pinMode(bz,OUTPUT);
   pinMode(m,3);
   lcd.begin(16,2);    
   lcd.setCursor(0,0);
   lcd.print("V2V Comunication");
   lcd.setCursor(0,1);
   lcd.print("Slave vehicle");
   delay(3000);
   lcd.clear();
 
}

void loop() {

      while(xbeeSerial.available()>0)
      {  xbeeSerial.listen();
         String text,text2;
         text= xbeeSerial.readString();
         
         Serial.println(text);
        Serial.println("------------------------------------------------------");
        
         delay(1000);
         
         lcd.clear();   
         String lt= text.substring(0, 9);
         String lg= text.substring(9, 18);
         String dst=text.substring(18, 21);
         String spd=text.substring(21, 28);
         String acc=text.substring(23, 29);
          

          int dt=dst.toInt();
          int sd=spd.toInt();
          int ac=acc.toInt();
          analogWrite(m,200);
          lcd.setCursor(0,0);
          lcd.print("SF:");
          lcd.setCursor(3,0);
          lcd.print(sd);

          lcd.setCursor(10,0);
          lcd.print("DF:");
          lcd.setCursor(13,0);
          lcd.print(dt);
          


                   if (ac<0)
                   {
                    Serial.println("Accident");
                    digitalWrite(bz,HIGH);
                    lcd.setCursor(0,0);
                    lcd.print("Lt:");
                    lcd.setCursor(3,0);
                    lcd.print(lt);
                    lcd.setCursor(10,0);
                    lcd.print("                            ");
                    
                    lcd.setCursor(0,1);
                    lcd.print("Lo:");
                    lcd.setCursor(3,1);
                    lcd.print(lg);
                     lcd.setCursor(10,1);
                    lcd.print("                            ");
                    delay(500);
                    digitalWrite(bz,LOW);
                    delay(500);
                   
                    
                   }
                   if(dt<20)
                   {
                    Serial.println("Alert Low distance warning");
                     digitalWrite(bz,HIGH);
                     analogWrite(m,50);
                    lcd.setCursor(0,0);
                    lcd.print("Dst:");
                    lcd.setCursor(5,0);
                    lcd.print(dt);
                    
                    lcd.setCursor(0,0);
                    lcd.print("spd:");
                    lcd.setCursor(5,0);
                    lcd.print(sd);   

                    lcd.setCursor(0,1);
                    lcd.print("Low dist Alert");
                    delay(500);
                    digitalWrite(bz,LOW);
                    delay(500);
                             
                   }

                   
         Serial.println(lt);
         Serial.println(lg);
         Serial.println(dt);
         Serial.println(sd);
         Serial.println(ac);
         
         Serial.println("------------------------------------------------------");
         
      
      }
   }
   
