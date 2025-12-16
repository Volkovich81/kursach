#ifndef MOVIEDIALOG_H
#define MOVIEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QMessageBox>

class MovieDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MovieDialog(QWidget* parent = nullptr);
    ~MovieDialog() = default;

    QString getTitle() const { return titleEdit->text(); }
    QString getGenre() const { return genreComboBox->currentText(); }
    int getDuration() const { return durationSpinBox->value(); }
    QString getDirector() const { return directorEdit->text(); }
    int getAgeRating() const { return ageRatingComboBox->currentText().replace("+", "").toInt(); }

    // днаюбэ щрнр лернд:
    void setMovieData(const QString& title, const QString& genre, int duration,
        const QString& director, int ageRating);

private slots:
    void validateInputs();

private:
    void setupUI();

    // нярюбкъел йюй еярэ - private:
    QLineEdit* titleEdit;
    QComboBox* genreComboBox;
    QSpinBox* durationSpinBox;
    QLineEdit* directorEdit;
    QComboBox* ageRatingComboBox;
    QDialogButtonBox* buttonBox;
};
#endif
