/**
 * @name Analyza vyrazov IFJ20
 * @author Timotej Ponek xponek00, Marek Valko xvalko11
 * @file expr.c
 * @date 20.11.2020
 */

#include <malloc.h>
#include "expr.h"
#include "Scanner.h"
#include "parser.h"
#include "codegen.h"
#include "Error.h"



extern TOKEN currentToken;

extern BSTreePtr globalSymbols;
extern STACK myStack;
extern itemStackptr idAssignStack;
extern string dummy;

extern itemStackptr idStack;
extern bool PrintFromStack;


extern bool inFncCall;
extern bool InForLoop;
extern bool DefVar;

extern lList ForList;

extern int nesting;
extern int labelNMB;
extern int ForID;

extern bool inNewScope;

int AnalyseExpr();
void DeriveFromIds();

tStackptr exprStack;

TOKEN semanticToken;

int derivehelper;



int Analyse(){
    derivehelper = 0;
    tokenStackInit(&exprStack);

    //itemStackInit(&idAssignStack);
    int analysisResult = CODE_OK;

    semanticToken.type = currentToken.type;
    copyTokenVal(&semanticToken, &currentToken);

    analysisResult = AnalyseExpr();

    // TODO skontrolovat ci je prazdny stack

    currentToken.isID = false; // safety check
    currentToken.type = semanticToken.type;
    copyTokenVal(&currentToken, &semanticToken);

    return analysisResult;
}




int AnalyseExpr( ){
    iNestValue NestingOfId;
    int scope;
    int analysisResult = CODE_OK;
    tToken lastType = T_START;
    bool isComparison = false;


    brackets:
    switch(semanticToken.type){

        /////////////////////////////////////////////////////////////////////////
        ////////////////////////////////STRING///////////////////////////////////
        /////////////////////////////////////////////////////////////////////////
        
        case T_END_STR:
            //NBMofprint++;
            if(DefVar && InForLoop){
                LinkedPushString(&ForList, &semanticToken.value.stringValue.str);
                if(ForID != labelNMB)
                    pushstring(&semanticToken.value.stringValue.str);
            }
            else
                pushstring(&semanticToken.value.stringValue.str);

            tokenStackPush(&exprStack, &semanticToken);
            lastType = semanticToken.type;
            getToken(&semanticToken);

            do{
                STR_expr:
                switch(semanticToken.type)
                {

                    case T_GREATER:
                    case T_GREATER_E:
                    case T_LESSER:
                    case T_LESSER_E:
                    case T_NOT_EQ:
                    case T_EQUAL:
                        if(isComparison){
                            analysisResult = SEMANTIC_ERR_TYPE; // v Go vybehne tento error
                        }
                        else if (lastType == T_END_STR){
                            tokenStackPush(&exprStack, &semanticToken);
                            lastType = semanticToken.type;
                            getToken(&semanticToken);
                            isComparison = true;
                        }
                        else
                            analysisResult = SYNTACTIC_ERR;
                    break;



                    case T_ADD:

                        if(lastType == T_END_STR ){
                            tokenStackPush(&exprStack, &semanticToken);
                            lastType = semanticToken.type;
                            getToken(&semanticToken);
                        }
                        else if(lastType == T_BOOL)
                        {
                            analysisResult = SEMANTIC_ERR_TYPE;
                        }
                        else
                            analysisResult = SYNTACTIC_ERR;
                        break;



                    case T_ID:
                    {
                        if(lastType == T_ADD || lastType == T_L_RBRACKET || lastType == T_GREATER || lastType == T_GREATER_E
                            || lastType == T_LESSER || lastType == T_LESSER_E || lastType == T_NOT_EQ || lastType == T_EQUAL)
                        {
                            vType idType = BSTreeGetFromStack(&semanticToken.value.stringValue, &NestingOfId, &myStack, &scope);
                            if(idType == STRING){

                                // TODO nejako checkovat ktory type to je
                                semanticToken.isID = true;
                                semanticToken.type = T_END_STR;
                                goto ID_STR;
                            }
                            else if(idType == FNC_NAME){
                                if(NestingOfId.functionPtr->rettypes == 1
                                    && NestingOfId.functionPtr->FreturnType[0] == STRING){
                                        goto ComplyingFNC;
                                    }
                                else
                                    analysisResult = SEMANTIC_ERR_PARAM;
                            }
                            else
                                analysisResult = SEMANTIC_ERR_TYPE;
                        }
                        else
                            analysisResult = SYNTACTIC_ERR;
                        
                    }
                    break;



                    case T_END_STR:

                        if(semanticToken.isID && lastType != T_ADD){
                            if(DefVar && InForLoop){
                                LinkedPushID(&ForList, &semanticToken, NestingOfId.nesting, scope);
                                if(ForID != labelNMB)
                                    pushID(&semanticToken.value.stringValue.str, NestingOfId.nesting, scope);
                            }
                            else
                                pushID(&semanticToken.value.stringValue.str, NestingOfId.nesting, scope);
                        }
                        else if(lastType != T_ADD){
                            if(DefVar && InForLoop){
                                LinkedPushString(&ForList, &semanticToken.value.stringValue.str);
                                if(ForID != labelNMB)
                                    pushstring(&semanticToken.value.stringValue.str);
                            }
                            else
                                pushstring(&semanticToken.value.stringValue.str);
                        }
                        ID_STR:
                        if (lastType == T_ADD){

                            lastType = semanticToken.type;

                            TOKEN operandB; // plus
                            tokenStackTopPop(&exprStack, &operandB);

                            TOKEN operandA; // prvy operand
                            tokenStackTopPop(&exprStack, &operandA);

                            // vygeneruj instrukciu
                            if(DefVar && InForLoop){
                                LinkedConcat(&ForList, &semanticToken, NestingOfId.nesting, scope);
                                if(ForID != labelNMB)
                                    StrConcatenation(&semanticToken, NestingOfId.nesting, scope);
                            }
                            else
                                StrConcatenation(&semanticToken, NestingOfId.nesting, scope);
                            // pushnni na stack token T_END_STR
                            tokenStackPush(&exprStack, &operandA);

                            getToken(&semanticToken);
                            
                        }

                        else if (lastType == T_GREATER || lastType == T_GREATER_E || lastType == T_LESSER
                            || lastType == T_LESSER_E || lastType == T_NOT_EQ || lastType == T_EQUAL
                            || lastType == T_L_RBRACKET){
                                tokenStackPush(&exprStack, &semanticToken);
                                lastType = semanticToken.type;
                                getToken(&semanticToken);
                            }

                        else if(lastType == T_END_STR)
                            analysisResult = SYNTACTIC_ERR;

                        else 
                            analysisResult = SEMANTIC_ERR_ELSE;
                        
                        break;


                    case T_R_RBRACKET:
                        if(lastType == T_END_STR || lastType == T_BOOL){
                            lastType = semanticToken.type;

                            TOKEN operandB; // operand (string)
                            tokenStackTopPop(&exprStack, &operandB);

                            TOKEN operandA; // lava zavorka
                            if(!tokenStackEmpty(&exprStack)){
                                tokenStackTopPop(&exprStack, &operandA);
                            }
                            // Predpokladam ze som vo fnci
                            else if(inFncCall){
                                itemStackPush(&idAssignStack, (iValue*) &operandB.value, STRING, operandB.isID);

                                //nacitame EOL
                                getToken(&semanticToken);
                                return CODE_OK;
                            }
                            
                            Left_br_string:
                            if(operandA.type == T_L_RBRACKET){
                                tokenStackPush(&exprStack, &operandB);
                                lastType = operandB.type;
                                getToken(&semanticToken);
                            }
                            else if(operandA.type == T_GREATER || operandA.type == T_GREATER_E || operandA.type == T_LESSER
                                    || operandA.type == T_LESSER_E || operandA.type == T_NOT_EQ || operandA.type == T_EQUAL
                                    || operandA.type == T_ADD){
                                        //vygeneruj instrukciu porovnania / scitania
                                        
                                        if(operandA.type != T_ADD)
                                           operandB.type = T_BOOL;

                                        switch(operandA.type){
                                            case T_ADD:
                                                if(DefVar && InForLoop){
                                                    LinkedConcat(&ForList, &operandB, NestingOfId.nesting, scope);
                                                    if(ForID != labelNMB)
                                                        StrConcatenation(&operandB, NestingOfId.nesting, scope);
                                                }
                                                else
                                                    StrConcatenation(&operandB, NestingOfId.nesting, scope);
                                                break;
                                            case T_GREATER:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, greaterS);
                                                    if(ForID != labelNMB)
                                                        greater();
                                                }
                                                else
                                                    greater();
                                                break;
                                            case T_LESSER:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, lesserS);
                                                    if(ForID != labelNMB)
                                                        lesser();
                                                }
                                                else
                                                    lesser();
                                                break;
                                            case T_EQUAL:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, equalS);
                                                    if(ForID != labelNMB)
                                                        equal();
                                                }
                                                else
                                                    equal();
                                                break;
                                            case T_GREATER_E:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, greaterEqualS);
                                                    if(ForID != labelNMB)
                                                        greaterEqual();
                                                }
                                                greaterEqual();
                                                break;
                                            case T_LESSER_E:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, lesserEqualS);
                                                    if(ForID != labelNMB)
                                                        lesserEqual();
                                                }
                                                lesserEqual();
                                                break;
                                            case T_NOT_EQ:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, notequalS);
                                                    if(ForID != labelNMB)
                                                        notequal();
                                                }
                                                notequal();
                                                break;
                                            // iba placeholder aby bol prekladac rad
                                            default:
                                                analysisResult = SEMANTIC_ERR_ELSE;
                                                break;
                                        }

                                        tokenStackTopPop(&exprStack, &currentToken);
                                        // na zaklade vhodnych tokenov
                                        tokenStackTopPop(&exprStack,&operandA);
                                        goto Left_br_string;
                                    }
                            else{
                                analysisResult = SYNTACTIC_ERR; // podla Go
                            }
                        }
                        else
                        {
                            analysisResult = SYNTACTIC_ERR; // podla Go
                        }
                        break;


                    case T_EOL:
                    case T_L_RBRACKET:
            
                        // if lastType == + - * / ( porovnavacie znamienka
                        if (lastType == T_ADD || lastType == T_EOL
                                || lastType == T_GREATER || lastType == T_GREATER_E || lastType == T_LESSER
                                || lastType == T_LESSER_E || lastType == T_NOT_EQ || lastType == T_EQUAL
                                || lastType == T_L_RBRACKET){

                            if(lastType == T_BOOL)
                                return SEMANTIC_ERR_TYPE;
                            switch(semanticToken.type){
                                case T_EOL:
                                    while(semanticToken.type == T_EOL){
                                        getToken(&semanticToken);
                                    }
                                    break;
                                default://case T_LRBRACKET
                                    tokenStackPush(&exprStack, &semanticToken);
                                    lastType = semanticToken.type;
                                    getToken(&semanticToken);

                            }
                        }
                        else if(semanticToken.type == T_EOL){
                            goto endStrExpr;
                        }
                        else
                            analysisResult = SYNTACTIC_ERR; // podla Go
                        break;


