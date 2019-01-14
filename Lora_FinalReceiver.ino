#include <SoftwareSerial.h>

#define LED 13
#define output_1 7
#define output_2 8
#define output_3 9
#define output_4 10
#define output_5 11
#define AUX 5

SoftwareSerial mySerial(2, 3); // RX, TX
void waitAUX();
byte incoming2Byte[2]={0xFF,0xFF}; /* buffer */

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(AUX,INPUT);
  pinMode(LED,OUTPUT);
  pinMode(output_1,OUTPUT);
  pinMode(output_2,OUTPUT);
  pinMode(output_3,OUTPUT);
  pinMode(output_4,OUTPUT);
  pinMode(output_5,OUTPUT);
  /*因該繼電器模組為低電位觸發，為了不誤動作，在重開機時都先將繼電器關閉*/
  digitalWrite(output_1,HIGH);
  digitalWrite(output_2,HIGH);
  digitalWrite(output_3,HIGH);
  digitalWrite(output_4,HIGH);
  digitalWrite(output_5,HIGH);
  waitAUX();
  Serial.println("Start.");
}

void loop() {
  waitAUX();
  if (mySerial.available() > 0) {
    mySerial.readBytes(incoming2Byte, 2); /* 將 2bytes 資料餵入 buffer */   
    digitalWrite(LED,!digitalRead(LED));
    Serial.flush();
  }
  
  Serial.print("incoming2Byte[0] : ");
  Serial.print(incoming2Byte[0]); /* 第一個 byte 的數據 */
  Serial.print(" , incoming2Byte[1] : ");
  Serial.println(incoming2Byte[1]); /* 第二個 byte 的數據 */
  Serial.print("B1: "); Serial.println((~incoming2Byte[0]>>4)&0x01);
  Serial.print("B2: "); Serial.println((~incoming2Byte[0]>>3)&0x01);
  Serial.print("B3: "); Serial.println((~incoming2Byte[0]>>2)&0x01);
  Serial.print("B4: "); Serial.println((~incoming2Byte[0]>>1)&0x01);
  Serial.print("B5: "); Serial.println((~incoming2Byte[0]>>0)&0x01);
  digitalWrite(output_1, (~incoming2Byte[0]>>4)&0x01 );
  digitalWrite(output_2, (~incoming2Byte[0]>>3)&0x01 );
  digitalWrite(output_3, (~incoming2Byte[0]>>2)&0x01 );
  digitalWrite(output_4, (~incoming2Byte[0]>>1)&0x01 );
  digitalWrite(output_5, (~incoming2Byte[0]>>0)&0x01 );
}

// ===== 等待 Lora 晶片不繁忙 =====
void waitAUX(){
  while(!digitalRead(AUX));
  delay(10);
}
