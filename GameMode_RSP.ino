///음성 수정 -> 5번 게임. 4번 이상 이기면 랜덤 음료 제공
//음성 녹음본 필요 -> 5번 게임 중 / XX번 이기셨습니다. / 축하합니다. 랜덤으로 음료 하나를 드리겠습니다.(다른 게임 꺼 이용할 수 있으면 이용하기)



///////////////////////////////////////////////////////////////////////////////////
/////////////////////////// 가위바위보 //////////////////////////////////

void RSPGame() {
  int sys, player;
  int win = 0;  //이긴 횟수
  int count = 5; //count = 10번의 게임 -> 5번으로 수정
  char numbuf[2];

  SetDirName("rsp");    //디렉토리 설정
  ChangeDir(dirName, ADD);  //path 변화
  sd.chdir(path, O_READ);  //디렉토리 이동
  PlayGuideTrack(GUIDE); //음성 : 하나, 둘, 셋 하면 랜덤으로 가위, 바위, 보 중 하나를 선택하시면 됩니다.
  //*핀 입력값 찾기*//
  FindSector();  //*핀 입력값 찾기*//
  if (ISMANUALBUT) { //메뉴얼 관련 섹터(다시듣기, 홈버튼, 취소버튼, )
    OperateManualBut();
    return;
  }

  while (count > 0) {
    //*랜덤으로 시스템 쪽 답안 생성*//
    sys = rand() % 3 + 1; //1 =scissors, 2 = rock, 3 = paper
    delay(500);
    //음성 출력 : 하나. 둘. 셋
    SetFileName("123.mp3");
    OpenFileAndPlaySound(false);

    do {
      FindSector();  //*핀 입력값 찾기*//
    } while (sector == -1);

    if (ISMANUALBUT)
      return;
    else if (sector / 5 == 3) { //sector = 17, 18, 19 중 하나
      player = sector % 5 - 1; //1= scissors, 2= rock, 3 = paper
      if (player >= 1 && player <= 3) { //sector = 17, 18, 19 중 하나
        //Serial.print("sys : ");
        //Serial.println(sys);
        //Serial.print("user : ");
        //Serial.println(player);
        if (player == sys) {
          SetFileName("youDraw.mp3");
        }
        else if ((player - 1) % 3 == sys) {
          SetFileName("youWin.mp3");
          win++;
        }
        else {
          SetFileName("youLose.mp3");
        }
        OpenFileAndPlaySound(false);
      }
    }
    count--;
  }
  //가위바위보 끝

  //결과통보
  mkNameAndSetFile("win", win, numbuf);
  OpenFileAndPlaySound(false);

  if (win >= 4) {
    SetFileName("gift.mp3");//음성 출력
    OpenFileAndPlaySound(false);
    //GameEvent();  //게임이벤트 진행
  }

/*
  SetFileName("return.mp3");//음성 출력
  OpenFileAndPlaySound(); //게임화면으로 돌아간다는 음성*/
   //path 변경하자'
   currentMode = GAMEMODE;
  ChangeDir(dirName, DEL);  //게임으로 바뀐다.
}
