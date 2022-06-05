//
// Created by WANG on 20/5/2022.
//

#ifndef PL_0_AST_H
#define PL_0_AST_H
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "handleError.h"
using namespace std;
enum lexType{Number,Identifier,KeyWord,Producer,Other,CONST,Variable};
struct wordTuple{
    string name;
    lexType type;
    explicit wordTuple(string Name = "",lexType Type=Other){
        name = std::move(Name);
        type = Type;
    }
};
class ASTNode{
public:
    string name;
    wordTuple val;
    vector<int> child;
    explicit ASTNode(string name1){
        name = std::move(name1);
    }
};

class AST {
public:
    AST(){
        out.open("AST.out");
    }
    ~AST(){out.close();}
    vector<ASTNode> AllNode; //所有节点
    ofstream out;
    int addNode(ASTNode node, int fa = -1);
    int addNode(wordTuple & node, int fa = 0);
    void printTree(int tree);
    int size() const{
        return (int)AllNode.size();
    }
};


#endif //PL_0_AST_H
