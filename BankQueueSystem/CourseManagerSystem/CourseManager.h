#ifndef COURSEMANAGER_H
#define COURSEMANAGER_H


#include <vector>
#include "Course.h"
using namespace std;

class CourseManager {

public:
    CourseManager() = default;

    CourseManager(const vector<Course>& courseVec);

    inline int GetCourseNum(){return courseList_.size();}

    void AddCourse(const Course &course);
    
    void AddCourse(const string& courseName);

    void RemoveLast();

    void RemoveByName(const string &name);

    void RemoveById(int id);

    void PrintAllCourse();

    void PrintCourse(const string &name);
    
    void PrintCourse(int id);

    void PrintLongNameCourse();


private:
    vector<Course> courseList_;

    int FindCourse_(const string& name); 
    // 根据课程ID查找课程，返回课程列表中的索引值，无法找到返回-1
    int FindCourse_(int id); 
};

#endif //COURSEMANAGER_H