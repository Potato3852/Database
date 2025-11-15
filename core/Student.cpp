#include "Student.h"

Student::Student(int id, std::string name, std::string group, double score)
    : id(id), name(std::move(name)), group(std::move(group)), 
      avgScore(score), admissionDate(std::chrono::system_clock::now()) {}

void Student::setName(const std::string& newName) { 
    if (!newName.empty()) name = newName; 
}

void Student::setGroup(const std::string& newGroup) { 
    if (!newGroup.empty()) group = newGroup; 
}

void Student::setAvgScore(double score) { 
    if (score >= 0.0 && score <= 5.0) avgScore = score; 
}

bool Student::isValid() const {
    return id > 0 && !name.empty() && !group.empty() && avgScore >= 0.0 && avgScore <= 5.0;
}

std::ostream& operator<<(std::ostream& os, const Student& student) {
    auto time = std::chrono::system_clock::to_time_t(student.admissionDate);
    os << "ID: " << student.id 
       << " | Name: " << student.name
       << " | Group: " << student.group
       << " | Score: " << student.avgScore
       << " | Date: " << std::ctime(&time);
    return os;
}