// compiler error handling
#include "Compiler_Errors.h"
#include <time.h> //make random number
#include <stdlib.h>
#include <string.h>
// touch includes
#include <MPR121.h>
#define MPR121_ADDR 0x5C
#define MPR121_INT 4
// mp3 includes
#include <SdFat.h>
#include <SFEMP3Shield.h>
// mp3 variables
SFEMP3Shield MP3player;
byte result;

SdFat sd;
SdFile file;

int inittouch[12];
int touchval[12];
//new v2 global variable
///////////////////////////터치 섹터 변수 관련 리스트//////////////////////
int sector = -1; //왼쪽 위 부터시작해서 왼->오 로 뻗어나가 0, 1, 2, 3, 4,.....
//만약 터치가 감지되지 않았다면 -1 값을 가진다.
///////////////////////////터치 인식 설정///////////////////////////////
int touched[12];    //터치 감지를 기록하는 배열

///////////////////////////메뉴얼 관련 번호 리스트////////////////////////////
boolean isDup = false;
boolean powerOn = true;  //전원 on/off감지
//true 이면 on, false이면 off
boolean isHomeSelect = false; //홈버튼이 눌렸는지 감지
boolean isReplay = false;
enum ModeList { //테이블 상황과 관련된 상수
  INIT = 1, MUSICMODE, GAMEMODE, ORDERMODE, //1, 2, 3, 4
  MUSIC_SADMODE, MUSIC_CHEERMODE, MUSIC_CALMMODE, //5, 6, 7
  OXQUIZMODE, RSPMODE, ISJSMODE, BACMODE, //8, 9, 10, 11
  OX_ENTERTAINMODE, OX_NATUREMODE, OX_MOVIEMODE //12, 13, 14
};
enum TrackTypeList {
  GUIDE = 0, SELECT
};
enum PathChangeList {
  ADD, DEL
};
enum RSPList {
  SCISSORS = 1, ROCK, PAPER
};

//테이블의 현재 상태를 나타냄. 가질 수 있는 값은 ModeList들에 있는 값을 따름.
//경우에 따라서 0으로 아무 상태도 아님을 나타내는 걸 고려할 것
int mainMode = INIT;
int subMode = 0;
int oxSubMode = 0;
int currentMode;
boolean isFirstTime = true; //처음으로 실행하는 단계인지 판별하는 변수
boolean wasCancelled = false;  //사용자가 취소버튼을 눌렀는지 확인하는 변수

/////////////////////////////////음악관련 전역변수////////////////////////////////////
boolean isMusPlay;
///////////////////////////음성 경로 관련 변수/////////////////////////////////
char path[37] = "/basic";  //경로 저장 문자열
char numberPath[] = "/number" ;  //숫자 음성 저장된 path -> 일단 디렉토리까지만
char dirName[10], fileName[13];
boolean canSkip = false; //재생이 끝날 때까지 기다려야하는 경우 false;
////////////////////////////섹터 판별 관련 전처리기 지정//////////////////////////////
#define ISMANUALBUT (sector <= 5 && sector != -1)
/////////////////////////////의자 관련 변수/////////////////////////////////////
byte isSeat; //앉았는지 체크하는 변수

//////////////////////////////주문 관련 변수//////////////////////////
#define MENULISTSIZE 16
#define TABLE 7
boolean isOrderFin = false; //주문완료 되었음을 나타내는 플래그
int add = 0, temp = 0, k = 1;  //k = index 역할  //add -> 페이지와 관련있는 변수
byte arr[15] = {0}; //테이블 번호 1개 + 주문 번호 10개 + 의자 2개*2
boolean canTakeOut = false;
int eventDrink;
////////////////////////사용 함수 선언/////////////////////////////
void ActMainFunc();
void SendList();
void PlayGuideTrack(int);
void OpenFileAndPlayTrack(char * trackPath);
void OperateManualBut();
void MusicMode();
void OrderMode();
void ChangeDir(char * dir, int mod);
void ChangeAllPath(char * newPath);
void RSPGame();
void printPath();
void OpenFileAndPlaySound(boolean canSkip);
void SetFileName(char * trackName);
void SetDirName(char * newDirName);
void FindSector();
void printFileName();
void MakeRanTrack(int list[], int listNum);
void printFlag();
void mkNameAndSetFile(char * word, int num, char * numbuf);
void BACGame();
void PlayOrderReadyTrack();
void receiveOrderSig(int howMany);
void GameEvent();
void initArr();
////////////////////////////코드 시작///////////////////////////////////

