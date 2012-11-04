#include "parser.hpp"
#include "../game/tables.hpp"
#include <iostream>
#include <assert.h>

using namespace std;

Parser:: Parser(QString filename) :
        input(filename), _line(1)
        //tokens(Tables::getInstance().tokens()),
        //names(Tables::getInstance().tokenNames())
{
    if(!input.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw EException(QString("Unable to open file %1").arg("data/tiles0"));
    }
}

/*!
    \brief end of stream?
*/
bool Parser::eof()
{
    return input.atEnd();
}


/*! 
    \brief current line number. 
*/
int Parser::line()
{
    return _line;
}


/*!
   \brief skips whitespace, C++ comments (//..),
           C comments (/ *...* /) in input. 
*/
void Parser::skipWhitespace()
{
    char c;    // current char
    
    while (input.getChar(&c)) 
    {
        if (c == '\n') 
            _line++;
        
        if (!isspace(c) && (c != ',') )  // ignore spaces
        {
            if (c == '/')
            {
                if(!input.getChar(&c)) return;
                
                switch (c) {
                    
                    case '/'  :  // c++ style comment // - skip to line end
                    {
                        char c1;
                        while (input.getChar(&c1))
                            if (c1 == '\n') break;
                        _line++;
                        break;
                    }
                    case '*'  :  // c style comment /**/ - find '*/'
                    {
                        do {
                            // search for '*'
                            do 
                            {
                                input.getChar(&c);
                                if (c == '\n') 
                                    _line++;
                            
                            } while ( (c != '*') && 
                                      (!input.atEnd()) );
                            
                            // check if next char is '/' or loop again
                            
                            if(!input.getChar(&c)) return;
                            if ((c == '/') || (input.atEnd())) break;
                        } while (1);
                        
                        break;
                    }
                    default   :  // not comment
                    {
                        input.ungetChar(c);
                        input.ungetChar('/');
                        return;
                    }     
                }
            }
            else 
            {
                input.ungetChar(c); 
                return;
            }
        }
    }
}


/*! 
 \brief skips whitespace and reads a token to mLastWord until whitespace or 
        any of {/ , . EOF} is found in stream.
*/
void Parser::readToken()
{
    skipWhitespace();
    lastWord.clear();
    char c;
    
    while (input.getChar(&c)) 
    {
        if (!isspace(c) && (c != ','))
        {
            lastWord.append(c);
        }
        else 
        {
            input.ungetChar(c);
            break;
        }
    }
}

/*!
   \brief reads integer from stream. 
   \throws exception if no int found.
*/    
int Parser::getInt()
{
    char c;
    int number = 0;
    
    skipWhitespace();
    
    if (!input.getChar(&c))  // eof
        return number;
    
    int sign = 1;
    if ((c == '+') || (c == '-')) {
        if (c == '-') sign = -1;
        if (!input.getChar(&c)) return number * sign;
    }
    
    do {

        if (!isdigit(c))
            throw EParser("Integer expected.", _line);
            else 
            {
                number *= 10;
                number += c - '0';
            }
    
        if (!input.getChar(&c)) return number * sign;
    }
    while (isdigit(c));
    
    input.ungetChar(c);
    return number * sign;
}

Parser& Parser::operator >>(int &number)
{
    number = getInt();
    return *this;
}


float Parser::getFloat()
{
    char buf[128];
    int i = 0;
    char c;
    
    skipWhitespace();
    
    while ( input.getChar(&c) && 
           (isdigit(c) || (c == '-') || (c == '.') || (c == 'e')) && 
           (i <= 127) )
    {
        buf[i] = c;
        i++;
    }
    input.ungetChar(c);
    
    buf[i] = '\0';
    
    return atof(buf);
}

Parser& Parser::operator >>(float &number)
{
    number = getFloat();
    return *this;
}

/*!
   \brief reads quoted string from input. 
   \throws EParser if no string found.
*/    
QString Parser::getString()
{
    QString text;
    
    skipWhitespace();
    char c;
    
    if (!input.getChar(&c))   //eof?
        return text;
    
    if (c != '"') {
        throw EParser("String expected.", _line); 
    }
 
    while ( input.getChar(&c) && (c != '"') && (c != '\n') ) 
    {
        text += c;
    }
    
    if (c != '"') 
        throw EParser("string expected", _line);
        
    return text;
}

Parser& Parser::operator >>(QString &text)
{
    text = getString();
    return *this;
}


/*!
   \brief reads token word from stream and returns corresponding integer, 
          registered with += and , operators. 
   \throws EParser if read word is not registered token.
*/
int Parser::nextToken()
{
    readToken();
    QString token = lastWord;
    
    if (!(Tables::tokenNameExists(token)))
        throw EParser("Unexpected token ", _line, token);
        
    _lastToken = Tables::nameToToken(token);
    return _lastToken;
}

bool Parser::tryNextToken()
{
    readToken();
    QString token = lastWord;
    
    if (!(Tables::tokenNameExists(token)))
    {
        for (int i = token.length() - 1; i >= 0; i--)
            input.ungetChar(token.at(i).toAscii());
        return false;
    }
    else
    {
        _lastToken = Tables::nameToToken(token);
        return true;
    }
}

int Parser::lastToken()
{
    return _lastToken;
}

/*!
   \brief reads and matches given string.
   \throws exception if input and string differs.
*/
void Parser::match(QString c)
{
    skipWhitespace();
    int i = 0;
    char a;
    
    while ((c[i] != '\0') && input.getChar(&a)) 
    {
        if (a != c[i])
            throw EParser("Unexpected token.", _line);
        i++;
    } 
}

bool Parser::tryMatch(QString c) 
{
    skipWhitespace();
    int i = 0;
    char a;
    
    while ((c[i] != '\0') && input.getChar(&a)) 
    {
        if (a != c[i])
        {
            input.ungetChar(a);
            
            for (int j = i-1; j >=0; j--)
                input.ungetChar(c[j].toAscii());
            return false;
        }
        i++;
    }  
    return true;
}
