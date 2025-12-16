#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QDateTimeEdit>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QFormLayout>
#include <QDateEdit>
#include "CinemaSystem.h"
#include "SessionDialog.h"
#include "HallMapDialog.h"
#include <QStatusBar>
#include <QApplication>
#include <QScreen>
#include <QGuiApplication>

class MainWindow : public QMainWindow
{
    /*Q_OBJECT*/

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onAddMovieClicked();
    void onDeleteMovieClicked();

    void onAddHallClicked();
    void onDeleteHallClicked();

    void onAddSessionClicked();
    void onDeleteSessionClicked();

    void onShowHallMapClicked();

    void onSearchTextChanged(const QString& text);
    void onShowHistoryClicked();  // в private slots
    void onEditMovieClicked();

    void onSearchMovieClicked();
    void onClearSearchClicked();
    void onEditMovieClicked(Movie* movie);  // ƒќЅј¬№ Ё“” —“–ќ ”
    void onEditHallClicked(CinemaHall* hall);
    void onDeleteHallClicked(CinemaHall* hall);
    void onBookFromTableClicked(Session* session);
    void onEditSessionClicked(Session* session);
    void onDeleteSessionClicked(Session* session);
    void onSessionSearchTypeChanged(int index);

private:
    CinemaSystem* cinemaSystem;

    QTabWidget* tabWidget;

    QTableWidget* moviesTable;
    QPushButton* addMovieButton;
    QLineEdit* searchMovieEdit;
    QComboBox* searchTypeComboBox;

    QTableWidget* hallsTable;
    QPushButton* addHallButton;

    QTableWidget* sessionsTable;
    QPushButton* addSessionButton;
    QPushButton* historyButton;  // в приватные переменные
    QLineEdit* sessionSearchEdit;      // ƒќЅј¬№ - дл€ поиска сеансов
    QComboBox* sessionSearchComboBox;  // ƒќЅј¬№ - тип поиска сеансов
    QComboBox* sessionFilterComboBox;  // ƒќЅј¬№ - фильтр активные/неактивные

    QString currentSearchText;
    QString currentSearchType;

    QDateEdit* sessionSearchDateEdit;
    
    void setupUI();
    void setupConnections();
    void refreshAll();
    void refreshMoviesTable();
    void refreshHallsTable();
    void refreshSessionsTable();
    void refreshSessionComboBox();
};

#endif
