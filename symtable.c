/**
 * @name symtable IFJ20
 * @author Timotej Ponek xponek00
 * @file symtable.h
 * @date 17.11.2020
 * Operácie BSTreeInsert, BSTreeDisposeTree a BSTreeSearch sú implementované na
 * základe môjho domáceho úkolu v premete IAL
*/

#include <malloc.h>
#include "symtable.h"

// Forward deklaracie funkcii, ktore nebudu potrebne pre uzivatela

// switch ktory vlozi do stromu value na zaklade jej typu
int BSTreeInsertValue(BSTreePtr *Tree, void* value, vType type);
// Alokuje pamat pre polozku stromu
int BSTreeAloc(BSTreePtr *Tree, string* name, void* value, vType type);

void BSTreeFreeValue(BSTreePtr *Tree);

/*************************************************************************/

void BSTreeInit(BSTreePtr *Tree)
{
    *Tree = NULL;
}

/**
 * @return vracia 1 ak nastala chyba (malloc), 0 ak OK
 */
int BSTreeInsert(BSTreePtr *Tree, string* name, void* value, vType type, bool DefVar) 
{
	if (*Tree == NULL){
	    return BSTreeAloc(&(*Tree), name, value, type);
	}

	else{
		BSTreePtr search = *Tree;
		while (search)
		{
			// snad idem do lava
			if (strCmpString(&search->iName, name) > 0){
				if (search->LPtr == NULL){
					BSTreeAloc(&search->LPtr, name, value, type);
					break;
				}
				search = search->LPtr;
			}
			else if (strCmpString(&search->iName, name) < 0){
				if (search->RPtr == NULL){
					BSTreeAloc(&search->RPtr, name, value, type);
					break;
				}
				search = search->RPtr;
			}
			else if (!strCmpString(&search->iName, name)){
				// priradenie
				if (search->iClass == type){
					;
				}
				// deklaracia premennej
				else if	(search->iClass == TYPE_DEFAULT){
					if (!DefVar)
					;
					else if (BSTreeInsertValue(&search, value, type)){
						return ALLOC_ERR;
					}
					else
						search->iClass = type;
				}

				else 
					return TABLE_REDEF;
				return TABLE_OK;
			}
		}
	}
	return TABLE_OK;
}



int BSTreeInsertValue(BSTreePtr *Tree, void* value, vType type)
{
	// ak este vkladame len parameter bez hodnoty
	if (value == NULL) return TABLE_OK;

    switch(type)
	{
		case TYPE_DEFAULT:
		case FLOAT:
        case STRING:
        case INT:
            (*Tree)->itemValue.nesting = *(int*) value;
            break;
        
        case FNC_NAME:
            if (((*Tree)->itemValue.functionPtr = malloc(sizeof(struct BSTreeFunction))) == NULL)
				return ALLOC_ERR;
			else
			{
				(*Tree)->itemValue.functionPtr->parameters = 0;
				(*Tree)->itemValue.functionPtr->rettypes = 0;
				(*Tree)->itemValue.functionPtr->FparamType = NULL;
				(*Tree)->itemValue.functionPtr->FreturnType = NULL;
			}
            break;
        default:
            break;
    }
	return TABLE_OK;
}

int BSTreeAloc(BSTreePtr *Tree, string* name, void* value, vType type)
{
	BSTreePtr inserted = malloc(sizeof(struct BSTree));

	if	(inserted == NULL){
		fprintf(stderr, "Chyba pri alokaci pameti!\n");
		return ALLOC_ERR;
	}
		// ak zlyha malloc alebo kopirovanie stringu
		if (strInit(&inserted->iName) || strCopyString(&inserted->iName, name)){
			strFree(&inserted->iName);
			free(inserted);
			return ALLOC_ERR;
		}
	    else if(BSTreeInsertValue(&inserted, value, type)){
			strFree(&inserted->iName);
			free(inserted);
			return ALLOC_ERR;
		}
		inserted->iClass = type;
		inserted->LPtr = NULL;
		inserted->RPtr = NULL;


		(*Tree) = inserted;
	return TABLE_OK;
}

/**
 * Maze tabuku symbolov
 * @param Tree tabulka na vymazanie
 */
void BSTreeDisposeTree(BSTreePtr *Tree)
{
	if(*Tree == NULL) return;
	
	if ((*Tree)->RPtr){
		BSTreeDisposeTree(&(*Tree)->RPtr);
	}
	if ((*Tree)->LPtr){
		BSTreeDisposeTree(&(*Tree)->LPtr);
	}
	BSTreeFreeValue(&(*Tree));
	strFree(&(*Tree)->iName);
	free(*Tree);
	(*Tree) = NULL;
}



