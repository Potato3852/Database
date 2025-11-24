#pragma once
#include "core/Database.h"
#include "core/FileManager.h"
#include <memory>

class DatabaseController {
private:
    std::unique_ptr<Database> database;
    std::string currentFile;
    
public:
    DatabaseController() : database(std::make_unique<Database>()) {}
    
    // Base
    bool createNewDatabase();
    bool openDatabase(const std::string& filename);
    bool saveDatabase();
    bool saveAsDatabase(const std::string& filename);
    void closeDatabase();
    
    // Work with data
    bool addStudent(const Student& student);
    bool removeStudent(int id);
    bool updateStudent(int id, const Student& updatedData);
    Student* findStudent(int id);
    std::vector<Student*> searchStudents(const std::string& field, const std::string& value);
    
    // Backup and export
    bool createBackup(const std::string& backupDir);
    bool restoreFromBackup(const std::string& backupFile);
    bool exportToCSV(const std::string& filename);
    bool importFromCSV(const std::string& filename);
    
    // Get for GUI
    const Database* getDatabase() const { return database.get(); }
    bool isDatabaseOpen() const { return database != nullptr; }
};