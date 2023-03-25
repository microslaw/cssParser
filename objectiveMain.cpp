
#include <stdio.h>
#include <stdlib.h>

#define T 8
#define STRLEN 16

#define NULLC '\0'
#define ENDL '\n'

#define SELECTORENDs '}'

#define COMMANDSTART "????"
#define COMMANDEND "****"

#define STARTBRACKET '{'
#define ENDBRACKET '}'
#define SELECTORSEPARATOR ','
#define ATTRIBUTENAMEVALSEPARATOR ':'
#define ATTRIBUTESEPARATOR ';'
#define SPACE ' '
#define TABULATOR '\t'
#define COMMANDARGSSEPARATOR ','

bool isWhiteSpace(char ch)
{
    return ch == SPACE || ch == TABULATOR || ch == ENDL;
}

class Str
{
public:
    char *charList;
    // reservedSize includes NULLC (minimum is 1), if 0 no memory allocated
    int reservedSize;

    // size excludes NULLC
    Str(int size = 0, Str *str = nullptr)
    {
        this->reservedSize = size;
        if (str == nullptr)
        {
            if (size == 0)
            {
                this->charList = nullptr;
                return;
            }
            this->charList = new char[size];
            this->charList[size] = NULLC;
        }
    }

    int length() const
    {
        int i = 0;
        while (this->charList[i] != NULLC)
        {
            i++;
        }
        return i;
    }

    void stripEnd()
    {
        int i = this->length();
        while (isWhiteSpace(this->charList[i]))
        {
            this->charList[i] = NULLC;
            i--;
            if (i < this->reservedSize - 16)
            {
                this->charList = (char *)realloc(this->charList, i - 16);
            }
        }
    }

    bool isEmpty() const
    {
        return !(this->length());
    }

    void copyToCharlist(char *charList) const
    {
        for (int i = 0; this->charList[i] != NULLC; i++)
        {
            charList[i] = this->charList[i];
        }
    }

    int toInt() const
    {
        int result = 0;
        for (int i = 0; i < this->length(); i++)
        {
            result *= 10;
            result += this->charList[i] - '0';
        }
        return result;
    }

    Str &operator+=(char ch)
    {
        if (this->reservedSize == 0)
        {
            this->reservedSize = STRLEN;
            this->charList = (char *)malloc(reservedSize);
            this->charList[0] = NULLC;
        }
        else if (this->length() + 2 > this->reservedSize)
        {
            this->reservedSize += STRLEN;
            char *tmp = (char *)realloc(this->charList, this->reservedSize);
            this->copyToCharlist(tmp);
            this->charList = tmp;
        }

        this->charList[this->length() + 1] = NULLC;
        this->charList[this->length()] = ch;

        return *this;
    }

    char &operator[](int index)
    {
        return this->charList[index];
    }

    ~Str()
    {
        if (this->charList != nullptr)
        {
            free(this->charList);
        }
    }
};

class Attr
{
public:
    Str name;
    Str value;

    Attr()
    {
        this->name = Str();
        this->value = Str();
    }
    Attr(Str name, Str value)
    {
        this->name = name;
        this->value = value;
    }

    ~Attr()
    {
    }
};

class Block
{
public:
    Block *next;
    Block *prev;

    Str *selectors;
    int selectorCount;

    Attr *attributes;
    int attrCount;

    Block(Block *prev = nullptr, int selectorCount = T, int attrCount = T)
    {

        this->prev = prev;
        this->next = nullptr;

        if (prev != nullptr)
        {
            prev->next = this;
        }

        Str *selectors = new Str[selectorCount];
        this->selectors = selectors;
        this->selectorCount = selectorCount;

        Attr *attr = new Attr[attrCount];
        this->attributes = attr;
        this->attrCount = attrCount;
    }

    // accepts negative indexes
    Block *operator[](int index)
    {
        Block *tmp = this;
        if (index > 0)
        {
            for (int i = 0; i < index; i++)
            {
                tmp = tmp->next;
            }
            return tmp;
        }
        index *= (-1);
        for (int i = 0; i < index; i++)
        {
            tmp = tmp->prev;
        }
    }

    ~Block()
    {
        this->prev->next = this->next;
        this->next->prev = this->prev;
        delete this->selectors;
        delete this->attributes;
    }
};

// returns first non whitespace char
char skipWhitespace()
{
    char ch = getchar();
    while (isWhiteSpace(ch))
    {
        ch = getchar();
    }
    return ch;
}

// after execution text left in ostream does not have STARTBRACKET
void readSelectors(Block *block)
{
    int i = 0;

    char ch = skipWhitespace();
    while (ch != STARTBRACKET)
    {
        while (ch != SELECTORSEPARATOR && ch != STARTBRACKET)
        {
            block->selectors[i] += ch;
            ch = getchar();
            // final ch value is ignored, It will always be STARTBRACKET
        }
        i++;
    }
    block->selectorCount = i;
}

void readAttributes(Block *block)
{
    char ch = skipWhitespace();
    int i = 0;
    while (ch != ENDBRACKET)
    {
        while (ch != ATTRIBUTENAMEVALSEPARATOR && ch != ENDBRACKET)
        {
            (block->attributes[i]).name += ch;
            ch = getchar();
        }
        (block->attributes[i]).name.stripEnd();
        ch = skipWhitespace();

        while (ch != ATTRIBUTESEPARATOR && ch != ENDBRACKET)
        {
            (block->attributes[i]).value += ch;
            ch = getchar();
        }

        block->attributes->value.stripEnd();
        ch = skipWhitespace();
        i++;
    }
    block->attrCount = i;
}
// reads blocks and COMMANDSTART
// if COMMANDSTART found returns true
bool readBlocks(Block *block)
{
    char ch = skipWhitespace();
    Str commandStr;
    while (ch != COMMANDSTART[0])
    {
        readSelectors(block);
        readAttributes(block);
    }
    if (ch == NULLC)
    {
        // returning here means no commands
        return;
    }

    commandStr += ch;

    //!!! do better input commandstart
    int i = 1;
    while (COMMANDSTART[i] != NULLC)
    {
        if (ch == COMMANDSTART[i])
        {
            commandStr += ch;
        }
        else
        {
            commandStr = Str();
        }
        i++;
    }
}


char readTill(Str buffor, char endChar, char endChar2 = NULLC)
{
    char ch = getchar();
    while (ch != endChar  && ch != endChar2)
    {
        buffor += ch;
        ch = getchar();
    }
    return ch;
}

// reads command type and arguments
// returns command type
char readCommand(Str &arg1, Str &arg2)
{

    int i = 0;
    int j = 0;
    char ch = skipWhitespace();

    readTill(arg1, COMMANDARGSSEPARATOR);
    
    char toReturn = getchar();
    getchar(); // get rid of COMMANDARGSSEPARATOR
    readTill(arg2, ENDL);
    return toReturn;

}

void executeCommands(Block *block)
{
    Str arg1, arg2;
    char ch = skipWhitespace();
}

int main()
{
    while (getchar() != '^')
        ;

    while (true)
    {
        char ch = getchar();
    }

    return 0;
}

// co ma być w array T = 8
// czy można używać klas
// czy można używać realloc

// dodać readtill?
// wczytywanie arg2 w readcommand w każdym przypadku