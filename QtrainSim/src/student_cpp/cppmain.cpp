#include "ctrain_handler.h"
#include "locomotive.h"
#include "locoworker.h"

#include <QList>
#include <QThread>

//Creation d'une locomotive
static Locomotive locomotive1;
static Locomotive locomotive2;

//Arret d'urgence
void emergency_stop()
{
    locomotive1.arreter();
    locomotive2.arreter();
    afficher_message("\nSTOP!");
}

//Fonction principale
int cmain()
{
    afficher_message("Hit play to start the simulation...");

    //Choix de la maquette
    selection_maquette(MAQUETTE_B);

    QList<Troncon> map;
    map.append(new Troncon(1, 2, 25));
    map.append(new Troncon(1, 3, 19, 22));
    map.append(new Troncon(2, 7, 8));
    map.append(new Troncon(3, 4, 5, 6));
    map.append(new Troncon(4, 9));
    map.append(new Troncon(5, 24));
    map.append(new Troncon(6, 11));
    map.append(new Troncon(7, 10, 13, 15));
    map.append(new Troncon(8, 20, 25, 26));
    map.append(new Troncon(9, 10, 11, 12));
    map.append(new Troncon(12, 17));
    map.append(new Troncon(13, 14, 26));
    map.append(new Troncon(14, 19, 20));
    map.append(new Troncon(15, 16, 17, 18));
    map.append(new Troncon(16, 21));
    map.append(new Troncon(18, 23));
    map.append(new Troncon(21, 22, 23, 24));

    //Initialisation d'un parcours
    QList<int> parcours1;
    QList<int> parcours2;
    parcours1 << 26 << 25 << 1 << 19 << 20 << 8 << 7 << 13;
    parcours2 << 13 << 7 << 2 << 1 << 19 << 14;

    //Initialisation des aiguillages
    diriger_aiguillage(5, DEVIE, 0);
    diriger_aiguillage(6, DEVIE, 0);
    diriger_aiguillage(17, TOUT_DROIT, 0);
    diriger_aiguillage(1, DEVIE, 0);
    diriger_aiguillage(2, DEVIE, 0);

    //Initialisation de la locomotive 1
    locomotive1.fixerNumero(1);
    locomotive1.fixerVitesse(30);
    locomotive1.fixerPosition(26, 13);
    locomotive1.allumerPhares();
    locomotive1.demarrer();
    locomotive1.afficherMessage("Loco1: Ready!");

    //Initialisation de la locomotive 2
    locomotive2.fixerNumero(2);
    locomotive2.fixerVitesse(30);
    locomotive2.fixerPosition(13, 14);
    locomotive2.allumerPhares();
    locomotive2.demarrer();
    locomotive2.afficherMessage("Loco2: Ready!");

    // Création du thread de gestion de la loco 1
    QThread* thread1 = new QThread;
    LocoWorker* worker1 = new LocoWorker(locomotive1, parcours1);
    worker1->moveToThread(thread1);
    QObject::connect(thread1, SIGNAL(started()), worker1, SLOT(process()));
    QObject::connect(worker1, SIGNAL(finished()), thread1, SLOT(quit()));
    QObject::connect(worker1, SIGNAL(finished()), worker1, SLOT(deleteLater()));
    QObject::connect(thread1, SIGNAL(finished()), thread1, SLOT(deleteLater()));
    thread1->start();

    // Création du thread de gestion de la loco 2
    QThread* thread2 = new QThread;
    LocoWorker* worker2 = new LocoWorker(locomotive2, parcours2);
    worker2->moveToThread(thread2);
    QObject::connect(thread2, SIGNAL(started()), worker2, SLOT(process()));
    QObject::connect(worker2, SIGNAL(finished()), thread2, SLOT(quit()));
    QObject::connect(worker2, SIGNAL(finished()), worker2, SLOT(deleteLater()));
    QObject::connect(thread2, SIGNAL(finished()), thread2, SLOT(deleteLater()));
    thread2->start();

    thread1->wait();
    thread2->wait();

    //Arreter la locomotive
    locomotive1.arreter();
    locomotive1.afficherMessage("Yeah, piece of cake!");

    //Fin de la simulation
    mettre_maquette_hors_service();

    //Exemple de commande
    afficher_message("Enter a command in the input field at the top of the window.");
    QString commande = getCommand();
    afficher_message(qPrintable(QString("Your command is: ") + commande));

    return EXIT_SUCCESS;
}
