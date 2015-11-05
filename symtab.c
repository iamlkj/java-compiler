#include "header.h"
#include "symtab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#define HASHBUNCH 1

void initSymTab( struct SymTable *table )
{	int i;
	for( i=0 ; i<HASHBUNCH ; ++i )
		table->entry[i] = 0;

	table->loopVarDepth = 0;
	table->loopVar = 0;
}

int hashFunc( const char *str )
{
	int result = 0;
	int i;
	for( i=0 ; i<strlen(str) ; i++ ) {
		i += str[i];
	}

	return (result % HASHBUNCH);
}

void insertTab( struct SymTable *table, struct SymNode *newNode )
{
	int location = hashFunc( newNode->name );

	if( table->entry[location] == 0 ) {	// the first
		table->entry[location] = newNode;
	} 
	else {
		struct SymNode *nodePtr;
		for( nodePtr=table->entry[location] ; (nodePtr->next)!=0 ; nodePtr=nodePtr->next );
		nodePtr->next = newNode;
		newNode->prev = nodePtr;
	}
}

void pushLoopVar( struct SymTable *table, struct SymNode *newNode )
{
	if( table->loopVar == 0 ) {	// the first
		table->loopVar = newNode;
	} 
	else {
		struct SymNode *nodePtr;
		for( nodePtr=(table->loopVar) ; (nodePtr->next)!=0 ; nodePtr=(nodePtr->next) );
		nodePtr->next = newNode;
		newNode->prev = nodePtr;
	}

	(table->loopVarDepth)++;
}

void popLoopVar( struct SymTable *table )
{
	struct SymNode *nodePtr;

	if(table->loopVar == NULL)
		return;

	if( table->loopVar->next == 0 ) {
		table->loopVar = 0;
	}
	else {
		for( nodePtr=(table->loopVar) ; (nodePtr->next->next)!=0 ; nodePtr=(nodePtr->next) );
		nodePtr->next = 0;
	}
	(table->loopVarDepth)--;
}

struct SymNode *createLoopVarNode( const char *name )
{
	struct SymNode *newNode = (struct SymNode *)malloc( sizeof(struct SymNode) );
	/* setup name */
	newNode->name = (char *)malloc(sizeof(char)*(strlen(name)+1));
	strcpy( newNode->name, name );
	/* setup scope */
	newNode->scope = -1;
	/* setup type */
	newNode->type = (struct PType *)malloc(sizeof(struct PType));
	newNode->type->isError = __FALSE;
	newNode->type->isArray = __FALSE;
	newNode->type->type = INTEGER_t;
	newNode->type->dimNum = 0;
	newNode->type->dim = 0;
	/* Category: variable */
	newNode->category = LOOPVAR_t;
	/* without attribute */
	newNode->attribute = 0;

	newNode->next = 0;
	newNode->prev = 0;

	return newNode;
}

struct SymNode* createVarNode( const char *name, int scope, struct PType *type ) 
{
	struct SymNode *newNode = (struct SymNode *)malloc( sizeof(struct SymNode) );
	/* setup name */
	newNode->name = (char *)malloc(sizeof(char)*(strlen(name)+1));
	strcpy( newNode->name, name );
	/* setup scope */
	newNode->scope = scope;
	/* setup type */
	newNode->type = type;
	/* Category: variable */
	newNode->category = VARIABLE_t;
	/* without attribute */
	newNode->attribute = 0;

	newNode->next = 0;
	newNode->prev = 0;

	return newNode;
}

struct SymNode* createParamNode( const char *name, int scope, struct PType *type )
{
	struct SymNode *newNode = (struct SymNode *)malloc( sizeof(struct SymNode) );
	/* setup name */
	newNode->name = (char *)malloc(sizeof(char)*(strlen(name)+1));
	strcpy( newNode->name, name );
	/* setup scope */
	newNode->scope = scope;
	/* setup type */
	newNode->type = type;
	/* Category: parameter */
	newNode->category = PARAMETER_t;
	/* without attribute */
	newNode->attribute = 0;

	newNode->next = 0;
	newNode->prev = 0;

	return newNode;	
}

