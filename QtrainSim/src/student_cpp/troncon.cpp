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

void Troncon::lock(Locomotive& loco, const std::vector<std::pair<int, int>>& switchesMap) {
    afficher_message("try lock");
    //afficher_message(std::to_string(_contacts.at(0)).c_str());
    //afficher_message(std::to_string(_contacts.at(1)).c_str());
    if (!_isLockBy) {
        afficher_message("lock");
        _isLockBy = loco.numero();

        for (unsigned int i = 0; i < switchesMap.size(); i++) {
            for (int j = 0; j < _switches.size(); j++) {
                if (switchesMap.at(i).first == _switches.at(j)) {
                    diriger_aiguillage(switchesMap.at(i).first, switchesMap.at(i).second, 0);
                }
            }
        }
    } else if(_isLockBy != loco.numero()) {
        afficher_message("already lock");
        // suspendre la tâche en attente dans la file associée à v
        afficher_message("stop");
        loco.arreter();
        _waitingLocomotive = &loco;
        _waitingLocomotiveSwitches = switchesMap;
    }
}

void Troncon::unlock() {
    afficher_message("try unlock");
    if (_waitingLocomotive) {
        afficher_message("run locked loco");

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
