#include "FileManager.h"
#include <fstream>
#include <filesystem>
#include <sstream>

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
        
        std::chrono::system_clock::from_time_t(time);
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

bool FileManager::importFromCSV(Database& db, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    bool firstLine = true;
    int importedCount = 0;
    
    while(std::getline(file, line)) {
        if(firstLine) {
            firstLine = false;
            continue;
        }
        
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;

        while(std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        if(tokens.size() >= 4) {
            try {
               int id = std::stoi(tokens[0]);
                std::string name = tokens[1];
                std::string group = tokens[2];
                double score = std::stod(tokens[3]);

                Student student(id, name, group, score);
                
                if (db.addStudent(student)) {
                    importedCount++;
                }
            } catch (const std::exception& e) {
                // skiping...
                continue;
            }
        }
    }
    
    return importedCount > 0;
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