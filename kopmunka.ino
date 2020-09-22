#include <dht11.h>                              //Be importálom a librariket.
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 11
#define DHT11_PIN 12
dht11 DHT11;
LiquidCrystal_I2C lcd1(0x27,20,4);
LiquidCrystal_I2C lcd2(0x23,16,2);



OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature. 

int moisturenumber=A5;    //szenzorok számai
int lightnumber=A4;
int gomb;
int gomb2;
int gomb3;
int gombnumber=7;        //gombok
int gomb2number=9;
int gomb3number=11;
int hangszoro=6;
int motor=10;
int fhiba;   //fény    // szenzorhibak,hogyha nem megfelelő értéket adnak vissza,akkor az értékük 1es lesz
int phiba;   //páratartalom
int hhiba;   //külső hőmérséklet
int mhiba;   // moisture,föld víztartalma
int vhiba;   // víz hőmérséklete 
int allapot=0; // Az lcd-én épp melyik értéket jelenítem meg,gombnyomással nő az értéke 2ig,után kinullázódik.Ha hibajelzés van nincs módunk tovább lépni.
int hang=100;  //Hangszóró értéke
int case_value;
int a;
int szenzorok;

void hibak(){
 
 a=case_value;
if(fhiba==1 && hhiba==0 && phiba==0 && mhiba==0 && vhiba==0){      
    case_value=0;
    }
  else if(fhiba==0 && hhiba==1 && phiba==0 && mhiba==0 && vhiba==0){            
    case_value=1;
     }
  
  else if(fhiba==0 && hhiba==0 && phiba==1 && mhiba==0 && vhiba==0){
    case_value=2;
     }
  
  else if(fhiba==0 && hhiba==0 && phiba==0 && mhiba==1 && vhiba==0){
    case_value=3;
     }
  
  else if(fhiba==0 && hhiba==0 && phiba==0 && mhiba==0 && vhiba==1){
    case_value=4;
    }
  
  else if(fhiba==0 && hhiba==1 && phiba==1 && mhiba==0 && vhiba==0){
    case_value=5;
     }
  
  else if(fhiba==1 && hhiba==0 && phiba==0 && mhiba==1 && vhiba==0){
    case_value=6;
     }
  
  else if(fhiba==0 && hhiba==1 && phiba==0 && mhiba==1 && vhiba==0){
    case_value=7;
     }
  
  else if(fhiba==0 && hhiba==0 && phiba==1 && mhiba==1 && vhiba==0){
    case_value=8;
    }
 
  else if(fhiba==0 && hhiba==0 && phiba==0 && mhiba==1 && vhiba==1){
    case_value=9;
     }

  else if(fhiba==1 && hhiba==1 && phiba==0 && mhiba==0 && vhiba==0){
    case_value=10;
     }
  
  else if(fhiba==1 && hhiba==0 && phiba==1 && mhiba==0 && vhiba==0){
    case_value=11;
    }
  
 else if(fhiba==1 && hhiba==0 && phiba==0 && mhiba==0 && vhiba==1){
    case_value=12;}
  
 else if(fhiba==0 && hhiba==1 && phiba==0 && mhiba==0 && vhiba==1){
    case_value=13;}
  
 else if(fhiba==0 && hhiba==0 && phiba==1 && mhiba==0 && vhiba==1){
    case_value=14;}
  
  else if(fhiba==0 && hhiba==0 && phiba==0 && mhiba==0 && vhiba==0){  
    case_value=15;}
  
  else{                                          
    case_value=16;}
  if(case_value!=a){
    lcd2.clear();}
  }




void setup(){
  lcd1.init();
  lcd1.backlight();
  lcd2.init();
  lcd2.backlight();               
  Serial.begin(9600);
  sensors.begin();   
  pinMode(hangszoro,OUTPUT);                //hangszóró
  pinMode(gomb3number,INPUT_PULLUP);
  pinMode(gomb2number,INPUT_PULLUP);         //hang gomb
  pinMode(gombnumber,INPUT_PULLUP);         //lcd változtatása
  pinMode(motor,OUTPUT);}              //relé