//////////////////////////////////////////////////////////////////////////////////////
                    case T_COMA:
                    case T_SEMIC:
                    case T_L_CBRACKET:
                        endStrExpr:
                        if (lastType == T_DIV || lastType == T_SUB 
                            || lastType == T_ADD || lastType == T_MUL)
                            analysisResult = SYNTACTIC_ERR;
                        else if(lastType == T_BOOL){
                            tokenStackTopPop(&exprStack, &currentToken);
                            itemStackPush(&idAssignStack, (iValue*) &currentToken.value ,BOOL, currentToken.isID);    
                        }
                        else{
                            tokenStackTopPop(&exprStack, &currentToken);
                            itemStackPush(&idAssignStack, (iValue*) &currentToken.value, STRING, currentToken.isID);    
                        }

                        // skontroluj ze je prazdny zasobnik
                        notEnded_str:
                        // vygeneruj instrukciu
                        // pushni na stack
                        if(!tokenStackEmpty(&exprStack)){

                        

                            itemStackPop(&idAssignStack);
                            //curentToken = operand

                            TOKEN operandB; // znamienko
                            tokenStackTopPop(&exprStack, &operandB);

                            TOKEN operandA; // operand
                            if(!tokenStackEmpty(&exprStack)){
                                tokenStackTopPop(&exprStack, &operandA);
                            }
                            else
                                analysisResult = SYNTACTIC_ERR;
                            
                            if(operandB.type == T_GREATER || operandB.type == T_GREATER_E || operandB.type == T_LESSER
                                    || operandB.type == T_LESSER_E || operandB.type == T_NOT_EQ || operandB.type == T_EQUAL)
                                    {
                                        if(lastType == T_BOOL)
                                            return SEMANTIC_ERR_TYPE;
                                        itemStackPush(&idAssignStack, (iValue*) &currentToken.value, BOOL, currentToken.isID);

                                        switch(operandB.type){
                                            case T_GREATER:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, greaterS);
                                                    if(ForID != labelNMB)
                                                        greater();
                                                }
                                                greater();
                                                break;
                                            case T_LESSER:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, lesserS);
                                                    if(ForID != labelNMB)
                                                        lesser();
                                                }
                                                lesser();
                                                break;
                                            case T_EQUAL:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, equalS);
                                                    if(ForID != labelNMB)
                                                        equal();
                                                }
                                                equal();
                                                break;
                                            case T_GREATER_E:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, greaterEqualS);
                                                    if(ForID != labelNMB)
                                                        greaterEqual();
                                                }
                                                greaterEqual();
                                                break;
                                            case T_LESSER_E:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, lesserEqualS);
                                                    if(ForID != labelNMB)
                                                        lesserEqual();
                                                }
                                                lesserEqual();
                                                break;
                                            case T_NOT_EQ:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, notequalS);
                                                    if(ForID != labelNMB)
                                                        notequal();
                                                }
                                                notequal();
                                                break;
                                            // iba placeholder aby bol prekladac rad
                                            default:
                                                analysisResult = SEMANTIC_ERR_ELSE;
                                                break;
                                        }
                                    }
                            else if(operandB.type == T_ADD){
                                if(lastType == T_BOOL)
                                    return SEMANTIC_ERR_TYPE;
                                itemStackPush(&idAssignStack, (iValue*) &currentToken.value, STRING, currentToken.isID);
                                
                                //TODO nebude konkatenacia pravdepodobne fungovat so zavorkami
                                if(DefVar && InForLoop){
                                    LinkedConcat(&ForList, &currentToken, NestingOfId.nesting, scope);
                                    if(ForID != labelNMB)
                                        StrConcatenation(&currentToken, NestingOfId.nesting, scope);
                                }
                                else
                                    StrConcatenation(&currentToken, NestingOfId.nesting, scope);
                                // safe check, ale netrebalo by

                            }

                            goto notEnded_str;
                        }
                        else
                            return analysisResult;
                        break;
                    break;



                    
                    case T_FLOAT_EX_R:
                    case T_F_FLOAT:
                    case T_INT19:
                    case T_INT0:
                        if(lastType == T_END_STR)
                            analysisResult = SYNTACTIC_ERR;
                        else
                            analysisResult = SEMANTIC_ERR_TYPE;
                        break;

                    case T_ASSIGN:
                    case T_DEFINITION:
                        analysisResult = SYNTACTIC_ERR;
                        break;

                    case T_UNDERSCORE:
                        analysisResult = SEMANTIC_ERR_UNDEF;
                        break;
                        
                    case T_LEX_ERR:
                        analysisResult = LEXICAL_ERR;
                        break;

                    default:
                        analysisResult = SEMANTIC_ERR_TYPE;
                    break;
                }
            }while(!analysisResult);

            break;

        /////////////////////////////////////////////////////////////////////////
        ////////////////////////////////INT//////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////

        case T_INT19:
        case T_INT0:
            if(DefVar && InForLoop){
                LinkedPushInt(&ForList, semanticToken.value.integerValue);
                if(ForID != labelNMB)
                    pushInt(semanticToken.value.integerValue);    
            }
            else
                pushInt(semanticToken.value.integerValue);
            tokenStackPush(&exprStack, &semanticToken);
            lastType = semanticToken.type;
            getToken(&semanticToken);

            do{    
                INT_expr:
                switch(semanticToken.type)
                {

                    case T_GREATER:
                    case T_GREATER_E:
                    case T_LESSER:
                    case T_LESSER_E:
                    case T_NOT_EQ:
                    case T_EQUAL:
                        if(isComparison){
                            analysisResult = SEMANTIC_ERR_TYPE; // v Go vybehne tento error
                        }
                        else if (lastType == T_INT0 || lastType == T_INT19){
                            tokenStackPush(&exprStack, &semanticToken);
                            lastType = semanticToken.type;
                            getToken(&semanticToken);
                            isComparison = true;
                        }
                        else
                            analysisResult = SYNTACTIC_ERR;
                    break;



                    case T_ADD:
                    case T_SUB:
                    case T_DIV:
                    case T_MUL:
                        //tuto switch
                        if(lastType == T_INT0 || lastType == T_INT19 ){
                            tokenStackPush(&exprStack, &semanticToken);
                            lastType = semanticToken.type;
                            getToken(&semanticToken);
                        }
                        else if(lastType == T_BOOL)
                        {
                            analysisResult = SEMANTIC_ERR_TYPE;
                        }
                        
                        else
                            analysisResult = SYNTACTIC_ERR;

                        break;



                    case T_ID:
                    {
                        if(lastType == T_ADD || lastType == T_SUB || lastType == T_MUL || lastType == T_DIV
                            || lastType == T_L_RBRACKET || lastType == T_GREATER || lastType == T_GREATER_E
                            || lastType == T_LESSER || lastType == T_LESSER_E || lastType == T_NOT_EQ
                            || lastType == T_EQUAL)
                        {
                            vType idType = BSTreeGetFromStack(&semanticToken.value.stringValue, &NestingOfId, &myStack, &scope);
                            if(idType == INT){
                                
                                semanticToken.isID = true;
                                // potrebujeme tam LF@ a nie hodnotu z tabulky
                                //semanticToken.value.integerValue = tmp.integerValue;
                                // TODO nejako checkovat ktory type to je
                                semanticToken.type = T_INT19;
                                goto ID_INT;
                            }
                            else if(idType == FNC_NAME){
                                if(NestingOfId.functionPtr->rettypes == 1
                                    && NestingOfId.functionPtr->FreturnType[0] == INT){
                                        goto ComplyingFNC;
                                    }
                                else
                                    analysisResult = SEMANTIC_ERR_PARAM;
                            }
                            else
                                analysisResult = SEMANTIC_ERR_TYPE;
                        }
                        else
                            analysisResult = SYNTACTIC_ERR;
                        
                    }
                    break;



                    case T_INT19:
                    case T_INT0:
                        ID_INT: 
                        
                        //do assembleru pushni regardless
                        if(semanticToken.isID){
                            if(DefVar && InForLoop){
                                LinkedPushID(&ForList, &semanticToken, NestingOfId.nesting, scope);
                                if(ForID != labelNMB)
                                    pushID(&semanticToken.value.stringValue.str, NestingOfId.nesting, scope);
                            }
                            else
                                pushID(&semanticToken.value.stringValue.str, NestingOfId.nesting, scope);
                        }
                        else{
                            if(DefVar && InForLoop){
                                pushInt(semanticToken.value.integerValue);
                                if(ForID != labelNMB){
                                    pushInt(semanticToken.value.integerValue);    
                                }
                            }
                            else
                                pushInt(semanticToken.value.integerValue);
                        }

                        if (lastType == T_MUL || lastType == T_DIV){
                            
                            if(!semanticToken.isID){
                                if(lastType == T_DIV && semanticToken.value.integerValue == 0)
                                    return SEMANTIC_ERR_ZERO;
                            }

                            lastType = semanticToken.type;

                            // toto by sme mohli zahodit lebo mame last stav
                            TOKEN operandB;
                            tokenStackTopPop(&exprStack, &operandB);

                            TOKEN operandA;
                            tokenStackTopPop(&exprStack, &operandA);
                            
                            //vygeneruj instrukciu
                            switch(operandB.type){
                                case T_DIV:
                                    if(DefVar && InForLoop){
                                        LinkedDataStack(&ForList, divIntS);
                                        if(ForID != labelNMB)
                                            divInt();
                                    }
                                    else
                                        divInt();
                                    break;
                                case T_MUL:
                                    if(DefVar && InForLoop){
                                        LinkedDataStack(&ForList, mulS);
                                        if(ForID != labelNMB)
                                            mul();
                                    }
                                    else
                                        mul();
                                    break;
                                // iba placeholder aby bol prekladac rad
                                default:
                                    analysisResult = SEMANTIC_ERR_ELSE;
                                    break;
                            }
                            // pushni token na stack
                            tokenStackPush(&exprStack, &operandA); // dolezite je pre nas len ze je to

                            getToken(&semanticToken);
                            
                            
                        }

                        else if (lastType == T_ADD || lastType == T_SUB){

                            lastType = semanticToken.type;

                            // potrebujeme sa pozriet na dalsi token a podla neho bud redukovat alebo nie
                            // toto by sme mohli zahodit lebo mame last stav
                            TOKEN operandB; // znamienko
                            tokenStackTopPop(&exprStack, &operandB);

                            TOKEN operandA; // prvy operand
                            tokenStackTopPop(&exprStack, &operandA);

                            TOKEN semanticCopy; // druhy operand
                            semanticCopy.type = semanticToken.type;
                            semanticCopy.isID = semanticToken.isID;
                            copyTokenVal(&semanticCopy, &semanticToken);

                            //operandB.type = T_INT19;
                            copyTokenVal(&operandB, &semanticToken);

                            // terajsie cislo pushni tak ci tak na stack

                            getToken(&semanticToken);
                            if (semanticToken.type == T_ADD || semanticToken.type == T_SUB || semanticToken.type == T_EOL
                                || semanticToken.type == T_GREATER || semanticToken.type == T_GREATER_E || semanticToken.type == T_LESSER
                                || semanticToken.type == T_LESSER_E || semanticToken.type == T_NOT_EQ || semanticToken.type == T_EQUAL
                                || semanticToken.type == T_R_RBRACKET){

                                //vygeneruj instrukciu
                                switch(operandB.type){
                                    case T_ADD:
                                        if(DefVar && InForLoop){
                                            LinkedDataStack(&ForList, addS);
                                            if(ForID != labelNMB)
                                                add();
                                        }
                                        else
                                            add();
                                        break;
                                    case T_SUB:
                                        if(DefVar && InForLoop){
                                            LinkedDataStack(&ForList, subS);
                                            if(ForID != labelNMB)
                                                sub();
                                        }
                                        else
                                            sub();
                                        break;
                                    // iba placeholder aby bol prekladac rad
                                    default:
                                        analysisResult = SEMANTIC_ERR_ELSE;
                                        break;
                                }
                                // pushnni na stack token T_INT19
                                tokenStackPush(&exprStack, &operandA);
                            }

                            else{
                                tokenStackPush(&exprStack, &operandA); //operand1
                                tokenStackPush(&exprStack, &operandB); //znamienko
                                tokenStackPush(&exprStack, &semanticCopy); //operand2
                            }
                            
                        }

                        else if (lastType == T_GREATER || lastType == T_GREATER_E || lastType == T_LESSER
                            || lastType == T_LESSER_E || lastType == T_NOT_EQ || lastType == T_EQUAL
                            || lastType == T_L_RBRACKET){//instrukcia??
                                tokenStackPush(&exprStack, &semanticToken);
                                lastType = semanticToken.type;
                                getToken(&semanticToken);
                            }

                        else if(lastType == T_INT0 || lastType == T_INT19)
                            analysisResult = SYNTACTIC_ERR;

                        else 
                            analysisResult = SEMANTIC_ERR_ELSE;
                        
                        break;



                    case T_R_RBRACKET:
                        if(lastType == T_INT0 || lastType == T_INT19 || lastType == T_BOOL){
                            lastType = semanticToken.type;

                            //tu nepovolujeme vsetko co povolujeme vyssie

                            TOKEN operandB; // operand
                            tokenStackTopPop(&exprStack, &operandB);



                            // TODO tuto dat check na TokenStackEmpty a podla toho ze ci som vo fncii
                            // tak vracat error alebo code ok
                            TOKEN operandA; // lava zavorka
                            if(!tokenStackEmpty(&exprStack)){
                                tokenStackTopPop(&exprStack, &operandA);
                            }
                            // Predpokladam ze som vo fnci
                            else if(inFncCall){
                                itemStackPush(&idAssignStack, (iValue*) &operandB.value, INT, operandB.isID);

                                //nacitame EOL
                                getToken(&semanticToken);
                                return CODE_OK;
                            }

                            Left_br_int:
                            if(operandA.type == T_L_RBRACKET){
                                tokenStackPush(&exprStack, &operandB);
                                lastType = operandB.type;
                                getToken(&semanticToken);
                            }
                            else if(operandA.type == T_GREATER || operandA.type == T_GREATER_E || operandA.type == T_LESSER
                                    || operandA.type == T_LESSER_E || operandA.type == T_NOT_EQ || operandA.type == T_EQUAL
                                    || operandA.type == T_ADD || operandA.type == T_SUB || operandA.type == T_MUL
                                    || operandA.type == T_DIV){

                                        if(operandA.type != T_ADD && operandA.type != T_SUB && operandA.type != T_MUL
                                            && operandA.type != T_DIV)
                                           operandB.type = T_BOOL;

                                        switch(operandA.type){
                                            case T_ADD:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, addS);
                                                    if(ForID != labelNMB)
                                                        add();
                                                }
                                                else
                                                    add();
                                                break;
                                            case T_SUB:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, subS);
                                                    if(ForID != labelNMB)
                                                        sub();
                                                }
                                                else
                                                    sub();
                                                break;
                                            case T_MUL:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, mulS);
                                                    if(ForID != labelNMB)
                                                        mul();
                                                }
                                                else
                                                    mul();
                                                break;
                                            case T_DIV:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, divIntS);
                                                    if(ForID != labelNMB)
                                                        divInt();
                                                }
                                                else
                                                    divInt();
                                                break;
                                            case T_GREATER:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, greaterS);
                                                    if(ForID != labelNMB)
                                                        greater();
                                                }
                                                else
                                                    greater();
                                                break;
                                            case T_LESSER:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, lesserS);
                                                    if(ForID != labelNMB)
                                                        lesser();
                                                }
                                                else
                                                    lesser();
                                                break;
                                            case T_EQUAL:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, equalS);
                                                    if(ForID != labelNMB)
                                                        equal();
                                                }
                                                else
                                                    equal();
                                                break;
                                            case T_GREATER_E:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, greaterEqualS);
                                                    if(ForID != labelNMB)
                                                        greaterEqual();
                                                }
                                                greaterEqual();
                                                break;
                                            case T_LESSER_E:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, lesserEqualS);
                                                    if(ForID != labelNMB)
                                                        lesserEqual();
                                                }
                                                lesserEqual();
                                                break;
                                            case T_NOT_EQ:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, notequalS);
                                                    if(ForID != labelNMB)
                                                        notequal();
                                                }
                                                notequal();
                                                break;
                                            // iba placeholder aby bol prekladac rad
                                            default:
                                                analysisResult = SEMANTIC_ERR_ELSE;
                                                break;
                                        }
                                        //vygeneruj instrukciu porovnania / scitania / odcitania
                                        tokenStackTopPop(&exprStack, &currentToken);
                                        // na zaklade vhodnych tokenov
                                        tokenStackTopPop(&exprStack,&operandA);
                                        goto Left_br_int;
                                    }

                            else{
                                analysisResult = SYNTACTIC_ERR; // podla Go
                            }
                        }
                        else
                        {
                            analysisResult = SYNTACTIC_ERR; // podla Go
                        }
                        break;


                    case T_EOL:
                    case T_L_RBRACKET:
            
                        // if lastType == + - * / ( porovnavacie znamienka
                        if (lastType == T_ADD || lastType == T_SUB || lastType == T_EOL
                                || lastType == T_MUL || lastType == T_DIV
                                || lastType == T_GREATER || lastType == T_GREATER_E || lastType == T_LESSER
                                || lastType == T_LESSER_E || lastType == T_NOT_EQ || lastType == T_EQUAL
                                || lastType == T_L_RBRACKET){

                            switch(semanticToken.type){
                                case T_EOL:
                                    while(semanticToken.type == T_EOL){
                                        getToken(&semanticToken);
                                    }
                                    break;
                                default://case T_LRBRACKET
                                    tokenStackPush(&exprStack, &semanticToken);
                                    lastType = semanticToken.type;
                                    getToken(&semanticToken);

                            }
                        }
                        else if(semanticToken.type == T_EOL)
                            goto endIntExpr;
                        else
                            analysisResult = SYNTACTIC_ERR; // podla Go
                        break;



                    // s '{' si uz poradi parser
                    case T_COMA:
                    case T_SEMIC:
                    case T_L_CBRACKET:
                        endIntExpr:
                        if (lastType == T_DIV || lastType == T_SUB 
                            || lastType == T_ADD || lastType == T_MUL)
                            analysisResult = SYNTACTIC_ERR;

                        else if(lastType == T_BOOL){
                            tokenStackTopPop(&exprStack, &currentToken);
                            itemStackPush(&idAssignStack, (iValue*) &currentToken.value ,BOOL, currentToken.isID);    
                        }
                        else{
                            tokenStackTopPop(&exprStack, &currentToken);
                            itemStackPush(&idAssignStack, (iValue*) &currentToken.value ,INT, currentToken.isID);    
                        }
                        // skontroluj ze je prazdny zasobnik


                        notEnded_int:
                        // vygeneruj instrukciu
                        // pushni na stack
                        if(!tokenStackEmpty(&exprStack)){

                            itemStackPop(&idAssignStack);

                            TOKEN operandB; // znamienko
                            tokenStackTopPop(&exprStack, &operandB);

                            TOKEN operandA; // operand
                            if(!tokenStackEmpty(&exprStack)){
                                tokenStackTopPop(&exprStack, &operandA);
                            }
                            // tu sa nedostanem, ale to be sure
                            else
                                analysisResult = SYNTACTIC_ERR;
                            
                            if((operandB.type == T_GREATER || operandB.type == T_GREATER_E || operandB.type == T_LESSER
                                    || operandB.type == T_LESSER_E || operandB.type == T_NOT_EQ || operandB.type == T_EQUAL))
                            {
                                if(lastType == T_BOOL)
                                    return SEMANTIC_ERR_TYPE;
                                itemStackPush(&idAssignStack, (iValue*) &currentToken.value, BOOL, currentToken.isID);

                                switch(operandB.type){
                                    case T_GREATER:
                                        if(DefVar && InForLoop){
                                            LinkedDataStack(&ForList, greaterS);
                                            if(ForID != labelNMB)
                                                greater();
                                        }
                                        greater();
                                        break;
                                    case T_LESSER:
                                        if(DefVar && InForLoop){
                                            LinkedDataStack(&ForList, lesserS);
                                            if(ForID != labelNMB)
                                                lesser();
                                        }
                                        lesser();
                                        break;
                                    case T_EQUAL:
                                        if(DefVar && InForLoop){
                                            LinkedDataStack(&ForList, equalS);
                                            if(ForID != labelNMB)
                                                equal();
                                        }
                                        equal();
                                        break;
                                    case T_GREATER_E:
                                        if(DefVar && InForLoop){
                                            LinkedDataStack(&ForList, greaterEqualS);
                                            if(ForID != labelNMB)
                                                greaterEqual();
                                        }
                                        greaterEqual();
                                        break;
                                    case T_LESSER_E:
                                        if(DefVar && InForLoop){
                                            LinkedDataStack(&ForList, lesserEqualS);
                                            if(ForID != labelNMB)
                                                lesserEqual();
                                        }
                                        lesserEqual();
                                        break;
                                    case T_NOT_EQ:
                                        if(DefVar && InForLoop){
                                            LinkedDataStack(&ForList, notequalS);
                                            if(ForID != labelNMB)
                                                notequal();
                                        }
                                        notequal();
                                        break;
                                    // iba placeholder aby bol prekladac rad
                                    default:
                                        analysisResult = SEMANTIC_ERR_ELSE;
                                        break;
                                }
                            }
                            // Vygenerovat instrukciu
                            else if(operandB.type == T_ADD || operandB.type == T_SUB || operandB.type == T_MUL || operandB.type == T_DIV){
                                if(lastType == T_BOOL)
                                    return SEMANTIC_ERR_TYPE;
                                itemStackPush(&idAssignStack, (iValue*) &currentToken.value ,INT, currentToken.isID);
                                switch(operandB.type){
                                    case T_ADD:
                                        if(DefVar && InForLoop){
                                            LinkedDataStack(&ForList, addS);
                                            if(ForID != labelNMB)
                                                add();
                                        }
                                        else
                                            add();
                                        break;
                                    case T_SUB:
                                        if(DefVar && InForLoop){
                                            LinkedDataStack(&ForList, subS);
                                            if(ForID != labelNMB)
                                                sub();
                                        }
                                        else
                                            sub();
                                        break;
                                    case T_MUL:
                                        if(DefVar && InForLoop){
                                            LinkedDataStack(&ForList, mulS);
                                            if(ForID != labelNMB)
                                                mul();
                                        }
                                        else
                                            mul();
                                        break;
                                    case T_DIV:
                                        if(DefVar && InForLoop){
                                            LinkedDataStack(&ForList, divIntS);
                                            if(ForID != labelNMB)
                                                divInt();
                                        }
                                        else
                                            divInt();
                                        break;
                                    // iba placeholder aby bol prekladac rad
                                    default:
                                        analysisResult = SEMANTIC_ERR_ELSE;
                                        break;
                                }
                                
                            }

                            goto notEnded_int;
                            
                        }
                        else
                            return analysisResult;
                        break;


                    
                    case T_FLOAT_EX_R:
                    case T_F_FLOAT:
                    case T_END_STR:
                        if(lastType == T_INT19 || lastType == T_INT0)
                            analysisResult = SYNTACTIC_ERR;
                        else
                            analysisResult = SEMANTIC_ERR_TYPE;
                        break;
                    
                    case T_ASSIGN:
                    case T_DEFINITION:
                        analysisResult = SYNTACTIC_ERR;
                        break;                    

                    case T_UNDERSCORE:
                        analysisResult = SEMANTIC_ERR_UNDEF;
                        break;

                    case T_LEX_ERR:
                        analysisResult = LEXICAL_ERR;
                        break;

                    default:
                        analysisResult = SEMANTIC_ERR_TYPE;;
                        break;
                }

            }while(!analysisResult);
        break;


        /////////////////////////////////////////////////////////////////////////
        ////////////////////////////////FLOAT////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////


        case T_F_FLOAT:
        case T_FLOAT_EX_R:

            if(DefVar && InForLoop){
                LinkedPushFloat(&ForList, semanticToken.value.floatValue);
                if(ForID != labelNMB)
                    pushFloat(semanticToken.value.floatValue);
            }
            pushFloat(semanticToken.value.floatValue);
            tokenStackPush(&exprStack, &semanticToken);
            lastType = semanticToken.type;
            getToken(&semanticToken);

            do{
                FLOAT_expr:
                switch(semanticToken.type)
                {
                    case T_GREATER:
                    case T_GREATER_E:
                    case T_LESSER:
                    case T_LESSER_E:
                    case T_NOT_EQ:
                    case T_EQUAL:
                        if(isComparison){
                            analysisResult = SEMANTIC_ERR_TYPE; // v Go vybehne tento error
                        }
                        else if (lastType == T_F_FLOAT || lastType == T_FLOAT_EX_R){
                            tokenStackPush(&exprStack, &semanticToken);
                            lastType = semanticToken.type;
                            getToken(&semanticToken);
                            isComparison = true;
                        }
                        else
                            analysisResult = SYNTACTIC_ERR;
                    break;
                    
                    case T_ADD:
                    case T_SUB:
                    case T_DIV:
                    case T_MUL:
                        //tuto switch
                        if(lastType == T_F_FLOAT || lastType == T_FLOAT_EX_R){
                            tokenStackPush(&exprStack, &semanticToken);
                            lastType = semanticToken.type;
                            getToken(&semanticToken);
                        }
                        else if(lastType == T_BOOL)
                        {
                            analysisResult = SEMANTIC_ERR_TYPE;
                        }

                        else
                            analysisResult = SYNTACTIC_ERR;

                        break;

                    case T_ID:
                    {
                        if(lastType == T_ADD || lastType == T_SUB || lastType == T_MUL || lastType == T_DIV
                            || lastType == T_L_RBRACKET || lastType == T_GREATER || lastType == T_GREATER_E
                            || lastType == T_LESSER || lastType == T_LESSER_E || lastType == T_NOT_EQ
                            || lastType == T_EQUAL)
                        {
                            
                            if(FLOAT == BSTreeGetFromStack(&semanticToken.value.stringValue, &NestingOfId, &myStack, &scope)){
                                
                                semanticToken.isID = true;

                                semanticToken.type = T_F_FLOAT;
                                goto ID_FLOAT;
                            }
                            else
                                analysisResult = SEMANTIC_ERR_TYPE;
                        }
                        else
                            analysisResult = SYNTACTIC_ERR;
                    }
                    break;

                    case T_F_FLOAT:
                    
                    case T_FLOAT_EX_R:
                        ID_FLOAT:     

                        if(semanticToken.isID){
                            if(DefVar && InForLoop){
                                LinkedPushID(&ForList, &semanticToken, NestingOfId.nesting, scope);
                                if(ForID != labelNMB)
                                    pushID(&semanticToken.value.stringValue.str, NestingOfId.nesting, scope);    
                            }
                            else
                                pushID(&semanticToken.value.stringValue.str, NestingOfId.nesting, scope);
                        }
                        else{
                            if(DefVar && InForLoop){
                                LinkedPushFloat(&ForList, semanticToken.value.floatValue);
                                if(ForID != labelNMB)
                                    pushFloat(semanticToken.value.floatValue);
                            }
                            else
                                pushFloat(semanticToken.value.floatValue);
                        }

                        if (lastType == T_MUL || lastType == T_DIV){
                            
                            if(!semanticToken.isID){
                                if(lastType == T_DIV && semanticToken.value.floatValue == 0)
                                    return SEMANTIC_ERR_ZERO;
                            }
                            
                            lastType = semanticToken.type;

                            TOKEN operandB;
                            tokenStackTopPop(&exprStack, &operandB);

                            TOKEN operandA;
                            tokenStackTopPop(&exprStack, &operandA);

                            //vygeneruj instrukciu
                            switch(operandB.type){
                                case T_DIV:
                                    if(DefVar && InForLoop){
                                        LinkedDataStack(&ForList, divFloatS);
                                        if(ForID != labelNMB)
                                            divFloat();
                                    }
                                    else
                                        divFloat();
                                    break;
                                case T_MUL:
                                    if(DefVar && InForLoop){
                                        LinkedDataStack(&ForList, mulS);
                                        if(ForID != labelNMB)
                                            mul();
                                    }
                                    else
                                        mul();
                                    break;
                                // iba placeholder aby bol prekladac rad
                                default:
                                    analysisResult = SEMANTIC_ERR_ELSE;
                                    break;
                            }
                            // pushni token na stack
                            tokenStackPush(&exprStack, &operandA);

                            getToken(&semanticToken);
                            
                            
                        }

                        else if (lastType == T_ADD || lastType == T_SUB){

                            lastType = semanticToken.type;

                            // potrebujeme sa pozriet na dalsi token a podla neho bud redukovat alebo nie
                            // toto by sme mohli zahodit lebo mame last stav
                            TOKEN operandB; // znamienko
                            tokenStackTopPop(&exprStack, &operandB);

                            TOKEN operandA; // prvy operand
                            tokenStackTopPop(&exprStack, &operandA);

                            TOKEN semanticCopy; // druhy operand
                            semanticCopy.type = semanticToken.type;
                            copyTokenVal(&semanticCopy, &semanticToken);

                            copyTokenVal(&operandB, &semanticToken);


                            getToken(&semanticToken);
                            if (semanticToken.type == T_ADD || semanticToken.type == T_SUB || semanticToken.type == T_EOL
                                || semanticToken.type == T_GREATER || semanticToken.type == T_GREATER_E || semanticToken.type == T_LESSER
                                || semanticToken.type == T_LESSER_E || semanticToken.type == T_NOT_EQ || semanticToken.type == T_EQUAL){

                                //vygeneruj instrukciu
                                switch(operandB.type){
                                    case T_ADD:
                                        if(DefVar && InForLoop){
                                            LinkedDataStack(&ForList, addS);
                                            if(ForID != labelNMB)
                                                add();
                                        }
                                        else
                                            add();
                                        break;
                                    case T_SUB:
                                        if(DefVar && InForLoop){
                                            LinkedDataStack(&ForList, subS);
                                            if(ForID != labelNMB)
                                                sub();
                                        }
                                        else
                                            sub();
                                        break;
                                    // iba placeholder aby bol prekladac rad
                                    default:
                                        analysisResult = SEMANTIC_ERR_ELSE;
                                        break;
                                }

                                tokenStackPush(&exprStack, &operandA);
                            }

                            else{
                                tokenStackPush(&exprStack, &operandA); //operand1
                                tokenStackPush(&exprStack, &operandB); //znamienko
                                tokenStackPush(&exprStack, &semanticCopy); //operand2
                            }
                            
                            
                        }

                        else if (lastType == T_GREATER || lastType == T_GREATER_E || lastType == T_LESSER
                            || lastType == T_LESSER_E || lastType == T_NOT_EQ || lastType == T_EQUAL
                            || lastType == T_L_RBRACKET){
                                tokenStackPush(&exprStack, &semanticToken);
                                lastType = semanticToken.type;
                                getToken(&semanticToken);
                            }

                        else if(lastType == T_F_FLOAT || lastType == T_FLOAT_EX_R)
                            analysisResult = SYNTACTIC_ERR;

                        else 
                            analysisResult = SEMANTIC_ERR_ELSE;
                        
                        break;



                    case T_R_RBRACKET:
                        if(lastType == T_F_FLOAT || lastType == T_FLOAT_EX_R || lastType == T_BOOL){
                            lastType = semanticToken.type;

                            //tu nepovolujeme vsetko co povolujeme vyssie

                            TOKEN operandB; // operand
                            tokenStackTopPop(&exprStack, &operandB);


                            TOKEN operandA; // lava zavorka
                            if(!tokenStackEmpty(&exprStack)){
                                tokenStackTopPop(&exprStack, &operandA);
                            }
                            // Predpokladam ze som vo fnci
                            else if(inFncCall){
                                itemStackPush(&idAssignStack, (iValue*) &operandB.value, FLOAT, operandB.isID);

                                //nacitame EOL
                                getToken(&semanticToken);
                                return CODE_OK;
                            }

                            Left_br_float:
                            if(operandA.type == T_L_RBRACKET){
                                tokenStackPush(&exprStack, &operandB);
                                lastType = operandB.type;
                                getToken(&semanticToken);
                            }
                            else if(operandA.type == T_GREATER || operandA.type == T_GREATER_E || operandA.type == T_LESSER
                                    || operandA.type == T_LESSER_E || operandA.type == T_NOT_EQ || operandA.type == T_EQUAL
                                    || operandA.type == T_ADD || operandA.type == T_SUB || operandA.type == T_MUL
                                    || operandA.type == T_DIV){

                                        if(operandA.type != T_ADD || operandA.type != T_SUB || operandA.type != T_MUL
                                            || operandA.type != T_DIV)
                                           operandB.type = T_BOOL;

                                        switch(operandA.type){
                                            case T_ADD:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, addS);
                                                    if(ForID != labelNMB)
                                                        add();
                                                }
                                                else
                                                    add();
                                                break;
                                            case T_SUB:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, subS);
                                                    if(ForID != labelNMB)
                                                        sub();
                                                }
                                                else
                                                    sub();
                                                break;
                                            case T_MUL:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, mulS);
                                                    if(ForID != labelNMB)
                                                        mul();
                                                }
                                                else
                                                    mul();
                                                break;
                                            case T_DIV:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, divFloatS);
                                                    if(ForID != labelNMB)
                                                        divFloat();
                                                }
                                                else
                                                    divFloat();
                                                break;
                                            case T_GREATER:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, greaterS);
                                                    if(ForID != labelNMB)
                                                        greater();
                                                }
                                                else
                                                    greater();
                                                break;
                                            case T_LESSER:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, lesserS);
                                                    if(ForID != labelNMB)
                                                        lesser();
                                                }
                                                else
                                                    lesser();
                                                break;
                                            case T_EQUAL:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, equalS);
                                                    if(ForID != labelNMB)
                                                        equal();
                                                }
                                                else
                                                    equal();
                                                break;
                                            case T_GREATER_E:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, greaterEqualS);
                                                    if(ForID != labelNMB)
                                                        greaterEqual();
                                                }
                                                greaterEqual();
                                                break;
                                            case T_LESSER_E:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, lesserEqualS);
                                                    if(ForID != labelNMB)
                                                        lesserEqual();
                                                }
                                                lesserEqual();
                                                break;
                                            case T_NOT_EQ:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, notequalS);
                                                    if(ForID != labelNMB)
                                                        notequal();
                                                }
                                                notequal();
                                                break;
                                            // iba placeholder aby bol prekladac rad
                                            default:
                                                analysisResult = SEMANTIC_ERR_ELSE;
                                                break;
                                        }

                                        //vygeneruj instrukciu porovnania / scitania / odcitania
                                        tokenStackTopPop(&exprStack, &currentToken);
                                        // na zaklade vhodnych tokenov
                                        tokenStackTopPop(&exprStack,&operandA);
                                        goto Left_br_float;
                                    }

                            else{
                                analysisResult = SYNTACTIC_ERR; // podla Go
                            }
                        }
                        else
                        {
                            analysisResult = SYNTACTIC_ERR; // podla Go
                        }
                        break;
                    

                    case T_EOL:
                    case T_L_RBRACKET:
            
                        // if lastType == + - * / ( porovnavacie znamienka
                        if (lastType == T_ADD || lastType == T_SUB || lastType == T_EOL
                                || lastType == T_MUL || lastType == T_DIV
                                || lastType == T_GREATER || lastType == T_GREATER_E || lastType == T_LESSER
                                || lastType == T_LESSER_E || lastType == T_NOT_EQ || lastType == T_EQUAL
                                || lastType == T_L_RBRACKET){

                            switch(semanticToken.type){
                                case T_EOL:
                                    while(semanticToken.type == T_EOL){
                                        getToken(&semanticToken);
                                    }
                                    break;
                                default://case T_LRBRACKET
                                    tokenStackPush(&exprStack, &semanticToken);
                                    lastType = semanticToken.type;
                                    getToken(&semanticToken);

                            }
                        }
                        else if(semanticToken.type == T_EOL)
                            goto endFloatExpr;
                        else
                            analysisResult = SYNTACTIC_ERR; // podla Go
                        break;
                        


                    case T_COMA:
                    case T_SEMIC:
                    case T_L_CBRACKET:
                        endFloatExpr:
                        if (lastType == T_DIV || lastType == T_SUB 
                            || lastType == T_ADD || lastType == T_MUL)
                            analysisResult = SYNTACTIC_ERR;

                        
                        else if(lastType == T_BOOL){
                            tokenStackTopPop(&exprStack, &currentToken);
                            itemStackPush(&idAssignStack, (iValue*) &currentToken.value ,BOOL, currentToken.isID);    
                        }
                        else{
                            tokenStackTopPop(&exprStack, &currentToken);
                            itemStackPush(&idAssignStack, (iValue*) &currentToken.value, FLOAT, currentToken.isID);
                        // skontroluj ze je prazdny zasobnik
                        }
                        notEnded_float:
                        // vygeneruj instrukciu
                        // pushni na stack
                        if(!tokenStackEmpty(&exprStack)){


                            itemStackPop(&idAssignStack);
                            //curentToken = operand

                            TOKEN operandB; // znamienko
                            tokenStackTopPop(&exprStack, &operandB);

                            TOKEN operandA; // operand
                            if(!tokenStackEmpty(&exprStack)){
                                tokenStackTopPop(&exprStack, &operandA);
                            }
                            // tu sa nedostanem, ale to be sure
                            else
                                analysisResult = SYNTACTIC_ERR;
                            
                            if(operandB.type == T_GREATER || operandB.type == T_GREATER_E || operandB.type == T_LESSER
                                    || operandB.type == T_LESSER_E || operandB.type == T_NOT_EQ || operandB.type == T_EQUAL)
                                    {
                                        if(lastType == T_BOOL)
                                            return SEMANTIC_ERR_TYPE;
                                        itemStackPush(&idAssignStack, (iValue*) &currentToken.value, BOOL, currentToken.isID);

                                        switch(operandB.type){
                                            case T_GREATER:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, greaterS);
                                                    if(ForID != labelNMB)
                                                        greater();
                                                }
                                                greater();
                                                break;
                                            case T_LESSER:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, lesserS);
                                                    if(ForID != labelNMB)
                                                        lesser();
                                                }
                                                lesser();
                                                break;
                                            case T_EQUAL:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, equalS);
                                                    if(ForID != labelNMB)
                                                        equal();
                                                }
                                                equal();
                                                break;
                                            case T_GREATER_E:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, greaterEqualS);
                                                    if(ForID != labelNMB)
                                                        greaterEqual();
                                                }
                                                greaterEqual();
                                                break;
                                            case T_LESSER_E:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, lesserEqualS);
                                                    if(ForID != labelNMB)
                                                        lesserEqual();
                                                }
                                                lesserEqual();
                                                break;
                                            case T_NOT_EQ:
                                                if(DefVar && InForLoop){
                                                    LinkedDataStack(&ForList, notequalS);
                                                    if(ForID != labelNMB)
                                                        notequal();
                                                }
                                                notequal();
                                                break;
                                            // iba placeholder aby bol prekladac rad
                                            default:
                                                analysisResult = SEMANTIC_ERR_ELSE;
                                                break;
                                        }
                                    }
                            else if(operandB.type == T_ADD || operandB.type == T_SUB || operandB.type == T_MUL || operandB.type == T_DIV){
                                if(lastType == T_BOOL)
                                    return SEMANTIC_ERR_TYPE;
                                itemStackPush(&idAssignStack, (iValue*) &currentToken.value ,FLOAT, currentToken.isID);
                                switch(operandB.type){
                                    case T_ADD:
                                        if(DefVar && InForLoop){
                                            LinkedDataStack(&ForList, addS);
                                            if(ForID != labelNMB)
                                                add();
                                        }
                                        else
                                            add();
                                        break;
                                    case T_SUB:
                                        if(DefVar && InForLoop){
                                            LinkedDataStack(&ForList, subS);
                                            if(ForID != labelNMB)
                                                sub();
                                        }
                                        else
                                            sub();
                                        break;
                                    case T_MUL:
                                        if(DefVar && InForLoop){
                                            LinkedDataStack(&ForList, mulS);
                                            if(ForID != labelNMB)
                                                mul();
                                        }
                                        else
                                            mul();
                                        break;
                                    case T_DIV:
                                        if(DefVar && InForLoop){
                                            LinkedDataStack(&ForList, divFloatS);
                                            if(ForID != labelNMB)
                                                divFloat();
                                        }
                                        else
                                            divFloat();
                                        break;
                                    // iba placeholder aby bol prekladac rad
                                    default:
                                        analysisResult = SEMANTIC_ERR_ELSE;
                                        break;
                                }
                            }

                            goto notEnded_float;
                            
                        }
                        else
                            return analysisResult;
                        break;
                    
                    case T_INT19:
                    case T_INT0:
                    case T_END_STR:
                        if(lastType == T_F_FLOAT || lastType == T_FLOAT_EX_R )
                            analysisResult = SYNTACTIC_ERR;
                        else    
                            analysisResult = SEMANTIC_ERR_TYPE;
                        break;

                    case T_ASSIGN:
                    case T_DEFINITION:
                        analysisResult = SYNTACTIC_ERR;
                        break;
                    
                    case T_UNDERSCORE:
                        analysisResult = SEMANTIC_ERR_UNDEF;
                        break;

                    case T_LEX_ERR:
                        analysisResult = LEXICAL_ERR;
                        break;

                    default:
                        analysisResult = SEMANTIC_ERR_TYPE;
                        break;
                }

            }while(!analysisResult);


        break;


        ///////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////ID///////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////////////////


        case T_ID:
        {
            iValue NameOfFnc;
            iNestValue tmp; // pointer na fnci
            vType typeOfID = BSTreeGetFromStack(&semanticToken.value.stringValue, &tmp, &myStack, &scope);
            ComplyingFNC:
            // TODO treba poriesit funkcie inputs...
            switch(typeOfID){
                case INT:
                    lastType = T_INT19;
                    semanticToken.isID = true;
                    break;

                case FLOAT:
                    lastType = T_F_FLOAT;
                    semanticToken.isID = true;
                    break;

                case STRING:
                    lastType = T_END_STR;
                    semanticToken.isID = true;
                    break;
                
                // uz Deklarovana funkcia
                case FNC_NAME:
                {
                    TOKEN FNC_nameToken;
                    copyTokenVal(&FNC_nameToken, &semanticToken);
                    
                    inFncCall = true;
                    getToken(&semanticToken); // ( zavorka
                    getToken(&semanticToken); // nejaky expr
                    analysisResult = AnalyseExpr();
                    
                    while(analysisResult == CODE_OK && semanticToken.type == T_COMA){
                        getToken(&semanticToken);
                        analysisResult = AnalyseExpr();
                        
                    }

                    if(typeOfID == FNC_NAME){

                        if(DefVar && InForLoop){
                            LinkedFncStart(&ForList);
                            if(ForID != labelNMB)
                                FncStart();
                        }
                        else
                            FncStart();
                    
                        int counter = 0;
                        // kontrolovanie parametrov funkcie
                        int iterator = tmp.functionPtr->parameters;

                        iValue exprResult;
                        vType exprType;

                        if (!iterator){
                            if(DefVar && InForLoop){
                                LinkedFncJump(&ForList, FNC_nameToken.value.stringValue.str);
                                if(ForID != labelNMB)
                                    FncJump(FNC_nameToken.value.stringValue.str);
                            }
                            else
                                FncJump(FNC_nameToken.value.stringValue.str);
                        }
                        

                        while(iterator > 0 && analysisResult == CODE_OK){
                            
                            --iterator;

                            if(itemStackEmpty(&idAssignStack))
                                analysisResult = SEMANTIC_ERR_PARAM;

                            else{
                                /*bool isID =*/ itemStackTopPop(&idAssignStack, &exprResult, &exprType);

                                if(DefVar && InForLoop){
                                    LinkedInsertParams(&ForList, &FNC_nameToken, counter++, tmp.functionPtr->parameters);
                                    if(ForID != labelNMB)
                                        FncParams(&FNC_nameToken, counter++, tmp.functionPtr->parameters);
                                }
                                else
                                    FncParams(&FNC_nameToken, counter++, tmp.functionPtr->parameters);
                                
                                if(tmp.functionPtr->FparamType[iterator] != exprType){
                                    analysisResult = SEMANTIC_ERR_PARAM;
                                }
                            }    
                            

                        }

                        if(!itemStackEmpty(&idAssignStack)){
                            analysisResult = SEMANTIC_ERR_PARAM;
                        }


                        else if(analysisResult == CODE_OK){
                            int retTypeNMB = tmp.functionPtr->rettypes;
                            iValue exprResult;
                            vType exprType;

                            while(iterator < retTypeNMB && analysisResult == CODE_OK){
                                exprType = tmp.functionPtr->FreturnType[iterator];
                                if(DefVar && InForLoop){
                                    LinkedPushRetValue(&ForList, iterator);
                                    if(ForID != labelNMB)
                                        pushRetValue(iterator);
                                }    
                                else
                                    pushRetValue(iterator);
                                itemStackPush(&idAssignStack, &exprResult, exprType, false);
                                ++iterator;
                            }

                            if(semanticToken.type != T_EOL && semanticToken.type != T_COMA){
                                if(retTypeNMB == 1){
                                    //potrebujem dat rettype na Tokenstack a zacat pokracovat v analyze
                                    //iValue temporary;
                                    itemStackTopPop(&idAssignStack, &exprResult, &exprType);
                                    copyiValue(&currentToken, &exprResult, exprType);
                                    tokenStackPush(&exprStack, &currentToken);
                                    
                                    switch(exprType){
                                        case INT:
                                            lastType = T_INT19;
                                            goto INT_expr;
                                            break;
                                        case FLOAT:
                                            lastType = T_F_FLOAT;
                                            goto FLOAT_expr;
                                            break;
                                        case STRING:
                                            lastType = T_END_STR;
                                            goto STR_expr;
                                            break;
                                        default:
                                            break;
                                    }
                                }
                                else{
                                    analysisResult = SEMANTIC_ERR_ELSE;
                                }
                                
                            }
                            else if(!tokenStackEmpty(&exprStack)){
                                if(retTypeNMB == 1){
                                    itemStackTopPop(&idAssignStack, &exprResult, &exprType);
                                    copyiValue(&currentToken, &exprResult, exprType);
                                    tokenStackPush(&exprStack, &currentToken);
                                    switch(exprType){
                                        case INT:
                                            lastType = T_INT19;
                                            goto endIntExpr;
                                            break;
                                        case FLOAT:
                                            lastType = T_F_FLOAT;
                                            goto endFloatExpr;
                                            break;
                                        case STRING:
                                            lastType = T_END_STR;
                                            goto endStrExpr;
                                            break;
                                        default:
                                            break;
                                    }
                                }
                                else{
                                    analysisResult = SEMANTIC_ERR_ELSE;
                                }
                            }

                        }

                        return analysisResult;
                    }
                }
                break;

                ///////////////NEDEFINOVANA FUNKCIA//////////////////

                // nacitame dalsi token a na zaklade toho ci je to ( vlozime do tabulky nedefinovanu fnci
                case TYPE_ERR:
                    getToken(&currentToken); // ( zavorka
                    switch (currentToken.type){
                        case T_L_RBRACKET:
                            // to ze funkcia ne je definovana sa mi automaticky vklada pri inserte
                            BSTreeInsert(&globalSymbols, &semanticToken.value.stringValue, &semanticToken.value, FNC_NAME, true);
                            strInit(&NameOfFnc.stringValue);
                            strCopyString(&NameOfFnc.stringValue, &semanticToken.value.stringValue);

                            // meno funkcie odlozime do dummy
                            strCopyString(&dummy, &semanticToken.value.stringValue);
                            typeOfID = FNC_NAME;
                            break;

                        default:
                            analysisResult = SEMANTIC_ERR_UNDEF;
                            break; 
                    }
                    break;


                default:
                    analysisResult = SEMANTIC_ERR_UNDEF;
                    break;
            }


            semanticToken.type = lastType;
            //na data stack pushni
            if(DefVar && InForLoop){
                LinkedPushID(&ForList, &semanticToken, tmp.nesting, scope);
                if(ForID != labelNMB)
                    pushID(&semanticToken.value.stringValue.str, tmp.nesting, scope);//??    
            }
            else
                pushID(&semanticToken.value.stringValue.str, tmp.nesting, scope);//??
            tokenStackPush(&exprStack, &semanticToken);
            getToken(&semanticToken);

    // tu je dalsi switch ktory by mohol byt uz vyssie ale opakoval by sa mi kod co nechcem
            if(analysisResult == CODE_OK){
                switch(typeOfID){
                    case FLOAT:
                        goto FLOAT_expr;             

                    case INT:
                        goto INT_expr;
                    
                // TODO
                    case STRING:
                        goto STR_expr;
                        

                    // nedefinovana fncia
                    case FNC_NAME:
                        {
                        inFncCall = true;
                        

                        getToken(&semanticToken); // nejaky expr
                        analysisResult = AnalyseExpr();
                        while(analysisResult == CODE_OK && semanticToken.type == T_COMA){
                            getToken(&semanticToken);
                            analysisResult = AnalyseExpr();
                            
                        }
                        iNestValue fncPtr;
                        typeOfID = BSTreeGet(&globalSymbols, &NameOfFnc.stringValue, &fncPtr);

                        if(DefVar && InForLoop){
                            LinkedFncStart(&ForList);
                            if(ForID != labelNMB)
                                FncStart();
                        }

                        iValue exprResult;
                        vType exprType;
                        TOKEN FNC_nameToken;
                        copyiValue(&FNC_nameToken, &NameOfFnc, STRING);
                        int counter = 0;

                        while(!itemStackEmpty(&idAssignStack) && analysisResult == CODE_OK){
                            

                            if(itemStackEmpty(&idAssignStack))
                                analysisResult = SEMANTIC_ERR_PARAM;
                                
                            else{
                                itemStackTopPop(&idAssignStack, &exprResult, &exprType);

                                if(DefVar && InForLoop){
                                    LinkedInsertParams(&ForList, &FNC_nameToken, counter++, 100);
                                    if(ForID != labelNMB)
                                        FncParams(&FNC_nameToken, counter++, 100);
                                }
                                else
                                    FncParams(&FNC_nameToken, counter++, 100);
                                
                            }    

                        }
                        FncJump(NameOfFnc.stringValue.str);

                        DeriveFromIds();
                        }
                        break;

                    default:
                        analysisResult = SEMANTIC_ERR_TYPE;
                        break;
                }
            }

        }

        break;

        case T_EOL:
            while(semanticToken.type == T_EOL){
                getToken(&semanticToken);
            }
            goto brackets;
            break;

        case T_L_RBRACKET:
            tokenStackPush(&exprStack, &semanticToken);
            lastType = semanticToken.type;
            getToken(&semanticToken);
            goto brackets;
            break;

        case T_R_RBRACKET:
            if(inFncCall){
                lastType = semanticToken.type;
                getToken(&semanticToken);
            }
            else
                analysisResult = SYNTACTIC_ERR;
            break;

        case T_UNDERSCORE:
            analysisResult = SEMANTIC_ERR_UNDEF;
            break;

        case T_LEX_ERR:
            analysisResult = LEXICAL_ERR;
            break;

        default:
            analysisResult = SYNTACTIC_ERR;
            break;

    }
   
    return analysisResult;
}

