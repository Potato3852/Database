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

    void setId(int id);
    void setName(const QString& name);
    void setGroup(const QString& group);
    void setScore(double score);

private:
    QLineEdit* idEdit;
    QLineEdit* nameEdit;
    QLineEdit* groupEdit;
    QDoubleSpinBox* scoreEdit;
};