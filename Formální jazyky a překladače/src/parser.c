/**
 * @name Syntakticka analyza IFJ20
 * @author Timotej Ponek xponek00
 * @file parser.c
 * @date 12.11.2020
 */


#include "Error.h"
#include "parser.h"
#include "expr.h"
#include "codegen.h"


tokenValue operandA;
tokenValue operandB; 
tokenValue target;
vType typeOfOperand;


TOKEN currentToken;

BSTreePtr globalSymbols;
STACK myStack;
itemStackptr idAssignStack;

itemStackptr idStack;
lList ForList;
string dummy;
vType dummyType;


// bude pouzivana hlavne pri definicii funkcie, pre porovnanie spravnych 
// return types
string dummyFNCid;

int NBMofprint;

bool AllPathsReturn;
bool currentBlockReturned;

bool PrintFromStack;
bool inFncCall;
bool expectDefinition; // premenna vyzadujuca definiciu pri prvej casti for_decl
bool expectAssignment; // premenna vyzadujuca deklaraciu pri tretej casti for_decl

bool DefVar; // ocakavam definiciu aspon jednej premennej
bool InForLoop; // true to its name

int counterforParams;
int counterforRetTypes;
int counterID;
int nesting;
int labelNMB;
int ForID;


bool isDefinedGlobal;
bool notIfElse; //pomocna premenna pre nastavovanie ci sa z non-void funkcie viem vratit
// forward declarations

// prida do tabulky symbolov funkcie z baličku package main
int addBuiltinFnc();

int start();
int fnc_list();
int fnc();
int fnc_main();

int parameters();
int param_n();

int return_types();
int retType();
int retType_n();

int stat_list();
int stat();

int FigAD(bool isDefinedLocal);
int expressions();
int next_id(bool isDefinedLocal);

int expr();
int expr_operator();
int fnc_call();

int if_clause();
int else_clause();

int for_clause();
int for_decl(int Thislabel);
int assign_i();

//int figure_ERR();
vType type();


/**
 * vracia typ premennej na zaklade terajsieho stavu Tokenu
 * @return zodpovedajuci enum vType
 */
vType type(){
    //int result
    switch (currentToken.type){
        case KeyWord_float64:
            return FLOAT;
        case KeyWord_int:
            return INT;
        case KeyWord_string:
            return STRING;
        default:
            // chyba zle deklarovaneho datoveho typu
            return TYPE_ERR;
    }
}

int expr(){
    int parsingResult = CODE_OK;
    switch(currentToken.type)
    {
        case T_ID:
            parsingResult = Analyse();
        break;

        case T_EOL: // v go mozem mat x odriakovani

        case T_L_RBRACKET:
        case T_END_STR:
	    case T_INT19:
	    case T_INT0:
	    case T_F_FLOAT:		
	    
	    case T_FLOAT_EX_R:
        
            parsingResult = Analyse();

        break;

        case T_R_RBRACKET:
            //void funkcia
            break;
        case T_UNDERSCORE:
            // tento specialny znak nemozem feedit do expresionov
            parsingResult = SEMANTIC_ERR_ELSE;
        break;

        case T_LEX_ERR:
            parsingResult = LEXICAL_ERR;
        break;

        default:
            parsingResult = SYNTACTIC_ERR;
        break;
    }
    return parsingResult;
}

int param_n(){
    int parsingResult = CODE_OK;
    switch(currentToken.type){
        case T_COMA:
            getToken(&currentToken);
            //podpora FUNEXP
            while(currentToken.type == T_EOL){
                getToken(&currentToken);
            }

            parsingResult = parameters();
            break;

        case T_R_RBRACKET:
            break;

        case T_LEX_ERR:
            parsingResult = LEXICAL_ERR;
            break;
        
        default:
            parsingResult = SYNTACTIC_ERR;
            break;
    }
    return parsingResult;
}



int parameters(){
    int parsingResult = CODE_OK;
    switch (currentToken.type){
        case T_ID:
        {
            string id;
            strInit(&id);
            strCopyString(&id, &currentToken.value.stringValue);
            getToken(&currentToken);
            if(currentToken.type == T_LEX_ERR){
                parsingResult = LEXICAL_ERR;
            }
            else{
                vType param_type = type();
                if(currentToken.type == T_EOL){
                    parsingResult = SYNTACTIC_ERR;
                }
                else if (param_type == TYPE_ERR){
                    // vrat error pri odvodzovani typu
                    parsingResult = SEMANTIC_ERR_ELSE;
                }
                // Teraz ho vlozime k funkcii do zoznamu parametrov TODO
                else
                {
                    if (!BSTreeInsert(tStackTop(&myStack), &id, &nesting, param_type,true)){
                        if (!BSTreeFuncAddParamType(&globalSymbols, &dummy, param_type)){
                            counterforParams++;
                            getToken(&currentToken);
                            parsingResult = param_n();

                            TOKEN idName;
                            strInit(&idName.value.stringValue);
                            strCopyString(&idName.value.stringValue, &id);
                            VarDefineFncParam(&idName, counterforParams);
                            counterforParams--;
                        }
                        else
                            parsingResult = COMPILER_ERR;
                    }
                    else
                        parsingResult = COMPILER_ERR;
    
                }
                
            }
            strFree(&id);
        }
            break;
        
        case T_R_RBRACKET:
            break;

        case T_LEX_ERR:
            parsingResult = LEXICAL_ERR;
            break;

        default:
            // neocakavany znak
            parsingResult = SYNTACTIC_ERR;
            break;
    }
    return parsingResult;
}



int retType_n()
{
    int parsingResult = CODE_OK;
    switch(currentToken.type){
        case T_COMA:
            getToken(&currentToken);
            //podpora FUNEXP
            while(currentToken.type == T_EOL){
                getToken(&currentToken);
            }
            parsingResult = retType();
            break;
        case T_R_RBRACKET:
        // zavolam novy token pre {
            getToken(&currentToken);
            break;

        case T_LEX_ERR:
            parsingResult = LEXICAL_ERR;
            break;

        default:
            parsingResult = SYNTACTIC_ERR;
            break;
    }
    return parsingResult;     
}



int retType()
{
    int parsingResult = CODE_OK;
    switch (currentToken.type){
        default:
            {
            vType ret_type = type();
            if (ret_type == TYPE_ERR){
                // vrat chybu pri odvodzovani datoveho typu
                parsingResult = SYNTACTIC_ERR;//SEMANTIC_ERR_ELSE;
            }
            else{
                //vlozime ho do zoznamu navratovych typov funkcie
                counterforRetTypes++;   
                if(!BSTreeFuncAddRetType(&globalSymbols, &dummy, ret_type)){
                    getToken(&currentToken);
                    parsingResult = retType_n();

                    FncReturnInit(counterforRetTypes);
                    counterforRetTypes--;
                }
                else
                    parsingResult = COMPILER_ERR;
                
            }
            }
            break;
        case T_LEX_ERR:
            parsingResult = LEXICAL_ERR;
            break;
    }
    return parsingResult;
}



