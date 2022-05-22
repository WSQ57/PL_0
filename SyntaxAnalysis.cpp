//
// Created by WANG on 20/5/2022.
//

#include "SyntaxAnalysis.h"

#include <utility>

using namespace std;

SyntaxAnalysis::SyntaxAnalysis(vector<wordTuple> param2) { // NOLINT
    nxq = 0;
    paramPos = 0;
    tmpCount = 0;
    param = std::move(param2);
}

// <程序>→<分程序>.
void SyntaxAnalysis::handleProc() {
    ASTNode root("PROGRAM");
    int pos = SyntaxTree.addNode(root);
    handleSubProc(0, pos, 0);
    if(param[paramPos].name != ".")
        throw handleError("语法错误 . handleProc");

    SyntaxTree.addNode(param[paramPos++], pos); // 加入CONST
    if(paramPos != param.size())
        throw handleError("未遍历完 handleProc");
}

// <分程序>→ [<常量说明部分>][<变量说明部分>][<过程说明部分>]<语句>
void SyntaxAnalysis::handleSubProc(int tablePos, int fa, int depth) { // NOLINT
    int pos = SyntaxTree.addNode(ASTNode("SUB PROGRAM"), fa);
    string tmp = param[paramPos].name;
    if (tmp == "CONST") {
        DeclareConst(tablePos, pos);
    }
    if (tmp == "VAR") {
        DeclareVar(tablePos, pos);
    }
    if (tmp == "PROCEDURE") {
        DeclareProcedure(tablePos, pos, depth);
    }
    nameTable.backPatchProc(tablePos,nxq); // 回填子程序四元式地址

    gen("ret","-","-","-");
}

// <常量说明部分> → CONST<常量定义>{ ,<常量定义>};
void SyntaxAnalysis::DeclareConst(int tablePos, int fa) {
    int pos = SyntaxTree.addNode(ASTNode("CONSTANT DECLARE"), fa);

    SyntaxTree.addNode(param[paramPos++], pos); // 加入CONST
    DefineConst(tablePos, pos);

    while (param[paramPos].name == ",") {
        // define multiple const
        SyntaxTree.addNode(param[paramPos++], fa); // add "," 存疑
        DefineConst(tablePos, pos);
    }
    if (param[paramPos].name != ";") throw handleError("语法错误 DeclareConst");
    else SyntaxTree.addNode(param[paramPos++], pos);
}

// <常量定义> → <标识符>=<无符号整数>
void SyntaxAnalysis::DefineConst(int tablePos, int fa) {
    int pos = SyntaxTree.addNode(ASTNode("CONSTANTDEFINE"), fa);

    if (param[paramPos].type != Identifier) throw handleError("语法错误 Identifier DefineConst"); // 后不跟标识符说明语法错误
    string name = param[paramPos].name;
    SyntaxTree.addNode(param[paramPos++], pos); // 加入标识符

    if (param[paramPos].name != "=") throw handleError("语法错误 \"=\" DefineConst");
    SyntaxTree.addNode(param[paramPos++], pos); // add "="

    if (param[paramPos].type != Number) throw handleError("语法错误 Number DefineConst");
    int tmpVal = stoi(param[paramPos].name);
    SyntaxTree.addNode(param[paramPos++], pos); // add number

    // 加入符号表
    if (!nameTable.enter(tablePos, name, CONST, tmpVal)) {// 加入常量
        throw handleError("重复定义 DefineConst");
    }
}

//  <变量说明部分> → VAR<标识符>{ ,<标识符>};
void SyntaxAnalysis::DeclareVar(int tablePos, int fa) {
    int pos = SyntaxTree.addNode(ASTNode("VARIABLE DECLARE"), fa);
    SyntaxTree.addNode(param[paramPos++], pos); // 加入VAR

    while (true) {
        if (param[paramPos].type != Identifier) throw handleError("语法错误 Identifier DeclareVar");
        string name = param[paramPos].name;
        SyntaxTree.addNode(param[paramPos++], pos);
        if(!nameTable.enter(tablePos, name, Identifier)) { // 加入变量
            throw handleError("重复定义 DeclareVar");
        }
        if (param[paramPos++].name != ",") break;
    }
    if (param[paramPos].name != ";") throw handleError("语法错误 \";\" DeclareVar");
    else SyntaxTree.addNode(param[paramPos++], pos);
}

