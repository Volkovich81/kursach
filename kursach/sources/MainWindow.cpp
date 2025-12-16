#include "../headers/MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>
#include <QGroupBox>
#include <QFormLayout>
#include <QSpinBox>
#include <QDateTimeEdit>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QRegularExpression>
#include <QDateTime>
#include <QRandomGenerator>
#include <vector>
#include "../headers/EmailValidator.h"
#include "../headers/PriceFormatter.h"
#include "../headers/DateUtils.h"
#include "../headers/CinemaException.h"
#include "../headers/MovieService.h"
#include "../headers/HallMapDialog.h"
#include "../headers/BookingDialog.h"
#include "headers/MovieDialog.h"
#include "headers/HallDialog.h"
#include <QFile>
#include <QTextStream>
#include <QStatusBar>
#include <QApplication>
#include <QStyle>

using namespace std;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , cinemaSystem(new CinemaSystem())
{
    // ★★★ КРАСИВЫЙ СТИЛЬ ДЛЯ ВСЕГО ПРИЛОЖЕНИЯ ★★★
    setStyleSheet(
        "QMainWindow {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #2c3e50, stop:1 #34495e);"
        "}"
        "QTabWidget::pane {"
        "    border: 2px solid #1a252f;"
        "    background-color: #ecf0f1;"
        "    border-radius: 8px;"
        "    margin: 5px;"
        "}"
        "QTabBar::tab {"
        "    background: #3498db;"
        "    color: white;"
        "    padding: 12px 20px;"
        "    margin: 2px;"
        "    border-radius: 6px;"
        "    font-weight: bold;"
        "    font-size: 12px;"
        "}"
        "QTabBar::tab:selected {"
        "    background: #2980b9;"
        "    border-bottom: 3px solid #e74c3c;"
        "}"
        "QTabBar::tab:hover {"
        "    background: #5dade2;"
        "    transform: scale(1.05);"
        "}"
        "QTableWidget {"
        "    background-color: white;"
        "    gridline-color: #bdc3c7;"
        "    border: 1px solid #bdc3c7;"
        "    border-radius: 6px;"
        "    font-size: 11px;"
        "}"
        "QTableWidget::item {"
        "    padding: 6px;"
        "    border-bottom: 1px solid #ecf0f1;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: #3498db;"
        "    color: white;"
        "    font-weight: bold;"
        "}"
        "QHeaderView::section {"
        "    background-color: #34495e;"
        "    color: white;"
        "    padding: 10px;"
        "    border: none;"
        "    font-weight: bold;"
        "    font-size: 11px;"
        "}"
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3498db, stop:1 #2980b9);"
        "    color: white;"
        "    border: none;"
        "    padding: 10px 16px;"
        "    border-radius: 6px;"
        "    font-weight: bold;"
        "    font-size: 11px;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #5dade2, stop:1 #3498db);"
        "    border: 2px solid #2c3e50;"
        "}"
        "QPushButton:pressed {"
        "    background: #21618c;"
        "}"
        "QComboBox, QLineEdit, QSpinBox, QDateEdit, QDoubleSpinBox {"
        "    padding: 8px;"
        "    border: 2px solid #bdc3c7;"
        "    border-radius: 5px;"
        "    background: white;"
        "    font-size: 11px;"
        "    selection-background-color: #3498db;"
        "}"
        "QComboBox:focus, QLineEdit:focus, QSpinBox:focus, QDateEdit:focus, QDoubleSpinBox:focus {"
        "    border-color: #3498db;"
        "    border-width: 2px;"
        "}"
        "QComboBox::drop-down {"
        "    border: none;"
        "}"
        "QComboBox::down-arrow {"
        "    image: none;"
        "    border-left: 5px solid transparent;"
        "    border-right: 5px solid transparent;"
        "    border-top: 5px solid #2c3e50;"
        "    width: 0px;"
        "    height: 0px;"
        "}"
        "QLabel {"
        "    color: #2c3e50;"
        "    font-weight: bold;"
        "}"
    );

    setupUI();

    // ★★★ СТАТУС БАР С ИНФОРМАЦИЕЙ ★★★
    QStatusBar* statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    statusBar->setStyleSheet(
        "QStatusBar {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #34495e, stop:1 #2c3e50);"
        "    color: white;"
        "    font-weight: bold;"
        "    padding: 5px;"
        "}"
    );
    statusBar->showMessage("✅ Система готова к работе | 🎭 Фильмы: " +
        QString::number(cinemaSystem->moviesCount) +
        " | 🏛️ Залы: " + QString::number(cinemaSystem->hallsCount) +
        " | 🕒 Сеансы: " + QString::number(cinemaSystem->sessionsCount));

    setupConnections();
    refreshAll();

    // ★★★ УЛУЧШЕННЫЙ ЗАГОЛОВОК ОКНА ★★★
    setWindowTitle("🎬 CinemaPro - Система управления кинотеатром");
    setMinimumSize(1000, 700);

    // ★★★ ЦЕНТРИРУЕМ ОКНО НА ЭКРАНЕ ★★★
    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            size(),
            qApp->primaryScreen()->availableGeometry()
        )
    );
}

MainWindow::~MainWindow()
{
    delete cinemaSystem;
}

