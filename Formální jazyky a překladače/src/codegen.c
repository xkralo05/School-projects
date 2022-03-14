/**
 * @name Code generator IFJ20
 * @author Timotej Ponek xponek00 , Kristian Kralovic xkralo05 , Marek Valko xvalko11
 * @file codegen.c
 * @date 22.11.2020
 */

// cislo zvolene na zaklade poctu charov v max velkom int
// a poctu int v Deklaracii
#define DECLsize 75
#define Isize 20

//#pragma once
#include "stdio.h"
#include "str.h"
#include "symtable.h"
#include "codegen.h"
#include "malloc.h"

void popTrash(){
	fprintf(stdout,"POPS GF@Bin\n");
}

/*
 * Funkcia sluzi na ukladanie hodnty na datovy zasobnik
 * Parameter name  obsahuje nazov premennej ktora sa ma ulozit
*/

void pushID(char** name, int label, int scope){
	if(scope == 1){
		label = 1;
	}
	fprintf(stdout, "PUSHS LF@?_%d_%d%s\n", label, scope, *name);
}

void pushRetValue(int counter){
	fprintf(stdout, "PUSHS TF@?result%d\n", counter);
}
/*
 * Funkcia sluzi na ukladanie hodnty na datovy zasobnik
 * Parameter name  obsahuje string ktory sa ma ulozit
*/
void pushstring(char** name){
	fprintf(stdout, "PUSHS string@%s\n", *name);
}
/*
 * Funkcia sluzi na ukladanie hodnty na datovy zasobnik
 * Parameter value obsahuje hodnotu ktora sa ma ulozit
*/
void pushInt(long long value){
	fprintf(stdout, "PUSHS int@%lld\n", value);
}
/*
 * Funkcia sluzi na ukladanie hodnty na datovy zasobnik
 * Parameter value obsahuje hodnotu ktora sa ma ulozit
*/
void pushFloat(double value){
	fprintf(stdout, "PUSHS float@%a\n", value);
}
/*
 * Funkcia sluzi na zasobnikovy plus pri vyrazoch
*/
void add(){
	fprintf(stdout,"ADDS\n");
}
/*
 * Funkcia sluzi na zasobnikovy minus pri vyrazoch
*/
void sub(){
	fprintf(stdout,"SUBS\n");
}
/*
 * Funkcia sluzi na zasobnikove nasobenie pri vyrazoch
*/
void mul(){
	fprintf(stdout,"MULS\n");
}
/*
 * Funkcia sluzi na zasobnikove delenie  pri vyrazoch
*/
void divFloat(){
	fprintf(stdout,"DIVS\n");
}
/*
 * Funkcia sluzi na zasobnikove delenie pri vyrazoch
*/
void divInt(){
	fprintf(stdout,"IDIVS\n");
}
/*
 * Funkcia sluzi na vymazanie datoveho zasobnika
*/
void ClearS(){
	fprintf(stdout, "CLEARS\n");
}
/*
 * Funckia sluzi na porovnanie >
*/
void greater(){
	fprintf(stdout, "GTS\n");
}
/*
 * Funckia sluzi na porovnanie <
*/
void lesser(){
	fprintf(stdout, "LTS\n");
}
/*
 * Funckia sluzi na porovnanie ==
*/
void equal(){
	fprintf(stdout, "EQS\n");
}

/*
 * Funckia sluzi na porovnanie !=
*/
void notequal(){
	fprintf(stdout, "EQS\n");
	fprintf(stdout, "NOTS\n");
}

/*
 * Funckia sluzi na porovnanie >=
*/
void greaterEqual(){
	fprintf(stdout, "LTS\n");
	fprintf(stdout, "NOTS\n");
}

/*
 * Funckia sluzi na porovnanie <=
*/
void lesserEqual(){
	fprintf(stdout, "GTS\n");
	fprintf(stdout, "NOTS\n");
}

/* Funkcia StrConcatenation sluzi ako riesenie konkatenacie retazcov 
 * Parameter tokenA obsahuje string ktory sa ma konkatenovat
*/
void StrConcatenation(TOKEN *tokenA, int label, int scope)
{
	fprintf(stdout, "CREATEFRAME\n");
	fprintf(stdout, "DEFVAR TF@**$concatedstring\n");
	fprintf(stdout,"POPS TF@**$concatedstring\n");
	if(tokenA->isID == true)
		fprintf(stdout, "CONCAT TF@**$concatedstring TF@**$concatedstring LF@?_%d_%d%s\n",label, scope, tokenA->value.stringValue.str);
	else
		fprintf(stdout, "CONCAT TF@**$concatedstring TF@**$concatedstring string@%s\n",tokenA->value.stringValue.str);
	fprintf(stdout, "PUSHS  TF@**$concatedstring\n");
}
/*
 * Funkcia sluzi na definovanie novej premennej
 * Parameter tokenA obsahuje nazov premennej
*/
void VarDefine(TOKEN *tokenA, int label, int scope) // tokenA ID , tokenB hodnota
{
	if(scope == 1){
		label = 1;
	}
	fprintf(stdout, "DEFVAR LF@?_%d_%d%s\n",label, scope, tokenA->value.stringValue.str);
	fprintf(stdout, "POPS LF@?_%d_%d%s\n",label, scope, tokenA->value.stringValue.str);
}
/*
 * Funkcia sluzi na na vypis if casti id -else
 * Parameter ifcounter urcuje v ktorom tele if sa nachadzame (pri vnorenych IF-och)
*/
void printIF(int ifcounter)
{
	fprintf(stdout, "\nDEFVAR LF@?ifcond%d\n", ifcounter); // 	fprintf(stdout, "\n");
	fprintf(stdout, "POPS LF@?ifcond%d\n",ifcounter);
	fprintf(stdout, "JUMPIFEQ $IFLABEL%d LF@?ifcond%d bool@true\n" ,ifcounter,ifcounter);
	fprintf(stdout, "JUMPIFNEQ $ELSELABEL%d LF@?ifcond%d bool@true\n",ifcounter,ifcounter);
	fprintf(stdout, "LABEL $IFLABEL%d\n",ifcounter);

}
/*
 * Funkcia sluzi na vypis zaciatku else casti If-else
 * Parameter ifcounter urcuje v ktorom tele if sa nachadzame (pri vnorenych IF-och)
*/
void printELSE(int ifcounter)
{
	fprintf(stdout, "LABEL $ELSELABEL%d\n", ifcounter);
}
/*
 * Funkcia sluzi na ukoncenie tela IF
 * Parameter ifcounter urcuje v ktorom tele if sa nachadzame (pri vnorenych IF-och)
*/
void printELSEend(int ifcounter)
{
	fprintf(stdout, "LABEL $ENDIFELSE%d\n", ifcounter);
}
/*
 * Funkcia sluzi na ukoncenie tela IF
 * Parameter ifcounter urcuje v ktorom tele if sa nachadzame (pri vnorenych IF-och)
*/

