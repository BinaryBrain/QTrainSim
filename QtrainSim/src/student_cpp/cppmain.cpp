#include "ctrain_handler.h"
#include "locomotive.h"
#include "locoworker.h"

#include <QList>
#include <QThread>

//Creation d'une locomotive
static Locomotive locomotive1;

//Arret d'urgence
void emergency_stop()
{
    locomotive1.arreter();
    afficher_message("\nSTOP!");
}

//Fonction principale
int cmain()
{
    afficher_message("Hit play to start the simulation...");

    //Choix de la maquette
    selection_maquette(MAQUETTE_B);

    //Initialisation d'un parcours
    QList<int> parcours;
    parcours << 26 << 25 << 1 << 19 << 20 << 8 << 7 << 13;

    //Initialisation des aiguillages
    diriger_aiguillage(5, DEVIE, 0);
    diriger_aiguillage(6, DEVIE, 0);
    diriger_aiguillage(17, TOUT_DROIT, 0);
    diriger_aiguillage(1, DEVIE, 0);
    diriger_aiguillage(2, DEVIE, 0);

    //Initialisation de la locomotive
    locomotive1.fixerNumero(1);
    locomotive1.fixerVitesse(20);
    locomotive1.fixerPosition(26, 13);
    locomotive1.allumerPhares();
    locomotive1.demarrer();
    locomotive1.afficherMessage("Ready!");

    // Création du thread de gestion de la loco 1
    QThread* thread = new QThread;
    LocoWorker* worker = new LocoWorker(locomotive1, parcours);
    worker->moveToThread(thread);
    QObject::connect(thread, SIGNAL(started()), worker, SLOT(process()));
    QObject::connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    QObject::connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
    thread->wait();

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