void MainWindow::setupUI()
{

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // ★★★ УЛУЧШЕННЫЙ ЗАГОЛОВОК ★★★
    QLabel* titleLabel = new QLabel("🎬 CINEMAPRO - СИСТЕМА УПРАВЛЕНИЯ КИНОТЕАТРОМ");
    titleLabel->setStyleSheet(
        "font-size: 26px; font-weight: bold; color: white; "
        "padding: 25px; background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #e74c3c, stop:1 #c0392b);"
        "border-radius: 12px; margin: 10px; border: 3px solid #2c3e50;"
    );
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    tabWidget = new QTabWidget(this);
    mainLayout->addWidget(tabWidget);

    QWidget* moviesTab = new QWidget();
    QVBoxLayout* moviesLayout = new QVBoxLayout(moviesTab);
    // ПРОСТОЙ ПОИСК - только поле ввода
   // ЗАМЕНИТЬ существующий searchLayout на:
    QHBoxLayout* searchLayout = new QHBoxLayout();

    searchMovieEdit = new QLineEdit();
    searchMovieEdit->setPlaceholderText("🔍 Введите текст для поиска...");
    searchMovieEdit->setClearButtonEnabled(true);

    // ComboBox для выбора типа поиска
    searchTypeComboBox = new QComboBox();
    searchTypeComboBox->addItem("Название");
    searchTypeComboBox->addItem("Жанр");
    searchTypeComboBox->addItem("Режиссер");
    searchTypeComboBox->setMaximumWidth(100);

    searchLayout->addWidget(searchTypeComboBox);
    searchLayout->addWidget(searchMovieEdit);
    searchLayout->addStretch();
    moviesLayout->addLayout(searchLayout);
    moviesTable = new QTableWidget(this);
    moviesTable->setColumnCount(7);
    moviesTable->setHorizontalHeaderLabels({ "ID", "Название", "Жанр", "Длительность", "Режиссер", "Возраст", "Действия" });
    moviesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    moviesLayout->addWidget(moviesTable);

    QHBoxLayout* moviesButtonsLayout = new QHBoxLayout();
    addMovieButton = new QPushButton("➕ Добавить фильм");
    historyButton = new QPushButton("📜 История сеансов");

    addMovieButton->setStyleSheet("QPushButton { background-color: #27ae60; color: white; padding: 8px; border-radius: 5px; }");
    historyButton->setStyleSheet("QPushButton { background-color: #7f8c8d; color: white; padding: 8px; border-radius: 5px; }");

    moviesButtonsLayout->addWidget(addMovieButton);
    moviesButtonsLayout->addStretch();
    moviesButtonsLayout->addWidget(historyButton);
    moviesLayout->addLayout(moviesButtonsLayout);
    // ==== ДОБАВЬ ЭТОТ КОД ПРЯМО ЗДЕСЬ ====
    // ==== КОНЕЦ ДОБАВЛЕНИЯ ====
    moviesLayout->addLayout(moviesButtonsLayout);

    tabWidget->addTab(moviesTab, "🎭 Фильмы");

    QWidget* hallsTab = new QWidget();
    QVBoxLayout* hallsLayout = new QVBoxLayout(hallsTab);

    hallsTable = new QTableWidget(this);
    hallsTable->setColumnCount(3);
    hallsTable->setHorizontalHeaderLabels({ "ID", "Номер зала", "Количество мест" });
    hallsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    hallsLayout->addWidget(hallsTable);

    QHBoxLayout* hallsButtonsLayout = new QHBoxLayout();
    addHallButton = new QPushButton("➕ Добавить зал");

    addHallButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; padding: 8px; border-radius: 5px; }");

    hallsButtonsLayout->addWidget(addHallButton);
    hallsButtonsLayout->addStretch();
    hallsLayout->addLayout(hallsButtonsLayout);

    tabWidget->addTab(hallsTab, "🏛️ Залы");

    QWidget* sessionsTab = new QWidget();
    QVBoxLayout* sessionsLayout = new QVBoxLayout(sessionsTab);
    QHBoxLayout* sessionSearchLayout = new QHBoxLayout();

    sessionSearchComboBox = new QComboBox();
    sessionSearchComboBox->addItem("Название фильма");
    sessionSearchComboBox->addItem("Номер зала");
    sessionSearchComboBox->addItem("Дата");
    sessionSearchComboBox->addItem("Цена");
    sessionSearchComboBox->setMaximumWidth(120);

    // ★★★ ЗАМЕНЯЕМ QLineEdit на QDateEdit для поиска по дате ★★★
    sessionSearchEdit = new QLineEdit();
    sessionSearchEdit->setPlaceholderText("🔍 Введите значение для поиска...");
    sessionSearchEdit->setClearButtonEnabled(true);

    sessionSearchDateEdit = new QDateEdit();  // ★★★ НОВЫЙ ВИДЖЕТ ДАТЫ ★★★
    sessionSearchDateEdit->setCalendarPopup(true);  // ★★★ ВЫПАДАЮЩИЙ КАЛЕНДАРЬ ★★★
    sessionSearchDateEdit->setDate(QDate::currentDate());
    sessionSearchDateEdit->setDisplayFormat("dd.MM.yyyy");
    sessionSearchDateEdit->setMaximumWidth(120);
    sessionSearchDateEdit->setVisible(false);  // ★★★ СКРЫТ ПО УМОЛЧАНИЮ ★★★

    sessionSearchLayout->addWidget(sessionSearchComboBox);
    sessionSearchLayout->addWidget(sessionSearchEdit);      // Для текстового поиска
    sessionSearchLayout->addWidget(sessionSearchDateEdit);  // Для поиска по дате
    sessionSearchLayout->addStretch();

    sessionFilterComboBox = new QComboBox();
    sessionFilterComboBox->addItem("🟢 Активные (будущие)");
    sessionFilterComboBox->addItem("🔴 Неактивные (прошедшие)");
    sessionFilterComboBox->addItem("📊 Все сеансы");
    sessionSearchLayout->addWidget(sessionFilterComboBox);

    sessionsLayout->addLayout(sessionSearchLayout);

    sessionsTable = new QTableWidget(this);
    sessionsTable->setColumnCount(8);

    sessionsTable->setHorizontalHeaderLabels({ "ID", "Фильм", "Зал", "Дата и время", "Цена", "Свободно", "Статус", "Действия" });
    sessionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    sessionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // ← ДОБАВЬ ЕСЛИ НЕТ
    sessionsLayout->addWidget(sessionsTable);

    QHBoxLayout* sessionsButtonsLayout = new QHBoxLayout();
    addSessionButton = new QPushButton("➕ Добавить сеанс");

    addSessionButton->setStyleSheet("QPushButton { background-color: #9b59b6; color: white; padding: 8px; border-radius: 5px; }");

    sessionsButtonsLayout->addWidget(addSessionButton);
    sessionsButtonsLayout->addStretch();
    sessionsButtonsLayout->addWidget(historyButton);
    sessionsLayout->addLayout(sessionsButtonsLayout);

    tabWidget->addTab(sessionsTab, "🕒 Сеансы");
}

void MainWindow::setupConnections()
{
    connect(addMovieButton, &QPushButton::clicked, this, &MainWindow::onAddMovieClicked);
    connect(addHallButton, &QPushButton::clicked, this, &MainWindow::onAddHallClicked);
    connect(addSessionButton, &QPushButton::clicked, this, &MainWindow::onAddSessionClicked);
    // ДОБАВЬ ЭТУ СТРОКУ:
    connect(historyButton, &QPushButton::clicked, this, &MainWindow::onShowHistoryClicked);

    // Автоматический поиск при вводе
    connect(searchMovieEdit, &QLineEdit::textChanged, this, [this](const QString& text) {
        currentSearchText = text.trimmed();

        // Определяем тип поиска
        QString type = searchTypeComboBox->currentText();
        if (type == "Название") currentSearchType = "title";
        else if (type == "Жанр") currentSearchType = "genre";
        else if (type == "Режиссер") currentSearchType = "director";

        refreshMoviesTable();
        });

    connect(sessionFilterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MainWindow::refreshSessionsTable);

    connect(sessionSearchEdit, &QLineEdit::textChanged, this, &MainWindow::refreshSessionsTable);
    connect(sessionSearchComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MainWindow::refreshSessionsTable);
    // ★★★ ДОБАВЬТЕ ЭТОТ CONNECT ★★★
    connect(sessionSearchComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MainWindow::onSessionSearchTypeChanged);
}

