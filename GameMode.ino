/////////////////////////////////////////////////////////////////////////
//////////////////////// 1_4단계 Game Mode 메뉴 ///////////////////////////////
void GameMode() {
  //게임모드 에서 -> dirName = game, path = /basic/game -> 고정

  if (isFirstTime) {
    //Serial.println("FirstTime");
    SetDirName("game");    //디렉토리 설정
    ChangeDir(dirName, ADD);  //path 변화
  }
  else{
    //Serial.println("is not FirstTime");
    isFirstTime = true;
  }
  //printPath();
  sd.chdir(path, O_READ);  //디렉토리 이동
  //*안내문 음성*//
  PlayGuideTrack(GUIDE);
  //*핀 입력값 찾기*//
  do {
    FindSector();  //*핀 입력값 찾기*//
  } while (sector == -1);
  if (ISMANUALBUT) //메뉴얼 관련 섹터(다시듣기, 홈버튼, 취소버튼, )
    OperateManualBut();
  else if (sector % 5 == 0) { //게임모드선택 부분(OX quiz, board game, rsp(가위바위보), ISJS(이심전심))
    isFirstTime = true; //각 모드에 들어갈 때 처음이라는 걸 표시
    subMode = (sector / 5) + 6; //8, 9, 10, 11
    currentMode = subMode;  //currentMode = 네 개의 게임 중 하나의 값으로 저장된다
    PlayGuideTrack(SELECT); //음원 플레이
    switch (subMode) {
      case OXQUIZMODE : //8번
        OXquiz();
        break;
      case RSPMODE :  //9번
        RSPGame();
        break;
      case ISJSMODE : //10
        ISJSGame();
        break;
      case BACMODE :  //11
        BACGame();
        break;
    }//switch 끝
    //한 게임 끝
    currentMode = GAMEMODE;
  }
}