void printIFjump(int ifcounter)
{
	fprintf(stdout, "JUMP $ENDIFELSE%d\n", ifcounter);
}
/*
 * Funkcia sluzi na priradenie parametra funkcie do premennej vo vnutri funkcie
 * Parameter tokenA obsahuje nazov premennej
 * Parameter conter sluzi na priradenie spravneho parametra
 */
void VarDefineFncParam(TOKEN *tokenA , int counter) // tokenA ID , tokenB hodnota
{
	fprintf(stdout, "DEFVAR LF@?_1_1%s\n", tokenA->value.stringValue.str);
	fprintf(stdout, "MOVE LF@?_1_1%s LF@?param%d\n", tokenA->value.stringValue.str,counter - 1);
}

/*
 * Funkcia sluzi na definovanie novej premennej a nasledne priradenie hodnoty do novo vzniknutej premennej
 * Parameter tokenA obsahuje nazov premennej
 * Parameter tokenB obsahuje nazov druhej premennej
*/ 
void VarDefineID(TOKEN *tokenA ,int label, int scope) // tokenA ID , tokenB hodnota
{
	if(scope == 1){
		label = 1;
	}
	fprintf(stdout, "DEFVAR LF@?_%d_%d%s\n",label, scope, tokenA->value.stringValue.str);
	fprintf(stdout, "POPS LF@?_%d_%d%s\n",label, scope, tokenA->value.stringValue.str);
	
}
/*
 * Funkcia sluzi na priradenie hodnoty do predom definovanej premennej
 * Parameter tokenA obsahuje nazov premennej
*/
void VarAssign(TOKEN *tokenA, int label, int scope)
{
	if(scope == 1){
		label = 1;
	}
	fprintf(stdout, "POPS LF@?_%d_%d%s\n",label, scope, tokenA->value.stringValue.str);
}
/*
 * Funkcia sluzi na priradenie navratovej hodnoty do predom definovanej premennej
 * Parameter tokenA obsahuje nazov premennej
 * Parameter counter sluzi na priradenie spravnej navratovej hodnoty
*/
void VarAssignFNC(TOKEN *tokenA, int counter, int label, int scope){
	if(scope == 1){
		label = 1;
	}
	fprintf(stdout, "MOVE LF@?_%d_%d%s TF@?result%d\n",label, scope, tokenA->value.stringValue.str,counter);
}
/*
 * Funkcia sluzi na definovanie premennej a priradenie navratovej hodnoty uzivatelskej funkcie do novo vytvorenej premennej
 * Parameter tokenA obsahuje nazov premennej
 * Parameter counter sluzi na priradenie spravnej navratovej hodnoty
*/
void VarDefineFNC(TOKEN *tokenA, int counter, int label, int scope){
	if(scope == 1){
		label = 1;
	}
	fprintf(stdout, "DEFVAR LF@?_%d_%d%s\n",label, scope, tokenA->value.stringValue.str);
	fprintf(stdout, "MOVE LF@?_%d_%d%s TF@?result%d\n",label, scope, tokenA->value.stringValue.str,counter);
}

/*
 * Funcia sluzi na vypisanie zaciatku programu v jazyku IFJcode20
*/
void PrintMain ()
{
	fprintf(stdout, "DEFVAR GF@Bin\n");
	fprintf(stdout,"CREATEFRAME\n");
	fprintf(stdout, "JUMP *?MAIN\n");
}
/*
 * Funcia sluzi na vypisanie zaciatku funkcie main
*/
void PrintMain2(){
	fprintf(stdout,"LABEL *?MAIN\n");	
	fprintf(stdout, "PUSHFRAME\n");
	fprintf(stdout,"CREATEFRAME\n"); // pre if podmienky
}
/*
 * Funcia sluzi na vypisanie skoku na koniec funkcie main
*/
void PrintEndMainJump()
{
	fprintf(stdout, "JUMP $?ENDOFMAIN\n");
}
/*
 * Funkcia sluzi na vypis konca programu v jazyku IFJcode20 a zaroven ukoncuje funkciu main
*/
void PrintEndMain()
{ 
	fprintf(stdout, "LABEL $?ENDOFMAIN\n");
	fprintf(stdout,"POPFRAME\n");
	fprintf(stdout,"CLEARS\n");
}
/*
 * Funkcia sluzi na vytvorenie TF pred volanim funkcie
*/
void FncStart()
{
	fprintf(stdout, "CREATEFRAME\n");
	//fprintf(stdout, "LABEL *fnc%s\n",tokenA->value.stringValue.str);

}
/*
 * Funckia sluzi na ukladanie parametrov pri volani uzivatelskych funkcii
 * Parameter FncName obsahuje nazov volanej funkcie
 * Parameter counter sluzi na na priradovanie hodnot do spravneho parametru
 * Parameter numberOfParams obsahuje pocet parametrov uzivatelskej funkcie 
*/
void FncParams(TOKEN *FncName, int counter , int numberOfParams)  /// PRIDAT MENO FUNKCIE 
{
	fprintf(stdout, "DEFVAR TF@?param%d\n", numberOfParams - counter - 1);
	fprintf(stdout, "POPS TF@?param%d\n",numberOfParams - counter - 1);
	
	if(counter == numberOfParams-1) /// pocet argumentov bude finalny
		{
			FncJump(FncName->value.stringValue.str);
		}
}

/*
 * Funcia sluzi pre vypis vstavanej funkcie print
 * Parameter tokenA predava hodnotu ktora sa ma vytlacit
 * Parameter type urcuje typ (INT, FLOAT, STRING) hodnoty ktora sa ma vytlacit
*/


void BuildInPrintTerms(int counter){
	fprintf(stdout, "DEFVAR TF@?print?%d\n", counter);
	fprintf(stdout, "POPS TF@?print?%d\n", counter);
}

void BuildInPrint(/*TOKEN *tokenA , vType type, int label, int scope*/ int counter)
{	
	fprintf(stdout, "WRITE TF@?print?%d\n", counter);
}

