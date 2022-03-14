/**
 * @name Code generator IFJ20
 * @author Timotej Ponek xponek00
 * @file codegen.h
 * @date 22.11.2020
 */
#include "Scanner.h"



typedef enum{
    addS,
    subS,
    mulS,
    divFloatS,
    divIntS,
    clearsS,
    greaterS,
    lesserS,
    equalS,
    notequalS,
    greaterEqualS,
    lesserEqualS
}iType;


void StrConcatenation(TOKEN *tokenA, int label, int scope);

void BuildInPrintTerms(int counter);
void BuildInPrint(/*TOKEN *tokenA , vType type, int label, int scope*/ int counter);
void PopFrame();

void PrintBuiltInFunc();
void PrintMain ();
void PrintMain2();
void PrintEndMainJump();
void PrintEndMain();

void VarDefine(TOKEN *tokenA ,int label, int scope);
void VarAssign(TOKEN *tokenA , int label, int scope);
void VarDefineFncParam(TOKEN *tokenA , int counter);
void VarDefineID(TOKEN *tokenA, int label, int scope);
void VarDefineFNC(TOKEN *tokenA, int counter, int label, int scope);
void VarAssignFNC(TOKEN *tokenA, int counter, int label, int scope);

void FncParams(TOKEN *FncName, int counter , int numberOfParams);
void FncStart();
void FncCall(TOKEN *tokenA);
void FncJump(char* Name);
void FncBody(TOKEN *tokenA);
void FncEndBody();

void FncReturnInit(int counter);
void FncReturn(/*TOKEN *FncName,*/TOKEN *tokenA, vType type , int counter);

void printIF(int ifcounter);
void printELSE(int ifcounter);
void printELSEend(int ifcounter);
void printIFjump(int ifcounter);


void ForDeclarations(int label);
void ForJumpMostOuter(int label);
void ForLabelMostOuter(int label);
void ForStart(int label);
void printFOR(int label);
void ForAssign(int label);
void ForJump(int label);
void ForBody(int label);
void ForEnd(int label);

void popTrash();
void pushID(char** name, int label, int scope);
void pushRetValue(int counter);
void pushstring(char** name);
void pushInt(long long value);
void pushFloat(double value);

void add();
void sub();
void mul();
void divFloat();
void divInt();

void greater();
void lesser();
void equal();
void notequal();
void greaterEqual();
void lesserEqual();

//void ClearS();

typedef struct ListItem{
    string line;
    struct ListItem* next;
} *lItem;

typedef struct linkedList{
    struct ListItem* first;
    struct ListItem* lastInserted;
} *lList;

void LinkedInit(lList* list);
void LinkedInsertDEFVAR (lList *list, TOKEN *tokenA, int label, int scope );
void LinkedPrintDispose(lList *list, int label);

void LinkedInsertParams (lList *list, TOKEN *FncName, int counter, int numberOfParams );
void LinkedFncStart (lList *list);
void LinkedFncJump (lList *list, char* Name);

void LinkedPushID (lList *list, TOKEN *tokenA, int label, int scope );
void LinkedPushRetValue (lList *list, int counter);
void LinkedPushString (lList *list, char** name);
void LinkedPushInt (lList *list, long long value);
void LinkedPushFloat (lList *list, double value);
void LinkedDataStack (lList *list, iType instr);

void LinkedConcat (lList *list, TOKEN *tokenA, int label, int scope);