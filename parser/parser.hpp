#ifndef PARSER_HPP_
#define PARSER_HPP_


#include <QFile>
#include <QString>
#include <QMap>
#include <QStack>
#include "../game/exception.hpp"

class EParser : public EException {
    protected: 
        int _line;
    public:
        EParser(QString message, int line, const QString s = "")
            : EException(QString("Exception \"%1\" at line %2 %3").arg(message).arg(line).arg(s)), _line(line) {};
        ~EParser() {};
        int line() { return _line; };
};

    
class Parser {
    protected:
        QFile input;
        int _line;
        int _lastToken;
        QByteArray lastWord;
        
        //QMap<QString, int>& tokens;
        //QMap<int, QString>& names;
        
        void skipWhitespace();
        void readToken();
        
    public:
    
        Parser(QString filename);
        virtual ~Parser() { input.close(); };
        
        bool eof();
        int line();
    
        /*Parser& operator +=(QString c);
        Parser& operator ,(QString c);
        Parser& operator ,(int number);
        */
        int nextToken();
        bool tryNextToken(); 
        int lastToken();
        
        Parser& operator >>(int &number);
        Parser& operator >>(float &number);
        Parser& operator >>(QString &text);
        
        int getInt();
        float getFloat();
        QString getString();
        
        void match(QString c);
        bool tryMatch(QString c);
        //QString getTokenName(int index);
};

#endif
