#include "qstatsevent.hpp"

QStatsEvent:: QStatsEvent(int hp, int maxhp, int fp, int maxfp,
                          int mp, int maxmp) : QEvent(User) {
    this->hp = hp;
    this->maxhp = maxhp;
    this->fp = fp;
    this->maxfp = maxfp;
    this->mp = mp;
    this->maxmp = maxmp;
}
