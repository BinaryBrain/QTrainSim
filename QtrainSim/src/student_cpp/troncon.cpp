#include "troncon.h"

Troncon::Troncon(QList<int> contacts)
{
    _contacts = contacts;
}

Troncon::~Troncon()
{
}

QList<int> Troncon::contacts() const {
    return _contacts;
}
