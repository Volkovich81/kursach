#include "../headers/SessionDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QPushButton>

SessionDialog::SessionDialog(QWidget* parent, CinemaSystem* system, Session* editingSession)
    : QDialog(parent), cinemaSystem(system), selectedMovie(nullptr), selectedHall(nullptr), currentEditingSession(editingSession)
{
    setupUI();
    populateMovies();
    populateHalls();

    // Если редактируем существующий сеанс - заполняем данные
    if (currentEditingSession) {
        setSessionData(currentEditingSession);
    }

    validateInputs();
}

void SessionDialog::setupUI()
{
    setWindowTitle("Добавить сеанс");
    setModal(true);
    setMinimumWidth(400);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QFormLayout* formLayout = new QFormLayout();

    movieComboBox = new QComboBox();
    hallComboBox = new QComboBox();

    QHBoxLayout* dateTimeLayout = new QHBoxLayout();
    dateEdit = new QDateEdit();
    dateEdit->setCalendarPopup(true);
    dateEdit->setDate(QDate::currentDate().addDays(1));
    dateEdit->setMinimumDate(QDate::currentDate().addDays(1));

    timeEdit = new QTimeEdit();
    timeEdit->setTime(QTime(18, 0));

    dateTimeLayout->addWidget(dateEdit);
    dateTimeLayout->addWidget(timeEdit);
 
    priceSpinBox = new QDoubleSpinBox();
    priceSpinBox->setRange(100, 2000);
    priceSpinBox->setSingleStep(50);
    priceSpinBox->setSuffix(" руб");

    formLayout->addRow("Фильм:", movieComboBox);
    formLayout->addRow("Зал:", hallComboBox);
    formLayout->addRow("Дата и время:", dateTimeLayout);
    formLayout->addRow("Цена:", priceSpinBox);

    validationLabel = new QLabel();
    validationLabel->setStyleSheet("color: red;");
    validationLabel->setWordWrap(true);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    layout->addLayout(formLayout);
    layout->addWidget(validationLabel);
    layout->addWidget(buttonBox);

    connect(movieComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(validateInputs()));
    connect(hallComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(validateInputs()));
    connect(priceSpinBox, SIGNAL(valueChanged(double)), this, SLOT(validateInputs()));
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(dateEdit, &QDateEdit::dateChanged, this, &SessionDialog::validateInputs);
    connect(timeEdit, &QTimeEdit::timeChanged, this, &SessionDialog::validateInputs);

    validateInputs(); // Первоначальная валидация
}

void SessionDialog::validateInputs()
{

    // Сбрасываем предыдущие значения
    selectedMovie = nullptr;
    selectedHall = nullptr;

    // Безопасное получение данных
    if (movieComboBox->currentIndex() >= 0) {
        selectedMovie = movieComboBox->currentData().value<Movie*>();
    }

    if (hallComboBox->currentIndex() >= 0) {
        selectedHall = hallComboBox->currentData().value<CinemaHall*>();
    }

    selectedMovie = movieComboBox->currentData().value<Movie*>();
    selectedHall = hallComboBox->currentData().value<CinemaHall*>();

    double price = priceSpinBox->value();

    bool isValid = true;
    QString errorMessage;

    if (!selectedMovie) {
        isValid = false;
        errorMessage += "Выберите фильм\n";
    }

    if (!selectedHall) {
        isValid = false;
        errorMessage += "Выберите зал\n";
    }


    if (price <= 0) {
        isValid = false;
        errorMessage += "Цена должна быть положительной\n";
    }

    // ★★★ ДОБАВИТЬ ПРОВЕРКУ ДОСТУПНОСТИ ЗАЛА ★★★
    if (isValid && selectedHall && selectedMovie) {
        QDateTime dateTime = QDateTime(dateEdit->date(), timeEdit->time());
        if (!cinemaSystem->getSessionService().isHallAvailable(
            selectedHall->getHallNumber(), dateTime, selectedMovie->getDuration(),
            currentEditingSession)) { // ★★★ ДОБАВИТЕ ЭТУ ПЕРЕМЕННУЮ ★★★
            isValid = false;
            errorMessage += "❌ Зал занят в это время!\n";
        }
    }

    validationLabel->setText(errorMessage);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(isValid);
}

/*bool SessionDialog::isHallAvailable(int hallNumber, const QDateTime& dateTime) {
    if (!cinemaSystem || !selectedMovie) return true;

    try {
        std::string datetimeStr = dateTime.toString("yyyy-MM-dd HH:mm").toStdString();

        cinemaSystem->createSession(selectedMovie,
            selectedHall,
            datetimeStr,
            priceSpinBox->value());

        return true;
    }
    catch (const BusinessLogicException& e) {
        return false;
    }
    catch (...) {
        return true;
    }
}*/

void SessionDialog::populateMovies()
{
    movieComboBox->clear();
    std::vector<Movie*> movies = cinemaSystem->getAllMovies();
    for (Movie* movie : movies) {
        if (movie) {
            movieComboBox->addItem(QString::fromStdString(movie->getTitle()), QVariant::fromValue(movie));
        }
    }
}

void SessionDialog::populateHalls()
{
    hallComboBox->clear();

    // Безопасный способ получения залов
    if (!cinemaSystem) return;

    // Используем доступ к залам через существующие методы
    for (int i = 0; i < cinemaSystem->hallsCount; i++) {
        CinemaHall* hall = cinemaSystem->halls[i];
        if (hall) {
            hallComboBox->addItem("Зал " + QString::number(hall->getHallNumber()), QVariant::fromValue(hall));
        }
    }

    // Если залов нет, добавляем сообщение
    if (hallComboBox->count() == 0) {
        hallComboBox->addItem("Нет доступных залов", QVariant::fromValue<CinemaHall*>(nullptr));
    }
}

QString SessionDialog::getDateTime() const {
    if (dateEdit && timeEdit) {
        return dateEdit->date().toString("yyyy-MM-dd") + " " + timeEdit->time().toString("HH:mm");
    }
    return "";
}

void SessionDialog::setSessionData(Session* session)
{
    if (!session) return;

    // Устанавливаем текущий фильм
    Movie* currentMovie = session->getMovie();
    if (currentMovie) {
        for (int i = 0; i < movieComboBox->count(); i++) {
            Movie* movie = movieComboBox->itemData(i).value<Movie*>();
            if (movie && movie->getId() == currentMovie->getId()) {
                movieComboBox->setCurrentIndex(i);
                selectedMovie = movie;
                break;
            }
        }
    }

    // Устанавливаем текущий зал
    CinemaHall* currentHall = session->getHall();
    if (currentHall) {
        for (int i = 0; i < hallComboBox->count(); i++) {
            CinemaHall* hall = hallComboBox->itemData(i).value<CinemaHall*>();
            if (hall && hall->getHallNumber() == currentHall->getHallNumber()) {
                hallComboBox->setCurrentIndex(i);
                selectedHall = hall;
                break;
            }
        }
    }

    // Устанавливаем дату и время
    QString datetimeStr = QString::fromStdString(session->getDateTime());
    QDateTime datetime = QDateTime::fromString(datetimeStr, "yyyy-MM-dd HH:mm");
    if (datetime.isValid()) {
        dateEdit->setDate(datetime.date());
        timeEdit->setTime(datetime.time());
    }

    // Устанавливаем цену
    priceSpinBox->setValue(session->getTicketPrice());
}