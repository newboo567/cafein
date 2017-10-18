void OpenFileAndPlaySound(boolean canSkip) {
  char temp[12];
  //canSkip = true이면 끝까지 재생하지 않아도 됨. 즉, 아무것도 하지 않는while문을 거치지 않는다.
  ///*test*/Serial.print("before open file : ");
  ///*test*/printPath();
  ///*test*/printFileName();
  file.open(sd.vwd(), fileName, O_READ);//이게 진짜!!!
  ///*test*/if (file.open(sd.vwd(), fileName, O_READ)) {
  ///*test*/Serial.print("OpenFileAndPlaySound() : file is open ");
  ///*test*/printFileName();
  // }
  if (MP3player.isPlaying()) //실행되고 있는 음성이 있다면 일단 끈다
    MP3player.stopTrack();
  MP3player.playMP3(fileName);
  ////테스트용
  //if (!MP3player.isPlaying()) {
  ///*Test*/Serial.println("no Sound");
  //}
  if (canSkip == false) {
    while (MP3player.isPlaying()) { //음성이 다 출력이 될 때까지 아무것도 하지 않고 기다림
      /*if (canTakeOut == true) {
        Serial.println("Here!");
        strcpy(temp, fileName);
        SetFileName(temp);
        OpenFileAndPlaySound(false);
        strcpy(fileName, temp);
        file.open(sd.vwd(), fileName, O_READ);
        MP3player.playMP3(fileName);
        canTakeOut = false;
        }
        if (isMusPlay || canSkip)
        break;
        }*/
      file.close();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////캔슬용 모드 전환 함수//////////////////////////////////
void Cancelled_ChangeMode() { //디렉토리 변경 -> 한개의 디렉토리만 삭제하면 되고, 각 모드 내에서 디렉토리의 값은 거의 고정이다.(일부 예의 주시. 특히 ox같은 경우 디렉토리가 왔다갔다한다.)
  if (oxSubMode != 0) { //현재 ox게임을 진행하고 있다. -> 취소시 game 모드로 되돌아간다.
    currentMode = subMode;
    //ChangeDir(dirName, DEL);  //디렉토리 삭제
  }
  else if (subMode != 0) {  //현재 세부 게임, 혹은 세부 테마 음악을 듣고 있다.
    currentMode = mainMode;
    if (mainMode == GAMEMODE) {
      ChangeDir(dirName, DEL);  //디렉토리 삭제
    }
    else if (mainMode == MUSICMODE) {
      ChangeDir(dirName, DEL);
    }
  }
  else { //게임선택, 음악선택, 주문선택 화면이 여기에 해당될 것
    currentMode = mainMode = INIT;
    ChangeDir(dirName, DEL);
  }
}

////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////메뉴얼 버튼 작동 함수//////////////////////////////////
void OperateManualBut() {
  //우선 음성파일이 있는 디렉토리로 이동.
  sd.chdir("/basic", O_READ);
  switch (sector) {
    case 2 :  //다시듣기
      isReplay = true;
      break;
    case 3 :  //홈버튼
      if (mainMode != INIT) {
        isHomeSelect = true;
        initModeSetting();
        strcpy(path, "/basic");
      }
      SetFileName("home.mp3");
      break;
    case 4 :  //취소버튼 -> 여기 고쳐야 한다.
      if (mainMode != INIT && mainMode != ORDERMODE) {//메인화면과 주문모드에서는 통하지 않는다.
        wasCancelled = true;
        Cancelled_ChangeMode();
      }
      SetFileName("cancel.mp3");
      break;
    case 5 :  //전원버튼
      mainMode = INIT;
      if (powerOn == true) {  //현재 전원이 켜져있다면
        powerOn = false;  //끄자
        SetFileName("pOff.mp3");
      }
      else {  //현재 전원이 꺼져있다면
        powerOn = true;  //켜자
        SetFileName("pOn.mp3");
      }
      break;
  }

  //리플레이 빼고 나머지 경우는 음원 실행
  if (sector != 2) {
    OpenFileAndPlaySound(false);
  }
  //홈, 전원버튼은 basic디렉토리에 그대로.
  if (wasCancelled == true || isReplay == true) {
    sd.chdir(path, O_READ); //지정 path를 참고하여 이동
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////안내 음성, 선택 음성 출력//////////////////////////////////////
void PlayGuideTrack(int trackType) {
  char modNum[3]; //파일에 붙을 번호 저장할 버퍼
  //트랙 타입에 따라 파일이름 지정
  if (trackType == GUIDE)
    mkNameAndSetFile("guide", currentMode, modNum);
  else if (trackType == SELECT)
    mkNameAndSetFile("slt", currentMode, modNum);
  if (trackType == GUIDE) {
    if (isFirstTime || isHomeSelect || wasCancelled || isReplay) {//플래그 체크
      //이번이 처음으로 작동 or 홈버튼이 눌러진 경우 or 전 모드에서 취소버튼을 누른경우
      // or 안내문 다시 누르기 버튼을 누른 경우
      //파일열고 재생


      /// printFlag();//테스트


      //일단 디렉토리 이동은 필요없는 듯하다.
      OpenFileAndPlaySound(true);

      if (isFirstTime)
        isFirstTime = false;
      else if (isHomeSelect)
        isHomeSelect = false;
      else if (wasCancelled)
        wasCancelled = false;
      else if (isReplay)
        isReplay = false;
    }
  }
  else {

    OpenFileAndPlaySound(false);
  }
}

//인터럽트 비슷한 역할
void PlayOrderReadyTrack() {
  //Serial.println("PlayOrderReadyTrack()");
  //sd.chdir("/basic", O_READ);
  if (MP3player.isPlaying()) {
    MP3player.stopTrack();
  }
  printPath();
  printFileName();
  file.open(sd.vwd(), "takeout.mp3", O_READ);
  MP3player.playMP3("takeout.mp3");
  while (MP3player.isPlaying()) {
    //do nothing
  }
  //delay(700);
  if(canTakeOut == true){
  //Serial.println("PlayOrderReadyTrack() end");
    OpenFileAndPlaySound(true);
    canTakeOut = false;
  } 
  
}

