
#include <stdio.h>
#include <stdlib.h>

#define T 8
#define STRLEN 16

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

#define COMMANDRESULTSEPERATOR " == "

#define NUMBASE 10

#define DELETENOTIFICATION "deleted"

int globalLineCounter = 0;

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
            free((this->charList));
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

void chainDeleteBlockHolders(BlockHolder *head)
{
    if (head->next == nullptr)
    {
        return;
    }
    chainDeleteBlockHolders(head->next);
    delete head;
}

int main()
{
    // BlockHolder *head = new BlockHolder;
    // BlockHolder *tail = getTail(head);
    Str arg1, arg2;
    // char commandType;
    // do
    // {
    // } while (commandType != NULLC);

    // chainDeleteBlockHolders(head);

    arg1 += 'c';
    arg1.erase();

    return 0;
}

// !!!\r
// !! selector and attr can inherit
// killNext w listach pojedyńczych
// print only in str