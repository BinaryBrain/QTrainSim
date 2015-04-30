#include "ctrain_handler.h"
#include "locomotive.h"
#include "locoworker.h"
#include "troncon.h"

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

    QList<Troncon*> map;
    map.append(new Troncon(QList<int> ({ 1, 2, 25 }),       QList<int>({ 2 })));
    map.append(new Troncon(QList<int> ({ 1, 3, 19, 22 }),   QList<int>({ 1, 13 })));
    map.append(new Troncon(QList<int> ({ 2, 7, 8 }),        QList<int>({ 6 })));
    map.append(new Troncon(QList<int> ({ 3, 4, 5, 6 }),     QList<int>({ 3, 4 })));
    map.append(new Troncon(QList<int> ({ 4, 9 }),           QList<int>({ })));
    map.append(new Troncon(QList<int> ({ 5, 24 }),          QList<int>({ 18 })));
    map.append(new Troncon(QList<int> ({ 6, 11 }),          QList<int>({ })));
    map.append(new Troncon(QList<int> ({ 7, 10, 13, 15 }),  QList<int>({ 5, 9 })));
    map.append(new Troncon(QList<int> ({ 8, 20, 25, 26 }),  QList<int>({ 17 })));
    map.append(new Troncon(QList<int> ({ 9, 10, 11, 12 }),  QList<int>({ 7, 8 })));
    map.append(new Troncon(QList<int> ({ 12, 17 }),         QList<int>({ 19 })));
    map.append(new Troncon(QList<int> ({ 13, 14, 26 }),     QList<int>({ 10 })));
    map.append(new Troncon(QList<int> ({ 14, 19, 20 }),     QList<int>({ 14 })));
    map.append(new Troncon(QList<int> ({ 15, 16, 17, 18 }), QList<int>({ 11, 12 })));
    map.append(new Troncon(QList<int> ({ 16, 21 }),         QList<int>({ })));
    map.append(new Troncon(QList<int> ({ 18, 23 }),         QList<int>({ })));
    map.append(new Troncon(QList<int> ({ 21, 22, 23, 24 }), QList<int>({ 15, 16 })));

    //Initialisation d'un parcours
    QList<int> parcours1;
    QList<int> parcours2;
    parcours1 << 25 << 1 << 19 << 20 << 8 << 7 << 13 << 26;
    parcours2 << 23 << 22 << 3 << 4 << 9 << 10 << 15 << 18;

    // Structure de données pour les aiguillages 1
    std::vector<std::pair<int, int>> switchesMap1;
    switchesMap1.push_back(std::make_pair(1, DEVIE));
    switchesMap1.push_back(std::make_pair(2, DEVIE));
    switchesMap1.push_back(std::make_pair(5, DEVIE));
    switchesMap1.push_back(std::make_pair(6, DEVIE));
    switchesMap1.push_back(std::make_pair(9, DEVIE));
    switchesMap1.push_back(std::make_pair(10, DEVIE));
    switchesMap1.push_back(std::make_pair(13, DEVIE));
    switchesMap1.push_back(std::make_pair(14, DEVIE));
    switchesMap1.push_back(std::make_pair(17, TOUT_DROIT));

    // Structure de données pour les aiguillages 3
    std::vector<std::pair<int, int>> switchesMap2;
    switchesMap2.push_back(std::make_pair(3, DEVIE));
    switchesMap2.push_back(std::make_pair(4, DEVIE));
    switchesMap2.push_back(std::make_pair(7, DEVIE));
    switchesMap2.push_back(std::make_pair(8, DEVIE));
    switchesMap2.push_back(std::make_pair(9, TOUT_DROIT));
    switchesMap2.push_back(std::make_pair(5, TOUT_DROIT));
    switchesMap2.push_back(std::make_pair(11, DEVIE));
    switchesMap2.push_back(std::make_pair(12, TOUT_DROIT));
    switchesMap2.push_back(std::make_pair(15, TOUT_DROIT));
    switchesMap2.push_back(std::make_pair(16, DEVIE));
    switchesMap2.push_back(std::make_pair(13, TOUT_DROIT));
    switchesMap2.push_back(std::make_pair(1, TOUT_DROIT));

    //Initialisation de la locomotive 1
    locomotive1.fixerNumero(1);
    locomotive1.fixerVitesse(10);
    locomotive1.fixerPosition(25, 26);
    diriger_aiguillage(17, TOUT_DROIT, 0);
    locomotive1.allumerPhares();
    locomotive1.demarrer();
    locomotive1.afficherMessage("Loco1: Ready!");

    //Initialisation de la locomotive 2
    locomotive2.fixerNumero(2);
    locomotive2.fixerVitesse(20);
    // locomotive2.fixerPosition(13, 14);
    locomotive2.fixerPosition(23, 18);
    locomotive2.allumerPhares();
    locomotive2.demarrer();
    locomotive2.afficherMessage("Loco2: Ready!");

    // Création du thread de gestion de la loco 1
    QThread* thread1 = new QThread;
    LocoWorker* worker1 = new LocoWorker(locomotive1, parcours1, map, switchesMap1);
    worker1->moveToThread(thread1);
    QObject::connect(thread1, SIGNAL(started()), worker1, SLOT(process()));
    QObject::connect(worker1, SIGNAL(finished()), thread1, SLOT(quit()));
    QObject::connect(worker1, SIGNAL(finished()), worker1, SLOT(deleteLater()));
    QObject::connect(thread1, SIGNAL(finished()), thread1, SLOT(deleteLater()));
    thread1->start();

    // Création du thread de gestion de la loco 2
    QThread* thread2 = new QThread;
    LocoWorker* worker2 = new LocoWorker(locomotive2, parcours2, map, switchesMap2);
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
