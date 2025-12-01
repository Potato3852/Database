#pragma once
#include "Student.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <string>
#include <iostream>
#include <algorithm>

struct Statistics {
    int totalStudents;
    double overallAvgScore;
    std::map<std::string, double> avgScoreByGroup;
    std::map<std::string, int> countByGroup;
};

class Database {
private:
    std::unordered_map<int, Student> students;
    
    std::unordered_multimap<std::string, int> nameIndex;
    std::unordered_multimap<std::string, int> groupIndex;
    std::unordered_multimap<std::string, int> namePrefixIndex;
    std::unordered_multimap<std::string, int> groupPrefixIndex;

    void updateIndicesAdd(const Student& student);
    void updateIndicesRemove(const Student& student);
    void updateIndicesUpdate(const Student& oldStudent, const Student& newStudent);
    
    void updateStatisticsAdd(const Student& student);
    void updateStatisticsRemove(const Student& student); 

    std::unordered_set<int> deletedIds;

    double totalScore;
    std::map<std::string, double> groupTotalScores;
    std::map<std::string, int> groupCounts;

    std::string toUpperCase(const std::string& str) const;

public:
    Database() : totalScore(0.0) {}
    
    // CRUD - O(1)
    bool addStudent(const Student& student);
    bool removeStudent(int id);
    Student* findStudent(int id);
    bool updateStudent(int id, const Student& updatedData);
    
    // SEARCH - O(1)
    std::vector<Student*> findStudentsByGroup(const std::string& group);
    std::vector<Student*> findStudentsByName(const std::string& name);
    std::vector<Student*> findStudentsByNamePrefix(const std::string& prefix);
    
    // STATISTIC
    size_t getStudentCount() const { return students.size(); }
    const std::unordered_map<int, Student>& getAllStudents() const { return students; }
    
    // VALID
    bool isIdUnique(int id) const;
    
    // STATISTICS - O(1) 
    Statistics getStatistics() const;
    
    void compact();

    enum class SortField { ID, NAME, GROUP, SCORE, DATE };
    enum class SortOrder { ASC, DESC };
    
    std::vector<const Student*> getSortedStudents(SortField field = SortField::ID, SortOrder order = SortOrder::ASC) const;
};