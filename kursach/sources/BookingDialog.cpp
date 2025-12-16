#include "../headers/BookingDialog.h"
#include "../headers/EmailValidator.h"
#include "../headers/DateUtils.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QtWidgets>
#include "../headers/HallMapDialog.h"  

BookingDialog::BookingDialog(QWidget* parent, CinemaSystem* system)
    : QDialog(parent), cinemaSystem(system)
{
    setupUI();
    populateSessions();
    validateInputs();
}

void BookingDialog::setupUI()
{
    setWindowTitle("🎫 Бронирование билета");
    setModal(true);
    setMinimumWidth(500);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QFormLayout* formLayout = new QFormLayout();

    sessionComboBox = new QComboBox();
    seatSpinBox = new QSpinBox(this);  // ← ДОБАВЬ this как parent
    seatSpinBox->setRange(1, 100);
    seatSpinBox->setValue(1);

    // ДОБАВИМ КНОПКУ ВЫБОРА МЕСТА РЯДОМ СО SPINBOX
    QHBoxLayout* seatLayout = new QHBoxLayout();
    seatLayout->addWidget(seatSpinBox);

    selectSeatButton = new QPushButton("🗺️ Выбрать на карте");
    selectSeatButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; padding: 5px; }");
    seatLayout->addWidget(selectSeatButton);

    customerNameEdit = new QLineEdit();
    customerEmailEdit = new QLineEdit();

    formLayout->addRow("Сеанс:", sessionComboBox);
    formLayout->addRow("Место:", seatLayout);  // ИСПОЛЬЗУЕМ LAYOUT ВМЕСТО SPINBOX
    formLayout->addRow("Имя:", customerNameEdit);
    formLayout->addRow("Email:", customerEmailEdit);

    validationLabel = new QLabel();
    validationLabel->setStyleSheet("color: red;");
    validationLabel->setWordWrap(true);

    seatInfoLabel = new QLabel();
    seatInfoLabel->setStyleSheet("color: #2c3e50;");

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Ok)->setText("🎫 Забронировать");
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    layout->addLayout(formLayout);
    layout->addWidget(seatInfoLabel);
    layout->addWidget(validationLabel);
    layout->addWidget(buttonBox);

    connect(sessionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(validateInputs()));
    connect(sessionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSessionChanged(int)));
    connect(seatSpinBox, SIGNAL(valueChanged(int)), this, SLOT(validateInputs()));
    connect(customerNameEdit, &QLineEdit::textChanged, this, &BookingDialog::validateInputs);
    connect(customerEmailEdit, &QLineEdit::textChanged, this, &BookingDialog::validateInputs);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // ДОБАВИМ ПОДКЛЮЧЕНИЕ ДЛЯ КНОПКИ ВЫБОРА МЕСТА
    connect(selectSeatButton, &QPushButton::clicked, this, &BookingDialog::onSelectSeatClicked);
}

// ДОБАВИМ НОВЫЙ МЕТОД ДЛЯ ВЫБОРА МЕСТА НА КАРТЕ
void BookingDialog::onSelectSeatClicked()
{
    if (sessionComboBox->currentIndex() == -1) {
        QMessageBox::warning(this, "Ошибка", "Сначала выберите сеанс!");
        return;
    }

    int sessionId = sessionComboBox->currentData().toInt();
    Session* session = cinemaSystem->getSessionById(sessionId);

    if (!session) {
        QMessageBox::warning(this, "Ошибка", "Сеанс не найден!");
        return;
    }

    int currentSeat = seatSpinBox->value();

    // ОТКРЫВАЕМ КАРТУ ЗАЛА
    HallMapDialog mapDialog(this, session, currentSeat);
    if (mapDialog.exec() == QDialog::Accepted) {
        int selectedSeat = mapDialog.getSelectedSeat();
        seatSpinBox->setValue(selectedSeat);

        QMessageBox::information(this, "Место выбрано",
            QString("Выбрано место: %1").arg(selectedSeat));
    }
}

void BookingDialog::populateSessions()
{
    sessionComboBox->clear();
    for (int i = 0; i < cinemaSystem->sessionsCount; i++) {
        Session* session = cinemaSystem->sessions[i];
        if (session != nullptr && session->getMovie() != nullptr && session->getHall() != nullptr) {
            // Показываем только будущие сеансы
            if (!DateUtils::isPast(QString::fromStdString(session->getDateTime()))) {
                QString sessionText = QString::fromStdString(session->getMovie()->getTitle()) +
                    " - Зал " + QString::number(session->getHall()->getHallNumber()) +
                    " - " + QString::fromStdString(session->getDateTime()) +
                    " - " + QString::number(session->getTicketPrice()) + " руб";

                sessionComboBox->addItem(sessionText, session->getSessionId());
            }
        }
    }
}

void BookingDialog::validateInputs()
{
    // ВРЕМЕННО - ВСЕГДА ВКЛЮЧАЕМ КНОПКУ
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    validationLabel->setText(""); // Очищаем сообщения об ошибках

    
    // ЗАКОММЕНТИРУЕМ СТАРУЮ ВАЛИДАЦИЮ
    bool isValid = true;
    QString errorMessage;

    if (sessionComboBox->currentIndex() == -1) {
        isValid = false;
        errorMessage += "• Выберите сеанс\n";
    }

    if (customerNameEdit->text().isEmpty()) {
        isValid = false;
        errorMessage += "• Введите имя\n";
    }

    if (customerEmailEdit->text().isEmpty()) {
        isValid = false;
        errorMessage += "• Введите email\n";
    }
    else if (!EmailValidator::isValid(customerEmailEdit->text())) {
        isValid = false;
        errorMessage += "• Неверный формат email\n";
    }

    // Проверка доступности места
    if (sessionComboBox->currentIndex() != -1) {
        int sessionId = sessionComboBox->currentData().toInt();
        Session* session = cinemaSystem->getSessionById(sessionId);
        if (session && !session->isSeatAvailable(seatSpinBox->value())) {
            isValid = false;
            errorMessage += "• Место уже занято\n";
        }
    }

    validationLabel->setText(errorMessage);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(isValid);
   
}

void BookingDialog::onSessionChanged(int index)
{
    if (index == -1) return;

    int sessionId = sessionComboBox->currentData().toInt();
    Session* session = cinemaSystem->getSessionById(sessionId);

    if (session && session->getHall()) {
        CinemaHall* hall = session->getHall();
        seatSpinBox->setRange(1, hall->getTotalSeats());

        double basePrice = session->getTicketPrice();

        // ★★★ ОБНОВИТЕ ИНФОРМАЦИЮ О ЦЕНАХ ★★★
        QString seatInfo = QString("💺 Свободно мест: %1 из %2\n").arg(hall->getAvailableSeatsCount()).arg(hall->getTotalSeats());
        seatInfo += QString("💰 Обычные: %1 руб\n").arg(basePrice);
        seatInfo += QString("💎 VIP (ряды 1-2): %1 руб").arg(basePrice * 1.2);

        seatInfoLabel->setText(seatInfo);
    }
}
int BookingDialog::getSelectedSessionId() const {
    return sessionComboBox->currentData().toInt();
}

int BookingDialog::getSelectedSeat() const {
    return seatSpinBox->value();
}

QString BookingDialog::getCustomerName() const {
    return customerNameEdit->text();
}

QString BookingDialog::getCustomerEmail() const {
    return customerEmailEdit->text();
}