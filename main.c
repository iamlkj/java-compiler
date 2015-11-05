/**
 * Introduction to Compiler Design by Prof. Yi Ping You, spring 2012
 * Prjoect 3 main function
 * Last Modification: 2012/06/14 12:19
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"
#include "symtab.h"

extern int yydebug;	/* declared by yacc */

extern int yyparse();	/* declared by yacc */
extern FILE* yyin;	/* declared by lex */
extern FILE* out;
extern struct SymTable *symbolTable;
extern struct PType *funcReturn;
extern char fileName[256];
FILE *out;
int  main( int argc, char **argv )
{
	if( argc != 2 ) {
		fprintf(  stdout,  "Usage:  ./parser  [filename]\n"  );
		exit(0);
	}
	//out=fopen("out.txt","w");
	FILE *fp = fopen( argv[1], "r" );

	if( fp == NULL )  {
		fprintf( stdout, "Open  file  error\n" );
		exit(-1);
	}

	// parse argv[1] and setup fileName
	char fileNameBuf[256];
	memset( fileName, 0, 256 );
	memset( fileNameBuf, 0, 256 );
	strcpy( fileNameBuf, argv[1] );
	char *token = strtok( fileNameBuf," /\t\n");
	char *prevTok;
	if( !strncmp(token,argv[1],strlen(argv[1])) ) {
		strcpy( fileName, argv[1] );
	}
	else {
		for( ; token!=0 ; prevTok=token, token=strtok(NULL," /\t\n") );
		strcpy( fileName, prevTok );
	}

	if( !((fileName[strlen(fileName)-2]=='.')&&(fileName[strlen(fileName)-1]=='p')) ) {
		fprintf( stdout, "input file: %s \tshould be [fileName].p \n", fileName );	
		exit(-1);
	}
	fileName[strlen(fileName)-2] = '\0';

	yyin = fp;

	// initial symbol table
	symbolTable = (struct SymTable *)malloc(sizeof(struct SymTable));
	initSymTab( symbolTable );

	// initial function return recoder

	yydebug = 0;

	yyparse();	/* primary procedure of parser */
	fclose(out);
	exit(0);
}