/*
 * Funkcia sluzi na ukladanie navratovych hodnot do premennych ktore sluzia ako navratove hodnoty funkcie
 * Parameter type urcuje typ (INT, FLOAT, STRING) hodnoty ktora sa uklada do premennej ktora sluzi ako navratova hodnota funkcie
 * Parameter counter sluzia ako ocislovanie navratovej hodnoty a nadobuda hodnoty podla poctu vystupnych parametrov funkcie
*/
void FncReturn(TOKEN *tokenA, vType type , int counter )  /// PRIDAT MENO FUNKCIE 
{
	//TODO vymazat nepotrebene veci
	if(tokenA->isID){
		fprintf(stdout, "POPS LF@?result%d\n",counter);
	}
	else{
		switch(type)
		{
			case INT:
					fprintf(stdout, "POPS LF@?result%d\n", counter);		
			break;

			case FLOAT:
					fprintf(stdout, "POPS LF@?result%d\n", counter);
			break;

			case STRING:
					fprintf(stdout, "POPS LF@?result%d\n", counter);
			break;

			default :
			
			break;
			
			
		}
	}
}
/*
 * Funkcia pre vytvorenie premennych ktore sluzia ako navratove hodnoty funkcie
 * Parameter counter sluzia ako ocislovanie navratovej hodnoty a nadobuda hodnoty podla poctu vystupnych parametrov funkcie
*/ 
void FncReturnInit(int counter){
	fprintf(stdout, "DEFVAR LF@?result%d\n",counter- 1);
	fprintf(stdout, "MOVE LF@?result%d nil@nil\n",counter- 1);
}
	
/*
 * Funkcia pre volanie uzivatelskej funkcie
 * Parameter Name obsahuje nazov uzivatelskej funkcie
*/ 	
void FncJump(char* Name)
{
	fprintf(stdout, "CALL $HELP1%s\n\n",Name);
}
/*
 * Funkcia pre vytvorenie tela uzivatelskej  funkcie
 * Parameter tokenA obsahuje nazov uzivatelskej funkcie
*/
void FncBody(TOKEN *tokenA) // telo funkcie
{
	fprintf(stdout, "LABEL $HELP1%s\n",tokenA->value.stringValue.str);
	fprintf(stdout, "PUSHFRAME\n");
	fprintf(stdout, "CREATEFRAME\n");
}
/*
 * Funkcia pre ukoncenie a navrat z funckie 
*/ 
void FncEndBody()
{
	fprintf(stdout, "POPFRAME\n");
	fprintf(stdout, "RETURN\n\n");
}

