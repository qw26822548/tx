#include <Servo.h>
#include <ESP8266.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#define TX 10   //ESP8266 TX接3
#define RX 11  //ESP8266 RX接2
#define SSID "espin"    //這裡填入你家AP的SSID
#define PASSWORD "12345678"   //這裡填入你家AP的密碼
#define Rec "192.168.5.1"   //這裡填入你要連線的目標IP
SoftwareSerial esp8266(TX,RX); 
int I2C_Address = 0xA7 >> 1; // ADXL345 的 I2C 地址
int X0, X1, Y0, Y1, Z1, Z0,C=0;
char i;
float X,Y,Z;
#define DEBUG true

int val1,val2;   
Servo myservo1;
Servo myservo2;// 建立 Servo 物件，控制伺服馬達

void setup() {
  myservo1.attach(12); // 連接 digital 腳位 13
  myservo2.attach(13); // 連接 digital 腳位 12
 /*pinMode(escpin1, OUTPUT);
 pinMode(escpin2, OUTPUT);*/
  esp8266.begin(115200);  //軟體序列埠速率 (與硬體同步調整)
  Serial.begin(9600);    //硬體序列埠速率 (與軟體同步調整)
  Serial.println("SoftSerial to ESP8266 AT commands test ...");
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASSWORD;
  cmd+="\"";
  cmd+="\r\n";
  sendData("AT+RST\r\n",2000,DEBUG); //重啟ESP8266
  sendData("AT+CWMODE=3\r\n",100,DEBUG); // 設定為STA模式
  sendData(cmd,10000,DEBUG); // 取得連線(視AP連線效能作調整，預設7000毫秒)
  sendData("AT+CIFSR\r\n",100,DEBUG); // 獲得IP
  sendData("AT+CIPMUX=1\r\n",100,DEBUG); // 開啟多除連接模式
  sendData("AT+CIPSERVER=1,8080\r\n",100,DEBUG); // 設定連接埠為8080
  
 

  String cipStart="AT+CIPSTART=0,\"TCP\",\"";
      cipStart += Rec; 
      cipStart += "\",8080";
      cipStart +="\r\n";
      sendData(cipStart,10000,DEBUG); //延遲時間800毫秒可以自行調整   
  Wire.begin();  //初始化 I2C
  setReg(0x2D, 0xA); // (打開電源, 設定輸出資料速度為 100 Hz)  

  }

void loop() {
     
    X0 = getData(0x32); // 取得 X 軸 低位元資料
    X1 = getData(0x33); // 取得 X 軸 高位元資料
    X = ((X1 << 😎  + X0) / 256.0 ;
 
    Y0 = getData(0x34); // 取得 Y 軸 低位元資料
    Y1 = getData(0x35); // 取得 Y 軸 高位元資料
    Y = ((Y1 << 😎  + Y0) / 256.0;
 
    Z0 = getData(0x36); // 取得 Z 軸 低位元資料
    Z1 = getData(0x37); // 取得 Y 軸 高位元資料
    Z = ((Z1 << 😎  + Z0) / 256.0;
    
    /*String CIPSEND="AT+CIPSEND=0,8";
       CIPSEND +="\r\n";
      sendData(CIPSEND,200,DEBUG); //延遲時間800毫秒可以自行調整
      delay(1000);*/
    if(X>=0.5)
    { 
   String CIPSEND="AT+CIPSEND=0,8";
       CIPSEND +="\r\n";
      sendData(CIPSEND,500,DEBUG); //延遲時間800毫秒可以自行調整
     
   String ADXLX="X= ";
  ADXLX+= X;
  ADXLX +="\r\n";
  sendData(ADXLX,200,DEBUG); //延遲時間800毫秒可以自行調整
  
/* for(int a = 30; a <= 150; a++){
myservo1.write(a); // 控制馬達角度 30~150 度
delay(20);
}
for(int a = 150; a >= 30; a--){
myservo1.write(a); // 150~30 度
delay(20);
}*/
  /*val2 = map(1000, 0, 1023, 0, 179);  
  analogWrite(escpin2,val2);
  delay(15);  */
       
   
   
   
  
 
   }
   
  else if(X<=-0.5)
  {
   String CIPSEND="AT+CIPSEND=0,8";
       CIPSEND +="\r\n";
      sendData(CIPSEND,500,DEBUG); //延遲時間800毫秒可以自行調整
   
   
  String ADXLX="X= ";
  ADXLX+= X;
  ADXLX +="\r\n";
  sendData(ADXLX,200,DEBUG); //延遲時間800毫秒可以自行調整
 /* val1 = map(1000, 0, 1023, 0, 179);  
  analogWrite(escpin1,val1);
  delay(15);*/

/*for(int a = 30; a <= 150; a++){
myservo2.write(a); // 控制馬達角度 30~150 度
delay(20);
}
for(int a = 150; a >= 30; a--){
myservo2.write(a); // 150~30 度
delay(20);
}*/
   
   
    }
    
 else
  {
  String CIPSEND="AT+CIPSEND=0,8";
       CIPSEND +="\r\n";
      sendData(CIPSEND,500,DEBUG); //延遲時間800毫秒可以自行調整
  String ADXLZ="Z= ";
  ADXLZ+= Z;
  ADXLZ +="\r\n"; 

  sendData(ADXLZ,200,DEBUG); //延遲時間800毫秒可以自行調整
   
   
    }   


if (esp8266.available()) {   //若軟體序列埠 Rx 收到資料 (來自 ESP8266)
    Serial.write(esp8266.read());  //讀取後寫入硬體序列埠 Tx (PC)
    }
  if (Serial.available()) {  //若硬體序列埠 Rx 收到資料 (來自 PC)
    esp8266.write(Serial.read());  //讀取後寫入軟體序列埠 Tx (ESP8266)
    }
     
  }

String sendData(String command, const int timeout, boolean debug) {
  String response="";
  esp8266.print(command); // send the read character to the esp8266
  long int time=millis();
   do{
    while(esp8266.available()) {
      // The esp has data so display its output to the serial window
      char c=esp8266.read(); // read the next character.
      response += c;
      } 

   if(response.charAt(response.length()-2)=='O'&&response.charAt(response.length()-1)=='K'){
     time=0;
   }
    }while ((time+timeout) > millis());
  if (debug) {Serial.print(response);}
  return response;
  }
  //ADXL345
  void setReg(int reg, int data){
    Wire.beginTransmission(I2C_Address);
    Wire.write(reg); // 指定佔存器
    Wire.write(data); // 寫入資料
    Wire.endTransmission();
}

/* getData(reg)：取得佔存器裡的資料
 * 參數：reg → 佔存器位址
 */
int getData(int reg){
    Wire.beginTransmission(I2C_Address);
    Wire.write(reg);
    Wire.endTransmission();
    
    Wire.requestFrom(I2C_Address,1);
    
    if(Wire.available()<=1){
        return Wire.read();
    }
}
