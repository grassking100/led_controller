#include <stdio.h>
#include <unistd.h>
#include <string.h>

const char * const RED_PATH = "/sys/class/leds/led1/brightness";
const char * const GREEN_PATH = "/sys/class/leds/led0/brightness";

bool controllLED(const char * path,int status){
    FILE * fpt = fopen(path,"w");
    if(fpt==0){
        return false;
    }
    fprintf(fpt,"%d\n",status);
    fclose(fpt);   
    return true;
}

bool controllLEDByHeart(const char * path,int time){
    int lightStatus = 0;
    for(int i=0;i<time*2;i++){
        if(!controllLED(path,lightStatus)){return false;}
        lightStatus = 1 - lightStatus;
        sleep(1);
    }
    return true;
}

bool blinkByAscii(const char * outputPath,const char * timePath,const char * input){
    int ascii;
    printf("Starting...\n");
    if(!controllLEDByHeart(timePath,0)){return false;}
    if(!controllLEDByHeart(outputPath,0)){return false;}
    for(int k=0;k<strlen(input);k++){
        ascii = int(input[k])-int('A');
        if(ascii<0){return false;}
        printf("Show %c by blinking %d times\n",input[k],ascii);
        if(!controllLED(timePath,1)){return false;}
        if(!controllLEDByHeart(outputPath,ascii)){return false;}
        sleep(1);
        if(!controllLED(outputPath,0)){return false;}
        if(!controllLED(timePath,0)){return false;}
        sleep(1);
    }
    return true;
}

int main()
{
    int lightStatus = 0;
    char input[1000];
    printf("Press any word (at most 999 characters)\n");
    scanf("%s",input);
    printf("Get %s, start encoding...\n",input);
    if(blinkByAscii(GREEN_PATH,RED_PATH,input)){
        printf("Finish!\n");
        return 0;
    }else{
        printf("Error!\n");
        return 1;
    }
}
