#define MAX 128
//http://www.esacademy.com/en/library/technical-articles-and-documents/miscellaneous/i2c-bus/i2c-bus-events/
byte const sensorNr[]= {17,19};//{B00010001,B00010011}; //de adressen van de slaves, LSB moet 1 zijn, want we gaan enkel lezen van de slaves
int const aantalBytesSensor[]={4,8}; //sensor 1 heeft 4 byte, sensor 2 heeft 8 bytes 

int const maxAantalSensoren=2;//voor snelheid we contoleren dan enkel de eerste maxAantalSensoren geldige sensoren
int const defaultAantalByte=16; //master sampled defaultAantalByte indien adress niet in de aantalBytesSensor-array
int const LENGTEADRESS=8; //onze adressen zijn 8 bit lang
int const BYTELENGTE=8; //onze bytes zijn 8 bit lang
unsigned long const vijfMin=300000; //5*60*1000
byte waardeSensor1[]   = {B00000000,B00000000};
byte waardeSensor2[]={B00000000,B00000000,B00000000,B00000000};
#define SDA A4  //SDA Adress/data lijn
#define SCL A5  //SCL clock lijn
#define HOOG HIGH //indien toch actief hoog, dit wisselen
#define LAAG LOW  // dit ook 
#define SNELHEID 5  //1khz voor 8 bit ->  125ms voor 1 bit
                      //10% flank 
                      //90% om stabiel te blijven 
                      // elke functie gaat er van uit dat er al een delay er boven is gebeurd en zal eindigen met een delay(snelheid*1)


void startConditie(){
  Serial.println("STARTconditie");
  pinMode(SDA,OUTPUT);  
  digitalWrite(SDA,HOOG);
    delay(SNELHEID*1);
  digitalWrite(SCL,HOOG);
    delay(SNELHEID*1);
  digitalWrite(SDA,LAAG);
    delay(SNELHEID*1);
  digitalWrite(SCL,LAAG);
    delay(SNELHEID*1); 
}
void stopConditie(){// gaat er van uit dat beide SDA,SCL hoog zijn
  delay(SNELHEID*0.1);
  delay(SNELHEID*0.9);
  digitalWrite(SCL,HOOG);
  pinMode(SDA,OUTPUT);
  digitalWrite(SDA,LAAG);
    delay(SNELHEID*0.8);
  digitalWrite(SCL,HOOG);
    delay(SNELHEID*0.1);
    delay(SNELHEID*0.9);
  digitalWrite(SDA,HOOG);
    delay(SNELHEID*0.1);  
    delay(SNELHEID*0.8);
  digitalWrite(SCL,LAAG);
    delay(SNELHEID*0.1);
    delay(SNELHEID*0.9);
}

