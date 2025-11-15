#include "AddStudentDialog.h"

AddStudentDialog::AddStudentDialog(QWidget* parent) 
    : QDialog(parent) {
    setWindowTitle("Добавить студента");
    setModal(true);
    resize(300, 200);
    
    // istream
    idEdit = new QLineEdit(this);
    nameEdit = new QLineEdit(this);
    groupEdit = new QLineEdit(this);
    scoreEdit = new QDoubleSpinBox(this);
    scoreEdit->setRange(0.0, 5.0);
    scoreEdit->setSingleStep(0.1);
    
    // Buttons
    QPushButton* okButton = new QPushButton("OK", this);
    QPushButton* cancelButton = new QPushButton("Отмена", this);
    
    // Build...
    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("ID:", idEdit);
    formLayout->addRow("Имя:", nameEdit);
    formLayout->addRow("Группа:", groupEdit);
    formLayout->addRow("Балл:", scoreEdit);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);
    
    // Connection...
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

int AddStudentDialog::getId() const {
    return idEdit->text().toInt();
}

QString AddStudentDialog::getName() const {
    return nameEdit->text();
}

QString AddStudentDialog::getGroup() const {
    return groupEdit->text();
}

double AddStudentDialog::getScore() const {
    return scoreEdit->value();
}