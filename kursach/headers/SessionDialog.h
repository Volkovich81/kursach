#ifndef SESSIONDIALOG_H
#define SESSIONDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QDateTime>
#include <QCalendarWidget>
#include <QDateEdit>
#include <QTimeEdit>
#include "CinemaSystem.h"

class SessionDialog : public QDialog
{
    Q_OBJECT

public:
    SessionDialog(QWidget* parent = nullptr, CinemaSystem* system = nullptr, Session* editingSession = nullptr); // ★★★ ДОБАВИЛИ ПАРАМЕТР ★★★

    Movie* getSelectedMovie() const { return selectedMovie; }
    CinemaHall* getSelectedHall() const { return selectedHall; }
    QString getDateTime() const; // ← ИЗМЕНИ - убрать { return dateTimeEdit->text(); }
    double getPrice() const { return priceSpinBox->value(); }
    void setSessionData(Session* session);

private slots:
    void validateInputs();

private:
    CinemaSystem* cinemaSystem;
    Session* currentEditingSession;

    QComboBox* movieComboBox;
    QComboBox* hallComboBox;
    QDoubleSpinBox* priceSpinBox;
    QLabel* validationLabel;
    QDialogButtonBox* buttonBox;
    QDateEdit* dateEdit;
    QTimeEdit* timeEdit;

    Movie* selectedMovie;
    CinemaHall* selectedHall;

    void setupUI();
    void populateMovies();
    void populateHalls();
};

#endif