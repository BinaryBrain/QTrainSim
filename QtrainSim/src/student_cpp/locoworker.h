#ifndef LOCOWORKER_H
#define LOCOWORKER_H

#include "locomotive.h"
#include "ctrain_handler.h"
#include "troncon.h"

#include <QObject>

class LocoWorker : public QObject
{
    Q_OBJECT

private:
    Locomotive _locomotive;
    QList<int> _parcours;
    QList<Troncon*> _map;
    Troncon* _currentTroncon = nullptr;
    std::vector<std::pair<int, int>> _switchesMap;

public:
    explicit LocoWorker(const Locomotive &locomotive, const QList<int> &parcours,
                        const QList<Troncon *> &map, const std::vector<std::pair<int, int>>);
    ~LocoWorker();

signals:

public slots:
    void process();

signals:
    void finished();
    void error(QString err);
};

#endif // LOCOWORKER_H
