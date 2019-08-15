#include <SoftwareSerial.h>
#define TX 10   //ESP8266 TX接3
#define RX 11  //ESP8266 RX接2
#define SSID "espout"    //這裡填入你家AP的SSID
#define PASSWORD "12345678"   //這裡填入你家AP的密碼
#define Rec "192.168.5.2"   //這裡填入你要連線的目標IP
SoftwareSerial esp8266(TX,RX); 

#define DEBUG true
char k;
char cmd[8];
void setup() {
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
 
 
  sendData("AT+CIPMUX=1\r\n",100,DEBUG); // 開啟多除連接模式
  sendData("AT+CIPSERVER=1,8080\r\n",100,DEBUG); // 設定連接埠為8080
//AT+CIPAP_DEF="192.168.5.1","192.168.5.1","255.255.255.0"
 String CIPAP_DEF="AT++CIPAP_DEF=\"192.168.5.1\",\"192.168.5.1\",\"255.255.255.0\"";
 sendData(CIPAP_DEF,10000,DEBUG); //延遲時間800毫秒可以自行調整   
Serial.setTimeout(10); // 設定為每10毫秒結束一次讀取(數字愈小愈快)
  }

void loop() {
  
/*if(esp8266.available()){
 Serial.readBytes(cmd,8);
 Serial.println(atof(cmd));
}*/
String str=get_ESP8266_response();
if (str.indexOf("+") != -1){
Serial.println(str);
delay(200);
/*if(str=='1')
{
  
}*/

}
 /* if (esp8266.available()) {   //若軟體序列埠 Rx 收到資料 (來自 ESP8266)
    Serial.write(esp8266.read());//讀取後寫入硬體序列埠 Tx (PC)
  }
  if (Serial.available()) {  //若硬體序列埠 Rx 收到資料 (來自 PC)
    esp8266.write(Serial.read());  //讀取後寫入軟體序列埠 Tx (ESP8266)
    }*/
    /*if (esp8266.available()) {   //若軟體序列埠 Rx 收到資料 (來自 ESP8266)
    Serial.write(esp8266.read());  //讀取後寫入硬體序列埠 Tx (PC)
    }*/
get_ESP8266_response;

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

   if(response.charAt(response.length()-2)=='0'&&response.charAt(response.length()-1)=='K'){
     time=0;
   }
    }while ((time+timeout) > millis());
  if (debug) {Serial.print(response);}
  return response;
  }
  
String get_ESP8266_response() {  //取得 ESP8266 的回應字串
  String str="";  //儲存接收到的回應字串
  char c;  //儲存接收到的回應字元
  while (esp8266.available()) {  //若軟體序列埠接收緩衝器還有資料
    c=esp8266.read();  //必須放入宣告為 char 之變數 (才會轉成字元)
    str.concat(c);  //串接回應字元 
    delay(10);  //務必要延遲, 否則太快
    }
  str.trim();  //去除頭尾空白字元
  return str;
 
  }
