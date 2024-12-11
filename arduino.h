#ifndef ARDUINO_H
#define ARDUINO_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

class Arduino {
public:
    Arduino(); // Constructeur par défaut
    QString getarduino_port_name(); // Retourne le nom du port
    QSerialPort* getserial(); // Retourne le pointeur vers le port série
    int connect_arduino(); // Connecte l'Arduino
    int close_arduino(); // Déconnecte l'Arduino
    QByteArray read_from_arduino(); // Lecture des données reçues
    int write_to_arduino(QByteArray); // Écriture des données vers Arduino

private:
    QSerialPort *serial; // Pointeur vers l'objet QSerialPort
    QString arduino_port_name; // Nom du port utilisé pour communiquer
    QByteArray data; // Données reçues depuis l'Arduino
    bool arduino_is_available; // Indique si l'Arduino est disponible
    static const quint16 arduino_uno_vendor_id = 9025; // Identifiant du fabricant
    static const quint16 arduino_uno_producy_id = 67; // Identifiant du produit
};

#endif // ARDUINO_H