//////////////////////////////////////////////////////

/**
 * Rekurzivna funkcia ktora odvodi return typy nedefinovanej funkcie z id na lavej strane
 * vsetky id musia byt definovane... inak TO BUDEME ROBIT ODZNOVA
 */
void DeriveFromIds(){

    iValue CurrRetTypeName;
    vType CurrRetType;

    strInit(&CurrRetTypeName.stringValue);
    itemStackTopPop(&idStack, &CurrRetTypeName, &CurrRetType); // skopirujeme si idcko
    CurrRetType = BSTreeGetType(&CurrRetTypeName.stringValue, &myStack); // zistime typ idcka

    if(DefVar && InForLoop){
        LinkedPushRetValue(&ForList, derivehelper++);
        if(ForID != labelNMB)
            pushRetValue(derivehelper++);
    }    
    else
        pushRetValue(derivehelper++);

    if(!itemStackEmpty(&idStack)){
        DeriveFromIds();
    }

    BSTreeFuncAddRetType(&globalSymbols, &dummy, CurrRetType); // vlozim parameter na stack

    itemStackPush(&idStack, &CurrRetTypeName, STRING, true); // vratime id na stack
    strFree(&CurrRetTypeName.stringValue);
}









/////////////////////////////////////////////////////


void tokenStackInit (tStackptr* top)
{

	*top = NULL;
}


