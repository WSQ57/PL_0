//
// Created by WANG on 20/5/2022.
//

#ifndef PL_0_SYNTAX_ANALYSIS_H
#define PL_0_SYNTAX_ANALYSIS_H
#include "SymbolTable.h"
#include "targetIns.h"
#include <stack>
#include <utility>
using namespace std;

struct attribute{
    string name;
    int trueList{};
    int falseList{};
    attribute(string Name,int TrueList,int FalseList){
        name = std::move(Name);
        trueList = TrueList;
        falseList = FalseList;
    }
};

struct Quadruple{
    // 四元式
    string op;
    string arg1;
    string arg2;
    string result;
    Quadruple(string Op, string Arg1, string Arg2, string Result){
        op = std::move(Op);
        arg1 = std::move(Arg1);
        arg2 = std::move(Arg2);
        result = std::move(Result);
    }
};



class SyntaxAnalysis {
private:
    AST SyntaxTree; // 语法树
    vector<Quadruple> quadruple; // 生成四元式
    targetIns instruction; // 生成目标代码
    TableSet nameTable;
    stack<attribute> attribute_stack;
    int nxq;
    int paramPos; // 遍历param
    int tmpCount; // 声名临时变量的下标
    string file;
public:
    vector<wordTuple> param;
    map<string,int> operatorSymbol{
            {"ODD",1},{"=",2},{"#",3},{"<",4},
            {"<=",5},{">",6},{">=",7},{"@",8},
            {"+",9},{"-",10},{"*",11},{"/",12},
            {"READ",13},{"WRITE",14}
    };
    explicit SyntaxAnalysis(string fileName);
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
    void printInstruction();
    targetIns getTargetIns();
};


#endif
