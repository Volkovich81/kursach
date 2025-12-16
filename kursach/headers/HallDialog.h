#ifndef HALLDIALOG_H
#define HALLDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>

class CinemaSystem;

class HallDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HallDialog(QWidget* parent = nullptr, CinemaSystem* system = nullptr);
    ~HallDialog() = default;

    int getHallNumber() const { return hallNumberSpinBox->value(); }
    int getSeatsCount() const { return seatsSpinBox->value(); }

    void setHallNumber(int number) { hallNumberSpinBox->setValue(number); }
    void setSeatsCount(int count) { seatsSpinBox->setValue(count); }

private slots:
    void validateInputs();

private:
    void setupUI();

    QSpinBox* hallNumberSpinBox;
    QSpinBox* seatsSpinBox;
    QDialogButtonBox* buttonBox;
    QLabel* validationLabel;

    CinemaSystem* cinemaSystem;
};

#endif // HALLDIALOG_H