/*
 * Funkcia pre vypis vstavanych funkcii
 * Funkcia vypise vstavane funkcie napisane v jazyku IFJcode20
*/
void PrintBuiltInFunc(){
	fprintf(stdout,"# Definicia vstavanej funkcie  len(string) (int)\n");
	fprintf(stdout, "LABEL $HELP1len\n");
	fprintf(stdout, "PUSHFRAME\n");
	fprintf(stdout, "DEFVAR LF@?result0\n"); // v res bude int hodnota dlzky retazca
	fprintf(stdout, "DEFVAR LF@?string\n"); // v string bude ulozeny retazec 
	fprintf(stdout, "MOVE LF@?string LF@?param0\n"); // presuniem do string hodnotu z premennej $0 ulozenej v LF (ukladam si ju ked volam funkciu)
	fprintf(stdout, "STRLEN LF@?result0 LF@?string\n");
	fprintf(stdout, "POPFRAME\n");
	fprintf(stdout, "RETURN\n");

	fprintf(stdout, "\n");
	
	fprintf(stdout,"# Definicia vstavanej funkcie int2float(i int) (float64)\n");
	fprintf(stdout, "LABEL $HELP1int2float\n");
	fprintf(stdout, "PUSHFRAME\n");
	fprintf(stdout, "DEFVAR LF@?result0\n");
	fprintf(stdout, "DEFVAR LF@?vali\n");
	fprintf(stdout, "DEFVAR LF@?typechecki\n");
	fprintf(stdout, "MOVE LF@?vali LF@?param0\n");
	fprintf(stdout, "TYPE LF@?typechecki LF@?vali\n");
	fprintf(stdout, "JUMPIFEQ $goodint LF@?typechecki string@int\n");
	fprintf(stdout, "JUMPIFNEQ $notint LF@?typechecki string@int\n");
	fprintf(stdout, "LABEL $goodint\n");
	fprintf(stdout, "INT2FLOAT LF@?result0 LF@?vali\n");
	fprintf(stdout, "JUMP $endint2float\n");
	fprintf(stdout, "LABEL $notint\n");
	fprintf(stdout, "JUMP $endint2float\n");
	fprintf(stdout, "LABEL $endint2float\n");
	fprintf(stdout, "POPFRAME\n");
	fprintf(stdout, "RETURN\n");

	fprintf(stdout, "\n");

	fprintf(stdout,"# Definicia vstavanej funkcie float2int(ffloat64) (int)\n");
	fprintf(stdout, "LABEL $HELP1float2int\n");
	fprintf(stdout, "PUSHFRAME\n");
	fprintf(stdout, "DEFVAR LF@?result0\n");
	fprintf(stdout, "DEFVAR LF@?valf\n");
	fprintf(stdout, "DEFVAR LF@?typecheckf\n");
	fprintf(stdout, "MOVE LF@?valf LF@?param0\n");
	fprintf(stdout, "TYPE LF@?typecheckf LF@?valf\n");
	fprintf(stdout, "JUMPIFEQ $goodfloat LF@?typecheckf string@float\n");
	fprintf(stdout, "JUMPIFNEQ $notfloat LF@?typecheckf string@float\n");
	fprintf(stdout, "LABEL $goodfloat\n");
	fprintf(stdout, "FLOAT2INT LF@?result0 LF@?valf\n");
	fprintf(stdout, "JUMP $endfloat2int\n");
	fprintf(stdout, "LABEL $notfloat\n");
	fprintf(stdout, "JUMP $endfloat2int\n");
	fprintf(stdout, "LABEL $endfloat2int\n");
	fprintf(stdout, "POPFRAME\n");
	fprintf(stdout, "RETURN\n");

	fprintf(stdout, "\n");

	fprintf(stdout,"# Definicia vstavanej funkcie func chr(i int) (string, int)\n");
	fprintf(stdout, "LABEL $HELP1chr\n");
	fprintf(stdout, "PUSHFRAME\n");
	fprintf(stdout, "DEFVAR LF@?result0\n");
	fprintf(stdout, "DEFVAR LF@?result1\n"); 
	fprintf(stdout, "DEFVAR LF@?load\n");
	fprintf(stdout, "MOVE LF@?load LF@?param0\n");
	fprintf(stdout, "DEFVAR LF@?isless\n");  			
	fprintf(stdout, "MOVE LF@?isless int@256\n");
	fprintf(stdout, "DEFVAR LF@?islessbool\n");
	fprintf(stdout, "LT LF@?islessbool LF@?load LF@?isless\n");
	fprintf(stdout, "JUMPIFEQ $badint LF@?islessbool bool@false\n");
	fprintf(stdout, "DEFVAR LF@?isgreatbool\n");
	fprintf(stdout, "GT LF@?isgreatbool LF@?load int@-1 \n");
	fprintf(stdout, "JUMPIFEQ $*badint LF@?isgreatbool bool@false\n");
	fprintf(stdout, "INT2CHAR LF@?result0 LF@?load\n");
	fprintf(stdout, "MOVE LF@?result1 int@0\n");
	fprintf(stdout, "JUMP $endchr \n");
	fprintf(stdout, "LABEL $badint\n");
	fprintf(stdout, "MOVE LF@?result1 int@1\n");
	fprintf(stdout, "MOVE LF@?result0 string@BADINPUT\n");
	fprintf(stdout, "JUMP $endchr\n");
	fprintf(stdout, "LABEL $endchr\n");	
	fprintf(stdout, "POPFRAME\n");
	fprintf(stdout, "RETURN\n");

	fprintf(stdout, "\n");	

	fprintf(stdout,"# Definicia vstavanej funkcie func ord(s string,i int) (int, int)\n");  // fprintf(stdout, "\n");
	fprintf(stdout, "LABEL $HELP1ord\n");
	fprintf(stdout, "PUSHFRAME\n");
	fprintf(stdout, "DEFVAR LF@?result1\n");
	fprintf(stdout, "DEFVAR LF@?result0\n");
	fprintf(stdout, "DEFVAR LF@?loadstringord\n");
	fprintf(stdout, "DEFVAR LF@?loadintord\n");
	fprintf(stdout, "MOVE LF@?loadintord LF@?param1\n");
	fprintf(stdout, "MOVE LF@?loadstringord LF@?param0\n");
	fprintf(stdout, "DEFVAR LF@?lengthofstr \n");
	fprintf(stdout, "STRLEN LF@?lengthofstr LF@?loadstringord\n");
	fprintf(stdout, "SUB LF@?lengthofstr  LF@?lengthofstr int@1\n");
	fprintf(stdout, "DEFVAR LF@?gtchceck\n");
	fprintf(stdout, "LT LF@?gtchceck LF@?lengthofstr LF@?loadintord\n");
	fprintf(stdout, "JUMPIFNEQ $badinputint LF@?gtchceck bool@false\n");
	fprintf(stdout, "STRI2INT LF@?result0 LF@?loadstringord LF@?loadintord\n");
	fprintf(stdout, "MOVE LF@?result1 int@0\n");
	fprintf(stdout, "JUMP $endord\n");
	fprintf(stdout, "LABEL $badinputint\n");
	fprintf(stdout, "MOVE LF@?result1 int@1\n");
	fprintf(stdout, "MOVE LF@?result0 int@99999\n");
	fprintf(stdout, "JUMP $endord\n");
	fprintf(stdout, "LABEL $endord\n");
	fprintf(stdout, "POPFRAME\n");
	fprintf(stdout, "RETURN\n");

	fprintf(stdout, "\n");

	fprintf(stdout,"# Definicia vstavanej funkcie  inputs() (string,int) \n");
	fprintf(stdout, "LABEL $HELP1inputs\n");
	fprintf(stdout, "PUSHFRAME\n");
	fprintf(stdout, "DEFVAR LF@?result1\n");
	fprintf(stdout, "DEFVAR LF@?result0\n");
	fprintf(stdout, "DEFVAR LF@?savedstring\n");
	fprintf(stdout, "MOVE LF@?savedstring nil@nil\n");
	fprintf(stdout, "READ LF@?savedstring string\n");
	//fprintf(stdout, "JUMPIFEQ $badinputtypes LF@?savedstring nil@nil\n");
	fprintf(stdout, "MOVE LF@?result1 int@0\n");
	fprintf(stdout, "MOVE LF@?result0 LF@?savedstring\n");
	
	//fprintf(stdout, "LABEL $badinputtypes\n");
	//fprintf(stdout, "MOVE LF@?result1 int@1\n");
	//fprintf(stdout, "MOVE LF@?result0 string@BADINPUT\n");
	fprintf(stdout, "JUMP $endinputs\n");
	
	fprintf(stdout, "LABEL $endinputs\n");
	fprintf(stdout, "POPFRAME\n");
	fprintf(stdout, "RETURN\n");

	fprintf(stdout, "\n");

	fprintf(stdout,"# Definicia vstavanej funkcie  inputf() (float64,int) \n");
	fprintf(stdout, "LABEL $HELP1inputf\n");
	fprintf(stdout, "PUSHFRAME\n");
	fprintf(stdout, "DEFVAR LF@?result1\n");
	fprintf(stdout, "DEFVAR LF@?result0\n");
	fprintf(stdout, "DEFVAR LF@?savednumberf\n");
	fprintf(stdout, "DEFVAR LF@?typecheckff0\n");
	fprintf(stdout, "DEFVAR LF@?typecheckff1\n");
	fprintf(stdout, "MOVE LF@?typecheckff0 string@float\n");
	fprintf(stdout, "READ LF@?savednumberf float\n");
	fprintf(stdout, "TYPE LF@?typecheckff1 LF@?savednumberf\n");
	fprintf(stdout, "JUMPIFNEQ $badinputtypef LF@?typecheckff1 LF@?typecheckff0\n");
	fprintf(stdout, "MOVE LF@?result1 int@0\n");
	fprintf(stdout, "MOVE LF@?result0 LF@?savednumberf\n");
	fprintf(stdout, "JUMP $endinputf\n");
	fprintf(stdout, "LABEL $badinputtypef\n");
	fprintf(stdout, "MOVE LF@?result1 int@1\n");
	fprintf(stdout, "MOVE LF@?result0 float@0x0.0000000000000p+0\n");
	fprintf(stdout, "JUMP $endinputf\n");
	fprintf(stdout, "LABEL $endinputf\n");
	fprintf(stdout, "POPFRAME\n");
	fprintf(stdout, "RETURN\n");
	
	fprintf(stdout, "\n");

	fprintf(stdout,"# Definicia vstavanej funkcie  inputi() (int,int) \n");
	fprintf(stdout, "LABEL $HELP1inputi\n");
	fprintf(stdout, "PUSHFRAME\n");
	fprintf(stdout, "DEFVAR LF@?result1\n");
	fprintf(stdout, "DEFVAR LF@?result0\n");
	fprintf(stdout, "DEFVAR LF@?savednumberi\n");
	fprintf(stdout, "DEFVAR LF@?typecheckfi0\n");
	fprintf(stdout, "DEFVAR LF@?typecheckfi1\n");
	fprintf(stdout, "MOVE LF@?typecheckfi0 string@int\n");
	fprintf(stdout, "READ LF@?savednumberi int\n");
	fprintf(stdout, "TYPE LF@?typecheckfi1 LF@?savednumberi\n");
	fprintf(stdout, "JUMPIFNEQ $badinputtypei LF@?typecheckfi1 LF@?typecheckfi0\n");
	fprintf(stdout, "MOVE LF@?result1 int@0\n");
	fprintf(stdout, "MOVE LF@?result0 LF@?savednumberi\n");
	fprintf(stdout, "JUMP $endinputi\n");
	fprintf(stdout, "LABEL $badinputtypei\n");
	fprintf(stdout, "MOVE LF@?result1 int@1\n");
	fprintf(stdout, "MOVE LF@?result0 int@0\n");
	fprintf(stdout, "JUMP $endinputi\n");
	fprintf(stdout, "LABEL $endinputi\n");
	fprintf(stdout, "POPFRAME\n");
	fprintf(stdout, "RETURN\n");
	fprintf(stdout, "\n");
	
	fprintf(stdout,"# Definicia vstavanej funkcie  substr (string,int) \n"); // fprintf(stdout,"\n");
	fprintf(stdout,"LABEL $HELP1substr\n");
	fprintf(stdout,"PUSHFRAME\n");
	fprintf(stdout,"DEFVAR LF@?result1\n");
	fprintf(stdout,"DEFVAR LF@?result0\n");;
	fprintf(stdout,"DEFVAR LF@?loadstringsub\n");
	fprintf(stdout,"DEFVAR LF@?loadfirstint\n");
	fprintf(stdout,"DEFVAR LF@?loadsecondtint\n");
	fprintf(stdout,"DEFVAR LF@?lengthofstring\n");
	fprintf(stdout,"DEFVAR LF@?lengthcheck1\n");
	fprintf(stdout,"DEFVAR LF@?lengthcheck2\n");
	fprintf(stdout,"DEFVAR LF@?lengthcheck3\n");
	fprintf(stdout,"DEFVAR LF@?lengthcheck4\n");
	fprintf(stdout,"DEFVAR LF@?lengthofsubstr\n");
	fprintf(stdout, "DEFVAR LF@?EQiterator\n");
	fprintf(stdout, "DEFVAR LF@?GETCHARparam\n");
	fprintf(stdout, "DEFVAR LF@?ITERATORsubstr\n");
	fprintf(stdout, "MOVE LF@?ITERATORsubstr int@0\n");
	fprintf(stdout,"MOVE LF@?loadstringsub LF@?param0\n");
	fprintf(stdout,"MOVE LF@?loadfirstint LF@?param1\n"); 
	fprintf(stdout,"MOVE LF@?loadsecondtint LF@?param2\n");
	fprintf(stdout,"MOVE LF@?result0 int@0\n");
	fprintf(stdout,"MOVE LF@?result1 string@\n");
	fprintf(stdout,"STRLEN LF@?lengthofstring LF@?loadstringsub\n");
	fprintf(stdout, "MOVE LF@?lengthofsubstr LF@?lengthofstring\n");
	fprintf(stdout,"LT LF@?lengthcheck1 LF@?lengthofstring LF@?loadfirstint\n");
	fprintf(stdout,"JUMPIFEQ $badinputsubstr LF@?lengthcheck1 bool@true\n");
	fprintf(stdout,"LT LF@?lengthcheck2  LF@?loadfirstint int@0\n");
	fprintf(stdout,"JUMPIFEQ $badinputsubstr LF@?lengthcheck2 bool@true\n");
	fprintf(stdout,"LT LF@?lengthcheck3  LF@?loadsecondtint int@0\n");
	fprintf(stdout,"JUMPIFEQ $badinputsubstr LF@?lengthcheck3 bool@true\n");
	fprintf(stdout,"SUB LF@?lengthofsubstr LF@?lengthofsubstr LF@?loadfirstint\n");
	fprintf(stdout,"GT LF@?lengthcheck4  LF@?loadsecondtint LF@?lengthofsubstr\n");
	fprintf(stdout,"JUMPIFEQ $tillendsubstr LF@?lengthcheck4 bool@true\n");
	fprintf(stdout,"LABEL $for\n");
	fprintf(stdout, "EQ LF@?EQiterator LF@?ITERATORsubstr LF@?loadsecondtint\n");
	fprintf(stdout, "JUMPIFEQ $ENDSUBSTRING LF@?EQiterator bool@true\n");
	fprintf(stdout,"GETCHAR LF@?GETCHARparam LF@?loadstringsub LF@?loadfirstint\n");
	fprintf(stdout,"CONCAT LF@?result1 LF@?result1 LF@?GETCHARparam\n");
	fprintf(stdout, "ADD LF@?ITERATORsubstr LF@?ITERATORsubstr int@1\n");
	fprintf(stdout, "ADD LF@?loadfirstint LF@?loadfirstint int@1\n");
	fprintf(stdout, "JUMP $for\n");
	fprintf(stdout, "LABEL $badinputsubstr\n");
	fprintf(stdout, "MOVE LF@?result0 int@1\n");
	fprintf(stdout, "MOVE LF@?result1 string@BADINPUT\n");
	fprintf(stdout, "JUMP $ENDSUBSTRING\n");
	fprintf(stdout,"LABEL $tillendsubstr\n");
	fprintf(stdout,"LABEL $for2\n");
	fprintf(stdout, "EQ LF@?EQiterator LF@?ITERATORsubstr LF@?lengthofsubstr \n");
	fprintf(stdout, "JUMPIFEQ $ENDSUBSTRING LF@?EQiterator bool@true\n");
	fprintf(stdout,"GETCHAR LF@?GETCHARparam LF@?loadstringsub LF@?loadfirstint\n");
	fprintf(stdout,"CONCAT LF@?result1 LF@?result1 LF@?GETCHARparam\n");
	fprintf(stdout, "ADD LF@?ITERATORsubstr LF@?ITERATORsubstr int@1\n");
	fprintf(stdout, "ADD LF@?loadfirstint LF@?loadfirstint int@1\n");
	fprintf(stdout, "JUMP $for2\n");
	fprintf(stdout,"LABEL $ENDSUBSTRING\n");
	fprintf(stdout,"POPFRAME\n");
	fprintf(stdout,"RETURN\n");

	fprintf(stdout, "\n");


}

