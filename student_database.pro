QT += core widgets gui

TARGET = student_database
TEMPLATE = app

CONFIG += c++17

SOURCES += \
    main.cpp \
    gui/MainWindow.cpp \
    core/Database.cpp \
    controllers/DatabaseController.cpp \
    core/FileManager.cpp \
    core/Student.cpp \
    gui/AddStudentDialog.cpp

HEADERS += \
    gui/MainWindow.h \
    core/Database.h \
    controllers/DatabaseController.h \
    core/FileManager.h \
    core/Student.h \
    gui/AddStudentDialog.h