//
// Created by WANG on 4/6/2022.
//

#ifndef PL_0_INTERPRETER_H
#define PL_0_INTERPRETER_H
#include "targetIns.h"
class interpreter {
private:
    vector<formatInstruction> code;
    vector<int> dataStack;
    map<string,int> targetCode{
            {"LIT",1},
            {"LOD",2},
            {"STO",3},
            {"CAL",4},
            {"INT",5},
            {"JMP",6},
            {"JPC",7},
            {"OPR",8}
    };

    int ra,obp,sbp;
    // 存放正在执行的指令。 存放下一条指令的地址。 指向运行栈的顶端。指向当前过程调用所分配的空间在栈中的起始地址。
    int IR,IP,SP,BP; // 指令寄存器 指令地址寄存器 栈顶寄存器 基址寄存器
#define RA dataStack[BP] // 返回地址
#define TOP dataStack[SP] // 栈顶
#define OBP dataStack[BP+1] // 动态链
#define SBP dataStack[BP+2] // 静态链
#define SEC dataStack[SP-1]  // 第二元素

public:
    explicit interpreter(targetIns input);
    void execute();
    void AllocateSpace(int num);
    int getTargetLevel(int levelDiff);
};


#endif //PL_0_INTERPRETER_H