void MainWindow::refreshAll()
{
    refreshMoviesTable();
    refreshHallsTable();
    // refreshSessionsTable();      // ← пока оставить закомментированным
    // refreshSessionComboBox();    // ← пока оставить закомментированным
}

void MainWindow::refreshMoviesTable()
{
    moviesTable->setRowCount(0);
    vector<Movie*> movies = cinemaSystem->getAllMovies();

    /*QString searchText = searchMovieEdit->text().toLower();*/  // ОСТАВЬ ЗАКОММЕНТИРОВАННЫМ

    for (Movie* movie : movies) {
        // ДОБАВИТЬ ЭТУ ПРОВЕРКУ:
        if (movie == nullptr) {
            continue; // Пропускаем удаленные фильмы
        }
        if (movie != nullptr) {
            QString title = QString::fromStdString(movie->getTitle()).toLower();

            /*if (!searchText.isEmpty() && !title.contains(searchText)) {  // ОСТАВЬ ЗАКОММЕНТИРОВАННЫМ
                continue;
            }*/  // ОСТАВЬ ЗАКОММЕНТИРОВАННЫМ

            // ФИЛЬТРАЦИЯ ПО РАСШИРЕННОМУ ПОИСКУ
            if (!currentSearchText.isEmpty()) {
                bool match = false;
                QString search = currentSearchText.toLower();

                if (currentSearchType == "title") {
                    QString title = QString::fromStdString(movie->getTitle()).toLower();
                    match = title.contains(search);
                }
                else if (currentSearchType == "genre") {
                    QString genre = QString::fromStdString(movie->getGenre()).toLower();
                    match = genre.contains(search);
                }
                else if (currentSearchType == "director") {
                    QString director = QString::fromStdString(movie->getDirector()).toLower();
                    match = director.contains(search);
                }

                if (!match) continue;
            }

            int row = moviesTable->rowCount();
            moviesTable->insertRow(row);
            // ★★★ УВЕЛИЧИМ ВЫСОТУ СТРОКИ ★★★
            moviesTable->setRowHeight(row, 50);

            moviesTable->setItem(row, 0, new QTableWidgetItem(QString::number(movie->getId())));
            moviesTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(movie->getTitle())));
            moviesTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(movie->getGenre())));
            moviesTable->setItem(row, 3, new QTableWidgetItem(QString::number(movie->getDuration()) + " мин"));
            moviesTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(movie->getDirector())));
            moviesTable->setItem(row, 5, new QTableWidgetItem(QString::number(movie->getAgeRating()) + "+"));

            // КНОПКИ ДЕЙСТВИЙ - ОБНОВЛЕННАЯ ВЕРСИЯ
            QWidget* actionsWidget = new QWidget();
            QHBoxLayout* actionsLayout = new QHBoxLayout(actionsWidget);

            // ★★★ ДОБАВЬТЕ СЮДА 2 СТРОКИ:
            actionsLayout->setSpacing(2);
            actionsLayout->setContentsMargins(2, 2, 2, 2);

            // ★★★ ПРОБУЕМ СИСТЕМНЫЕ ИКОНКИ QT С ПРАВИЛЬНЫМИ НАСТРОЙКАМИ ★★★
            QPushButton* editBtn = new QPushButton();
            QPushButton* deleteBtn = new QPushButton();

            // ★★★ ВАЖНО: СНАЧАЛА РАЗМЕР КНОПКИ, ПОТОМ ИКОНКА ★★★
            editBtn->setFixedSize(30, 30);
            deleteBtn->setFixedSize(30, 30);

            // ★★★ УСТАНАВЛИВАЕМ ИКОНКУ ПОСЛЕ РАЗМЕРА КНОПКИ ★★★
            editBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogDetailedView));
            deleteBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_TrashIcon));

            // ★★★ УСТАНАВЛИВАЕМ РАЗМЕР ИКОНКИ ★★★
            editBtn->setIconSize(QSize(20, 20));
            deleteBtn->setIconSize(QSize(20, 20));

            // ★★★ ПРОСТЫЕ СТИЛИ БЕЗ ТЕКСТА ★★★
            editBtn->setStyleSheet("QPushButton { background-color: #3498db; border: 1px solid #2c3e50; border-radius: 3px; }");
            deleteBtn->setStyleSheet("QPushButton { background-color: #e74c3c; border: 1px solid #2c3e50; border-radius: 3px; }");

            editBtn->setToolTip("Редактировать");
            deleteBtn->setToolTip("Удалить");;

            // ОБРАБОТЧИК РЕДАКТИРОВАНИЯ
            connect(editBtn, &QPushButton::clicked, this, [this, movie]() {
                onEditMovieClicked(movie);
                });

            // ОБРАБОТЧИК УДАЛЕНИЯ (оставляем существующий)
            connect(deleteBtn, &QPushButton::clicked, this, [this, movie]() {
                // ПРОВЕРКА СЕАНСОВ
                bool hasSessions = false;
                for (int i = 0; i < cinemaSystem->sessionsCount; i++) {
                    if (cinemaSystem->sessions[i] &&
                        cinemaSystem->sessions[i]->getMovie() &&
                        cinemaSystem->sessions[i]->getMovie()->getId() == movie->getId()) {
                        hasSessions = true;
                        break;
                    }
                }

                if (hasSessions) {
                    QMessageBox::warning(this, "Ошибка",
                        QString("Нельзя удалить фильм '%1', для которого есть сеансы!")
                        .arg(QString::fromStdString(movie->getTitle())));
                    return;
                }

                // ПОДТВЕРЖДЕНИЕ
                if (QMessageBox::question(this, "Подтверждение удаления",
                    QString("Вы уверены, что хотите удалить фильм '%1'?")
                    .arg(QString::fromStdString(movie->getTitle())))
                    == QMessageBox::Yes) {

                    // УДАЛЯЕМ ИЗ ОБОИХ МЕСТ
                    // 1. Из MovieService
                    cinemaSystem->getMovieService().deleteMovie(movie->getId());

                    // 2. Из старого массива
                    for (int i = 0; i < cinemaSystem->moviesCount; i++) {
                        if (cinemaSystem->movies[i] && cinemaSystem->movies[i]->getId() == movie->getId()) {
                            cinemaSystem->movies[i] = nullptr;
                            break;
                        }
                    }

                    refreshMoviesTable();
                    QMessageBox::information(this, "Успех", "Фильм удален!");
                }
                });

            actionsLayout->addWidget(editBtn);
            actionsLayout->addWidget(deleteBtn);
            actionsLayout->setAlignment(Qt::AlignCenter);
            actionsLayout->setContentsMargins(0, 0, 0, 0);

            moviesTable->setCellWidget(row, 6, actionsWidget);
        }
    }
}
void MainWindow::refreshHallsTable()
{
    hallsTable->setRowCount(0);
    hallsTable->setColumnCount(4); // Добавляем колонку для действий
    hallsTable->setHorizontalHeaderLabels({ "ID", "Номер зала", "Количество мест", "Действия" });

    for (int i = 0; i < cinemaSystem->hallsCount; i++) {
        CinemaHall* hall = cinemaSystem->halls[i];
        if (hall != nullptr) {
            int row = hallsTable->rowCount();
            hallsTable->insertRow(row);

            // ★★★ УВЕЛИЧИМ ВЫСОТУ СТРОКИ ★★★
            hallsTable->setRowHeight(row, 50);

            hallsTable->setItem(row, 0, new QTableWidgetItem(QString::number(i + 1)));
            hallsTable->setItem(row, 1, new QTableWidgetItem(QString::number(hall->getHallNumber())));
            hallsTable->setItem(row, 2, new QTableWidgetItem(QString::number(hall->getTotalSeats())));

            // КНОПКИ ДЕЙСТВИЙ
            QWidget* actionsWidget = new QWidget();
            QHBoxLayout* actionsLayout = new QHBoxLayout(actionsWidget);

            // ★★★ ДОБАВЬТЕ СЮДА 2 СТРОКИ:
            actionsLayout->setSpacing(2);
            actionsLayout->setContentsMargins(2, 2, 2, 2);

            // ★★★ ПРОБУЕМ СИСТЕМНЫЕ ИКОНКИ QT С ПРАВИЛЬНЫМИ НАСТРОЙКАМИ ★★★
            QPushButton* editBtn = new QPushButton();
            QPushButton* deleteBtn = new QPushButton();

            // ★★★ ВАЖНО: СНАЧАЛА РАЗМЕР КНОПКИ, ПОТОМ ИКОНКА ★★★
            editBtn->setFixedSize(30, 30);
            deleteBtn->setFixedSize(30, 30);

            // ★★★ УСТАНАВЛИВАЕМ ИКОНКУ ПОСЛЕ РАЗМЕРА КНОПКИ ★★★
            editBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogDetailedView));
            deleteBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_TrashIcon));

            // ★★★ УСТАНАВЛИВАЕМ РАЗМЕР ИКОНКИ ★★★
            editBtn->setIconSize(QSize(20, 20));
            deleteBtn->setIconSize(QSize(20, 20));

            // ★★★ ПРОСТЫЕ СТИЛИ БЕЗ ТЕКСТА ★★★
            editBtn->setStyleSheet("QPushButton { background-color: #3498db; border: 1px solid #2c3e50; border-radius: 3px; }");
            deleteBtn->setStyleSheet("QPushButton { background-color: #e74c3c; border: 1px solid #2c3e50; border-radius: 3px; }");

            editBtn->setToolTip("Редактировать");
            deleteBtn->setToolTip("Удалить");

            connect(editBtn, &QPushButton::clicked, this, [this, hall]() {
                onEditHallClicked(hall);
                });

            connect(deleteBtn, &QPushButton::clicked, this, [this, hall]() {
                onDeleteHallClicked(hall);
                });

            actionsLayout->addWidget(editBtn);
            actionsLayout->addWidget(deleteBtn);
            actionsLayout->setAlignment(Qt::AlignCenter);
            actionsLayout->setContentsMargins(0, 0, 0, 0);

            hallsTable->setCellWidget(row, 3, actionsWidget);
        }
    }
}