void setup() {
  Serial.begin(57600);
  powerOn = true; //전원은 처음엔 항상 ON
  pinMode(LED_BUILTIN, OUTPUT);

  //while (!Serial) ; {} //uncomment when using the serial monitor
  //Serial.println("Bare Conductive Proximity MP3 player");

  if (!sd.begin(SD_SEL, SPI_HALF_SPEED)) sd.initErrorHalt();

  if (!MPR121.begin(MPR121_ADDR))
    Serial.println("error setting up MPR121");
  MPR121.setInterruptPin(MPR121_INT);

  MPR121.setRegister(MPR121_USL, 202);
  MPR121.setRegister(MPR121_TL, 182);
  MPR121.setRegister(MPR121_LSL, 131);
  MPR121.setRegister(MPR121_ACCR0, 11);
  delay(9000);

  for (int i = 0; i < 12; i++) {
    //  Serial.print("pin ");
    //  Serial.print(i);
    //  Serial.print(":");
    inittouch[i] = (unsigned int)MPR121.getRegister(MPR121_E0FDL + 2 * i) + ((unsigned int)MPR121.getRegister(MPR121_E0FDH + 2 * i) << 8);

    //  Serial.println(inittouch[i]);
  }

  result = MP3player.begin();
  MP3player.setVolume(10, 10);  //볼륨은 나중에 다시 조절하기

  if (result != 0) {
    //Serial.print("Error code: ");
    //Serial.print(result);
    //Serial.println(" when trying to start MP3 player");
  }

  //인터럽트
  //attachInterrupt(digitalPinToInterrupt(2), PlayOrderReadyTrack, RISING);

  //rand()함수가 제대로 동작하게 하기 위해
  srand(time(NULL));

  ///I2C통신
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(SendList); // register event
  Wire.onReceive(receiveOrderSig); //receive event
  SetDirName("basic");
  sd.chdir(path, O_READ);
  SetFileName("welcome.mp3");
  OpenFileAndPlaySound(false);
}
/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////전체 프로그램 동작///////////////////////////////////

void loop() {
  currentMode = INIT;
  ActMainFunc();      //섹터에 따른 모드 판단 후, 각각에 대한 알맞은 기능을 동작.
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////기본 설정(플래그) 초기화 함수//////////////////////////////////
void initModeSetting() {
  mainMode = INIT;
  subMode = 0;
  oxSubMode = 0;
}

void InitSetting() {
  initModeSetting();
  isFirstTime = true;
  isHomeSelect = false;
  wasCancelled = false;
  isReplay = false;
  canSkip = false;
  canTakeOut = false;
}

  //test용
  void printPath() {
  Serial.println();
  Serial.print("path :");
  for (int i = 0; path[i] != NULL; i++) {
    Serial.print(path[i]);
  }
  Serial.println();
  }
  //test 용
  void printFileName() {
  Serial.println();
  Serial.print("fileName :");
  for (int i = 0; fileName[i] != NULL; i++) {
    Serial.print(fileName[i]);
  }
  Serial.println();
  }
  
  
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////테이블 초기 상태.(0단계)//////////////////////////////
void ActMainFunc() {
  //이 함수내에서 dir = basic(꼭 basic이 아닐 수도 있다. 홈버튼을 누른 경우 설정을 따로 하지 않음), path = /basic 고정

  if (powerOn) { //전원이 켜져있는 경우
    //*안내문 음성/
    if (!isFirstTime) {
      isFirstTime = true;
    }
    PlayGuideTrack(GUIDE);
    do {
      FindSector();  //*핀 입력값 찾기*//
    } while (sector == -1);
    if (ISMANUALBUT) { //메뉴얼 관련 섹터(다시듣기, 홈버튼, 취소버튼, 전원버튼)
      OperateManualBut();
    }
    else if (sector % 5 == 0) { //모드선택 부분(음악, 게임, 대화, 주문)
      isFirstTime = true; //각 모드에 들어갈 때 처음이라는 걸 표시
      mainMode = sector / 5; //music = 2, game = 3, order = 4
      currentMode = mainMode;
      PlayGuideTrack(SELECT); //선택되었다는 음성 출력

      switch (mainMode) {
        case MUSICMODE : //음악모드
          while (mainMode == MUSICMODE) {
            MusicMode();
          }
          break;
        case GAMEMODE : //게임모드
          while (mainMode == GAMEMODE) {
           //GameMode();
          }
          break;
        case ORDERMODE : //주문모드
          OrderMode();  //주문은 일회성
          break;
      }
      //각 모드가 끝나면 dirName설정
      SetDirName("basic");
      if (isFirstTime == false) //생각 좀 해볼 것
        isFirstTime = true;
    }
  }
  else { //전원이 꺼져있는 경우
    do { //전원 버튼이 눌려지지 않은 경우
      FindSector();
    } while (sector != 5);
    OperateManualBut(); //여기서 전원을 키게 한다.
    strcpy(path, "/basic"); //패스 초기화
    sd.chdir(path, O_READ);
    InitSetting();  //플래그값을 초기화 시킨다.
  }
}
