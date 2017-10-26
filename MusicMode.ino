
void MusicMode() {
  boolean isLyric;
  //이 함수내에서 dir = music, path = /basic/music 고정
  if (isFirstTime) {
    ChangeDir("music", ADD);  //path 변화
  }
  else//리플레이 할 경우 이걸 통과 시킬지 결정해봐라
    isFirstTime = true; //한번 음악을 들었을 경우 안내음성을 출력해주게 하기 위해서이다.
  sd.chdir(path, O_READ);
  PlayGuideTrack(GUIDE);  //음성 : 어떤 음악을 선택하시겠습니까? 슬픔 테마를 원하신다면 일번, 신남 테마를 원하신다면 이번, 차분함 테마를 원하신다면 삼번, 다시 듣길 원하신다면 리플레이 버튼을 눌러주세요.
  do {
    FindSector();  //*핀 입력값 찾기*//
  } while (sector == -1);
  if (ISMANUALBUT) //메뉴얼 관련 섹터(다시듣기, 홈버튼, 취소버튼)
    OperateManualBut();
  else if (sector % 5 == 0) {
    currentMode = (sector / 5) + 3; //5, 6, 7 중 하나
    PlayGuideTrack(SELECT);

    //가사가 있는 음악 or 가사가 없는 음악??
    OpenFileAndPlaySound("lyricGui.mp3", true);
    do {
      FindSector();  //*핀 입력값 찾기*//
    } while (sector == -1);
    if (ISMANUALBUT) //메뉴얼 관련 섹터(다시듣기, 홈버튼, 취소버튼)
      OperateManualBut();
    else {
      if (sector == 10) //가사 있음
        isLyric = true;
      else if (sector == 15) //가사 없음
        isLyric = false;
      PlayMusic(isLyric);  //음악 재생 함수 -> 들어가서 가사 있는, 혹은 없는 트랙 모두 한번씩 듣고 나온다.
    }
    //음악 한 트랙 감상 끝
    currentMode = MUSICMODE;
  }
}//music mode 끝

void PlayMusic(boolean isLyric) { //음악 한 트랙을 연속으로 듣게 한다.
  int noLyric, lyric, total;
  int * trackList;  //가사 있는 혹은 가사 없는 트랙을 저장할 공간
  int trackNum, start;
  //처음으로 들어온 경우 진행할 수 있게 해야할 듯??
  ChangeDir("musicLst", ADD);  //path 변화(ADD)
  sd.chdir(path, O_READ); // -> 현재 /basic/music/musicList에 있음

  //테마에 따라 값을 달리 지정   --> 쌔끈하게 바꿀 수 있다면 그렇게 할 것
  if (currentMode == MUSIC_SADMODE) {
    noLyric = 110; lyric = 113; total = 7;
  }
  else if (currentMode == MUSIC_CHEERMODE) {
    noLyric = 120; lyric = 123; total = 7;
  }
  else if (currentMode == MUSIC_CALMMODE) {
    noLyric = 130; lyric = 133; total = 6;
  }

  if (isLyric == true) { //가사 있음
    trackNum = (total - (lyric - noLyric)); //트랙개수
    start = lyric;
  }
  else { //가사 없음
    trackNum = (lyric - noLyric);//트랙개수
    start = noLyric;
  }
  trackList = (int *)malloc(sizeof(int) * trackNum); //동적할당
  for (int i = 0; i < trackNum; i++) {
    trackList[i] = start + i;
  }
  //랜덤 트랙을 만든다.
  MakeRanTrack(trackList, trackNum);


  //트랙 실행
  if (PlayMusicTrack(trackList, trackNum)) {//중도에 그만두지 않았다면(true 반환)
    //음악모드로 돌아가서 음성을 틀기 위해 디렉토리를 지운다.
    ChangeDir("music", BACK);  //path 변화(music으로 BACK)
  }
  free(trackList);
}

bool PlayMusicTrack(int * track, int trackNum) {
  char numbuf[4];
  char * trackName;

  for (int i = 0; i < trackNum; i++) {

    isMusPlay = true; //음악재생 동안은 컵소리 방해 없음
    //파일이름 설정
    trackName = mkNameAndSetFile("TRACK", track[i], numbuf);
    OpenFileAndPlaySound(trackName, true);
    free(trackName);
    //플레이 시작
    while (MP3player.isPlaying()) {
      Serial.println("Music is Playing...");
      FindSector();  //*핀 입력값 찾기*//
      if (ISMANUALBUT) { //메뉴얼 관련 섹터(홈버튼, 취소버튼, 전원버튼)
        isMusPlay = false;
        OperateManualBut();
        return false;
      }
      else { //메뉴
        switch (sector) {
          case 7 :
            MP3player.setVolume(0, 0);//대
            //MP3player.setVolume(90, 90);//대 -> 이어폰용
            break;
          case 8 :
            MP3player.setVolume(10, 10);//중
            //MP3player.setVolume(100, 100);//중
            break;
          case 9 :
            MP3player.setVolume(20, 20);//소
            //MP3player.setVolume(110, 110);//소
            break;
        }
        if (sector == 15) //다음 곡 재생
          break;
        else if (sector == 20) { //이전 곡 재생
          i -= 2;
        }
      }
    }//while end

    isMusPlay = false;
    delay(500);
  }//for문 end
  return true;  //여기까지 왔다는 것은 끝까지 재생했다는 소리
}

void MakeRanTrack(int list[], int listNum) {
  int temp;
  while (listNum != 1) {
    int index = rand() % listNum;
    if (index != listNum - 1) {
      temp = list[listNum - 1];
      list[listNum - 1] = list[index];
      list[index] = temp;
    }
    listNum--;
  }
}
