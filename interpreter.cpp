//
// Created by WANG on 4/6/2022.
//

#include "interpreter.h"

#include <utility>

interpreter::interpreter(targetIns input) {
    code = input.getInst();
    ra = obp = sbp = 0;
    BP = IR = IP = 0;
    SP = -1;

}

void interpreter::AllocateSpace(int num) {
    while(num--){
        //栈满，扩容
        if(SP == dataStack.size() - 1){
            dataStack.push_back(0);
        }
        SP++;
    }
}

int interpreter::getTargetLevel(int levelDiff) {
    int targetLevel = BP; // 当前起始地址
    while(levelDiff--) targetLevel = dataStack[targetLevel+2]; // 静态链
    return targetLevel;
}

void interpreter::execute() {
    while(true){
        IR = IP; IP++; // 下一条指令
        switch (targetCode[code[IR].f]) {
            // LIT 将立即数移到栈顶
            case 1:{
                AllocateSpace(1);
                dataStack[SP] = code[IR].a;
                break;
            }
            // LOD 将变量放入栈顶
            case 2:{
                AllocateSpace(1);
                int targetLevel = getTargetLevel(code[IR].l);
                dataStack[SP] = dataStack[targetLevel + code[IR].a];
                break;
            }
            // STO 将栈顶放入变量
            case 3:{
                dataStack[getTargetLevel(code[IR].l) + code[IR].a] = dataStack[SP--];
                break;
            }
            // CAL 调用过程
            case 4:{
                ra = IR + 1; // 返回地址
                obp = BP; // 动态链
                sbp = getTargetLevel(code[IR].l); // 静态链

                IP = code[IR].a; // 跳转
                BP = SP + 1; // 开辟新栈
                break;
            }
            // INT 开辟数据区
            case 5:{
                AllocateSpace(code[IR].a);
                RA = ra;OBP = obp;SBP=sbp; // 初始化
                break;
            }
            // JMP 无条件转移
            case 6:{
                IP = code[IR].a;
                break;
            }
            // JPC 条件转移
            case 7:{
                if(!TOP) IP = code[IR].a; // true
                SP--; // false;
                break;
            }
            // OPR 关系和算数运算
            case 8:{
                switch (code[IR].a) {
                    // return
                    case 0:{
                        // 程序执行完毕
                        if(!BP){
                            return;
                        }
                        IP = RA; // 返回
                        SP = BP - 1; // 栈顶
                        BP = OBP; // 旧BP
                        break;
                    }
                    // ODD
                    case 1:{
                        TOP &= 1;
                        break;
                    }
                    // ==
                    case 2:{
                        if(SEC == TOP) SEC = 1;
                        else SEC = 0;
                        SP--;
                        break;
                    }
                    // #
                    case 3:{
                        if(SEC != TOP) SEC = 1;
                        else SEC = 0;
                        SP--;
                        break;
                    }
                    // <
                    case 4:{
                        if(SEC < TOP) SEC = 1;
                        else SEC = 0;
                        SP--;
                        break;
                    }
                    // <=
                    case 5:{
                        if(SEC <= TOP) SEC = 1;
                        else SEC = 0;
                        SP--;
                        break;
                    }
                    // >
                    case 6:{
                        if(SEC > TOP) SEC = 1;
                        else SEC = 0;
                        SP--;
                        break;
                    }
                    // >=
                    case 7:{
                        if(SEC >= TOP) SEC = 1;
                        else SEC = 0;
                        SP--;
                        break;
                    }
                    // @
                    case 8:{
                        TOP = -TOP;
                        break;
                    }
                    // +
                    case 9:{
                        SEC = SEC + TOP;
                        SP--;
                        break;
                    }
                    // -
                    case 10:{
                        SEC = SEC - TOP;
                        SP--;
                        break;
                    }
                    // *
                    case 11:{
                        SEC = SEC * TOP;
                        SP--;
                        break;
                    }
                    // /
                    case 12:{
                        SEC = SEC / TOP;
                        SP--;
                        break;
                    }
                    // read
                    case 13:{
                        AllocateSpace(1);
                        scanf("%d",&TOP); // NOLINT
                        break;
                    }
                    case 14:{
                        printf("%d",TOP);
                        SP--;
                        break;
                    }
                }
                default:break;
            }
        }
    }
}