#include "headers/HallDialog.h"
#include "headers/CinemaSystem.h" 
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QPushButton>

HallDialog::HallDialog(QWidget* parent, CinemaSystem* system)
    : QDialog(parent), cinemaSystem(system)
{
    setupUI();
    validateInputs();
}

void HallDialog::setupUI()
{
    setWindowTitle("➕ Добавить зал");
    setModal(true);
    setMinimumWidth(300);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QFormLayout* formLayout = new QFormLayout();

    hallNumberSpinBox = new QSpinBox();
    hallNumberSpinBox->setRange(1, 20);
    hallNumberSpinBox->setValue(1);

    seatsSpinBox = new QSpinBox();
    seatsSpinBox->setRange(10, 200);
    seatsSpinBox->setValue(50);
    seatsSpinBox->setSuffix(" мест");

    formLayout->addRow("Номер зала:", hallNumberSpinBox);
    formLayout->addRow("Количество мест:", seatsSpinBox);

    // ДОБАВЬ LABEL ДЛЯ ВАЛИДАЦИИ
    validationLabel = new QLabel();
    validationLabel->setStyleSheet("color: red;");
    validationLabel->setWordWrap(true);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Ok)->setText("Добавить");

    layout->addLayout(formLayout);
    layout->addWidget(validationLabel);  // ДОБАВЬ ЭТУ СТРОКУ
    layout->addWidget(buttonBox);

    connect(hallNumberSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
        this, &HallDialog::validateInputs);
    connect(seatsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
        this, &HallDialog::validateInputs);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void HallDialog::validateInputs()
{
    bool isValid = hallNumberSpinBox->value() > 0 && seatsSpinBox->value() > 0;
    QString errorMessage;

    // ПРОВЕРКА УНИКАЛЬНОСТИ НОМЕРА ЗАЛА
    if (cinemaSystem && isValid) {
        int hallNumber = hallNumberSpinBox->value();
        for (int i = 0; i < cinemaSystem->hallsCount; i++) {
            if (cinemaSystem->halls[i] && cinemaSystem->halls[i]->getHallNumber() == hallNumber) {
                isValid = false;
                errorMessage = "❌ Зал с таким номером уже существует!";
                break;
            }
        }
    }

    validationLabel->setText(errorMessage);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(isValid);
}