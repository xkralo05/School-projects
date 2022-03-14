/* 
 *	@Name Chybove kody pre vystup prekladacu
 *  @Author Kristian Kralovic <xkralo05>
 *	@file Error.h
 *	@date 24.10.2020
*/

#ifndef ERROR_H
#define ERROR_H

	#define CODE_OK 						0 // kod je v poriadku
	#define LEXICAL_ERR						1 // chyba v ramci lexiklanej analyzy ( chybna struktura lexemu)
	#define	SYNTACTIC_ERR					2 // chyba v ramci syntactickej analyzy (chybna syntax programu , neocakavane odriadkovanie)
	#define	SEMANTIC_ERR_UNDEF				3 // semanticka chyba v programe (nedefinovana funkcia/premenna , pokus o redefiniciu funkcie/premennej)
	#define	SEMANTIC_ERR_VARIABLE			4 // semanticka chyba v programe (chyba pri odvodzovani datoveho typu novo definovanej premennej)
	#define SEMANTIC_ERR_TYPE 				5 // semanticka chyba v programe (chyba typovej kompability v aritmetickych/retazovych/relacnych vyrazoch) // invalid operation v Go
	#define SEMANTIC_ERR_PARAM				6 // semanticka chyba v programe (zly pocet/typ parametrov volania funkcie -- zly pocet/typ navratovych hodnot)
	#define SEMANTIC_ERR_ELSE				7 // ostatne semanticke chyby 
	#define SEMANTIC_ERR_ZERO 				9 // semanticka chyba delenia 0 
 	#define COMPILER_ERR 					99 // interna chyba prekladacu (chyba neovplivnena vstupnym programom npr. chyba alokacie pamati atd.)

#endif