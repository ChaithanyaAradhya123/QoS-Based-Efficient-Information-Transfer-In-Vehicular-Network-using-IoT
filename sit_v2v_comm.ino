#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include<LiquidCrystal.h>
String in;
#include<LiquidCrystal.h>
LiquidCrystal lcd(16,17,4,5,6,7);

const int trigPin = 14;
const int echoPin = 15;

const int tg = 9;
const int ec = 10;
int bz=8;

long duration;
int distance1=0;
int distance2=0;
int Speed=0;
int distance=0;

long drt;
int dst1=0;
int dst2=0;
int spd=0;
int dst=0;



#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
 
long lat,lon; // create variable for latitude and longitude object
 SoftwareSerial gpsSerial(3, 2); // create gps sensor connection
 SoftwareSerial xbeeSerial(11,12); //RX, TX
TinyGPS gps; // create gps object
 
void setup(){
   
  Serial.begin(9600); // connect serial
  gpsSerial.begin(9600); // connect gps sensor
  xbeeSerial.begin(9600);
  pinMode(bz,OUTPUT);
     lcd.begin(16,2);    
   lcd.setCursor(0,0);
   lcd.print("V2V Comunication");
   lcd.setCursor(0,1);
   lcd.print("Master vehicle");
  
pinMode(trigPin, OUTPUT); 
pinMode(echoPin, INPUT);
pinMode(tg, OUTPUT); 
pinMode(ec, INPUT);
    delay(3000);

    lcd.clear();

  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }

  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}
 
void loop(){
   gpsSerial.listen();
  
   
   while(gpsSerial.available()>0){                         
  if(gps.encode(gpsSerial.read())){
   
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
                        

    // **************************************************************************

                  sensors_event_t a, g, temp;
                  mpu.getEvent(&a, &g, &temp);
                
                  /* Print out the values */
                  Serial.print("Acceleration X: ");
                  Serial.print(a.acceleration.x);
                  Serial.print(", Y: ");
                  Serial.print(a.acceleration.y);
                  Serial.print(", Z: ");
                  Serial.print(a.acceleration.z);
                  Serial.println(" m/s^2");
                
                
                  Serial.print("Temperature: ");
                  Serial.print(temp.temperature);
                  Serial.println(" degC");
                
                  Serial.println("");
  
  //******************************************************************************

     
           gps.get_position(&lat,&lon);    
           String la=String(lat);
           String lo=String(lon);

              String lt= la.substring(0, 7);
              String lg=lo.substring(0,7);
              
        Serial.print("lt:");
        Serial.println(lt);

         Serial.print("lg:");
         Serial.println(lg);

        gpsSerial.stopListening();
      //--------------------------------------------------
      //---------------------------------------------------------
          xbeeSerial.listen();
               digitalWrite(bz,LOW);        
            distance1 = ultrasonicRead();       //Front data distance & Speed
            delay(1000);
            distance2 = ultrasonicRead();
            Speed = (distance2 - distance1)/1.0;
             if(Speed<0)
            {
              Speed=0;
            } 
              Serial.print("spd1 in cm/s  :"); 
            Serial.println(Speed);  
             
            dst1 = ultrasonicReadA(); 
            delay(1000);
            lcd.clear();
            dst2 = ultrasonicReadA();
            spd = (dst2 - dst1)/1.0;  
            if(spd<0)
            {
              spd=0;
            } 
            Serial.print("spd2 in cm/s  :"); 
            Serial.println(spd);                  // Back Data  dst&spd

        if(a.acceleration.x<0)
        {
          Serial.println("Accident");
        }

        if (distance<20)
        {
          Serial.println("Warning");
        }

        if(dst<20)
        {
          digitalWrite(bz,HIGH);
          delay(500);
          digitalWrite(bz,HIGH);
          delay(LOW);
          
        }
      lcd.setCursor(0,0);
      lcd.print("SF:");
      lcd.setCursor(3,0);
      lcd.print(Speed);

      lcd.setCursor(10,0);
      lcd.print("DF:");
      lcd.setCursor(13,0);
      lcd.print(distance);

      lcd.setCursor(0,1);
      lcd.print("SB:");
      lcd.setCursor(3,1);
      lcd.print(spd);

      lcd.setCursor(10,1);
      lcd.print("DB:");
      lcd.setCursor(13,1);
      lcd.print(dst);
                           
         
 String loc=la+"\t" +lo+"\t" +String(distance)+"\t" + String(Speed) +"\t" + String(a.acceleration.x)+"";
      
   xbeeSerial.println(loc);


  }    //if GPS
  }    // While GPS

  }    //loop


 float ultrasonicRead ()
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance= duration*0.034/2;
Serial.print("Distance in cm : ");
Serial.println(distance);
return distance;

}

float ultrasonicReadA ()
{
digitalWrite(tg, LOW);
delayMicroseconds(2);
digitalWrite(tg, HIGH);
delayMicroseconds(10);
digitalWrite(tg, LOW);
drt = pulseIn(ec, HIGH);
dst= drt*0.034/2;
Serial.print("dst in cm : ");
Serial.println(dst);
return dst;

}

 
