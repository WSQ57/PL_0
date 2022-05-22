#include <utility>

//
// Created by WANG on 21/5/2022.
//

#ifndef PL_0_HANDLE_ERROR_H
#define PL_0_HANDLE_ERROR_H
using namespace std;

class handleError :public exception{
private:
    string ErrorType;
public:
    explicit handleError(string ErrorType1){
        ErrorType = std::move(ErrorType1);
    }
    void printError(){
        cout<<ErrorType<<endl;
    }
};


#endif //PL_0_HANDLE_ERROR_H
