/*
@Name Lexikalna analyza IFJ20
@Author Kristian Kralovic <kralo05>
@file Scanner.c
@date 23.10.2020
*/
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "Scanner.h"
#include "Error.h"
/*
 * Funkcia sluzi na overenie ci je zadane slovo jednym z klucovych slov  
 * Parameter newToken obsahuje dane slovo
 * Premenna isKeyWord sluzi pre identifikaciu klucoveho slova . Ak je dane slovo jendym z klucovich slov nastavi sa na hodnotu 1 inak je nastavena na hodnotu 0 a signalizuje ze sa jedna o ID
*/
int checkKey (TOKEN *newToken)
{
	int isKeyWord ;
	
	if(strCmpConstStr(&newToken->value.stringValue , "else") == 0)
	{
		newToken->type = KeyWord_else;
		isKeyWord = 1;
	}
	else if(strCmpConstStr(&newToken->value.stringValue , "float64") == 0)
	{
		newToken->type = KeyWord_float64;
		isKeyWord = 1;
	}
	else if(strCmpConstStr(&newToken->value.stringValue , "for") == 0)
	{
		newToken->type = KeyWord_for;
		isKeyWord = 1;
	}
	else if(strCmpConstStr(&newToken->value.stringValue , "func") == 0)
	{
		newToken->type = KeyWord_func;
		isKeyWord = 1;
	}
	else if(strCmpConstStr(&newToken->value.stringValue , "if") == 0)
	{
		newToken->type = KeyWord_if;
		isKeyWord = 1;
	}
	else if(strCmpConstStr(&newToken->value.stringValue , "int") == 0)
	{
		newToken->type = KeyWord_int;
		isKeyWord = 1;
	}
	else if(strCmpConstStr(&newToken->value.stringValue , "package") == 0)
	{
		newToken->type = KeyWord_package;
		isKeyWord = 1;
	}
	else if(strCmpConstStr(&newToken->value.stringValue , "return") == 0)
	{
		newToken->type = KeyWord_return;
		isKeyWord = 1;
	}
	else if(strCmpConstStr(&newToken->value.stringValue , "string") == 0)
	{
		newToken->type = KeyWord_string;
		isKeyWord = 1;
	}
	else
	{
		isKeyWord = 0;
	}
	
	return isKeyWord;
}

/*
* Funkcia sluzi na naplnenie tokenu udajmi 
* Parameter newToken je token ktory treba naplnit udajmi
* Premenna isHexa a hexHelp sluzia ako pomocne premenne pri cislach a znakoch zadanych pomocou hexa reprezentacie 
*/