/* SKOK na deklaracie vsetkych premennych pocas foru*/
void ForDeclarations(int label){
	fprintf(stdout, "\nJUMP $FORDEFVAR%d\n", label);
}

// Skok ktori preskoci deklaracie pred forom
void ForJumpMostOuter(int label){
	fprintf(stdout, "\nJUMP $ENDOUTER%d\n", label);
}

void ForLabelMostOuter(int label){
	fprintf(stdout, "LABEL $ENDOUTER%d\n", label);
}

void ForStart(int label){
	fprintf(stdout, "\nLABEL $FORSTART%d\n", label);
}

/* Vytlaci podmienku foru a jej kontrolovanie*/
void printFOR(int label)
{
	fprintf(stdout, "\nCREATEFRAME\n");
	fprintf(stdout, "DEFVAR TF@?forcond%d\n", label);
	fprintf(stdout, "POPS TF@?forcond%d\n",label);
	fprintf(stdout, "JUMPIFEQ $FORBODY%d TF@?forcond%d bool@true\n",label, label);
	fprintf(stdout, "JUMPIFNEQ $FOREND%d TF@?forcond%d bool@true\n",label, label);
}

void ForAssign(int label){
	fprintf(stdout, "\nLABEL $FORPLUS%d\n", label);
}

void ForJump(int label){
	fprintf(stdout, "JUMP $FORSTART%d\n", label);
}

