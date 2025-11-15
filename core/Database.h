#pragma once
#include "Student.h"
#include <vector>
#include <unordered_map>
#include <memory>

class Database {
private:
    std::vector<Student> students;
    std::unordered_map<int, size_t> idIndex;
    
    void rebuildIndex();

public:
    // CRUD
    bool addStudent(const Student& student);
    bool removeStudent(int id);
    Student* findStudent(int id);
    bool updateStudent(int id, const Student& updatedData);
    
    // SEARCH
    std::vector<Student*> findStudentsByGroup(const std::string& group);
    std::vector<Student*> findStudentsByName(const std::string& name);
    
    // STATISTIC
    size_t getStudentCount() const { return students.size(); }
    const std::vector<Student>& getAllStudents() const { return students; }
    
    // VALID
    bool isIdUnique(int id) const;
};