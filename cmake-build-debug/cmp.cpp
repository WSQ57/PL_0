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
#define StateNum 10
using namespace std;

class LEX {
private:
    set<string> key_word;   //关键词
    set<string> operator_symbol;    //运算符
    set<string> separator_symbol;   //分隔符
    map<char,int> JumpState[StateNum];    //状态转换表
    void InitJumpMap();
    int getNextState(int nowState,char ch, const string & word);
public:
    LEX(const set<string> & kWord, const set<string> & OSymbol, const set<string> & SSymbol);
    bool lexical_analysis(const string &fileName, vector<string> &ans,
                          vector<string> &sym, vector<string> &num);
};

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
    for(char i = 'A'; i <= 'Z'; i++) JumpState[6][i] = 9;   //数字开头不可字幕
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

bool LEX::lexical_analysis(const string &fileName, vector<string> &ans,
                           vector<string> &sym, vector<string> &num) {
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
                }
                else if(nowState == 5){
                    //标识符
                    string outStr;
                    if(key_word.find(word) == key_word.end()) outStr = "IDENTIFIER " + word;
                    else outStr = word;
                    ans.push_back(outStr);
                }
                else ans.push_back(word);
                word.clear();
                nowState = nextState;
                continue;
            }
            word.push_back(tmp[i]);
            nowState = nextState;
        }

        if(!word.empty()){  //遇到空格
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
                }
                else if(nowState == 5){
                    //标识符
                    string outStr;
                    if(key_word.find(word) == key_word.end()) outStr = "IDENTIFIER " + word;
                    else outStr = word;
                    ans.push_back(outStr);
                }
                else ans.push_back(word);
                nowState = 0;
                word.clear();
            }
        }
    }

    return true;
}

int main() {
    // freopen("ans.out","w",stdout);
    vector<string> sym;
    vector<string> name;
    vector<string> ans;
    set<string> KWord = {"CONST", "VAR", "PROCEDURE", "BEGIN",
                            "END", "ODD",  "IF", "THEN",
                            "CALL", "WHILE", "DO", "READ", "WRITE"};
    set<string> OSymbol = {"=", ":=", "#", "<", "<=",
                           ">", ">=","+", "-", "*","/"};
    set<string> SSymbol = {"(", ")", ",", ";","."};
    LEX Lex_machine(KWord,OSymbol,SSymbol);
    // string fileName = "more.in";
   string fileName;
    if(Lex_machine.lexical_analysis(fileName,ans,sym,name)){
        for(auto & i: ans)
            cout<<i<<endl;
    }

    return 0;
}