void ForBody(int label){
	fprintf(stdout, "\nLABEL $FORBODY%d\n", label);
}

void ForEnd(int label){
	fprintf(stdout, "\nJUMP $FORPLUS%d\n", label);
	fprintf(stdout, "LABEL $FOREND%d\n\n", label);
}

//////////////////////////////////////////////////////////////////////////
/// Linked List
//////////////////////////////////////////////////////////////////////////

void LinkedInit(lList* list){
	*list = malloc(sizeof(struct linkedList));
	if(*list == NULL){
		fprintf(stderr, "Zlyhala alokacia linkedListu");
	}
	(*list)->first = NULL;
	(*list)->lastInserted = NULL;
}



void LinkedInsertDEFVAR (lList *list, TOKEN *tokenA, int label, int scope ) {
	if(scope == 1){
		label = 1;
	}

	if((*list)->first == NULL){
		(*list)->first = malloc(sizeof(struct ListItem));
		if((*list)->first == NULL){
			fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
		}
		else
		{
			char* defvar = malloc(DECLsize + sizeof(tokenA->value.stringValue.str));
			sprintf(defvar, "DEFVAR LF@?_%d_%d%s\n", label, scope, tokenA->value.stringValue.str);
			strInit(&(*list)->first->line);
			strAddConstChar(&(*list)->first->line, defvar);

			(*list)->lastInserted = (*list)->first;	
			free(defvar);

			lItem new = malloc(sizeof(struct ListItem));
				if(new == NULL){
					fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
				}
				else{
					defvar = malloc(DECLsize + sizeof(tokenA->value.stringValue.str));
					sprintf(defvar, "POPS LF@?_%d_%d%s\n", label, scope, tokenA->value.stringValue.str);
					strInit(&new->line);
					strAddConstChar(&new->line, defvar);
					new->next = NULL;
					(*list)->lastInserted->next = new;
					(*list)->lastInserted = new;
					free(defvar);
				}
		
		}
	}
	else
	{
		lItem new = malloc(sizeof(struct ListItem));
		if(new == NULL){
			fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
		}
		else{
			char* defvar = malloc(DECLsize + sizeof(tokenA->value.stringValue.str));
			sprintf(defvar, "DEFVAR LF@?_%d_%d%s\n", label, scope, tokenA->value.stringValue.str);
			strInit(&new->line);
			strAddConstChar(&new->line, defvar);
			new->next = NULL;
			(*list)->lastInserted->next = new;
			(*list)->lastInserted = new;
			free(defvar);

			lItem new = malloc(sizeof(struct ListItem));
				if(new == NULL){
					fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
				}
				else{
					defvar = malloc(DECLsize + sizeof(tokenA->value.stringValue.str));
					sprintf(defvar, "POPS LF@?_%d_%d%s\n", label, scope, tokenA->value.stringValue.str);
					strInit(&new->line);
					strAddConstChar(&new->line, defvar);
					new->next = NULL;
					(*list)->lastInserted->next = new;
					(*list)->lastInserted = new;
					free(defvar);
				}
		}
	}
}




void LinkedInsertParams (lList *list, TOKEN *FncName, int counter, int numberOfParams ) {

	if((*list)->first == NULL){
		(*list)->first = malloc(sizeof(struct ListItem));
		if((*list)->first == NULL){
			fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
		}
		else
		{
			char* defvar = malloc(DECLsize);
			sprintf(defvar, "DEFVAR TF@param%d\n", numberOfParams - counter - 1);
			strInit(&(*list)->first->line);
			strAddConstChar(&(*list)->first->line, defvar);

			(*list)->lastInserted = (*list)->first;	
			free(defvar);

			lItem new = malloc(sizeof(struct ListItem));
				if(new == NULL){
					fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
				}
				else{
					defvar = malloc(DECLsize);
					sprintf(defvar, "POPS TF@?param%d\n", numberOfParams - counter - 1);
					strInit(&new->line);
					strAddConstChar(&new->line, defvar);
					new->next = NULL;
					(*list)->lastInserted->next = new;
					(*list)->lastInserted = new;
					free(defvar);
				}
		
		}
	}
	else
	{
		lItem new = malloc(sizeof(struct ListItem));
		if(new == NULL){
			fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
		}
		else{
			char* defvar = malloc(DECLsize);
			sprintf(defvar, "DEFVAR TF@?param%d\n",numberOfParams - counter - 1);
			strInit(&new->line);
			strAddConstChar(&new->line, defvar);
			new->next = NULL;
			(*list)->lastInserted->next = new;
			(*list)->lastInserted = new;
			free(defvar);

			lItem new = malloc(sizeof(struct ListItem));
				if(new == NULL){
					fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
				}
				else{
					defvar = malloc(DECLsize);
					sprintf(defvar, "POPS TF@?param%d\n",numberOfParams - counter - 1);
					strInit(&new->line);
					strAddConstChar(&new->line, defvar);
					new->next = NULL;
					(*list)->lastInserted->next = new;
					(*list)->lastInserted = new;
					free(defvar);
				}
		}
	}
	if(counter == numberOfParams-1) /// pocet argumentov bude finalny
		{
			LinkedFncJump(list, FncName->value.stringValue.str);
		}
}