int retList()
{
    int parsingResult = CODE_OK;
    switch (currentToken.type){
        case T_R_RBRACKET:
            getToken(&currentToken);
            break;
        default:
            parsingResult = retType();
            break;
    }
    return parsingResult;
}



int return_types()
{
    int parsingResult = CODE_OK; 
    switch(currentToken.type){
        case T_L_RBRACKET:
            getToken(&currentToken);
            parsingResult = retList();

            break;
        case T_L_CBRACKET:
            // void fnc
            break;
        case T_LEX_ERR:
            parsingResult = LEXICAL_ERR;
            break;
        default:
            parsingResult = SYNTACTIC_ERR;
            break;
    }
    return parsingResult;
}



int fnc()
{
    nesting = 1;
    AllPathsReturn = false;
    currentBlockReturned = false;

    TOKEN FNC_name;
    strInit(&FNC_name.value.stringValue);
    strCopyString(&FNC_name.value.stringValue, &dummy);
    FncBody(&FNC_name);

    BSTreePtr local_tree; 
    BSTreeInit(&local_tree);
    if(tStackPush(&myStack, &local_tree))
        return COMPILER_ERR;
    // urobim lokalnu tabulku symbolov ktoru hodim na stack

    int parsingResult = CODE_OK;
    switch (currentToken.type)
    {
        case T_L_RBRACKET:
            getToken(&currentToken);
            if(currentToken.type == T_LEX_ERR){
                return LEXICAL_ERR;
            }
            else{
                parsingResult = parameters();
                if (parsingResult == CODE_OK)
                {
                    getToken(&currentToken);
                    parsingResult = return_types();
                    if (parsingResult == CODE_OK && currentToken.type == T_L_CBRACKET)
                    {
                        getToken(&currentToken);
                        if (currentToken.type == T_EOL){
                            getToken(&currentToken);
                            parsingResult = stat_list();

                            iNestValue FncPtr;
                            vType type = BSTreeGet(&globalSymbols, &FNC_name.value.stringValue, &FncPtr);

                            if(parsingResult == CODE_OK){

                                if(type == FNC_NAME && FncPtr.functionPtr->rettypes > 0){
                                    if(!AllPathsReturn){
                                        parsingResult = SEMANTIC_ERR_PARAM;
                                    }
                                }
                                
                            }
                            
                                
                        }
                        else if (currentToken.type == T_LEX_ERR)
                            parsingResult = LEXICAL_ERR;
                        else
                            parsingResult = SYNTACTIC_ERR;
                    }
                    
                }
            }
            break;

        case T_LEX_ERR:
            parsingResult = LEXICAL_ERR;
            break;

        default:
            parsingResult = SYNTACTIC_ERR;
            break;
    }

    FncEndBody();
    BSTreeDisposeTree(&local_tree);
    tStackPop(&myStack);
    nesting--;
    return parsingResult;
}




int else_clause(int label)
{

    int parsingResult = CODE_OK;

    switch(currentToken.type)
    {
        case T_EOL:
            getToken(&currentToken);
        break;

        case KeyWord_else:
            getToken(&currentToken);

            switch(currentToken.type)
            {
                case T_L_CBRACKET:
                    getToken(&currentToken);
                    
                    switch(currentToken.type)
                    {
                        case T_EOL:
                            getToken(&currentToken);
                            
                            printELSE(label);
                            BSTreePtr localTree;
                            BSTreeInit(&localTree);
                            if(!tStackPush(&myStack, &localTree)){

                                parsingResult = stat_list();

                                if(!currentBlockReturned)
                                    AllPathsReturn = false;

                                BSTreeDisposeTree(&localTree);
                                tStackPop(&myStack);
                            }
                            else
                                parsingResult = COMPILER_ERR;
                            break;

                        case  T_LEX_ERR:
                            parsingResult = LEXICAL_ERR;
                            break;

                        default:
                            parsingResult = SYNTACTIC_ERR;
                            break;
                    }
                break;

                case T_LEX_ERR:
                    parsingResult = LEXICAL_ERR;
                break;

                default:
                    parsingResult = SYNTACTIC_ERR;
                break;
            }
        break;

        case T_LEX_ERR:
            parsingResult = LEXICAL_ERR;
        break;

        default:
            parsingResult = SYNTACTIC_ERR;
        break;
    }
    return parsingResult;
}



int if_clause(){
    labelNMB++;
    int label = labelNMB;

    int parsingResult = CODE_OK;
    
    switch(currentToken.type){
        
        case T_ID:
        case T_L_RBRACKET:
        case T_END_STR:
	    case T_INT19:
	    case T_INT0:
	    case T_F_FLOAT:		
	    
	    case T_FLOAT_EX_R:
            parsingResult = expr();
            
            if (parsingResult == CODE_OK){
                

                iValue exprResult;
                vType exprType;
                itemStackTopPop(&idAssignStack, &exprResult, &exprType);

                if(exprType == STRING)
                    strFree(&exprResult.stringValue);

                if(exprType != BOOL){
                    parsingResult = SEMANTIC_ERR_TYPE;
                }
                else if(!itemStackEmpty(&idAssignStack))
                    parsingResult = SEMANTIC_ERR_PARAM;

                else
                switch(currentToken.type){
                    case T_L_CBRACKET:
                    // { EOL <stat_list>
                        getToken(&currentToken);

                        switch(currentToken.type){
                            case T_EOL:
                            {
                                // lokalna tabulka
                                BSTreePtr localTree;
                                BSTreeInit(&localTree);
                                if(!tStackPush(&myStack, &localTree)){

                                    printIF(label);
                                    getToken(&currentToken);
                                    notIfElse = false;
                                    parsingResult = stat_list();

                                    if(currentBlockReturned)
                                        AllPathsReturn = true;

                                    BSTreeDisposeTree(&localTree);
                                    tStackPop(&myStack);
                                    if (parsingResult == CODE_OK){
                                        printIFjump(label);
                                        parsingResult = else_clause(label);
                                        printELSEend(label);
                                        notIfElse = true;
                                    }
                                }
                                else
                                    parsingResult = COMPILER_ERR;                                
                            }
                            break;

                            case T_LEX_ERR:
                                parsingResult = LEXICAL_ERR;
                                break;

                            default:
                                parsingResult = SYNTACTIC_ERR;
                                break;
                        }
                        break;

                    case T_LEX_ERR:
                        parsingResult = LEXICAL_ERR;
                        break;

                    default:
                        parsingResult = SYNTACTIC_ERR;
                        break;
                }
            }
        break;

        case T_LEX_ERR:
            parsingResult = LEXICAL_ERR;
        break;

        default:
            parsingResult = SYNTACTIC_ERR;
        break;
    }
    nesting--;
    return parsingResult;
}


