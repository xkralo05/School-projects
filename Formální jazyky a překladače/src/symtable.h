// tabulka symbolu implementovana binarnim stromem
/**
 * @name symtable IFJ20
 * @author Timotej Ponek xponek00
 * @file symtable.h
 * @date 17.11.2020
 */

#include "str.h"
#include <stdbool.h>

#define TABLE_OK 0
#define ALLOC_ERR 1
#define TABLE_REDEF 2

typedef enum
{
    INT,
    FLOAT,
    STRING,
    FNC_NAME,
    BOOL, // potrebujem na vyhodenie erroru napr. 5 + (5==5) -> relacny error
   // BOOL, // nerobime boolthen ale potrebujeme bool na kontrolu priradenia typu 
    TYPE_ERR,
    TYPE_DEFAULT
}vType;

/**
 * Polozka stromu ktora sa moze vyskytovat iba na globalnej urovni
 * reprezentuje funkciu, jej parametre/retTypy a symboly vo fnci
 */
struct BSTreeFunction{
    int parameters;
    int rettypes;
    vType* FreturnType; //pole s uspodriadanymi return type-mi fnce
    vType* FparamType; //pole s usporiadanymi parametrami fnce
    bool isDefined; // 
};//*BSTreeFunctionPtr;


typedef union
{
    int nesting;
    struct BSTreeFunction* functionPtr;
}iNestValue;

/**
 * Polozka stromu
 */
typedef struct BSTree{
    struct BSTree* LPtr;
    struct BSTree* RPtr;
    string iName;
    vType iClass;
    iNestValue itemValue; 
}*BSTreePtr;



void BSTreeInit(BSTreePtr *Tree);

int BSTreeInsert(BSTreePtr *Tree, string* name, void* value, vType type, bool DefVar);

// zisti ci uz je polozka s danym menom v tabulke
bool BSTreeSearch(BSTreePtr *Tree, string *name);

// vracia aj ukazatel na fnci, z tej dostanem parametre/ ret types
vType BSTreeGet(BSTreePtr *Tree, string *name, iNestValue *getter);


void BSTreeDisposeTree(BSTreePtr *Tree);



// nastavenie ze je definovana budeme volat vzdy, az tam nie je natvrdo true/false
void BSTreeFuncSetDefined(BSTreePtr *Tree, string *fncName, bool value);

int BSTreeFuncAddParamType(BSTreePtr *Tree, string *fncName, vType paramType);

int BSTreeFuncAddRetType(BSTreePtr *Tree, string *fncName, vType retType);

bool BSTreeFuncIsDefined(BSTreePtr *Tree, string *fncName);

bool BSTreeFuncCheckAllDefined(BSTreePtr *Tree);


////////////////////////////////////////////////////////

/**
 * Stack implementovany pomocou linked listu aby sme nemuseli byt obmedzeny polom
 * lStack ako linked stack
 */
typedef struct lStack{
    int scopeNMB;
    struct lStack *previous;
    BSTreePtr* table;
} *stackItem;

typedef stackItem STACK;

void tStackInit (STACK*);

int tStackPush (STACK*, BSTreePtr *ptr);

BSTreePtr* tStackTop (STACK* top);


void tStackPop (STACK*);

bool tStackEmpty (STACK*);

vType BSTreeGetFromStack(string *name, iNestValue *getter, STACK *top, int* scope);

/* Funkcia ktora je uzitocnejsia ako obycajny BSTTreesearch */
bool BSTreeSearchFromStack(string *name, STACK *top, int* scope);

vType BSTreeGetType(string *name, STACK *top);
