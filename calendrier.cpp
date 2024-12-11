
#include "fournisseur.h"

// Implémentation de la méthode retrieveAllCalanderDataAsList
QList<QMap<QString, QString>> fournisseur::retrieveAllCalanderDataAsList(const QString& tableName, const QDate& date)
{
    QList<QMap<QString, QString>> dataList;
    QString dateString = date.toString("yyyy-MM-dd");

    QSqlQuery query;
    QString queryString = QString("SELECT NOM_CL, CIN_CL, PRENOM_CL, TELEPHONE_CL, SEXE_CL, RES_DATE_CL, NB_RES_CL FROM %1 WHERE RES_DATE_CL = '%2'")
                              .arg(tableName, dateString);

    if (!query.exec(queryString)) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return dataList;
    }

    while (query.next()) {
        QMap<QString, QString> rowData;
        QSqlRecord record = query.record();

        for (int i = 0; i < record.count(); ++i) {
            rowData[record.fieldName(i).toLower()] = query.value(i).toString();
        }

        dataList.append(rowData);
    }

    qDebug() << "Data retrieved successfully!";
    return dataList;
}

// Implémentation de la méthode retrieveReservationDates
QList<QDate> fournisseur::retrieveReservationDates(const QString& tableName)
{
    QList<QDate> reservationDates;

    QSqlQuery query;
    QString queryString = QString("SELECT DISTINCT RES_DATE_CL FROM %1").arg(tableName);

    if (!query.exec(queryString)) {
        qDebug() << "Error executing query for reservation dates:" << query.lastError().text();
        return reservationDates;
    }

    while (query.next()) {
        reservationDates.append(query.value(0).toDate());
    }

    qDebug() << "Reservation dates retrieved successfully!";
    return reservationDates;
}