int for_decl(int Thislabel){
    //int Thislabel = labelNMB;

    int parsingResult = CODE_OK;
    switch(currentToken.type)
    {
        case T_UNDERSCORE:
        case T_ID:
            {
            string iterator_id;
            strInit(&iterator_id);
            strCopyString(&iterator_id,&currentToken.value.stringValue);

            //mozme poslat do stat, ta za nas urobi aj vlozenie do tabulky
            //symbolov a nemusime opakovat kod
            expectDefinition = true;
            parsingResult = stat();
            expectDefinition = false;

            if(parsingResult == CODE_OK)
            {
                switch (currentToken.type)
                {                
                    case T_SEMIC:
                        ForStart(Thislabel);
                        getToken(&currentToken);
                        parsingResult = expr();
                        if(parsingResult == CODE_OK){

                            printFOR(Thislabel);
                            iValue exprResult;
                            vType exprType;
                            itemStackTopPop(&idAssignStack, &exprResult, &exprType);

                            if(exprType == STRING)
                                strFree(&exprResult.stringValue);

                            if(exprType != BOOL){
                                parsingResult = SEMANTIC_ERR_TYPE;
                            }
                            else if(!itemStackEmpty(&idAssignStack))
                                parsingResult = SEMANTIC_ERR_PARAM;

                            else{
                                    expectDefinition = false;
                                    switch (currentToken.type)
                                    {
                                        case T_SEMIC:
                                            ForAssign(Thislabel);
                                            getToken(&currentToken);
                                            if(currentToken.type != T_L_CBRACKET){
                                                expectAssignment = true;
                                                parsingResult = stat();
                                                expectAssignment = false;
                                            }
                                            ForJump(Thislabel);
                                            break;

                                        
                                        case T_LEX_ERR:
                                            parsingResult = LEXICAL_ERR;
                                            break;
                                        default:
                                            parsingResult = SYNTACTIC_ERR;
                                            break;
                                    }   
                            }
                        }
                        break;
                    case T_LEX_ERR:
                        parsingResult = LEXICAL_ERR;
                        break;
                    default:
                        parsingResult = SYNTACTIC_ERR;
                        break;
                    }
                        
                        
                    
            }
            strFree(&iterator_id);
            }
        break;
        
        // vynechana deklaracia novej premennej
        case T_SEMIC:
            ForStart(Thislabel);
            getToken(&currentToken);
            parsingResult = expr();
            printFOR(Thislabel);

            if (parsingResult == CODE_OK)
            {
                //Kontrolujeme ci sme mali porovnanie

                iValue exprResult;
                vType exprType;
                itemStackTopPop(&idAssignStack, &exprResult, &exprType);

                if(exprType == STRING)
                    strFree(&exprResult.stringValue);

                if(exprType != BOOL){
                    parsingResult = SEMANTIC_ERR_TYPE;
                }
                else if(!itemStackEmpty(&idAssignStack))
                    parsingResult = SEMANTIC_ERR_PARAM;
                
                else
                switch (currentToken.type)
                {
                    case T_SEMIC:
                        ForAssign(Thislabel);
                        getToken(&currentToken);
                        if(currentToken.type != T_L_CBRACKET){
                            expectAssignment = true;
                            parsingResult = stat();
                            expectAssignment = false;
                        }
                        ForJump(Thislabel);
                        break;
                    case T_LEX_ERR:
                        parsingResult = LEXICAL_ERR;
                        break;
                    default:
                        parsingResult = SYNTACTIC_ERR;
                        break;
                }   
            }
            break;

        case T_LEX_ERR:
            parsingResult = LEXICAL_ERR;
            break;

        default:
            parsingResult = SYNTACTIC_ERR;
            break;
    }
    return parsingResult;
}

int for_clause(){
    labelNMB ++;
    int Thislabel = labelNMB;
    if(ForID == 0){
        InForLoop = true;
        ForID = Thislabel;
    }
    BSTreePtr localTree;
    BSTreeInit(&localTree);
    if(tStackPush(&myStack, &localTree))
        return COMPILER_ERR;

    FncStart();
    int parsingResult = CODE_OK;
    switch(currentToken.type)
    {
        case T_SEMIC:
        case T_ID:
        {
            // uz mam tabulku vyssie
            if(Thislabel == ForID){
                    //JUMP na FOR deklaracie
                    ForDeclarations(Thislabel);
            }

            parsingResult = for_decl(Thislabel);
            if (parsingResult == CODE_OK){
                labelNMB ++;

                ForBody(Thislabel);

                switch (currentToken.type)
                {
                    case T_L_CBRACKET:
                        getToken(&currentToken);

                        switch (currentToken.type)
                        {
                            case T_EOL: 
                            {
                                //vytvorim novu symbolTable pre blok for-u
                                BSTreePtr localTree;
                                BSTreeInit(&localTree);
                                if(!tStackPush(&myStack, &localTree)){

                                    getToken(&currentToken);
                                    parsingResult = stat_list();

                                    ForEnd(Thislabel);

                                    if(Thislabel == ForID){
                                        // JUMP za forove deklaracie
                                        ForJumpMostOuter(Thislabel);
                                        InForLoop = false;
                                        //JUMP na ForStart
                                        LinkedPrintDispose(&ForList, Thislabel);
                                        //Koniec vsetkoho v most outer fore
                                        ForLabelMostOuter(Thislabel);
                                    }

                                    BSTreeDisposeTree(&localTree);
                                    tStackPop(&myStack);
                                }
                                else
                                    parsingResult = COMPILER_ERR;
                            }
                                break;

                            case T_LEX_ERR:
                                parsingResult = LEXICAL_ERR;
                                break;

                            default:
                                parsingResult = SYNTACTIC_ERR;
                                break;
                        }
                    break;

                    case T_LEX_ERR:
                        parsingResult = LEXICAL_ERR;
                    break;

                    default:
                        parsingResult = SYNTACTIC_ERR;
                    break;
                }
            }
        }
        break;

        case T_LEX_ERR:
            parsingResult = LEXICAL_ERR;
        break;

        default:
            parsingResult = SYNTACTIC_ERR;
    }
    BSTreeDisposeTree(&localTree);
    tStackPop(&myStack);

    return parsingResult;
}




