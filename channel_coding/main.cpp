#include <iostream>

#include "global.h"


using namespace std;

int main (){

    SevenFourCoding sfc;
    sfc.setErrorProb100(10);
    sfc.fileOpen();
    for (int i = 0; i < 10000; i++)
    {
        sfc.makeMsg(); //무작위의 메세지 생성

        sfc.channelCoding(); //4bit msg로 7bit (7,4) code 생성
        sfc.sending(); //(7,4), (4,4)의 패킷 송신 <error가 더해짐>

        sfc.makeSyndrome(); //수신된 packet으로 syndrome산출
        sfc.channelDecoding(); //error 발생 여부확인 및 원래 코드 복원

        sfc.checkRestored(); //복원된 packet이 원래 코드와 동일한지 확인
        sfc.checkOriginalPacket(); //(4,4) 코드 packet이 원래와 동일한지 확인
        sfc.storeMsg();
    }
    sfc.printCode();
}