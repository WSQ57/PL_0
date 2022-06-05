//
// Created by WANG on 20/5/2022.
//
#include "AST.h"

int AST::addNode(ASTNode node, int fa) {
    AllNode.emplace_back(node); //  加入新节点
    // 父节点连一条边到新节点
    if(fa >= 0) AllNode[fa].child.emplace_back(AllNode.size() - 1);
    return (int)AllNode.size() - 1;
}

int AST::addNode(wordTuple &node, int fa) {
    ASTNode tmp(node.name);
    tmp.val = node;

    AllNode.emplace_back(tmp); //  加入新节点
    // 父节点连一条边到新节点
    AllNode[fa].child.emplace_back(AllNode.size() - 1);
    return (int)AllNode.size() - 1;
}

void AST::printTree(int tree)  { // NOLINT

    int num = (int)AllNode[tree].child.size();
    if(!num){   //  叶子节点
        out<<AllNode[tree].name;
    }
    else{ // 内部节点
        out<<AllNode[tree].name<<"(";
        for(int i = 0; i < AllNode[tree].child.size(); i++){
            // 遍历所有的孩子
            printTree(AllNode[tree].child[i]);
            if (i < AllNode[tree].child.size() - 1)
                out<<",";
        }
        out<<")";
    }
    if(!tree) out<<"\n";
}