
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
                realloc(this->charList, i - 16);
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

    ~Str()
    {
        if (this->charList != nullptr)
        {
            delete this->charList;
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
}

void readAttributes(Block *block)
{
    char ch = skipWhitespace();
    int i = 0;
    while (ch != ENDBRACKET)
    {
        while (ch != ATTRIBUTENAMEVALSEPARATOR && ch != ENDBRACKET)
        {
            block->attributes->name += ch;
            ch = getchar();
        }

        while (ch != ATTRIBUTESEPARATOR && ch != ENDBRACKET)
        {
            block->attributes->name += ch;
            ch = getchar();
        }
    }
}

int main()
{

    Block *block = new Block;
    readSelectors(block);
    readAttributes(block);

    Block *tmp = block;

    return 0;
}
