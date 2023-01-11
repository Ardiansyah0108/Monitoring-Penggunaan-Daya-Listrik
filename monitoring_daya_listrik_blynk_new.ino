/*     MONITORING DAYA LISTRIK
 * ILHAM YOGA WIBOWO   (3.32.20.2.11)
 * MUHAMMAD ARDIANSYAH (3.32.20.2.15)
 * KELAS : Ek3C
 */

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLf-fMuni_"
#define BLYNK_DEVICE_NAME "Monitoring Daya"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "uBA5or-tO2n6mjvVA5G8NKZ824SZ2aP9";
char ssid[] = "Khususon";
char pass[] = "aishakinanti";

#include <PZEM004Tv30.h>
PZEM004Tv30 pzem(12,13);  // 12=D6(TX) 13=D7(RX)

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27,16,2); //SDA(D2) SCL(D1)

float Power, 
      Energy, 
      Voltase, 
      Current, 
      Harga, 
      HargaRp, 
      Hargaa, 
      Energyy;

int Ampli = 0,  //D3 V5
    Kipas = 2,  //D4 V6
    Lampu = 14, //D5 V7
    Extra = 15; //D8 V8

BLYNK_WRITE(V5){                
  //Switch 1
  int pinValue = param.asInt();
  digitalWrite(Ampli,!pinValue);
}
BLYNK_WRITE(V6){                
  //Switch 2
  int pinValue = param.asInt();
  digitalWrite(Kipas,!pinValue);
}
BLYNK_WRITE(V7){                
  //Switch 3
  int pinValue = param.asInt();
  digitalWrite(Lampu,!pinValue);
}
BLYNK_WRITE(V8){                
  //Switch 4
  int pinValue = param.asInt();
  digitalWrite(Extra,!pinValue);
}

void setup(){
 Serial.begin(115200);

 pinMode(Ampli,OUTPUT);
 pinMode(Kipas,OUTPUT);
 pinMode(Lampu,OUTPUT);
 pinMode(Extra,OUTPUT);

 digitalWrite(Ampli,HIGH);
 digitalWrite(Kipas,HIGH);
 digitalWrite(Lampu,HIGH);
 digitalWrite(Extra,HIGH);

 lcd.begin();
 lcd.backlight();
 lcd.setCursor(0,0);
 lcd.print("   MONITORING   ");
 lcd.setCursor(0,1);
 lcd.print("  DAYA LISTRIK  ");
 delay(5000);
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("  EK3C POLINES  ");
 lcd.setCursor(0,1);
 lcd.print("ELEKTRONIKA 2022");
 delay(5000);
 lcd.clear();

 delay(100);
 Blynk.begin(auth, ssid, pass);
}

void loop(){
 Power = pzem.power();
 if(isnan(Power)){
   Serial.println("Gagal Membaca Power");
   lcd.setCursor(0,0);
   lcd.print("GAGAL MEMBACA   ");
   lcd.setCursor(0,1);
   lcd.print("                ");
 }
  else{
   Serial.print("Power : ");
   Serial.print(Power);
   Serial.println("W");
   }
 
 Energy = pzem.energy();
 if(isnan(Energy)){
   Serial.println("Gagal Membaca Energy");
   lcd.setCursor(0,0);
   lcd.print("GAGAL MEMBACA   ");
   lcd.setCursor(0,1);
   lcd.print("                ");
 }
  else{
   Serial.print("Energy : ");
   Energyy=Energy-0.63;//RESET ENERGY
   Serial.print(Energyy);
   Serial.println("kWh");
   lcd.setCursor(0,0);
   lcd.print("Daya :");
   lcd.print(Energyy);
   lcd.setCursor(11,0);
   lcd.print("kWh");
  }
  
 Voltase = pzem.voltage();
 if(isnan(Voltase)){
   Serial.println("Gagal Membaca Voltase");
   lcd.setCursor(0,0);
   lcd.print("GAGAL MEMBACA   ");
   lcd.setCursor(0,1);
   lcd.print("                "); 
 }
  else{
   Serial.print("Voltase : ");
   Serial.print(Voltase);
   Serial.println("V");
  }
 
 Current = pzem.current();
 if(isnan(Current)){
   Serial.println("Gagal Membaca Current");
   lcd.setCursor(0,0);
   lcd.print("GAGAL MEMBACA   ");
   lcd.setCursor(0,1);
   lcd.print("                ");
 }
  else{
   Serial.print("Current : ");
   Serial.print(Current);
   Serial.println("A");
  }

   Serial.print("Harga : Rp.");
   Serial.print(Hargaa);
   Harga=Energy*1352,00;//Harga perkWh PLN
   HargaRp=Harga-858.52;//RESET HARGA
   Hargaa=HargaRp;
   lcd.setCursor(0,1);
   lcd.print("Harga:Rp");
   lcd.print(Hargaa);
   Serial.println();

 //kirim data ke blynk
 Blynk.virtualWrite(V0,Power);
 Blynk.virtualWrite(V1,Energyy);
 Blynk.virtualWrite(V2,Voltase);
 Blynk.virtualWrite(V3,Current); 
 Blynk.virtualWrite(V4,Hargaa);
 
 Blynk.run();
} 
