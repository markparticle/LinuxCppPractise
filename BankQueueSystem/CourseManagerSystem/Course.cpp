#include <iostream>
#include "Course.h"

using namespace std;
int Course::currentId = 1;

// 课程类构造函数
Course::Course(){
    // 将currentId当前值赋值给id，再将currentID自增
    id_ = currentId++;
    
    // 默认课程名称为空字符串
    name_ = "";
}

Course::Course(const Course& course){
    id_ = course.GetId();
    name_ = course.GetName();
}

void Course::PrintInfo() const{
    cout << "Course:" << id_ << " : " << name_ << endl;
}

//友元类，可访问private protected成员
void read(istream &is, Course &item){
    is >> item.name_;
}

ostream& operator<<(ostream& os, const Course& course){
    os << "Course: " << course.id_ << " : " << course.name_;
    return os;
}

void ProjectCourse::PrintInfo() const {
    cout << "ProjectCourse: " << id_ << " : " << name_ << " : " << tag_ << endl;
}
