#include "LEX.h"
#include "SyntaxAnalysis.h"
int main() {
    freopen("tmp.out","w",stdout);
    vector<string> sym;
    vector<string> name;
    vector<string> ans;
    set<string> KWord = {"CONST", "VAR", "PROCEDURE", "BEGIN",
                            "END", "ODD",  "IF", "THEN",
                            "CALL", "WHILE", "DO", "READ", "WRITE"};
    set<string> OSymbol = {"=", ":=", "#", "<", "<=",
                           ">", ">=","+", "-", "*","/"};
    set<string> SSymbol = {"(", ")", ",", ";","."};
    LEX Lex_machine(KWord,OSymbol,SSymbol);
    string fileName = "tmp.in";
    Lex_machine.lexical_analysis(fileName,ans,sym,name);
    SyntaxAnalysis SyntaxAnalysis_machine(Lex_machine.getParam());
    try {
        SyntaxAnalysis_machine.handleProc();
    }
    catch (handleError & e) {
        e.printError();
        exit(1);
    }
    SyntaxAnalysis_machine.printTree();
    SyntaxAnalysis_machine.printTable();
    SyntaxAnalysis_machine.printQuadruple();
//    if(Lex_machine.lexical_analysis(fileName,ans,sym,name)){
//        for(auto & i: ans)
//            cout<<i<<endl;
//        Lex_machine.printParam();
//    }

    return 0;
}