int next_id(bool isDefinedLocal){
    int parsingResult = CODE_OK;
    switch (currentToken.type){
        case T_UNDERSCORE:
            getToken(&currentToken);
            // potrebujem si na stack dat aj _
            itemStackPush(&idStack, (iValue*) &currentToken.value, STRING, currentToken.isID);
            parsingResult = FigAD(isDefinedLocal);

            if(parsingResult == CODE_OK){
                
                //potrebujem zo stacku popnut '_'
                if(!itemStackEmpty(&idAssignStack)){

                    popTrash();
                    itemIDStackTopPop(&idStack, &dummy);

                    //a popnut vysledok
                    iValue exprResult;
                    vType exprType;
                    itemStackTopPop(&idAssignStack, &exprResult, &exprType);

                    if(exprType == STRING){
                        strFree(&exprResult.stringValue);
                    }
                }

                else
                    parsingResult = SEMANTIC_ERR_ELSE;
            }

            break;

        case T_ID:
            counterID++;
            int scope;
            if (BSTreeSearchFromStack(&currentToken.value.stringValue, &myStack, &scope)){

                itemStackPush(&idStack, (iValue*) &currentToken.value, STRING, currentToken.isID);
                
                getToken(&currentToken);
                parsingResult = FigAD(isDefinedLocal);    
            }
            else
            {
                if(!BSTreeInsert(tStackTop(&myStack), &currentToken.value.stringValue, &nesting, TYPE_DEFAULT,false)
                    && !itemStackPush(&idStack, (iValue*) &currentToken.value, STRING, currentToken.isID)){
                    getToken(&currentToken);
                    // aspon jeden identifikator bol nedefinovany, takze povolujem definition
                    parsingResult = FigAD(false);
                }
                else
                    parsingResult = COMPILER_ERR;
            }            

            if(parsingResult == CODE_OK){

                if(!itemStackEmpty(&idAssignStack)){


                    itemIDStackTopPop(&idStack, &dummy);
                    dummyType = BSTreeGetType(&dummy, &myStack);

                    iValue exprResult;
                    vType exprType;
                    /*bool definedID =*/ itemStackTopPop(&idAssignStack, &exprResult, &exprType);

                    TOKEN identifikator;

                    iNestValue pomocna;

                    vType bolaDefinovana = BSTreeGet(tStackTop(&myStack), &dummy, &pomocna);

            //kopirovanie idcka
                    strInit(&identifikator.value.stringValue);
                    strCopyString(&identifikator.value.stringValue, &dummy);
                    
                    if((bolaDefinovana == TYPE_ERR || bolaDefinovana == TYPE_DEFAULT) && DefVar && InForLoop){
                        LinkedInsertDEFVAR(&ForList, &identifikator, pomocna.nesting, myStack->scopeNMB);
                        if(ForID != labelNMB)
                            VarAssign(&identifikator, pomocna.nesting, myStack->scopeNMB);
                    }
                    else if((bolaDefinovana == TYPE_ERR || bolaDefinovana == TYPE_DEFAULT) && DefVar){
                        VarDefine(&identifikator, pomocna.nesting, myStack->scopeNMB);
                    }
                    else{
                        BSTreeGetFromStack(&dummy, &pomocna, &myStack, &scope);
                        VarAssign(&identifikator, pomocna.nesting, scope);
                    }


                    // kontrola typov
                    if( dummyType == exprType || dummyType == TYPE_DEFAULT)
                        BSTreeInsert(tStackTop(&myStack), &dummy, &pomocna.nesting, exprType,DefVar);
                    else
                        parsingResult = SEMANTIC_ERR_TYPE;
                

                    if(exprType == STRING){
                        strFree(&exprResult.stringValue);
                    }
                }

                else
                    parsingResult = SEMANTIC_ERR_ELSE;
            }

            break;

        case T_LEX_ERR:
            parsingResult = LEXICAL_ERR;
            break;

        default:
            parsingResult = SYNTACTIC_ERR;
            break;
    }
    return parsingResult;
}



int expressions(){
    int parsingResult = CODE_OK;
    switch (currentToken.type){
        case T_COMA:
            getToken(&currentToken);
            parsingResult = expr();
            if (parsingResult == CODE_OK){
                parsingResult = expressions();
            }
            break;
        case T_EOL:
        case T_SEMIC:
        case T_L_CBRACKET:
            break;

        case T_LEX_ERR:
            parsingResult = LEXICAL_ERR;
            break;
        default:
            parsingResult = SYNTACTIC_ERR;
            break;
    }
    return parsingResult;
}



int FigAD(bool isDefinedLocal){
    int parsingResult = CODE_OK;
    switch (currentToken.type)
    {
        case T_COMA: 
            getToken(&currentToken);
            // ak je id v tabulke, potrebujem skontrolovat ci ide o priradenie/definiciu
            parsingResult = next_id(isDefinedLocal);
            break;


        case T_DEFINITION:
            if(!isDefinedLocal){
                if(!expectAssignment){
                    DefVar = true;
                    getToken(&currentToken);
                    parsingResult = expr();
                    if (parsingResult == CODE_OK){
                        // bud mam nacitany EOL alebo ','
                        parsingResult = expressions();
                    }
                }
                else //error pre for-> deklaracia namiesto priradenia
                    parsingResult = SYNTACTIC_ERR;
            }
            else
                parsingResult = SEMANTIC_ERR_UNDEF;
            break;


        case T_ASSIGN:
            if(isDefinedLocal || isDefinedGlobal){
                if(!expectDefinition){
                    getToken(&currentToken);
                    parsingResult = expr();
                    if (parsingResult == CODE_OK){
                        // bud mam nacitany EOL alebo ','
                        parsingResult = expressions();
                    }
                }
                else
                    //vo fore nemam definiciu kde ju ocakavam
                    parsingResult = SYNTACTIC_ERR;
            }
            else
                parsingResult = SEMANTIC_ERR_UNDEF;

            break;

        // ide o valanie fnce, potrebujem sa dostat o token spat
        case T_L_RBRACKET:
        {
            iValue fnc_name;
            iNestValue fncPtr;
            vType tmp;
            // zoberiem si nazov zo stacku
            itemStackTopPop (&idStack, &fnc_name, &tmp);
            inFncCall = true;
            getToken(&currentToken);
            parsingResult = expr();
            if (parsingResult == CODE_OK){

                if(currentToken.type == T_R_RBRACKET);
                    // preskoc
                else
                    parsingResult = expressions();

                int bin;

                tmp = BSTreeGetFromStack(&fnc_name.stringValue, &fncPtr, &myStack, &bin);

                if(tmp == FNC_NAME){

                    
                    int scope;
                    int counter = 0;
                    // kontrolovanie parametrov funkcie
                    int iterator = fncPtr.functionPtr->parameters;
                    iValue exprResult;
                    vType exprType;

                    TOKEN PushP;

                    iNestValue pomocna;

                    TOKEN FNC_nameToken;
                    copyiValue(&FNC_nameToken, &fnc_name, STRING);

                    if(!strCmpConstStr(&fnc_name.stringValue, "print")){
                        itemStackptr valuesToBePrinted;
                        itemStackInit(&valuesToBePrinted);
                        int ToPrint = 0;
                        
                            while(!itemStackEmpty(&idAssignStack)){
                                bool isID = itemStackTopPop(&idAssignStack, &exprResult, &exprType);
                                BuildInPrintTerms(ToPrint++);

                                itemStackPush(&valuesToBePrinted, &exprResult, exprType, isID);
                            }
                            
                            while(!itemStackEmpty(&valuesToBePrinted)){

                                bool isID = itemStackTopPop(&valuesToBePrinted, &exprResult, &exprType);
                                
                                copyiValue(&PushP, &exprResult, exprType);
                                PushP.isID = isID;
                                if(isID)
                                    BSTreeGetFromStack(&PushP.value.stringValue, &pomocna, &myStack, &scope);

                                BuildInPrint(--ToPrint);
                            }
                        FncStart();
                    }
                    else{
                        FncStart();

                        if (!iterator){
                            FncJump(FNC_nameToken.value.stringValue.str);
                        }

                        while(iterator > 0 && parsingResult == CODE_OK){
                            --iterator;                       
                            

                            if(itemStackEmpty(&idAssignStack))
                                    parsingResult = SEMANTIC_ERR_PARAM;
                            
                            else{
                                itemStackTopPop(&idAssignStack, &exprResult, &exprType);

                                FncParams(&FNC_nameToken, counter++, fncPtr.functionPtr->parameters);
                            
                                if(fncPtr.functionPtr->FparamType[iterator] != exprType){
                                    parsingResult = SEMANTIC_ERR_PARAM;
                                }
                            }
                            
                        }
                        if(!itemStackEmpty(&idAssignStack)){
                            parsingResult = SEMANTIC_ERR_PARAM;
                        }

                        copyiValue(&PushP, &fnc_name, STRING);    
                    }
                }
                else
                    parsingResult = SEMANTIC_ERR_UNDEF;
                
            }
        }
        break;

        case T_LEX_ERR:
            parsingResult = LEXICAL_ERR;
            break;
        default:
            parsingResult = SYNTACTIC_ERR;
            break;
    }
    return parsingResult;
}