int tokenStackPush (tStackptr* top, TOKEN *inserted){

	tStackptr new = malloc (sizeof(struct tStack));
	if (new == NULL){
		fprintf(stderr, "Podtecenie stacku -- nedostatok pamete, tabulka nebola vlozena na stack");
		return ALLOC_ERR;
	}
	//TokenCopy
    new->stackedToken.type = inserted->type;
    new->stackedToken.value.stringValue.str = NULL;
    new->stackedToken.isID = inserted->isID;
    
    copyTokenVal(&new->stackedToken, inserted);

		new->next = *top;
		*top = new;
	return TABLE_OK;
}

void tokenStackTopPop (tStackptr* top, TOKEN *poped){
    poped->type = (*top)->stackedToken.type;
    poped->isID = (*top)->stackedToken.isID;

    copyTokenVal(poped, &(*top)->stackedToken);

    tStackptr newTop = (*top)->next;
		free(*top);
		*top = newTop;
}

void tokenStackPop(tStackptr* top){
    tStackptr newTop = (*top)->next;
		free(*top);
		*top = newTop;
}

bool tokenStackEmpty (tStackptr* top){
    return (*top == NULL);
}

int copyTokenVal(TOKEN* target, TOKEN* source){
    if (!target->isID && !source->isID){
    switch (source->type)
        {
        case T_END_STR:
        case T_KEYWORD:
        case T_ID:	
        case KeyWord_else:
        case KeyWord_float64:
        case KeyWord_for:
        case KeyWord_func:
        case KeyWord_if:
        case KeyWord_int:
        case KeyWord_package:
        case KeyWord_return:
        case KeyWord_string:
            strInit(&target->value.stringValue);
            strCopyString(&target->value.stringValue, &source->value.stringValue);
            break;
        default:
            target->value = source->value;
            break;
        }
    }
    else{
        strInit(&target->value.stringValue);
        strCopyString(&target->value.stringValue, &source->value.stringValue);
    }
    
    return TABLE_OK;
}

