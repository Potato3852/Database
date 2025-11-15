#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QFormLayout>

class AddStudentDialog : public QDialog {
    Q_OBJECT

public:
    AddStudentDialog(QWidget* parent = nullptr);
    
    int getId() const;
    QString getName() const;
    QString getGroup() const;
    double getScore() const;

private:
    QLineEdit* idEdit;
    QLineEdit* nameEdit;
    QLineEdit* groupEdit;
    QDoubleSpinBox* scoreEdit;
};