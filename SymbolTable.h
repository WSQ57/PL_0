//
// Created by WANG on 20/5/2022.
//

#ifndef PL_0_SYMBOL_TABLE_H
#define PL_0_SYMBOL_TABLE_H
#include<vector>
#include<string>
#include "AST.h"
struct TableItem{ // 表项
    std::string name;
    lexType kind{};
    int val{};
    int address{};
    TableItem(std::string name, lexType kind, int val = 0, int address = -1){
        val = val;
        address = address;
        name = name;
        kind = kind;
    }
};
class SymbolTable { // 表
public:
    int level{};
    int faLink{}; // 指向直接外层表
    int nowAddress{};
    std::vector<TableItem> OneTable;
    explicit SymbolTable(int level = 0, int faLink = -1){
        level = level;
        faLink = faLink;
        nowAddress = 0;
    }
    void emplace_back(const std::string& name, lexType kind, int val = 0){
        if(kind == Number)
            OneTable.emplace_back(name,kind,val);
        if(kind == Producer || kind == Identifier)
            OneTable.emplace_back(name,kind,val,nowAddress++);
    }

    int findItem(const std::string& name){
        for(int i = 0; i < OneTable.size(); i++){
            if(OneTable[i].name == name)
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
    pair<int,int> lookup(int tablePos,std::string name);
    void backPatchProc(int tablePos,int nxq);
    TableItem getData(int tableNum,int pos);
    void print();
};

#endif //PL_0_SYMBOL_TABLE_H
