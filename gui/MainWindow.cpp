#include "MainWindow.h"
#include "AddStudentDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QStatusBar>
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), isDarkTheme(false) {
    setupUI();
    setupSortingUI();
    createMenus();
    refreshTable();
    statusBar()->showMessage("Готово");
}

void MainWindow::setupUI() {
    // Main widget
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Create table
    table = new QTableWidget(this);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"ID", "Имя", "Группа", "Балл", "Дата"});
    table->horizontalHeader()->setStretchLastSection(true);

    // Safety
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    // Buttons
    addButton = new QPushButton("Добавить студента", this);
    deleteButton = new QPushButton("Удалить студента", this);

    // Search
    searchFieldCombo = new QComboBox(this);
    searchFieldCombo->addItem("ID");
    searchFieldCombo->addItem("Имя");
    searchFieldCombo->addItem("Группа");
    
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("Введите текст для поиска...");
    
    searchButton = new QPushButton("Найти", this);
    QPushButton* clearButton = new QPushButton("Очистить", this);
    
    // Build...
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();
    
    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchLayout->addWidget(new QLabel("Поиск по:", this));
    searchLayout->addWidget(searchFieldCombo);
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(clearButton);
    searchLayout->addStretch();

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(table);
    
    // Connections...
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddStudent);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteStudent);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearchStudent);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::onClearSearch);
    connect(searchEdit, &QLineEdit::returnPressed, this, &MainWindow::onSearchStudent);
    connect(searchFieldCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, [this](int index) {
    QString field = searchFieldCombo->itemText(index);
    if (field == "ID") {
        searchEdit->setPlaceholderText("Введите число...");
        searchEdit->setValidator(new QIntValidator(1, 99999, this));
    } else {
        searchEdit->setPlaceholderText("Введите текст для поиска...");
        searchEdit->setValidator(nullptr);
    }
    });

    setupTableSignals();
}

void MainWindow::refreshTable() {
    onSortTable();
}

void MainWindow::onAddStudent() {
    AddStudentDialog dialog(this);
    
    if(dialog.exec() == QDialog::Accepted) {
        Student student(
            dialog.getId(),
            dialog.getName().toStdString(),
            dialog.getGroup().toStdString(),
            dialog.getScore()
        );
        
        if(controller.addStudent(student)) {
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
    
    if(ok) {
        if(controller.removeStudent(id)) {
            refreshTable();
            QMessageBox::information(this, "Успех", "Студент удален!");
        } else {
            QMessageBox::warning(this, "Ошибка", "Студент с таким ID не найден!");
        }
    }
}

void MainWindow::onSearchStudent() {
    QString searchText = searchEdit->text().trimmed();
    if(searchText.isEmpty()) {
        refreshTable();
        return;
    }
    
    QString field = searchFieldCombo->currentText();
    
    std::vector<Student*> results;
    
    if(field == "ID") {
        bool ok;
        int id = searchText.toInt(&ok);
        if (ok) {
            Student* student = controller.findStudent(id);
            if (student) {
                results.push_back(student);
            }
        }
    } else if(field == "Имя") {
        results = controller.searchStudents("name", searchText.toStdString());
    } else if(field == "Группа") {
        results = controller.searchStudents("group", searchText.toStdString());
    }

    table->setRowCount(0);
    for(auto* student : results) {
        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(QString::number(student->getId())));
        table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(student->getName())));
        table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(student->getGroup())));
        table->setItem(row, 3, new QTableWidgetItem(QString::number(student->getAvgScore())));
        
        auto time = std::chrono::system_clock::to_time_t(student->getAdmissionDate());
        table->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(std::ctime(&time))));
    }

    statusBar()->showMessage(QString("Найдено: %1").arg(results.size()));
}

void MainWindow::onClearSearch() {
    searchEdit->clear();
    refreshTable();
    statusBar()->showMessage("Поиск очищен");
}

void MainWindow::createStatisticMenu() {
    QMenu* statsMenu = menuBar()->addMenu("Статистика");
    QAction* showStatsAction = new QAction("Показать статистику", this);
    statsMenu->addAction(showStatsAction);
    connect(showStatsAction, &QAction::triggered, this, &MainWindow::showStatistics);
}

// Theme -------------------------
void MainWindow::createTheme() {
    QMenu* themeMenu = menuBar()->addMenu("Тема");
    QAction* toggleThemeAction = new QAction("Переключить тему", this);
    themeMenu->addAction(toggleThemeAction);
    connect(toggleThemeAction, &QAction::triggered, this, &MainWindow::toggleTheme);
}

void MainWindow::toggleTheme() {
    isDarkTheme = !isDarkTheme;
    
    if(isDarkTheme) {
        applyDarkTheme();
        setWindowTitle("Student Database 🌙");
        statusBar()->showMessage("Тёмная тема включена");
    } else {
        applyLightTheme();
        setWindowTitle("Student Database ☀️");
        statusBar()->showMessage("Светлая тема включена");
    }
}
//--------------------------------

