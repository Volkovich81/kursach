#ifndef HALLMAPDIALOG_H
#define HALLMAPDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include "CinemaSystem.h"

class HallMapDialog : public QDialog
{
    Q_OBJECT

public:
    HallMapDialog(QWidget* parent = nullptr, Session* session = nullptr, int selectedSeat = 0);
    int getSelectedSeat() const { return selectedSeat; }
    double calculateTicketPrice() const;

private slots:
    void onSeatClicked();

private:
    void setupUI();
    void createSeatLayout();

    Session* session;
    int selectedSeat;
    QGridLayout* gridLayout;
    QList<QPushButton*> seatButtons;
    QLabel* infoLabel;
};

#endif