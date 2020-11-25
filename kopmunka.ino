#include <dht11.h>                        //szükséges könyvtárak beimportálása
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 11                   //vízhőmérséklet mérő szenzor definiállása
#define DHT11_PIN 12                      //páratartalom és hőmérséklet mérő szenzor definiállása
dht11 DHT11;
LiquidCrystal_I2C lcd1(0x27,20,4);        //kijelzők definiállása   
LiquidCrystal_I2C lcd2(0x23,16,2);
OneWire oneWire(ONE_WIRE_BUS);            //vízhőmérséklet mérő szenzor funkciói                                                                                          
DallasTemperature sensors(&oneWire);          

int moisturenumber=A5;   //nedvesség érzékelő szenzor bekötési száma
int lightnumber=A4;      //fotoellenállás bekötési száma

                         //gombok értékét tároló változók
int gomb;                //sárga színű gomb
int gomb2;               //kék színű gomb
int gomb3;               //piros színű gomb

int gombnumber=7;       //gombok bekötési számai
int gomb2number=9;
int gomb3number=8;

int hangszoro=6;        //hangszóró bekötési száma
int hang=0;             //a hangszóró a program indulásakor ki van kapcsolva,így az értéke nulla

int green=52;           //zöld színű led bekötési száma
int red=53;             //piros színű led bekötési száma

int motor=10;           //relé bekötési száma


int allapot=0;          //gomb segítségével választom ki,hogy a nagy kijelzőn,mely szenzor értékek jelenjenek meg

int szenzorok=1;        //a program indulásakor,a szenzorok által történő vezérlés kapcsol be

int motorbekapcsolva=1; //ha a kézi vezérlésre váltunk,a motor kikapcsolt állapotba kerül,amit a sárga gomb segítségével tudunk ki és be kapcsolni
                        //hogyha a szenzorok olyan értékeket adnak vissza,amelyek nem felelnek meg az előre beálított paramétereknek,akkor a változóknak 1es értéke lesz

int fhiba;              //fotoellenállás hiba   
int phiba;              //páratartalom érzékelő szenzor hiba
int hhiba;              //hőmérséklet érzékelő szenzor hiba
int mhiba;              //nedvesség érzékelő szenzor hiba
int vhiba;              //víz hőmérséklet érzékelő szenzor hiba

int case_value;         //különböző hiba kombinációkra,különböző értékeket kap a változó




void hibak(){                      //külön void a hibákra
 
int a=case_value; //ha az a értéke a lefutás után,nemlesz egyenlő a case value értékével ez azt jelenti hogy a case value értéke megváltozott,igy cleareli a kijelzőt
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
  lcd1.init();                           //kijelzők bekapcsolása
  lcd1.backlight();
  lcd2.init();
  lcd2.backlight();                               
  sensors.begin();                      //víz hőmérséklet érzékelő szenzorhoz használt funkció
  pinMode(hangszoro,OUTPUT);              
  pinMode(gombnumber,INPUT_PULLUP);         
  pinMode(gomb2number,INPUT_PULLUP);         
  pinMode(gomb3number,INPUT_PULLUP);         
  pinMode(motor,OUTPUT);                //relé
  pinMode(green,OUTPUT);                    
  pinMode(red,OUTPUT);}


