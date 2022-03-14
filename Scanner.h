/*
@Name Lexikalna analyza IFJ20
@Author Kristian Kralovic <xkralo05>
@file Scanner.h
@date 23.10.2020
*/
#include <stdio.h>
#include "str.h"
#include <stdbool.h>

#ifndef Scanner_H
#define Scanner_H
/*typedef enum
{
	KeyWord_else,
	KeyWord_float64,
	KeyWord_for,
	KeyWord_func,
	KeyWord_if,
	KeyWord_int,
	KeyWord_package,
	KeyWord_return,
	KeyWord_string,
	KeyWord_Nil,
}KeyWords;*/

// 
// K nazvom stavov je pridane S_ , inak  odpovedaju nazvom stavov v diagrame konecneho automatu 
typedef enum
{
	S_START,		// Pociatocny stav automatu / moze byt aj koncovi stav
	S_LEX_ERR,		// Koncovy stav error
	S_GREATER,		// Koncovy stav  >
	S_GREATER_E,	// Koncovy stav >=
	S_LESSER,		// Koncovy stav <
	S_LESSER_E,		// Koncovy stav <=
	S_NOT,			// Prechodny stav !
	S_NOT_E,		// Koncovy stav !=
	S_H_SLASH,		// Prechodny stav /
	S_DIV,			// Koncovy stav / (delenie)
	S_LCOMMENT,		// Prechodny stav // (line comment)
	S_BCOMMENT,		// Prechodny stav /* (block commnet)
	S_H_ENDBCOMM,	// Prechodny stav pre prvy znak konca blokoveho komentara *
	S_H_STRING, 	// Prechodny stav "
	S_H_STRING_E,	// Prechodny stav pre escape sekvenciu znakov '\'
	S_H_STRING_HE1,	// Prechodny stav pre hexa znaky
	S_H_STRING_HE2,	// Prechodny stav 2  pre hexa znaky 
	S_END_STR,		// Koncovy stav string
	S_KEYWORD_ID, 	// Koncovy stav pre keyword alebo ID
	S_INT_19,		// Koncovy stav pre integer 1..9
	S_INT_0,		// Koncovy stav pre integer 0
	S_H_FLOAT1,		// Prechodny stav pre float cisla
	S_F_FLOAT,		// Koncovy stav pre float cisla 
	S_FLOAT_EX,		// Prechodny stav pre float cisla s exponentom
//	S_FLOAT_EX_0, 	// Koncovy stav pre float cisla s nulovym exponentom
	S_FLOAT_EX_PM,	// Prechodny stav pre float cisla +/- exponentom
	S_FLOAT_EX_R,	// Koncovy stav pre float cisla s exponentom
	S_EOL,			// Koncovy stav  \n
	S_L_CBRACKET,	// Koncovy stav { 
	S_R_CBRACKET,	// Koncovy stav }
	S_COMA,			// Koncovy stav ,
	S_L_RBRACKET,	// Koncovy stav (
	S_R_RBRACKET,	// Koncovy stav )
	S_SUB, 			// Koncovy stav -
	S_ADD,			// koncovy stav +
	S_MUL,			// Koncovy stav *
	S_COLON,		// Prechodny stav :
	S_DEFINITION,	// Koncovy stav :=
	S_ASSIGN,		// Koncovy stav =
	S_EQUAL,			// Koncovy stav ==
	S_EOF,			// Koncovy stav EOF
	S_SEMIC,		// Koncovy stav pre ;
	S_UNDERSCORE, 	// Koncovy stav pre _
}tState;

typedef enum 
{
	T_START,		// Token pre zaciatok				0
	T_EMPTY,		// Prazdny TOKEN 					1
	T_EOF,			// EOF token 						2
	T_LEX_ERR,		// Token pre lexikalny error 		3
	T_INTERPRETER_ERR,	//								4
	T_EOL,			// Token pre EOL 					5
	
	T_KEYWORD,		// Token pre keyword 				6
	T_ID,			// Token pre ID 					7
	
	T_GREATER,		// Token pre > 						8
	T_GREATER_E,	// Token pre >= 					9
	T_LESSER,		// Token pre < 						10
	T_LESSER_E,		// Token pre <= 					11
	T_NOT_EQ,		// Token pre != 					12
	T_EQUAL,		// Token pre == 					13
	
	T_DIV, 			// Token pre delenie(/) 			14
	T_SUB,			// Token pre minus (-) 				15
	T_ADD,			// Token pre plus (+) 				16
	T_MUL,			// Token pre nasobenie(*) 			17

	T_END_STR,		// Token pre string 				18
	T_INT19,		// Token pre int 1..9 				19
	T_INT0,			// Token pre int 0 					20
	T_F_FLOAT,		// Token pre float cisla 			21
	T_FLOAT_EX_R,	// Token pre float cislo s exponentom     22
	
	T_L_CBRACKET,	// Token pre {  					23
	T_R_CBRACKET,	// Token pre } 						24
	T_COMA,			// Token pre , 						25
	T_L_RBRACKET,	// Token pre (  					26
	T_R_RBRACKET,	// Token pre ) 						27
	
	T_DEFINITION,	// Token pre := 					28
	T_ASSIGN,		// Token pre = 						29

	KeyWord_else,	//									30
	KeyWord_float64,//									31
	KeyWord_for,	//									32
	KeyWord_func,	//									33
	KeyWord_if,		//									34
	KeyWord_int,	//									35
	KeyWord_package,//									36	
	KeyWord_return,//									37
	KeyWord_string,//									38
	T_SEMIC,		//									39
	T_UNDERSCORE,	// 									40
	T_BOOL			//									41 -> pridany kvoli semantike
}tToken;

typedef union
{
	long long integerValue;
	double floatValue;
	string stringValue;
}tokenValue;

typedef struct 
{
	tToken type;
	tokenValue value;
	bool isID;
}TOKEN;

void getToken(TOKEN *newToken);
int checkKey (TOKEN *token);
void CleanToken(TOKEN *toClean);
int IsPunctCorrect(int c);
void CharToEscape(int c, string* NewStr);

#endif 