int stat(){
    int parsingResult = CODE_OK;
    switch(currentToken.type)
    {
        case T_UNDERSCORE:
            // daj na stack '_'
            itemStackPush(&idStack, (iValue*) &currentToken.value, STRING, currentToken.isID);
            getToken(&currentToken);

            // AK ide o priradenie/definiciu
            if(currentToken.type != T_EOL){
                
                // Go podporuje '_ = 1'
                parsingResult = FigAD(true);

                if(parsingResult == CODE_OK){
                    
                    //potrebujem zo stacku popnut '_'
                    if(!itemStackEmpty(&idAssignStack)){

                        itemIDStackTopPop(&idStack, &dummy);

                        popTrash();
                        //a popnut vysledok
                        iValue exprResult;
                        vType exprType;
                        itemStackTopPop(&idAssignStack, &exprResult, &exprType);

                        
                        if(exprType == STRING){
                            strFree(&exprResult.stringValue);
                        }
                    }

                    else
                        parsingResult = SEMANTIC_ERR_ELSE;
                }
                
            }
    
            else
                parsingResult = SEMANTIC_ERR_UNDEF;
            break;

        case T_ID:
            {
            // vyhladat v tabulke symbolov, ak nie je tak ho tam vlozit?
            int scope;
            counterID ++;
            

            bool isDefinedLocal = BSTreeSearch(myStack->table, &currentToken.value.stringValue);
            isDefinedGlobal = BSTreeSearchFromStack(&currentToken.value.stringValue, &myStack, &scope);

            if(!isDefinedLocal)
            {
                if (BSTreeInsert(tStackTop(&myStack), &currentToken.value.stringValue, &nesting, TYPE_DEFAULT, false) 
                    || itemStackPush(&idStack, (iValue*) &currentToken.value, STRING, currentToken.isID))
                    parsingResult = COMPILER_ERR;
            }
            else
                itemStackPush(&idStack, (iValue*) &currentToken.value, STRING, true);

            if(parsingResult != COMPILER_ERR){
                getToken(&currentToken);
                // AK ide o priradenie
                if(currentToken.type == T_DEFINITION && isDefinedLocal)
                    parsingResult = SEMANTIC_ERR_UNDEF;

                else if(currentToken.type != T_EOL){
                    parsingResult = FigAD(isDefinedLocal);

                    if(parsingResult == CODE_OK){
                        
                        // porovnavam typy vyrazov s typmi priradovanych identifikatorov
                        if(!itemStackEmpty(&idAssignStack)){
                            
                            itemIDStackTopPop(&idStack, &dummy);
                            

                            iValue exprResult;
                            vType exprType;
                            /*bool definedID = */itemStackTopPop(&idAssignStack, &exprResult, &exprType);

                            TOKEN identifikator;
                            

                            iNestValue pomocna;

                            vType bolaDefinovana = BSTreeGet(tStackTop(&myStack), &dummy, &pomocna);

                            //kopirovanie idcka
                            strInit(&identifikator.value.stringValue);
                            strCopyString(&identifikator.value.stringValue, &dummy);
                            
                        
                            if((bolaDefinovana == TYPE_ERR || bolaDefinovana == TYPE_DEFAULT) && DefVar && InForLoop){
                                LinkedInsertDEFVAR(&ForList, &identifikator, pomocna.nesting, myStack->scopeNMB);
                                if(ForID != labelNMB)
                                    VarAssign(&identifikator, pomocna.nesting, myStack->scopeNMB);
                            }
                            else if((bolaDefinovana == TYPE_ERR || bolaDefinovana == TYPE_DEFAULT) && DefVar){
                                VarDefine(&identifikator, pomocna.nesting, myStack->scopeNMB);
                            }
                            else{
                                BSTreeGetFromStack(&dummy, &pomocna, &myStack, &scope);
                                VarAssign(&identifikator, pomocna.nesting, scope);
                            }
                                
                                
                            int insertResult = BSTreeInsert(tStackTop(&myStack), &dummy, &pomocna.nesting, exprType, DefVar);
                            if( insertResult == TABLE_OK){
                                ;
                            }

                            else if(insertResult == TABLE_REDEF)
                                parsingResult = SEMANTIC_ERR_TYPE;

                            else
                                parsingResult = COMPILER_ERR;


                            if(!itemStackEmpty(&idAssignStack)){
                                if(inFncCall)
                                    parsingResult = SEMANTIC_ERR_PARAM;
                                else
                                    parsingResult = SEMANTIC_ERR_ELSE;
                            }
                            
                            DefVar = false;
                            inFncCall = false;

                            if(exprType == STRING){
                                strFree(&exprResult.stringValue);
                            }
                        }
                        // premenna bez hodnoty ktora by sa jej priradila
                        else if(inFncCall){
                            if(itemStackEmpty(&idAssignStack) && itemStackEmpty(&idStack))
                                inFncCall = false;
                            else
                                parsingResult = SEMANTIC_ERR_PARAM;
                        }
                            
                        else
                            parsingResult = SEMANTIC_ERR_ELSE;


                        counterID = 0;
                    }
                }
                    //vlozit?        
                else
                    parsingResult = SEMANTIC_ERR_UNDEF;
                
            }

            }
        break;

        case KeyWord_if:
            getToken(&currentToken);
            parsingResult = if_clause();

        break;

        case KeyWord_for:
            getToken(&currentToken);
            parsingResult = for_clause();
        break;

        case KeyWord_return:
            getToken(&currentToken);
            //non void funkcia
            if(currentToken.type != T_EOL){
                parsingResult = expr();
            }

            if (parsingResult == CODE_OK){
                //vracia 2 a viac parametrov
                if(currentToken.type != T_EOL){
                    parsingResult = expressions();
                }

                if (parsingResult == CODE_OK){
                    iNestValue fncPtr;
                    vType tmp;

                    tmp = BSTreeGet(&globalSymbols, &dummyFNCid, &fncPtr);
                    
                    // na idAssignStacku mam vyrazy, kontrolujem
                    // kontrolovanie parametrov funkcie
                    if(tmp == FNC_NAME){
                        int iterator = fncPtr.functionPtr->rettypes;
                        iValue exprResult;
                        vType exprType;

                        while(iterator > 0 && parsingResult == CODE_OK){
                            --iterator;

                            // return ma menej termov ako ocakavam
                            if(itemStackEmpty(&idAssignStack))
                                parsingResult = SEMANTIC_ERR_PARAM;

                            else{
                                TOKEN hodnota;

                                hodnota.isID = itemStackTopPop(&idAssignStack, &exprResult, &exprType);


                                copyiValue(&hodnota, &exprResult, exprType);

                                FncReturn(&hodnota, exprType, iterator);

                                currentBlockReturned = true;

                                if(fncPtr.functionPtr->FreturnType[iterator] != exprType){
                                    parsingResult = SEMANTIC_ERR_PARAM;
                                }
                            }

                        }
                        // return ma viac termov ako ocakavam
                        if(!itemStackEmpty(&idAssignStack))
                            parsingResult = SEMANTIC_ERR_PARAM;
                    }
                    else
                        parsingResult = SEMANTIC_ERR_PARAM;
                }
            }

        break;

        case T_L_CBRACKET:
        {
            labelNMB++;
            BSTreePtr localTree;
            BSTreeInit(&localTree);
            if(!tStackPush(&myStack, &localTree)){
                //Safecheck
                int tmp = nesting;
                nesting += labelNMB;
                getToken(&currentToken);
                parsingResult = stat_list();

                nesting = tmp;
                BSTreeDisposeTree(&localTree);
                tStackPop(&myStack);
            }
            else
                parsingResult = COMPILER_ERR;            
        }
        break;


        // TU zacina expresion
        case T_END_STR:
	    case T_INT19:
	    case T_INT0:
	    case T_F_FLOAT:		
	    
	    case T_FLOAT_EX_R:
        
            parsingResult = expr();
        break;
        
        default:
            break; // tu sa nedostanem lebo stat_list ma ochrani
    }
    return parsingResult;
}



