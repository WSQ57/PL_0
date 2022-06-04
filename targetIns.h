//
// Created by WANG on 3/6/2022.
//

#ifndef PL0_COMPILE_TARGETINS_H
#define PL0_COMPILE_TARGETINS_H
#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
using namespace std;
struct formatInstruction{
    string f;
    int l;
    int a;
    explicit formatInstruction(string F,int L = 0,int A = 0){
        f = std::move(F);
        l = L;
        a = A;
    }
};

struct WaitingPatch{
    int addr;
    int tableNum;
    int tableLine;
    WaitingPatch(int a,int n,int l){
        addr = a;
        tableNum = n;
        tableLine = l;
    }
};

class targetIns {
private:
    vector<formatInstruction> code;

public:
    vector<WaitingPatch> toBackPatch;
    targetIns(){
        code.emplace_back("JMP");
    }
    void push(const string& f,int l,int a){
        code.emplace_back(f,l,a);
    }
    void waitBackPatch(int addr,int tableNum,int tableLine){
        toBackPatch.emplace_back(addr,tableNum,tableLine);
    }
    int size(){
        return (int)code.size();
    }
    void print(){
        int count = 0;
        for(auto & i:code){
            cout<<count++<<" "<<i.f<<" "<<i.l<<" "<<i.a<<endl;
        }
    }
    int backPatch(int i,int addr){
        code[i].a = addr;
        return i;
    }

    vector<formatInstruction> getInst(){
        return code;
    }
};


#endif //PL0_COMPILE_TARGETINS_H
