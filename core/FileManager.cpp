#include "FileManager.h"
#include <fstream>
#include <filesystem>

bool FileManager::saveToFile(const Database& db, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    size_t count = db.getStudentCount();
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for(const auto& student : db.getAllStudents()) {
        int id = student.getId();
        file.write(reinterpret_cast<const char*>(&id), sizeof(id));
        
        std::string name = student.getName();
        std::string group = student.getGroup();
        double score = student.getAvgScore();
        auto date = student.getAdmissionDate();
        time_t time = std::chrono::system_clock::to_time_t(date);
        
        file.write(name.c_str(), 50);
        file.write(group.c_str(), 10);
        file.write(reinterpret_cast<const char*>(&score), sizeof(score));
        file.write(reinterpret_cast<const char*>(&time), sizeof(time));
    }
    
    return file.good();
}

bool FileManager::loadFromFile(Database& db, const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    size_t count;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));

    for (size_t i = 0; i < count; ++i) {
        int id;
        char name[50] = {0};
        char group[10] = {0};
        double score;
        time_t time;
        
        file.read(reinterpret_cast<char*>(&id), sizeof(id));
        file.read(name, 50);
        file.read(group, 10);
        file.read(reinterpret_cast<char*>(&score), sizeof(score));
        file.read(reinterpret_cast<char*>(&time), sizeof(time));
        
        auto date = std::chrono::system_clock::from_time_t(time);
        Student student(id, std::string(name), std::string(group), score);
        
        db.addStudent(student);
    }

    return file.good();
}

bool FileManager::exportToCSV(const Database& db, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    file << "ID,Name,Group,Score,AdmissionDate\n";

    for(const auto& student : db.getAllStudents()) {
        auto time = std::chrono::system_clock::to_time_t(student.getAdmissionDate());
        file << student.getId() << ","
            << student.getName() << ","
            << student.getGroup() << ","
            << student.getAvgScore() << ","
            << time << "\n";
    }

    return file.good();
}

bool FileManager::createBackup(const Database& db, const std::string& backupDir) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    
    std::string backupFile = backupDir + "/backup_" + std::to_string(time) + ".dat";
    return saveToFile(db, backupFile);
}

bool FileManager::restoreFromBackup(Database& db, const std::string& backupFile) {
    return loadFromFile(db, backupFile);
}