int stat_list(){
    // povolene odriadkovanie medzi jednotlivymi stat
    while(currentToken.type == T_EOL){
        getToken(&currentToken);
    }

    int parsingResult = CODE_OK;
    switch(currentToken.type){
        case T_L_CBRACKET:
        case T_UNDERSCORE:
        case T_ID:
            parsingResult = stat();
            if (parsingResult == CODE_OK){
                getToken(&currentToken);
                parsingResult = stat_list();
            }
        break;

        case KeyWord_if:
        {
            int tmp = nesting;
            nesting += labelNMB;
            parsingResult = stat();
            if (parsingResult == CODE_OK){
                nesting = tmp;
                parsingResult = stat_list();
            }
        } 
        break;

        case KeyWord_for:
        {
            int tmp = nesting;
            nesting += labelNMB;
            parsingResult = stat();
            if (parsingResult == CODE_OK){
                getToken(&currentToken);
                nesting = tmp;
                parsingResult = stat_list();
            }
        }
        break;

        case T_R_CBRACKET:
                getToken(&currentToken);
        break;

        // mozem returnovat expr, snad
        case KeyWord_return:
            parsingResult = stat();

            if (parsingResult == CODE_OK)
            {
                getToken(&currentToken);
                if(currentBlockReturned && notIfElse){
                    AllPathsReturn = true;
                }
                parsingResult = stat_list();
            }
        break;


        case T_END_STR:
	    case T_INT19:
	    case T_INT0:
	    case T_F_FLOAT:		
	    
	    case T_FLOAT_EX_R:
        
            parsingResult = stat();

            if (parsingResult == CODE_OK)
            {
                getToken(&currentToken);
                parsingResult = stat_list();
            }
        break;

        case T_LEX_ERR:
            parsingResult = LEXICAL_ERR;
        break;

        default:
            //necakany znak, chyba
            parsingResult = SYNTACTIC_ERR;
        break;
    }
    return parsingResult;
}



