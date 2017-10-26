/////////////////////////////////////path 설정///////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void ChangeDir(char * dir, int mod/*, boolean haveToChangePath*/) {
  if (mod == ADD) {
    strcat(path, "/");  //구분자
    strcat(path, dir);
  }
  else if (mod == BACK) {//끝을 null로 해놓자.
    char * ptr = strstr(path, dir);
    path[strlen(path) - strlen(ptr) + strlen(dir)] = NULL;
  }
}

//기본 파일이름에 번호를 붙임.
char * mkNameAndSetFile(char * str, int num, char * numbuf) {
  char * temp = (char *)malloc(sizeof(char) * 13);
  strcpy(temp, str);
  strcat(temp, itoa(num, numbuf, 10));
  strcat(temp, ".mp3");   //확장자 붙이기
  return temp;
}

//path 초기화 + 디렉토리 이동
void InitPath(){
  strcpy(path, "/basic"); //패스 초기화
  sd.chdir(path, O_READ); //디렉토리 설정
}

