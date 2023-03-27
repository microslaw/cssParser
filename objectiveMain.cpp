
#include <stdio.h>
#include <stdlib.h>

#define T 8
#define STRLEN 16

#define NULLC '\0'
#define ENDL '\n'

#define SELECTORENDs '}'

#define COMMANDSTART "????"
#define COMMANDEND "****"

#define COMMANDSECTIONCOUNT '?'
#define COMMANDSELECTORS 'S'
#define COMMANDATTRIBUTES 'A'
#define COMMANDSEARCH 'E'
#define COMMANDDELETE 'D'
#define COMMANDNOARG '?'

#define STARTBRACKET '{'
#define ENDBRACKET '}'
#define SELECTORSEPARATOR ','
#define ATTRIBUTENAMEVALSEPARATOR ':'
#define ATTRIBUTESEPARATOR ';'
#define SPACE ' '
#define TABULATOR '\t'
#define COMMANDARGSSEPARATOR ','

// getchar but can return char if it is passed as argument
int movechar(char toPushBack = NULLC)
{
    static char buffer[STRLEN];
    static int bufferLen = 0;
    if (toPushBack == NULLC)
    {
        if (bufferLen == 0)
        {
            return getchar();
        }
        return buffer[--bufferLen];
    }

    buffer[bufferLen] = toPushBack;
    bufferLen++;
    return NULLC;
}