void LinkedFncStart (lList *list) {

	if((*list)->first == NULL){
		(*list)->first = malloc(sizeof(struct ListItem));
		if((*list)->first == NULL){
			fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
		}
		else
		{
			//char* defvar = malloc(DECLsize + sizeof(tokenA->value.stringValue.str));
			//sprintf(defvar, "PUSHS LF@?_%d_%d%s\n", label, scope, tokenA->value.stringValue.str);
			strInit(&(*list)->first->line);
			strAddConstChar(&(*list)->first->line, "CREATEFRAME\n\0");

			(*list)->lastInserted = (*list)->first;	
			//free(defvar);
		}
	}
	else
	{
		lItem new = malloc(sizeof(struct ListItem));
		if(new == NULL){
			fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
		}
		else{
			//char* defvar = malloc(DECLsize + sizeof(tokenA->value.stringValue.str));
			//sprintf(defvar, "PUSHS LF@?_%d_%d%s\n", label, scope, tokenA->value.stringValue.str);
			strInit(&new->line);
			strAddConstChar(&new->line, "CREATEFRAME\n\0");
			new->next = NULL;
			(*list)->lastInserted->next = new;
			(*list)->lastInserted = new;
			//free(defvar);
		}
	}
}



void LinkedFncJump (lList *list, char* Name) {

	if((*list)->first == NULL){
		(*list)->first = malloc(sizeof(struct ListItem));
		if((*list)->first == NULL){
			fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
		}
		else
		{
			char* defvar = malloc(Isize + sizeof(Name));
			sprintf(defvar, "CALL $HELP1%s\n", Name);
			strInit(&(*list)->first->line);
			strAddConstChar(&(*list)->first->line, defvar);

			(*list)->lastInserted = (*list)->first;	
			free(defvar);
		}
	}
	else
	{
		lItem new = malloc(sizeof(struct ListItem));
		if(new == NULL){
			fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
		}
		else{
			char* defvar = malloc(Isize + sizeof(Name));
			sprintf(defvar, "CALL $HELP1%s\n", Name);
			strInit(&new->line);
			strAddConstChar(&new->line, defvar);
			new->next = NULL;
			(*list)->lastInserted->next = new;
			(*list)->lastInserted = new;
			free(defvar);
		}
	}
}



void LinkedPushID (lList *list, TOKEN *tokenA, int label, int scope ) {
	if(scope == 1){
		label = 1;
	}

	if((*list)->first == NULL){
		(*list)->first = malloc(sizeof(struct ListItem));
		if((*list)->first == NULL){
			fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
		}
		else
		{
			char* defvar = malloc(DECLsize + sizeof(tokenA->value.stringValue.str));
			sprintf(defvar, "PUSHS LF@?_%d_%d%s\n", label, scope, tokenA->value.stringValue.str);
			strInit(&(*list)->first->line);
			strAddConstChar(&(*list)->first->line, defvar);

			(*list)->lastInserted = (*list)->first;	
			free(defvar);
		}
	}
	else
	{
		lItem new = malloc(sizeof(struct ListItem));
		if(new == NULL){
			fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
		}
		else{
			char* defvar = malloc(DECLsize + sizeof(tokenA->value.stringValue.str));
			sprintf(defvar, "PUSHS LF@?_%d_%d%s\n", label, scope, tokenA->value.stringValue.str);
			strInit(&new->line);
			strAddConstChar(&new->line, defvar);
			new->next = NULL;
			(*list)->lastInserted->next = new;
			(*list)->lastInserted = new;
			free(defvar);
		}
	}
}



void LinkedPushRetValue (lList *list, int counter) {

	if((*list)->first == NULL){
		(*list)->first = malloc(sizeof(struct ListItem));
		if((*list)->first == NULL){
			fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
		}
		else
		{
			char* defvar = malloc(DECLsize);
			sprintf(defvar, "PUSHS TF@?result%d\n", counter);
			strInit(&(*list)->first->line);
			strAddConstChar(&(*list)->first->line, defvar);

			(*list)->lastInserted = (*list)->first;	
			free(defvar);
		}
	}
	else
	{
		lItem new = malloc(sizeof(struct ListItem));
		if(new == NULL){
			fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
		}
		else{
			char* defvar = malloc(DECLsize);
			sprintf(defvar, "PUSHS TF@?result%d\n", counter);
			strInit(&new->line);
			strAddConstChar(&new->line, defvar);
			new->next = NULL;
			(*list)->lastInserted->next = new;
			(*list)->lastInserted = new;
			free(defvar);
		}
	}
}



void LinkedPushString (lList *list, char** name) {
	if((*list)->first == NULL){
		(*list)->first = malloc(sizeof(struct ListItem));
		if((*list)->first == NULL){
			fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
		}
		else
		{
			char* defvar = malloc(DECLsize + sizeof(*name));
			sprintf(defvar, "PUSHS string@%s\n", *name);
			strInit(&(*list)->first->line);
			strAddConstChar(&(*list)->first->line, defvar);

			(*list)->lastInserted = (*list)->first;	
			free(defvar);
		}
	}
	else
	{
		lItem new = malloc(sizeof(struct ListItem));
		if(new == NULL){
			fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
		}
		else{
			char* defvar = malloc(DECLsize + sizeof(*name));
			sprintf(defvar, "PUSHS string@%s\n", *name);
			strInit(&new->line);
			strAddConstChar(&new->line, defvar);
			new->next = NULL;
			(*list)->lastInserted->next = new;
			(*list)->lastInserted = new;
			free(defvar);
		}
	}
}



void LinkedPushInt (lList *list, long long value) {
	if((*list)->first == NULL){
		(*list)->first = malloc(sizeof(struct ListItem));
		if((*list)->first == NULL){
			fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
		}
		else
		{
			char* defvar = malloc(DECLsize);
			sprintf(defvar, "PUSHS int@%lld\n", value);
			strInit(&(*list)->first->line);
			strAddConstChar(&(*list)->first->line, defvar);

			(*list)->lastInserted = (*list)->first;	
			free(defvar);
		}
	}
	else
	{
		lItem new = malloc(sizeof(struct ListItem));
		if(new == NULL){
			fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
		}
		else{
			char* defvar = malloc(DECLsize);
			sprintf(defvar, "PUSHS int@%lld\n", value);
			strInit(&new->line);
			strAddConstChar(&new->line, defvar);
			new->next = NULL;
			(*list)->lastInserted->next = new;
			(*list)->lastInserted = new;
			free(defvar);
		}
	}
}



