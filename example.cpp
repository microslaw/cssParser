#define STRLBUFFORLEN 16

#include <stdio.h>
#include <stdlib.h>

int strlen(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        i++;
    }
    return i;
}

char *addChar(char *str, int reservedSize, char a)
{
    int len = strlen(str);
    char *newStr;
    
    if (len + 2 > reservedSize)
    {
        reservedSize += STRLBUFFORLEN;
        newStr = (char *)malloc(reservedSize);
        for (int i = 0; i < len; i++)
        {
            newStr[i] = str[i];
        }
    }

    newStr[len] = a;
    newStr[len + 1] = '\0';
    free(str);
    return newStr;
}

char *addChar(char *str, char a)
{
    int len = strlen(str);
    char *newStr;

    newStr = (char *)malloc(len + 2);
    for (int i = 0; i < len; i++)
    {
        newStr[i] = str[i];
    }

    newStr[len] = a;
    newStr[len + 1] = '\0';
    free(str);
    return newStr;
}
