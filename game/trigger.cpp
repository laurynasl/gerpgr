#include "trigger.hpp"
#include "tables.hpp"
#include "actionrandom.hpp"
#include "actiontravel.hpp"

Trigger:: Trigger() {
}

Trigger:: ~Trigger() {
    foreach (AbstractAction* action, actions)
        delete action;
}

void Trigger:: loadActions(Parser& parser) {
    //cout << "loading actions" << endl;
    int token;
    do {
        token = parser.nextToken();
        switch(token) {
            case tktRandom: {
                actions.append(new ActionRandom(parser));
                break;
            }
            case tktTravel: {
                actions.append(new ActionTravel(parser));
                break;
            }
            case tktEnd: {
                break;
            }
            default: {
                throw QString("gamemap.cpp: loadActions(): Unexpected token %1").arg(Tables::tokenToName(token));
            }
        }
    } while (token != tktEnd);
}

void Trigger:: saveActions(ofstream& file) const {
    foreach (AbstractAction* action, actions)
        action->save(file);
    file << "    end" << endl;
}

void Trigger:: load(Parser& parser) {
    coordinates.load(parser);
    parser >> duration;
    custLoad(parser);
    loadActions(parser);
}

void Trigger:: save(ofstream& file) const {
    file << "    trigger " << name().toStdString() << " ";
    coordinates.save(file);
    file << duration << " ";
    custSave(file);
    saveActions(file);
}

bool Trigger:: activate(Monster& monster) {
    if (canActivate(monster)) {
        foreach (AbstractAction* action, actions) {
            if (!action->run(monster)) return false;
        }
    }
    return true;
}

QList<QIntsPair> Trigger:: getPoints() const {
    return coordinates.getPoints();
}

int Trigger:: test(bool verbose) const {
    int result = 0;
    foreach (AbstractAction* action, actions) {
        result = max(result, action->test(verbose));
    }
    if (verbose && result) {
        cout << "Something bad with trigger: " << name().toStdString() << endl;
    }
    return result;
}