void loop(){
  gomb3=digitalRead(gomb3number);      //a piros színű gomb segítségével kapcsolok az automatikus és a kézi vezérlés között
  if (gomb3==0){
   lcd1.clear();                       //hogyha átkapcsolok a kijelzők kitörlik a rajtuk lévő adatokat
   lcd2.clear();
    if (szenzorok==0){
      szenzorok=1;}
    else if (szenzorok==1){
      szenzorok=0;                   
      delay(200);
      }}
  
  
  
  if(szenzorok==0){                  //amikor kézi vezérlésre van kapcsolva,akkor a szenzor értékekről nem vesz tudomást
  
  noTone(hangszoro);
  int motorvezerlogomb=digitalRead(gombnumber);   //sárga gombbal állítom, hogy a motor be legyen e kapcsolva
  lcd1.setCursor(0,0);
  lcd1.print("kezi vezerles");
  if (motorvezerlogomb==0){
    lcd2.clear();               
    if (motorbekapcsolva==0){
      motorbekapcsolva=1;}                        
    else if (motorbekapcsolva==1){
      motorbekapcsolva=0;
      delay(200);}}

  if (motorbekapcsolva==0){  
  digitalWrite(motor,LOW);
  lcd2.setCursor(0,0);
  lcd2.print("motor");
  lcd2.setCursor(0,1);
  lcd2.print("bekapcsolva");
  digitalWrite(red,LOW);
  digitalWrite(green,HIGH);
  
  
  }
  else if (motorbekapcsolva==1){
  digitalWrite(motor,HIGH);
  lcd2.setCursor(0,0);
  lcd2.print("motor");
  lcd2.setCursor(0,1);
  lcd2.print("kikapcsolva");
  digitalWrite(green,LOW);
  digitalWrite(red,HIGH);
  }}
  
  
  
  
  
  
  else if(szenzorok==1){             // amikor automatikus vezérlésre van kapcsolva
  motorbekapcsolva=1;                // a motrot automatikusan kikapcsolja
  int chk = DHT11.read(DHT11_PIN);
  sensors.requestTemperatures();
  float vizhomerseklet=sensors.getTempCByIndex(0);
  float homerseklet=DHT11.temperature;
  float paratartalom=DHT11.humidity;
  int moisture=analogRead(moisturenumber);                                   // adatok bekérése
  moisture=1023-moisture;                            //a moisture szenzorom csökkentette az értéket,ezután növelni kezdte.                                                  
  int light=analogRead(lightnumber);
  int gomb2=digitalRead(gomb2number);                   //hibaüzenetnél szól a hangszóró,a gomb lenyomásával némíthassuk
  if (gomb2==0){
    if (hang==0){
      hang=100;}
    else if (hang==100){
      hang=0;
      delay(200);}}
  
  if (light<980){                         // ha nem megfelelő az érték,akkor a hibánál 1es lesz
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
 
  if(vizhomerseklet<30.00){
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
    digitalWrite(green,LOW);
    digitalWrite(red,HIGH);
    break;
  
    case 1:            
   
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a homerseklet");
    lcd2.setCursor(0,1);
    lcd2.print(homerseklet);
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red,HIGH);
    break; 
  
    case 2:
   
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a paratartalom");
    lcd2.setCursor(0,1);
    lcd2.print(paratartalom);
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red,HIGH);
    break; 
  
    case 3:
    
    lcd2.setCursor(0,0);
    lcd2.print("Keves a vizt.");
    lcd2.setCursor(0,1);
    lcd2.print(moisture);
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,LOW);
    digitalWrite(red,LOW);
    digitalWrite(green,HIGH);
    break; 
  
    case 4:
    
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a vizho.");
    lcd2.setCursor(0,1);
    lcd2.print( vizhomerseklet);
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red,HIGH);
    break; 
  
    case 5:
   
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a homerseklet");
    lcd2.setCursor(0,1);
    lcd2.print("Nagy a paratartalom");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red,HIGH);
    break; 
  
    case 6:
    
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a feny");
    lcd2.setCursor(0,1);
    lcd2.print("Keves a vizt.");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red,HIGH);
    break; 
  
    case 7:
    
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a homerseklet");
    lcd2.setCursor(0,1);
    lcd2.print("Keves a vizt.");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red,HIGH);
    break; 
  
    case 8:
   
    lcd2.setCursor(0,0);
    lcd2.print("Keves a vizt.");
    lcd2.setCursor(0,1);
    lcd2.print("Nagy a paratartalom:");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red,HIGH);
    break; 
  
    case 9:
    
    lcd2.setCursor(0,0);
    lcd2.print("Keves a vizt.");
    lcd2.setCursor(0,1);
    lcd2.print("Nagy a vizho");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red,HIGH);
    break; 

    case 10:
    
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a feny");
    lcd2.setCursor(0,1);
    lcd2.print("Nagy a homerseklet");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red,HIGH);
    break; 
  
    case 11:
    
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a feny");
    lcd2.setCursor(0,1);
    lcd2.print("Nagy a paratartalom:");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red,HIGH);
    break; 
  
    case 12:
    
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a feny");
    lcd2.setCursor(0,1);
    lcd2.print("Nagy a vizho:");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red,HIGH);
    break; 
  
    case 13:
   
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a homerseklet");
    lcd2.setCursor(0,1);
    lcd2.print("Nagy a vizho");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red,HIGH);
    break; 
  
    case 14:
   
    lcd2.setCursor(0,0);
    lcd2.print("Nagy a vizho");
    lcd2.setCursor(0,1);
    lcd2.print("Nagy a paratartalom");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red,HIGH);
    break; 
  
    case 15:
   
    lcd2.setCursor(0,0);
    lcd2.print("Minden ertek");
    lcd2.setCursor(0,1);
    lcd2.print("megfelelo");
    noTone(hangszoro);
    digitalWrite(motor,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red,HIGH);
    break; 
    
    case 16:
    
    lcd2.setCursor(0,0);
    lcd2.print("2-nel tobb ertek");
    lcd2.setCursor(0,1);
    lcd2.print("nem megfelelo!");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red,HIGH);
    break; 
    }

  
  gomb=digitalRead(gombnumber);   //a sárga színű gombbal változtatom,hogy a nagy kijelző,mely szenzor értékeket mutassa
  if (gomb==0){
    lcd1.clear();
    if (allapot==0){
      allapot=1;}
    else if (allapot==1){
      allapot=0;}}
   
  
 
  if (allapot==0){     
   
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
  
  else if (allapot==1){
 
    lcd1.setCursor(0,0);
    lcd1.print("Viztartalom");
    lcd1.setCursor(12,0);
    lcd1.print(moisture);
    lcd1.setCursor(0,1);
    lcd1.print("Vizho");
    lcd1.setCursor(11,1);
    lcd1.print(vizhomerseklet);
    lcd1.setCursor(0,2);
    lcd1.print("Hangszoro");
    lcd1.setCursor(0,3);
    if (hang==0){
      lcd1.print("kikapcsolva");}
    else{
      lcd1.print("bekapcsolva");}
      }}}
