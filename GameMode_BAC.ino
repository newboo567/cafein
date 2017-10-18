////////////추가음원사항

///Bulls And Cows(숫자 야구 게임)///
//세자리만 가능하다고 일단 가정
//중복은 허용되지 않는다는 걸 알려주자 ->>음성으로  ->> 보류

void BACGame() {
  int ansNum[3]; //정답 배열
  int playerNum[3];  //플레이어가 선택한 수의 배열
  int count = 10; //몇번 안에 정답 맞추나
  boolean BACGame_isCorrect = false;
  //숫자야구 모드 -> dirName = bac, path = /basic/game/bac  ->>  고정

  if (isFirstTime) {
    SetDirName("bac");    //디렉토리 설정
    ChangeDir(dirName, ADD);  //path 변화
  }
  else
    isFirstTime = true;//한번 음성을 들었을 경우 안내음성을 출력해주게 하기 위해서이다.
  sd.chdir(path, O_READ);  //디렉토리 이동
  //*안내문 음성*//
  PlayGuideTrack(GUIDE); //음성 : 룰 설명, 열번 이내에 맞추신 분께는 저희 가게의 음료 중 하나를 랜덤으로 드립니다.
  FindSector();  //*핀 입력값 찾기*//
  if (ISMANUALBUT) { //메뉴얼 관련 섹터(다시듣기, 홈버튼, 취소버튼, )
    OperateManualBut(); //dirName = bac이어서 삭제할 걱정은 없다.
    return;
  }

  //자리 선택에 따라 숫자를 랜덤 생성
  //함수 이용

  int * list = (int *)malloc(sizeof(int) * 10); //0~9까지 숫자 저장하는 배열 동적할당
  for (int i = 0; i < 10; i++)
    list[i] = i;
  //Serial.print("list : ");
  // for (int i = 0; i < 10; i++) {
  //   Serial.print(list[i]);
  //   Serial.print(" ");
  // }
  // Serial.println();
  MakeRanTrack(list, 10);  //배열을 랜덤으로 배치. 함수 호출할 때 마다 달라진다.
  for (int i = 0; i < 3; i++)
    ansNum[i] = list[9 - i];
  /*
    //test용
    Serial.print("list : ");
    for (int i = 0; i < 10; i++) {
      Serial.print(list[i]);
      Serial.print(" ");
    }
    Serial.println();
    //테스트
    Serial.print("ansNum = ");
    for (int x = 0; x < 3; x++) {
      Serial.print(ansNum[x]);
    }
    Serial.println();
  */
  //시작합니다 음성
  SetFileName("start.mp3");
  OpenFileAndPlaySound(false);

  while (count != 0) {
    int i = 0;
    SetFileName("slt3num.mp3"); //음성 : 연속으로 3개의 수를 터치하십시오.
    OpenFileAndPlaySound(false);
    while (i < 3) {
      do {
        FindSector(); //*사용자의 입력값을 찾는다.*//
      } while (sector == -1);
      if (ISMANUALBUT) {
        OperateManualBut();
        return;
      }
      else {
        ReadPlayerInput(playerNum, i);
        i++;
      }
    }
    /*
      //테스트
      Serial.print("userNum = ");
      for (int x = 0; x < 3; x++) {
        Serial.print(playerNum[x]);
      }
      Serial.println();*/
    ////*판별
    BACGame_isCorrect = CompareAnsAndPlayer(ansNum, playerNum);

    if (BACGame_isCorrect && count > 0) {
      //음성 : 축하합니다! 열번 안에 맞추셨습니다! 랜덤으로 음료를 드립니다.
      //congratu + gvRanone 파일 하나 (gift)로 통합한다.
      SetFileName("gift.mp3");
      OpenFileAndPlaySound(false);
      //GameEvent();  //게임이벤트 진행
      break;
    }
    else
      count--;
  }//while end


  //gameOver + cheerUp 파일 하나 (gmOver)로 통합한다.
  if (count == 0) { //
    SetFileName("gmOver.mp3");
    OpenFileAndPlaySound(false);
  }

  //숫자야구 끝

  //path 바꾸자.
  ChangeDir(dirName, DEL);  //게임모드로 바뀐다.

  free(list);
  delay(1500);
}

void ReadPlayerInput(int * arr, int index) {
  //번호 1~9 => sector - (2 * 내림(sector/5)+ 4)
  if (sector < 22) {
    arr[index] = sector - (2 * (sector / 5) + 4);
  }
  else if (sector == 23) {
    arr[index] = 0;
  }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////정답과 사용자의 답을 비교판정//////////////////////////
boolean CompareAnsAndPlayer(int * ans, int * player) {
  char numbuf[3]; //스트라이크수와 볼의 수를 이어붙여서 만듬
  int strike = 0, ball = 0;
  int i, j;
  boolean isRight = false;

  for (i = 0; i < 3; i++) { //ans
    for (j = 0; j < 3; j++) { //player
      if (ans[i] == player[j]) {
        if (i == j) { //strike
          strike++;
        }
        else { //ball
          ball++;
        }
        break;
      }
    }
  }

  if (strike == 3) //쓰리 스트라이트 == 정답
    isRight = true;
  //*결과에 대한 음성을 여기서 출력해준다.*//
  ////*결과를 바탕으로 track 번호 지정
  mkNameAndSetFile("bacR", strike * 10 + ball, numbuf);
  OpenFileAndPlaySound(false); //음악재생
  return isRight;
}