// <过程说明部分> → <过程首部><分程序>;{<过程说明部分>}
//  <过程首部> → procedure<标识符>;
void SyntaxAnalysis::DeclareProcedure(int tablePos, int depth, int fa) { //NOLINT
    if (depth >= 3) throw handleError("栈溢出 DeclareProcedure");
    int pos = SyntaxTree.addNode(ASTNode("PROCEDURE DECLARE"),fa);

    //  <过程首部> → procedure<标识符>;
    int pos2 = SyntaxTree.addNode(ASTNode("PROCEDURE HEAD"), pos);
    string name = param[paramPos].name;
    SyntaxTree.addNode(param[paramPos++], pos2); // add "producer"
    int newTablePos = nameTable.mkTable(tablePos, name);

    if (param[paramPos].type != Identifier) throw handleError("语法错误 Identifier DeclareProcedure");
    SyntaxTree.addNode(param[paramPos++], pos2); // add producer name

    if (param[paramPos].name != ";") throw handleError("语法错误 \";\" DeclareProcedure");
    SyntaxTree.addNode(param[paramPos++], pos2);


    // 程序嵌套
    if (param[paramPos].name == "PROCEDURE")
        handleSubProc(newTablePos, pos, depth + 1);

}

// <语句> → <赋值语句>|<条件语句>|<当型循环语句>|<过程调用语句>|<读语句>|<写语句>|<复合语句>|<空>
void SyntaxAnalysis::handleSentence(int tablePos, int fa) { // NOLINT
    int pos = SyntaxTree.addNode(ASTNode("SENTENCE"),fa);
    if(param[paramPos].type == Identifier) // 赋值语句
        AssignmentValue(tablePos,pos);
    else if(param[paramPos].name == "BEGIN"){
        handleBegin(tablePos,pos);
    }
    else if(param[paramPos].name == "IF"){
        handleIf(tablePos,pos);
    }
    else if(param[paramPos].name == "WHILE"){
        handleWhile(tablePos,pos);
    }
    else if(param[paramPos].name == "CALL"){
        handleCall(tablePos,pos);
    }
    else if(param[paramPos].name == "READ"){
        handleRead(tablePos,pos);
    }
    else if(param[paramPos].name == "Write"){
        handleWrite(tablePos,pos);
    }
    else SyntaxTree.addNode(ASTNode("EMPTY"),pos);

}

// <赋值语句> → <标识符>:=<表达式>
void SyntaxAnalysis::AssignmentValue(int tablePos, int fa) {
    int pos = SyntaxTree.addNode(ASTNode("ASSIGNMENT"),fa);
    string name = param[paramPos].name;
    SyntaxTree.addNode(param[paramPos++],pos); // 加入赋值的变量
    if(param[paramPos].name != ":=" && param[paramPos].name != "=")
        throw handleError("语法错误 AssignmentValue");
    paramPos++;
    Expression(tablePos,pos);

    if(nameTable.lookup(tablePos,name).first == -1)
        throw handleError("未声明的标识符 AssignmentValue");
    else{
        string arg = attribute_stack.top().name; attribute_stack.pop();
        gen("=",arg,"-",name);
    }
}

//  <表达式> → [+|-]<项>{<加减运算符><项>}
void SyntaxAnalysis::Expression(int tablePos, int fa) { // NOLINT
    int pos = SyntaxTree.addNode(ASTNode("EXPRESSION"),fa);
    bool negative = false;
    if(param[paramPos].name == "+" || param[paramPos].name == "-"){
        if(param[paramPos].name == "-") negative = true;
        SyntaxTree.addNode(param[paramPos++],pos);
    }

    handleItem(tablePos,pos);

    if(negative){
        // 是负数
        string arg = attribute_stack.top().name;attribute_stack.pop();
        string tmp = tmpVariable();
        attribute_stack.push({tmp,0,0});
        gen("@",arg,"0",tmp);
    }

    while(true){
        string name;
        if(param[paramPos].name == "+" || param[paramPos].name == "-"){
            if(param[paramPos].name == "-") negative = true;
            name = param[paramPos].name;
            SyntaxTree.addNode(param[paramPos++],pos);
        }
        else break;

        handleItem(tablePos,pos);

        string arg1,arg2;
        arg2 = attribute_stack.top().name; attribute_stack.pop();
        arg1 = attribute_stack.top().name; attribute_stack.pop();
        string tmp = tmpVariable(); // 申请临时变量
        attribute_stack.push({tmp,0,0});
        gen(name,arg1,arg2,tmp);
    }
}

