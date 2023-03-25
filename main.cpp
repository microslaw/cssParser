
#include <stdio.h>
#include <malloc.h>

#define T 8

#define NULLC '\0'
#define ENDL '\n'


#define COMMANDSTART "????"
#define COMMANDEND "****"

#define STARTBRACKET '{'
#define ENDBRACKET '}'
#define SELECTORSEPARATOR ','
#define ATTRIBUTENAMEVALUESEPARATOR ':'
#define ATTRIBUTESEPARATOR ';'




struct str
{
    char *str;
} typedef Str;

struct block
{
    Str *names;
    Block *next;
    Block *prev;
    Attr *attr;

} typedef Block;

<<<<<<< HEAD
struct attr
{
    Str *name;
    Str *value;
} typedef Attr;

Block *newBlock(Block *prev = nullptr, Str *names = nullptr, Attr *attr = nullptr)
{
    if (names == nullptr)
    {
        names = (Str *)malloc(sizeof(Str)*T);
        for (int i = 0; i < T; i++)
        {
            names[i] = newStr(1);
        }
    }
    if (attr == nullptr)
    {
        attr = newAttr(newStr(1), newStr(1));
    }

    Block *block = (Block *)malloc(sizeof(Block));
    block->names = names;
    block->prev = prev;
    block->attr = attr;
    block->next = nullptr;
    return block;
}

void delBlock(Block *block)
{
    block->prev->next = block->next;
    block->next->prev = block->prev;
    delStr(block->names);
=======
    delStr(*(block->names));
>>>>>>> 786e389fa065486ad46c9f8fef53cded05d228ec
    free(block);
}

Attr *newAttr(Str *name = nullptr, Str *value = nullptr)
{
    if (name == nullptr)
    {
        name = newStr(1);
    }
    if (value == nullptr)
    {
        value = newStr(1);
    }

    Attr *attr = (Attr *)malloc(sizeof(Attr));
    attr->name = name;
    attr->value = value;
    return attr;
}

void delAttr(Attr *attr)
{
    delStr(attr->name);
    delStr(attr->value);
    free(attr);
}

Str *newStr(int size = 0, Str* str = nullptr)
{
    Str *str;
    if(str == nullptr)
    {
        str = (Str *)malloc(sizeof(Str)*size);
    }
    str->str[0] = NULLC;
    return str;
}

void delStr(Str *str)
{
    free(str->str);
}


// returns unwanted character
// adds NULLC at the end
// if c is not NULL, it will stop reading when it finds c or NULLC
char readTill(Str buffer, char c [])
{
    int i = 0;
    while (buffer.str[i] != c && buffer.str[i] != NULLC)
    {
        buffer.str[i] = getchar();
        i++;
    }
    char ending = buffer.str[i];
    buffer.str[i] = NULLC;
    return ending;
}

void printStr(Str str)
{
    printf("%s", str.str);
}

int main()
{
    Block *block = newBlock();
    char tab []=  {SELECTORSEPARATOR, 'a', NULLC};
    readTill(block->names[0], {SELECTORSEPARATOR, 'a', NULLC});

    return 0;
}



#define NAMEENDs "\n{,"
