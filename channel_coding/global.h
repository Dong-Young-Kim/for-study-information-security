#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//function prototype
int randomCode();
bool bitError(int);
void mul_m_G(vector<bool>&m, bool G[4][7], vector<bool>&c);
void mul_c_Ht(vector<bool>&c, bool H[7][3], vector<bool>&s);

//matrix
extern bool G[4][7];
extern bool Ht[7][3];

class SevenFourCoding{
public:

    void setErrorProb100(int p){
        errorProb100 = p;
    }

    void makeMsg(){
        msgBlock.clear();
        int msg = randomCode();
        //cout << msg << endl;
        for (int i = 0; i < 4; i++){
            msgBlock.push_back(msg % 2);
            msg /= 2;
        }
    }

    void channelCoding(){
        mul_m_G(msgBlock, G, packet_send);
    }

    void makeSyndrome(){
        mul_c_Ht(packet_recv, Ht, syndrome);
    }

    void sending(){ //각 비트마다 정해진 error발생 error 더하고, 연산 결과를 packet_recv 저장
        packet_recv.clear();
        for (int i = 0; i < 7; i++){
            packet_recv.push_back(packet_send[i] ^ bitError(errorProb100)); //xor -> error값이 1인 경우 값이 변화
        }

    }

    void channelDecoding(){

        //if there is no error
        if(syndrome[0] == 0 && syndrome[1] == 0 && syndrome[2] == 0) {
            printf("no error detected\n");
            restoredBlock = packet_recv;
            return;
        }

        //detect which bit has error
        int i;
        for (i = 0; i < 7; i++){
            if (Ht[i][0] == syndrome[0] &&
                Ht[i][1] == syndrome[1] &&
                Ht[i][2] == syndrome[2])
                break;
        }
        printf("%dth bit has error\n", i);
        
        //restore code
        restoredBlock = packet_recv;
        restoredBlock[i] = restoredBlock[i] ^ 1;

    }

    void checkRestored(){
        if(restoredBlock == packet_send){
            if (syndrome[0] == 0 && syndrome[1] == 0 && syndrome[2] == 0) restore_check = 0;
            else restore_check = 2;
        }
        else{
            restore_check = 1;
        }
    }

    void checkOriginalPacket(){
        if (msgBlock[0] == packet_recv[0] &&
            msgBlock[1] == packet_recv[1] &&
            msgBlock[2] == packet_recv[2] &&
            msgBlock[3] == packet_recv[3]) uncoded_check = 0;
        else uncoded_check = 1;
    }

    void printCode(){
        printf("\norignial msg    = ");
        for (int i = 0; i < 4; i++) cout << msgBlock[i];
        printf("\ncode block senc = ");
        for (int i = 0; i < 7; i++) cout << packet_send[i];
        printf("\ncode block rcvd = ");
        for (int i = 0; i < 7; i++) cout << packet_recv[i];
        printf("\nsyndrome        = ");
        for (int i = 0; i < 3; i++) cout << syndrome[i];
        printf("\nrestored code   = ");
        for (int i = 0; i < 7; i++) cout << restoredBlock[i];
        printf("\n");
        printf("\ncheck correction -> ");
        cout << (restore_check == 1 ? "\"correction FAILED\"" : "\"correction success\"") << endl;
        printf("\n");
    }

    string vecToStr(vector<bool>& v){
        string s;
        for (int i = 0; i < v.size(); i++){
            s.push_back(v[i] ? '1' : '0');
        }
        return s;
    }

    void storeMsg(){
        ofs << vecToStr(msgBlock) << ",";
        ofs << vecToStr(packet_send) << ",";
        ofs << vecToStr(packet_recv) << ",";
        ofs << vecToStr(restoredBlock) << ",";
        ofs << vecToStr(syndrome) << ",";
        ofs << to_string(uncoded_check) << ",";
        ofs << to_string(restore_check) << "\n";
    }

    void fileOpen(){ofs.open("data.txt", ios::out);}

private:
    int errorProb100;   //error발생 확률을 백분율 값으로 저장
    vector<bool> msgBlock;
    vector<bool> packet_send;
    vector<bool> packet_recv;
    vector<bool> restoredBlock;
    vector<bool> syndrome;
    int restore_check; //0: 전송과정 에러 x, 1: 에러를 고치지 못함, 2: 에러가 성공적으로 수정됨
    int uncoded_check; //0: 전송과정 에러 x, 1: packet에 에러 존재
    ofstream ofs;

};