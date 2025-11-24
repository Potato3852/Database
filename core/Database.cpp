#include "Database.h"
#include <algorithm>

void Database::rebuildIndex() {
    idIndex.clear();
    for(size_t i = 0; i < students.size(); ++i) {
        idIndex[students[i].getId()] = i;
    }
}

bool Database::addStudent(const Student& student) {
    if(!student.isValid() || !isIdUnique(student.getId())) {
        return false;
    }

    students.push_back(student);
    idIndex[student.getId()] = students.size() - 1;
    return true;
}

bool Database::removeStudent(int id) {
    auto it = idIndex.find(id);
    if(it == idIndex.end()) {
        return false;
    }

    size_t pos = it->second;
    students.erase(students.begin() + pos);
    rebuildIndex();
    return true;
}

Student* Database::findStudent(int id) {
    auto it = idIndex.find(id);
    if(it != idIndex.end()) {
        return &students[it->second];
    }
    return nullptr;
}

bool Database::updateStudent(int id, const Student& updatedData) {
    Student* student = findStudent(id);
    if(!student || !updatedData.isValid()) {
        return false;
    }

    if (id != updatedData.getId() && !isIdUnique(updatedData.getId())) {
        return false;
    }

    *student = updatedData;

    if (id != updatedData.getId()) {
        idIndex.erase(id);
        idIndex[updatedData.getId()] = idIndex[id];
    }

    return true;
}

std::vector<Student*> Database::findStudentsByGroup(const std::string& group) {
    std::vector<Student*> result;
    for(auto& student : students) {
        if(student.getGroup() == group) {
            result.push_back(&student);
        }
    }
    return result;
}

std::vector<Student*> Database::findStudentsByName(const std::string& name) {
    std::vector<Student*> result;
    for(auto& student : students) {
        if(student.getName().find(name) != std::string::npos) {
            result.push_back(&student);
        }
    }
    return result;
}

bool Database::isIdUnique(int id) const {
    return idIndex.find(id) == idIndex.end();
}