int fnc_list(){
    int parsingResult = CODE_OK;
    //pasca
    while (currentToken.type == T_EOL){ 
        getToken(&currentToken);
    }
    switch (currentToken.type)
    {
        case KeyWord_func:
            getToken(&currentToken);
            if (currentToken.type == T_ID){
                if (!strCmpConstStr(&currentToken.value.stringValue, "main")){
                    return CODE_OK; // potrebujem sa vratit MAIN
                };
                // vlozit id funkcie
                if (!BSTreeSearch(&globalSymbols, &currentToken.value.stringValue)){
                    if(!BSTreeInsert(&globalSymbols, &currentToken.value.stringValue,
                                &currentToken.value.stringValue, FNC_NAME, true))
                    {
                        BSTreeFuncSetDefined(&globalSymbols, &currentToken.value.stringValue, true);
                        // ulozenie nazvu funkcie pre buduce pridanie parametrov/ navratovych typov 
                        // do globalnej tabulky symbolov
                        if(!strCopyString(&dummy, &currentToken.value.stringValue)){

                            // zatial len pre return, TODO
                            strCopyString(&dummyFNCid, &currentToken.value.stringValue);

                            //tabulka symbolov pre danu fnci

                            getToken(&currentToken);
                            parsingResult = fnc();
                            if (parsingResult == CODE_OK){
                                parsingResult = fnc_list();
                            } 
                        }
                        else
                            parsingResult = COMPILER_ERR;
                    }
                    else
                        parsingResult = COMPILER_ERR;
                }
                //pokus o redefiniciu funkcie (tymto riadkom zaroven zakazujeme redefiniciu
                // buildin funkcii, co vsak v Go mozne je)
                else
                    parsingResult = SEMANTIC_ERR_UNDEF;
            }
            else if (currentToken.type == T_LEX_ERR)
                parsingResult = LEXICAL_ERR;
            else
                parsingResult = SYNTACTIC_ERR;//error
        break;

        // ziadne funkcie definovane pred/za mainom
        case T_EOF:
            //parsingResult = SEMANTIC_ERR_UNDEF;
            break;

        case T_LEX_ERR:
            parsingResult = LEXICAL_ERR;
            break;

        default:
            parsingResult = SYNTACTIC_ERR;
            break;
    }
    return parsingResult;
}



int fnc_main(){
    nesting = 1;
    PrintMain2();
    // tabulka pre fncMain
    BSTreePtr localTree;
    BSTreeInit(&localTree);
    if(tStackPush(&myStack, &localTree) || BSTreeInsert(&globalSymbols, &currentToken.value.stringValue,
        &currentToken.value.stringValue, FNC_NAME, true))
        return COMPILER_ERR;

    BSTreeFuncSetDefined(&globalSymbols, &currentToken.value.stringValue, true);

    strCopyString(&dummyFNCid, &currentToken.value.stringValue);

    int parsingResult = CODE_OK;
    // viem ze id je main
    getToken(&currentToken);
    switch (currentToken.type)
    {
        case T_L_RBRACKET:
            getToken(&currentToken);

            switch (currentToken.type)
            {
                case T_R_RBRACKET:
                    getToken(&currentToken);

                    //Main_Begin:
                    switch (currentToken.type)
                    {
                        // func main ()
                        case T_L_CBRACKET:
                            getToken(&currentToken);

                            switch (currentToken.type)
                            {
                                case T_EOL:
                                    getToken(&currentToken);
                                    parsingResult = stat_list();
                                    break;
                                case T_LEX_ERR:
                                    parsingResult = LEXICAL_ERR;
                                    break;
                                default:
                                    parsingResult = SYNTACTIC_ERR;
                                    break;
                            }
                        break;
                        
                        // func main () ()
                        case T_L_RBRACKET:
                            getToken(&currentToken);

                            switch (currentToken.type)
                            {
                                case T_R_RBRACKET:
                                    getToken(&currentToken);
                                    //goto Main_Begin;
                                    switch(currentToken.type){
                                        case T_L_CBRACKET:
                                            getToken(&currentToken);

                                            switch (currentToken.type)
                                            {
                                                case T_EOL:
                                                    getToken(&currentToken);
                                                    parsingResult = stat_list();
                                                    break;
                                                case T_LEX_ERR:
                                                    parsingResult = LEXICAL_ERR;
                                                    break;
                                                default:
                                                    parsingResult = SYNTACTIC_ERR;
                                                    break;
                                            }
                                        break;

                                        case T_LEX_ERR:
                                            parsingResult = LEXICAL_ERR;
                                        break;

                                        default:
                                            parsingResult = SYNTACTIC_ERR;
                                        break;

                                    }
                                    break;

                                case T_ID: // podla Go
                                case KeyWord_float64:
                                case KeyWord_int:
                                case KeyWord_string:
                                    parsingResult = SEMANTIC_ERR_PARAM;
                                    break;

                                case T_LEX_ERR:
                                    parsingResult = LEXICAL_ERR;
                                    break;

                                default:
                                    parsingResult = SYNTACTIC_ERR;
                                    break;
                            }
                        break;

                        case T_ID: // podla Go
                        case KeyWord_float64:
                        case KeyWord_int:
                        case KeyWord_string:
                            parsingResult = SEMANTIC_ERR_PARAM;
                            break;

                        case T_LEX_ERR:
                            parsingResult = LEXICAL_ERR;
                        break;

                        default:
                            parsingResult = SYNTACTIC_ERR;
                        break;
                    }
                break;
                
                case T_ID:

                case KeyWord_float64://podla Go
                case KeyWord_int:
                case KeyWord_string:
                    parsingResult = SEMANTIC_ERR_PARAM;
                break;

                case T_LEX_ERR:
                    parsingResult = LEXICAL_ERR;
                break;

                default:
                    parsingResult = SYNTACTIC_ERR;
                break;
            }
        break;

        case T_LEX_ERR:
            parsingResult = LEXICAL_ERR;
        break;

        default:
            parsingResult = SYNTACTIC_ERR;
        break;
    }
    BSTreeDisposeTree(&localTree);
    tStackPop(&myStack);
    nesting--;
    return parsingResult;
}



int start()
{
    DefVar = false;
    counterforParams = 0;
    counterforRetTypes = 0;
    isDefinedGlobal = false;
    expectAssignment = false;
    expectDefinition = false;
    notIfElse = true;
    nesting = 0;
    labelNMB = 0;
    ForID = 0;

	int parsingResult = CODE_OK;
    // preskakuj prazdne riadky
    while (currentToken.type == T_EOL && currentToken.type != T_LEX_ERR){ 
        getToken(&currentToken);
    }

	switch (currentToken.type)
    {
        case KeyWord_package:
            getToken(&currentToken);
            if(currentToken.type == T_ID && !strCmpConstStr(&currentToken.value.stringValue, "main")){
                getToken(&currentToken);

                if(currentToken.type == T_EOL){
                    parsingResult = fnc_list();
                    if (parsingResult == CODE_OK){
                        
                        while (currentToken.type == T_EOL){ 
                            getToken(&currentToken);
                        }

                        
                        if(currentToken.type != T_EOF){
                            parsingResult = fnc_main();
                            PrintEndMainJump();
                            
                            if(parsingResult == CODE_OK){

                                //funkcie definovane za mainom
                                parsingResult = fnc_list();

                                if(parsingResult == CODE_OK){

                                    while (currentToken.type == T_EOL){ 
                                        getToken(&currentToken);
                                    }
                                    if (currentToken.type == T_EOF){
                                        CleanToken(&currentToken);
                                        parsingResult = CODE_OK;
                                    }
                                    else if(currentToken.type == T_LEX_ERR)
                                        parsingResult = LEXICAL_ERR;
                                    else{
                                        CleanToken(&currentToken);
                                        // nespravne ukonceny subor, main nie je posledna funkcia
                                        parsingResult = SYNTACTIC_ERR;
                                    }
                                }                            
                            }
                        }
                        else{
                            parsingResult = SEMANTIC_ERR_UNDEF;
                        }
                        
                    }
                }
                else if (currentToken.type == T_LEX_ERR){
                    parsingResult = LEXICAL_ERR;
                }
                else
                    parsingResult = SYNTACTIC_ERR;
            }

            else if (currentToken.type == T_LEX_ERR){
                parsingResult = LEXICAL_ERR;
            }
            else
                parsingResult = SYNTACTIC_ERR;
        break;
        case T_LEX_ERR:
            parsingResult = LEXICAL_ERR;
            break;

        default:
            parsingResult = SYNTACTIC_ERR;
            break;
    }
    if(!BSTreeFuncCheckAllDefined(&globalSymbols)){
       parsingResult = SEMANTIC_ERR_UNDEF;
    }
    PrintEndMain();
    CleanToken(&currentToken);
    BSTreeDisposeTree(&globalSymbols);
    tStackPop(&myStack);
    strFree(&dummy);
    return parsingResult;
}



