#ifndef BOOKINGDIALOG_H
#define BOOKINGDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>
#include "CinemaSystem.h"

class HallMapDialog;
class Session;

class BookingDialog : public QDialog
{
    Q_OBJECT

public:
    BookingDialog(QWidget* parent = nullptr, CinemaSystem* system = nullptr);

    int getSelectedSessionId() const;
    int getSelectedSeat() const;
    QString getCustomerName() const;
    QString getCustomerEmail() const;

private slots:
    void validateInputs();
    void onSessionChanged(int index);
    void onSelectSeatClicked();

private:
    void setupUI();
    void populateSessions();

    CinemaSystem* cinemaSystem;
    QComboBox* sessionComboBox;
    QSpinBox* seatSpinBox;
    QLineEdit* customerNameEdit;
    QLineEdit* customerEmailEdit;
    QLabel* validationLabel;
    QLabel* seatInfoLabel;
    QDialogButtonBox* buttonBox;
    QPushButton* selectSeatButton;
};

#endif