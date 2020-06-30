#ifndef CMDMANAGER_H
#define CMDMANAGER_H

#include <iostream>
#include <map>

#include "CourseManager.h"
#include "Cmd.h"
using namespace std;


class CmdManager{
public:
    CmdManager() = default;

    void Init();

    //打印帮助中心
    void PrintAllHelp() const;

    //根据命令，返回帮助信息
    void PrintCmdHelp(const CourseCmd cmd) const;

    //处理操作
    bool HandleCmd(const CourseCmd cmd);

    //返回课程对象
    CourseManager& GetCourseManager(){ return manager_; }


private:
    CourseManager manager_;

    map<CourseCmd, string> cmdMap_;
};

#endif //CMDMANAGER_H