//  <项> → <因子>{<乘除运算符><因子>}
void SyntaxAnalysis::handleItem(int tablePos, int fa) { // NOLINT
    int pos = SyntaxTree.addNode(ASTNode("ITEM"),fa);
    handleFactor(tablePos,pos);
    while(true){
        if(param[paramPos].name == "*" ||param[paramPos].name == "/")
            SyntaxTree.addNode(param[paramPos++],pos);
        else break;
        handleFactor(tablePos,pos);

        string arg1,arg2;
        arg2 = attribute_stack.top().name; attribute_stack.pop();
        arg1 = attribute_stack.top().name; attribute_stack.pop();
        string tmp = tmpVariable(); // 申请临时变量
        attribute_stack.push({tmp,0,0});
        gen(param[paramPos].name,arg1,arg2,tmp);

    }
}
//  <因子> → <标识符>|<无符号整数>|(<表达式>)
void SyntaxAnalysis::handleFactor(int tablePos, int fa) { // NOLINT
    int pos = SyntaxTree.addNode(ASTNode("FACTOR"),fa);
    if(param[paramPos].type == Identifier){

        if(nameTable.lookup(tablePos,param[paramPos].name).first == -1)
            throw handleError("未定义的变量 handleFactor");
        else attribute_stack.push({param[paramPos].name,0,0}); // 加入属性栈
        SyntaxTree.addNode(param[paramPos++],pos); // 加入标识符
    }
    else if(param[paramPos].type == Number){
        attribute_stack.push({param[paramPos].name,0,0}); // 加入属性栈
        SyntaxTree.addNode(param[paramPos++],pos); // 加入数字
    }
    else{
        //()
        if(param[paramPos].name != "(") throw handleError("语法错误 ( handleFactor");
        else SyntaxTree.addNode(param[paramPos++],pos);

        Expression(tablePos,pos);

        if(param[paramPos].name != ")") throw handleError("语法错误 ) handleFactor");
        else SyntaxTree.addNode(param[paramPos++],pos);
    }

}
//  <复合语句> → begin<语句>{ ;<语句>}<end>
void SyntaxAnalysis::handleBegin(int tablePos, int fa) { // NOLINT
    int pos = SyntaxTree.addNode(ASTNode("COMBINED"),fa);
    SyntaxTree.addNode(param[paramPos++],pos); // 加入begin
    handleSentence(tablePos,pos); // 处理语句
    while(true){
        if(param[paramPos].name != ";") break;
        SyntaxTree.addNode(param[paramPos++],pos); // add ";"
        handleSentence(tablePos,pos);
    }
    if(param[paramPos].name != "END") throw handleError("语法错误 END handleBegin");
    SyntaxTree.addNode(param[paramPos++],pos);
}

// <条件语句> → if<条件>then<语句>
void SyntaxAnalysis::handleIf(int tablePos, int fa) { // NOLINT
    int pos = SyntaxTree.addNode(ASTNode("IF SENTENCE"),fa);
    SyntaxTree.addNode(param[paramPos++],pos); // 加入IF
    handleCondition(tablePos,pos);

    //回填
    int trueList = attribute_stack.top().trueList;
    int falseList = attribute_stack.top().falseList;
    attribute_stack.pop();
    backPatch(trueList,nxq);
    if(param[paramPos].name != "THEN") throw handleError("语法错误 then handleIf");
    SyntaxTree.addNode(param[paramPos++],pos); // 加入THEN
    handleSentence(tablePos,pos);
    backPatch(falseList, nxq);
}
// <条件> → <表达式><关系运算符><表达式> | odd<表达式>
void SyntaxAnalysis::handleCondition(int tablePos, int fa) {
    int pos = SyntaxTree.addNode(ASTNode("CONDITION"),fa);
    if(param[paramPos].name == "ODD"){
        SyntaxTree.addNode(param[paramPos++],pos); // 加入ODD
        Expression(tablePos,pos); // 处理项

        string arg1 = attribute_stack.top().name;attribute_stack.pop();
        string tmp = tmpVariable();
        attribute_stack.push({tmp,nxq+1,nxq+2});
        gen("ODD",arg1,"-",tmp);
        gen("jnz",tmp,"-","0"); // 等待回填
        gen("j","-","-","0"); // 等待回填
    }
    else{
        Expression(tablePos,pos); // 处理项
        string op = param[paramPos].name;
        if(op=="="||op=="#"||op=="<"||op=="<="||op==">"||op==">=")
            SyntaxTree.addNode(param[paramPos++],pos); // 加入op
        else throw handleError("语法错误 handleCondition");
        Expression(tablePos,pos); // 处理项

        string arg1,arg2;
        arg2 = attribute_stack.top().name; attribute_stack.pop();
        arg1 = attribute_stack.top().name; attribute_stack.pop();
        string tmp = tmpVariable(); // 申请临时变量
        attribute_stack.push({tmp,nxq+1,nxq+2});
        gen(op,arg1,arg2,tmp);
        gen("jnz",tmp,"-","0"); // 等待回填
        gen("j","-","-","0"); // 等待回填
    }
}

