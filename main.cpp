
#include <stdio.h>
#include <malloc.h>

#define nullc '\0'

struct str
{
    char *str;
} typedef Str;

struct block
{
    Str * names;
    Block *next;
    Block *prev;
    Str *attr;

} typedef Block;

Block *newBlock(Block *prev, Str name, Str *attr)
{
    Block *block = (Block*)malloc(sizeof(Block));
    block->names = name;
    block->prev = prev;
    block->attr = attr;
    block->next = nullptr;
    return block;
}

void delBlock(Block *block)
{
    block->prev->next = block->next;
    block->next->prev = block->prev;
    delStr(block->name);
    free(block);
}

Str newStr(int size)
{
    Str str;
    str.str = (char*)malloc(sizeof(char) * size);
    str.str[0] = nullc;
    return str;
}


void delStr(Str str)
{
    free(str.str);
}


int main()
{

    return 0;
}