// returns true if skipped char
bool skip(char toSkip)
{
    char ch = movechar();
    if (ch == toSkip)
    {
        return true;
    }
    movechar(ch);
    return false;
}

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
        if (this->reservedSize == 0)
        {
            return 0;
        }
        int i = 0;
        while (this->charList[i] != NULLC)
        {
            i++;
        }
        return i;
    }

    void stripEnd()
    {
        int i = this->length() - 1;
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

    bool isInt() const
    {
        if (this->isEmpty())
        {
            return false;
        }
        for (int i = 0; this->charList[i] != NULLC; i++)
        {
            if (this->charList[i] > '9' || this->charList[i] < '0')
            {
                return false;
            }
        }
        return true;
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

    bool operator==(Str right)
    {
        if (this->length() != right.length())
        {
            return false;
        }
        for (int i = 0; i < this->length(); i++)
        {
            if (this->charList[i] != right.charList[i])
            {
                return false;
            }
        }
        return true;
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
    int countBlocks()
    {
        Block *tmp = this;
        int count = 1;
        while (tmp->next != nullptr)
        {
            tmp = tmp->next;
            count++;
        }
        tmp = this;
        while (tmp->prev != nullptr)
        {
            tmp = tmp->prev;
            count++;
        }
        return count;
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
        return tmp;
    }

    ~Block()
    {
        this->prev->next = this->next;
        this->next->prev = this->prev;
        delete this->selectors;
        delete this->attributes;
    }
};

void readTill(Str &buffor, char endChar, char endChar2 = NULLC)
{
    char ch = movechar();
    while (ch != endChar && ch != endChar2)
    {
        buffor += ch;
        ch = movechar();
    }
    movechar(ch);
}

// returns first non whitespace char
void skipWhitespace()
{
    char ch = movechar();
    while (isWhiteSpace(ch))
    {
        ch = movechar();
    }
    movechar(ch);
}

// after execution text left in ostream does not have STARTBRACKET
void readSelectors(Block *block)
{
    int i = 0;

    skipWhitespace();

    while (true)
    {
        readTill((block->selectors)[i], SELECTORSEPARATOR, STARTBRACKET);
        (block->selectors)[i].stripEnd();
        skipWhitespace();

        if (skip(STARTBRACKET)) // value returned by movechar will be either SELECTORSEPARATOR or STARTBRACKET, and can be ignored
        {
            break;
        }
        skip(SELECTORSEPARATOR);

        skipWhitespace();
        i++;
    }
    block->selectorCount = i;
}

void readAttributes(Block *block)
{
    skipWhitespace();
    int i = 0;
    while (true)
    {
        readTill((block->attributes[i]).name, ATTRIBUTENAMEVALSEPARATOR);
        (block->attributes[i]).name.stripEnd();
        skip(ATTRIBUTENAMEVALSEPARATOR);
        skipWhitespace();

        readTill((block->attributes[i]).value, ATTRIBUTESEPARATOR, ENDBRACKET);
        block->attributes->value.stripEnd();
        skip(ATTRIBUTENAMEVALSEPARATOR);
        skipWhitespace();

        i++;

        if (skip(ENDBRACKET))
        {
            break;
        }
        skipWhitespace();
    }
    block->attrCount = i;
}

///!!! fix below function

// reads blocks and COMMANDSTART
// returns number of blocks read
Block *readBlocks()
{
    Str commandStr;
    Block *block;
    Block *prev = nullptr;

    skipWhitespace();
    int i = 0;

    while (!skip(COMMANDSTART[i]))
    {
        block = new Block(prev);
        readSelectors(block);
        readAttributes(block);
        
        skipWhitespace();
        prev = block;
        
    }

    while (COMMANDSTART[++i] != NULLC)
    {
        skip(COMMANDSTART[i]);
    }
}

// reads command type and arguments
// returns command type or null if NULLC or EOF is reached
char readCommand(Str &arg1, Str &arg2)
{

    int i = 0;
    skipWhitespace();

    if (skip(COMMANDSECTIONCOUNT))
    {
        return COMMANDSECTIONCOUNT;
    }
    else if (skip(NULLC) || skip(EOF))
    {
        return NULLC;
    }
    readTill(arg1, COMMANDARGSSEPARATOR);
    skip(COMMANDARGSSEPARATOR);

    char commandType = movechar();
    skip(COMMANDARGSSEPARATOR);

    if (skip(COMMANDNOARG))
    {
        return commandType;
    }
    readTill(arg2, ENDL);
    return commandType;
}

void sCommands(Block *head, int blockCount, const Str &arg1, const Str &arg2)
{
    if (arg1.isInt() && arg2.isEmpty())
    {
        int i = arg1.toInt();
        if (i <= blockCount)
        {
            printf("%d", head[arg1.toInt() - 1].selectorCount + 1);
        }
        return;
    }
    else if (arg1.isInt() && arg2.isInt())
    {
        int i = arg1.toInt();
        int j = arg2.toInt();
        if (i <= blockCount && j <= head[i - 1].selectorCount)
        {
            printf("%s", head[i - 1].selectors[j - 1].charList);
        }
        return;
    }
    else if (arg2.isEmpty())
    {
        int selectorCount = 0;
        for (int i = 0; i < blockCount; i++)
        {
            if (head[i].selectors[0] == arg1)
            {
                selectorCount++;
            }
            i++;
        }
        printf("%d", selectorCount);
    }
}

void aCommands(Block *head, int blockCount, const Str &arg1, const Str &arg2)
{
    if (arg1.isInt() && arg2.isEmpty())
    {
        int i = arg1.toInt();
        if (i <= blockCount)
        {
            printf("%d", head[arg1.toInt() - 1].attrCount + 1);
        }
        return;
    }
    else if (arg1.isInt())
    {
        int i = arg1.toInt();
        if (i <= blockCount)
        {
            for (int j = 0; j < head[i - 1].attrCount; j++)
            {
                if (head[i - 1].attributes[j].name == arg2)
                {
                    printf("%s", head[i - 1].attributes[j].value.charList);
                }
            }
        }
        return;
    }
    ///!!! remove when reading
    else if (arg2.isEmpty())
    {
        int selectorCount = 0;
        for (int i = 0; i < blockCount; i++)
        {
            for (int j = 0; j < head[i].attrCount; j++)
            {
                if (head[i].attributes[j].name == arg1)
                {
                    selectorCount++;
                }
            }
            i++;
        }
        printf("%d", selectorCount);
    }
    else
    {
        printf("error");
    }
}

void eCommands(Block *head, int blockCount, const Str &arg1, const Str &arg2)
{
    int lastI = -1;
    int lastJ = -1;
    for (int i = 0; i < blockCount; i++)
    {
        for (int j = 0; j < (head[i]).selectorCount; j++)
        {
            if ((head[i]).selectors[j] == arg1)
            {
                lastI = i;
                lastJ = j;
            }
        }
    }
    if (lastI != -1)
    {
        Str val = (head[lastI]).attributes[lastJ].value;
        printf("%s", val);
    }
}

void dCommands(Block *head, int blockCount, const Str &arg1, const Str &arg2)
{
    if (arg1.isInt() && arg2.isEmpty())
    {
        delete (*head)[arg1.toInt() - 1];
        printf("deleted");
    }
    else if (arg1.isInt())
    {
        Block *section = (*head)[arg1.toInt() - 1];
        for (int i = 0; i < section->attrCount; i++)
        {
            if ((section->attributes)[i].name == arg2)
            {
                delete ((section->attributes) + i);
            }
        }
        printf("deleted");
        section->attrCount--;
    }
    else
    {
        printf("error");
    }
}

void executeCommands(Block *head, int blockCount, char command, const Str &arg1, const Str &arg2)
{
    switch (command)
    {
    case 'S':
        sCommands(head, blockCount, arg1, arg2);
        break;
    case 'A':
        aCommands(head, blockCount, arg1, arg2);
        break;
    case 'E':
        eCommands(head, blockCount, arg1, arg2);
        break;
    case 'D':
        dCommands(head, blockCount, arg1, arg2);
        break;
    case '?':
        printf("%d", blockCount);
        break;
    default:
        printf("error");
        break;
    }
}

int main()
{
    Block *head = readBlocks();
    int blockCount = head->countBlocks();
    Str arg1, arg2;
    char commandType;
    do
    {
        commandType = readCommand(arg1, arg2);
        executeCommands(head, blockCount, commandType, arg1, arg2);
    } while (commandType != NULLC);

    return 0;
}

// co ma być w array T = 8
// czy można używać realloc
// czy w n,A,? trzeba usuwać duplikaty pomiędzy blokami
// czy można includować listy
// czu można używać ungetch