void getToken(TOKEN *newToken)
{
	CleanToken(newToken);
	newToken->type = T_START;
	newToken->isID = false;
	char isHexa[2] = {0,0};
	isHexa[0] = 0;
	isHexa[1] = 0;
	isHexa[2] = '\0';
	int hexHelp;


	string newString ;
	if(strInit(&newString) == 1)
	{
		newToken->type = T_INTERPRETER_ERR;
		return;
	}


	int c; // akutalne nacitany znak
	tState state = T_START;
	strClear(&newString);
	
	while(1)
	{
		c = getchar();
		
		switch(state)
		{
			case T_START:
				if (c!='\n' && (isspace(c))) //194 je divny polospace
				{
					state = S_START;
				}
				else if (c == EOF)
				{
					state = S_EOF;
				}
				else if (c == ',')
				{
					state = S_COMA;
				}
				else if(c == ')')
				{
					state = S_R_RBRACKET;
				}
				else if (c == '(')
				{
					state = S_L_RBRACKET;
				}
				else if (c == '-')
				{
					state = S_SUB;
				}
				else if (c == '+')
				{
					state = S_ADD;
				}	
				else if (c == '/')
				{
					state = S_H_SLASH;
				}
				else if (c == '*')
				{
					state = S_MUL;
				}
				else if (c == ':')
				{
					state = S_COLON;
				}
				else if (c == '=')
				{
					state = S_ASSIGN;
				}
				else if (c == '}')
				{
					state = S_R_CBRACKET;
				}
				else if (c == '{' )
				{
					state = S_L_CBRACKET;
				}
				else if (c == '\n' )
				{
					state = S_EOL;
				}
				else if (c == '>' )
				{
					state = S_GREATER;
				}
				else if (c == '<' )
				{
					state = S_LESSER;
				}
				else if (c == '!' )
				{
					state = S_NOT;
				}
				else if (c == ';')
				{
					state = S_SEMIC;
				}
				else if ( c == '"')
				{
					
					state = S_H_STRING;
				
				}
				else if (c == '_')
				{
					if (strAddChar(&newString,c) == STR_ERROR)
					{
						newToken->type = T_INTERPRETER_ERR;
						strFree(&newString);
						return;
					}
					else
					{
						state = S_UNDERSCORE;
					}

				}
				else if (isalpha(c) )
				{
					if (strAddChar(&newString,c) == STR_ERROR)
					{
						newToken->type = T_INTERPRETER_ERR;
						strFree(&newString);
						return;
					}
					else
					{
						state = S_KEYWORD_ID;
					}
					//state = S_KEYWORD_ID;

				}
				else if (isdigit(c))
				{
					if (c == '0')
					{
						if (strAddChar(&newString,c) == STR_ERROR)
						{
							newToken->type = T_INTERPRETER_ERR;
							strFree(&newString);
							return;
						}
						else
						{
							state = S_INT_0;
						}
						//state = S_INT_0;
					}
					else
					{
						if (strAddChar(&newString,c) == STR_ERROR)
						{
							newToken->type = T_INTERPRETER_ERR;
							strFree(&newString);
							return;
						}
						else
						{
							state = S_INT_19;
						}
						//state =S_INT_19;
					}
				}
				else
				{
					
					state = S_LEX_ERR;
				}
			break;

			case S_SEMIC:
					ungetc(c,stdin);
					newToken->type = T_SEMIC;
					strFree(&newString);
					return;

			break;

			case S_UNDERSCORE:
					if((isdigit(c) || isalpha(c)) || c == '_' )
					{
						if (strAddChar(&newString,c) == STR_ERROR)
						{
							newToken->type = T_INTERPRETER_ERR;
							strFree(&newString);
							return;
						}
						else
						{
							state = S_KEYWORD_ID;
						}
					}
					else
					{
						ungetc(c,stdin);
						newToken->type = T_UNDERSCORE;
						strInit(&newToken->value.stringValue);
						strCopyString(&newToken->value.stringValue, &newString);
						strFree(&newString);
						return;
					}

			break;

			case S_EOF:
					ungetc(c,stdin);
					newToken->type = T_EOF;
					strFree(&newString);
				
					return;
			break;

			case S_COMA:
					ungetc(c,stdin);
					newToken->type = T_COMA;
					strFree(&newString);
					
					return;
			break;

			case  S_R_RBRACKET:
					ungetc(c,stdin);
					newToken->type = T_R_RBRACKET;
					strFree(&newString);
					
					return;
			break;

			case S_L_RBRACKET:
					ungetc(c,stdin);
					newToken->type = T_L_RBRACKET;
					strFree(&newString);
					
					return;
			break;

			case S_SUB:
					ungetc(c,stdin);
					newToken->type = T_SUB;
					strFree(&newString);
					
					return;

			break;

			case S_ADD:
					ungetc(c,stdin);
					newToken->type = T_ADD;
					strFree(&newString);
					
					return;
			break;

			case S_H_SLASH:
					if(c == '/')
					{
						state = S_LCOMMENT;
					}
					else if (c == '*')
					{
						state = S_BCOMMENT;
					}
					else
					{
						ungetc(c,stdin);
						state = S_DIV;
					}

			break;

			case S_MUL:
					ungetc(c,stdin);
					newToken->type = T_MUL;
					strFree(&newString);
					
					return;
			break;

			case S_COLON:
					if(c == '=')
					{
						state = S_DEFINITION; 
					}
					else
					{
						newToken->type = T_LEX_ERR;
						strFree(&newString);
						
						return;
					}
			break;

			case S_ASSIGN:
				if(c == '=')
				{
					state = S_EQUAL;
				}
				else
				{
					ungetc(c,stdin);
					newToken->type = T_ASSIGN;
					strFree(&newString);
					
					return;
				}
			break;

			case S_R_CBRACKET:
					ungetc(c,stdin);
					newToken->type = T_R_CBRACKET;
					strFree(&newString);
					
					return;
			break;

			case S_L_CBRACKET:
					ungetc(c,stdin);
					newToken->type = T_L_CBRACKET;
					strFree(&newString);
					
					return;
			break;

			case S_EOL:
					
					if(c == EOF){
						ungetc(c,stdin);
						newToken->type = T_EOL;
						strFree(&newString);
					}
					else{
						ungetc(c,stdin);
						newToken->type = T_EOL;
						strFree(&newString);
					}
					
					return;

			break;

			case S_GREATER:
			if(c == '=')
			{
				state = S_GREATER_E;
			}
			else
			{
					ungetc(c,stdin);
					newToken->type = T_GREATER;
					strFree(&newString);
					
					return;
			}

			break;

			case S_LESSER:
			if(c == '=')
			{
				state = S_LESSER_E;
			}
			else
			{
					ungetc(c,stdin);
					newToken->type = T_LESSER;
					strFree(&newString);
					
					return;

			}

			break;

			case S_NOT:
					if(c == '=')
					{
						state =S_NOT_E;
					}
					else
					{
						newToken->type = T_LEX_ERR;
						strFree(&newString);
						
						return;
					}


			break;

			case S_H_STRING:
				if (c < 32)
				{
					newToken->type = T_LEX_ERR;
					strFree(&newString);
					
					return;
				}
				else if(c == '"')
				{
						//if (strAddChar(&newString,c) == STR_ERROR)
						//{
						//	newToken->type = T_INTERPRETER_ERR;
						//	strFree(&newString);
						//	return;
						//}
					//	else
					//	{
							state = S_END_STR;
					//	}
					//state = S_END_STR;
				}
				else if (c == '\\')
				{

					state = S_H_STRING_E;

				}
				else if ((c>=0 && c<33) || c==35 || c== 92)
				{
					CharToEscape(c, &newString);
				}

				else
				{
						if (strAddChar(&newString,c) == STR_ERROR)
						{
							newToken->type = T_INTERPRETER_ERR;
							strFree(&newString);
							return;
						}
				}

			break;

			case S_KEYWORD_ID:
					if( (isdigit(c) || isalpha(c)) || c == '_' )
					{
						if (strAddChar(&newString,c) == STR_ERROR)
						{
							newToken->type = T_INTERPRETER_ERR;
							strFree(&newString);
							return;
						}
					}
					/*else if (!((isdigit(c) || isalpha(c)) || c == '_'  ))
					{
						newToken->type = T_LEX_ERR;
						strFree(&newString);
						
						return;
					}*/
					// vratim token iba ak nasleduje znak ocakavany za T_ID podla gramatiky
					else if(isspace(c) || ispunct(c))
					{
						if (strInit(&newToken->value.stringValue) == 1)
						{
							newToken->type = T_INTERPRETER_ERR;
							strFree(&newString);
							
							return;
						}
						ungetc(c,stdin);
						strCopyString(&newToken->value.stringValue,&newString);
						strFree(&newString);
						if(checkKey(newToken) == 1)
						{
							return;
						}
						else
						{
							newToken->type = T_ID;
							return;
						}
						
					}
					else
					{
						newToken->type = T_LEX_ERR;
						return;
					}
			break;

			case S_INT_0: 
					if(c == '.')
					{
						if (strAddChar(&newString,c) == STR_ERROR)
						{
							newToken->type = T_INTERPRETER_ERR;
							strFree(&newString);
							return;
						}
						else
						{
							state = S_H_FLOAT1;
						}
						//state = S_H_FLOAT1;
					}
					else if (c == 'e' || c == 'E')
					{
						if (strAddChar(&newString,c) == STR_ERROR)
						{
							newToken->type = T_INTERPRETER_ERR;
							strFree(&newString);
							return;
						}
						else
						{
							state = S_FLOAT_EX;
						}
						//state = S_FLOAT_EX;
					}
					else if ((isdigit(c) || isalpha(c)) && !(c == 'e' || c == 'E'))
					{
						newToken->type = T_LEX_ERR;
						strFree(&newString);
						
						return;
					}
					else
					{
						ungetc(c,stdin);
						newToken->type = T_INT0;
						newToken->value.integerValue = (int) strtoll(strGetStr(&newString),NULL,10);
						strFree(&newString);
						
						return;
					}
			break;

			case S_INT_19:
					if(isdigit(c))
					{
						if (strAddChar(&newString,c) == STR_ERROR)
						{
							newToken->type = T_INTERPRETER_ERR;
							strFree(&newString);
							return;
						}
					}
					else if (c == '.')
					{
						if (strAddChar(&newString,c) == STR_ERROR ) 
						{
							newToken->type = T_INTERPRETER_ERR;
							strFree(&newString);
							return;
						}
						else
						{
							state = S_H_FLOAT1;
						}
						//state = S_H_FLOAT1;
					}
					else if (c == 'e' || c == 'E')
					{
						if (strAddChar(&newString,c) == STR_ERROR )
						{
							newToken->type = T_INTERPRETER_ERR;
							strFree(&newString);
							return;
						}
						else
						{
							state = S_FLOAT_EX;
						}
						//state = S_FLOAT_EX;
					}
					else if (isalpha(c) && !(c == 'E' || c == 'e'))
					{
						newToken->type = T_LEX_ERR;
						strFree(&newString);
						
						return;
					}
					else
					{
						ungetc(c,stdin);
						newToken->type = T_INT19;
						newToken->value.integerValue = (int) strtoll(strGetStr(&newString),NULL,10);
						strFree(&newString);
						
						return;
					}
			break;

			case S_H_FLOAT1:
					if (isdigit(c))
					{
						if (strAddChar(&newString,c) == STR_ERROR ) 
						{
							newToken->type = T_INTERPRETER_ERR;
							strFree(&newString);
							return;
						}
						else
						{
							state = S_F_FLOAT;
						}
						//state = S_F_FLOAT;

					}
					else if (c == 'E' || c == 'e')
					{
						 if (strAddChar(&newString,c) == STR_ERROR)
						 {
						 	newToken->type = T_INTERPRETER_ERR;
							strFree(&newString);
							return;
						 }
						 else
						 {
						 	state = S_FLOAT_EX;
						 }
						//state = S_FLOAT_EX;
					}
					else if (isalpha(c) && !(c == 'E' || c == 'e'))
					{
						newToken->type = T_LEX_ERR;
						strFree(&newString);
						
						return;
					}
			break;
			
			case S_F_FLOAT:
					if (isdigit(c))
					{
						if (strAddChar(&newString,c) == STR_ERROR )
						{
							newToken->type = T_INTERPRETER_ERR;
							strFree(&newString);
							return;
						}

					}
					else if (c == 'E' || c == 'e')
					{
						if (strAddChar(&newString,c) == STR_ERROR ) 
						{
							newToken->type = T_INTERPRETER_ERR;
							strFree(&newString);
							return;
						}
						else
						{
							state = S_FLOAT_EX;
						}
						//state = S_FLOAT_EX;
					}
					else if (isalpha(c) && !(c == 'E' || c == 'e'))
					{
						newToken->type = T_LEX_ERR;
						strFree(&newString);
						
						return;
					}
					else
					{
						ungetc(c,stdin);
						newToken->type = T_F_FLOAT;
						newToken->value.floatValue = strtod(strGetStr(&newString),NULL);
						strFree(&newString);
						
						return;
					}

			break;
			
			case S_FLOAT_EX:
					if(isdigit(c))
					{
						if(c == '0')
						{
							if (strAddChar(&newString,c) == STR_ERROR)
							{
								newToken->type = T_INTERPRETER_ERR;
								strFree(&newString);
								return;
							}
							else
							{
								state = S_FLOAT_EX_R;
							}
						}
						else
						{
							if(strAddChar(&newString,c) == STR_ERROR)
							{
								newToken->type = T_INTERPRETER_ERR;
								strFree(&newString);
								return;
							}
							else
							{
								state = S_FLOAT_EX_R;
							}
							//state = S_FLOAT_EX_R;
						}
					}
					else if (c == '+' || c == '-')
					{
						if(strAddChar(&newString,c) == STR_ERROR)
						{
							newToken->type = T_INTERPRETER_ERR;
							strFree(&newString);
							return;
						}
						else
						{
							state = S_FLOAT_EX_PM;
						}
						//state = S_FLOAT_EX_PM;
					}
					else
					{
						newToken->type = T_LEX_ERR;
						strFree(&newString);
						
						return;
					}


			break;

			case S_FLOAT_EX_PM:
					if(isdigit(c))
					{
						if(c =='0')
						{
							if (strAddChar(&newString,c)== STR_ERROR)
							{
								newToken->type = T_INTERPRETER_ERR;
								strFree(&newString);
								return;
							}
							else
							{
								state = S_FLOAT_EX_R;
							}
						}
						else
						{
							if (strAddChar(&newString,c) == STR_ERROR )
							{
								newToken->type = T_INTERPRETER_ERR;
								strFree(&newString);
								return;
							}
							else
							{
								state = S_FLOAT_EX_R;
							}
							//state = S_FLOAT_EX_R;
						}
					}
					else
					{
						newToken->type = T_LEX_ERR;
						strFree(&newString);
						
						return;
					}


			break;

			case S_FLOAT_EX_R:
				
				if(isdigit(c))
				{
					if (strAddChar(&newString,c) == STR_ERROR)
					{
						newToken->type = T_INTERPRETER_ERR;
						strFree(&newString);
						return;
					}
				}
				else
				{
					ungetc(c,stdin);
					newToken->type = T_FLOAT_EX_R;
					newToken->value.floatValue = strtod(strGetStr(&newString),NULL);
					strFree(&newString);
					
					return;	
				}

			break;

			case S_H_STRING_E:
				if(c < 32)
				{
					newToken->type = T_LEX_ERR;
					strFree(&newString);
					
					return;
				}
				else if (c == 'x')
				{
					state = S_H_STRING_HE1;
				}
				else if (c =='n')
				{
					if (strAddConstChar(&newString,"\\010\0") == STR_ERROR)
					{
						newToken->type = T_INTERPRETER_ERR;
						strFree(&newString);
						return;
					}
					else
					{
						state = S_H_STRING;
					}
					//state = S_H_STRING;
				}
				else if (c == '"')
				{
					if (strAddConstChar(&newString,"\\034\0")== STR_ERROR)
					{
						newToken->type = T_INTERPRETER_ERR;
						strFree(&newString);
						return;
					}
					else
					{
						state = S_H_STRING;
					}
					//state = S_H_STRING;
				}
				else if (c == 't')
				{
					if (strAddConstChar(&newString,"\\009\0") == STR_ERROR)
					{
						newToken->type = T_INTERPRETER_ERR;
						strFree(&newString);
						return;
					}
					else
					{
						state = S_H_STRING;
					}
					//strAddChar(&newString,'\\');
					//state = S_H_STRING;
				}
				else if (c == '\\')
				{
					if (strAddConstChar(&newString,"\\092\0") ==STR_ERROR)
					{
						newToken->type = T_INTERPRETER_ERR;
						strFree(&newString);
						return;
					}
					else
					{
						state = S_H_STRING;
					}
					//state = S_H_STRING;
				}
				else
				{
					newToken->type = T_LEX_ERR;
					strFree(&newString);
					
					return;
				}


			break;
			
			case S_H_STRING_HE1:
				if(isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c<= 'f'))
				{
					isHexa[0] = c;
					state = S_H_STRING_HE2;
				}
				else
				{
					newToken->type = T_LEX_ERR;
					strFree(&newString);
					
					return;
				}
			break; 

			case S_H_STRING_HE2:
				if(isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c<= 'f'))
				{
					isHexa[1] = c;
					state = S_H_STRING;
					hexHelp = strtol(isHexa,NULL,16);
					if ((hexHelp >= 0 && hexHelp < 33) || hexHelp == 35 || hexHelp == 92)
					{
						CharToEscape(c, &newString);
					}
					else if(strAddChar(&newString,hexHelp)== STR_ERROR)
					{
						newToken->type = T_INTERPRETER_ERR;
						strFree(&newString);
						return;
					}
					ungetc(c,stdin);
				}
				else
				{
					newToken->type = T_LEX_ERR;
					strFree(&newString);
					
					return;
				}



			break;
			case S_BCOMMENT:
					if(c == '*')
					{
						state = S_H_ENDBCOMM;
					}
					else if (c == EOF)
					{
						newToken->type = T_LEX_ERR;
						strFree(&newString);
						
						return;
					}

			break;
			case S_LCOMMENT:
					if(c == '\n' || c == EOF)
					{
						state = S_EOL;
						strFree(&newString);
					}


			break;
			case S_DIV:
					ungetc(c,stdin);
					newToken->type = T_DIV;
					strFree(&newString);
					return;

			break;
			case S_H_ENDBCOMM:
			   	 	if (c == '/')
			    	{
			    		state = S_START;
			    		strFree(&newString);
			   		}
			    	else if (c == EOF)
			    	{
			    		newToken->type = T_LEX_ERR;
						strFree(&newString);
						
						return;
			    	}
			    	else
			    	{
			    		state = S_BCOMMENT;
			    	}
			break;
			case S_NOT_E:
					ungetc(c,stdin);
					newToken->type = T_NOT_EQ;
					strFree(&newString);
				
					return;
			break;
			case S_GREATER_E:
					ungetc(c,stdin);
					newToken->type = T_GREATER_E;
					strFree(&newString);
					
					return;
					
			break;
			case S_LESSER_E:
					ungetc(c,stdin);
					newToken->type = T_LESSER_E;
					strFree(&newString);
					
					return;

			break;
			case S_EQUAL:
					ungetc(c,stdin);
					newToken->type = T_EQUAL;
					strFree(&newString);
					
					return;

			break;
			case S_DEFINITION:
					ungetc(c,stdin);
					newToken->type = T_DEFINITION;
					strFree(&newString);
					
					return;
			break;
			case S_END_STR:
					if (strInit(&newToken->value.stringValue) == 1)
					{
						newToken->type = T_INTERPRETER_ERR;
						strFree(&newString);
						
						return;
					}
					ungetc(c,stdin);
					newToken->type = T_END_STR;
					strCopyString(&newToken->value.stringValue,&newString);
					strFree(&newString);
					return;

			break;
			case S_LEX_ERR:
					ungetc(c,stdin);
					newToken->type = T_LEX_ERR;
					strFree(&newString);
					return;
			break;
		}
	} 
