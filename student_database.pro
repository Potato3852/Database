QT += core widgets gui

TARGET = student_database
TEMPLATE = app

CONFIG += c++17

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    Database.cpp \
    DatabaseController.cpp \
    FileManager.cpp \
    Student.cpp \
    AddStudentDialog.cpp

HEADERS += \
    MainWindow.h \
    Database.h \
    DatabaseController.h \
    FileManager.h \
    Student.h \
    AddStudentDialog.h