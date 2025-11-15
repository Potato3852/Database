#include "DatabaseController.h"

bool DatabaseController::createNewDatabase() {
    database = std::make_unique<Database>();
    currentFile.clear();
    return true;
}

bool DatabaseController::openDatabase(const std::string& filename) {
    auto newDb = std::make_unique<Database>();

    if(FileManager::loadFromFile(*newDb, filename)) {
        database = std::move(newDb);
        currentFile = filename;
        return true;
    }

    return false;
}

bool DatabaseController::saveDatabase() {
    if(currentFile.empty())
        return false;
    return FileManager::saveToFile(*database, currentFile);
}

bool DatabaseController::saveAsDatabase(const std::string& filename) {
    currentFile = filename;
    return FileManager::saveToFile(*database, currentFile);
}

void DatabaseController::closeDatabase() {
    database.reset();
    currentFile.clear();
}

bool DatabaseController::addStudent(const Student& student) {
    return database->addStudent(student);
}

bool DatabaseController::removeStudent(int id) {
    return database->removeStudent(id);
}

Student* DatabaseController::findStudent(int id) {
    return database->findStudent(id);
}

std::vector<Student*> DatabaseController::searchStudents(const std::string& field, const std::string& value) {
    if(field == "group") {
        return database->findStudentsByGroup(value);
    } else if(field == "name") {
        return database->findStudentsByName(value);
    }
    return {};
}

bool DatabaseController::createBackup(const std::string& backupDir) {
    return FileManager::createBackup(*database, backupDir);
}

bool DatabaseController::exportToCSV(const std::string& filename) {
    return FileManager::exportToCSV(*database, filename);
}