struct SymNode * createConstNode( const char *name, int scope, struct PType *pType, struct ConstAttr *constAttr )
{
	struct SymNode *newNode = (struct SymNode *)malloc( sizeof(struct SymNode) );
	// setup name /
	newNode->name = (char *)malloc(sizeof(char)*(strlen(name)+1));
	strcpy( newNode->name, name );
	//* setup scope /
	newNode->scope = scope;
	//* setup type /
	newNode->type = pType;
	//* Category: constant /
	newNode->category = CONSTANT_t;
	//* setup attribute /
	newNode->attribute = (union SymAttr*)malloc(sizeof(union SymAttr));
	newNode->attribute->constVal = constAttr;

	newNode->next = 0;
	newNode->prev = 0;

	return newNode;
}

struct SymNode *createFuncNode( const char *name, int scope, struct PType *pType, struct FuncAttr *params )
{
	struct SymNode *newNode = (struct SymNode *)malloc( sizeof(struct SymNode) );
	// setup name /
	newNode->name = (char *)malloc(sizeof(char)*(strlen(name)+1));
	strcpy( newNode->name, name );
	//* setup scope /
	newNode->scope = scope;
	//* setup type /
	newNode->type = pType;
	//* Category: constant /
	newNode->category = FUNCTION_t;
	//* setup attribute /
	newNode->attribute = (union SymAttr*)malloc(sizeof(union SymAttr));
	newNode->attribute->formalParam = params;

	newNode->next = 0;
	newNode->prev = 0;

	return newNode;
}

struct SymNode *createProgramNode( const char *name, int scope, struct PType *pType )
{
	struct SymNode *newNode = (struct SymNode *)malloc( sizeof(struct SymNode) );

	// setup name /
	newNode->name = (char *)malloc(sizeof(char)*(strlen(name)+1));
	strcpy( newNode->name, name );
	//* setup scope /
	newNode->scope = scope;
	//* setup type: void /
	newNode->type = pType;
	//newNode->type = createPType( VOID_t );
	//* Category: program /
	newNode->category = PROGRAM_t;
	/* without attribute */
	newNode->attribute = 0;

	newNode->next = 0;
	newNode->prev = 0;

	return newNode;
}

/**
 * __BOOLEAN currentScope: true: only search current scope
 */

struct SymNode *lookupSymbol( struct SymTable *table, const char *id, int scope, __BOOLEAN currentScope )
{
	int index = hashFunc( id );

	struct SymNode *nodePtr, *result=0;
	for( nodePtr=(table->entry[index]) ; nodePtr!=0 ; nodePtr=(nodePtr->next) ) {
		if( !strcmp(nodePtr->name,id) && ((nodePtr->scope)==scope) ) {
			return nodePtr;
		}
	}
	// not found...
	if( scope == 0 )	return 0;	// null
	else {
		if( currentScope == __TRUE ) {
			return 0;
		}
		else {
			return lookupSymbol( table, id, scope-1, __FALSE );
		}
	}
}

struct SymNode *lookupLoopVar( struct SymTable *table, const char *id )
{
	int depth = table->loopVarDepth;
	int i;
	struct SymNode *nodePtr;
	if( depth == 0 ) {
		return 0;
	}
	else {
		for( i=0, nodePtr=(table->loopVar) ; i<depth ; i++, nodePtr=(nodePtr->next) ) {
			if( !strcmp(nodePtr->name,id) ) {
				return nodePtr;
			}
		}
	}
	return 0;
}

void deleteSymbol( struct SymNode *symbol )
{
	// delete name
	if( symbol->name != 0 )
		free( symbol->name );
	// delete PType
	deletePType( symbol->type );
	// delete SymAttr, according to category
	deleteSymAttr( symbol->attribute, symbol->category );
	//
	symbol->next = 0;
	symbol->prev = 0;

	free( symbol );
}

void deleteScope( struct SymTable *table, int scope )
{
	int i;

	//struct SymNode *collectList = 0;

	struct SymNode *current, *previous;
	for( i=0 ; i<HASHBUNCH ; ++i ) {
		if( table->entry[i] == 0 ) {	// no element in this list
			continue;
		}
		else if( table->entry[i]->next == 0 ) {	// only one element in this list
			if( table->entry[i]->scope == scope ) {
				//deleteSymbol( table->entry[i] );
				table->entry[i] = 0;
			}
		}
		else {
			for( previous=(table->entry[i]), current=(table->entry[i]->next) ; current!=0 ; previous=current, current=(current->next) ) {
				if( previous->scope == scope ) {
					if( previous->prev == 0 ) {
						previous->next->prev = 0;
						table->entry[i] = current;
						//deleteSymbol( previous );
					}
					else {
						previous->prev->next = current;
						current->prev = previous->prev;
						//deleteSymbol( previous );
					}
				}
			}
			if( previous->scope == scope ) {
				//previous->prev->next = 0;
				if( previous->prev == 0 ) {
					table->entry[0] = 0;
					//deleteSymbol( previous );
				}
				else {
					previous->prev->next = 0;
					//deleteSymbol( previous );

				}
			}

		}

	}
}
/**
 * if flag == 1, invoked at symbol table dump
 */ 
