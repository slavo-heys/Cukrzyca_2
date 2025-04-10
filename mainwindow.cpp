#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDate>
#include <algorithm>


QString fraza, frazaDwa;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // wczytaj dane z pliku poraDnia.txt i wstaw do comboBoxPora
    QFile file("poraDnia.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Nie można otworzyć pliku!";
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        ui->comboBoxPora->addItem(line);
    }
    file.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_wyczyscPola_clicked()
{
    // wyczysc wszystkie pola
    ui->lineEditWaga->clear();
    ui->lineEditPoziomy->clear();
    ui->pokazWyniki->clear();
    ui->lineEditCisnienie->clear();
    ui->lineEditPuls->clear();
    ui->info->clear();
    ui->info->setPlainText("Wszystkie pola zostały wyczyszczone");
    ui->info->setStyleSheet("color: green");
    // ustaw border 1px na green
    ui->info->setStyleSheet("border: 1px solid green");

}


void MainWindow::on_zapiszWyniki_clicked()
{
    // pobierz dane z calendarWidget
    QString data = ui->calendarWidget->selectedDate().toString("yyyy-MM-dd");

    // pobierz dane z comboBoxPora
    QString pora = ui->comboBoxPora->currentText();

    // pobierz dane z lineEditWaga
    QString waga = ui->lineEditWaga->text();
    // jesli waga jest pusta wstaw "none"
    if (waga.isEmpty()) {
        waga = "none";
    }

    // pobierz dane z lineEditPoziomy
    QString poziomy = ui->lineEditPoziomy->text();
    // jesli poziomy jest pusty pokaz komunikat "brak pomiaru cukru" i nie zapisuj
    if (poziomy.isEmpty()) {
        // kolor czcionki red
        ui->info->setStyleSheet("color: red");
        ui->info->setPlainText("Brak pomiaru cukru!\nUzupełnij pole poziomy!\nDane nie zostały zapisane!");
        // ustaw border 1px na red
        ui->info->setStyleSheet("border: 1px solid red");
        // wyczysc wszystkie pola
        ui->lineEditWaga->clear();
        ui->lineEditPoziomy->clear();
        ui->lineEditCisnienie->clear();
        ui->lineEditPuls->clear();
        return;
    }

    // pobierz dane z lineEditCisnienie
    QString cisnienie = ui->lineEditCisnienie->text();
    // jesli cisnienie jest puste wstaw "none"
    if (cisnienie.isEmpty()) {
        cisnienie = "none";
    }

    // pobierz dane z lineEditPuls
    QString puls = ui->lineEditPuls->text();
    // jesli puls jest pusty wstaw "none"
    if (puls.isEmpty()) {
        puls = "none";
    }

    // utworz fraze
    if (cisnienie.isEmpty()) {
        fraza = data + " | " + pora + " | " + "Waga: " + waga + " | Poziomy: " + poziomy + "\n";
    }else{
    fraza = data + " | " + pora + " | " + "Waga: " + waga + " | Poziomy: " + poziomy + "\n";
    frazaDwa = data + " | Ciśnienie: " + cisnienie + " | Puls: " + puls + "\n";
    }

    // sprawdz czy plik poziomy.txt istnieje jesli nie to utworz
    QFile file("poziomy.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Nie można otworzyć pliku!";
        return;
    }
    QTextStream out(&file);

    // zapisz dane do pliku
    if (cisnienie.isEmpty()) {
        out << fraza;
    }else{
        out << fraza;
        out << frazaDwa;
    }
    out << "-------------------------------------------------------------------------------------\n";
    file.close();
    // wyczysc wszystkie pola
    ui->lineEditWaga->clear();
    ui->lineEditPoziomy->clear();
    ui->pokazWyniki->clear();
    ui->lineEditCisnienie->clear();
    ui->lineEditPuls->clear();
}


void MainWindow::on_pushButton_3_clicked()
{
    // odczytaj dane z pliku poziomy.txt i wstaw do pokazWyniki od tylu
    QFile file("poziomy.txt");
    if (!file.exists()) {
        ui->pokazWyniki->setPlainText("Brak danych");
        return;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Nie można otworzyć pliku!";
        return;
    }

    QTextStream in(&file);
    QStringList lines;
    while (!in.atEnd()) {
        QString line = in.readLine();
        lines.append(line);
    }

    file.close();

    //  odwróć kolejność linii
    std::reverse(lines.begin(), lines.end());

    // wstaw do pokazWyniki
    QString allLines;
    for (const QString &line : lines) {
        allLines += line + "\n";
    }
    ui->pokazWyniki->setPlainText(allLines);
}

