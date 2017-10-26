///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////음성 출력//////////////////////////////////////////////////
void OpenFileAndPlaySound(char * fileInput, boolean canSkip) {
  boolean wasPlaying;
  file.open(sd.vwd(), fileInput, O_READ);//이게 진짜!!!
  if(canTakeOut == false){  //주문 완료 음성 신호가 아직 없는 경우..
    strcpy(tempFile, fileInput);  //미리 백업(주문 완료 음성에 대처하기 위함.)
    wasCanSkip = canSkip;
  }
  else{
    if(MP3player.isPlaying())
      wasPlaying = true;
    else
      wasPlaying = false;
  }
  if (MP3player.isPlaying()) //실행되고 있는 음성이 있다면 일단 끈다
    MP3player.stopTrack();
  MP3player.playMP3(fileInput);
  if (canSkip == false) {
    while (MP3player.isPlaying()) { //음성이 다 출력이 될 때까지 아무것도 하지 않고 기다림
      
    }
  }
  if(canTakeOut == true){
    canTakeOut = false; //초기황
    if(wasPlaying == true)
      OpenFileAndPlaySound(tempFile, wasCanSkip);      
  }
  file.close();
}

////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////캔슬용 모드 전환 함수//////////////////////////////////
void Cancelled_ChangeMode() { //디렉토리 변경 -> 한개의 디렉토리만 삭제하면 되고, 각 모드 내에서 디렉토리의 값은 거의 고정이다.(일부 예의 주시. 특히 ox같은 경우 디렉토리가 왔다갔다한다.)
  if (currentMode >= OXQUIZMODE && currentMode <= OX_MOVIEMODE) { 
  //현재 ox게임을 진행하고 있다.(테마 이미 선택) 혹은 다른 게임 메뉴얼이나 하는 중이다 -> 취소시 game 모드로 되돌아간다.
    currentMode = GAMEMODE;
    ChangeDir("game", BACK);
  }
  else if(currentMode >=  MUSIC_SADMODE && currentMode <= MUSIC_CALMMODE){
    //음악세부테마 플레이 중..
    currentMode = MUSICMODE;
    ChangeDir("music", BACK);
  }
  else { //게임선택, 음악선택, 주문선택 화면이 여기에 해당될 것
    currentMode = INIT;
    ChangeDir("basic", BACK);
  }
}

////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////메뉴얼 버튼 작동 함수//////////////////////////////////
void OperateManualBut() {
  //우선 음성파일이 있는 디렉토리로 이동.
  sd.chdir("/basic", O_READ); //basic디렉토리로 이동
  switch (sector) {
    case 2 :  //다시듣기
      isReplay = true;
      sd.chdir(path, O_READ); //지정 path를 참고하여 이동
      break;
    case 3 :  //홈버튼
      if (currentMode != INIT) {
        isHomeSelect = true;
        //initModeSetting();
        InitPath();
      }
      OpenFileAndPlaySound("home.mp3", false);
      break;
    case 4 :  //취소버튼 -> 여기 고쳐야 한다.
      if (currentMode != INIT && currentMode != ORDERMODE) {//메인화면과 주문모드에서는 통하지 않는다.
        wasCancelled = true;
        Cancelled_ChangeMode();
      }
      OpenFileAndPlaySound("cancel.mp3", false);
      sd.chdir(path, O_READ); //지정 path를 참고하여 이동
      break;
    case 5 :  //전원버튼
      currentMode = INIT;
      if (powerOn == true) {  //현재 전원이 켜져있다면
        powerOn = false;  //끄자
        OpenFileAndPlaySound("pOff.mp3", false);
      }
      else {  //현재 전원이 꺼져있다면
        powerOn = true;  //켜자
        OpenFileAndPlaySound("pOn.mp3", false);
      }
      break;
  }//switch 끝
}

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////안내 음성, 선택 음성 출력//////////////////////////////////////
void PlayGuideTrack(int trackType) {
  char modNum[3]; //파일에 붙을 번호 저장할 버퍼
  char * fileName;
  
  if (trackType == GUIDE) {
    if (isFirstTime || isHomeSelect || wasCancelled || isReplay) {//플래그 체크
            //이번이 처음으로 작동 or 홈버튼이 눌러진 경우 or 전 모드에서 취소버튼을 누른경우
            // or 안내문 다시 누르기 버튼을 누른 경우
      //파일열고 재생
      fileName = mkNameAndSetFile("guide", currentMode, modNum);
      if(currentMode >= RSPMODE && currentMode <= BACMODE)
        OpenFileAndPlaySound(fileName, false);
      else
        OpenFileAndPlaySound(fileName, true);
      if (isFirstTime)
        isFirstTime = false;
      else if (isHomeSelect)
        isHomeSelect = false;
      else if (wasCancelled)
        wasCancelled = false;
      else if (isReplay)
        isReplay = false;
    }
  }//trackType == GUIDE 끝
  else {//trackType ==SELECT
    fileName = mkNameAndSetFile("slt", currentMode, modNum);
    OpenFileAndPlaySound(fileName, false);
  }//trackType == SELECT 끝
}
