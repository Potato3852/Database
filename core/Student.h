#pragma once
#include <string>
#include <chrono>
#include <iostream>

class Student {
private:
    int id;
    std::string name;
    std::string group;
    double avgScore;
    std::chrono::system_clock::time_point admissionDate;

public:
    // CONSTRUCTOR
    Student() = default;
    Student(int id, std::string name, std::string group, double score);
    
    // GET
    int getId() const { return id; }
    const std::string& getName() const { return name; }
    const std::string& getGroup() const { return group; }
    double getAvgScore() const { return avgScore; }
    auto getAdmissionDate() const { return admissionDate; }
    
    // SET
    void setName(const std::string& name);
    void setGroup(const std::string& group);
    void setAvgScore(double score);
    
    // VALID
    bool isValid() const;
    
    // OUTPUT
    friend std::ostream& operator<<(std::ostream& os, const Student& student);
};