void MainWindow::createMenus() {
    createFileMenu();
    createStatisticMenu();
    createTheme();
}

void MainWindow::createFileMenu() {
    fileMenu = menuBar()->addMenu("Файл");
    
    newAction = new QAction("Новая база", this);
    openAction = new QAction("Открыть...", this);
    saveAction = new QAction("Сохранить", this);
    saveAsAction = new QAction("Сохранить как...", this);
    exportAction = new QAction("Экспорт в CSV...", this);
    importAction = new QAction("Импорт из CSV...", this);
    backupAction = new QAction("Создать backup...", this);
    restoreAction = new QAction("Восстановить из backup...", this);
    exitAction = new QAction("Выход", this);
    
    // Hotkeys
    newAction->setShortcut(QKeySequence::New);
    openAction->setShortcut(QKeySequence::Open);
    saveAction->setShortcut(QKeySequence::Save);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exportAction);
    fileMenu->addAction(importAction);
    fileMenu->addSeparator();
    fileMenu->addAction(backupAction);
    fileMenu->addAction(restoreAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    
    // Adding connection
    connect(newAction, &QAction::triggered, this, &MainWindow::onFileNew);
    connect(openAction, &QAction::triggered, this, &MainWindow::onFileOpen);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onFileSave);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::onFileSaveAs);
    connect(exportAction, &QAction::triggered, this, &MainWindow::onFileExportCSV);
    connect(importAction, &QAction::triggered, this, &MainWindow::onFileImportCSV);
    connect(backupAction, &QAction::triggered, this, &MainWindow::onBackupCreate);
    connect(restoreAction, &QAction::triggered, this, &MainWindow::onBackupRestore);
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);
}

void MainWindow::onFileNew() {
    if(controller.isDatabaseOpen()) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Новая база", 
            "Создать новую базу? Текущие данные будут потеряны!",
            QMessageBox::Yes | QMessageBox::No);
        
        if (reply == QMessageBox::No) return;
    }
    
    controller.createNewDatabase();
    refreshTable();
    statusBar()->showMessage("Создана новая база данных");
}

void MainWindow::onFileOpen() {
    QString filename = QFileDialog::getOpenFileName(this, "Открыть базу данных", 
        "", "Database Files (*.dat);;All Files (*)");
    
    if(!filename.isEmpty()) {
        if(controller.openDatabase(filename.toStdString())) {
            refreshTable();
            statusBar()->showMessage("База данных загружена: " + filename);
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл!");
        }
    }
}

void MainWindow::onFileSave() {
    if(controller.saveDatabase()) {
        statusBar()->showMessage("База данных сохранена");
    } else {
        onFileSaveAs();
    }
}

void MainWindow::onFileSaveAs() {
    QString filename = QFileDialog::getSaveFileName(this, "Сохранить базу данных", 
        "", "Database Files (*.dat);;All Files (*)");
    
    if(!filename.isEmpty()) {
        if(controller.saveAsDatabase(filename.toStdString())) {
            statusBar()->showMessage("База данных сохранена как: " + filename);
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл!");
        }
    }
}

void MainWindow::onFileExportCSV() {
    QString filename = QFileDialog::getSaveFileName(this, "Экспорт в CSV", 
        "", "CSV Files (*.csv);;All Files (*)");
    
    if(!filename.isEmpty()) {
        if(controller.exportToCSV(filename.toStdString())) {
            statusBar()->showMessage("Данные экспортированы в: " + filename);
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось экспортировать данные!");
        }
    }
}

void MainWindow::onFileImportCSV() {
    QString filename = QFileDialog::getOpenFileName(this, "Импорт из CSV", 
        "", "CSV Files (*.csv);;All Files (*)");
    
    if(!filename.isEmpty()) {
        if(controller.importFromCSV(filename.toStdString())) {
            refreshTable();
            statusBar()->showMessage("Данные импортированы из: " + filename);
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось импортировать данные из CSV!");
        }
    }
}

void MainWindow::onFileExit() {
    close();
}

void MainWindow::setupTableSignals() {
    connect(table, &QTableWidget::cellDoubleClicked, this, &MainWindow::onEditStudent);
}

void MainWindow::onEditStudent(int row, int column) {
    if(row < 0 || row >= table->rowCount()) return;

    int id = table->item(row, 0)->text().toInt();

    Student* student = controller.findStudent(id);
    if(!student) return;

    AddStudentDialog dialog(this);
    dialog.setId(student->getId());
    dialog.setName(QString::fromStdString(student->getName()));
    dialog.setGroup(QString::fromStdString(student->getGroup()));
    dialog.setScore(student->getAvgScore());
    
    if(dialog.exec() == QDialog::Accepted) {
        Student updatedStudent(
            dialog.getId(),
            dialog.getName().toStdString(),
            dialog.getGroup().toStdString(),
            dialog.getScore()
        );

        if(controller.updateStudent(id, updatedStudent)) {
            refreshTable();
            statusBar()->showMessage("Данные студента обновлены!");
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось обновить данные студента!");
        }
    }
}

