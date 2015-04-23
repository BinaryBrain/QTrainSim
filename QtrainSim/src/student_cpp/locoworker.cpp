#include "locoworker.h"

LocoWorker::LocoWorker(Locomotive &locomotive, QList<int> &parcours)
{
    _locomotive = locomotive;
    _parcours = parcours;
}

LocoWorker::~LocoWorker()
{

}

void LocoWorker::process() {
    forever {
        for (int nb_tours = 0; nb_tours < 2; nb_tours++) {
            //Attente du passage sur les contacts
            for (int i = 0; i < _parcours.size(); i++) {
                attendre_contact(_parcours.at(i));
                afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.").arg(_locomotive.numero()).arg(_parcours.at(i))));
                _locomotive.afficherMessage(QString("I've reached contact no. %1.").arg(_parcours.at(i)));
            }
        }

        inverser_sens_loco(_locomotive.numero());
        std::reverse(_parcours.begin(), _parcours.end());
    }

    emit finished();
}