void BSTreeFreeValue(BSTreePtr *Tree)
{
	switch((*Tree)->iClass){
        case FNC_NAME:
			// odstran parametre
			if ((*Tree)->itemValue.functionPtr->parameters){
				free((*Tree)->itemValue.functionPtr->FparamType);
			}
			// odstran rettypes
			if ((*Tree)->itemValue.functionPtr->rettypes){
				free((*Tree)->itemValue.functionPtr->FreturnType);
			}
			free((*Tree)->itemValue.functionPtr);
            break;
        default:
            break;
    }
}

/**
 * Tuto funkciu pouzivat ak chcem zistit ci bola premenna uz deklarovana pri ':=' a vratit zodpovedajucu chybu
 */
bool BSTreeSearch(BSTreePtr *Tree, string *name){
	if ((*Tree) == NULL)
		return false;
	BSTreePtr search = *Tree;
		while (search)
		{
			// snad idem do lava
			if (strCmpString(&search->iName, name) > 0){
				search = search->LPtr;
			}
			else if (strCmpString(&search->iName, name) < 0){
				search = search->RPtr;
			}
			else if (!strCmpString(&search->iName, name)){
				return true;
			}	
		}	
	return false;
}

/**
 * vrati typ danej premennej a ulozi jej hodnotu do parametru
 * TYPE_ERR ak premenna nebola deklarovana
 */

vType BSTreeGet(BSTreePtr *Tree, string *name, iNestValue *getter){
	BSTreePtr search = *Tree;
		while (search)
		{
			// snad idem do lava
			if (strCmpString(&search->iName, name) > 0){
				search = search->LPtr;
			}
			else if (strCmpString(&search->iName, name) < 0){
				search = search->RPtr;
			}
			else if (!strCmpString(&search->iName, name)){
				switch(search->iClass){
        			case INT:
					case FLOAT:
					case STRING:
					case TYPE_DEFAULT:
						getter->nesting = search->itemValue.nesting;
						return search->iClass;
					break;
					case FNC_NAME:
						getter->functionPtr = search->itemValue.functionPtr;
						return FNC_NAME;
						break;
					default:
						return TYPE_ERR;
						break;
			}	
		}	
	}
	// tabulka bola NULL
	return TYPE_ERR;
}


/**
 * Pouzivat v pripade definicie premennej ktora nie je v terajsej tabulke symbolov
 * Inak snad nie
 */

vType BSTreeGetFromStack(string *name, iNestValue *getter, STACK *top, int* scope){
	STACK tmp = (*top);

	BSTreePtr search = *(tmp->table);
	*scope = tmp->scopeNMB;

	// symbol nedefinovany na terajsej tabulke
		while (search == NULL){
			if((tmp = tmp->previous)){
				search = *(tmp->table);						
				*scope = tmp->scopeNMB;
			}
		}

		while (search || tmp)
		{
			// vo vyssom scope je prazdna tabulka
			if(search == NULL){
				if((tmp = tmp->previous)){
					search = *(tmp->table);
					*scope = tmp->scopeNMB;
				}
			}
			// snad idem do lava
			else if (strCmpString(&search->iName, name) > 0){
				search = search->LPtr;
				if (search == NULL){
					if((tmp = tmp->previous)){
						search = *(tmp->table);
						*scope = tmp->scopeNMB;					
					}
					// vracia type default lebo
					// dana premenna este nema priradenu hodnotu
					else
						return TYPE_DEFAULT;
				}
			}
			else if (strCmpString(&search->iName, name) < 0){
				search = search->RPtr;
				if (search == NULL){
					if((tmp = tmp->previous)){
						search = *(tmp->table);						
					}
					// vracia type default lebo
					// dana premenna este nema priradenu hodnotu
					else
						return TYPE_DEFAULT;

				}
			}
			else if (!strCmpString(&search->iName, name)){
				switch(search->iClass){
					case STRING:
					case FLOAT:
        			case INT:
						getter->nesting = search->itemValue.nesting;
						*scope = tmp->scopeNMB;
						return search->iClass;
						break;

					case FNC_NAME:
						getter->functionPtr = search->itemValue.functionPtr;
						return FNC_NAME;
						break;

					// premenna rovnakeho mena vo vyssom scope, hladaj dalej
					// napr. b := b + 6
					case TYPE_DEFAULT:
						if((tmp = tmp->previous)){
							search = *(tmp->table);
							*scope = tmp->scopeNMB;
						}
						break;

					default:
						return TYPE_ERR;
						break;
			}
		}	
	}
	// tabulka bola NULL
	return TYPE_ERR;
}

