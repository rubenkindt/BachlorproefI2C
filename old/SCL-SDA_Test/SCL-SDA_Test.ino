#include <Wire.h>
#define MAX 128
int slaveNr=1;
int werkende[MAX];

void werkt(int adress){
  static int j =0;
  static int backup=128;
  if (adress!=backup){
    werkende[j]=adress;
    j++;
    Serial.println("werkend adress: "+adress);
  }
  backup=adress;
}

void checkAdressen(){
  for(int adress=0;adress<MAX;adress++){
    Wire.requestFrom(adress,1);//(adress,#bytes)
    while(Wire.available()){
      char data = Wire.read();
      //Serial.print(data);
      werkt(adress);
      }
    }
    delay(500);
}
int aantalWerkendeAdressen(){
  int lengte=0;
  int i=0;
  while(werkende[i]!='\0'){
    lengte=i;
    i++;
  }
  return lengte;
}

void vulArray(int lengte,int vul[]) {
  for(int i=0;i<lengte;i++){
    vul[i]=werkende[i];
  }
  Serial.print("gevult");
}


void setup() {
  Wire.begin();
  Serial.begin(500);

  /*checkAdressen();
  int lengte=aantalWerkendeAdressen();
  Serial.print(lengte+" werkende adressen gevonden");
  int korter[lengte];
  vulArray(lengte,korter);
  */
}
  

void loop() {

  
  /*for(int i=0;i<127;i++){
    Wire.requestFrom(i,1);//(adress,#bytes)
    while(Wire.available()){
      char c = Wire.read();
      Serial.print(c);
    }
  */

  Wire.requestFrom(85,1);//(adress,#bytes)
    while(Wire.available()){
      char data = Wire.read();
    }
  //checkAdressen();
    delay(50);
  }
  


void lcd(char c){
  Serial.print(c);
  
}
