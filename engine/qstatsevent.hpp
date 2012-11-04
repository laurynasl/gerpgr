#ifndef QSTATSEVENT_HPP_
#define QSTATSEVENT_HPP_

#include <Qt>
#include <QEvent>

class QStatsEvent : public QEvent {
    public:
        int hp;
        int maxhp;
        int fp;
        int maxfp;
        int mp;
        int maxmp;
        QStatsEvent(int hp, int maxhp, int fp, int maxfp, int mp, int maxmp);
};

#endif
