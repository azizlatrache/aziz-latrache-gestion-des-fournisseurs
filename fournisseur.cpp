#include "fournisseur.h"

#include <QSqlError>

fournisseur::fournisseur()
{

    nom = "";
    prenom = "";
    type = "";
    id = 0;
    numero = 0;

}
fournisseur::fournisseur(QString nom,QString prenom,QString type,int id,int numero)
{
    this->nom = nom;
    this->prenom = prenom;
    this->type = type;
    this->id = id;
    this->numero = numero;

}

bool fournisseur::ajouter()
{
    // Assurez-vous que la connexion à la base de données est valide
    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "Erreur: la connexion à la base de données n'est pas ouverte.";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO fournisseur (NOM, PRENOM, TYPE, ID, NUMERO) "
                  "VALUES (:nom, :prenom, :type, :id, :numero)");

    // Liaison des valeurs
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":type", type);
    query.bindValue(":id", id);
    query.bindValue(":numero", numero);

    // Exécution de la requête et gestion des erreurs
    if (!query.exec()) {
        // Si l'exécution échoue, affichez l'erreur
        qDebug() << "Erreur d'ajout dans la base de données: " << query.lastError().text();
        return false;
    }

    return true;
}

QSqlQueryModel *fournisseur::afficher()
{
    QSqlQueryModel *model=new QSqlQueryModel();
    model->setQuery("select *from fournisseur");
    model->setHeaderData(0, Qt::Horizontal,QObject::tr("nom"));
    model->setHeaderData(1, Qt::Horizontal,QObject::tr("prenom"));
    model->setHeaderData(2, Qt::Horizontal,QObject::tr("type"));
    model->setHeaderData(6, Qt::Horizontal,QObject::tr("id"));
    model->setHeaderData(7, Qt::Horizontal,QObject::tr("numero"));


    return model;
}

bool fournisseur::supprimer(QString cc)
{
    QSqlQuery query;
    query.prepare("DELETE FROM fournisseur WHERE id = :id");
    query.bindValue(":id", cc);
    return query.exec();
}


bool fournisseur::modifier(QString nom, QString prenom, QString type, int id, int numero)
{
    QSqlQuery qry;
    qry.prepare("UPDATE fournisseur SET prenom = :prenom, type = :type"
                "id = :id, numero = :numero WHERE nom = :nom");

    qry.bindValue(":nom", nom);
    qry.bindValue(":prenom", prenom);
    qry.bindValue(":type", type);
    qry.bindValue(":id", id);
    qry.bindValue(":numero", numero);


    return qry.exec();
}

void fournisseur::rechercher(QTableView *table, QString cas)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT * FROM fournisseur WHERE id= :id");
    query.bindValue(":id", cas);

    if (query.exec())
    {
        model->setQuery(query);
        table->setModel(model);
        table->show();
    }
    else
    {
        qDebug() << "Erreur lors de l'exécution de la requête :" << query.lastError().text();
    }
}


#include <QDate>
#include <QMessageBox>
#include <QSqlQuery>





#include <QSqlQuery>

#include <QVariant>


int fournisseur::getTotalfournisseurs()
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM fournisseur");

    if (query.exec()) {
        if (query.next()) {
            return query.value(0).toInt();
        }
    } else {
        qDebug() << "Error retrieving total fournisseurs: " << query.lastError().text();
    }

    return 0;  // Return 0 in case of an error
}

