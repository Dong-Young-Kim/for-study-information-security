#include "global.h"
#include <random>

int randomCode(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 15);
    return dis(gen);
}

bool bitError(int errorProb100){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 99);

    //errorProb100이 1 (에러 발생 확률 1%) 이라면 dis(gen)이 1% 확률로 0이 나와 error발생여부를 전달
    return (dis(gen) < errorProb100);

    // if (dis(gen) < errorProb100) return false;
    // else return true;
}

void mul_m_G(vector<bool>&m, bool G[4][7], vector<bool>&c){
    c.clear();
    for(int col = 0; col < 7; col++){
        bool tmp = 0;
        for (int k = 0; k < 4; k++){
            tmp = tmp ^ m[k] & G[k][col];
        }
        c.push_back(tmp);
    }
}

void mul_c_Ht(vector<bool>&c, bool H[7][3], vector<bool>&s){
    s.clear();
    for(int col = 0; col < 3; col++){
        bool tmp = 0;
        for (int k = 0; k < 7; k++){
            tmp = tmp ^ c[k] & H[k][col];
        }
        s.push_back(tmp);
    }
}