void MainWindow::onBackupCreate() {
    QString dir = QFileDialog::getExistingDirectory(this, "Выберите папку для backup", "");
    
    if(!dir.isEmpty()) {
        if(controller.createBackup(dir.toStdString())) {
            statusBar()->showMessage("Backup создан в: " + dir);
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось создать backup!");
        }
    }
}

void MainWindow::onBackupRestore() {
    QString filename = QFileDialog::getOpenFileName(this, "Выберите backup файл", 
        "", "Backup Files (*.dat);;All Files (*)");
    
    if(!filename.isEmpty()) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Восстановление", 
            "Восстановить данные из backup? Текущие данные будут потеряны!",
            QMessageBox::Yes | QMessageBox::No);
        
        if(reply == QMessageBox::Yes) {
            if(controller.restoreFromBackup(filename.toStdString())) {
                refreshTable();
                statusBar()->showMessage("Данные восстановлены из backup");
            } else {
                QMessageBox::warning(this, "Ошибка", "Не удалось восстановить данные из backup!");
            }
        }
    }
}

void MainWindow::showStatistics() {
    auto stats = controller.getStatistics();
    
    QString message = QString("СТАТИСТИКА БАЗЫ ДАННЫХ\n\n");
    message += QString("Всего студентов: %1\n").arg(stats.totalStudents);
    message += QString("Общий средний балл: %1\n\n").arg(stats.overallAvgScore, 0, 'f', 2);
    
    message += "По группам:\n";
    for(const auto& [group, avgScore] : stats.avgScoreByGroup) {
        message += QString("  %1: %2 (студентов: %3)\n")
            .arg(QString::fromStdString(group))
            .arg(avgScore, 0, 'f', 2)
            .arg(stats.countByGroup.at(group));
    }
    
    QMessageBox::information(this, "Статистика", message);
}

// DARK THEME...
void MainWindow::applyDarkTheme() {
    QPalette darkPalette;
    
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    
    qApp->setPalette(darkPalette);
}

void MainWindow::applyLightTheme() {
    qApp->setPalette(qApp->style()->standardPalette());
}

// Sort
void MainWindow::setupSortingUI() {
    QHBoxLayout* sortLayout = new QHBoxLayout();
    
    sortLayout->addWidget(new QLabel("Сортировка:", this));
    
    sortFieldCombo = new QComboBox(this);
    sortFieldCombo->addItem("ID");
    sortFieldCombo->addItem("Имя");
    sortFieldCombo->addItem("Группа");
    sortFieldCombo->addItem("Балл");
    sortFieldCombo->addItem("Дата");
    sortLayout->addWidget(sortFieldCombo);
    
    sortOrderCombo = new QComboBox(this);
    sortOrderCombo->addItem("По возрастанию");
    sortOrderCombo->addItem("По убыванию");
    sortLayout->addWidget(sortOrderCombo);
    
    sortButton = new QPushButton("Сортировать", this);
    sortLayout->addWidget(sortButton);
    sortLayout->addStretch();
    
    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(centralWidget()->layout());
    if(mainLayout) {
        mainLayout->insertLayout(2, sortLayout);
    }
    
    connect(sortButton, &QPushButton::clicked, this, &MainWindow::onSortTable);
}

void MainWindow::onSortTable() {
    QString fieldText = sortFieldCombo->currentText();
    Database::SortField field;
    
    if (fieldText == "ID") field = Database::SortField::ID;
    else if (fieldText == "Имя") field = Database::SortField::NAME;
    else if (fieldText == "Группа") field = Database::SortField::GROUP;
    else if (fieldText == "Балл") field = Database::SortField::SCORE;
    else field = Database::SortField::DATE;
    
    Database::SortOrder order = (sortOrderCombo->currentText() == "По возрастанию") ? Database::SortOrder::ASC : Database::SortOrder::DESC; auto sortedStudents = controller.getSortedStudents(field, order);
    
    table->setRowCount(0);
    
    int row = 0;
    for(const Student* student : sortedStudents) {
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(QString::number(student->getId())));
        table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(student->getName())));
        table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(student->getGroup())));
        table->setItem(row, 3, new QTableWidgetItem(QString::number(student->getAvgScore())));
        
        auto time = std::chrono::system_clock::to_time_t(student->getAdmissionDate());
        table->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(std::ctime(&time))));
        
        row++;
    }
    
    statusBar()->showMessage(QString("Отсортировано по: %1 (%2)").arg(fieldText).arg(sortOrderCombo->currentText()));
}