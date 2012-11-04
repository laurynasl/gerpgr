#include "objectclass.hpp"
#include "tables.hpp"
#include <iostream>

using namespace std;

ObjectClass:: ObjectClass() {
}

ObjectClass:: ObjectClass(const ObjectClass& other) {
    name = other.name;
    options = other.options;
}

void ObjectClass:: load(Parser &parser) {
    parser >> name;
    int token;
    do {
        token = parser.nextToken();
        switch (token) {
            case tktOptionString: {
                QString optname;
                QString value;
                parser >> optname >> value;
                options[optname] = value;
                break;
            }
            case tktOptionBool: {
                QString optname;
                int value;
                parser >> optname;
                value = parser.nextToken();
                switch (value) {
                    case tktFalse: {
                        options[optname] = false;
                        break;
                    }
                    case tktTrue: {
                        options[optname] = true;
                        break;
                    }
                    default: {
                        throw EException(QString("objectclass.cpp: tktOptionBool: Unexpected token \"%1\"").arg(Tables::tokenToName((int)token)));
                    }
                }
                break;
            }
            case tktOptionInt: {
                QString optname;
                int value;
                parser >> optname >> value;
                options[optname] = value;
                break;
            }
            case tktOptionFloat: {
                QString optname;
                float value;
                parser >> optname >> value;
                options[optname] = value;
                break;
            }
            case tktEnd: {
                break;
            }
            default: {
                cout << "ObjectClass default " << token << endl;
                throw EException(QString("objectclass.cpp: Unexpected token \"%1\"").arg(Tables::tokenToName(token)));
            }
        }
    } while (token != tktEnd);
    //print();
}

void ObjectClass:: print() const {
    cout << "object \"" << name.toStdString() << "\"" << endl;
    foreach(const QString& key, options.keys()) {
        cout << "    " << key.toStdString() << "=" << options[key].toString().toStdString() << endl;
    }
}