void MainWindow::refreshSessionsTable()
{
    sessionsTable->setRowCount(0);

    QString searchText = sessionSearchEdit->text().trimmed().toLower();
    QString searchType = sessionSearchComboBox->currentText();
    int filterIndex = sessionFilterComboBox ? sessionFilterComboBox->currentIndex() : -1;

    for (int i = 0; i < cinemaSystem->sessionsCount; i++) {
        Session* session = cinemaSystem->sessions[i];
        if (session != nullptr && session->getMovie() != nullptr && session->getHall() != nullptr) {

            bool isPast = DateUtils::isPast(QString::fromStdString(session->getDateTime()));

            // ФИЛЬТРАЦИЯ БЕЗ ОТЛАДКИ
            if (filterIndex == 0 && isPast) {
                continue;
            }
            if (filterIndex == 1 && !isPast) {
                continue;
            }

            if (searchType == "Дата" && sessionSearchDateEdit->isVisible()) {
                // Поиск по календарю (только когда календарь виден)
                QDate searchDate = sessionSearchDateEdit->date();
                QDateTime sessionDateTime = QDateTime::fromString(QString::fromStdString(session->getDateTime()), "yyyy-MM-dd HH:mm");
                if (!sessionDateTime.isValid() || sessionDateTime.date() != searchDate) {
                    continue; // Пропускаем если дата не совпадает
                }
            }
            else if (!searchText.isEmpty()) {
                // Текстовый поиск для других типов
                bool match = false;

                if (searchType == "Название фильма") {
                    QString title = QString::fromStdString(session->getMovie()->getTitle()).toLower();
                    match = title.contains(searchText);
                }
                else if (searchType == "Номер зала") {
                    QString hallNumber = QString::number(session->getHall()->getHallNumber());
                    match = hallNumber.contains(searchText);
                }
                else if (searchType == "Дата") {
                    // Текстовый поиск по дате (если календарь не используется)
                    QString date = QString::fromStdString(session->getDateTime());
                    match = date.contains(searchText, Qt::CaseInsensitive);
                }
                else if (searchType == "Цена") {
                    QString price = QString::number(session->getTicketPrice());
                    match = price.contains(searchText);
                }

                if (!match) continue;
            }
            // ОСТАВЛЯЕМ ТОЛЬКО КОД ОТРИСОВКИ ТАБЛИЦЫ
            int row = sessionsTable->rowCount();
            sessionsTable->insertRow(row);

            // ★★★ УВЕЛИЧИМ ВЫСОТУ СТРОКИ ★★★
            sessionsTable->setRowHeight(row, 50);

            sessionsTable->setItem(row, 0, new QTableWidgetItem(QString::number(session->getSessionId())));
            sessionsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(session->getMovie()->getTitle())));
            sessionsTable->setItem(row, 2, new QTableWidgetItem("Зал " + QString::number(session->getHall()->getHallNumber())));
            sessionsTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(session->getDateTime())));
            sessionsTable->setItem(row, 4, new QTableWidgetItem(PriceFormatter::format(session->getTicketPrice())));
            sessionsTable->setItem(row, 5, new QTableWidgetItem(QString::number(session->getHall()->getAvailableSeatsCount())));

            // СТАТУС МЕСТ
            QTableWidgetItem* statusItem = new QTableWidgetItem();
            if (session->getHall()->getAvailableSeatsCount() > 10) {
                statusItem->setText("🟢 Много мест");
                statusItem->setForeground(QBrush(QColor("#27ae60")));
            }
            else if (session->getHall()->getAvailableSeatsCount() > 0) {
                statusItem->setText("🟡 Мало мест");
                statusItem->setForeground(QBrush(QColor("#f39c12")));
            }
            else {
                statusItem->setText("🔴 Нет мест");
                statusItem->setForeground(QBrush(QColor("#e74c3c")));
            }
            sessionsTable->setItem(row, 6, statusItem);

            // КНОПКИ ДЕЙСТВИЙ - ПОЛНАЯ ВЕРСИЯ (3 КНОПКИ)
            QWidget* actionsWidget = new QWidget();
            QHBoxLayout* actionsLayout = new QHBoxLayout(actionsWidget);

            // ★★★ ДОБАВЬТЕ СЮДА 2 СТРОКИ:
            actionsLayout->setSpacing(2);
            actionsLayout->setContentsMargins(2, 2, 2, 2);

            QPushButton* bookBtn = new QPushButton();
            QPushButton* editBtn = new QPushButton();
            QPushButton* deleteBtn = new QPushButton();

            // ★★★ УСТАНАВЛИВАЕМ РАЗМЕР КНОПОК ★★★
            bookBtn->setFixedSize(30, 30);
            editBtn->setFixedSize(30, 30);
            deleteBtn->setFixedSize(30, 30);

            // ★★★ УСТАНАВЛИВАЕМ ИКОНКИ ★★★
            bookBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogApplyButton)); // галочка
            editBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon));          // файл
            deleteBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_TrashIcon));       // корзина

            // ★★★ УСТАНАВЛИВАЕМ РАЗМЕР ИКОНОК ★★★
            bookBtn->setIconSize(QSize(20, 20));
            editBtn->setIconSize(QSize(20, 20));
            deleteBtn->setIconSize(QSize(20, 20));

            // ★★★ СТИЛИ ★★★
            bookBtn->setStyleSheet("QPushButton { background-color: #f39c12; border: 1px solid #2c3e50; border-radius: 3px; }");
            editBtn->setStyleSheet("QPushButton { background-color: #3498db; border: 1px solid #2c3e50; border-radius: 3px; }");
            deleteBtn->setStyleSheet("QPushButton { background-color: #e74c3c; border: 1px solid #2c3e50; border-radius: 3px; }");

            // ★★★ ПОДСКАЗКИ ★★★
            bookBtn->setToolTip("Забронировать билет");
            editBtn->setToolTip("Редактировать сеанс");
            deleteBtn->setToolTip("Удалить сеанс");

            // ОБРАБОТЧИК БРОНИРОВАНИЯ
            connect(bookBtn, &QPushButton::clicked, this, [this, session]() {
                onBookFromTableClicked(session);
                });

            // ОБРАБОТЧИК РЕДАКТИРОВАНИЯ
            connect(editBtn, &QPushButton::clicked, this, [this, session]() {
                onEditSessionClicked(session);
                });

            // ОБРАБОТЧИК УДАЛЕНИЯ  
            connect(deleteBtn, &QPushButton::clicked, this, [this, session]() {
                onDeleteSessionClicked(session);
                });

            actionsLayout->addWidget(bookBtn);
            actionsLayout->addWidget(editBtn);
            actionsLayout->addWidget(deleteBtn);
            actionsLayout->setAlignment(Qt::AlignCenter);
            actionsLayout->setContentsMargins(0, 0, 0, 0);

            sessionsTable->setCellWidget(row, 7, actionsWidget);
        }
    }
}