void sendAck(){
  Serial.println("sendAck");
  pinMode(SDA,OUTPUT);
  digitalWrite(SDA,LAAG);
  delay(SNELHEID*1.0);
  clk();
  delay(SNELHEID*0.05);
}
void sendNack(){
  Serial.println("NACK");
  pinMode(SDA,OUTPUT);
  digitalWrite(SDA,HOOG);
  delay(SNELHEID*1.0);
  clk();
  delay(SNELHEID*0.05);
}
void clk(){
  digitalWrite(SCL,HOOG);
    delay(SNELHEID*0.8);
  digitalWrite(SCL,LAAG);
    delay(SNELHEID*1);
}
boolean lissenForAck(){
  Serial.println("WAITING for ACK");
  pinMode(SDA,INPUT);
  delay(SNELHEID*1);  //slave adres laten controlleren
  float factor=2.0;
  int i=0;
  int debounce=5;
  unsigned long tijd=millis();
  digitalWrite(SCL,HOOG);
  while(millis()<tijd+(SNELHEID*factor)){  //debug
    if(digitalRead(SDA)==LAAG){
      debounce++;
    }else{
      debounce--;
    }
    i++;
    delay(SNELHEID*factor/10);
  }
  digitalWrite(SCL,LAAG);
  delay(SNELHEID*1);
  if (debounce>5){
    return true;
  }
  Serial.println("Timeout: geen ack gekregen");
  return false;
}
boolean sendAdress(byte adres){
  Serial.print("SEND ADRESS:");
  Serial.println(adres);
  pinMode(SDA,OUTPUT);
  int attemps=0;
  while(attemps<1){
    startConditie();
    for(int j=0;j<LENGTEADRESS;j++){//MSB eerst doorsturen
      if(bitRead(adres,(LENGTEADRESS-1)-j)==0){ 
        digitalWrite(SDA,LAAG);
        //Serial.print('0');
      }else{
        digitalWrite(SDA,HOOG);
        //Serial.print('1');
      }
      delay(SNELHEID*1);
      clk();
    }
    //Serial.println();
    if (lissenForAck()==false){
      Serial.print("adress ack ? poging ");
      Serial.println(attemps);
      attemps++;
    }else{
      Serial.println("adress ACk gekregen"); 
      return true;}  // success
  }
  Serial.println("TIMEOUT, geen adres ack");
  return false;  //failed
}
byte readByte(){
  pinMode(SDA,INPUT);
  byte data;
  boolean dataBit=false;
  for (int i=0; i<8;i++){
    digitalWrite(SCL,HOOG);
      delay(SNELHEID*1);
    if (digitalRead(SDA)==LAAG){
      dataBit=true;
    }else{dataBit==false;}
      delay(SNELHEID*0.05);
    digitalWrite(SCL,LAAG);
      delay(SNELHEID*1);
    bitWrite(data,(BYTELENGTE-1)-i,dataBit);  //MSB eerst
    Serial.print(dataBit);
  }
  Serial.print("=");
  delay(SNELHEID*1);
  return data;
}

void setup() {
  Serial.begin(9600); 
  pinMode(SDA,OUTPUT);
  pinMode(SCL,OUTPUT);
  digitalWrite(SDA,HOOG);  
  digitalWrite(SCL,HOOG);
  Serial.println("setup DONE");

}
void loop() {
  delay(2000);
  byte adres=1;
  int aantalBytes=defaultAantalByte;
 
  for(adres=17;;adres+=2){ //loop over alle adressen
    //Serial.println(adres);
    if (B00010000>adres){continue;}// alle adressen onder 16 en boven 239 zijn reserved
    if (adres>=B11110000){break;}
    if (adres>(maxAantalSensoren*2)+15){adres=15; continue;}
   /* for (int i=0;i<(sizeof(sensorNr)/sizeof(sensorNr[0]));i++){  //zoek of de sensor hardcoded is 
      if (adres==sensorNr[i]){                                 //
        aantalBytes=aantalBytesSensor[i];
        break;
      }
    }*/ 
 /* }
  for (int i=0;i<(sizeof(sensorNr)/sizeof(sensorNr[0]));i++){ //sizeof(sensorNr)/sizeof(sensorNr[0])   want sizeof geeft aantal bytes terug en we hebben length nodig 
    adres =sensorNr[i];
    aantalBytes=aantalBytesSensor[i];
*/ 
    if (sendAdress(adres)==false){
      Serial.println();
      stopConditie();
      continue;}  //reageert het adres?
    
    delay(SNELHEID*5);
    byte receivedBytes[aantalBytes];
    for (int j=0;j<aantalBytes;j++){
      Serial.print("READ Byte: ");
      Serial.print(j+1);
      Serial.print("/");
      Serial.println(aantalBytes);
      receivedBytes[j]=readByte();
      Serial.println(receivedBytes[j]);
      if((j+1)==aantalBytes){  //excentriek ideetje van de assambler programmeurs
        sendNack();
        stopConditie();
      }else{
        sendAck();  
      }
      delay(SNELHEID*5);
    
    
    }

    // TODO receivedBytes naar display sturen

  }
  /*unsigned long tijd=millis();
  while(millis()<(tijd+vijfMin)){
    delay(1000);  
  }*/
  
}
void lcd(char c){
  Serial.print(c);
}