int copyiValue(TOKEN* target, iValue* source, vType type){
    switch (type)
    {

        case INT:
            target->value.integerValue = source->integerValue;
            target->type = T_INT19;
            break;

        case FLOAT:
            target->value.floatValue = source->floatValue;
            target->type = T_F_FLOAT;
            break;
        case STRING:
            strInit(&target->value.stringValue);
            strCopyString(&target->value.stringValue, &source->stringValue);
            break;

    default:
        // TODO
        break;
    }
    return TABLE_OK;
}

//////////////////////////////////////////////////////////////////

void itemStackInit (itemStackptr* top){
    *top = NULL;
}

int itemStackPush (itemStackptr* top, iValue* itemVal, vType itemType, bool DefinedID){

    itemStackptr new = malloc (sizeof(struct itemStack));
	if (new == NULL){
		fprintf(stderr, "Podtecenie stacku -- nedostatok pamete, tabulka nebola vlozena na stack");
		return ALLOC_ERR; // ERR?
	}

    if(itemType == STRING){
        if(strInit(&new->itemValue.stringValue)
           || strCopyString(&new->itemValue.stringValue, &itemVal->stringValue))
           return ALLOC_ERR;
    }
    else
        new->itemValue = *itemVal;
    
    new->itemType = itemType;
    
    new->isID = DefinedID;
    
    
		new->next = *top;
		*top = new;
	return TABLE_OK;
}

