#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include "DatabaseController.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void onAddStudent();
    void onDeleteStudent();
    void refreshTable();

private:
    void setupUI();
    
    DatabaseController controller;
    QTableWidget* table;
    QPushButton* addButton;
    QPushButton* deleteButton;
};