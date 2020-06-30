#include <iostream>
#include <stdexcept>
#include "CmdManager.h"

using namespace std;

void CmdManager::Init(){

    // 初始化课程列表
    manager_.AddCourse("Linux");
    manager_.AddCourse("NodeJS");
    manager_.AddCourse("C++");
    manager_.AddCourse("Python");
    manager_.AddCourse("Spark");
    manager_.AddCourse("Hadoop");
    manager_.AddCourse("Ruby");
    manager_.AddCourse("Java");

    // 初始化命令列表
    cmdMap_.insert(pair<CourseCmd, string>(CMD_PRINT_HELP, "Help Info"));
    cmdMap_.insert(pair<CourseCmd, string>(CMD_PRINT_COURSE, "Course List"));
    cmdMap_.insert(pair<CourseCmd, string>(CMD_PRINT_COURSE_NUM, "Course Number"));
    cmdMap_.insert(pair<CourseCmd, string>(CMD_PRINT_LONG_NAME, "Longest Course Name"));
    cmdMap_.insert(pair<CourseCmd, string>(CMD_REMOVE_LAST_COURSE, "Remove Last Course"));
    cmdMap_.insert(pair<CourseCmd, string>(CMD_EXIT, "Exit"));
}

bool CmdManager::HandleCmd(const CourseCmd cmd){
    auto iter = cmdMap_.find(cmd);
    if(iter == cmdMap_.end()){
        cout << "Not Found!" << endl;
        return true;
    }
    switch(cmd){
        case CMD_PRINT_HELP: PrintAllHelp(); break;
        case CMD_PRINT_COURSE: manager_.PrintAllCourse(); break;
        case CMD_PRINT_COURSE_NUM: cout << manager_.GetCourseNum() << endl; break;
        case CMD_PRINT_LONG_NAME: manager_.PrintLongNameCourse(); break;
        case CMD_REMOVE_LAST_COURSE: manager_.RemoveLast(); break;
        case CMD_EXIT: return false;
        default: break;
    }
    return true;
}


void CmdManager::PrintAllHelp() const{
    cout << "Cmd List:" << endl;
    for(auto item: cmdMap_){
        cout << item.first << " : " << item.second << endl;
    }
}

void CmdManager::PrintCmdHelp(const CourseCmd cmd) const{
    auto iter = cmdMap_.find(cmd);

    if(iter != cmdMap_.end()){
        cout << iter->first << " : " << iter->second << endl;
    }
    else cout << "Not Found!" << endl;
}