/*void MainWindow::refreshSessionComboBox()
{
    try {
        sessionComboBox->clear();

        // Безопасный перебор сеансов
        for (int i = 0; i < cinemaSystem->sessionsCount; i++) {
            Session* session = cinemaSystem->sessions[i];

            // ПРОВЕРКА НА nullptr И НАЛИЧИЕ СВЯЗАННЫХ ОБЪЕКТОВ
            if (session != nullptr &&
                session->getMovie() != nullptr &&
                session->getHall() != nullptr) {

                QString sessionText = QString::fromStdString(session->getMovie()->getTitle()) +
                    " - Зал " + QString::number(session->getHall()->getHallNumber()) +
                    " - " + QString::fromStdString(session->getDateTime()) +
                    " - " + QString::number(session->getTicketPrice()) + " руб";

                sessionComboBox->addItem(sessionText, session->getSessionId());
            }
        }

        // Если сеансов нет, добавляем заглушку
        if (sessionComboBox->count() == 0) {
            sessionComboBox->addItem("Нет доступных сеансов", -1);
        }
    }
    catch (const std::exception& e) {
        qDebug() << "Ошибка в refreshSessionComboBox:" << e.what();
        sessionComboBox->clear();
        sessionComboBox->addItem("Ошибка загрузки сеансов", -1);
    }
    catch (...) {
        qDebug() << "Неизвестная ошибка в refreshSessionComboBox";
        sessionComboBox->clear();
        sessionComboBox->addItem("Ошибка загрузки сеансов", -1);
    }
}*/

void MainWindow::onAddMovieClicked()
{
    MovieDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Movie* movie = new Movie(
            dialog.getTitle().toStdString(),
            dialog.getGenre().toStdString(),
            dialog.getDuration(),
            dialog.getDirector().toStdString(),
            dialog.getAgeRating()
        );

        cinemaSystem->addMovie(movie);
        refreshMoviesTable();

        QMessageBox::information(this, "Успех",
            QString("Фильм '%1' успешно добавлен!").arg(dialog.getTitle()));
    }
}

