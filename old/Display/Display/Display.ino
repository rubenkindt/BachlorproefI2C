#define D7 13
#define D6 12
#define D5 11
#define D4 10
#define D3 9
#define D2 8
#define D1 7
#define D0 6
#define EN 5  //enable
#define RW 4  //R/W
#define RS 3  //Register Select
//#define V0 2 // bepaalt contrast
#define HOOG HIGH
#define LAAG LOW

void setData(byte b){
  digitalWrite(EN,HOOG);
  if (bitRead(b,0)==1){digitalWrite(D0,HOOG);}
  else{digitalWrite(D0,LAAG);}
  if (bitRead(b,1)==1){digitalWrite(D1,HOOG);}
  else{digitalWrite(D1,LAAG);}
  if (bitRead(b,2)==1){digitalWrite(D2,HOOG);}
  else{digitalWrite(D2,LAAG);}
  if (bitRead(b,3)==1){digitalWrite(D3,HOOG);}
  else{digitalWrite(D3,LAAG);}
  if (bitRead(b,4)==1){digitalWrite(D4,HOOG);}
  else{digitalWrite(D4,LAAG);}
  if (bitRead(b,5)==1){digitalWrite(D5,HOOG);}
  else{digitalWrite(D5,LAAG);}
  if (bitRead(b,6)==1){digitalWrite(D6,HOOG);}
  else{digitalWrite(D6,LAAG);}
  if (bitRead(b,7)==1){digitalWrite(D7,HOOG);}
  else{digitalWrite(D7,LAAG);}
  for (int i=7;i>=0;i--){
    Serial.print(bitRead(b,i));
  }
  Serial.println();
  delay(2);
  digitalWrite(EN,LAAG);
}
void setPinsToOutput(){
  pinMode(D7,OUTPUT);
  pinMode(D6,OUTPUT);
  pinMode(D5,OUTPUT);
  pinMode(D4,OUTPUT);
  pinMode(D3,OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(D1,OUTPUT);
  pinMode(D0,OUTPUT);
  pinMode(EN,OUTPUT);
  pinMode(RW,OUTPUT);
  pinMode(RS,OUTPUT);
  Serial.println("pinMode SET");
}
void clr(){
  digitalWrite(RS,LAAG);
  digitalWrite(RW,LAAG);
  setData(B00000001);
  Serial.println("clr screen");
}
void off(){
  digitalWrite(RS,LAAG);
  digitalWrite(RW,LAAG);
  setData(B00001100);
}

void setup() {
  Serial.begin(9600); 
  setPinsToOutput();
    delay(30);
  digitalWrite(RS,LAAG);
  digitalWrite(RW,LAAG);
  digitalWrite(EN,LAAG);
  setData(B00110000);   //FunctionSet
    delay(30);
  digitalWrite(RS,LAAG);
  digitalWrite(RW,LAAG);
  setData(B00110000);   //FunctionSet
    delay(30);
  digitalWrite(RS,LAAG);
  digitalWrite(RW,LAAG);
  setData(B00110000);   //FunctionSet
    delay(300);
  digitalWrite(RS,LAAG);
  digitalWrite(RW,LAAG);
  setData(B00110000);    //FunctionSet
  delay(30);
  off();
    delay(30);
  clr();
    delay(30);
  digitalWrite(RS,LAAG);
  digitalWrite(RW,LAAG);
  setData(B00000110);
  Serial.println("SetUp DONE");
    delay(50);
  //setData(B00000010); //return home
}
void writeAdress(byte b){
  delay(50);
  digitalWrite(RS,LAAG);
  digitalWrite(RW,LAAG);
  Serial.print("Adress: ");
  setData(b);
  
}
void dataRam(byte b){
  delay(50);
  digitalWrite(RS,HOOG);
  digitalWrite(RW,LAAG);
  Serial.print("Data: ");
  setData(b);
}
void printLetter(char a){
  
  if (  atoi("A")<=atoi(a) && atoi("Z")>=atoi("a") ){
    
  }
  
  
  
}



void loop() {
  writeAdress(B10000000); // 1,adress 00h=1-1
  delay(500);
  dataRam(B00110000);
  delay(500);
  dataRam(B00110001);
  delay(500);
  dataRam(B00110010);
  delay(500);
  dataRam(B00110011);
  delay(10000);
  
}