void loop(){
  gomb3=digitalRead(gomb3number);   //a gombbal variálom hogy a szenzorok be e legyenek kapcsolva
  if (gomb3==0){
    if (szenzorok==0){
      szenzorok=1;}
    else if (szenzorok==1){
      szenzorok=0;                   //ha kezi vezerlesen van torolje ki az lcdeken levo adatokat,es ne szoljon a hangszoro
      lcd1.clear();
      lcd2.clear();
      noTone(hangszoro);}}
  
  
  
  if (szenzorok==0){
  int motorvezerlogomb=digitalRead(gombnumber);   //a gombbal variálom hogy a motor be  e legyen kapcsolva
  int motorbekapcsolva;
  lcd1.write("kezi vezerles");
  if (motorvezerlogomb==0){
    lcd2.clear();                   // mivel atlesz irv az lcd hogy menjen e vagy sem
    if (motorbekapcsolva==0){
      motorbekapcsolva=1;}          // hogy a motor be e van kapcsolva,gombbal aligatom az ahhoz tartozo variablet
    else if (motorbekapcsolva==1){
      motorbekapcsolva=0;}}

  switch (motorbekapcsolva){  
  case 0:
  digitalWrite(motor,LOW);
  lcd2.write("motor bekapcsolva");
  break;
  case 1:
  digitalWrite(motor,HIGH);
  lcd2.write("motor kikapcsolva");
  break;
  }}
  else if (szenzorok==1){             // mejnenek a szenzorok
  int chk = DHT11.read(DHT11_PIN);
  sensors.requestTemperatures();
  float vizh=sensors.getTempCByIndex(0);
  float homerseklet=DHT11.temperature;
  float paratartalom=DHT11.humidity;
  int moisture=analogRead(moisturenumber);                                   // adatok bekérése
  moisture=1023-moisture;                                                                                
  int light=analogRead(lightnumber);
  light=1023-light;        // a light resistorjaim fény hatására csökkentették az értéküket,ezután növelni kezdik.
  int gomb2=digitalRead(gomb2number);                   //hibaüzenetnél szól a hangszóró,a gomb lenyomásával némíthassuk
  if (gomb2==0){
    if (hang==0){
      hang=100;}
    else if (hang==100){
      hang=0;}}
  
  if (light<900){                         // ha nem megfelelő az érték,akkor a hibánál 1es lesz
    fhiba=0;}
  else{fhiba=1;}

  if(homerseklet<32.00){
    hhiba=0;}
  else{hhiba=1;}
  
  if(paratartalom<75.00){
    phiba=0;}
  else{phiba=1;}

  if (moisture>150){
    mhiba=0;}
  else{mhiba=1;}  
 
  if(vizh<30.00){
    vhiba=0;}
  else{vhiba=1;}
  
  
  hibak();
  
  switch(case_value){
   
    case 0:
    
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a feny");
    lcd2.setCursor(0,1);
    lcd2.print(light);
    if (hang==0){noTone(hangszoro);}          
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);               
   
    break;
  
    case 1:            
   
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a homerseklet");
    lcd2.setCursor(0,1);
    lcd2.print(homerseklet);
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
  
     break; 
  
    case 2:
   
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a paratartalom");
    lcd2.setCursor(0,1);
    lcd2.print(paratartalom);
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
   
     break; 
  
  
    case 3:
    
    lcd2.setCursor(0,0);
    lcd2.print("Keves a vizt.");
    lcd2.setCursor(0,1);
    lcd2.print(moisture);
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,LOW);
    
     break; 
  
    case 4:
    
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a vizho");
    lcd2.setCursor(0,1);
    lcd2.print(vizh);
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
   
     break; 
  
    case 5:
   
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a homerseklet");
    lcd2.setCursor(0,1);
    lcd2.print("Nagy a paratartalom");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
     
     break; 
  
    case 6:
    
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a feny");
    lcd2.setCursor(0,1);
    lcd2.print("Keves a vizt.");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
    
     break; 
  
    case 7:
    
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a homerseklet");
    lcd2.setCursor(0,1);
    lcd2.print("Keves a vizt.");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
   
     break; 
  
    case 8:
   
    lcd2.setCursor(0,0);
    lcd2.print("Keves a vizt.");
    lcd2.setCursor(0,1);
    lcd2.print("Nagy a paratartalom:");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
  
     break; 
  
    case 9:
    
    lcd2.setCursor(0,0);
    lcd2.print("Keves a vizt.");
    lcd2.setCursor(0,1);
    lcd2.print("Nagy a vizho");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
     
     break; 

    case 10:
    
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a feny");
    lcd2.setCursor(0,1);
    lcd2.print("Nagy a homerseklet");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
   
    break; 
  
    case 11:
    
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a feny");
    lcd2.setCursor(0,1);
    lcd2.print("Nagy a paratartalom:");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
   
    break; 
  
    case 12:
    
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a feny");
    lcd2.setCursor(0,1);
    lcd2.print("Nagy a vizho:");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
 
    break; 
  
    case 13:
   
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a homerseklet");
    lcd2.setCursor(0,1);
    lcd2.print("Nagy a vizho");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
   
    break; 
  
    case 14:
   
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a vizho");
    lcd2.setCursor(0,1);
    lcd2.print("Nagy a paratartalom");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
    
     break; 
  
    case 15:
   
    lcd2.setCursor(0,0);
    lcd2.print("Minden ertek");
    lcd2.setCursor(0,1);
    lcd2.print("megfelelo");
    noTone(hangszoro);
    digitalWrite(motor,HIGH);
     
     break; 
    
    case 16:
    
    lcd2.setCursor(0,0);
    lcd2.print("2-nel tobb ertek");
    lcd2.setCursor(0,1);
    lcd2.print("nem megfelelo!");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,LOW);
   
    break; 
    
  }

  
int  gomb=digitalRead(gombnumber);   //a gombbal variálom az állapotokat.
  if (gomb==0){
    lcd1.clear();
    if (allapot==0){
      allapot=1;}
    else if (allapot==1){
      allapot=0;}}
   
  
 
  if (allapot==0 && fhiba==0 && hhiba==0 && phiba==0 && mhiba==0 && vhiba==0){     //csak akkor érvényesül ha nincs szükség a hibajelzésre.
   
   lcd1.setCursor(0,0); 
   lcd1.print("Temp:");
   lcd1.setCursor(9,0);
   lcd1.print(homerseklet);
   lcd1.print((char)223);
   lcd1.print("C");
   lcd1.setCursor(0,1);
   lcd1.print("Humidity:");
   lcd1.print(paratartalom);
   lcd1.print("%");
   lcd1.setCursor(0,2);
   lcd1.print("Fenyerosseg:");
   lcd1.setCursor(0,3);
   lcd1.print(light);}
  
  else if (allapot==1 && fhiba==0 && hhiba==0 && phiba==0 && mhiba==0 && vhiba==0){
 
    lcd1.setCursor(0,0);
    lcd1.print("Viztartalom");
    lcd1.setCursor(12,0);
    lcd1.print(moisture);
    lcd1.setCursor(0,1);
    lcd1.print("Vizho");
    lcd1.setCursor(11,1);
    lcd1.print(vizh);}}}
  
 
