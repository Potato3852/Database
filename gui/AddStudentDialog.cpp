#include "AddStudentDialog.h"

AddStudentDialog::AddStudentDialog(QWidget* parent) 
    : QDialog(parent) {
    setWindowTitle("Добавить студента");
    setModal(true);
    resize(300, 200);
    
    // istream
    idEdit = new QLineEdit(this);
    idEdit->setValidator(new QIntValidator(1, 99999, this));

    nameEdit = new QLineEdit(this);
    groupEdit = new QLineEdit(this);
    scoreEdit = new QDoubleSpinBox(this);
    scoreEdit->setRange(0.0, 5.0);
    scoreEdit->setSingleStep(0.1);
    scoreEdit->setDecimals(1);
    
    // Buttons
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);
    
    // Build...
    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("ID:", idEdit);
    formLayout->addRow("Имя:", nameEdit);
    formLayout->addRow("Группа:", groupEdit);
    formLayout->addRow("Балл:", scoreEdit);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);
    
    // Connection...
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(idEdit, &QLineEdit::textChanged, this, &AddStudentDialog::validateInput);
    connect(nameEdit, &QLineEdit::textChanged, this, &AddStudentDialog::validateInput);
    connect(groupEdit, &QLineEdit::textChanged, this, &AddStudentDialog::validateInput);
    connect(scoreEdit, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &AddStudentDialog::validateInput);
}

// getters...

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

// setters...

void AddStudentDialog::setId(int id) {
    idEdit->setText(QString::number(id));
}

void AddStudentDialog::setName(const QString& name) {
    nameEdit->setText(name);
}

void AddStudentDialog::setGroup(const QString& group) {
    groupEdit->setText(group);
}

void AddStudentDialog::setScore(double score) {
    scoreEdit->setValue(score);
}

// Validation...
void AddStudentDialog::validateInput() {
    bool valid = validateId() && validateName() && validateGroup() && validateScore();
    okButton->setEnabled(valid);

    QString styleValid = "border: 1px solid green;";
    QString styleInvalid = "border: 1px solid red;";

    idEdit->setStyleSheet(validateId() ? styleValid : styleInvalid);
    nameEdit->setStyleSheet(validateName() ? styleValid : styleInvalid);
    groupEdit->setStyleSheet(validateGroup() ? styleValid : styleInvalid);
    scoreEdit->setStyleSheet(validateScore() ? styleValid : styleInvalid);
}

bool AddStudentDialog::validateId() {
    QString idText = idEdit->text();
    bool ok;
    int id = idText.toInt(&ok);
    return ok && id > 0 && id <= 99999;
}

bool AddStudentDialog::validateName() {
    return !nameEdit->text().trimmed().isEmpty();
}

bool AddStudentDialog::validateGroup() {
    return !groupEdit->text().trimmed().isEmpty();
}

bool AddStudentDialog::validateScore() {
    double score = scoreEdit->value();
    return score >= 0.0 && score <= 10.0;
}