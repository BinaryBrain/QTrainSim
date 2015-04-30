#include "locoworker.h"

LocoWorker::LocoWorker(const Locomotive &locomotive, const QList<int> &parcours,
                       const QList<Troncon*>& map, const std::vector<std::pair<int, int>> switchesMap,
                       const bool highPriority)
{
    _locomotive = locomotive;
    _parcours = parcours;
    _map = map;
    _switchesMap = switchesMap;
    _highPriority = highPriority;
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

                int contact1 = _parcours.at(i);
                int contact2;
                int contact3;
                int contact4;

                contact2 = _parcours.at((i + 1) % _parcours.size());
                contact3 = _parcours.at((i + 2) % _parcours.size());
                contact4 = _parcours.at((i + 3) % _parcours.size());

                for (int j = 0; j < _map.size(); j++) {
                    Troncon* troncon = _map.at(j);

                    if (troncon->containsContact(contact1, contact2)) {
                        if(_currentTroncon) {
                            _currentTroncon->unlock();
                        }

                        _currentTroncon = troncon;
                        troncon->lock(_locomotive, _switchesMap);
                    } else if (troncon->containsContact(contact2, contact3)) {
                        troncon->lock(_locomotive, _switchesMap);
                    } else if (troncon->containsContact(contact3, contact4) && _highPriority) {
                        troncon->lock(_locomotive, _switchesMap, false);
                    }
                }

                afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.").arg(_locomotive.numero()).arg(_parcours.at(i))));
                _locomotive.afficherMessage(QString("I've reached contact no. %1.").arg(_parcours.at(i)));
            }
        }

        inverser_sens_loco(_locomotive.numero());
        std::reverse(_parcours.begin(), _parcours.end());
    }

    emit finished();
}
