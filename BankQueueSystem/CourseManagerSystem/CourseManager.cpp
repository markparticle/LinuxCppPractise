#include <iostream>
#include "CourseManager.h"

using namespace std;

CourseManager::CourseManager(const vector<Course>& course){
    for(auto item: course){
        //*
        courseList_.push_back(item);
    }
}

void CourseManager::AddCourse(const string& courseName){
    Course course(courseName);
    courseList_.push_back(course);
}

void CourseManager::AddCourse(const Course &course){
    courseList_.push_back(course);
}

void CourseManager::RemoveLast(){
    try{
        if(!courseList_.empty()){
            courseList_.pop_back();
            cout << "Deleted successfully!" << endl;
        } else{
            throw runtime_error("Deleted error, there is no course");
        }
    }
    catch(runtime_error err){
        cout << err.what() << endl;
    }
}

void CourseManager::RemoveById(int id){
    int index = FindCourse_(id);
    if(index > 0){
        courseList_.erase(courseList_.begin() + index);
    }
    else {
        cout << "Not Found!" << endl;
    }
}

void CourseManager::PrintAllCourse(){
    cout << "CourseList:" << endl;
    for(auto item: courseList_){
        //调用
        cout << item << endl;
    }
}

void CourseManager::PrintCourse(int id){
    int index = FindCourse_(id);
    if(index > 0){
        if(index > 0){
            cout << courseList_[index] << endl;
        } else{
            cout << "Not Found!" << endl;
        }
    }
}

void CourseManager::PrintCourse(const string& name){
    int index = FindCourse_(name);
    if(index > 0){
        cout << courseList_[index] << endl;
    }
    else {
        cout << "Not Found!" << endl;
    }
}

void CourseManager::PrintLongNameCourse(){
    int maxLen = 0;
    Course curs;
    for(auto item: courseList_){
        int currLen = item.GetName().size();
        if(currLen > maxLen){
            maxLen = currLen;
            curs = item;
        }
    }
    cout << curs << endl;
}

int CourseManager::FindCourse_(int id){
    for(int i = 0; i < courseList_.size(); i++){
        if(courseList_[i].GetId() == id){
            return i;
        }
    }
    return -1;
}

int CourseManager::FindCourse_(const string& name){
    for(int i = 0; i < courseList_.size(); i++){
        if(courseList_[i].GetName() == name){
            return i;
        }
    }
    return -1;
}