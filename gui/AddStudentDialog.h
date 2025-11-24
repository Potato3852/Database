#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox> 

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

private slots:
    void validateInput();

private:
    QLineEdit* idEdit;
    QLineEdit* nameEdit;
    QLineEdit* groupEdit;
    QDoubleSpinBox* scoreEdit;
    QPushButton* okButton;
    
    bool validateId();
    bool validateName(); 
    bool validateGroup();
    bool validateScore();
};