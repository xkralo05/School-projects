/**
 * @name Rozhranie kniznice pre pracu so strings
 * @brief Prevzaté z http://www.fit.vutbr.cz/study/courses/IFJ/private/projekt/jednoduchy_interpret.zip
 * link dostupný len pre študentov VUT FIT
 * @file str.h
 * @date 24.10.2020
*/
//hlavickovy soubor pro praci s nekonecne dlouhymi retezci

#ifndef STR_H
#define STR_H

#define STR_ERROR   1
#define STR_SUCCESS 0
typedef struct
{
  char* str;		// misto pro dany retezec ukonceny znakem '\0'
  int length;		// skutecna delka retezce
  int allocSize;	// velikost alokovane pameti
} string;


int strInit(string *s);
void strFree(string *s);

void strClear(string *s);
int strAddChar(string *s1, char c);
int strAddConstChar(string *s1, const char* c);
int strCopyString(string *s1, string *s2);
int strCmpString(string *s1, string *s2);
int strCmpConstStr(string *s1, char *s2);
// Kopiruje const char do stringu
int strCopyChar(string *s1, const char* s2);

char *strGetStr(string *s);
int strGetLength(string *s);
#endif