void LinkedPushFloat (lList *list, double value) {
	if((*list)->first == NULL){
		(*list)->first = malloc(sizeof(struct ListItem));
		if((*list)->first == NULL){
			fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
		}
		else
		{
			char* defvar = malloc(DECLsize);
			sprintf(defvar, "PUSHS int@%a\n", value);
			strInit(&(*list)->first->line);
			strAddConstChar(&(*list)->first->line, defvar);

			(*list)->lastInserted = (*list)->first;	
			free(defvar);
		}
	}
	else
	{
		lItem new = malloc(sizeof(struct ListItem));
		if(new == NULL){
			fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom\n");
		}
		else{
			char* defvar = malloc(DECLsize);
			sprintf(defvar, "PUSHS int@%a\n", value);
			strInit(&new->line);
			strAddConstChar(&new->line, defvar);
			new->next = NULL;
			(*list)->lastInserted->next = new;
			(*list)->lastInserted = new;
			free(defvar);
		}
	}
}


void LinkedDataStack (lList *list, iType instr) {

	char* defvar = malloc(Isize);
	switch(instr){
		case addS:
			sprintf(defvar, "ADDS\n");
			break;
		case subS:
			sprintf(defvar, "SUBS\n");
			break;
		case mulS:
			sprintf(defvar, "MULS\n");
			break;
		case divFloatS:
			sprintf(defvar, "DIVS\n");
			break;
		case divIntS:
			sprintf(defvar, "IDIVS\n");
			break;
		case clearsS:
			sprintf(defvar, "CLEARS\n");
			break;
		case greaterS:
			sprintf(defvar, "GTS\n");
			break;
		case lesserS:
			sprintf(defvar, "LRS\n");
			break;
		case equalS:
			sprintf(defvar, "EQS\n");
			break;
		case notequalS:
			sprintf(defvar, "EQS\nNOTS\n");
			break;
		case greaterEqualS:
			sprintf(defvar, "LTS\nNOTS\n");
			break;
		case lesserEqualS:
			sprintf(defvar, "GTS\nNOTS\n");
			break;
	}

	if((*list)->first == NULL){
		(*list)->first = malloc(sizeof(struct ListItem));
		if((*list)->first == NULL){
			fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
		}
		else
		{			
			strInit(&(*list)->first->line);
			strAddConstChar(&(*list)->first->line, defvar);
			(*list)->lastInserted = (*list)->first;	
			free(defvar);
		}
	}
	else
	{
		lItem new = malloc(sizeof(struct ListItem));
		if(new == NULL){
			fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
		}
		else{
			strInit(&new->line);
			strAddConstChar(&new->line, defvar);
			new->next = NULL;
			(*list)->lastInserted->next = new;
			(*list)->lastInserted = new;
			free(defvar);
		}
	}
}



void LinkedConcat (lList *list, TOKEN *tokenA, int label, int scope) {
	if((*list)->first == NULL){
		(*list)->first = malloc(sizeof(struct ListItem));
		if((*list)->first == NULL){
			fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom");
		}
		else
		{
			strInit(&(*list)->first->line);
			strAddConstChar(&(*list)->first->line, "CREATEFRAME\nDEFVAR TF@**$concatedstring\nPOPS TF@**$concatedstring\n\0");
			(*list)->lastInserted = (*list)->first;
			
			lItem new = malloc(sizeof(struct ListItem));
			if(new == NULL){
				fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom\n");
			}
			else{
				char* defvar = malloc(DECLsize + sizeof(tokenA->value.stringValue.str) + 2*sizeof(int));
				if(tokenA->isID)
					sprintf(defvar, "CONCAT TF@**$concatedstring TF@**$concatedstring LF@?_%d_%d%s\n", label, scope, tokenA->value.stringValue.str);
				else
					sprintf(defvar, "CONCAT TF@**$concatedstring TF@**$concatedstring string@%s\n", tokenA->value.stringValue.str);
				strInit(&new->line);
				strAddConstChar(&new->line, defvar);
				new->next = NULL;
				(*list)->lastInserted->next = new;
				(*list)->lastInserted = new;
				free(defvar);

				lItem new = malloc(sizeof(struct ListItem));
				if(new == NULL){
					fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom\n");
				}
				else{
					//char* defvar = malloc(DECLsize + sizeof(tokenA->value.stringValue.str));
					//sprintf(defvar, "CONCAT TF@**$concatedstring TF@**$concatedstring string@%s\n", tokenA->value.stringValue.str);
					strInit(&new->line);
					strAddConstChar(&new->line, "PUSHS  TF@**$concatedstring\n\0");
					new->next = NULL;
					(*list)->lastInserted->next = new;
					(*list)->lastInserted = new;
					free(defvar);
				}
			}

		}
	}
	else
	{
		lItem new = malloc(sizeof(struct ListItem));
		if(new == NULL){
			fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom\n");
		}
		else{
			strInit(&new->line);
			strAddConstChar(&new->line, "CREATEFRAME\nDEFVAR TF@**$concatedstring\nPOPS TF@**$concatedstring\n\0");
			new->next = NULL;
			(*list)->lastInserted->next = new;
			(*list)->lastInserted = new;

			lItem new = malloc(sizeof(struct ListItem));
			if(new == NULL){
				fprintf(stderr, "Zlyhala alokacia DEFVAR pred forom\n");
			}
			else{
				char* defvar = malloc(DECLsize + sizeof(tokenA->value.stringValue.str) + 2*sizeof(int));
				if(tokenA->isID)
					sprintf(defvar, "CONCAT TF@**$concatedstring TF@**$concatedstring LF@?_%d_%d%s\n", label, scope, tokenA->value.stringValue.str);
				else
					sprintf(defvar, "CONCAT TF@**$concatedstring TF@**$concatedstring string@%s\n", tokenA->value.stringValue.str);
				strInit(&new->line);
				strAddConstChar(&new->line, defvar);
				new->next = NULL;
				(*list)->lastInserted->next = new;
				(*list)->lastInserted = new;
				free(defvar);

				lItem new = malloc(sizeof(struct ListItem));
				if(new == NULL){
					fprintf(stderr, "Zlyhala alokacia Linked listu\n");
				}
				else{
					strInit(&new->line);
					strAddConstChar(&new->line, "PUSHS  TF@**$concatedstring\n\0");
					new->next = NULL;
					(*list)->lastInserted->next = new;
					(*list)->lastInserted = new;
					free(defvar);
				}
			}

		}
	}
}

void LinkedPrintDispose(lList *list, int label){
	fprintf(stdout, "\nLABEL $FORDEFVAR%d\n", label);
	(*list)->lastInserted = (*list)->first;
	while((*list)->lastInserted){
		fprintf(stdout, "%s", (*list)->lastInserted->line.str);
		//presun sa na dalsi riadok na tlacenie
		(*list)->lastInserted = (*list)->lastInserted->next;

		strFree(&(*list)->first->line);
		free((*list)->first);

		// 'uloz' si dalsi riadok na vymazanie
		(*list)->first = (*list)->lastInserted;
	}
	fprintf(stdout, "JUMP $FORSTART%d\n", label);
}