void MainWindow::onDeleteMovieClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int movieId = btn->property("movieId").toInt();

    // Находим фильм
    Movie* movieToDelete = nullptr;
    vector<Movie*> movies = cinemaSystem->getAllMovies();
    for (Movie* movie : movies) {
        if (movie->getId() == movieId) {
            movieToDelete = movie;
            break;
        }
    }

    if (!movieToDelete) return;

    // Проверяем, есть ли сеансы с этим фильмом
    bool hasSessions = false;
    for (int i = 0; i < cinemaSystem->sessionsCount; i++) {
        if (cinemaSystem->sessions[i] && cinemaSystem->sessions[i]->getMovie()->getId() == movieId) {
            hasSessions = true;
            break;
        }
    }

    if (hasSessions) {
        QMessageBox::warning(this, "Ошибка", "Нельзя удалить фильм, для которого есть сеансы!");
        return;
    }

    // Удаляем
    if (QMessageBox::question(this, "Подтверждение",
        QString("Удалить фильм '%1'?").arg(QString::fromStdString(movieToDelete->getTitle())))
        == QMessageBox::Yes) {

        // Удаляем из системы
        for (int i = 0; i < cinemaSystem->moviesCount; i++) {
            if (cinemaSystem->movies[i] && cinemaSystem->movies[i]->getId() == movieId) {
                delete cinemaSystem->movies[i];
                cinemaSystem->movies[i] = nullptr;
                break;
            }
        }
        refreshMoviesTable();
        QMessageBox::information(this, "Успех", "Фильм удален!");
    }
}

void MainWindow::onAddHallClicked()
{
    // ПЕРЕДАЕМ cinemaSystem В ДИАЛОГ:
    HallDialog dialog(this, cinemaSystem);
    if (dialog.exec() == QDialog::Accepted) {
        CinemaHall* hall = new CinemaHall(
            dialog.getHallNumber(),
            dialog.getSeatsCount()
        );

        cinemaSystem->addHall(hall);
        refreshHallsTable();

        QMessageBox::information(this, "Успех",
            QString("Зал %1 на %2 мест успешно добавлен!")
            .arg(dialog.getHallNumber())
            .arg(dialog.getSeatsCount()));
    }
}

void MainWindow::onAddSessionClicked()
{
    SessionDialog dialog(this, cinemaSystem);
    if (dialog.exec() == QDialog::Accepted) {
        Movie* movie = dialog.getSelectedMovie();
        CinemaHall* hall = dialog.getSelectedHall();
        QString dateTime = dialog.getDateTime();
        double price = dialog.getPrice();

        if (!movie) {
            QMessageBox::warning(this, "Ошибка", "Фильм не выбран!");
            return;
        }
        if (!hall) {
            QMessageBox::warning(this, "Ошибка", "Зал не выбран!");
            return;
        }

        cinemaSystem->createSession(movie, hall, dateTime.toStdString(), price);
        refreshSessionsTable();

        QMessageBox::information(this, "Успех", "Сеанс добавлен!");
    }
}

void MainWindow::onShowHistoryClicked()
{
    vector<Session*> pastSessions = cinemaSystem->getPastSessions();

    if (pastSessions.empty()) {
        QMessageBox::information(this, "История", "Нет завершенных сеансов.");
        return;
    }

    QString historyText = "📜 ИСТОРИЯ СЕАНСОВ\n\n";

    for (Session* session : pastSessions) {
        if (session && session->getMovie() && session->getHall()) {
            historyText += QString("🎬 %1\n").arg(QString::fromStdString(session->getMovie()->getTitle()));
            historyText += QString("   🕒 %1\n").arg(QString::fromStdString(session->getDateTime()));
            historyText += QString("   🏛️ Зал %1\n").arg(session->getHall()->getHallNumber());
            historyText += QString("   💰 %1 руб\n\n").arg(session->getTicketPrice());
        }
    }

    historyText += QString("Всего завершенных сеансов: %1").arg(pastSessions.size());

    QMessageBox::information(this, "История сеансов", historyText);
}

void MainWindow::onSearchMovieClicked()
{
    currentSearchText = searchMovieEdit->text().trimmed();
    refreshMoviesTable();
}

void MainWindow::onClearSearchClicked()
{
    searchMovieEdit->clear();
    currentSearchText.clear();
    refreshMoviesTable();
}
void MainWindow::onSearchTextChanged(const QString& text)
{
    if (text.isEmpty()) {
        currentSearchText.clear();
        refreshMoviesTable();
    }
}

void MainWindow::onEditMovieClicked(Movie* movie)
{
    if (!movie) return;

    // Проверяем, есть ли сеансы с этим фильмом
    bool hasSessions = false;
    for (int i = 0; i < cinemaSystem->sessionsCount; i++) {
        if (cinemaSystem->sessions[i] &&
            cinemaSystem->sessions[i]->getMovie() &&
            cinemaSystem->sessions[i]->getMovie()->getId() == movie->getId()) {
            hasSessions = true;
            break;
        }
    }

    if (hasSessions) {
        QMessageBox::warning(this, "Ошибка",
            "Нельзя редактировать фильм, для которого есть сеансы!");
        return;
    }

    // Создаем диалог редактирования
    MovieDialog dialog(this);
    dialog.setWindowTitle("✏️ Редактировать фильм");

    // ИСПОЛЬЗУЕМ НОВЫЙ МЕТОД:
    dialog.setMovieData(
        QString::fromStdString(movie->getTitle()),
        QString::fromStdString(movie->getGenre()),
        movie->getDuration(),
        QString::fromStdString(movie->getDirector()),
        movie->getAgeRating()
    );

    if (dialog.exec() == QDialog::Accepted) {
        // Обновляем данные фильма
        movie->setTitle(dialog.getTitle().toStdString());
        movie->setGenre(dialog.getGenre().toStdString());
        movie->setDuration(dialog.getDuration());
        movie->setDirector(dialog.getDirector().toStdString());
        movie->setAgeRating(dialog.getAgeRating());

        refreshMoviesTable();
        QMessageBox::information(this, "Успех", "Фильм отредактирован!");
    }
}

