
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
#define ESCAPECHAR '\r'
#define COMMANDARGSSEPARATOR ','

#define COMMANDRESULTSEPERATOR " == "

#define DELETENOTIFICATION "deleted"

// class Str
// {
// public:
//     char *charList;
//     // reservedSize includes NULLC (minimum is 1), if 0 no memory allocated
//     int reservedSize;

//     // size excludes NULLC
//     Str(int size = 0, Str *str = nullptr);
//     Str(const Str &old);
//     Str(Str &&old);
//     Str &operator=(const Str &right);
//     Str &operator=(Str &&right);
//     int length() const;
//     void stripEnd();
//     bool isEmpty() const;
//     void copyTo(char *newCopy) const;
//     bool isInt() const;
//     int toInt() const;
//     Str &operator+=(char ch);
//     char &operator[](int index);
//     char operator[](int index) const;
//     bool operator==(const Str &right);

//     ~Str();
// };

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
    return ch == SPACE || ch == TABULATOR || ch == ENDL || ch == ESCAPECHAR;
}

void swap(int &first, int &second)
{
    int tmp = first;
    first = second;
    second = tmp;
}

void swap(char *&first, char *&second)
{
    char *tmp = first;
    first = second;
    second = tmp;
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
            this->charList = (char *)malloc(size * sizeof(char));
            this->charList[size - 1] = NULLC;
        }
    }

    void swapWith(Str &second)
    {
        swap((*this).reservedSize, second.reservedSize);
        swap((*this).charList, second.charList);
    }

    Str(const Str &old)
    {
        this->reservedSize = old.reservedSize;
        if (this->reservedSize != 0)
        {
            this->charList = (char *)malloc(this->reservedSize);
            old.copyTo(this->charList);
        }
        else
        {
            this->charList = nullptr;
        }
    }
    Str(Str &&old)
    {
        this->reservedSize = old.reservedSize;
        this->charList = old.charList;
    }

    // !!! prefferably would use swap here
    Str &operator=(const Str &right)
    {
        Str tmp(right);
        swapWith(tmp);
        return *this;
    }

    Str &operator=(Str &&right)
    {
        Str tmp(right);
        swapWith(tmp);
        return *this;
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
        return (this->reservedSize == 0);
    }

    void copyTo(char *newCopy) const
    {
        if (this->isEmpty())
        {
            return;
        }
        int i;
        for (i = 0; this->charList[i] != NULLC; i++)
        {
            newCopy[i] = this->charList[i];
        }
        newCopy[i] = NULLC;
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

    void erase()
    {
        this->~Str();
        this->charList = nullptr;
        this->reservedSize = 0;
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
            // this->copyTo(tmp);
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

    bool operator==(const Str &right)
    {
        int len = this->length();
        if (len != right.length())
        {
            return false;
        }
        for (int i = 0; i < len; i++)
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

/// !!! move to other swaps

void swap(Str &left, Str &right)
{
    left.swapWith(right);
}

Str intToStr(int number)
{
    Str strNumber;
    char charDigit;
    do
    {
        charDigit = (char)((number % 10) + (int)'0');
        strNumber += charDigit;
        number /= 10;
    } while (number > 0);
    return strNumber;
}

Str getCOMMANDRESULTSEPERATORSTR()
{
    static Str COMMANDRESULTSEPERATORSTR;
    static int i = 0;
    if (i == 0)
    {
        while (COMMANDRESULTSEPERATOR[i] != NULLC)
        {
            COMMANDRESULTSEPERATORSTR += COMMANDRESULTSEPERATOR[i++];
        }
    }
    return COMMANDRESULTSEPERATORSTR;
}

Str getDELETENOTIFICATION()
{
    static Str DELETENOTIFICATIONSTR;
    static int i = 0;
    if (i == 0)
    {
        while (DELETENOTIFICATION[i] != NULLC)
        {
            DELETENOTIFICATIONSTR += DELETENOTIFICATION[i++];
        }
    }
    return DELETENOTIFICATIONSTR;
}

void print(const Str &toPrint, char ending = NULLC)
{
    int len = toPrint.length();
    for (int i = 0; i < len; i++)
    {
        putchar(toPrint[i]);
    }
    if (ending != NULLC)
    {
        putchar(ending);
    }
}

void print(int toPrint, char ending = NULLC)
{
    Str strToPrint = intToStr(toPrint);
    print(strToPrint, ending);
}

void print(char toPrint, char ending = NULLC)
{
    putchar(toPrint);
    if (ending != NULLC)
    {
        putchar(ending);
    }
}

void printResult(const Str &arg1, char commandType, const Str &arg2, const Str &result)
{
    if (commandType != COMMANDSECTIONCOUNT)
    {
        print(arg1, COMMANDARGSSEPARATOR);
        print(commandType, COMMANDARGSSEPARATOR);
        if (arg2.isEmpty())
        {
            print(COMMANDNOARG);
        }
        else
        {
            print(arg2);
        }
    }
    else
    {
        print(COMMANDSECTIONCOUNT);
    }

    /// !!! ugly
    static Str COMMANDRESULTSEPERATORSTR;
    if (COMMANDRESULTSEPERATORSTR.isEmpty())
    {
        COMMANDRESULTSEPERATORSTR = getCOMMANDRESULTSEPERATORSTR();
    }

    print(COMMANDRESULTSEPERATORSTR);
    print(result, ENDL);
}

void printResult(const Str &arg1, char commandType, const Str &arg2, const int &result)
{
    Str resultStr = intToStr(result);
    printResult(arg1, commandType, arg2, resultStr);
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

    // allows to move forward in a list

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

    Attr(const Attr &right)
    {
        this->name = right.name;
        this->value = right.value;
        this->next = right.next;
    }

    Attr(Attr &&right)
    {
        this->name = right.name;
        this->value = right.value;
        this->next = right.next;
    }

    Attr &operator=(const Attr &right)
    {
        Attr tmp(right);
        swapWith(tmp);

        return *this;
    }

    Attr &operator=(Attr &&right)
    {
        Attr tmp(right);
        swapWith(tmp);

        return *this;
    }

    void swapWith(Attr &right)
    {
        swap(this->name, right.name);
        Attr *tmp = this->next;
        this->next = right.next;
        right.next = tmp;
    }

    void killChildren()
    {
        if (this->next != nullptr)
        {
            this->next->killChildren();
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

    // selector with given index must exist
    Selector &getSelector(int index)
    {
        Selector *tmp = this->selectorHead;
        while (index-- != 0)
        {
            tmp = tmp->next;
        }
        return *tmp;
    }

    void addAttribute(Attr *newAttr)
    {
        if (this->attributeHead == nullptr)
        {
            this->attributeTail = newAttr;
            this->attributeHead = newAttr;
        }
        else
        {
            this->attributeTail->next = newAttr;
            this->attributeTail = newAttr;
        }
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

    // selector with given index must exist
    Attr &getAttr(int index)
    {
        Attr *tmp = this->attributeHead;
        while (index-- != 0)
        {
            tmp = tmp->next;
        }
        return *tmp;
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

///!!! half
class BlockHolder
{
public:
    Block *blocks;
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
        this->blocks = new Block[T];
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
    // !!! optimize moving between blocks
    Block &operator[](int index)
    {
        int i = 0;
        while (index != 0)
        {
            if (!(this->blocks)[i].isEmpty())
            {
                index--;
            }
            i++;
            if (i >= T)
            {
                return (*(this->next))[index];
            }
        }
        return (this->blocks)[i];
    }

    ~BlockHolder()
    {
        this->prev->next = this->next;
        this->next->prev = this->prev;
        delete (this->blocks);
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

    int j = 0;
    while (skip(COMMANDSTART[j]))
    {
        j++;
        if (COMMANDSTART[j] == NULLC)
        {
            break;
        }
    }
    return i;
}

// reads command type and arguments
// returns command type or null if NULLC or EOF is reached
char readCommand(Str &arg1, Str &arg2)
{
    arg1.erase();
    arg2.erase();
    skipWhitespace();

    if (skip(COMMANDSECTIONCOUNT))
    {
        return COMMANDSECTIONCOUNT;
    }

    else if (skip(NULLC) || skip(EOF))
    {
        return NULLC;
    }

    int j = 0;
    while (skip(COMMANDEND[j]))
    {
        j++;
        if (COMMANDEND[j] == NULLC)
        {
            return COMMANDEND[0];
        }
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
        int i = arg1.toInt() - 1;
        if (i < blockCount)
        {
            printResult(arg1, COMMANDSELECTORS, arg2, head[i].countSelectors());
        }
        return;
    }

    else if (arg1.isInt() && arg2.isInt())
    {
        int i = arg1.toInt() - 1;
        int j = arg2.toInt() - 1;
        if (i < blockCount && j < head[i].countSelectors())
        {
            printResult(arg1, COMMANDSELECTORS, arg2, head[i].getSelector(j).name);
        }
        return;
    }
    else if (arg2.isEmpty())
    {
        int selectorsFound = 0;
        int selectorCount;
        for (int i = 0; i < blockCount; i++)
        {
            selectorCount = head[i].countSelectors();

            for (int j = 0; j < selectorCount; j++)
            {
                if (head[i].getSelector(j).name == arg1)
                {
                    selectorsFound++;
                }
            }
        }
        printResult(arg1, COMMANDSELECTORS, arg2, selectorsFound);
    }
}

void aCommands(BlockHolder &head, int blockCount, const Str &arg1, const Str &arg2)
{
    if (arg1.isInt() && arg2.isEmpty())
    {
        int i = arg1.toInt() - 1;
        if (i < blockCount)
        {
            printResult(arg1, COMMANDATTRIBUTES, arg2, head[i].countAttributes());
        }
        return;
    }
    else if (arg1.isInt())
    {
        int i = arg1.toInt() - 1;
        int attrCount;
        int lastI = -1;
        int lastJ = -1;
        if (i < blockCount)
        {
            attrCount = head[i].countAttributes();
            for (int j = 0; j < attrCount; j++)
            {

                if (head[i].getAttr(j).name == arg2)
                {
                    lastI = i;
                    lastJ = j;
                }
            }
        }
        if (lastI != -1)
        {
            printResult(arg1, COMMANDATTRIBUTES, arg2, head[lastI].getAttr(lastJ).value);
        }
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
                if (head[i].getAttr(j).name == arg1)
                {
                    attrFound++;
                }
            }
        }
        printResult(arg1, COMMANDATTRIBUTES, arg2, attrFound);
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
            if (head[i].getSelector(j).name == arg1)
            {
                lastI = i;
                lastJ = j;
            }
        }
    }
    if (lastI != -1)
    {
        printResult(arg1, COMMANDSEARCH, arg2, head[lastI].getAttr(lastJ).value);
    }
}

void dCommands(BlockHolder &head, int &blockCount, const Str &arg1, const Str &arg2)
{
    if (arg1.isInt() && arg2.isEmpty())
    {
        int i = arg1.toInt() - 1;
        if (i < blockCount)
        {
            delete &(head[i]);
            printResult(arg1, COMMANDDELETE, arg2, getDELETENOTIFICATION());
            blockCount--;
        }
    }
    else if (arg1.isInt())
    {
        int i = arg1.toInt() - 1;
        int attrCount = head[i].countAttributes();
        bool deleteSuccesfull = false;

        for (int j = 0; j < attrCount; j++)
        {
            if (head[i].getAttr(j).name == arg2)
            {
                delete &(head[i].getAttr(i));
                deleteSuccesfull = true;
            }
        }

        if (head[i].isEmpty())
        {
            delete &(head[i]);
            blockCount--;
        }
        if (deleteSuccesfull)
        {
            /// !!! also ugly
            printResult(arg1, COMMANDDELETE, arg2, getDELETENOTIFICATION());
        }
    }
}

void executeCommands(BlockHolder &head, int &blockCount, char command, const Str &arg1, const Str &arg2)
{
    switch (command)
    {
    case COMMANDSELECTORS:
        sCommands(head, blockCount, arg1, arg2);
        break;
    case COMMANDATTRIBUTES:
        aCommands(head, blockCount, arg1, arg2);
        break;
    case COMMANDSEARCH:
        eCommands(head, blockCount, arg1, arg2);
        break;
    case COMMANDDELETE:
        dCommands(head, blockCount, arg1, arg2);
        break;
    case COMMANDSECTIONCOUNT:
        printResult(arg1, COMMANDSECTIONCOUNT, arg2, blockCount);
        break;
    case COMMANDEND[0]:
        blockCount += readBlocks(&head);
    default:
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

// !!!\r

// killNext w listach pojedyńczych
// lastI = -1
// print only in str