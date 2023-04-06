
#include <stdio.h>
#include <stdlib.h>

#define T 8
#define STRLEN 8

#define NULLC '\0'
#define ENDL '\n'
#define NOINDEX -1

#define SELECTORENDs '}'

#define COMMANDSTART "????"
#define COMMANDEND "****"

#define COMMANDSECTIONCOUNT '?'
#define COMMANDSELECTORS 'S'
#define COMMANDATTRIBUTES 'A'
#define COMMANDSEARCH 'E'
#define COMMANDDELETE 'D'
#define COMMANDNOARG '?'
#define COMMANDNOARG2 '*'

#define ZEROCHAR '0'

#define STARTBRACKET '{'
#define ENDBRACKET '}'
#define SELECTORSEPARATOR ','
#define ATTRIBUTENAMEVALSEPARATOR ':'
#define ATTRIBUTESEPARATOR ';'
#define SPACE ' '
#define TABULATOR '\t'
#define ESCAPECHAR '\r'
#define COMMANDARGSSEPARATOR ','
#define DEBUGSTRDEF ".ms-Button:hover"

#define COMMANDRESULTSEPERATOR " == "

#define NUMBASE 10

#define DELETENOTIFICATION "deleted"

int globalLineCounter = 0;

// getchar but can ungetch char if it is passed as argument
char movechar(char toPushBack = NULLC)
{
    static char buffer[STRLEN];
    static int bufferLen = 0;
    char tmp = 0;
    if (toPushBack == NULLC)
    {
        if (bufferLen == 0)
        {
            tmp = getchar();
            if (tmp == ENDL)
            {
                globalLineCounter++;
            }
            return tmp;
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
    return ch == SPACE || ch == TABULATOR || ch == ENDL || ch == ESCAPECHAR; // || ch < -1;
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
    Str()
    {
        this->reservedSize = 0;
        this->charList = nullptr;
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

    Str(char *&&old)
    {
        this->charList = (char *)malloc(32);
        int i = 0;
        while (old[i] != NULLC)
        {
            this->reservedSize = 32;
            this->charList[i] = old[i];
            i++;
        }
        this->charList[i] = NULLC;
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
        if (this->length() <= 0)
        {
            return;
        }
        while (isWhiteSpace(this->charList[i]))
        {
            this->charList[i] = NULLC;
            i--;
            if (i < this->reservedSize - STRLEN - 1)
            {
                this->reservedSize -= STRLEN;
                this->charList = (char *)realloc(this->charList, this->reservedSize);
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
            if (this->charList[i] > NUMBASE - 1 + ZEROCHAR || this->charList[i] < ZEROCHAR)
            {
                return false;
            }
        }
        return true;
    }

    bool isStr() const
    {
        return (!isInt() && !isEmpty());
    }

    int toInt() const
    {
        int result = 0;
        for (int i = 0; i < this->length(); i++)
        {
            result *= NUMBASE;
            result += this->charList[i] - ZEROCHAR;
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

    bool operator==(char *&&right)
    {
        int charLen = 0;
        for (charLen = 0; right[charLen] != NULLC; charLen++)
            ;
        int len = this->length();
        if (len != charLen)
        {
            return false;
        }
        for (int i = 0; i < len; i++)
        {
            if (this->charList[i] != right[i])
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
            // free(this->charList);
            this->charList = nullptr;
        }
    }
};

/// !!! move to other swaps

void swap(Str &left, Str &right)
{
    left.swapWith(right);
}

void swap(char &left, char &right)
{
    char tmp = left;
    left = right;
    right = tmp;
}

// maybe do a reverse string?
Str intToStr(int number)
{
    Str strNumber;

    if (number == 0)
    {
        strNumber += ZEROCHAR;
    }

    while (number > 0)
    {
        strNumber += ZEROCHAR + (number % NUMBASE);
        number /= NUMBASE;
    }

    int len = strNumber.length() - 1;
    int i = 0;

    while (i <= len - i)
    {
        swap(strNumber[i], strNumber[len - i]);
        i++;
    }

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
    Str resultStr;
    resultStr = intToStr(result);
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

    void killChildren()
    {
        if (this->next != nullptr)
        {
            this->next->killChildren();
        }
        delete (this->next);
    }

    // destroying selector is handled by block

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
        this->next = nullptr;
    }

    // destroying attr is handled by block
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

    Selector *searchForSelector(const Str &val) const
    {
        Selector *currentSelector = this->selectorHead;
        while (currentSelector != nullptr)
        {
            if (currentSelector->name == val)
            {
                return currentSelector;
            }
            currentSelector = currentSelector->next;
        }
        return nullptr;
    }

    // will check if selector exists
    // !! can be optimized by omitting getSelector()
    bool removeSelector(int i)
    {
        if (i > this->countSelectors())
        {
            return false;
        }
        Selector *deleted = &(this->getSelector(i));
        if (i > 0)
        {
            // sets .next of previous node (i-1) to &node(i+1), skipping node i
            (this->getSelector(i - 1)).next = &(this->getSelector(i + 1));
        }
        else
        {
            this->selectorHead = deleted->next;
        }
        if (this->selectorTail == deleted)
        {
            this->selectorTail = &(this->getSelector(i - 1));
        }

        delete deleted;
        return true;
    }

    int removeDuplicateSelectors(Str toRemove)
    {
        int selectorCount;
        int prevJ;
        int removed = 0;

        selectorCount = this->countSelectors();

        prevJ = NOINDEX;

        for (int j = 0; j < selectorCount; j++)
        {
            if (this->getSelector(j).name == toRemove)
            {
                this->removeSelector(j);
                selectorCount--;
                removed++;
            }
        }
        return removed;
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

    Attr *searchForAttr(const Str &val) const
    {
        Attr *currentAttr = this->attributeHead;
        while (currentAttr != nullptr)
        {
            if (currentAttr->name == val)
            {
                return currentAttr;
            }
            currentAttr = currentAttr->next;
        }
        return nullptr;
    }

    // will check if attribute exists
    // !! can be optimized by omitting getAttr()
    // !!! copy to selector
    bool removeAttr(int i)
    {
        if (i > this->countAttributes())
        {
            return false;
        }
        Attr *deleted = &(this->getAttr(i));
        if (i > 0)
        {
            // sets .next of previous node (i-1) to &node(i+1), skipping node i
            (this->getAttr(i - 1)).next = &(this->getAttr(i + 1));
        }
        else
        {
            this->attributeHead = deleted->next;
        }
        if (this->attributeTail == deleted)
        {
            // check if there are any nodes left
            if (this->attributeHead != nullptr)
            {
                this->attributeTail = &(this->getAttr(i - 1));
            }
            // if that was the last attribute, entire block is pointless, so it should be deleted
            else
            {
                this->attributeTail = nullptr;
                this->~Block();
            }
        }

        delete deleted;
        return true;
    }
    // to remove first (attributeHead), pass nullptr
    bool removeAttr(Attr *prevToRemove)
    {
        Attr *toRemove;
        if (prevToRemove != nullptr)
        {
            // sets .next of previous node (i-1) to &node(i+1), skipping node i
            toRemove = prevToRemove->next;
            prevToRemove->next = prevToRemove->next->next;
        }
        else
        {
            if (this->attributeHead == nullptr)
            {
                return false;
            }
            toRemove = this->attributeHead;
            this->attributeHead = this->attributeHead->next;
        }
        if (this->attributeTail == toRemove)
        {
            // check if there are any nodes left
            if (this->attributeHead != nullptr)
            {
                this->attributeTail = prevToRemove;
            }
            // if that was the last attribute, entire block is pointless, so it should be deleted
            else
            {
                this->attributeTail = nullptr;
                this->~Block();
            }
        }

        delete toRemove;
        return true;
    }

    // removes all attributes with this name, except for the last one
    //  returns number of removals
    int removeDuplicateAttr(Str toRemove)
    {
        int attrCount;
        int prevJ;
        int removed = 0;

        attrCount = this->countAttributes();

        prevJ = NOINDEX;

        for (int j = 0; j < attrCount; j++)
        {
            if (this->getAttr(j).name == toRemove)
            {
                if (prevJ != NOINDEX)
                {
                    // this is removal of repeating (therefore not last) argument in a block, so block will never neeed to be deleted here
                    this->removeAttr(prevJ);
                    attrCount--;
                    removed++;
                }
                prevJ = j;
            }
        }
        return removed;
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
        if (this->attributeHead != nullptr)
        {
            this->attributeHead->killChildren();
            delete attributeHead;
        }

        if (this->selectorHead != nullptr)
        {
            this->selectorHead->killChildren();
            delete selectorHead;
        }

        // destructor creates hollow object, pointers need to be zeroed so that isEmpty() will consider object hollow

        this->attributeHead = nullptr;
        this->attributeTail = nullptr;
        this->selectorHead = nullptr;
        this->selectorTail = nullptr;
    }
};

///!!! half
class BlockHolder
{
public:
    Block *blocks;
    BlockHolder *next;
    BlockHolder *prev;
    int lastBlock;

    BlockHolder(BlockHolder *prev = nullptr)
    {

        this->prev = prev;
        this->next = nullptr;

        if (prev != nullptr)
        {
            prev->next = this;
        }
        this->blocks = new Block[T];
        this->lastBlock = 0;
    }

    // will add new block in first empty slot
    // if no argument is passed will return pointer to first empty slot
    Block *addBlock(Block *newBlock = nullptr)
    {
        if (newBlock == nullptr)
        {
            newBlock = new Block;
        }
        if (this->next == nullptr)
        {

            if (this->lastBlock != T)
            {
                blocks[this->lastBlock] = *newBlock;
                this->lastBlock++;
                return this->blocks + this->lastBlock - 1;
            }
            this->next = new BlockHolder(this);
        }
        return this->next->addBlock(newBlock);
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

    /// !!! fix this
    Block *nextBlock(bool reset = false)
    {
        static BlockHolder *currentNode;
        static int blockIndex;

        if (reset)
        {
            blockIndex = 0;
            currentNode = this;
            return nullptr;
        }

        if (currentNode == nullptr)
        {
            return nullptr;
        }

        if (currentNode->blocks[blockIndex].isEmpty())
        {
            blockIndex++;
            return nextBlock();
        }

        if (blockIndex == T)
        {
            blockIndex = 0;
            currentNode = currentNode->next;
        }
        if (currentNode == nullptr)
        {
            return nullptr;
        }
        return currentNode->blocks + blockIndex++;
    }
    Block *prevBlock(bool reset = false)
    {
        static BlockHolder *currentNode;
        static int blockIndex;

        if (reset)
        {
            blockIndex = T - 1;
            currentNode = this;
        }

        if (currentNode == nullptr)
        {
            return nullptr;
        }

        if (blockIndex < 0)
        {
            blockIndex = T - 1;
            currentNode = currentNode->prev;
        }
        if (currentNode == nullptr)
        {
            return nullptr;
        }
        return currentNode->blocks + blockIndex--;
    }

    BlockHolder *removeEmptyNodes()
    {
        BlockHolder *newHead = nullptr;
        if (this->next != nullptr)
        {
            newHead = this->next->removeEmptyNodes();
        }
        if (this->countBlocks() == 0)
        {
            delete this;
            return newHead;
        }
        else
        {
            return this;
        }
    }

    // returns reference to block. can jump to another nodes, skips empty blocks
    // block with specified index must exist;
    // accepts negative indices (due to lack of -0, all are moved by -1)
    // !! optimize moving between blocks
    Block &operator[](int index)
    {
        if (index >= 0)
        {
            for (int i = 0; i < T; i++)
            {
                if (!(this->blocks)[i].isEmpty())
                {
                    index--;
                    if (index < 0)
                    {
                        return (this->blocks)[i];
                    }
                }
            };
            return (*(this->next))[index];
        }
        else
        {
            // move by 1
            index = -index - 1;

            for (int i = T - 1; i >= 0; i--)
            {
                if (!(this->blocks)[i].isEmpty())
                {
                    index--;
                    if (index < 0)
                    {
                        return (this->blocks)[i];
                    }
                }
            };
            return (*(this->prev))[-index];
        }
    }

    void tryDelete(BlockHolder *&pHead, BlockHolder *&pTail)
    {
        if (this->countBlocks() == 0)
        {
            if (this->prev == nullptr)
            {
                pHead = this->next;
            }
            if (this->next == nullptr)
            {
                pTail = this->prev;
            }
            delete this;
        }
    }

    ~BlockHolder()
    {
        delete[] (this->blocks);
        if (this->prev != nullptr)
        {
            this->prev->next = this->next;
        }

        if (this->next != nullptr)
        {
            this->next->prev = this->prev;
        }
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
    // if (skip(ENDL))
    // {
    //     globalLineCounter++;
    // }
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
        Selector *newSelector = new Selector;
        Str &selectorName = newSelector->name;
        readTill(selectorName, SELECTORSEPARATOR, STARTBRACKET);
        selectorName.stripEnd();
        block.removeDuplicateSelectors(selectorName);

        if (selectorName.length() > 0)
        {

            block.addSelector(newSelector);
        }

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
        block.removeDuplicateAttr(attrName);
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

BlockHolder *getTail(BlockHolder *head)
{
    BlockHolder *tail = head;

    if (tail == nullptr)
    {
        return nullptr;
    }

    while (tail->next != nullptr)
    {
        tail = tail->next;
    }
    return tail;
}

// reads blocks and COMMANDSTART
// returns number of blocks read
int readBlocks(BlockHolder *&head, BlockHolder *&tail, int blockCount)
{
    skipWhitespace();

    int i = 0;
    int j = 0;
    Block *block;
    while (true)
    {
        block = head->addBlock();
        readSelectors(*block);
        readAttributes(*block);

        skipWhitespace();
        i++;

        while (skip(COMMANDSTART[j]))
        {
            j++;
            if (COMMANDSTART[j] == NULLC)
            {
                tail = getTail(head);
                return i + blockCount;
            }
        }
        while (j > 0)
        {
            movechar(COMMANDSTART[j]);
            j--;
        }
    }
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
        return EOF;
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
    j--;
    while (j > 0)
    {
        movechar(COMMANDEND[j]);
    }

    readTill(arg1, COMMANDARGSSEPARATOR);
    skip(COMMANDARGSSEPARATOR);

    char commandType = movechar();
    skip(COMMANDARGSSEPARATOR);

    if (skip(COMMANDNOARG) || skip(COMMANDNOARG2))
    {
        return commandType;
    }
    readTill(arg2, ENDL);
    return commandType;
}

void sCommands(BlockHolder *pHead, int blockCount, const Str &arg1, const Str &arg2)
{
    BlockHolder &head = *pHead;
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
        if (i < blockCount)
        {
            Block &iBlock = head[i];
            if (j < iBlock.countSelectors())
            {
                /// !!! switch getSelector() to pointer?
                printResult(arg1, COMMANDSELECTORS, arg2, iBlock.getSelector(j).name);
            }
        }
    }
    else if (arg1.isStr() && arg2.isEmpty())
    {
        int selectorsFound = 0;
        int h = 0;
        Block *currentBlock, *prevBlock;
        head.nextBlock(true);

        // pointer assinged to currentBlock will be treated as a condition in while
        // loop will end when head.nextBlock() returns nullptr
        while (currentBlock = head.nextBlock())
        {
            if (currentBlock->searchForSelector(arg1) != nullptr)
            {
                selectorsFound++;
            }
            prevBlock = currentBlock;
            h++;
        }
        printResult(arg1, COMMANDSELECTORS, arg2, selectorsFound);
    }
}

void aCommands(BlockHolder *pHead, int blockCount, const Str &arg1, const Str &arg2)
{
    BlockHolder &head = *pHead;

    if (arg1.isInt() && arg2.isEmpty())
    {
        int i = arg1.toInt() - 1;
        if (i < blockCount)
        {
            printResult(arg1, COMMANDATTRIBUTES, arg2, head[i].countAttributes());
        }
    }
    else if (arg1.isInt() && arg2.isStr())
    {
        int i = arg1.toInt() - 1;
        Attr *foundAttr;
        if (i < blockCount)
        {
            foundAttr = head[i].searchForAttr(arg2);
            if (foundAttr != nullptr)
            {
                printResult(arg1, COMMANDATTRIBUTES, arg2, foundAttr->value);
            }
        }
    }
    else if (arg1.isStr() && arg2.isEmpty())
    {
        int attrFoundCount = 0;

        Block *currentBlock;
        Attr *foundAttr;

        head.nextBlock(true);
        // pointer assinged to currentBlock will be treated as a condition in while
        // loop will end when head.nextBlock() returns nullptr
        while (currentBlock = head.nextBlock())
        {
            Block &iBlock = *currentBlock;
            foundAttr = iBlock.searchForAttr(arg1);
            if (foundAttr != nullptr)
            {
                attrFoundCount++;
            }
        }
        printResult(arg1, COMMANDATTRIBUTES, arg2, attrFoundCount);
    }
}

void eCommands(BlockHolder *pTail, int blockCount, const Str &arg1, const Str &arg2)
{
    if (arg1.isStr() && arg2.isStr())
    {
        BlockHolder &tail = *pTail;
        Block *currentBlock;
        Selector *pSelector;
        Attr *foundAttr;
        int selectorCount, attrCount;

        for (int i = -1; i > -blockCount; i--)
            tail.prevBlock(true);
        while (currentBlock = tail.prevBlock())
        {
            Block &iBlock = *currentBlock;
            pSelector = iBlock.selectorHead;

            while (pSelector != nullptr)
            {
                if (pSelector->name == arg1)
                {
                    foundAttr = iBlock.searchForAttr(arg2);
                    if (foundAttr != nullptr)
                    {
                        printResult(arg1, COMMANDSEARCH, arg2, foundAttr->value);
                        return;
                    }
                }
                pSelector = pSelector->next;
            }
        }
    }
}

bool findBlockHolder(BlockHolder *&currentBlockHolder, int &index)
{
    int blockIndex = 0;
    while (currentBlockHolder != nullptr && index >= 0)
    {

        for (int j = 0; j < T && index >= 0; j++)
        {
            if (!(currentBlockHolder->blocks[j].isEmpty()))
            {
                index--;
            }
            blockIndex = j;
        }

        if (index >= 0)
        {
            currentBlockHolder = currentBlockHolder->next;
        }
    }
    if (currentBlockHolder == nullptr)
    {
        return false;
    }
    index = blockIndex;
    return true;
}

void dCommands(BlockHolder *&pHead, BlockHolder *&pTail, int &blockCount, const Str &arg1, const Str &arg2)
{
    BlockHolder &head = *pHead;
    /// !!! remove code repetition
    if (arg1.isInt() && arg2.isStr())
    {
        BlockHolder *currentBlockHolder = pHead;
        int blockIndex = arg1.toInt() - 1;

        if (!findBlockHolder(currentBlockHolder, blockIndex))
        {
            return;
        }

        Block &block = currentBlockHolder->blocks[blockIndex];
        Attr *currentAttr = block.attributeHead;
        Attr *prevAttr = nullptr;
        blockCount -= currentBlockHolder->countBlocks();
        while (true)
        {
            if (currentAttr->name == arg2)
            {
                block.removeAttr(prevAttr);
                break;
            }
            prevAttr = currentAttr;
            currentAttr = currentAttr->next;
            if (currentAttr == nullptr)
            {
                blockCount += currentBlockHolder->countBlocks();
                return;
            }
        }

        blockCount += currentBlockHolder->countBlocks();

        currentBlockHolder->tryDelete(pHead, pTail);

        /// !!! also ugly
        printResult(arg1, COMMANDDELETE, arg2, getDELETENOTIFICATION());
    }
    else if (arg1.isInt() && arg2.isEmpty())
    {
        BlockHolder *currentBlockHolder = pHead;
        int blockIndex = arg1.toInt() - 1;

        if (!findBlockHolder(currentBlockHolder, blockIndex))
        {
            return;
        }

        currentBlockHolder->blocks[blockIndex].~Block(); // after delete head[i] calling head[i].isEmpty() may result in segmentation fault
        blockCount--;
        currentBlockHolder->tryDelete(pHead, pTail);

        // !!!ugly
        Str tmp;
        tmp += COMMANDNOARG2;
        printResult(arg1, COMMANDDELETE, tmp, getDELETENOTIFICATION());
        return;
    }
}

void executeCommand(BlockHolder *&head, BlockHolder *&tail, int &blockCount, char command, const Str &arg1, const Str &arg2)
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
        eCommands(tail, blockCount, arg1, arg2);
        break;
    case COMMANDDELETE:
        dCommands(head, tail, blockCount, arg1, arg2);
        break;
    case COMMANDSECTIONCOUNT:
        printResult(arg1, COMMANDSECTIONCOUNT, arg2, blockCount);
        break;
    default:
        break;
    }
}

void chainDeleteBlockHolders(BlockHolder *head)
{
    if (head == nullptr)
    {

        return;
    }
    chainDeleteBlockHolders(head->next);
    delete head;
}

void printAll(BlockHolder &head, int blockCount)
{
    putchar('\n');
    putchar('\n');
    putchar('\n');
    putchar('\n');
    int attrCount, selecCount;

    for (int i = 0; i < blockCount; i++)
    {

        putchar('\n');
        putchar('\n');
        selecCount = head[i].countSelectors();
        print(i + 1, '\n');
        for (int j = 0; j < selecCount; j++)
        {
            print(head[i].getSelector(j).name);
            putchar(',');
            putchar(' ');
        }
        putchar('{');
        putchar('\n');
        attrCount = head[i].countAttributes();
        for (int j = 0; j < attrCount; j++)
        {
            putchar('\t');
            print(head[i].getAttr(j).name);
            putchar(':');
            putchar(' ');
            print(head[i].getAttr(j).value);
            putchar(';');
            putchar('\n');
        }
        putchar('}');
    }
}

bool readAndExecuteCommands(BlockHolder *&head, BlockHolder *&tail, int &blockCount)
{
    Str arg1, arg2;
    char commandType = NULLC;
    static int a = 0;
    a++;
    while (commandType != COMMANDEND[0])
    {
        commandType = readCommand(arg1, arg2);
        executeCommand(head, tail, blockCount, commandType, arg1, arg2);

        if (commandType == EOF)
        {
            return true;
        }
    }
    return false;
}

int main()
{
    //    Str tmp = intToStr(89);
    BlockHolder *head = nullptr;
    BlockHolder *tail;
    int blockCount = 0;
    bool endOfFile = false;
    while (!endOfFile)
    {
        if (head == nullptr)
        {
            head = new BlockHolder;
        }

        blockCount = readBlocks(head, tail, blockCount);
        endOfFile = readAndExecuteCommands(head, tail, blockCount);
    }

    //printAll(*head, blockCount);

    chainDeleteBlockHolders(head);

    return 0;
}

// !! selector and attr can inherit
// print only in str
//!! delete debugstr
// delete attr when reading data
// l references