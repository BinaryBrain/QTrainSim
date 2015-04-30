#ifndef TRONCON_H
#define TRONCON_H

#include "locomotive.h"
#include "ctrain_handler.h"

#include <QList>
#include <QString>
#include <QMutex>

class Troncon
{
private:
    int _isLockBy;
    QList<int> _contacts;
    QList<int> _switches;
    Locomotive* _waitingLocomotive = nullptr;
    std::vector<std::pair<int, int>> _waitingLocomotiveSwitches;
    QMutex _mutex;

    Troncon(const Troncon&);

public:
    Troncon(QList<int> contacts, QList<int> switches);
    ~Troncon();

    QList<int> contacts() const;

    void lock(Locomotive& loco, const std::vector<std::pair<int, int>>& switchesMap, const bool blocking=true);
    void unlock();

    bool containsContact(const int contact) const;
    bool containsContact(const int contact1, const int contact2) const;
};

#endif // TRONCON_H
