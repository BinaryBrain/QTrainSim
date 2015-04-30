#include "troncon.h"

Troncon::Troncon(QList<int> contacts, QList<int> switches) {
    _contacts = contacts;
    _switches = switches;
    _isLockBy = 0;
}

Troncon::Troncon(const Troncon&) {}

Troncon::~Troncon()
{
}

QList<int> Troncon::contacts() const {
    return _contacts;
}

void Troncon::lock(Locomotive& loco, const std::vector<std::pair<int, int>>& switchesMap, const bool blocking) {
    _mutex.lock();

    if (!_isLockBy) {
        _isLockBy = loco.numero();
        afficher_message("free, locking for:");
        afficher_message(std::to_string(_isLockBy).c_str());

        for (unsigned int i = 0; i < switchesMap.size(); i++) {
            for (int j = 0; j < _switches.size(); j++) {
                if (switchesMap.at(i).first == _switches.at(j)) {
                    diriger_aiguillage(switchesMap.at(i).first, switchesMap.at(i).second, 0);
                }
            }
        }

    } else if(_isLockBy != loco.numero()) {
        afficher_message("Already locked by:");
        afficher_message(std::to_string(_isLockBy).c_str());

        if (blocking) {
           loco.arreter();
        }

        _waitingLocomotive = &loco;
        _waitingLocomotiveSwitches = switchesMap;
    }

    _mutex.unlock();
}

void Troncon::unlock() {
    _mutex.lock();

    if (_waitingLocomotive) {
        for (unsigned int i = 0; i < _waitingLocomotiveSwitches.size(); i++) {
            for (int j = 0; j < _switches.size(); j++) {
                if (_waitingLocomotiveSwitches.at(i).first == _switches.at(j)) {
                    diriger_aiguillage(_waitingLocomotiveSwitches.at(i).first, _waitingLocomotiveSwitches.at(i).second, 0);
                }
            }
        }

        // Débolquer une tâche en attente dans la file
        _isLockBy = _waitingLocomotive->numero();
        _waitingLocomotive->demarrer();
        _waitingLocomotive = nullptr;
    } else {
        _isLockBy = 0;
    }

    _mutex.unlock();
}

bool Troncon::containsContact(const int contact) const {
    for (int i = 0; i < _contacts.size(); i++) {
        if (_contacts[i] == contact) {
            return true;
        }
    }

    return false;
}

bool Troncon::containsContact(const int contact1, const int contact2) const {
    int n = 0;

    for (int i = 0; i < _contacts.size(); i++) {
        if (_contacts[i] == contact1 || _contacts[i] == contact2) {
            n++;
            if (n == 2) {
                return true;
            }
        }
    }

    return false;
}
