#include <SoftwareSerial.h>

#define LED 13
#define AUX 5
#define button_1 7
#define button_2 8
#define button_3 9
#define button_4 10
#define button_5 11

SoftwareSerial mySerial(2, 3); // RX, TX

const byte address[] = {0xEA,0x00,0x0D};
byte data[2] = {0x00,0x00}; 
byte bstate = 0;
byte bstate_1,bstate_2,bstate_3,bstate_4,bstate_5;

void waitAUX();
void sendData2Bytes (const byte* address, byte* data, int dataSize);
byte countOne(byte bstate);
void LedBlink();

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(AUX,INPUT);
  pinMode(LED,OUTPUT);
  pinMode(button_1,INPUT);
  pinMode(button_2,INPUT);
  pinMode(button_3,INPUT);
  pinMode(button_4,INPUT);
  pinMode(button_5,INPUT);
  waitAUX();
  Serial.println("Start.");
}

void loop() {
  digitalWrite(LED,!digitalRead(LED)); //以 LED 交替亮滅代表持續發送數據
  // ----- 讀取五個乾接點訊號 -----
  bstate_1 = !digitalRead(button_1);
  bstate_2 = !digitalRead(button_2);
  bstate_3 = !digitalRead(button_3);
  bstate_4 = !digitalRead(button_4);
  bstate_5 = !digitalRead(button_5);
  // ----- 將訊號進行合併，以利傳送 -----
  Serial.print("Button state : 0b");
  bstate = (bstate_5<<4) | (bstate_4<<3) | (bstate_3<<2) | (bstate_2<<1) | (bstate_1<<0);
  Serial.println( bstate , BIN );
  data[0] = bstate; //將合併後的訊號塞進去第一個 byte
  
  data[1] = countOne(bstate); //計算出已觸發的乾接點個數  
  
  sendData2Bytes(address,data, sizeof(data)/sizeof(data[0]) );

  delay(1500);
}

// ===== 等待 Lora 晶片不繁忙 =====
void waitAUX(){
  while(!digitalRead(AUX));
  delay(10);
}

// ===== sendData1Byte =====
void sendData2Bytes (const byte* address, byte* data, int dataSize){
  waitAUX();
  for(int i = 0; i < 3; i++){
    Serial.println(address[i]);
    mySerial.write(address[i]);
    delay(2);
  }
  for(int i = 0; i < dataSize; i++){
    Serial.println(data[i]);
    mySerial.write(data[i]);
    delay(2);
  }
  
  mySerial.flush();  
  waitAUX();
}

// ===== countOne =====
byte countOne(byte bstate){
  byte count = 0;
  
  for (int i = 0; i<5 ; i++)
    count += ( (bstate >> i)&0x01 );  

  return count;
}
// ===== LedBlink =====
void LedBlink(){
  digitalWrite(LED,HIGH);
  delay(100);
  digitalWrite(LED,LOW);
  delay(100);
  digitalWrite(LED,HIGH);
  delay(100);
  digitalWrite(LED,LOW);
  delay(100);
  digitalWrite(LED,HIGH);
  delay(100);
  digitalWrite(LED,LOW);
  delay(100);
  digitalWrite(LED,HIGH);
  delay(100);
  digitalWrite(LED,LOW);
  delay(100);
  digitalWrite(LED,HIGH);
  delay(100);
  digitalWrite(LED,LOW);
  delay(100);
}
