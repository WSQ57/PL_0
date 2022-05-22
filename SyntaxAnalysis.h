//
// Created by WANG on 20/5/2022.
//

#ifndef PL_0_SYNTAX_ANALYSIS_H
#define PL_0_SYNTAX_ANALYSIS_H
#include "SymbolTable.h"
#include <stack>
using namespace std;

struct attribute{
    string name;
    int trueList{};
    int falseList{};
    attribute(string name,int trueList,int falseList){
        name = name;
        trueList = trueList;
        falseList = falseList;
    }
};

struct Quadruple{
    // 四元式
    string op;
    string arg1;
    string arg2;
    string result;
    Quadruple(string op, string arg1, string arg2, string result){
        op = op;
        arg1 = arg1;
        arg2 = arg2;
        result = result;
    }
};



class SyntaxAnalysis {
private:
    AST SyntaxTree; // 语法树
    vector<wordTuple> param;
    vector<Quadruple> quadruple; // 生成四元式
    TableSet nameTable;
    stack<attribute> attribute_stack;
    int nxq;
    int paramPos; // 遍历param
    int tmpCount; // 临时变量
public:
    explicit SyntaxAnalysis(vector<wordTuple> param2);
    void handleProc();
    void handleSubProc(int tablePos, int fa, int depth);
    void DefineConst(int tablePos, int fa);
    void DeclareConst(int tablePos, int fa);
    void DeclareVar(int tablePos, int fa);
    void DeclareProcedure(int tablePos,int depth, int fa);
    void handleSentence(int tablePos,int fa);
    void AssignmentValue(int tablePos,int fa);
    void Expression(int tablePos,int fa);
    void handleItem(int tablePos,int fa);
    void handleFactor(int tablePos,int fa);
    void handleBegin(int tablePos,int fa);
    void handleIf(int tablePos,int fa);
    void handleCondition(int tablePos,int fa);
    void handleWhile(int tablePos,int fa);
    void handleCall(int tablePos,int fa);
    void handleRead(int tablePos,int fa);
    void handleWrite(int tablePos,int fa);
    string tmpVariable();
    void backPatch(int pos,int nextStat); // 回填
    void gen(const string& op,const string& arg1,const string& arg2,const string& result);
    void printTree();
    void printTable();
    void printQuadruple();
};


#endif
