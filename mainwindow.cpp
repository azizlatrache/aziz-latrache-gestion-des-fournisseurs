
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fournisseur.h"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QValueAxis>
#include <QtGui/QPixmap>
#include <QPdfWriter>
#include <QPainter>
#include <QSqlTableModel>
#include <QSqlError>
#include <QTextCharFormat>
#include <QBrush>
#include <QColor>
#include <QSqlQuery>
#include <QMessageBox>





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    calendar = ui->calendarWidget;






    QSqlTableModel *model = new QSqlTableModel(this);



    // Connexion des boutons aux fonctions appropriées
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);
    connect(ui->afficher, &QPushButton::clicked, this, &MainWindow::on_afficher_clicked);
    connect(ui->supp, &QPushButton::clicked, this, &MainWindow::on_supp_clicked);
    connect(ui->modifier, &QPushButton::clicked, this, &MainWindow::on_modifier_clicked);
    connect(ui->rechercher, &QPushButton::clicked, this, &MainWindow::on_rechercher_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::on_pushButton_2_clicked);
    connect(ui->pushButton_Statistics, &QPushButton::clicked, this, &MainWindow::showStatistics);



    model->setTable("tableView");  // Replace with your actual table name
    model->select();  // Fetch data from the table

    // Set the model for the QTableView
    ui->tableView->setModel(model);
    // Lancer la connexion à Arduino
    int ret = A.connect_arduino();
    switch (ret) {
    case 0:
        qDebug() << "Arduino est disponible et connecté au port :" << A.getarduino_port_name();
        break;
    case 1:
        qDebug() << "Arduino est disponible mais pas connecté au port :" << A.getarduino_port_name();
        break;
    case -1:
        qDebug() << "Arduino n'est pas disponible";
        break;
    }

    // Connecter le signal de réception de données Arduino au slot update_label
    QObject::connect(A.getserial(), SIGNAL(readyRead()), this, SLOT(update_label()));
}



MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_pushButton_clicked()
{
    int id = ui->lineEdit_8->text().toInt();
    QString nom = ui->lineEdit_2->text();
    QString prenom = ui->lineEdit_6->text();
    QString type = ui->lineEdit_5->currentText();

    int numero = ui->lineEdit_9->text().toInt();

    fournisseur r(nom, prenom, type, id, numero);

    bool test = r.ajouter();
    if (test)
    {
        QMessageBox::information(nullptr, QObject::tr("ok"), QObject::tr("ajout effectué"), QMessageBox::Cancel);

        ui->tableView->setModel(tfournisseur.afficher()); // Actualisation de la vue
        // Nettoyage des champs
        ui->lineEdit_8->clear();
        ui->lineEdit_2->clear();
        ui->lineEdit_6->clear();
        ui->lineEdit_5->setCurrentIndex(-1);

        ui->lineEdit_9->clear();
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("not ok"), QObject::tr("ajout non effectué"), QMessageBox::Cancel);
    }
}

void MainWindow::on_afficher_clicked()
{
    ui->tableView->setModel(tfournisseur.afficher()); // Actualisation de la vue
}

