#ifndef FOURNISSEUR_H
#define FOURNISSEUR_H


#include <QDialog>
#include <QString>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QtDebug>
#include <QSqlQueryModel>
#include <QTableView>
#include <QStandardItemModel>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
class fournisseur
{
    QString nom,prenom,type;
    int id,numero;
public:
    fournisseur();
    fournisseur(QString,QString,QString,int,int);


    QString getnom() const { return nom; }
    QString getprenom() const { return prenom; }
    QString gettype() const { return type; }
    int getid() const { return id; }
    int getnumero() const { return numero; }
    QList<QMap<QString, QString>> retrieveAllCalanderDataAsList(const QString& tableName, const QDate& date);
    QList<QDate> retrieveReservationDates(const QString& tableName);



    void setId(const int &id) { this->id = id; }
    void setNumero(const int &num) { this->numero = num; }
    void setnom(const QString &n) { this->nom = n; }
    void setprenom(const QString &p) { this->prenom = p; }
    void settype(const QString &t) { this->type = t; }





    bool ajouter();
    QSqlQueryModel *afficher();
    bool supprimer(QString);
    bool modifier(QString,QString,QString,int,int);
    void rechercher(QTableView *table,QString cas );
    void on_pb_PDF_clicked();
    void checkUpcomingfournisseurs();


    int getTotalfournisseurs();
    float getTotalRevenue();



};

#endif // fournisseur_H
