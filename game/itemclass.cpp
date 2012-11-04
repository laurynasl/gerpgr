#include "itemclass.hpp"
#include "masks.hpp"
#include "tables.hpp"
#include <assert.h>
#include <iostream>

using namespace std;

ItemClass:: ItemClass() {
}

ItemClass:: ~ItemClass() {
}

ItemClass:: ItemClass(const ItemClass& other) {
    onehanded = other.onehanded;
    twohanded = other.twohanded;
    thrown = other.thrown;
    name = other.name;
    stackability = other.stackability;
    features = other.features;
    stringFeatures = other.stringFeatures;
    intFeatures = other.intFeatures;
    tokenFeatures = other.tokenFeatures;
}

void ItemClass:: load(Parser& parser) {
    stackability = Stackable; // most items are stackable, except containers, depletable and some others
    parser >> name;
    int token;
    do {
        token = parser.nextToken();
        switch (token) {
            case tktOnehanded: {
                onehanded.load(parser);
                break;
            }
            case tktTwohanded: {
                twohanded.load(parser);
                break;
            }
            case tktThrown: {
                thrown.load(parser);
                break;
            }
            // just features
            case tktPrototype: {
                features.insert(token);
                break;
            }
            // string features
            case tktKey: {
                stringFeatures.insert(token, parser.getString());
                Tables::addKeyCode(stringFeatures[token]);
                break;
            }
            // int features
            case tktDv: {
                intFeatures.insert(token, parser.getInt());
                break;
            }
            // token features
            case tktEquipSlot: {
                tokenFeatures.insert(token, parser.nextToken());
                break;
            }
            case tktLike: {
                QString otherName;
                parser >> otherName;
                int otherIndex = Tables::nameToItem(otherName);
                assert(otherIndex >= 0);
                const ItemClass& other = Tables::itemToClass(otherIndex);
                {// name isn't copied. everything else is
                    onehanded = other.onehanded;
                    twohanded = other.twohanded;
                    thrown = other.thrown;
                    stackability = other.stackability;
                    features = other.features;
                    stringFeatures = other.stringFeatures;
                    intFeatures = other.intFeatures;
                    tokenFeatures = other.tokenFeatures;
                    // parent (other) item mustn't be marked as prototype to be used as prototype
                    features.remove(tktPrototype);
                }
                break;
            }
            case tktDamage: {
                int token2 = parser.nextToken();
                switch (token2) {
                    case tktHp: {
                        int amount;
                        parser >> amount;
                        onehanded.hDamage += amount;
                        twohanded.hDamage += amount;
                        thrown.hDamage += amount;
                        break;
                    }
                    case tktFp: {
                        int amount;
                        parser >> amount;
                        onehanded.fDamage += amount;
                        twohanded.fDamage += amount;
                        thrown.fDamage += amount;
                        break;
                    }
                    default: {
                        throw EException(QString("itemclass.cpp: tktDamage: Unexpected token \"%1\"").arg(Tables::tokenToName(token2)));
                    }
                }
                break;
            }
            case tktMaximum: {
                int token2 = parser.nextToken();
                switch (token2) {
                    case tktHp: {
                        int amount;
                        parser >> amount;
                        onehanded.hMaximum += amount;
                        twohanded.hMaximum += amount;
                        thrown.hMaximum += amount;
                        break;
                    }
                    case tktFp: {
                        int amount;
                        parser >> amount;
                        onehanded.fMaximum += amount;
                        twohanded.fMaximum += amount;
                        thrown.fMaximum += amount;
                        break;
                    }
                    default: {
                        throw EException(QString("itemclass.cpp: tktMaximum: Unexpected token \"%1\"").arg(Tables::tokenToName(token2)));
                    }
                }
                break;
            }
            case tktMinstrength:{
                int amount;
                parser >> amount;
                onehanded.minStrength += amount;
                twohanded.minStrength += amount;
                thrown.minStrength += amount;
                break;
            }
            case tktEnd: {
                break;
            }
            default: {
                throw EException(QString("itemclass.cpp: Unexpected token \"%1\"").arg(Tables::tokenToName(token)));
            }
        }
    } while (token != tktEnd);
}

void ItemClass:: print() const {
    cout << "item \"" << name.toStdString() << "\"" << endl;
    cout << "    onehanded" << endl;
    onehanded.print();
    cout << "    end" << endl;
    cout << "    twohanded" << endl;
    twohanded.print();
    cout << "    end" << endl;
    cout << "    thrown" << endl;
    thrown.print();
    cout << "    end" << endl;
    foreach (int feature, features) {
        cout << "    " << Tables::tokenToName(feature).toStdString() << endl;
    }
    foreach (int feature, stringFeatures.keys()) {
        cout << "    " << Tables::tokenToName(feature).toStdString() << " \"" << stringFeatures[feature].toStdString() << "\"" << endl;
    }
    foreach (int feature, intFeatures.keys()) {
        cout << "    " << Tables::tokenToName(feature).toStdString() << " " << intFeatures[feature] << endl;
    }
    foreach (int feature, tokenFeatures.keys()) {
        cout << "    " << Tables::tokenToName(feature).toStdString() << " " << Tables::tokenToName(tokenFeatures[feature]).toStdString() << endl;
    }
    cout << endl;
}

QString ItemClass:: getKeyValue() const {
    if (stringFeatures.contains(tktKey)) {
        return stringFeatures[tktKey];
    }
    else {
        return "";
    }
}

bool ItemClass:: isPrototype() const {
    return features.contains(tktPrototype);
}

int ItemClass:: getDv() const {
    if (intFeatures.contains(tktDv)) {
        return intFeatures[tktDv];
    }
    else {
        return 0;
    }
}

int ItemClass:: getEquipSlot() const {
    if (tokenFeatures.contains(tktEquipSlot)) {
        return tokenFeatures[tktEquipSlot];
    }
    else {
        return -1;
    }
}
