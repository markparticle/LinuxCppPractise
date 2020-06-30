#include <iostream>
#include "CmdManager.h"
using namespace std;

int main(){
    CmdManager cmdManager;
    cmdManager.Init();
    int cmd;
    cmdManager.PrintAllHelp();
    cout << "New Command: ";

    while(cin >> cmd){
        if(cin.good()){
            bool exitCode = cmdManager.HandleCmd(static_cast<CourseCmd>(cmd));
            if(!exitCode) return 0;
        }
        cout << "-------------------------" << endl;
        cout << "New Command:";

        // 清理输入流，避免刚才流中的字符影响后续输入
        cin.clear();
        cin.ignore();
    }
    return 0;
}