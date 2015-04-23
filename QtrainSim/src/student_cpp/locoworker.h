#ifndef LOCOWORKER_H
#define LOCOWORKER_H

#include "locomotive.h"
#include "ctrain_handler.h"

#include <QObject>

class LocoWorker : public QObject
{
    Q_OBJECT

private:
    Locomotive _locomotive;
    QList<int> _parcours;

public:
    explicit LocoWorker(Locomotive &locomotive, QList<int> &parcours);
    ~LocoWorker();

signals:

public slots:
    void process();

signals:
    void finished();
    void error(QString err);
};

#endif // LOCOWORKER_H
