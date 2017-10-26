//필요음성
// 모든 지시어가 끝났다. 라는 말이 필요할 것 같다.

//////////////////////////////////////////////////////////////////////////////////
/////////////////////////// 이심전심 //////////////////////////////////
void ISJSGame() {
  char numbuf[3]; //리스트 트랙넘버 버퍼
  char * trackName;
  int * track; int trackNum = 0;
  if (isFirstTime) {
    //SetDirName("isjs");    //디렉토리 설정
    //ChangeDir(dirName, ADD);  //path 변화
    ChangeDir("isjs", ADD);  //path 변화
  }
  else
    isFirstTime = true;
  sd.chdir(path, O_READ);  //디렉토리 이동
  PlayGuideTrack(GUIDE);    //룰 안내 음성
  delay(500);
  //시작합니다 음성
  OpenFileAndPlaySound("start.mp3", false);

  //리스트 파일에 접근하기 위해 디렉토리 이동
  ChangeDir("wordList", ADD);  //path 변화
  sd.chdir(path, O_READ);  //디렉토리 이동
  while (file.openNext(sd.vwd(), O_READ)) {
    trackNum++; //트랙수 카운트
    file.close();
  }
  track = (int *)malloc(sizeof(int) * trackNum);  //동적할당
  for (int i = 0; i < trackNum; i++)
    track[i] = i + 1;
  MakeRanTrack(track, trackNum);  //랜덤 트랙 배열 생성
  
  int num = 0;  //인덱스 역할
  while (num < trackNum) {
    //음성출력
    trackName = mkNameAndSetFile("word", track[num], numbuf);
    OpenFileAndPlaySound(trackName, false);
    free(trackName);
    do {
      FindSector();  //*핀 입력값 찾기*//
    } while (sector == -1);
    if (ISMANUALBUT) {  //취소/홈/전원버튼
      OperateManualBut();
      return;
    }//메뉴얼버튼 끝
    else
      num++;
  }
  
  //여기에 왔다면 트랙을 다 돌았다는 이야기가 된다.
  //트랙을 다 돌았을 경우 다시 게임모드로 되돌아갈 준비 한다.
  ChangeDir("game", BACK);  //path 변화(BACK)  //게임모드로 가기위함이다.
  free(track);
}//이심전심 끝

