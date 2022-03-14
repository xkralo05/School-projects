/**
 * @name Analyza vyrazov header IFJ20
 * @author Timotej Ponek xponek00, Marek Valko xvalko11
 * @file expr.c
 * @date 20.11.2020
 */
#include <stdbool.h>
#include "symtable.h"
#include "Scanner.h"

typedef union
{
	long long integerValue;
	double floatValue;
	string stringValue;

    struct BSTreeFunction* functionPtr;
}iValue;


int copyTokenVal(TOKEN* target, TOKEN* source);

int Analyse();

// tStack ako token stack

typedef struct tStack{
    TOKEN stackedToken;
    struct tStack* next;
} *tStackptr;


void tokenStackInit (tStackptr*);

int tokenStackPush (tStackptr*, TOKEN*);

void tokenStackTopPop (tStackptr* top, TOKEN*);


void tokenStackPop(tStackptr* top);

bool tokenStackEmpty (tStackptr*);


////////////////////////////////////////

typedef struct itemStack{
    iValue itemValue;
    bool isID;
    vType itemType;
    struct itemStack* next;
} *itemStackptr;


int copyiValue(TOKEN* target, iValue* source, vType type);

void itemStackInit (itemStackptr*);

int itemStackPush (itemStackptr* top, iValue* itemVal, vType itemType, bool helper);

bool itemStackTopPop (itemStackptr* top, iValue* itemVal, vType* itemType);

void itemStackPop(itemStackptr* top);

void itemIDStackTopPop (itemStackptr* top, string* id);


bool itemStackEmpty (itemStackptr*);