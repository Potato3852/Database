#include "Database.h"
#include <algorithm>
#include <cctype>
#include <iostream>

std::string Database::toUpperCase(const std::string& str) const {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return result;
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

void Database::updateIndicesAdd(const Student& student) {
    int id = student.getId();
 
    std::string nameUpper = toUpperCase(student.getName());
    std::string groupUpper = toUpperCase(student.getGroup());

    nameIndex.emplace(nameUpper, id);
    groupIndex.emplace(groupUpper, id);
    
    if(nameUpper.length() >= 2) {
        namePrefixIndex.emplace(nameUpper.substr(0, 2), id);
    } else if(!nameUpper.empty()) {
        namePrefixIndex.emplace(nameUpper, id);
    }
    
    if(!groupUpper.empty()) {
        groupPrefixIndex.emplace(groupUpper.substr(0, 1), id);
    }
}

void Database::updateIndicesRemove(const Student& student) {
    int id = student.getId();
    
    std::string nameUpper = toUpperCase(student.getName());
    auto nameRange = nameIndex.equal_range(nameUpper);
    for(auto it = nameRange.first; it != nameRange.second; ) {
        if(it->second == id) {
            nameIndex.erase(it);
            break;
        }
        ++it;
    }

    std::string groupUpper = toUpperCase(student.getGroup());
    auto groupRange = groupIndex.equal_range(groupUpper);
    for(auto it = groupRange.first; it != groupRange.second; ) {
        if(it->second == id) {
            groupIndex.erase(it);
            break;
        }
        ++it;
    }
    
    if(nameUpper.length() >= 2) {
        std::string prefix = nameUpper.substr(0, 2);
        auto prefixRange = namePrefixIndex.equal_range(prefix);
        for(auto it = prefixRange.first; it != prefixRange.second; ) {
            if(it->second == id) {
                it = namePrefixIndex.erase(it);
                break;
            }
            ++it;
        }
    }

    if(!groupUpper.empty()) {
        std::string groupPrefix = groupUpper.substr(0, 1);
        auto groupPrefRange = groupPrefixIndex.equal_range(groupPrefix);
        for(auto it = groupPrefRange.first; it != groupPrefRange.second; ) {
            if(it->second == id) {
                it = groupPrefixIndex.erase(it);
                break;
            }
            ++it;
        }
    }
}

void Database::updateIndicesUpdate(const Student& oldStudent, const Student& newStudent) {
    std::string oldNameUpper = toUpperCase(oldStudent.getName());
    std::string newNameUpper = toUpperCase(newStudent.getName());
    std::string oldGroupUpper = toUpperCase(oldStudent.getGroup());
    std::string newGroupUpper = toUpperCase(newStudent.getGroup());

    if(oldNameUpper != newNameUpper || oldGroupUpper != newGroupUpper) {
        updateIndicesRemove(oldStudent);
        updateIndicesAdd(newStudent);
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
    
    return true;
}

Student* Database::findStudent(int id) {
    auto it = students.find(id);
    if(it != students.end()) {
        return &it->second;
    }
    return nullptr;
}

bool Database::updateStudent(int id, const Student& updatedData) {
    if(!updatedData.isValid()) return false;
    
    auto it = students.find(id);
    if(it == students.end()) return false;
    
    Student& oldStudent = it->second;
    
    if(id != updatedData.getId()) {
        if(!isIdUnique(updatedData.getId())) return false;
    }
    
    updateStatisticsRemove(oldStudent);
    
    updateIndicesUpdate(oldStudent, updatedData);
    
    oldStudent = updatedData;
    
    if(id != updatedData.getId()) {
        students.erase(it);
        students[updatedData.getId()] = oldStudent;
    }
    
    updateStatisticsAdd(oldStudent);
    
    return true;
}

bool Database::isIdUnique(int id) const {
    return !students.count(id) && !deletedIds.count(id);
}

std::vector<Student*> Database::findStudentsByName(const std::string& name) {
    std::vector<Student*> result;
    
    if(name.empty()) return result;
    
    std::string searchUpper = toUpperCase(name);
    
    auto exactRange = nameIndex.equal_range(searchUpper);
    for(auto it = exactRange.first; it != exactRange.second; ++it) {
        auto studentIt = students.find(it->second);
        if(studentIt != students.end()) {
            result.push_back(&studentIt->second);
        }
    }
    
    if(result.empty()) {
        if(searchUpper.length() >= 2) {
            std::string prefix = searchUpper.substr(0, 2);
            auto prefixRange = namePrefixIndex.equal_range(prefix);
            
            std::unordered_set<int> uniqueIds;
            for(auto it = prefixRange.first; it != prefixRange.second; ++it) {
                if(uniqueIds.insert(it->second).second) {
                    auto studentIt = students.find(it->second);
                    if(studentIt != students.end()) {
                        std::string studentNameUpper = toUpperCase(studentIt->second.getName());
                        if(studentNameUpper.find(searchUpper) != std::string::npos) {
                            result.push_back(&studentIt->second);
                        }
                    }
                }
            }
        }

        if(result.empty()) {
            for(auto& pair : students) {
                Student* studentPtr = const_cast<Student*>(&pair.second);
                std::string studentNameUpper = toUpperCase(studentPtr->getName());
                if(studentNameUpper.find(searchUpper) != std::string::npos) {
                    result.push_back(studentPtr);
                }
            }
        }
    }
    
    return result;
}

std::vector<Student*> Database::findStudentsByGroup(const std::string& group) {
    std::vector<Student*> result;
    
    if(group.empty()) return result;
    
    std::string searchUpper = toUpperCase(group);
    
    auto exactRange = groupIndex.equal_range(searchUpper);
    for(auto it = exactRange.first; it != exactRange.second; ++it) {
        auto studentIt = students.find(it->second);
        if(studentIt != students.end()) {
            result.push_back(&studentIt->second);
        }
    }
    
    if(result.empty()) {
        if(!searchUpper.empty()) {
            std::string prefix = searchUpper.substr(0, 1);
            auto prefixRange = groupPrefixIndex.equal_range(prefix);
            
            std::unordered_set<int> uniqueIds;
            for(auto it = prefixRange.first; it != prefixRange.second; ++it) {
                if(uniqueIds.insert(it->second).second) {
                    auto studentIt = students.find(it->second);
                    if(studentIt != students.end()) {
                        std::string studentGroupUpper = toUpperCase(studentIt->second.getGroup());
                        if(studentGroupUpper.find(searchUpper) != std::string::npos) {
                            result.push_back(&studentIt->second);
                        }
                    }
                }
            }
        }
    }
    
    return result;
}

std::vector<Student*> Database::findStudentsByNamePrefix(const std::string& prefix) {
    std::vector<Student*> result;
    
    if(prefix.empty()) return result;
    
    std::string searchUpper = toUpperCase(prefix);
    std::string searchPrefix = searchUpper.substr(0, std::min(static_cast<size_t>(2), searchUpper.length()));
    
    auto range = namePrefixIndex.equal_range(searchPrefix);
    std::unordered_set<int> uniqueIds;
    
    for(auto it = range.first; it != range.second; ++it) {
        if(uniqueIds.insert(it->second).second) {
            auto studentIt = students.find(it->second);
            if(studentIt != students.end()) {
                result.push_back(&studentIt->second);
            }
        }
    }
    
    return result;
}

Statistics Database::getStatistics() const {
    Statistics stats;
    stats.totalStudents = static_cast<int>(students.size());
    stats.overallAvgScore = students.empty() ? 0.0 : totalScore / students.size();
    
    for(const auto& [group, total] : groupTotalScores) {
        stats.avgScoreByGroup[group] = total / groupCounts.at(group);
        stats.countByGroup[group] = groupCounts.at(group);
    }
    
    return stats;
}

void Database::compact() {
    deletedIds.clear();
}