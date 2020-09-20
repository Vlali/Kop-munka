#include <dht11.h>                              //Be importálom a librariket.
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 11
#define DHT11_PIN 12
dht11 DHT11;
LiquidCrystal_I2C lcd(0x27,16,2);

OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature. 

int moisturenumber=A5;    //szenzorok számai
int lightnumber=A4;
int gombnumber=7;        //gombok
int gomb2number=9;
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


void hibak(){
if(fhiba==1 && hhiba==0 && phiba==0 && mhiba==0 && vhiba==0){      
    case_value=0;}
  else if(fhiba==0 && hhiba==1 && phiba==0 && mhiba==0 && vhiba==0){            
    case_value=1;}
  
  else if(fhiba==0 && hhiba==0 && phiba==1 && mhiba==0 && vhiba==0){
    case_value=2;}
  
  else if(fhiba==0 && hhiba==0 && phiba==0 && mhiba==1 && vhiba==0){
    case_value=3;}
  
  else if(fhiba==0 && hhiba==0 && phiba==0 && mhiba==0 && vhiba==1){
    case_value=4;}
  
  else if(fhiba==0 && hhiba==1 && phiba==1 && mhiba==0 && vhiba==0){
    case_value=5;}
  
  else if(fhiba==1 && hhiba==0 && phiba==0 && mhiba==1 && vhiba==0){
    case_value=6;}
  
  else if(fhiba==0 && hhiba==1 && phiba==0 && mhiba==1 && vhiba==0){
    case_value=7;}
  
  else if(fhiba==0 && hhiba==0 && phiba==1 && mhiba==1 && vhiba==0){
    case_value=8;}
 
  else if(fhiba==0 && hhiba==0 && phiba==0 && mhiba==1 && vhiba==1){
    case_value=9;}

  else if(fhiba==1 && hhiba==1 && phiba==0 && mhiba==0 && vhiba==0){
    case_value=10;}
  
  else if(fhiba==1 && hhiba==0 && phiba==1 && mhiba==0 && vhiba==0){
    case_value=11;}
  
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
}



void setup(){
  lcd.init();
  lcd.backlight();               
  Serial.begin(9600);
  sensors.begin();   
  pinMode(hangszoro,OUTPUT);                //hangszóró
  pinMode(gomb2number,INPUT_PULLUP);         //hang gomb
  pinMode(gombnumber,INPUT_PULLUP);         //lcd változtatása
  pinMode(motor,OUTPUT);}              //relé


void loop(){
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

  if (moisture>100){
    mhiba=0;}
  else{mhiba=1;}  
 
  if(vizh<30.00){
    vhiba=0;}
  else{vhiba=1;}
  
  
  hibak();
  
  switch(case_value){
    case 0:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Nagy a feny");
    lcd.setCursor(0,1);
    lcd.print(light);
    if (hang==0){noTone(hangszoro);}          
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);               
    break;
  
    case 1:            
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Nagy a homerseklet");
    lcd.setCursor(0,1);
    lcd.print(homerseklet);
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
     break; 
  
    case 2:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Nagy a paratartalom");
    lcd.setCursor(0,1);
    lcd.print(paratartalom);
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
     break; 
  
  
    case 3:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Keves a vizt.");
    lcd.setCursor(0,1);
    lcd.print(moisture);
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,LOW);
     break; 
  
    case 4:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Nagy a vizho");
    lcd.setCursor(0,1);
    lcd.print(vizh);
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
     break; 
  
    case 5:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Nagy a homerseklet");
    lcd.setCursor(0,1);
    lcd.print("Nagy a paratartalom");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
     break; 
  
    case 6:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Nagy a feny");
    lcd.setCursor(0,1);
    lcd.print("Keves a vizt.");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
     break; 
  
    case 7:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Nagy a homerseklet");
    lcd.setCursor(0,1);
    lcd.print("Keves a vizt.");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
     break; 
  
    case 8:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Keves a vizt.");
    lcd.setCursor(0,1);
    lcd.print("Nagy a paratartalom:");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
     break; 
  
    case 9:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Keves a vizt.");
    lcd.setCursor(0,1);
    lcd.print("Nagy a vizho");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
     break; 

    case 10:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Nagy a feny");
    lcd.setCursor(0,1);
    lcd.print("Nagy a homerseklet");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
    break; 
  
    case 11:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Nagy a feny");
    lcd.setCursor(0,1);
    lcd.print("Nagy a paratartalom:");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
    break; 
  
    case 12:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Nagy a feny");
    lcd.setCursor(0,1);
    lcd.print("Nagy a vizho:");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
    break; 
  
    case 13:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Nagy a homerseklet");
    lcd.setCursor(0,1);
    lcd.print("Nagy a vizho");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
    break; 
  
    case 14:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Nagy a vizho");
    lcd.setCursor(0,1);
    lcd.print("Nagy a paratartalom");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,HIGH);
     break; 
  
    case 15:
    noTone(hangszoro);
    digitalWrite(motor,HIGH);
     break; 
    
    case 16:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("2-nel tobb ertek");
    lcd.setCursor(0,1);
    lcd.print("nem megfelelo!");
    if (hang==0){noTone(hangszoro);}
    else{tone(hangszoro,hang);}
    digitalWrite(motor,LOW);
    break; 
  }

  
int  gomb=digitalRead(gombnumber);   //a gombbal variálom az állapotokat.
  if (gomb==0){
    if (allapot==0){
      allapot=1;}
    else if (allapot==1){
      allapot=2;}
    else if (allapot==2){
      allapot=3;}
    else if (allapot==3){
      allapot=0;}}
  
 
  if (allapot==0 && fhiba==0 && hhiba==0 && phiba==0 && mhiba==0 && vhiba==0){     //csak akkor érvényesül ha nincs szükség a hibajelzésre.
   lcd.clear();
   lcd.setCursor(0,0); 
   lcd.print("Temp:");
   lcd.setCursor(9,0);
   lcd.print(homerseklet);
   lcd.print((char)223);
   lcd.print("C");
   lcd.setCursor(0,1);
   lcd.print("Humidity:");
   lcd.print(paratartalom);
   lcd.print("%");
   delay(20);}
  
  if (allapot==1 && fhiba==0 && hhiba==0 && phiba==0 && mhiba==0 && vhiba==0){
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Fenyerosseg:");
   lcd.setCursor(0,1);
   lcd.print(light);
   delay(20);}

  if (allapot==2 && fhiba==0 && hhiba==0 && phiba==0 && mhiba==0 && vhiba==0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Viztartalom");
    lcd.setCursor(12,0);
    lcd.print(moisture);
    lcd.setCursor(0,1);
    lcd.print("Vizho");
    lcd.setCursor(11,1);
    lcd.print(vizh);
    delay(20);}}
 