void printType( struct PType *type, int flag )
{
	char buffer[50];
	memset( buffer, 0, sizeof(buffer) );
	struct PType *pType = type;

	switch( pType->type ) {
	 case INTEGER_t:
	 	sprintf(buffer, "integer");
		break;
	 case REAL_t:
	 	sprintf(buffer, "real");
		break;
	 case BOOLEAN_t:
	 	sprintf(buffer, "boolean");
		break;
	 case STRING_t:
	 	sprintf(buffer, "string");
		break;
	 case VOID_t:
	 	sprintf(buffer, "void");
		break;
	}

	int i;
	struct ArrayDimNode *ptrr;
	for( i=0, ptrr=pType->dim ; i<(pType->dimNum) ; i++,ptrr=(ptrr->next) ) {
		char buf[15];
		memset( buf, 0, sizeof(buf) );
		sprintf( buf, "[%d]", ptrr->size );
		//printf("[%d]", ptrr->size);
		strcat( buffer, buf  );
	}
	if( flag == 1 )
		printf("%-17s\t", buffer);
	else
		printf("%s",buffer );
}

void printSymTable( struct SymTable *table, int __scope )
{
	int i;
	/*
	for(i=0;i< 115;i++)
		printf("=");
	printf("\n");
	*/
	printf("%-32s\t%-11s\t%-11s\t%-17s\t%-11s\t\n","Name","Kind","Level","Type","Attribute");
	for(i=0;i< 110;i++)
		printf("-");
	printf("\n");
	struct SymNode *ptr;
	for( i=0 ; i<HASHBUNCH ; i++ ) {
		for( ptr=(table->entry[i]) ; ptr!=0 ; ptr=(ptr->next) ) {
			if( ptr->scope == __scope ) {
				printf("%-32s\t", ptr->name);

				switch( ptr->category ) {
				 case VARIABLE_t:
					printf("%-11s\t", "variable");
					break;
			 	 case CONSTANT_t:
			 		printf("%-11s\t", "constant");
					break;
				 case PROGRAM_t:
				 	printf("%-11s\t", "program   ");
					break;
				 case FUNCTION_t:
				 	printf("%-11s\t", "function  ");
					break;
				 case PARAMETER_t:
				 	printf("%-11s\t", "parameter ");
					break;
				}

				if( ptr->scope == 0 ) {
					printf("%d%-10s\t", ptr->scope, "(global)");
				}
				else {
					printf("%d%-10s\t", ptr->scope, "(local)");
				}

				printType( ptr->type, 1 );
			
				if( ptr->category == FUNCTION_t ) {
					int i;
					struct PTypeList *pTypePtr;
					for( i=0, pTypePtr=(ptr->attribute->formalParam->params) ; i<(ptr->attribute->formalParam->paramNum) ; i++, pTypePtr=(pTypePtr->next) ) {
						printType( pTypePtr->value, 0 );
						printf(", ");
					}
				}
				else if( ptr->category == CONSTANT_t ) {
					switch( ptr->attribute->constVal->category ) {
					 case INTEGER_t:
						printf("%d",ptr->attribute->constVal->value.integerVal);
						break;
					 case REAL_t:
					 	printf("%lf",ptr->attribute->constVal->value.realVal);
						break;
					 case BOOLEAN_t:
					 	if( ptr->attribute->constVal->value.booleanVal == __TRUE ) 
							printf("true");
						else
							printf("false");
						break;
					 case STRING_t:
					 	printf("%s",ptr->attribute->constVal->value.stringVal);
						break;
					}
				}

				printf("\n");
			}	// if( ptr->scope == __scope )
		}	// for( ptr=(table->entry[i]) ; ptr!=0 ; ptr=(ptr->next) )
	}	// for( i=0 ; i<HASHBUNCH ; i++ )
	for(i=0;i< 110;i++)
		printf("-");
	printf("\n");
	/*
	for(i=0;i< 115;i++)
		printf("=");
	printf("\n");
	*/
}

