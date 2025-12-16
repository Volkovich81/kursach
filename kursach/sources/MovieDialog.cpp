#include "../headers/MovieDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QPushButton>  // ← ДОБАВИТЬ ЭТУ СТРОКУ
#include <QMessageBox>

MovieDialog::MovieDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUI();
    validateInputs();
}

void MovieDialog::setupUI()
{
    setWindowTitle("➕ Добавить фильм");
    setModal(true);
    setMinimumWidth(400);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QFormLayout* formLayout = new QFormLayout();

    titleEdit = new QLineEdit();
    titleEdit->setPlaceholderText("Введите название фильма");

    genreComboBox = new QComboBox();
    QStringList genres = { "Фантастика", "Боевик", "Драма", "Комедия", "Ужасы",
                         "Мелодрама", "Приключения", "Детектив", "Триллер", "Мультфильм" };
    genreComboBox->addItems(genres);

    durationSpinBox = new QSpinBox();
    durationSpinBox->setRange(60, 300);
    durationSpinBox->setSuffix(" мин");
    durationSpinBox->setValue(120);

    directorEdit = new QLineEdit();
    directorEdit->setPlaceholderText("Введите имя режиссера");

    ageRatingComboBox = new QComboBox();
    ageRatingComboBox->addItems({ "0+", "6+", "12+", "16+", "18+" });

    formLayout->addRow("Название:", titleEdit);
    formLayout->addRow("Жанр:", genreComboBox);
    formLayout->addRow("Продолжительность:", durationSpinBox);
    formLayout->addRow("Режиссер:", directorEdit);
    formLayout->addRow("Возрастной рейтинг:", ageRatingComboBox);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Ok)->setText("Добавить");
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    connect(titleEdit, &QLineEdit::textChanged, this, &MovieDialog::validateInputs);
    connect(directorEdit, &QLineEdit::textChanged, this, &MovieDialog::validateInputs);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void MovieDialog::validateInputs()
{
    bool isValid = !titleEdit->text().isEmpty() && !directorEdit->text().isEmpty();
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(isValid);
}

void MovieDialog::setMovieData(const QString& title, const QString& genre, int duration,
    const QString& director, int ageRating)
{
    titleEdit->setText(title);

    // Устанавливаем жанр
    int genreIndex = genreComboBox->findText(genre);
    if (genreIndex != -1) {
        genreComboBox->setCurrentIndex(genreIndex);
    }

    durationSpinBox->setValue(duration);
    directorEdit->setText(director);

    // Устанавливаем возрастной рейтинг
    QString ageStr = QString::number(ageRating) + "+";
    int ageIndex = ageRatingComboBox->findText(ageStr);
    if (ageIndex != -1) {
        ageRatingComboBox->setCurrentIndex(ageIndex);
    }
}