#pragma once
#include "Database.h"
#include <string>

class FileManager {
public:
    static bool saveToFile(const Database& db, const std::string& filename);
    static bool loadFromFile(Database& db, const std::string& filename);
    static bool createBackup(const Database& db, const std::string& backupDir);
    static bool restoreFromBackup(Database& db, const std::string& backupFile);
    static bool exportToCSV(const Database& db, const std::string& filename);
    static bool importFromCSV(Database& db, const std::string& filename);
};