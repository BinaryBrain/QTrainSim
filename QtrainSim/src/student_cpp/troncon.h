#ifndef TRONCON_H
#define TRONCON_H

#include <QList>

class Troncon
{
    QList<int> _contacts;

public:
    Troncon(QList<int> contacts);
    ~Troncon();

    QList<int> contacts() const;
};

#endif // TRONCON_H
