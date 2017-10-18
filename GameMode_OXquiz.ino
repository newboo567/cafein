//필요음성 목록
//통합 음성 리스트
//-> 열개중(한파일) / XX개 맞추셨습니다. / 축하합니다 일곱개 이상 맞추셨습니다. 음료를 하나 랜덤으로 드립니다.(녹음 필요)
//

///////////////////////////////////////////////////////////////////////////////////
/////////////////////////// OX quiz //////////////////////////////////////////////

void OXquiz() {
  //ox 퀴즈 -> 이 함수 내에서는 dirName = ox, path = /basic/game/ox -> 고정
  if (isFirstTime) {
    SetDirName("ox");    //디렉토리 설정
    ChangeDir(dirName, ADD);  //path 변화
  }
  else {
    isFirstTime = true;
  }
  sd.chdir(path, O_READ);  //디렉토리 이동
  //*안내문 음성
  PlayGuideTrack(GUIDE);  //어떤 분야 선택할래요.
  //*핀 입력값 찾기
  do {
    FindSector();  //*핀 입력값 찾기*//
  } while (sector == -1);
  if (ISMANUALBUT) //메뉴얼 관련 섹터(다시듣기, 홈버튼, 취소버튼, )
    OperateManualBut();
  else if (sector % 5 == 0) { //게임모드선택 부분(OX quiz, board game, rsp(가위바위보), ISJS(이심전심))
    isFirstTime = true; //각 모드에 들어갈 때 처음이라는 걸 표시
    oxSubMode = (sector / 5) + 10;  //가능 번호 12, 13, 14
    currentMode = oxSubMode;
    PlayGuideTrack(SELECT);
    ///게임시작
    PlayOXquiz();
  }
  //모드 설정 고려??
  //currentMode = GAMEMODE;
  //게임화면으로 리턴
  //디렉토리 설정 고려할 것 -> 밑에 거면 될 것 같다.
  SetDirName("ox");
  ChangeDir(dirName, DEL);  //path 변화(DEL)
}

//일단 sd 파일에 저장된 문제의 개수가 이미 정해져있단 가정하에 진행을 시작하기로 한다.
void PlayOXquiz() {
  int ques; //문제 개수
  char * oxAns; //정답 저장 버퍼
  char buf[4];  //번호 저장하는 임시버퍼
  char theme[6];   //테마별 파일이름
  int i = 0, count = 0;
  char userAns;  //사용자 입력 답안
  //이 함수내에서는 dirName = ox <=> cs, ent, movie  // path <- dirName에 따라 변동

  //규칙 안내문 출력제조사 ->
  SetFileName("rule.mp3");
  OpenFileAndPlaySound(false); //음성 : 게임하는 방법~~~~


  switch (oxSubMode) {
    case OX_ENTERTAINMODE :
      SetFileName("entStr.mp3");
      OpenFileAndPlaySound(false);     //연예계 분야 시작합니다.
      SetFileName("entAns.txt");
      strcpy(theme, "ent");
      break;
    case OX_NATUREMODE :
      SetFileName("csStr.mp3");
      OpenFileAndPlaySound(false);     //상식 분야 시작합니다.
      SetFileName("csAns.txt");
      strcpy(theme, "cs");
      break;
    case OX_MOVIEMODE :
      SetFileName("movieStr.mp3");
      OpenFileAndPlaySound(false);     //영화 분야 시작합니다.
      SetFileName("movieAns.txt");
      strcpy(theme, "movie");
      break;
  }

  //문제를 위해서
  //디렉토리 임시 변경
  SetDirName(theme);
  ChangeDir(dirName, ADD);  //path 변화   //'cs' or 'ent' or 'movie' dir
  sd.chdir(path, O_READ);  //디렉토리 이동
  file.open(sd.vwd(), fileName, O_READ); //파일 오픈
  file.fgets(buf, sizeof(buf));
  ques = atoi(buf); //문제 개수 저장
  oxAns = (char *)malloc(sizeof(char) * ques);
  while (file.fgets(buf, sizeof(buf), " ")) {   //문제 답 저장.
    if (buf[0] == 'O' || buf[0] == 'X') {
      oxAns[i] = buf[0];  //문제 답 저장
      i++;
    }
  }
  ques = i; //문제 개수 저장
  file.close();
  //*시작
  for (i = 1; i <= 3; i++) {
    //문제출력(첫트랙 돌릴때는 path변경 없음)
    mkNameAndSetFile(theme, i, buf);
    OpenFileAndPlaySound(false); //문제 음성 출력  //통합 -> 문제 + 맞으면 오, 틀리면 x => 문제파일이름 -> 했음

    //*플레이어가 누를 때까지 기다림
    do {
      FindSector();  //*핀 입력값 찾기*//
    } while (sector == -1);
    if (ISMANUALBUT) { //메뉴얼 버튼을 눌렀을 시
      OperateManualBut();
      return;
    }
    else { //경계선을 기준으로 왼편에 있는 지 오른편에 있는 지에 따라 유저의 답을 판단한다.
      int num = (sector - 1) % 5;
      if (num < 2)
        userAns = 'O';
      else if (num > 2)
        userAns = 'X';
    }

    //부모 디렉토리 이동 -> 메뉴얼 음성을 위해
    ChangeDir(dirName, DEL);  //path 변화
    sd.chdir(path, O_READ);  //디렉토리 이동


    //답 판별
    if (oxAns[i] == 'O')
      SetFileName("ISO.mp3");
    else
      SetFileName("ISX.mp3");
    OpenFileAndPlaySound(false);//음성 출력 //통합 -> 정답은 + 오(엑스)입니다. -> isO, isX쪽으로 통합.

    //채점
    if (oxAns[i] == userAns) {
      SetFileName("correct.mp3");
      count++;
    }
    else
      SetFileName("wrong.mp3");
    OpenFileAndPlaySound(false);

    //if (i != ques) {  //처음 ~ 마지막 문제까지 온 경우
    if (i != 3) {  //처음 ~ 마지막 문제까지 온 경우
      //테마 문제 파일로 이동
      ChangeDir(dirName, ADD);  //path 변화
      sd.chdir(path, O_READ);  //디렉토리 이동
    }
  }//////*for loop end////
  //음성 출력 =결과 통보
  SetFileName("theEnd.mp3");
  OpenFileAndPlaySound(false);

  //()문제 중 ()문제 맞추셨습니다.
  mkNameAndSetFile("score", count, buf); //맞힌 문제수에 따라서 작동하게 한다.
  OpenFileAndPlaySound(false);

  if (count >= 7) {
    SetFileName("gift.mp3");//음성 출력  //통합
    OpenFileAndPlaySound(false);
    //GameEvent();  //게임이벤트 진행
  }
  //Serial.println("ox the end!");
  free(oxAns);
}


