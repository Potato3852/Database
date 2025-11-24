#pragma once
#include "Student.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <string>

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

    std::unordered_set<int> deletedIds;
    std::vector<int> freeIds;

    double totalScore;
    std::map<std::string, double> groupTotalScores;
    std::map<std::string, int> groupCounts;
    
    void updateIndicesAdd(const Student& student);
    void updateIndicesRemove(const Student& student);
    void updateStatisticsAdd(const Student& student);
    void updateStatisticsRemove(const Student& student);

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
    
    // STATISTIC
    size_t getStudentCount() const { return students.size(); }
    const std::unordered_map<int, Student>& getAllStudents() const { return students; }
    
    // VALID
    bool isIdUnique(int id) const;
    
    // STATISTICS - O(1)
    Statistics getStatistics() const;
    
    // OPTIMIZATION
    void compact();
};