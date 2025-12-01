#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QMenuBar>
#include <QMenu> 
#include <QAction>
#include <QStatusBar>
#include "controllers/DatabaseController.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void onAddStudent();
    void onDeleteStudent();
    void refreshTable();
    void onSearchStudent();
    void onClearSearch();
    void onFileNew();
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
    void onFileExportCSV();
    void onFileImportCSV();
    void onFileExit();
    void onBackupCreate();
    void onBackupRestore();
    void showStatistics();
    void onEditStudent(int row, int column);
    void toggleTheme();
    void onSortTable();

private:
    void setupUI();
    void createMenus();
    void createFileMenu();
    void createStatisticMenu();
    void createTheme();
    void setupTableSignals();
    void setupSortingUI();

    void applyDarkTheme();
    void applyLightTheme();
    bool isDarkTheme;
    
    DatabaseController controller;
    QTableWidget* table;
    QPushButton* addButton;
    QPushButton* deleteButton;
    QLineEdit* searchEdit;
    QPushButton* searchButton;
    QComboBox* searchFieldCombo;
    QMenu* fileMenu;
    QAction* newAction;
    QAction* openAction;
    QAction* saveAction;
    QAction* saveAsAction;
    QAction* exportAction;
    QAction* importAction;
    QAction* backupAction;
    QAction* restoreAction;
    QAction* exitAction;
    QComboBox* sortFieldCombo;
    QComboBox* sortOrderCombo;
    QPushButton* sortButton;
};