bool itemStackTopPop (itemStackptr* top, iValue* itemVal, vType* itemType){

    if((*top)->itemType == STRING){
        strInit(&itemVal->stringValue);
        strCopyString(&itemVal->stringValue, &(*top)->itemValue.stringValue);
    }
    else
        *itemVal = (*top)->itemValue;

    *itemType = (*top)->itemType;
    
    bool isID = (*top)->isID;


    if((*top)->itemType == STRING){
        strFree(&(*top)->itemValue.stringValue);
    }
    itemStackptr newTop = (*top)->next;
		free(*top);
		*top = newTop;

    return isID;
}

void itemStackPop(itemStackptr* top){
    if((*top)->itemType == STRING){
        strFree(&(*top)->itemValue.stringValue);
    }
    itemStackptr newTop = (*top)->next;
		free(*top);
		*top = newTop;
}


bool itemStackEmpty (itemStackptr* top){
    return (*top == NULL);
}


//And yet another stack
void itemIDStackTopPop (itemStackptr* top, string* id){
    strCopyString(id, &(*top)->itemValue.stringValue);

//musi tu byt check
    if((*top)->itemType == STRING){
        strFree(&(*top)->itemValue.stringValue);
    }
    itemStackptr newTop = (*top)->next;
		free(*top);
		*top = newTop;
}