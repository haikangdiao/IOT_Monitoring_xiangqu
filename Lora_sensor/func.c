#include "func.h"


void delayms(){
    for(int i = 0;i < 0xF8;++i){
        for(int j = 0;j < 0x05;++j){}
    }
}

int power(int a,int b){
  int i,c=1;
  if(b==0){return 1;}       //check if the index b is 0
  if(b==1){return a;}       //check if the index b is 1
  for(i=0;i<b;i++){c=c*a;}   //other cases
  return c;
}

void hexToDec(unsigned int hex,char* dec){
  short i,j,size=0;
  int opt;
  if(hex<0){                                    //check if input value is < 0
      hex=-hex;
      dec[i]='-';
      size=1;
  }
  if(hex==0){dec[0]='0';dec[1]='\0';return;}    //check if input value is 0
  int temp=hex;
  for(i=0;temp;i++){                            //calculating the length of hex
    temp/=10;
  }
  temp=hex;
  for(j=size;j<i;j++){                          //converting to dec style
    opt=power(10,i-j-1);
	dec[j]=temp/opt+48;
    temp-=temp/opt*opt;
  }
  dec[i]='\0';                                  //add the end symbol
}