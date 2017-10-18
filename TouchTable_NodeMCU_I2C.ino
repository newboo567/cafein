#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <string.h>
#include <stdlib.h>
#define SLAVE 8
#define MAXSECTOR 25
#define MINSECTOR 1
#define DEBUG true
#define SIZE 15
const char* ssid = "please";
const char* password = "00009999";
const char* host = "203.255.177.171"; //공유기의 주소를 입력.
char streamId[50]; //php 이름
int temp[11];
boolean isFirstTime = true;
//받은 데이터를 저장할 배열
int arr[SIZE];  //전송받을 데이터의 개수보다 일부러 더 크게 설정함.
boolean isRealOrder = false;
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  //D1 -> SCL
  //D2 -> SDA
  // Wire.begin(SLAVE); //join i2c bus with address 0x5C
  Wire.begin(8); //join i2c bus with address 0x5C
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  setupMDNS();
}

////////////////////////////////////////////////////////////////////////////////////////
void setupMDNS()
{
  //Call mdns to setup mdns to pint to domain.local
  if (!MDNS.begin("thing"))
  {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
}

//////////////////////////////////////////////////////////////////////////////////////

void loop() {
  chair();
  table();
  SendOrderReadySignal();
}

int menuSize = 10;
void table() {
  int arr[11];
  boolean isDup = false;
  Wire.requestFrom(8, 11);    // request 6 bytes from slave device #8
  int i = 0;
  while (Wire.available()) { // slave may send less than requested
    arr[i] = (int)Wire.read(); // receive a byte as character
    i++;
  }
  //판별
  if (arr[0] == 7) {
    if (isFirstTime == true) {
      for (int i = 0; i < SIZE; i++) {
        temp[i] = arr[i]; //백업
      }
      //보낸다.
      sendTableRequest(arr, menuSize);
      isFirstTime = false;
    }//isFirsttime ture check
    else { // isFirsttime == false
      for (int i = 1; i <= menuSize; i++) {
        if (arr[i] == temp[i] && (arr[i] > 0 && arr[i] <= 70)) {
          isDup = true;
          break;
        }
      }//for loop end
      if (isDup == true) {  //중복이다.
        Serial.println("dup - return");
        return; //안해
      }
      else {  //중복아니다(= 처음 보내지는 주문) -> 일단 보낸다.
        //테이블 정보 수신
        sendTableRequest(arr, menuSize);
      }
    }//isFirsttime == false
  }//테이블 번호 확인 if(테이블 번호있는 겨웅) 끝
  else { //거의 맨 마지막과도 같다.(arr[0] <= 0)
    //isRealOrder = false;
    for (int i = 0; i < 11; i++) {
      temp[i] = 255; //원상태로
    }
  }
  delay(1000);
}

void chair() {
  int arr[3];
  int chair;  //의자 좌석번호
  int isSeat; //앉았는지 체크
  Wire.requestFrom(8, 3);    // request 6 bytes from slave device #8
  int i = 0;
  while (Wire.available()) { // slave may send less than requested
    arr[i] = (int)Wire.read(); // receive a byte as character
    Serial.print("Got Cha!!  ");
    i++;
  }
  if (arr[0] >= 100) {
    if (arr[0] == 100) {
      //앉음
      chair = arr[1];
      isSeat = 1;
    }
    else if (arr[0] == 200) {
      chair = arr[1];
      isSeat = 0;
    }
    sendChairRequest(chair, isSeat);
  }

  delay(1000);
}

///////////////////////////////////////////////////////

void sendTableRequest(int * arr, int menuSize) {
  int count;
  Serial.println("I'm comming!!");

  strcpy(streamId, "receivedata.php");
  Serial.print("connecting to ");
  Serial.println(host);

  for (int i = 1; i <= menuSize; i++) {
    if (arr[i] <= 0 || arr[i] > 70) {
      count++;
    }
  }
  if (count == menuSize)
    return;

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  Serial.println("Client declaration");
  const int port = 8888;  //포트포워딩을 위한 포트
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  Serial.println("Client connetion ok");
  // We now create a URI for the request
  String url = "/";
  url += streamId;
  url += "?table=";
  url += arr[0];
  url += "&data=";

  //메뉴번호 유호성 검사
  for (int i = 1; arr[i] != 0 && arr[i] != 255; i++) {
    if (arr[i] >= 1 && arr[i] <= 70) { //메뉴번호에 해당되는 지 체크
      url += arr[i];
      if (arr[i + 1] != 0 && arr[i + 1] != 255)
        url += ",";
    }
  }//for loop end

  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println("closing connection");
}


//////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////자리 정보 전송함수///////////////////////////////////

void sendChairRequest(int chair, int isSeat) {
  strcpy(streamId, "receiveChair.php");
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int port = 8888;  //포트포워딩을 위한 포트
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/";
  url += streamId;
  url += "?seat=";
  url += chair;
  url += "&check=";
  url += isSeat;


  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println("closing connection");
}


///////////////////////////////////////////////////////////////////////////////////////
////I2C통신으로 주문이 완료된 경우 보드에게 전송한다.
void SendOrderReadySignal() {
  int table;
  // Check if connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  //Read the first line of the req.
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  if (req.indexOf("/led/7") != -1)
    table = 7;

  if (table == 7) {
    Wire.beginTransmission(8); // transmit to device #8
    Wire.write((byte)table);              // sends one byte
    Wire.endTransmission();    // stop transmitting
    Serial.println("Send Data!");
  }
}

