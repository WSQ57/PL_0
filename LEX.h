//
// Created by WANG on 2022/4/23.
//

#ifndef PL_0_LEX_H
#define PL_0_LEX_H
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <vector>
#include <set>
#include <map>
#include "AST.h"
#define StateNum 10
using namespace std;

class LEX {
private:
    set<string> key_word;   //关键词
    set<string> operator_symbol;    //运算符
    set<string> separator_symbol;   //分隔符
    vector<wordTuple> param; // 用于语法分析
    map<char,int> JumpState[StateNum];    //状态转换表
    vector<string> ans;
    ofstream out;
    void InitJumpMap();
    int getNextState(int nowState,char ch, const string & word);
public:
    LEX(const set<string> & kWord, const set<string> & OSymbol, const set<string> & SSymbol);
    bool lexical_analysis(const string &fileName,vector<string> &sym, vector<string> &num);
    void printParam();
    void printAns();
    vector<wordTuple> getParam() {return param;}
};


#endif //PL_0_LEX_H
