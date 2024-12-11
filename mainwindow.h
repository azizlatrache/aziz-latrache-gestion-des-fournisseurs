#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QCalendarWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>

#include "arduino.h"
#include "fournisseur.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
     void addDataToTableViewfournisseurCalander(const QList<QMap<QString, QString>>& dataList);
     void highlightReservationDates();

private:
    Ui::MainWindow *ui;
    Arduino A;                    // Gestion d'Arduino
    fournisseur tfournisseur;     // Gestion des fournisseurs
    QTimer *alertTimer;           // Timer pour les alertes automatiques
    QString dataBuffer;           // Tampon pour les données Arduino
    QCalendarWidget *calendar;    // Widget pour afficher le calendrier
    bool sortAscending = true;
    QStandardItemModel *model;

    // Méthodes de gestion des fournisseurs
    void on_pushButton_clicked();
    void on_afficher_clicked();
    void on_supp_clicked();
    void on_modifier_clicked();
    void on_rechercher_clicked();
    void on_pushButton_2_clicked();
    void exportToPDF();

private slots:
    // Slots pour Arduino
    void update_label();
    void send_automatic_alert();

    // Slots pour les fournisseurs
    void checkUpcomingArrivals();
    void updateCalendarWithArrivals();
    void showStatistics();
    void on_pushButton_ExportPDF_clicked();
    void on_fournisseur_calendarWidget_selectionChanged();
};

#endif // MAINWINDOW_H