/**
 * Funkcia hlada id vo vsetkych tabulkach na stacku
 * @param name hladane id
 * @param top stack
 * @return true ak bol id najdeny, false inak
 */

bool BSTreeSearchFromStack(string *name, STACK *top, int* scope){
	STACK tmp = (*top);

	BSTreePtr search = *(tmp->table);
	*scope = tmp->scopeNMB;
		
		// symbol nedefinovany na terajsej tabulke
		while (search == NULL){
			if((tmp = tmp->previous)){
				search = *(tmp->table);
				*scope = tmp->scopeNMB;
			}
		}
		while (search || tmp)
		{
			if(search == NULL){
				if((tmp = tmp->previous)){
				search = *(tmp->table);
				*scope = tmp->scopeNMB;
				}						
			}
			// snad idem do lava
			else if (strCmpString(&search->iName, name) > 0){
				search = search->LPtr;
				if (search == NULL){
					if((tmp = tmp->previous)){
						search = *(tmp->table);						
					}

				}
			}
			else if (strCmpString(&search->iName, name) < 0){
				search = search->RPtr;
				if (search == NULL){
					if((tmp = tmp->previous)){
						search = *(tmp->table);	
						*scope = tmp->scopeNMB;					
					}
				}
			}
			else if(!strCmpString(&search->iName, name)){
				*scope = tmp->scopeNMB;
			// nedefinovane id, hladaj dalej..
				if(search->iClass == TYPE_DEFAULT){
					// na stacku je vlozena neinicializovana premenna, hladaj dalej
					// premenna rovnakeho mena vo vyssom scope, hladaj dalej

					if((tmp = tmp->previous)){
						search = *(tmp->table);
						*scope = tmp->scopeNMB;
					}
					
					return true; // a toto vymaz
				}

				else
					return true;
			}
			
		}	
	// tabulka bola NULL
	return false;
}

////////////////////////////////////////////////////////////////

void tStackInit (STACK* top)
{
	*top = NULL;
}

int tStackPush (STACK* top, BSTreePtr *ptr)
{

	stackItem new = malloc (sizeof(struct lStack));
	if (new == NULL){
		fprintf(stderr, "Podtecenie stacku -- nedostatok pamete, tabulka nebola vlozena na stack");
		return ALLOC_ERR; // ERR?
	}
	new->table = ptr;
	if(*top == NULL)
		new->scopeNMB = 0;
	else
		new->scopeNMB = (*top)->scopeNMB + 1;
	// ak bol top NULL, prvok najnizsie bude mat prev tiez null
		new->previous = *top;
		*top = new;
	return TABLE_OK;
}

//ptm hladanie v tabulke implementovat rekurzivne
BSTreePtr* tStackTop (STACK* top){
	return (*top)->table;
}


void tStackPop (STACK* top){
	
		stackItem newTop = (*top)->previous;
		free(*top);
		*top = newTop;
}

bool tStackEmpty (STACK* top){
	return (*top == NULL);
}

/////////////////////////////////////////////////////////////////////////////////////////

/**
 * @return TABLE_OK ak sme insert-li parameter alebo sme nic nenasli, ALLOC_ERR inak
 */
int BSTreeFuncAddParamType(BSTreePtr *Tree, string *fncName, vType paramType) 
{
	if (*Tree == NULL){
	    return TABLE_OK; 
	}

	else{
		BSTreePtr search = *Tree;
		while (search)
		{
			// snad idem do lava
			if (strCmpString(&search->iName, fncName) > 0){
				search = search->LPtr;
			}
			else if (strCmpString(&search->iName, fncName) < 0){
				search = search->RPtr;
			}
			else if (!strCmpString(&search->iName, fncName)){
				if (search->iClass == FNC_NAME){
					int which = search->itemValue.functionPtr->parameters++;
					if(search->itemValue.functionPtr->FparamType == NULL){
						(search->itemValue.functionPtr->FparamType) = (vType*)malloc(sizeof(vType));
						if (search->itemValue.functionPtr->FparamType == NULL) 
							return ALLOC_ERR;
						search->itemValue.functionPtr->FparamType[which] = paramType;
					}
					// realloc
					else{
						vType* tmp = (search->itemValue.functionPtr->FparamType);
						(search->itemValue.functionPtr->FparamType) = (vType*)malloc((which+1)*sizeof(vType));
						if (search->itemValue.functionPtr->FparamType == NULL){
							free(tmp);
							return ALLOC_ERR;
						}
							
						search->itemValue.functionPtr->FparamType[which] = paramType;

						while (which > 0){
							--which;
							(search->itemValue.functionPtr->FparamType[which]) = tmp[which];
						}
						free(tmp);
						
					}

					return TABLE_OK;
				}
			}	
		}
	}
	return TABLE_OK;
}

