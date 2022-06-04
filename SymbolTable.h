//
// Created by WANG on 20/5/2022.
//

#ifndef PL_0_SYMBOL_TABLE_H
#define PL_0_SYMBOL_TABLE_H
#include <utility>
#include<vector>
#include<string>
#include "AST.h"
struct TableItem{ // 表项
    std::string name;
    lexType kind{};
    int val{};
    int address{};
    TableItem(std::string Name, lexType Kind, int Val = 0, int Address = -1){
        val = Val;
        address = Address;
        name = std::move(Name);
        kind = Kind;
    }
};
class SymbolTable { // 表
public:
    int level{};
    int faLink{}; // 指向直接外层表
    int nowAddress{};
    int variableNum; // 变量总数
    std::vector<TableItem> OneTable;
    explicit SymbolTable(int Level = 0, int FaLink = -1){
        level = Level;
        faLink = FaLink;
        nowAddress = 0;
        variableNum = 0;
    }
    void emplace_back(const std::string& Name, lexType Kind, int Val = 0){
        if(Kind == Variable) variableNum++;
        if(Kind == Number)
            OneTable.emplace_back(Name,Kind,Val);
        if(Kind == Producer || Kind == Identifier || Kind == CONST || Kind == Variable)
            OneTable.emplace_back(Name,Kind,Val,nowAddress++);
    }

    int findItem(const std::string& Name){
        for(int i = 0; i < OneTable.size(); i++){
            if(OneTable[i].name == Name)
                return i;
        }
        return -1;
    }
};

class TableSet{ // 整体符号表
public:
    std::vector<SymbolTable> tables; // 多个表
    TableSet();
    int mkTable(int fa, const string& producer);
    bool enter(int tablePos,const std::string& name, lexType kind, int val = 0);
    pair<int,int> lookup(int tablePos,const std::string& name);
    void backPatchProc(int tablePos,int nxq);
    int getLevelDiff(int table1,int table2);
    int getVarNum(int tablePos);
    TableItem getData(int tableNum,int pos);
    void print();
};

#endif //PL_0_SYMBOL_TABLE_H
