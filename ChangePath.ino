/////////////////////////////////////path 설정///////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void ChangeDir(char * dir, int mod) {
  if (mod == ADD) {
    strcat(path, "/");  //구분자
    strcat(path, dir);
  }
  else if (mod == DEL) {//끝을 null로 해놓자.
    path[strlen(path) - (strlen(dir) + 1)] = NULL;
  }
}

void ChangeFile(char * trackPath, char * file, int mod) { //패스 <- / + 확장자 붙은 파일(이미 붙어있음)
  if (mod == ADD) {
    strcat(trackPath, "/");  //구분자
    strcat(trackPath, file); //파일+확장자 붙임
  }
  else if (mod == DEL) {
    path[strlen(trackPath) - (strlen(file) + 1)] = NULL;
  }
}

//특정 파일이름을 전역변수에 복사
void SetFileName(char * trackName) {
  strcpy(fileName, trackName);
}

void SetDirName(char * newDirName) {
  strcpy(dirName, newDirName);
}

void mkNameAndSetFile(char * str, int num, char * numbuf) {
  SetFileName(str);
  strcat(fileName, itoa(num, numbuf, 10));
  strcat(fileName, ".mp3");   //확장자 붙이기
}