void MainWindow::onEditHallClicked(CinemaHall* hall)
{
    if (!hall) return;

    // Создаем диалог редактирования
    HallDialog dialog(this);
    dialog.setWindowTitle("✏️ Редактировать зал");

    // ИСПОЛЬЗУЕМ НОВЫЕ МЕТОДЫ:
    dialog.setHallNumber(hall->getHallNumber());
    dialog.setSeatsCount(hall->getTotalSeats());

    if (dialog.exec() == QDialog::Accepted) {
        int newHallNumber = dialog.getHallNumber();
        int newSeatsCount = dialog.getSeatsCount();

        // Проверяем, не занят ли номер зала другим залом (если номер изменился)
        if (newHallNumber != hall->getHallNumber()) {
            for (int i = 0; i < cinemaSystem->hallsCount; i++) {
                if (cinemaSystem->halls[i] &&
                    cinemaSystem->halls[i] != hall &&
                    cinemaSystem->halls[i]->getHallNumber() == newHallNumber) {
                    QMessageBox::warning(this, "Ошибка",
                        QString("Зал с номером %1 уже существует!").arg(newHallNumber));
                    return;
                }
            }
        }

        // Создаем новый зал с обновленными данными
        CinemaHall* newHall = new CinemaHall(newHallNumber, newSeatsCount);

        // Копируем состояние мест из старого зала
        for (int seat = 0; seat < std::min(hall->getTotalSeats(), newSeatsCount); seat++) {
            if (!hall->isSeatAvailable(seat)) {
                newHall->bookSeat(seat);
            }
        }

        // Заменяем старый зал в сеансах
        for (int i = 0; i < cinemaSystem->sessionsCount; i++) {
            if (cinemaSystem->sessions[i] && cinemaSystem->sessions[i]->getHall() == hall) {
                cinemaSystem->sessions[i]->setHall(newHall);
            }
        }

        // Заменяем в массиве залов
        for (int i = 0; i < cinemaSystem->hallsCount; i++) {
            if (cinemaSystem->halls[i] == hall) {
                delete cinemaSystem->halls[i]; // Удаляем старый зал
                cinemaSystem->halls[i] = newHall;
                break;
            }
        }

        refreshHallsTable();
        refreshSessionsTable();
        QMessageBox::information(this, "Успех", "Зал отредактирован!");
    }
}

void MainWindow::onDeleteHallClicked(CinemaHall* hall)
{
    if (!hall) return;

    // Проверяем, есть ли сеансы в этом зале
    bool hasSessions = false;
    for (int i = 0; i < cinemaSystem->sessionsCount; i++) {
        if (cinemaSystem->sessions[i] && cinemaSystem->sessions[i]->getHall() == hall) {
            hasSessions = true;
            break;
        }
    }

    if (hasSessions) {
        QMessageBox::warning(this, "Ошибка",
            "Нельзя удалить зал, для которого есть сеансы!");
        return;
    }

    if (QMessageBox::question(this, "Подтверждение удаления",
        QString("Вы уверены, что хотите удалить зал %1?").arg(hall->getHallNumber()))
        == QMessageBox::Yes) {

        // Удаляем зал из системы
        for (int i = 0; i < cinemaSystem->hallsCount; i++) {
            if (cinemaSystem->halls[i] == hall) {
                delete cinemaSystem->halls[i];
                cinemaSystem->halls[i] = nullptr;

                // Сдвигаем массив
                for (int j = i; j < cinemaSystem->hallsCount - 1; j++) {
                    cinemaSystem->halls[j] = cinemaSystem->halls[j + 1];
                }
                cinemaSystem->halls[cinemaSystem->hallsCount - 1] = nullptr;
                cinemaSystem->hallsCount--;
                break;
            }
        }

        refreshHallsTable();
        QMessageBox::information(this, "Успех", "Зал удален!");
    }
}

void MainWindow::onBookFromTableClicked(Session* session)
{
    if (!session) return;

    // Проверяем, что сеанс еще не прошел
    if (DateUtils::isPast(QString::fromStdString(session->getDateTime()))) {
        QMessageBox::warning(this, "Ошибка", "Нельзя бронировать билет на прошедший сеанс!");
        return;
    }

    // Создаем упрощенный диалог бронирования
    QDialog bookDialog(this);
    bookDialog.setWindowTitle("🎫 Бронирование билета");
    bookDialog.setModal(true);
    bookDialog.setMinimumWidth(400);

    QVBoxLayout* layout = new QVBoxLayout(&bookDialog);

    // Информация о сеансе
    QLabel* infoLabel = new QLabel(
        QString("🎬 %1\n🏛️ Зал %2\n🕒 %3\n💰 %4 руб")
        .arg(QString::fromStdString(session->getMovie()->getTitle()))
        .arg(session->getHall()->getHallNumber())
        .arg(QString::fromStdString(session->getDateTime()))
        .arg(session->getTicketPrice())
    );
    infoLabel->setStyleSheet("font-weight: bold; padding: 10px; background-color: #f8f9fa; border-radius: 5px;");
    layout->addWidget(infoLabel);

    QFormLayout* formLayout = new QFormLayout();

    QSpinBox* seatSpinBox = new QSpinBox(&bookDialog);  // ← ДОБАВЬ &bookDialog как parent
    seatSpinBox->setRange(1, session->getHall()->getTotalSeats());
    seatSpinBox->setValue(1);

    QPushButton* selectSeatButton = new QPushButton("🗺️ Выбрать место на карте");
    selectSeatButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; padding: 5px; }");

    QHBoxLayout* seatLayout = new QHBoxLayout();
    seatLayout->addWidget(seatSpinBox);
    seatLayout->addWidget(selectSeatButton);

    QLineEdit* customerNameEdit = new QLineEdit();
    QLineEdit* customerEmailEdit = new QLineEdit();

    formLayout->addRow("Место:", seatLayout);
    formLayout->addRow("Имя:", customerNameEdit);
    formLayout->addRow("Email:", customerEmailEdit);

    QLabel* validationLabel = new QLabel();
    validationLabel->setStyleSheet("color: red;");
    validationLabel->setWordWrap(true);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Ok)->setText("🎫 Забронировать");

    layout->addLayout(formLayout);
    layout->addWidget(validationLabel);
    layout->addWidget(buttonBox);

    // Подключение кнопки выбора места
    connect(selectSeatButton, &QPushButton::clicked, [&]() {
        HallMapDialog mapDialog(&bookDialog, session, seatSpinBox->value());
        if (mapDialog.exec() == QDialog::Accepted) {
            seatSpinBox->setValue(mapDialog.getSelectedSeat());
        }
        });

    // Валидация
    auto validateInputs = [&]() {
        bool isValid = !customerNameEdit->text().isEmpty() &&
            !customerEmailEdit->text().isEmpty() &&
            EmailValidator::isValid(customerEmailEdit->text());

        if (isValid && !session->isSeatAvailable(seatSpinBox->value() - 1)) {
            isValid = false;
            validationLabel->setText("❌ Место уже занято!");
        }
        else {
            validationLabel->clear();
        }

        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(isValid);
        };

    connect(customerNameEdit, &QLineEdit::textChanged, validateInputs);
    connect(customerEmailEdit, &QLineEdit::textChanged, validateInputs);
    connect(seatSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), validateInputs);

    connect(buttonBox, &QDialogButtonBox::accepted, &bookDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &bookDialog, &QDialog::reject);

    validateInputs();

    if (bookDialog.exec() == QDialog::Accepted) {
        // Процесс бронирования
        Customer* customer = cinemaSystem->registerCustomer(
            customerNameEdit->text().toStdString(),
            customerEmailEdit->text().toStdString(),
            "0000000000");

        // ★★★ ДОБАВЬТЕ РАСЧЕТ ЦЕНЫ ПЕРЕД СООБЩЕНИЕМ ★★★
        double finalPrice = session->getTicketPrice();
        int seatNumber = seatSpinBox->value() - 1;

        // Проверяем VIP место (первые 2 ряда по 10 мест)
        if (seatNumber < 20) { // 2 ряда * 10 мест = 20 мест
            finalPrice *= 1.2;
        }

        if (!customer) {
            QMessageBox::warning(this, "Ошибка", "Не удалось зарегистрировать клиента!");
            return;
        }

        Booking* booking = cinemaSystem->createBooking(customer);
        if (!booking) {
            QMessageBox::warning(this, "Ошибка", "Не удалось создать бронирование!");
            return;
        }

        Ticket* ticket = new Ticket(session, customer, seatSpinBox->value() - 1);
        booking->addTicket(ticket);
        booking->confirmBooking();

        QMessageBox::information(this, "Успех",
            QString("Билет забронирован!\nМесто: %1\n💰 Итоговая цена: %2 руб")
            .arg(seatSpinBox->value())
            .arg(finalPrice));
    }
}

