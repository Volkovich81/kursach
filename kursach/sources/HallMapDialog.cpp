#include "../headers/HallMapDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QScrollArea>

HallMapDialog::HallMapDialog(QWidget* parent, Session* session, int selectedSeat)
    : QDialog(parent), session(session), selectedSeat(selectedSeat)
{
    setupUI();
}

void HallMapDialog::setupUI()
{
    setWindowTitle("Выбор места - " + QString::fromStdString(session->getMovie()->getTitle()));
    setModal(true);
    setMinimumSize(800, 600);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Информация о сеансе
    QLabel* infoLabel = new QLabel(
        "🎬 " + QString::fromStdString(session->getMovie()->getTitle()) +
        "   |   🕒 " + QString::fromStdString(session->getDateTime()) +
        "   |   💺 Зал " + QString::number(session->getHall()->getHallNumber())
    );
    infoLabel->setStyleSheet("font-size: 14px; font-weight: bold; padding: 10px;");
    mainLayout->addWidget(infoLabel);

    // Экран
    QLabel* screenLabel = new QLabel("🎬 🎬 🎬 🎬 🎬 Э К Р А Н 🎬 🎬 🎬 🎬 🎬");
    screenLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50; background-color: #34495e; color: white; padding: 10px; margin: 10px; border-radius: 5px;");
    screenLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(screenLabel);

    // Прокручиваемая область для мест
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);

    QWidget* seatsWidget = new QWidget;
    gridLayout = new QGridLayout(seatsWidget);
    gridLayout->setSpacing(5);
    gridLayout->setAlignment(Qt::AlignCenter);

    createSeatLayout();

    scrollArea->setWidget(seatsWidget);
    mainLayout->addWidget(scrollArea);

    // Легенда
    QHBoxLayout* legendLayout = new QHBoxLayout();
    legendLayout->addStretch();

    QLabel* freeLabel = new QLabel("🟢 Свободно");
    QLabel* occupiedLabel = new QLabel("🔴 Занято");
    QLabel* selectedLabel = new QLabel("🟡 Выбрано");
    QLabel* vipLabel = new QLabel("🔵 VIP");

    for (QLabel* label : { freeLabel, occupiedLabel, selectedLabel, vipLabel }) {
        label->setStyleSheet("margin: 0 10px;");
        legendLayout->addWidget(label);
    }

    legendLayout->addStretch();
    mainLayout->addLayout(legendLayout);

    // Кнопки
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* confirmButton = new QPushButton("✅ Подтвердить выбор");
    QPushButton* cancelButton = new QPushButton("❌ Отмена");

    confirmButton->setStyleSheet("QPushButton { background-color: #27ae60; color: white; padding: 10px; font-weight: bold; }");
    cancelButton->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; padding: 10px; }");

    connect(confirmButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    buttonLayout->addWidget(confirmButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);
}

void HallMapDialog::createSeatLayout()
{
    if (!session || !session->getHall()) return;

    CinemaHall* hall = session->getHall();
    int totalSeats = hall->getTotalSeats();
    int seatsPerRow = 10; // 10 мест в ряду
    int totalRows = (totalSeats + seatsPerRow - 1) / seatsPerRow;

    // Добавляем номера рядов слева
    for (int row = 0; row < totalRows; row++) {
        QLabel* rowLabel = new QLabel(QString("Ряд %1").arg(row + 1));
        rowLabel->setAlignment(Qt::AlignCenter);
        rowLabel->setStyleSheet("font-weight: bold; margin: 5px;");
        gridLayout->addWidget(rowLabel, row, 0);
    }

    // Создаем места
    for (int seatNum = 1; seatNum <= totalSeats; seatNum++) {
        int row = (seatNum - 1) / seatsPerRow;
        int col = (seatNum - 1) % seatsPerRow + 1; // +1 потому что 0 колонка занята номерами рядов

        QPushButton* seatBtn = new QPushButton(QString::number(seatNum));
        seatBtn->setFixedSize(40, 40);
        seatBtn->setProperty("seatNumber", seatNum);

        // Определяем тип места
        bool isVip = (row < 2); // Первые 2 ряда - VIP
        bool isOccupied = !session->isSeatAvailable(seatNum - 1);
        bool isSelected = (seatNum == selectedSeat);

        QString style = "QPushButton { ";
        style += "font-size: 12px; ";     // ★★★ ЧУТЬ МЕНЬШЕ ШРИФТ ★★★
        style += "font-weight: bold; ";
        style += "color: white; ";
        style += "padding: 2px; ";        // ★★★ ДОБАВИЛИ ОТСТУПЫ ★★★

        if (isSelected) {
            style += "background-color: #f39c12; border: 2px solid #d35400;";
        }
        else if (isOccupied) {
            style += "background-color: #e74c3c;";
            seatBtn->setEnabled(false);
        }
        else if (isVip) {
            style += "background-color: #3498db;";
        }
        else {
            style += "background-color: #27ae60;";
        }

        style += " border-radius: 3px; }";

        seatBtn->setStyleSheet(style);
        connect(seatBtn, &QPushButton::clicked, this, &HallMapDialog::onSeatClicked);

        gridLayout->addWidget(seatBtn, row, col);
        seatButtons.append(seatBtn);
    }
}

void HallMapDialog::onSeatClicked()
{
    QPushButton* clickedBtn = qobject_cast<QPushButton*>(sender());
    if (clickedBtn) {
        int newSelectedSeat = clickedBtn->property("seatNumber").toInt();

        // Если место уже занято - ничего не делаем
        if (!session->isSeatAvailable(newSelectedSeat - 1)) {  // -1 потому что в CinemaHall места с 0
            QMessageBox::warning(this, "Место занято", "Это место уже занято! Выберите другое.");
            return;
        }

        // Сбрасываем предыдущее выбранное место
        if (selectedSeat > 0) {
            for (QPushButton* btn : seatButtons) {
                if (btn->property("seatNumber").toInt() == selectedSeat) {
                    bool isVip = ((selectedSeat - 1) / 10 < 2);
                    QString style = "QPushButton { ";
                    style += isVip ? "background-color: #3498db; color: white; font-weight: bold;"
                        : "background-color: #27ae60; color: white;";
                    style += " border-radius: 3px; }";
                    btn->setStyleSheet(style);
                    break;
                }
            }
        }

        // Устанавливаем новое выбранное место
        selectedSeat = newSelectedSeat;
        clickedBtn->setStyleSheet(
            "QPushButton { background-color: #f39c12; color: white; font-weight: bold; border: 2px solid #d35400; border-radius: 3px; }"
        );

        // НА этот:
        double finalPrice = calculateTicketPrice();
        QString seatInfo = QString("Выбрано место: %1\n").arg(selectedSeat);

        if ((selectedSeat - 1) / 10 < 2) {
            seatInfo += QString("💎 VIP место | 💰 Цена: %1 руб (+20%)").arg(finalPrice);
        }
        else {
            seatInfo += QString("💰 Цена: %1 руб").arg(finalPrice);
        }

        QMessageBox::information(this, "Место выбрано", seatInfo);
    }
}

// ★★★ ДОБАВЬТЕ ЭТОТ МЕТОД В КОНЕЦ ФАЙЛА HallMapDialog.cpp ★★★
double HallMapDialog::calculateTicketPrice() const {
    if (!session) return 0.0;

    double basePrice = session->getTicketPrice();

    // Проверяем VIP место (первые 2 ряда)
    if (selectedSeat > 0 && (selectedSeat - 1) / 10 < 2) {
        return basePrice * 1.2; // +20% для VIP
    }

    return basePrice;
}