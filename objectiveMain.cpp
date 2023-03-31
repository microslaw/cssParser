
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
char movechar(char toPushBack = NULLC)
{
    static char buffer[STRLEN];
    static int bufferLen = 0;
    if (toPushBack == NULLC)
    {
        if (bufferLen == 0)
        {
            return (int)getchar();
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

    void copyToCharlist(char *newCopy) const
    {
        for (int i = 0; this->charList[i] != NULLC; i++)
        {
            newCopy[i] = this->charList[i];
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

    char operator[](int index) const
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

void print(Str &toPrint)
{
    int len = toPrint.length();
    for (int i = 0; i < len; i++)
    {
        putchar(toPrint[i]);
    }
}

Str intToStr(int number)
{
    Str strNumber;
    char charDigit;
    while (number > 0)
    {
        charDigit = (char)((number % 10) + (int)'0');
        strNumber += charDigit;
        number /= 10;
    }
    return strNumber;
}

void print(int toPrint)
{
    Str strToPrint = intToStr(toPrint);
    print(strToPrint);
}

class Selector
{
public:
    Str name;
    Selector *next;

    Selector(Selector *next = nullptr)
    {
        this->name = Str();
        this->next = next;
    }
    Selector(Str name, Str valueSelector, Selector *next = nullptr)
    {
        this->name = name;
        this->next = next;
    }

    // allows to move forward in a list
    Selector &operator[](int index)
    {
        Selector *tmp = this;
        for (int i = 0; i < index; i++)
        {
            tmp = tmp->next;
        }
        return *tmp;
    }

    void killChildren()
    {
        if (this->next != nullptr)
        {
            this->next->killChildren();
        }
        delete (this->next);
    }

    ~Selector()
    {
    }
};
class Attr
{
public:
    Str name;
    Str value;
    Attr *next;

    Attr(Attr *next = nullptr)
    {
        this->name = Str();
        this->value = Str();
        this->next = next;
    }
    Attr(Str name, Str valueAttr, Attr *next = nullptr)
    {
        this->name = name;
        this->value = value;
        this->next = next;
    }

    Attr &operator[](int index)
    {
        Attr *tmp = this;
        for (int i = 0; i < index; i++)
        {
            tmp = tmp->next;
        }
        return *tmp;
    }

    void killChilden()
    {
        if (this->next != nullptr)
        {
            this->next->killChilden();
        }
        delete (this->next);
    }

    ~Attr()
    {
    }
};

class Block
{
public:
    Selector *selectorHead;
    Selector *selectorTail;
    Attr *attributeHead;
    Attr *attributeTail;

    Block(Selector *selectors = nullptr, Attr *attributes = nullptr)
    {
        this->selectorHead = selectors;
        this->attributeHead = attributes;
    }

    void addSelector(Selector *newSelector)
    {
        if (this->selectorHead == nullptr)
        {
            this->selectorTail = newSelector;
            this->selectorHead = newSelector;
        }
        else
        {
            this->selectorTail->next = newSelector;
            this->selectorTail = newSelector;
        }
    }

    int countSelectors()
    {
        Selector *tmp = (this->selectorHead);
        int i;
        for (i = 0; tmp != nullptr; i++)
        {
            tmp = tmp->next;
        }
        return i;
    }

    void addAttribute(Attr *newAttr)
    {
        if (this->attributeHead == nullptr)
        {
            this->attributeTail = newAttr;
            this->attributeHead = newAttr;
        }

        this->attributeTail->next = newAttr;
        this->attributeTail = newAttr;
    }

    int countAttributes() const
    {
        Attr *tmp = (this->attributeHead);
        int i;
        for (i = 0; tmp != nullptr; i++)
        {
            tmp = tmp->next;
        }
        return i;
    }

    bool isEmpty() const
    {
        if (this->attributeHead == nullptr)
        {
            return true;
        }
        return false;
    }

    ~Block()
    {
    }
};

class BlockHolder
{
public:
    Block blocks[T];
    BlockHolder *next;
    BlockHolder *prev;
    BlockHolder(BlockHolder *prev = nullptr)
    {

        this->prev = prev;
        this->next = nullptr;

        if (prev != nullptr)
        {
            prev->next = this;
        }
    }

    // will add new block in first empty place

    Block *addBlock(Block &newBlock)
    {
        for (int i = 0; i < T; i++)
        {
            if (blocks[i].isEmpty())
            {
                blocks[i] = newBlock; ///!!! move constructor?
                return blocks + i;
            }
        }
        if (this->next == nullptr)
        {
            this->next = new BlockHolder(this);
        }
        return this->next->addBlock(newBlock);
    }

    Block *addBlock()
    {
        Block newBlock;
        Block *pBlock = addBlock(newBlock);
        return pBlock;
    }

    int countBlocks()
    {
        int blockCount = 0;
        for (int i = 0; i < T; i++)
        {
            if (!(blocks[i].isEmpty()))
            {
                blockCount++;
            }
        }
        return blockCount;
    }

    // returns reference to block. can jump to another nodes, skips empty blocks
    Block &operator[](int index)
    {
        if (index > this->countBlocks())
        {
            index -= this->countBlocks();
            return ((*(this->next))[index]);
        }
        int i;
        for (i = 0; i < index; i++)
        {
            if ((this->blocks)[i].isEmpty())
            {
                i--;
            }
        }
        return (this->blocks)[i];
    }

    ~BlockHolder()
    {
        this->prev->next = this->next;
        this->next->prev = this->prev;
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
void readSelectors(Block &block)
{
    skipWhitespace();

    while (true)
    {
        block.addSelector(new Selector);
        Str &selectorName = block.selectorTail->name;
        readTill(selectorName, SELECTORSEPARATOR, STARTBRACKET);
        selectorName.stripEnd();
        skipWhitespace();

        if (skip(STARTBRACKET)) // value returned by movechar will be either SELECTORSEPARATOR or STARTBRACKET, and can be ignored
        {
            break;
        }
        skip(SELECTORSEPARATOR);

        skipWhitespace();
    }
}

void readAttributes(Block &block)
{
    skipWhitespace();
    while (true)
    {
        block.addAttribute(new Attr);
        Str &attrName = block.attributeTail->name;
        Str &attrValue = block.attributeTail->value;

        readTill(attrName, ATTRIBUTENAMEVALSEPARATOR);
        attrName.stripEnd();
        skip(ATTRIBUTENAMEVALSEPARATOR);
        skipWhitespace();

        readTill(attrValue, ATTRIBUTESEPARATOR, ENDBRACKET);
        skip(ATTRIBUTESEPARATOR);
        attrValue.stripEnd();
        skip(ATTRIBUTENAMEVALSEPARATOR);
        skipWhitespace();

        if (skip(ENDBRACKET))
        {
            break;
        }
        skipWhitespace();
    }
}

///!!! fix below function

// reads blocks and COMMANDSTART
// returns number of blocks read
int readBlocks(BlockHolder *holder)
{
    Str commandStr;

    skipWhitespace();
    int i = 0;

    while (!skip(COMMANDSTART[i]))
    {
        Block *block = holder->addBlock();
        readSelectors(*block);
        readAttributes(*block);

        skipWhitespace();
        i++;
    }

    while (COMMANDSTART[++i] != NULLC)
    {
        skip(COMMANDSTART[i]);
    }
    return i;
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

void sCommands(BlockHolder &head, int blockCount, const Str &arg1, const Str &arg2)
{
    if (arg1.isInt() && arg2.isEmpty())
    {
        int i = arg1.toInt();
        if (i <= blockCount)
        {
            print(head[arg1.toInt()].countSelectors());
        }
        return;
    }

    else if (arg1.isInt() && arg2.isInt())
    {
        int i = arg1.toInt() - 1;
        int j = arg2.toInt() - 1;
        if (i <= blockCount && j <= head[i].countSelectors())
        {
            print(head[i].selectorHead[j].name);
        }
        return;
    }

    else if (arg2.isEmpty())
    {
        int selectorsFound = 0;
        int selectorCount = 0;
        for (int i = 0; i < blockCount; i++)
        {
            selectorCount = head[i].countSelectors();
            for (int j = 0; j < selectorCount;)
                if (head[i].selectorHead[j].name == arg1)
                {
                    selectorsFound++;
                }
            i++;
        }
        print(selectorCount);
    }
}

void aCommands(BlockHolder &head, int blockCount, const Str &arg1, const Str &arg2)
{
    if (arg1.isInt() && arg2.isEmpty())
    {
        int i = arg1.toInt() - 1;
        if (i <= blockCount)
        {
            print(head[i].countAttributes() + 1);
        }
        return;
    }
    else if (arg1.isInt())
    {
        int i = arg1.toInt() - 1;
        int attrCount;
        int lastI, lastJ;
        if (i <= blockCount)
        {
            attrCount = head[i].countAttributes();
            for (int j = 0; j < attrCount; j++)
            {
                if (head[i].attributeHead[j].name == arg2)
                {
                    lastI = i;
                    lastJ = j;
                }
            }
        }
        print(head[lastI].attributeHead[lastJ].value);
        return;
    }
    ///!!! remove when reading
    else if (arg2.isEmpty())
    {
        int attrFound = 0;
        int attrCount;
        int lastI, lastJ;

        for (int i = 0; i < blockCount; i++)
        {
            attrCount = head[i].countAttributes();

            for (int j = 0; j < attrCount; j++)
            {
                if (head[i].attributeHead[j].name == arg1)
                {
                    attrFound++;
                }
            }
            i++;
        }
        print(attrFound);
    }
    else
    {
        printf("error");
    }
}

void eCommands(BlockHolder &head, int blockCount, const Str &arg1, const Str &arg2)
{
    int lastI = -1;
    int lastJ = -1;
    int selectorCount;
    for (int i = 0; i < blockCount; i++)
    {
        selectorCount = head[i].countSelectors();

        for (int j = 0; j < selectorCount; j++)
        {
            if (head[i].selectorHead[j].name == arg1)
            {
                lastI = i;
                lastJ = j;
            }
        }
    }
    if (lastI != -1)
    {
        print(head[lastI].attributeHead[lastJ].value);
    }
}

void dCommands(BlockHolder &head, int &blockCount, const Str &arg1, const Str &arg2)
{
    if (arg1.isInt() && arg2.isEmpty())
    {

        delete &(head[arg1.toInt() - 1]);
        // print("deleted");
    }
    else if (arg1.isInt())
    {
        int i = arg1.toInt();
        int attrCount = head[i].countAttributes();

        for (int j = 0; j < attrCount; j++)
        {
            if (head[i].attributeHead[j].name == arg2)
            {
                ///!!!delete ((section->attributes) + i);
            }
        }
        // print("deleted");
    }
    else
    {
        // print("error");
    }
}

void executeCommands(BlockHolder &head, int &blockCount, char command, const Str &arg1, const Str &arg2)
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
    BlockHolder *head = new BlockHolder;
    int blockCount = readBlocks(head);
    Str arg1, arg2;
    char commandType;
    do
    {
        commandType = readCommand(arg1, arg2);
        executeCommands(*head, blockCount, commandType, arg1, arg2);
    } while (commandType != NULLC);

    return 0;
}

// killNext w listach pojedyńczych
// lastI = -1

//adds one extra empty selector