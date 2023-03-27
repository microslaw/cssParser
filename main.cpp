
#include <stdio.h>
#include <malloc.h>

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

struct Str
{
    char *charList;
};

struct Attr
{
    Str name;
    Str value;
};

struct Block
{
    Block *next;
    Block *prev;

    Str *selectors;
    int selectorCount;

    Attr *attr;
    int attrCount;
};

// returns new string
// size excludes NULLC
Str newStr(int size = 0, Str *str = nullptr)
{
    Str newString;
    if (str == nullptr)
    {
        if (size == 0)
        {
            newString.charList = nullptr;
            return newString;
        }
        newString.charList = (char *)malloc(sizeof(Str) * (size + 1));
    }
    newString.charList[size] = NULLC;
    return newString;
}

void delStr(Str string)
{
    free(string.charList);
}

Str *newStrList(int length)
{
    Str *newStringList;
    newStringList = (Str *)malloc(sizeof(Str) * length);
    for (int i = 0; i < length; i++)
    {
        newStringList[i] = newStr();
    }
    return newStringList;
}

void delStrList(Str *stringList, int length)
{
    for (int i = 0; i < length; i++)
    {

        delStr(stringList[i]);
    }
    free(stringList);
}

Attr *newAttrList(int size)
{
    Attr *attrList = (Attr *)malloc(sizeof(Attr) * size);
    for (int i = 0; i < size; i++)
    {
        Attr attribute;
        attribute.name = newStr();
        attribute.value = newStr();
        attrList[i] = attribute;
    }
    return attrList;
}

void delAttrList(Attr *attr, int size)
{
    for (int i = 0; i < size; i++)
    {
        delStr(attr[i].name);
        delStr(attr[i].value);
    }
    free(attr);
}

Block *newBlock(Block *prev = nullptr, int selectorCount = T, int attrCount = T)
{
    Block *newBlock = (Block *)malloc(sizeof(Block));

    newBlock->prev = prev;
    newBlock->next = nullptr;

    if (prev != nullptr)
    {
        prev->next = newBlock;
    }

    Str *selectors = newStrList(selectorCount);
    newBlock->selectors = selectors;
    newBlock->selectorCount = selectorCount;

    Attr *attr = newAttrList(attrCount);
    newBlock->attr = attr;
    newBlock->attrCount = attrCount;

    newBlock->attr = attr;
    return newBlock;
}

void delBlock(Block *block)
{
    block->prev->next = block->next;
    block->next->prev = block->prev;
    delStrList(block->selectors, block->selectorCount);
    delAttrList(block->attr, block->attrCount);
    free(block);
}

// returns unwanted character
// adds NULLC at the end
// if c is not NULL, it will stop reading when it finds c or NULLC
// does not read \t
char readTill(Str &buffer, char endList[])
{
    int i = 0;
    int j = 0;
    char ch = getchar();
    while (ch != NULLC)
    {
        while (endList[j] != NULLC)
        {
            if (ch == endList[j])
            {
                buffer.charList[i] = NULLC;
                return ch;
            }
        }
        buffer.charList[i] = ch;
        if (ch != '\t')
        {
            i++;
        }

        ch = getchar();
    }
    return NULLC;
}

char readTill(Str buffer, char endChar)
{
    int i = 0;
    int j = 0;
    char ch = getchar();
    while (ch != NULLC)
    {
        if (ch == endChar)
        {
            buffer.charList[i] = NULLC;
            return ch;
        }
        buffer.charList[i] = ch;
        if (ch != '\t')
        {
            i++;
        }
        ch = getchar();
    }
    return NULLC;
}

void readSelectors(Block *block)
{
    int i = 0;
    char ch = NULLC;
    while (ch != STARTBRACKET)
    {
        ch = readTill(block->selectors[i], SELECTORENDs);
        i++;
    }
    block->selectorCount = i;
}

void readAttributes(Block *block)
{
    int i = 0;
    char ch = NULLC;
    while (ch != ENDBRACKET)
    {
        ch = readTill((block->attr[i]).name, ATTRIBUTENAMEVALSEPARATOR);
        ch = readTill((block->attr[i]).value, ATTRIBUTESEPARATOR);
        i++;
    }
    block->attrCount = i;
}

void printStr(Str str)
{
    int i = 0;
    while (str.charList[i] != NULLC)
    {
        printf("%c", str.charList[i]);
        i++;
    }
}

int main()
{
    Block *block = newBlock();
    readSelectors(block);
    readAttributes(block);

    delBlock(block);

    return 0;
}