//  <当型循环语句> → while<条件>do<语句>
void SyntaxAnalysis::handleWhile(int tablePos, int fa) { // NOLINT
    int pos = SyntaxTree.addNode(ASTNode("CONDITION"),fa);
    SyntaxTree.addNode(param[paramPos++],pos); // 加入WHILE
    int conditionPos = nxq;
    handleCondition(tablePos,pos);
    if(param[paramPos].name != "DO")
        throw handleError("语法错误 DO handleWhile");
    SyntaxTree.addNode(param[paramPos++],pos); // 加入DO

    int trueList = attribute_stack.top().trueList;
    int falseList = attribute_stack.top().falseList;
    attribute_stack.pop();
    backPatch(trueList,nxq);
    handleSentence(tablePos,pos);
    gen("j","-","-", to_string(conditionPos));
    backPatch(falseList,nxq);
}

// <过程调用语句> → call<标识符>
void SyntaxAnalysis::handleCall(int tablePos, int fa) {
    int pos = SyntaxTree.addNode(ASTNode("CALL SENTENCE"),fa);
    SyntaxTree.addNode(param[paramPos++],pos); // 加入CALL
    if(param[paramPos].type != Identifier)
        throw handleError("语法错误 Identifier handleCall");

    SyntaxTree.addNode(param[paramPos],pos); // 加入标识符
    auto tmp = nameTable.lookup(tablePos,param[paramPos].name);
    if(tmp.first == -1)
        throw handleError("标识符未定义 handleCall");
    auto target = nameTable.getData(tmp.first,tmp.second);
    if(target.kind != Producer)
        throw handleError("跳转错误 handleCall");
    gen("jal","-","-", to_string(target.val));
}

string SyntaxAnalysis::tmpVariable() {
    string tmp = "T" + to_string(tmpCount++);
    return tmp;
}

//  <读语句> → read(<标识符>{ , <标识符>})
void SyntaxAnalysis::handleRead(int tablePos, int fa) {
    int pos = SyntaxTree.addNode(ASTNode("READ SENTENCE"),fa);
    SyntaxTree.addNode(param[paramPos++],pos); // 加入READ
    if(param[paramPos].name != "(")
        throw handleError("语法错误 ( handleRead");
    SyntaxTree.addNode(param[paramPos++],pos); // 加入(

    if(param[paramPos].type != Identifier)
        throw handleError("语法错误 Identifier handleRead");
    SyntaxTree.addNode(param[paramPos++],pos); // 加入标识符

    while(true){
        if(param[paramPos].name != ",") break;
        SyntaxTree.addNode(param[paramPos++],pos); // 加入,
        if(param[paramPos].type != Identifier)
            throw handleError("语法错误 Identifier handleRead");
        SyntaxTree.addNode(param[paramPos++],pos); // 加入标识符
    }
    if(param[paramPos].name != ")")
        throw handleError("语法错误 ) handleRead");
    SyntaxTree.addNode(param[paramPos++],pos); // 加入(
}

// <写语句> → write(<标识符>{，<标识符>})
void SyntaxAnalysis::handleWrite(int tablePos, int fa) {
    int pos = SyntaxTree.addNode(ASTNode("write SENTENCE"),fa);
    SyntaxTree.addNode(param[paramPos++],pos); // 加入write
    if(param[paramPos].name != "(")
        throw handleError("语法错误 ( handleWrite");
    SyntaxTree.addNode(param[paramPos++],pos); // 加入(

    if(param[paramPos].type != Identifier)
        throw handleError("语法错误 Identifier handleWrite");
    SyntaxTree.addNode(param[paramPos++],pos); // 加入标识符

    while(true){
        if(param[paramPos].name != ",") break;
        SyntaxTree.addNode(param[paramPos++],pos); // 加入,
        if(param[paramPos].type != Identifier)
            throw handleError("语法错误 Identifier handleWrite");
        SyntaxTree.addNode(param[paramPos++],pos); // 加入标识符
    }
    if(param[paramPos].name != ")")
        throw handleError("语法错误 ) handleWrite");
    SyntaxTree.addNode(param[paramPos++],pos); // 加入(

}

void SyntaxAnalysis::backPatch(int pos, int nextStat) {
    quadruple[pos].result = to_string(nextStat);
}

void SyntaxAnalysis::gen(const string& op, const string& arg1, const string& arg2, const string& result) {
    quadruple.emplace_back(op,arg1,arg2,result);
    nxq++;
}

void SyntaxAnalysis::printTree() {
    SyntaxTree.printTree(0);
}

void SyntaxAnalysis::printTable() {
    nameTable.print();
}

void SyntaxAnalysis::printQuadruple() {
    int cnt = 100;
    for(const auto& x:quadruple){
        cout<<cnt++<<" ("<<x.op<<","<<x.arg1<<","<<x.arg2<<","<<x.result<<endl;
    }
}
