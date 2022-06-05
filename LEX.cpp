//
// Created by WANG on 2022/4/23.
//

#include "LEX.h"
LEX::LEX(const set<string> &kWord, const set<string> &OSymbol, const set<string> &SSymbol) {
    key_word = kWord;
    operator_symbol = OSymbol;
    separator_symbol = SSymbol;
    InitJumpMap();
}

void LEX::InitJumpMap() {
    //状态10为结束，回归开始状态0，状态9为错误状态
    //状态0
    for(char i = 32; i <= 126; i++) JumpState[0][i] = 9;
    for(auto & i : separator_symbol) JumpState[0][i[0]] = 1;
    for(auto & i : operator_symbol) JumpState[0][i[0]] = 2;
    //后有可能跟 =
    JumpState[0]['<'] = 3;
    JumpState[0]['>'] = 3;
    JumpState[0][':'] = 4;

    for(char i = 'A'; i <= 'Z'; i++) JumpState[0][i] = 5;
    for(char i = '0'; i <= '9'; i++) JumpState[0][i] = 6;

    //状态1任意符号跳转到10
    for(char i = 32; i <= 126; i++) JumpState[1][i] = 10;   // 可显示的字符
    //状态2任意符号跳转到10
    for(char i = 32; i <= 126; i++) JumpState[2][i] = 10;   // 可显示的字符
    //状态3
    for(char i = 32; i <= 126; i++) JumpState[3][i] = 10;
    JumpState[3]['='] = 7;
    //状态4
    for(char i = 32; i <= 126; i++) JumpState[4][i] = 9;
    JumpState[4]['='] = 8;
    //状态5
    for(char i = 32; i <= 126; i++) JumpState[5][i] = 10;
    for(char i = 'A'; i <= 'Z'; i++) JumpState[5][i] = 5;   //覆盖
    for(char i = '0'; i <= '9'; i++) JumpState[5][i] = 5;
    //状态6
    for(char i = 32; i <= 126; i++) JumpState[6][i] = 10;
    for(char i = '0'; i <= '9'; i++) JumpState[6][i] = 6;
    for(char i = 'A'; i <= 'Z'; i++) JumpState[6][i] = 9;   //数字开头不可字母
    //状态7
    for(char i = 32; i <= 126; i++) JumpState[7][i] = 10;
    //状态8
    for(char i = 32; i <= 126; i++) JumpState[8][i] = 10;
}

int LEX::getNextState(int nowState, char ch, const string & word) {
    int nextState = JumpState[nowState][ch];
    if(nextState != 10) return nextState;
    else{
        //长度过长
        if(word.size() > 10) return 9;
        return 10;
    }
}

bool LEX::lexical_analysis(const string &fileName, vector<string> &sym, vector<string> &num) {
    //文件读取
    stringstream oss;
    if(!fileName.empty()) {
        ifstream in(fileName);
        assert(("文件打开失败", in.is_open()));
        oss << in.rdbuf();
        string file_content = oss.str();
        transform(file_content.begin(), file_content.end(),
                  file_content.begin(), ::toupper);  //改为大写
        oss.str("");
        oss<<file_content;
    }
    else{
        string a;
        while(getline(cin,a)) {
            oss << a;
            oss <<" ";
        }
        string file_content = oss.str();
        transform(file_content.begin(), file_content.end(),
                  file_content.begin(), ::toupper);  //改为大写
        oss.str("");
        oss<<file_content;
    }
    string tmp;
    int nowState = 0;
    int nextState;
    string word;
    while (oss >> tmp) {
        for(int i = 0; i < tmp.size(); i++) {
            nextState = getNextState(nowState,tmp[i],word);
            if(nextState == 9){
                cout<<"Lexical Error\n";
                return false;
            }
            if(nextState == 10){
                i--;nextState = 0;
                if(nowState == 6){
                    //数字
                    string outStr = "NUMBER " + to_string(stoi(word));
                    ans.push_back(outStr);
                    param.emplace_back(to_string(stoi(word)),Number);
                }
                else if(nowState == 5){
                    //标识符
                    string outStr;
                    if(key_word.find(word) == key_word.end()) {
                        outStr = "IDENTIFIER " + word;
                        param.emplace_back(word,Identifier);
                    }
                    else {
                        outStr = word;
                        param.emplace_back(word,KeyWord);
                    }
                    ans.push_back(outStr);
                }
                else {
                    ans.push_back(word);
                    param.emplace_back(word,Other);
                }
                word.clear();
                nowState = nextState;
                continue;
            }
            word.push_back(tmp[i]);
            nowState = nextState;
        }

        if(!word.empty()){
            nextState = getNextState(nowState,' ',word);
            if(nextState == 9){
                cout<<"Lexical Error\n";
                return false;
            }
            if(nextState == 10){
                if(nowState == 6){
                    //数字
                    string outStr = "NUMBER " + to_string(stoi(word));
                    ans.push_back(outStr);
                    param.emplace_back(to_string(stoi(word)),Number);
                }
                else if(nowState == 5){
                    //标识符
                    string outStr;
                    if(key_word.find(word) == key_word.end()) {
                        outStr = "IDENTIFIER " + word;
                        param.emplace_back(word,Identifier);
                    }
                    else {
                        outStr = word;
                        param.emplace_back(word,KeyWord);
                    }
                    ans.push_back(outStr);
                }
                else {
                    ans.push_back(word);
                    param.emplace_back(word,Other);
                }
                nowState = 0;
                word.clear();
            }
        }
    }

    return true;
}

void LEX::printParam() {
    out.open("lex.out");
    for(auto & i:param){
        out<<i.name<<" "<<i.type<<endl;
    }
    out.close();
}

void LEX::printAns() {
    out.open("lex.out");
    for(auto & i:ans)
        out<<i<<endl;
    out.close();
}