int parse()
{
    NBMofprint = 0;
    PrintFromStack = false;
    counterID = 0;
    BSTreeInit(&globalSymbols);
    tStackInit(&myStack);
    itemStackInit(&idStack);
    LinkedInit(&ForList);

    // check
    tStackPush(&myStack, &globalSymbols);
    strInit(&dummy);
    strInit(&dummyFNCid);
    addBuiltinFnc();


    // inicializacia tokenu
    currentToken.value.integerValue = 0;
    getToken(&currentToken);

    inFncCall = false;
    expectDefinition = false;
    printf(".IFJcode20\n");
    PrintMain();
    PrintBuiltInFunc();

	if (currentToken.type == T_LEX_ERR){
        CleanToken(&currentToken);
		return LEXICAL_ERR;
    }
	else
		return start();
}

/**
 * Funkcia prida vsetky vstavane fncie do globalnej tabulky symbolov
 * nemozem vynechat, lebo v GO mozem prekrit buildin fnci premennou,
 * a chceme byt GO friendly
 */
int addBuiltinFnc(){
    strCopyChar(&dummy, "inputs");
    // nemozem pouzit NULL asi iba pre fncie, lebo mi zlyha implementacia
    BSTreeInsert(&globalSymbols, &dummy, &dummy, FNC_NAME,true);
    BSTreeFuncAddRetType(&globalSymbols, &dummy, STRING);
    BSTreeFuncAddRetType(&globalSymbols, &dummy, INT);
    BSTreeFuncSetDefined(&globalSymbols, &dummy, true);

    strCopyChar(&dummy, "inputi");
    BSTreeInsert(&globalSymbols, &dummy, &dummy, FNC_NAME,true);
    BSTreeFuncAddRetType(&globalSymbols, &dummy, INT);
    BSTreeFuncAddRetType(&globalSymbols, &dummy, INT);
    BSTreeFuncSetDefined(&globalSymbols, &dummy, true);

    strCopyChar(&dummy, "inputf");
    BSTreeInsert(&globalSymbols, &dummy, &dummy, FNC_NAME,true);
    BSTreeFuncAddRetType(&globalSymbols, &dummy, FLOAT);
    BSTreeFuncAddRetType(&globalSymbols, &dummy, INT);
    BSTreeFuncSetDefined(&globalSymbols, &dummy, true);

    strCopyChar(&dummy, "print");
    BSTreeInsert(&globalSymbols, &dummy, &dummy, FNC_NAME,true);
    BSTreeFuncSetDefined(&globalSymbols, &dummy, true);

    strCopyChar(&dummy, "int2float");
    BSTreeInsert(&globalSymbols, &dummy, &dummy, FNC_NAME,true);
    BSTreeFuncAddParamType(&globalSymbols, &dummy, INT);
    BSTreeFuncAddRetType(&globalSymbols, &dummy, FLOAT);
    BSTreeFuncSetDefined(&globalSymbols, &dummy, true);
    
    strCopyChar(&dummy, "float2int");
    BSTreeInsert(&globalSymbols, &dummy, &dummy, FNC_NAME,true);
    BSTreeFuncAddParamType(&globalSymbols, &dummy, FLOAT);
    BSTreeFuncAddRetType(&globalSymbols, &dummy, INT);
    BSTreeFuncSetDefined(&globalSymbols, &dummy, true);

    strCopyChar(&dummy, "len");
    BSTreeInsert(&globalSymbols, &dummy, &dummy, FNC_NAME,true);
    BSTreeFuncAddParamType(&globalSymbols, &dummy, STRING);
    BSTreeFuncAddRetType(&globalSymbols, &dummy, INT);
    BSTreeFuncSetDefined(&globalSymbols, &dummy, true);

    strCopyChar(&dummy, "substr");
    BSTreeInsert(&globalSymbols, &dummy, &dummy, FNC_NAME,true);
    BSTreeFuncAddParamType(&globalSymbols, &dummy, STRING);
    BSTreeFuncAddParamType(&globalSymbols, &dummy, INT);
    BSTreeFuncAddParamType(&globalSymbols, &dummy, INT);
    BSTreeFuncAddRetType(&globalSymbols, &dummy, STRING);
    BSTreeFuncAddRetType(&globalSymbols, &dummy, INT);
    BSTreeFuncSetDefined(&globalSymbols, &dummy, true);

    strCopyChar(&dummy, "ord");
    BSTreeInsert(&globalSymbols, &dummy, &dummy, FNC_NAME,true);
    BSTreeFuncAddParamType(&globalSymbols, &dummy, STRING);
    BSTreeFuncAddParamType(&globalSymbols, &dummy, INT);
    BSTreeFuncAddRetType(&globalSymbols, &dummy, INT);
    BSTreeFuncAddRetType(&globalSymbols, &dummy, INT);
    BSTreeFuncSetDefined(&globalSymbols, &dummy, true);

    strCopyChar(&dummy, "chr");
    BSTreeInsert(&globalSymbols, &dummy, &dummy, FNC_NAME,true);
    BSTreeFuncAddParamType(&globalSymbols, &dummy, INT);
    BSTreeFuncAddRetType(&globalSymbols, &dummy, STRING);
    BSTreeFuncAddRetType(&globalSymbols, &dummy, INT);
    BSTreeFuncSetDefined(&globalSymbols, &dummy, true);

    // vlozena kvoli povoleniu prazdneho returnu z main
    strCopyChar(&dummy, "main");
    BSTreeInsert(&globalSymbols, &dummy, &dummy, FNC_NAME,true);
    BSTreeFuncSetDefined(&globalSymbols, &dummy, true);

    return 0;
}