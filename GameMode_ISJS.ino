//필요음성
// 모든 지시어가 끝났다. 라는 말이 필요할 것 같다.

//////////////////////////////////////////////////////////////////////////////////
/////////////////////////// 이심전심 //////////////////////////////////
void ISJSGame() {
  char numbuf[3]; //리스트 트랙넘버 버퍼
  int * track; int trackNum = 0;
  if (isFirstTime) {
  SetDirName("isjs");    //디렉토리 설정
  ChangeDir(dirName, ADD);  //path 변화
  }
  else
    isFirstTime = true;
  sd.chdir(path, O_READ);  //디렉토리 이동
  PlayGuideTrack(GUIDE);    //룰 안내 음성
  delay(500);
  //시작합니다 음성
  SetFileName("start.mp3");
  OpenFileAndPlaySound(false);

  //리스트 파일에 접근하기 위해 디렉토리 이동
  SetDirName("wordList");    //디렉토리 설정
  ChangeDir(dirName, ADD);  //path 변화
  sd.chdir(path, O_READ);  //디렉토리 이동
  while (file.openNext(sd.vwd(), O_READ)) {
    trackNum++; //트랙수 카운트
    file.close();
  }
  track = (int *)malloc(sizeof(int) * trackNum);  //동적할당
  for (int i = 0; i < trackNum; i++)
    track[i] = i + 1;
  MakeRanTrack(track, trackNum);  //랜덤 트랙 배열 생성
  //for(int i = 0; i <  trackNum; i++){
  //  Serial.print(track[i]);
  //  Serial.print(" ");
  //}
  int num = 0;  //인덱스 역할
  while (num < trackNum) {
    //음성출력
    mkNameAndSetFile("word", track[num], numbuf);
    OpenFileAndPlaySound(false); //단어 재생
    do {
      FindSector();  //*핀 입력값 찾기*//
    } while (sector == -1);
    if (ISMANUALBUT) {  //취소/홈/전원버튼
      OperateManualBut();
      SetDirName("isjs");
      ChangeDir(dirName, DEL);  //path 변화(DEL)  //게임모드로 가기위함이다.
     // printPath();
      return;
    }
    /*else if (sector == 11)  //나중에 지울 것
      num = trackNum-1;
      num++;*/
    num++;
  }

  //트랙을 다 돌았을 경우 다시 게임모드로 되돌아갈 준비 한다.
  if (num ==  trackNum) { //트랙을 다 돌은 경우 일단 wordList 디렉토리 이름을 제거한다.
    ChangeDir(dirName, DEL);  //path 변화(DEL)
  }
 // printPath();
  SetDirName("isjs");    //디렉토리 설정
  ChangeDir(dirName, DEL);  //path 변화(DEL)  //게임모드로 가기위함이다.
  free(track);
}//이심전심 끝

