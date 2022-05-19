#include "LEX.h"

int main() {
//    freopen("ans.out","w",stdout);
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
    string fileName = "PL0_code.in";
//    string fileName;
    if(Lex_machine.lexical_analysis(fileName,ans,sym,name)){
        for(auto & i: ans)
            cout<<i<<endl;
    }

    return 0;
}
