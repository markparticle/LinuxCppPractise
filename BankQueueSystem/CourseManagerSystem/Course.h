#ifndef COURSE_H
#define COURSE_H

#include <string>
using namespace std;


class Course{
    //友元类函数：读取输入创建新的课程
    friend void read(istream  &inputStream, Course &course);
    //操作符<<重载函数，当cout<<输出课程信息时使用
    friend ostream& operator <<(ostream& out, const Course& course);

public:
    //无参数构造函数
    Course();

    // 课程类构造函数：根据课程名称创建课程对象
    Course(const string& cName): name_(cName) {};
    
    // 课程类拷贝构造函数
    Course(const Course& course);

    //虚函数 因为项目课及评估课对应的子类都含有新的成员需要显示
    virtual void PrintInfo() const;

    inline string GetName() const {return name_;}

    inline void SetName(const string& newName){name_ = newName;}

    inline const int GetId() const {return id_;}
//子类也不能访问
protected:
    //课程类：储存和处理课程信息
    string name_;

    int id_;


    //设置静态变量来生成唯一的ID值
    static int currentId;

};

class ProjectCourse: public Course{
public:
    inline void SetTag(const string& newTag){tag_ = newTag;}

    inline string GetTag() const {return tag_;};

    virtual void PrintInfo() const override;

private:
    string tag_;
};


#endif