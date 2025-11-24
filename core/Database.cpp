#include "Database.h"
#include <algorithm>

void Database::updateIndicesAdd(const Student& student) {
    int id = student.getId();
    nameIndex.emplace(student.getName(), id);
    groupIndex.emplace(student.getGroup(), id);
}

void Database::updateIndicesRemove(const Student& student) {
    int id = student.getId();
    
    auto nameRange = nameIndex.equal_range(student.getName());
    for(auto it = nameRange.first; it != nameRange.second; ) {
        if(it->second == id) {
            it = nameIndex.erase(it);
            break;
        } else {
            ++it;
        }
    }

    auto groupRange = groupIndex.equal_range(student.getGroup());
    for(auto it = groupRange.first; it != groupRange.second; ) {
        if(it->second == id) {
            it = groupIndex.erase(it);
            break;
        } else {
            ++it;
        }
    }
}

void Database::updateStatisticsAdd(const Student& student) {
    totalScore += student.getAvgScore();
    
    std::string group = student.getGroup();
    groupTotalScores[group] += student.getAvgScore();
    groupCounts[group]++;
}

void Database::updateStatisticsRemove(const Student& student) {
    totalScore -= student.getAvgScore();
    
    std::string group = student.getGroup();
    groupTotalScores[group] -= student.getAvgScore();
    groupCounts[group]--;
    
    if(groupCounts[group] == 0) {
        groupTotalScores.erase(group);
        groupCounts.erase(group);
    }
}

bool Database::addStudent(const Student& student) {
    if(!student.isValid()) return false;
    
    int id = student.getId();

    if(students.count(id) || deletedIds.count(id)) {
        return false;
    }
    
    students[id] = student;
    
    updateIndicesAdd(student);
    updateStatisticsAdd(student);
    
    return true;
}

bool Database::removeStudent(int id) {
    auto it = students.find(id);
    if(it == students.end()) return false;
    
    const Student& student = it->second;
    
    updateIndicesRemove(student);
    updateStatisticsRemove(student);
    
    students.erase(it);
    deletedIds.insert(id);
    freeIds.push_back(id);
    
    return true;
}

Student* Database::findStudent(int id) {
    auto it = students.find(id);
    return it != students.end() ? &it->second : nullptr;
}

bool Database::updateStudent(int id, const Student& updatedData) {
    if(!updatedData.isValid()) return false;
    
    auto it = students.find(id);
    if(it == students.end()) return false;
    
    Student& oldStudent = it->second;
    
    if(id != updatedData.getId()) {
        if (!isIdUnique(updatedData.getId())) return false;
    }
    
    updateIndicesRemove(oldStudent);
    updateStatisticsRemove(oldStudent);

    oldStudent = updatedData;

    if(id != updatedData.getId()) {
        students.erase(it);
        students[updatedData.getId()] = oldStudent;
    }
    
    updateIndicesAdd(oldStudent);
    updateStatisticsAdd(oldStudent);
    
    return true;
}

std::vector<Student*> Database::findStudentsByGroup(const std::string& group) {
    std::vector<Student*> result;
    auto range = groupIndex.equal_range(group);
    
    for(auto it = range.first; it != range.second; ++it) {
        if(students.count(it->second)) {
            result.push_back(&students[it->second]);
        }
    }
    return result;
}

std::vector<Student*> Database::findStudentsByName(const std::string& name) {
    std::vector<Student*> result;
    auto range = nameIndex.equal_range(name);
    
    for(auto it = range.first; it != range.second; ++it) {
        if(students.count(it->second)) {
            result.push_back(&students[it->second]);
        }
    }
    return result;
}

bool Database::isIdUnique(int id) const {
    return !students.count(id) && !deletedIds.count(id);
}

Statistics Database::getStatistics() const {
    Statistics stats;
    stats.totalStudents = students.size();
    stats.overallAvgScore = students.empty() ? 0.0 : totalScore / students.size();
    
    for(const auto& [group, total] : groupTotalScores) {
        stats.avgScoreByGroup[group] = total / groupCounts.at(group);
        stats.countByGroup[group] = groupCounts.at(group);
    }
    
    return stats;
}

void Database::compact() {
    deletedIds.clear();
    freeIds.clear();
}