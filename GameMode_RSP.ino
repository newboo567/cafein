///음성 수정 -> 5번 게임. 4번 이상 이기면 랜덤 음료 제공
//음성 녹음본 필요 -> 5번 게임 중 / XX번 이기셨습니다. / 축하합니다. 랜덤으로 음료 하나를 드리겠습니다.(다른 게임 꺼 이용할 수 있으면 이용하기)



///////////////////////////////////////////////////////////////////////////////////
/////////////////////////// 가위바위보 //////////////////////////////////

void RSPGame() {
  int sys, player;
  int win = 0;  //이긴 횟수
  int count = 5; //count = 10번의 게임 -> 5번으로 수정
  char numbuf[2];
  char * result;

  ChangeDir("rsp", ADD);  //path 변화
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
    OpenFileAndPlaySound("123.mp3", false);

    do {
      FindSector();  //*핀 입력값 찾기*//
    } while (sector == -1);

    if (ISMANUALBUT){
      OperateManualBut();
      return;
    }
    else if (sector / 5 == 3) { //sector = 17, 18, 19 중 하나
      player = sector % 5 - 1; //1= scissors, 2= rock, 3 = paper
      if (player >= 1 && player <= 3) { //sector = 17, 18, 19 중 하나
        //Serial.print("sys : ");
        //Serial.println(sys);
        //Serial.print("user : ");
        //Serial.println(player);
        if (player == sys) {
          //SetFileName("youDraw.mp3");
          OpenFileAndPlaySound("youDraw.mp3", false);
        }
        else if ((player - 1) % 3 == sys) {
          //SetFileName("youWin.mp3");
          OpenFileAndPlaySound("youWin.mp3", false);
          win++;
        }
        else {
          //SetFileName("youLose.mp3");
          OpenFileAndPlaySound("youLose.mp3", false);
        }
        //OpenFileAndPlaySound(false);
      }
    }
    count--;
  }
  //가위바위보 끝

  //결과통보
  result = mkNameAndSetFile("win", win, numbuf);
  OpenFileAndPlaySound(result, false);
  free(result);

  if (win >= 4) {
    OpenFileAndPlaySound("gift.mp3", false);
    isEvent = true;
    SendList(); //이벤트 음료
  }

  //path 변경하자
  ChangeDir("game", BACK);  //게임으로 바뀐다.
}
