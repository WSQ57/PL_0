//
// Created by WANG on 20/5/2022.
//

#include "SymbolTable.h"
TableSet::TableSet() {
    tables.emplace_back(); // 新加一张空表
    tables[0].emplace_back("main",Producer,0);
}

int TableSet::mkTable(int fa, const string& producer) {
    if(fa == -1) {
        tables.emplace_back();
        tables.back().emplace_back(producer, Producer);
    }
    else{
        tables.emplace_back(tables[fa].level + 1, fa); // 新建一张表
        tables[fa].emplace_back(producer, Producer); // 旧表也加入一条
        tables.back().emplace_back(producer, Producer); // 表中第一行一定为程序
    }
    return (int)tables.size()-1;
}

bool TableSet::enter(int tablePos, const std::string& name, lexType kind, int val) {
    int posIter = tablePos;
    while(posIter != -1){
        int tmp = tables[posIter].findItem(name);
        if(tmp != -1) return false;
        posIter = tables[posIter].faLink;
    }
    tables[tablePos].emplace_back(name,kind,val);
    return true;
}

pair<int,int> TableSet::lookup(int tablePos, const std::string& name) {
    pair<int,int> ans; // first为表下标，second为项下标
    int nowPos = tablePos;
    while(nowPos != -1){
        ans.first = nowPos;
        ans.second = tables[nowPos].findItem(name);
        if(ans.second == -1){
            nowPos = tables[nowPos].faLink;
            if(nowPos < 0) break;
        }
        else return ans;
    }
    ans = make_pair(-1,-1);
    return ans;
}


void TableSet::backPatchProc(int tablePos, int nxq) {
    // 回填程序的四元式
    tables[tablePos].OneTable[0].val = nxq;
    string name = tables[tablePos].OneTable[0].name;
    int fa = tables[tablePos].faLink;
    if(fa == -1) return;
    int a = tables[fa].findItem(name);
    if(tables[fa].OneTable[a].kind == Producer)
        tables[fa].OneTable[a].val = nxq;
    else throw handleError("回填程序四元式未找到");
}

TableItem TableSet::getData(int tableNum,int pos) {
    return tables[tableNum].OneTable[pos];
}

void TableSet::print() {
    for(const auto& x:tables){
        cout<<"============================================"<<endl;
        for(const auto& a:x.OneTable)
            cout<<a.name<<" "<<a.kind<<" "<<a.val<<" "<<a.address<<endl;
    }
}

int TableSet::getLevelDiff(int table1, int table2) {
    return tables[table2].level - tables[table1].level;
}

int TableSet::getVarNum(int tablePos) {
    return tables[tablePos].variableNum;
}