//return;
}

/**	Custom funkcia na vratenie len jazykom uznavanych ispunct() znakov
 * @return 1 ak je znak jazykom akceptovany 0 inak
 */
int IsPunctCorrect(int c)
{	// TODO mozno vymazat...
	if (ispunct(c)){
		switch(c){
			case '(':
			case ')':
			case '=':
			case '+':
			case '-':
			case '*':
			case '/':
			case '<':
			case '>':
			case ':':
			case ',': // pre '_, a'
				return 1; // TODO korektnut ak som nejaky vynechal
				break;
			default:
				return 0;
				break;
		}
	}
	return 0;
}

/** Funkcia maze string obsah tokenu na zaklade jeho terajsieho typu
 * @param toClean pointer na token ktoreho obsah bude vymazany
 */
void CleanToken(TOKEN *toClean){
	switch(toClean->type){

	case T_UNDERSCORE:
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
		toClean->value.stringValue.length = 0;
		toClean->value.stringValue.allocSize = 0;
		free(toClean->value.stringValue.str);
	break;

	//nezaujima ma string value
	default:
	break;
	}
}

void CharToEscape(int c, string* NewStr){
	strAddChar(NewStr, 92);
	strAddChar(NewStr, 48);

	char digits[3];
	switch (c)
	{
		case 1-9:
			strAddChar(NewStr, 48);
			sprintf(digits, "%d", c);
			strAddChar(NewStr, digits[0]);
		break;
		default:
			sprintf(digits, "%d", c);
			strAddChar(NewStr, digits[0]);
			strAddChar(NewStr, digits[1]);
		break;
	}
}
