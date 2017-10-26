/////하나 주문하고 ok버튼 눌러도 전송될 수 있게 하기
//sector = 10 : OK 버튼
//sector = 15 : 이전 버튼
//sector = 20 : 다음 버튼
//sector = 25 : 연속
// sector = 6 - 9/ 11- 14/ 16 -19/ 21- 24 => 메뉴판
//메뉴 70개(1번부터 시작 / 라지사이즈는 61~70번)
#define MENULISTSIZE 16
#define TABLE 7
/*boolean isOrderFin = false; //주문완료 되었음을 나타내는 플래그
  int add = 0, temp = 0, k = 1;  //k = index 역할  //add -> 페이지와 관련있는 변수
  byte arr[10] = {-1};
*/
void OrderMode() {
  //주문 모드 내에서 dirName = order, path = /basic/order ->> 고정
  char buf[3];
  int pageNum = 1;
  int i, menu;
  boolean orderExist = false;
  char * menuName;
  //*안내문 음성  &&  디렉토리 설정, path 변경
  ChangeDir("order", ADD); //path 변화
  sd.chdir(path, O_READ); //현재 모드의 디렉토리로-> order
  //*안내문 음성*//
  PlayGuideTrack(GUIDE);  //음성 : 원하시는 메뉴를 선택해주세요.
  while (isOrderFin != true) { //주문 완료되면 isOrderFin = true
    //핀 인식
    do {
      FindSector();
    } while (sector == -1); //핀 인식 끝
    if (sector == 5 || sector == 3) {
      initArr();  //초기화
      OperateManualBut();
      return;
    } //sector = 5, 3 end
    else if (sector == 4) { //상단 취소 버튼
      if (k != 1) { //현재 주문 리스트에 뭔가 있다면
        OpenFileAndPlaySound("cancel.mp3", false);
        arr[k - 1] = 0;
        k--;
        arr[--k] = 0;
        // printArr(); //테스트용
      }
    }//sector = 4 end
    else if (sector % 5 == 0) { //메뉴얼 버튼
      switch (sector) {
        case 10 : //주문 접수 완료(ok 버튼)
          if (k != 1) { //주문 리스트에 뭐라도 있다면
            orderExist = true;
            if (temp != 0)
              arr[k++] = menu;
          }
          else { //k == 1
            if (temp != 0) {
              arr[k++] = menu;
              orderExist = true;
            }
          }
          if (orderExist) {
            OpenFileAndPlaySound("finish.mp3", false);
            SendList();  //배열 전송
            isOrderFin = true;  //주문 완료 되었음
          }
          //printArr();//테스트용
          break;
        case 15 : //이전 페이지 이동
          //음성: 이전 페이지로
          if (add > 19) {
            add = add - 20;
            OpenFileAndPlaySound("prePage.mp3", false);
          }
          else { //음성: 첫 페이지 입니다.//0~19까지
            OpenFileAndPlaySound("firPage.mp3", false);
          }
          break;
        case 20 : //다음 페이지 이동
          //음성: 다음 페이지로
          if (add <= 60) {
            OpenFileAndPlaySound("nextPage.mp3", false);
            add = add + 20;
          }
          else {
            //음성: 마지막 페이지 입니다.//최대 80까지
            OpenFileAndPlaySound("lstPage.mp3", false);
          }
          break;
        case 25 : //추가 메뉴 선택 (연속 버튼)
          if (temp != 0) {  //temp != 0<=> 현재 뭔가를 주문했다.
            //음성: 추가 메뉴를 선택해주세요
            OpenFileAndPlaySound("sltAno.mp3", false);
            arr[k++] = menu;
            temp = 0;
            //printArr();//테스트용
          }
          break;
      }//switch end
      //OpenFileAndPlaySound(false); //각각의 파일에 맞게 음성 방출
    }//sector = 10, 25, 20, 15 end(sector % == 5)
    else {  //sector ! = 3,4,5,10,15,20,25
      temp = sector;  //주문 번호 임시 저장
      if (sector < 18 && (add == 0)) { //첫 페이지 첫 10 메뉴인 경우
        //음성: 어떤 사이즈 할건지 물어보기 small이면 왼쪽 large면 오른쪽을 눌러주세요
        OpenFileAndPlaySound("sOrL.mp3", false);
        do {
          FindSector();
        } while (sector == -1); //do while 끝
        if ( sector > 5) {
          if (sector % 5 == 0 || sector % 5 == 4) { //가운데 선을 기준으로 오른쪽이 large 임
            temp = temp + 75;// break;
          }
        }
      }
      temp = temp + add;  //add는 20의 배수, 주문 번호는 add + 섹터번호(temp)  //temp는 1이상
      menu = changeToMenuNum(temp); //진짜 메뉴 번호 나왔다!
      menuName = mkNameAndSetFile("menu", menu, buf);
      OpenFileAndPlaySound(menuName, false);
      free(menuName);
    }//else 끝 ( sector ! = 3,4,5,10,15,20,25 끝)
  }//while 끝
  //주문 모드 끝
  OpenFileAndPlaySound("return.mp3", false);
  //디렉토리 바꾸자
  isOrderFin = false;  //
  add = 0;
  k = 1;
  initArr();  //주문 배열 초기화
  currentMode = INIT;
} //ORDER MODE 끝
int changeToMenuNum(int a) {
  //섹터번호를 메뉴번호로 변경해줌  한 페이지 바뀌면 20씩 더해주기  사이즈 라지면 섹터 번호에 75더해주기
  int p, q, r;
  p = (a - 1) / 5;
  q = (a - 1) % 5;
  r = 4 * (p - 1) + q + 1;
  return 4 * (p - 1) + q + 1;
}
//주문 리스트를 보내는 코드
int menuSize = 11;
int chairDataSize = 3;
void SendList() {
  if (isOrderFin) {
    arr[0] = TABLE; //테이블 번호 삽입
    Wire.write(arr, menuSize); // respond with message of 6 bytes
  }
  else if(isEvent == true){
    arr[0] = TABLE; //테이블 번호 삽입
    int drink = rand() % 39 + 1;
    arr[1] = drink;
    Wire.write(arr, menuSize); // respond with message of 6 bytes
    isEvent = false;
    initArr();
  }
  else {
    Wire.write(chair, chairDataSize);
  }
}

//주문이 다 되었다는 코드
void receiveOrderSig(int howMany) {
  while (Wire.available() > 0) { // loop through all but the last
    int c = (int)Wire.read(); // receive byte as a character
    if (c == TABLE) {
      canTakeOut = true;
      OpenFileAndPlaySound("takeout.mp3", false);
    }
  }
}

void initArr() {
  for (int i = 0; i < sizeof(arr) / sizeof(byte); i++) {
    arr[i] = -1;
  }
}