/**
 * @return TABLE_OK ak sme insert-li return type alebo sme nic nenasli, ALLOC_ERR inak
 */
int BSTreeFuncAddRetType(BSTreePtr *Tree, string *fncName, vType retType)
{
	if (*Tree == NULL){
	    return TABLE_OK;
	}

	else{
		BSTreePtr search = *Tree;
		while (search)
		{
			// snad idem do lava
			if (strCmpString(&search->iName, fncName) > 0){
				search = search->LPtr;
			}
			else if (strCmpString(&search->iName, fncName) < 0){
				search = search->RPtr;
			}
			else if (!strCmpString(&search->iName, fncName)){
				if (search->iClass == FNC_NAME){
					int which = search->itemValue.functionPtr->rettypes++;
					if(search->itemValue.functionPtr->FreturnType == NULL){
						(search->itemValue.functionPtr->FreturnType) = (vType*)malloc(sizeof(vType));
						if ((search->itemValue.functionPtr->FreturnType) == NULL)
							return ALLOC_ERR;
						search->itemValue.functionPtr->FreturnType[which] = retType;
					}
					// realloc
					else{
						vType* tmp = (search->itemValue.functionPtr->FreturnType);
						(search->itemValue.functionPtr->FreturnType) = (vType*)malloc((which+1)*sizeof(vType));
						if ((search->itemValue.functionPtr->FreturnType) == NULL){
							free(tmp);
							return ALLOC_ERR;
						}
						
						search->itemValue.functionPtr->FreturnType[which] = retType;

						while (which > 0){
							--which;
							(search->itemValue.functionPtr->FreturnType[which]) = tmp[which];
						}
						free(tmp);
						
					}
					return TABLE_OK;
				}
			}	
		}
	}
	return TABLE_OK;
}



/**
 * Funkcia vracia hodnotu isDefined
 * @param Tree ukazatel na tabulku symbolov
 * @param fncnNme nazov hladanej funkcie
 * @return aktualna hodnota isDefined true-definovana, false-bez definicie
 */
bool BSTreeFuncIsDefined(BSTreePtr *Tree, string *fncName){
	if (*Tree == NULL){
	    return false;
	}

	else{
		BSTreePtr search = *Tree;
		while (search)
		{
			// snad idem do lava
			if (strCmpString(&search->iName, fncName) > 0){
				search = search->LPtr;
			}
			else if (strCmpString(&search->iName, fncName) < 0){
				search = search->RPtr;
			}
			else if (!strCmpString(&search->iName, fncName)){
				if (search->iClass == FNC_NAME){
						return search->itemValue.functionPtr->isDefined;
					}
				}
		}	
	}
	return false;
}


/**
 * Funkcia nastavuje hodnotu isDefined
 * @param Tree ukazatel na tabulku symbolov
 * @param fncnNme nazov nastavovanej funkcie
 * @param value hodnota 
 */
void BSTreeFuncSetDefined(BSTreePtr *Tree, string *fncName, bool value){
	if (*Tree == NULL){
	    return;
	}

	else{
		BSTreePtr search = *Tree;
		while (search)
		{
			// snad idem do lava
			if (strCmpString(&search->iName, fncName) > 0){
				search = search->LPtr;
			}
			else if (strCmpString(&search->iName, fncName) < 0){
				search = search->RPtr;
			}
			else if (!strCmpString(&search->iName, fncName)){
				if (search->iClass == FNC_NAME){
						search->itemValue.functionPtr->isDefined = value;
						return;
					}
				}
		}	
	}
}

/**
 * Implementovany pruchod stromu sposobom InOrder
 * @return true ak boli vsetky fnce definovane, false inak
 */
bool BSTreeFuncCheckAllDefined(BSTreePtr *Tree){
	if (*Tree != NULL){
	    BSTreeFuncCheckAllDefined(&(*Tree)->LPtr);
		if((*Tree)->itemValue.functionPtr->isDefined == false)
			return false;
		else
			BSTreeFuncCheckAllDefined(&(*Tree)->RPtr);
	}
	return true;
}

/**
 * 
 */
vType BSTreeGetType(string *name, STACK *top){
	iNestValue tmp;
	int scope;

	return BSTreeGetFromStack(name, &tmp, top, &scope);
}