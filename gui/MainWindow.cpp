#include "MainWindow.h"
#include "AddStudentDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget* parent) 
    : QMainWindow(parent) {
    setupUI();
    refreshTable();
}

void MainWindow::setupUI() {
    // Main widget
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Create table
    table = new QTableWidget(this);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"ID", "Имя", "Группа", "Балл", "Дата"});
    
    // Buttons
    addButton = new QPushButton("Добавить студента", this);
    deleteButton = new QPushButton("Удалить студента", this);
    
    // Build...
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(table);
    
    // Connections...
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddStudent);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteStudent);
}

void MainWindow::refreshTable() {
    table->setRowCount(0);
    
    if (!controller.isDatabaseOpen()) {
        controller.createNewDatabase();
    }
    
    const Database* db = controller.getDatabase();
    if (!db) return;
    
    int row = 0;
    for (const auto& student : db->getAllStudents()) {
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(QString::number(student.getId())));
        table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(student.getName())));
        table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(student.getGroup())));
        table->setItem(row, 3, new QTableWidgetItem(QString::number(student.getAvgScore())));
        
        auto time = std::chrono::system_clock::to_time_t(student.getAdmissionDate());
        table->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(std::ctime(&time))));
        
        row++;
    }
}

void MainWindow::onAddStudent() {
    AddStudentDialog dialog(this);
    
    if (dialog.exec() == QDialog::Accepted) {
        Student student(
            dialog.getId(),
            dialog.getName().toStdString(),
            dialog.getGroup().toStdString(),
            dialog.getScore()
        );
        
        if (controller.addStudent(student)) {
            refreshTable();
            QMessageBox::information(this, "Успех", "Студент добавлен!");
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось добавить студента!\nПроверьте ID и данные.");
        }
    }
}

void MainWindow::onDeleteStudent() {
    bool ok;
    int id = QInputDialog::getInt(this, "Удалить студента", "Введите ID студента:", 0, 1, 100000, 1, &ok);
    
    if (ok) {
        if (controller.removeStudent(id)) {
            refreshTable();
            QMessageBox::information(this, "Успех", "Студент удален!");
        } else {
            QMessageBox::warning(this, "Ошибка", "Студент с таким ID не найден!");
        }
    }
}