void MainWindow::onEditSessionClicked(Session* session)
{
    if (!session) return;

    // ★★★ ПРОВЕРКА НА ПРОШЕДШИЙ СЕАНС ★★★
    if (DateUtils::isPast(QString::fromStdString(session->getDateTime()))) {
        QMessageBox::warning(this, "Ошибка",
            "Нельзя редактировать прошедший сеанс!");
        return;
    }

    // Проверяем, есть ли проданные билеты
    if (session->getHall()->getAvailableSeatsCount() < session->getHall()->getTotalSeats()) {
        QMessageBox::warning(this, "Ошибка",
            "Нельзя редактировать сеанс с проданными билетами!");
        return;
    }

    // Сохраняем старые данные для отката
    Movie* oldMovie = session->getMovie();
    CinemaHall* oldHall = session->getHall();
    string oldDateTime = session->getDateTime();
    double oldPrice = session->getTicketPrice();

    // ★★★ ИСПРАВЛЯЕМ ЭТУ СТРОКУ - ДОБАВЛЯЕМ session ★★★
    SessionDialog dialog(this, cinemaSystem, session);  // ← ДОБАВИЛИ session
    dialog.setWindowTitle("✏️ Редактировать сеанс");
    dialog.setSessionData(session);  // Заполняем текущими данными

    if (dialog.exec() == QDialog::Accepted) {
        try {
            // Получаем новые данные
            Movie* newMovie = dialog.getSelectedMovie();
            CinemaHall* newHall = dialog.getSelectedHall();
            QString newDateTime = dialog.getDateTime();
            double newPrice = dialog.getPrice();

            if (!newMovie || !newHall) {
                throw ValidationException("Не выбраны фильм или зал");
            }

            // ★★★ ПРОВЕРКА ЧТО НОВОЕ ВРЕМЯ НЕ ПРОШЕДШЕЕ ★★★
            if (DateUtils::isPast(newDateTime)) {
                throw BusinessLogicException("Нельзя перенести сеанс на прошедшее время!");
            }

            // Обновляем данные сеанса
            session->setMovie(newMovie);
            session->setHall(newHall);
            session->setDateTime(newDateTime.toStdString());
            session->setTicketPrice(newPrice);

            refreshSessionsTable();
            QMessageBox::information(this, "Успех", "Сеанс отредактирован!");

        }
        catch (const CinemaException& e) {
            // В случае ошибки восстанавливаем старые данные
            session->setMovie(oldMovie);
            session->setHall(oldHall);
            session->setDateTime(oldDateTime);
            session->setTicketPrice(oldPrice);

            QMessageBox::warning(this, "Ошибка", e.what());
        }
    }
}

void MainWindow::onDeleteSessionClicked(Session* session)
{
    if (!session) return;

    // Проверяем, есть ли проданные билеты
    if (session->getHall()->getAvailableSeatsCount() < session->getHall()->getTotalSeats()) {
        QMessageBox::warning(this, "Ошибка",
            "Нельзя удалить сеанс с проданными билетами!");
        return;
    }

    if (QMessageBox::question(this, "Подтверждение удаления",
        QString("Вы уверены, что хотите удалить сеанс?\n\n%1 - %2")
        .arg(QString::fromStdString(session->getMovie()->getTitle()))
        .arg(QString::fromStdString(session->getDateTime())))
        == QMessageBox::Yes) {

        // Удаляем сеанс из системы
        for (int i = 0; i < cinemaSystem->sessionsCount; i++) {
            if (cinemaSystem->sessions[i] == session) {
                delete cinemaSystem->sessions[i];
                cinemaSystem->sessions[i] = nullptr;

                // Сдвигаем массив
                for (int j = i; j < cinemaSystem->sessionsCount - 1; j++) {
                    cinemaSystem->sessions[j] = cinemaSystem->sessions[j + 1];
                }
                cinemaSystem->sessions[cinemaSystem->sessionsCount - 1] = nullptr;
                cinemaSystem->sessionsCount--;
                break;
            }
        }

        refreshSessionsTable();
        QMessageBox::information(this, "Успех", "Сеанс удален!");
    }
}

void MainWindow::onSessionSearchTypeChanged(int index)
{
    QString searchType = sessionSearchComboBox->currentText();

    // ★★★ ПЕРЕКЛЮЧАЕМ МЕЖДУ ТЕКСТОВЫМ ПОЛЕМ И КАЛЕНДАРЕМ ★★★
    if (searchType == "Дата") {
        sessionSearchEdit->setVisible(false);
        sessionSearchDateEdit->setVisible(true);
        sessionSearchDateEdit->setFocus();
    }
    else {
        sessionSearchEdit->setVisible(true);
        sessionSearchDateEdit->setVisible(false);
        sessionSearchEdit->setFocus();
    }

    // Очищаем оба поля при переключении типа поиска
    sessionSearchEdit->clear();
    refreshSessionsTable();
}