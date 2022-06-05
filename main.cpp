#include "LEX.h"
#include "SyntaxAnalysis.h"
#include "interpreter.h"
using namespace std;
int main() {
//    freopen("tmp.out","w",stdout);

    vector<string> sym;
    vector<string> name;

    set<string> KWord = {"CONST", "VAR", "PROCEDURE", "BEGIN",
                            "END", "ODD",  "IF", "THEN",
                            "CALL", "WHILE", "DO", "READ", "WRITE"};
    set<string> OSymbol = {"=", ":=", "#", "<", "<=",
                           ">", ">=","+", "-", "*","/"};
    set<string> SSymbol = {"(", ")", ",", ";","."};
    LEX Lex_machine(KWord,OSymbol,SSymbol);
    string fileName = "tmp.in";
//    string fileName;
    Lex_machine.lexical_analysis(fileName,sym,name);
    Lex_machine.printAns();

    SyntaxAnalysis SyntaxAnalysis_machine;
    SyntaxAnalysis_machine.param = Lex_machine.getParam();
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

    SyntaxAnalysis_machine.printInstruction();

    interpreter interpreter_machine(SyntaxAnalysis_machine.getTargetIns());
    interpreter_machine.execute();


    return 0;
}