void MainWindow::on_supp_clicked()
{
    QString nom = ui->lineEdit_7->text();
    QMessageBox msgbox;
    msgbox.setWindowTitle("supprimer");
    msgbox.setText("Voulez-vous supprimer cette fournisseur ?");
    msgbox.setStandardButtons(QMessageBox::Yes);
    msgbox.addButton(QMessageBox::No);

    if (msgbox.exec() == QMessageBox::Yes)
    {
        bool test = tfournisseur.supprimer(nom);
        if (test)
        {
            ui->tableView->setModel(tfournisseur.afficher()); // Actualisation de la vue
            ui->lineEdit_7->clear();
        }
        else
        {
            QMessageBox::critical(nullptr, QObject::tr("supprimer un fournisseur"), QObject::tr("Erreur.\nCliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        }
    }
    else
    {
        ui->tableView->setModel(tfournisseur.afficher()); // Actualisation de la vue si l'utilisateur annule
    }
}

void MainWindow::on_modifier_clicked()
{
    QString nom = ui->lineEdit_17->text();
    QString prenom = ui->lineEdit_16->text();
    QString type = ui->lineEdit_15->currentText();
    int id = ui->lineEdit_12->text().toInt();
    int numero = ui->lineEdit_14->text().toInt();
    // Vérification que l'ID est fourni avant la modification
    if (nom.isEmpty()) {
        QMessageBox::information(nullptr, QObject::tr("No ID Entered"), QObject::tr("Please enter an ID to modify."), QMessageBox::Ok);
        return;
    }

    bool test = tfournisseur.modifier(nom, prenom, type, id, numero);

    if (test) {
        ui->tableView->setModel(tfournisseur.afficher()); // Actualisation de la vue
        // Nettoyage des champs
        ui->lineEdit_17->clear();
        ui->lineEdit_16->clear();
        ui->lineEdit_15->setCurrentIndex(-1);
        ui->lineEdit_12->clear();
        ui->lineEdit_14->clear();

        QMessageBox::information(nullptr, QObject::tr("Modifier une fournisseur"), QObject::tr("Modification réussie."), QMessageBox::Ok);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Modifier une fournisseur"), QObject::tr("Erreur lors de la modification.\nCliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
}

void MainWindow::on_rechercher_clicked()
{
    QString nom = "1";
    QString prenom = "";
    QString type = "";
    int id = 0;
    int numero = 0;


    fournisseur R(nom, prenom, type, id, numero);
    QString cas = ui->lineEdit_19->text();
    R.rechercher(ui->tableView_2, cas);

    if (ui->lineEdit_19->text().isEmpty())
    {
        ui->tableView_2->setModel(R.afficher());
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("type"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("id"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("numero"));


    ui->tableView_2->setModel(model);

    QSqlQuery query;

    QString orderByClause;

    // Vérification de la colonne de tri choisie
    if (ui->comboBox->currentText() == "id")
        orderByClause = "ORDER BY id ASC";
    else if (ui->comboBox->currentText() == "montant")
        orderByClause = "ORDER BY montant ASC";

    query.prepare("SELECT * FROM fournisseur " + orderByClause);

    if (query.exec() && query.next())
    {
        model->setQuery(query);
        ui->tableView_2->setModel(model);
    }
}

#include <QDate>
#include <QMessageBox>
#include <QSqlQuery>

void MainWindow::checkUpcomingArrivals()
{
    QDate currentDate = QDate::currentDate();
    qDebug() << "Current date:" << currentDate.toString("yyyy-MM-dd");

    QSqlQuery query;
    query.prepare("SELECT nom, date_arrive FROM fournisseur");
    query.exec();

    bool foundUpcomingArrival = false;

    while (query.next()) {
        QString nom = query.value(0).toString();
        QString date_arrive_str = query.value(1).toString();
        QDate dateArrive = QDate::fromString(date_arrive_str, "dd-MM-yyyy");

        qDebug() << "Checking nom:" << nom << "Arrival date:" << dateArrive.toString("yyyy-MM-dd");

        if (currentDate.daysTo(dateArrive) <= 3 && currentDate.daysTo(dateArrive) >= 0) {
            // If the arrival date is within the next 3 days
            QString message = QString("Client with nom %1 has an arrival within 3 days: %2")
                                  .arg(nom)
                                  .arg(dateArrive.toString("yyyy-MM-dd"));
            QMessageBox::information(this, "Upcoming Arrival", message);
            foundUpcomingArrival = true;
            break;  // If you find one upcoming arrival, stop further checks.
        }
    }

    if (!foundUpcomingArrival) {
        QMessageBox::information(this, "No Upcoming Arrivals", "No upcoming arrivals within 3 days.");
    }
}



















#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtGui/QPixmap>


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fournisseur.h"
#include <QMessageBox>

void MainWindow::showStatistics()
{
    fournisseur r;

    int totalfournisseurs = r.getTotalfournisseurs();

    // Display the results in a message box (you can customize this as needed)
    QString statsMessage = QString("Total fournisseurs: %1\n")
                               .arg(totalfournisseurs);


    QMessageBox::information(this, "Statistics", statsMessage);
}

#include <QPdfWriter>
#include <QPainter>
#include <QTableView>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QPageSize>  // Include to fix A4 page size issue

void MainWindow::exportToPDF()
{
    QString filePath = "C:\\Users\\DELL\\Desktop\\youssef msabah\\Atelier_Connexion\\fournisseur.pdf";
    QPdfWriter writer(filePath);

    // Set A4 page size
    writer.setPageSize(QPageSize::A4);
    QPainter painter(&writer);
    if (!painter.isActive()) {
        QMessageBox::critical(this, "Error", "Failed to start painting the PDF.");
        return;
    }

    // Set the font and pen for the painter
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 10));

    // Write title on top of the PDF
    painter.drawText(50, 50, "fournisseurs List");

    // Get the model from the QTableView
    QTableView *tableView = ui->tableView;
    QAbstractItemModel *model = tableView->model();  // Get the model

    if (!model) {
        QMessageBox::critical(this, "Error", "No model set for the table view.");
        return;
    }

    // Set starting position for the table data
    int rowHeight = 2000;  // height of each row in the PDF
    int colWidth = 1250;  // width of each column in the PDF
    int startX = 15;    // starting X position for the first column
    int startY = 2000;    // starting Y position for the first row

    // Draw column headers
    for (int col = 0; col < model->columnCount(); ++col) {
        QString header = model->headerData(col, Qt::Horizontal).toString();
        painter.drawText(startX + (col * colWidth), startY, header);
    }

    // Move the Y position down for the table data
    startY += rowHeight + 5;  // Add extra space between header and data

    // Iterate through the rows and columns of the model to draw the data
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            // Retrieve the data for each cell
            QString text = model->data(model->index(row, col)).toString();

            // Draw the text in the PDF with additional spacing
            painter.drawText(startX + (col * colWidth), startY + (row * rowHeight), text);
        }
    }

    // Draw the table borders (optional for better structure)
    for (int row = 0; row <= model->rowCount(); ++row) {
        painter.drawLine(startX, startY + (row * rowHeight), startX + colWidth * model->columnCount(), startY + (row * rowHeight)); // Horizontal lines
    }
    for (int col = 0; col <= model->columnCount(); ++col) {
        painter.drawLine(startX + (col * colWidth), startY, startX + (col * colWidth), startY + (rowHeight * model->rowCount())); // Vertical lines
    }

    painter.end();  // Finalize PDF writing

    // Notify the user that the PDF was successfully created
    QMessageBox::information(this, "Export Complete", "PDF has been successfully created.");
}








void MainWindow::on_pushButton_ExportPDF_clicked()
{
    exportToPDF();  // Call the exportToPDF function when the button is clicked
}





#include <QSqlQuery>
#include <QCalendarWidget>
#include <QDate>
#include <QDebug>
#include <QMessageBox>


#include <QCalendarWidget>
#include <QTextCharFormat>
#include <QBrush>
#include <QColor>

void MainWindow::updateCalendarWithArrivals()
{
    // Create a calendar widget object if not already created
    QCalendarWidget *calendar = new QCalendarWidget(this);
    calendar->setGridVisible(true);

    // List of arrival dates to highlight
    QStringList arrivalDates = {"24/11/2024", "22/11/2024", "23/11/2024", "31/12/1999"};

    // Loop through each arrival date and highlight on the calendar
    for (const QString &dateStr : arrivalDates) {
        // Convert the string to a QDate
        QDate date = QDate::fromString(dateStr, "dd/MM/yyyy");

        // Check if the date is valid
        if (date.isValid()) {
            QTextCharFormat format;
            format.setBackground(QBrush(QColor(0, 255, 0)));  // Highlight with green background

            // Set the text format for the specific date
            calendar->setDateTextFormat(date, format);
        }
    }

    // Show the calendar widget (you might want to adjust its position)
    calendar->show();
}


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

void MainWindow::update_label() {
    // Append received data to the buffer
    dataBuffer += A.read_from_arduino();
    qDebug() << "Raw data received from Arduino:" << dataBuffer;

    // Check if the buffer contains a complete message (ending with '\n')
    int newlineIndex = dataBuffer.indexOf('\n');
    while (newlineIndex != -1) { // Process all complete lines
        // Extract and trim the complete message
        QString completeMessage = dataBuffer.left(newlineIndex).trimmed();
        dataBuffer.remove(0, newlineIndex + 1);

        qDebug() << "Complete message received:" << completeMessage;

        // Process the complete message
        if (completeMessage == "AUTHORIZED") {
            int bytes = A.write_to_arduino("1"); // Send "1" to Arduino
            if (bytes != -1) {
                qDebug() << "Sent '1' to Arduino successfully.";
            } else {
                qDebug() << "Failed to send '1' to Arduino.";
            }
            ui->statusLabel->setText("Carte autorisée !");
        } else if (completeMessage == "DENIED") {
            int bytes = A.write_to_arduino("0"); // Send "0" to Arduino
            if (bytes != -1) {
                qDebug() << "Sent '0' to Arduino successfully.";
            } else {
                qDebug() << "Failed to send '0' to Arduino.";
            }
            ui->statusLabel->setText("Carte refusée !");
        } else {
            qDebug() << "Unknown message received: " << completeMessage;
        }

        // Check for more complete lines in the buffer
        newlineIndex = dataBuffer.indexOf('\n');
    }
}

    void MainWindow::addDataToTableViewfournisseurCalander(const QList<QMap<QString, QString>>& dataList)
    {
        QStandardItemModel *model = new QStandardItemModel(this);
        model->setHorizontalHeaderLabels({"Nom", "Prenom", "type","id","numero"}); // Exemples d'en-têtes

        for (const auto& row : dataList) {
            QList<QStandardItem*> items;
            items << new QStandardItem(row.value("numero_fl"));
            items << new QStandardItem(row.value("nom"));
            model->appendRow(items);
        }

        ui->tableView->setModel(model); // Assurez-vous que ce widget existe
    }

void MainWindow::highlightReservationDates()
{
    fournisseur f;

    QList<QDate> reservationDates = f.retrieveReservationDates("fournisseur");

    // Create a QTextCharFormat for highlighting
    QTextCharFormat format;
    format.setBackground(Qt::yellow);  // Highlight with a yellow background
    format.setForeground(Qt::black);  // Set text color to black
    format.setFontWeight(QFont::Bold);

    // Apply formatting to the calendar widget
    for (const QDate& date : reservationDates) {
        ui->calendarWidget->setDateTextFormat(date, format);
    }
}

void MainWindow::on_fournisseur_calendarWidget_selectionChanged()
{
    fournisseur f;
    // Get the selected date from the calendar widget
    QDate selectedDate = ui->calendarWidget->selectedDate();

    // Display the selected date in the line edit
    ui->lineEdit_7->setText(selectedDate.toString("yyyy-MM-dd"));

    // Retrieve fournisseur data for the selected date
    QList<QMap<QString, QString>> dataList = f.retrieveAllCalanderDataAsList("fournisseur", selectedDate);

    // Populate the table view with the retrieved data
    addDataToTableViewfournisseurCalander(dataList);
}






