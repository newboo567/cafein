byte chair[3];
/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////섹터를 찾는 함수///////////////////////////////////////
void FindSector() {
  int row, col;
  initTouched();  //touched 배열 초기화
  AckPinNum(&row, &col);    //눌러진 핀 번호 인식
  sector = CalcSector(row, col);    //눌러진 핀 번호를 바탕으로 눌려진 섹터를 판단
  delay(500);
}

/////////////////////핀 배열, sector 초기화///////////////////////////////////////
void initTouched() {
  for (int i = 0; i < 12; i++) { //핀번호 배열 초기화
    touched[i] = 0;
  }
  sector = -1;  //섹터 번호 초기화
}

////////////////////터치 감지된 핀 번호 알려주는 함수 /////////////////////////
void AckPinNum(int * row, int * col) {

  int untouchCount = 0;
  for (int i = 0; i < 12; i++) {
    touchval[i] = inittouch[i] - ((unsigned int)MPR121.getRegister(MPR121_E0FDL + 2 * i) + ((unsigned int)MPR121.getRegister(MPR121_E0FDH + 2 * i) << 8));
    if (touchval[i] > 50) {//touched
      touched[i] = 1; //터치됨.
      if (i < 5)
        *col = i;
      else if (i <= 9)
        *row = i;
      else if (i <= 11) {//의자 판별
        if (i == 11) { //11번의자
          //Serial.print("not yet");
          //chair[0] = 7;
          chair[0] = 100; //의자 관련 신호
          chair[1] = 4; //의자번호
          chair[2] = 1;   //앉았음
          SendList();
        }
      }
    }
    else {//낮은 값을 유지함...
      if (i >= 0 && i <= 9)
        untouchCount++;
      else if (i == 11) {
        chair[0] = 200; //의자 관련 신호
        chair[1] = 4; //의자번호
        chair[2] = 0;   //자리 뗌
        SendList();
      }
    }
  }
  if (untouchCount > 8) {
    *row = 0; *col = 100;
  }
}

//////////////////////섹터 번호를 알려주는 함수///////////////////////////////
int CalcSector(int row, int col) {
  int row_remain_mul6
    = 6 * (row % 5); //6a
  int touchedSector
    = row_remain_mul6 + 5 - col
      - (row_remain_mul6 % 5);

  if (touchedSector < 0) {
    touchedSector = -1;
  }

  if (touchedSector > 0) {
    if (!(isMusPlay && (touchedSector >= 7) && (touchedSector <= 9))) //음악플레이에서 소리 볼륨 조절이 아니라면 삑소리 내도 괜찮아
      PlayCupSE(touchedSector);

  }
  return touchedSector;
}

void PlayCupSE(int sectorVal) {
  //컵의 삑소리

  sd.chdir("/basic", O_READ);
  if ((currentMode >= OX_ENTERTAINMODE && currentMode <= OX_MOVIEMODE)
      && (sectorVal >= 6)) {
    OpenFileAndPlaySound("ansSE.mp3", false);
  }
  else {
    OpenFileAndPlaySound("cupSE.mp3", false);
  }
  sd.chdir(path, O_READ); //현재 모드의 디렉토리로 back
}
