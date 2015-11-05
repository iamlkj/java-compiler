%{
/**
 * Introduction to Compiler Design by Prof. Yi Ping You, spring 2012
 * Project 3 YACC sample
 * Last Modification : 2012/06/14 12:18
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"
#include "symtab.h"
#include "semcheck.h"

//#include "test.h"

int yydebug;

extern int linenum;		/* declared in lex.l */
extern FILE *yyin;		/* declared by lex */

extern char *yytext;		/* declared by lex */
extern char buf[256];		/* declared in lex.l */
FILE *out;
int i;
int scope = 0;
int varcount=0;
int Opt_D = 1;			/* symbol table dump option */
char fileName[256];
char* LocalItem[1000];
int ItemScope[1000];
SEMTYPE ItemType[1000];
char* ExprStack[1000];
int StackCount=0;
int LocalCount=0;
struct SymTable *symbolTable;	// main symbol table
int lCount=0;
int lC[20];
int lCtop=0;
__BOOLEAN paramError;			// indicate is parameter have any error?

struct PType *funcReturn;		// record function's return type, used at 'return statement' production rule
int dumpEStack(const char*);
void superfree(char**);
%}

%union {
	int intVal;
	float realVal;
	//__BOOLEAN booleanVal;
	char *lexeme;
	struct idNode_sem *id;
	//SEMTYPE type;
	struct ConstAttr *constVal;
	struct PType *ptype;
	struct param_sem *par;
	struct expr_sem *exprs;
	/*struct var_ref_sem *varRef; */
	struct expr_sem_node *exprNode;
};

/* tokens */
%token ARRAY BEG BOOLEAN DEF DO ELSE END FALSE FOR INTEGER IF OF PRINT READ REAL RETURN STRING THEN TO TRUE VAR WHILE
%token OP_ADD OP_SUB OP_MUL OP_DIV OP_MOD OP_ASSIGN OP_EQ OP_NE OP_GT OP_LT OP_GE OP_LE OP_AND OP_OR OP_NOT
%token MK_COMMA MK_COLON MK_SEMICOLON MK_LPAREN MK_RPAREN MK_LB MK_RB

%token <lexeme>ID
%token <intVal>INT_CONST 
%token <realVal>FLOAT_CONST
%token <realVal>SCIENTIFIC
%token <lexeme>STR_CONST

%type<id> id_list
%type<constVal> literal_const
%type<ptype> type scalar_type array_type ret_type
%type<par> param param_list opt_param_list
%type<exprs> var_ref boolean_expr boolean_term boolean_factor relop_expr expr term factor boolean_expr_list opt_boolean_expr_list
%type<intVal> dim mul_op add_op rel_op array_index loop_param

/* start symbol */
%start program
%%

program			: ID
			{
				char b[30];
				strcat(b,fileName);
				strcat(b,".j");
				out=fopen(b,"w");
		//	if(ExprStack[StackCount]==NULL)ExprStack[StackCount]=(char*)malloc(sizeof(char)*10000);
			  struct PType *pType = createPType( VOID_t );
			  struct SymNode *newNode = createProgramNode( $1, scope, pType );
			  insertTab( symbolTable, newNode );
			  
			  if( strcmp(fileName,$1) ) {
				fprintf( stdout, "<Error> found in Line %d: program beginning ID inconsist with file name  \n", linenum );
			  }
			  fprintf(out,"; %s.j \n",$1);
			  fprintf(out,".class public %s \n",$1);
			  fprintf(out,".super java/lang/Object\n");
			  fprintf(out,".field public static _sc Ljava/util/Scanner;\n");

			}
			  MK_SEMICOLON
			  program_body
			  END ID
			{
			  if( strcmp($1, $6) ) { fprintf( stdout, "<Error> found in Line %d: %s", linenum,"Program end ID inconsist with the beginning ID  \n"); }
			  if( strcmp(fileName,$6) ) {
				 fprintf( stdout, "<Error> found in Line %d: program end ID inconsist with file name  \n", linenum );
			  }
			  // dump symbol table
			  if( Opt_D == 1 )
				printSymTable( symbolTable, scope );
				fprintf(out,"return \n\
.end method");
			}
			;

program_body		: opt_decl_list opt_func_decl_list {
fprintf(out,".method public static main([Ljava/lang/String;)V\n\
.limit stack 100\n\
.limit locals 100 \n\
new java/util/Scanner \n\
dup \n\
getstatic java/lang/System/in Ljava/io/InputStream; \n\
invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V \n\
putstatic %s/_sc Ljava/util/Scanner; \n ",fileName);
								
								}compound_stmt
			;


opt_decl_list		: decl_list
			| /* epsilon */		
			;

decl_list		: decl_list decl
			| decl
			;

decl			: VAR id_list MK_COLON scalar_type MK_SEMICOLON       /* scalar type declaration */
			{
			  // insert into symbol table
			  struct idNode_sem *ptr;
			  struct SymNode *newNode;
			  for( ptr=$2 ; ptr!=0 ; ptr=(ptr->next) ) {
			  	if( verifyRedeclaration( symbolTable, ptr->value, scope ) ==__FALSE ) { }
				else {
					newNode = createVarNode( ptr->value, scope, $4 );
					insertTab( symbolTable, newNode );
					//fprintf(out,"[%d haha]\n",scope);
					if(scope==0)
					{
							if($4->type==INTEGER_t)
										{fprintf(out,".field public static %s I\n",ptr->value);}
						else if(BOOLEAN_t==$4->type)
										{fprintf(out,".field public static %s Z\n",ptr->value);}
						else if(REAL_t==$4->type)
										{fprintf(out,".field public static %s F\n",ptr->value);}
					}
					else
					{
					//	fprintf(stdout,"[local %s]",ptr->value);
						LocalItem[++LocalCount]=(char*)calloc(40,sizeof(char));
						//for(int i=0;i<1000;i++)tmp[i]=0;
						ItemScope[LocalCount]=scope;
						ItemType[LocalCount]=$4->type;
						strcpy(LocalItem[LocalCount],ptr->value);
						//fprintf(stdout,"[%d\t%s]\n",LocalCount,LocalItem[LocalCount-1]);
					}
				}
			  }
			  deleteIdList( $2 );
			}
			| VAR id_list MK_COLON array_type MK_SEMICOLON        /* array type declaration */
			{
			  verifyArrayType( $2, $4 );
			  // insert into symbol table
			  struct idNode_sem *ptr;
			  struct SymNode *newNode;
			  for( ptr=$2 ; ptr!=0 ; ptr=(ptr->next) ) {
			  	if( $4->isError == __TRUE ) { }
				else if( verifyRedeclaration( symbolTable, ptr->value, scope ) ==__FALSE ) { }
				else {
					newNode = createVarNode( ptr->value, scope, $4 );
					insertTab( symbolTable, newNode );
				}
			  }
			  deleteIdList( $2 );
			}
			| VAR id_list MK_COLON literal_const MK_SEMICOLON     /* const declaration */
			{
			  struct PType *pType = createPType( $4->category );
			  // insert constants into symbol table
			  struct idNode_sem *ptr;
			  struct SymNode *newNode;
			  for( ptr=$2 ; ptr!=0 ; ptr=(ptr->next) ) {
			  	if( verifyRedeclaration( symbolTable, ptr->value, scope ) ==__FALSE ) { }
				else {
					newNode = createConstNode( ptr->value, scope, pType, $4 );
					insertTab( symbolTable, newNode );
				}
			  }
			  
			  deleteIdList( $2 );
			}
			;

literal_const		: INT_CONST
			{
			  int tmp = $1;
			  $$ = createConstAttr( INTEGER_t, &tmp );
			}
			| OP_SUB INT_CONST
			{
			  int tmp = -$2;
			  $$ = createConstAttr( INTEGER_t, &tmp );
			}
			| FLOAT_CONST
			{
			  float tmp = $1;
			  $$ = createConstAttr( REAL_t, &tmp );
			}
			| OP_SUB FLOAT_CONST
			{
			  float tmp = -$2;
			  $$ = createConstAttr( REAL_t, &tmp );
			}
			| SCIENTIFIC 
			{
			  float tmp = $1;
			  $$ = createConstAttr( REAL_t, &tmp );
			}
			| OP_SUB SCIENTIFIC
			{
			  float tmp = -$2;
			  $$ = createConstAttr( REAL_t, &tmp );
			}
			| STR_CONST
			{
			  $$ = createConstAttr( STRING_t, $1 );
			}
			| TRUE
			{
			  SEMTYPE tmp = __TRUE;
			  $$ = createConstAttr( BOOLEAN_t, &tmp );
			}
			| FALSE
			{
			  SEMTYPE tmp = __FALSE;
			  $$ = createConstAttr( BOOLEAN_t, &tmp );
			}
			;

opt_func_decl_list	: func_decl_list
			| /* epsilon */
			;

func_decl_list		: func_decl_list func_decl
			| func_decl
			;

func_decl		: ID MK_LPAREN opt_param_list
			{
			  // check and insert parameters into symbol table
			  paramError = insertParamIntoSymTable( symbolTable, $3, scope+1 );
			  //fprintf(out,"");
			  								/*struct idNode_sem*ptr;
											for(ptr=$1;ptr->next!=NULL;ptr=ptr->next)
											{
												LocalItem[++LocalCount]=(char*)calloc(40,sizeof(char));
												ItemScope[LocalCount]=scope;
											}*/
			}
			  MK_RPAREN ret_type 
			{
			  // check and insert function into symbol table
			  if( paramError == __TRUE ) {
			  	printf("--- param(s) with several fault!! ---\n");
			  } else {
				insertFuncIntoSymTable( symbolTable, $1, $3, $6, scope );
			  }
			  funcReturn = $6;
				fprintf(out,".method public static %s(",$1);
				char*ptmp=calloc(100,sizeof(char));
				//strcat(ptmp);
				struct param_sem* list;
				//fprintf(out,"[: )]\n");fflush(out);
				for(list=$3;list!=NULL;list=list->next)
				{
				//fprintf(out,"[! %d]",list->pType->type);fflush(out);
					struct idNode_sem*ptr;
					for(ptr=list->idlist;ptr!=NULL;ptr=ptr->next)
					{
						if(LocalCount==0)LocalCount=-1;
						LocalItem[++LocalCount]=(char*)calloc(40,sizeof(char));
						ItemScope[LocalCount]=scope;
						strcpy(LocalItem[LocalCount],ptr->value);
						//fprintf(out,"[inhere %d]\n",list->pType->type);fflush(out);
						switch(list->pType->type)
						{
							case INTEGER_t: fprintf(out,"I"); break;
							case BOOLEAN_t: fprintf(out,"Z"); break;
							case REAL_t:    fprintf(out,"F"); break;
							//default:	fprintf(out,"?");
						}
					}		
				}
				fprintf(out,")");
				char rettp;
				switch($6->type)
				{
							case INTEGER_t: fprintf(out,"I\n"); break;
							case BOOLEAN_t: fprintf(out,"Z\n"); break;
							case REAL_t:    fprintf(out,"F\n"); break;
							case VOID_t:	fprintf(out,"V\n"); break;				
				
				}
			  
			   fprintf(out,".limit stack 100\n");
			   fprintf(out,".limit locals 100\n"); 
 
			}
			  MK_SEMICOLON
			  compound_stmt
			  END ID
			{
			  if( strcmp($1,$11) ) {
				fprintf( stdout, "<Error> found in Line %d: the end of the functionName mismatch  \n", linenum );
			  }
			  funcReturn = 0;
			  	switch($6->type)
				{
							case INTEGER_t: fprintf(out,"ireturn \n"); break;
							case BOOLEAN_t: fprintf(out,"ireturn \n"); break;
							case REAL_t:    fprintf(out,"freturn \n"); break;
							case VOID_t:	fprintf(out,"return \n"); break;				
				
				}
			  fprintf(out,".end method \n");
			}
			;

opt_param_list		: param_list { $$ = $1; }
			| /* epsilon */ { $$ = 0; }
			;

param_list		: param_list MK_SEMICOLON param
			{
			  param_sem_addParam( $1, $3 );
			  $$ = $1;
			}
			| param { $$ = $1; }
			;

param			: id_list MK_COLON type { $$ = createParam( $1, $3 ); 
											
											
											
										}
			;

id_list			: id_list MK_COMMA ID
			{
			  idlist_addNode( $1, $3 );
			  $$ = $1;
			}
			| ID { $$ = createIdList($1); }
			;

ret_type		: MK_COLON scalar_type { $$ = $2; }
			| /* epsilon */ { $$ = createPType( VOID_t ); }

type			: scalar_type { $$ = $1; }
			| array_type { $$ = $1; }
			;

scalar_type		: INTEGER { $$ = createPType( INTEGER_t ); }
			| REAL { $$ = createPType( REAL_t ); }
			| BOOLEAN { $$ = createPType( BOOLEAN_t ); }
			| STRING { $$ = createPType( STRING_t ); }
			;

array_type		: ARRAY array_index TO array_index OF type
			{
				verifyArrayDim( $6, $2, $4 );
				increaseArrayDim( $6, $2, $4 );
				$$ = $6;
			}
			;

array_index		: INT_CONST { $$ = $1; }
			| OP_SUB INT_CONST { $$ = -$2; }
			;

stmt			: compound_stmt
			| simple_stmt
			| cond_stmt
			| while_stmt
			| for_stmt
			| return_stmt
			| proc_call_stmt
			;

compound_stmt		: 
			{ 
			  scope++;
			}
			  BEG
			  opt_decl_list
			  opt_stmt_list
			  END 
			{ 
			  // print contents of current scope
			  if( Opt_D == 1 )
			  	printSymTable( symbolTable, scope );
			  deleteScope( symbolTable, scope );	// leave this scope, delete...
			  scope--; 
			}
			;

opt_stmt_list		: stmt_list
			| /* epsilon */
			;

stmt_list		: stmt_list stmt
			| stmt
			;

simple_stmt		: var_ref OP_ASSIGN boolean_expr MK_SEMICOLON
			{
			  // check if LHS exists
			  __BOOLEAN flagLHS = verifyExistence( symbolTable, $1, scope, __TRUE );
			  // id RHS is not dereferenced, check and deference
			  __BOOLEAN flagRHS = __TRUE;
			  if( $3->isDeref == __FALSE ) {
				//flagRHS = verifyExistence( symbolTable, $3, scope, __FALSE );
				flagRHS = verifyAndDerefenced( symbolTable, $3, scope, __FALSE );	
			  }
			  // if both LHS and RHS are exists, verify their type
			  if( flagLHS==__TRUE && flagRHS==__TRUE )
				verifyAssignmentTypeMatch( $1, $3 );
				fprintf(out,"%s",ExprStack[--StackCount]);
				superfree(&ExprStack[StackCount]);
			 /* if($3->varRef!=0)
			  {
			  //fprintf(out,"[haha]\n");fflush(out);
			  		struct expr_sem*v=$3;
					int shoot=0;
					struct SymNode*glo=0;
					if(glo=lookupSymbol(symbolTable, v->varRef->id, 0, __TRUE ),glo!=0)
					{
						//fprintf(out,"typ=%d  %s\n",glo->type->type,glo->name);fflush(out);
						if(glo->type->type==INTEGER_t)
 						{
 							fprintf(out,"getstatic %s/%s I\n",fileName,glo->name);
																	
 						}
 						else if(glo->type->type==BOOLEAN_t)
 						{
 							fprintf(out,"getstatic %s/%s I\n",fileName,glo->name);
																
 						}
 						else if(glo->type->type==REAL_t)
 						{
							fprintf(out,"getstatic %s/%s F\n",fileName,glo->name);
 						}								
 					}
 					else
 					{
 						for(i=LocalCount;i>0;i--)
 						{
 								
 								if(v->varRef!=0 && v->varRef->id!=0 && ItemScope[i]<=scope && strcmp(v->varRef->id,LocalItem[i])==0)
 								{
 									shoot=i;
 									break;
 								}
 						}
 						
 						if(shoot!=0)
 						{
 							if(v->pType->type==INTEGER_t)
 							{
 								fprintf(out,"iload %d\n",shoot);
 							}	
 								else if(v->pType->type==BOOLEAN_t)
 							{
 								fprintf(out,"iload %d\n",shoot);
																
 							}
 							else if(v->pType->type==REAL_t)
 							{
									fprintf(out,"fload %d\n",shoot);
 																
 							}			
						}	
					}

			  
			  }
			  else
			  {

														//fprintf(out,";[const]\n");fflush(out);
						if($3->pType->type==STRING_t)
						{
															//fprintf(stdout,"[%d]",$2->pType->type);
							fprintf(out,"ldc \"%s\"\n",$3->pType->strv);
							//fprintf(out,"invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V \n");
						}
						else if($3->pType->type==INTEGER_t)
						{
							fprintf(out,"ldc %d\n",$3->pType->value);
							//fprintf(out,"invokevirtual java/io/PrintStream/print(I)V \n");	
						}
						else if($3->pType->type==BOOLEAN_t)
						{
							if($3->pType->value.booleanVal ==__TRUE)
							{
								fprintf(out,"iconst_1\n");
							}
							else
							{
								fprintf(out,"iconst_0\n");
							}
							//fprintf(out,"ldc %d\n",$2->pType->value);
							//fprintf(out,"invokevirtual java/io/PrintStream/print(Z)V \n");
						}
						else if($3->pType->type==REAL_t)
 						{
							fprintf(out,"ldc %f\n",$3->pType->value);
 							//fprintf(out,"invokevirtual java/io/PrintStream/print(F)V \n");
 						}
			  }*/
			  
			  
			  
					struct expr_sem*v=$1;
					int shoot=-1;
 					for(i=LocalCount;i>=0;i--)
 					{
 							if( LocalItem[i]!=0 &&ItemScope[i]<=scope && strcmp(v->varRef->id,LocalItem[i])==0)
 							{
 								shoot=i;
 								break;
 							}

 					}
 					if(shoot!=-1)
 					{
 						if(v->pType->type==INTEGER_t)
 						{
 							fprintf(out,"istore %d\n",shoot);
 						}	
 						else if(v->pType->type==BOOLEAN_t)
 						{
 							fprintf(out,"istore %d\n",shoot);
															
 						}
 						else if(v->pType->type==REAL_t)
 						{
								fprintf(out,"fstore %d\n",shoot);
 															
 						}
 															//fprintf(out,";[finish]\n");fflush(out);
					}	
					else
					{
						struct SymNode*glo;
						if(glo=lookupSymbol(symbolTable, v->varRef->id, 0, __TRUE ),glo!=0)
						{
							if(glo->type->type==INTEGER_t)
 							{
 								fprintf(out,"putstatic %s/%s I\n",fileName,glo->name);
 																	
 							}
 							else if(glo->type->type==BOOLEAN_t)
 							{
 								fprintf(out,"putstatic %s/%s I\n",fileName,glo->name);
																	
 							}
 							else if(glo->type->type==REAL_t)
 							{
								fprintf(out,"putstatic %s/%s F\n",fileName,glo->name);
 							}				
 							//fprintf(stdout,"glo=%d\n",glo->category);					
 						}
					}										
				//	superfree(&ExprStack[--StackCount]);
			}
			| PRINT boolean_expr MK_SEMICOLON { verifyScalarExpr( $2, "print" ); 
												//struct expr_sem *expr=$2;
												//fprintf(out,";[vref= %s]",)
												dumpEStack("print");
												fprintf(out,"getstatic java/lang/System/out Ljava/io/PrintStream; \n");
												fprintf(out,"%s",ExprStack[--StackCount]);
												superfree(&ExprStack[StackCount]);
												switch($2->pType->type)
												{
													case STRING_t:
														fprintf(out,"invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V \n");	break;
													case INTEGER_t:
														fprintf(out,"invokevirtual java/io/PrintStream/print(I)V \n");	break;
													case BOOLEAN_t:
														fprintf(out,"invokevirtual java/io/PrintStream/print(Z)V \n");	break;
													case REAL_t:
														fprintf(out,"invokevirtual java/io/PrintStream/print(F)V \n");	break;
							
												}
													/*if($2->varRef==NULL)
													{
														//fprintf(out,";[const]\n");fflush(out);
														if($2->pType->type==STRING_t)
														{
															//fprintf(stdout,"[%d]",$2->pType->type);
															fprintf(out,"ldc \"%s\"\n",$2->pType->strv);
															fprintf(out,"invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V \n");
														}
														else if($2->pType->type==INTEGER_t)
														{
															fprintf(out,"ldc %d\n",$2->pType->value);
															fprintf(out,"invokevirtual java/io/PrintStream/print(I)V \n");	
														}
														else if($2->pType->type==BOOLEAN_t)
														{
															fprintf(out,"ldc %d\n",$2->pType->value);
															fprintf(out,"invokevirtual java/io/PrintStream/print(Z)V \n");
														}
														else if($2->pType->type==REAL_t)
 														{
															fprintf(out,"ldc %f\n",$2->pType->value);
 															fprintf(out,"invokevirtual java/io/PrintStream/print(F)V \n");
 														}
													}
													
													
													else
													{
			
														//fprintf(out,";[local]\n");fflush(out);
														int shoot=0;
 														for(i=LocalCount;i>0;i--)
 														{
 															if(ItemScope[i]<=scope && strcmp($2->varRef->id,LocalItem[i])==0)
 															{
 																shoot=i;
 																break;
 															}	
 															else
 															{
 																//fprintf(out,";[not match! %s\t%s]\n",$2->varRef->id,LocalItem[i]);fflush(out);
 															}
 														}
 														if(shoot!=0)
 														{
 															if($2->pType->type==INTEGER_t)
 															{
 																fprintf(out,"iload %d\n",shoot);
 																fprintf(out,"invokevirtual java/io/PrintStream/print(I)V \n");
 															}
 															else if($2->pType->type==BOOLEAN_t)
 															{
 																fprintf(out,"iload %d\n",shoot);
																fprintf(out,"invokevirtual java/io/PrintStream/print(Z)V \n");
 															}
 															else if($2->pType->type==REAL_t)
 															{
																fprintf(out,"fload %d\n",shoot);
 																fprintf(out,"invokevirtual java/io/PrintStream/print(F)V \n");
 															}
 															//fprintf(out,";[finish]\n");fflush(out);
														}	
														else
														{
															struct SymNode*glo;
															if(glo=lookupSymbol(symbolTable, $2->varRef->id, 0, __TRUE ),glo!=0)
															{
																if(glo->type->type==INTEGER_t)
 																{
 																	fprintf(out,"getstatic %s/%s I\n",fileName,glo->name);
 																	fprintf(out,"invokevirtual java/io/PrintStream/print(I)V \n");
 																}
 																else if(glo->type->type==BOOLEAN_t)
 																{
 																	fprintf(out,"getstatic %s/%s I\n",fileName,glo->name);
																	fprintf(out,"invokevirtual java/io/PrintStream/print(Z)V \n");
 																}
 																else if(glo->type->type==REAL_t)
 																{
																	fprintf(out,"getstatic %s/%s F\n",fileName,glo->name);
 																	fprintf(out,"invokevirtual java/io/PrintStream/print(F)V \n");
 																}
															}
														}											
													}*/
												
												}
 			| READ boolean_expr MK_SEMICOLON  { verifyScalarExpr( $2, "read" ); 
 												dumpEStack("read");
 												int shoot=-1;
 												for(i=LocalCount;i>=0;i--)
 												{
 													if(LocalItem[i]!=0 && ItemScope[i]<=scope && strcmp($2->varRef->id,LocalItem[i])==0)
 													{
 														shoot=i;
 														break;
 													}
 												
 												}
 													fprintf(out,"; invoke java.util.Scanner.nextXXX() \n");
													fprintf(out,"getstatic %s/_sc Ljava/util/Scanner; \n",fileName);
													//fprintf(out,"beforepass! %d\n",StackCount);fflush(out);
													superfree(&ExprStack[--StackCount]);
	
													//fprintf(out,"pass! %d\n",StackCount);fflush(out);
 												if(shoot==-1)
 												{
 													struct SymNode*glo;
													if(glo=lookupSymbol(symbolTable, $2->varRef->id, 0, __TRUE ),glo!=0)
													{
														if(glo->type->type==INTEGER_t)
 														{
 															fprintf(out,"invokevirtual java/util/Scanner/nextInt()I\n");
 															fprintf(out,"putstatic %s/%s I\n",fileName,glo->name);
 																	
 														}
 														else if(glo->type->type==BOOLEAN_t)
 														{
 															fprintf(out,"invokevirtual java/util/Scanner/nextBoolean()Z\n");
 															fprintf(out,"putstatic %s/%s I\n",fileName,glo->name);
																	
 														}
 														else if(glo->type->type==REAL_t)
 														{
 															fprintf(out,"invokevirtual java/util/Scanner/nextFloat()F \n");
															fprintf(out,"putstatic %s/%s F\n",fileName,glo->name);
 														}				
 														//fprintf(stdout,"glo=%d\n",glo->category);					
 													}
 												}
 												else
 												{

 													if($2->pType->type==INTEGER_t)
 													{
 														fprintf(out,"invokevirtual java/util/Scanner/nextInt()I\n");
 														fprintf(out,"istore %d\n",shoot);
 													}
 													else if($2->pType->type==BOOLEAN_t)
 													{
 														fprintf(out,"invokevirtual java/util/Scanner/nextBoolean()Z\n");
														fprintf(out,"istore %d\n",shoot);
 													}
 													else if($2->pType->type==REAL_t)
 													{
 														fprintf(out,"invokevirtual java/util/Scanner/nextFloat()F \n");
 														fprintf(out,"fstore %d\n",shoot);
 													}
 													
 												}
 												
 												dumpEStack("endread");
 												}
			;

proc_call_stmt		: ID MK_LPAREN opt_boolean_expr_list MK_RPAREN MK_SEMICOLON
			{
			struct expr_sem*ret;
			  ret=verifyFuncInvoke( $1, $3, symbolTable, scope );

				int count=0;
				 struct expr_sem*ha;
				for(ha=$3;ha!=NULL;ha=ha->next)count++;
				
				for(i=count;i>0;i--)
				{
					fprintf(out,"%s",ExprStack[StackCount-i]);
					superfree(&ExprStack[StackCount-i]);
				}
				StackCount-=count;
				
				fprintf(out,"invokestatic %s/%s(",fileName,$1);
				struct expr_sem*t;
				for(t=$3;t!=NULL;t=t->next)
				{
					switch(t->pType->type)
						{
							case INTEGER_t: fprintf(out,"I"); break;
							case BOOLEAN_t: fprintf(out,"Z"); break;
							case REAL_t:    fprintf(out,"F"); break;
							//default:	fprintf(out,"?");
						}
				}
				switch(ret->pType->type)
						{
							case INTEGER_t: fprintf(out,")I\n"); break;
							case BOOLEAN_t: fprintf(out,")Z\n"); break;
							case REAL_t:    fprintf(out,")F\n"); break;
							case VOID_t:    fprintf(out,")V\n"); break;
						}
			}
			;

cond_stmt		: IF condition THEN 
			 opt_stmt_list
			  ELSE
			  {
			  	fprintf(out,"goto Lexit_%d\n",lC[lCtop]);
			  	fprintf(out,"Lfalse_%d:\n",lC[lCtop]);
			  }
			  opt_stmt_list
			  END IF
			  {
			  		fprintf(out,"Lexit_%d:\n",lC[lCtop]);
			  		lCtop--;
			  }
			| IF condition THEN
			 opt_stmt_list END IF
			{								  		
		  		fprintf(out,"Lfalse_%d:\n",lC[lCtop]);
			  			lCtop--;	  		
			}
			;

condition		: boolean_expr { verifyBooleanExpr( $1, "if" ); 
								lC[++lCtop]=lCount++;
								fprintf(out,"%s",ExprStack[--StackCount]);
								fprintf(out,"ifeq Lfalse_%d\n",lC[lCtop]);								
								superfree(&ExprStack[StackCount]);
								} 
			;

while_stmt		: WHILE	 condition_while DO
			  opt_stmt_list{
			  		fprintf(out,"goto Lbegin_%d \n Lexit_%d:\n",lC[lCtop],lC[lCtop]);
			  		lCtop--;
			  		}
			  END DO
			;

condition_while		: boolean_expr { verifyBooleanExpr( $1, "while" ); 
									lC[++lCtop]=lCount++;
									fprintf(out,"Lbegin_%d:\n",lC[lCtop]);  
									fprintf(out,"%s",ExprStack[--StackCount]);
									superfree(&ExprStack[StackCount]);
									fprintf(out,"ifeq Lexit_%d\n",lC[lCtop]);
									} 
			;

for_stmt		: FOR ID 
			{ 
			  insertLoopVarIntoTable( symbolTable, $2 );
			  LocalItem[++LocalCount]=(char*)calloc(40,sizeof(char));
			  ItemScope[LocalCount]=scope;
			  strcpy(LocalItem[LocalCount],$2);
			}
			  OP_ASSIGN loop_param TO loop_param
			{
			  verifyLoopParam( $5, $7 );
			  lC[++lCtop]=lCount++;
			  fprintf(out,"ldc %d\n",$5);
			  int shoot=-1;
			   			for(i=LocalCount;i>=0;i--)
 						{
 								
 								if( LocalItem[i]!=0 && ItemScope[i]==scope && strcmp($2,LocalItem[i])==0)
 								{
 									shoot=i;
 									break;
 								}
 						}
 						
 						if(shoot!=-1)
 						{

 								fprintf(out,"istore %d\n",shoot);														
						}
				//lC[lCtop]
				fprintf(out,"Lbegin_%d:\n",lC[lCtop]);
						if(shoot!=-1)
 						{

 								fprintf(out,"iload %d\n",shoot); 															
						}
/*
	ifle Ltrue_4 
	iconst_0 
	goto Lfalse_4 
Ltrue_4: 
	iconst_1 
Lfalse_4: 
	ifeq Lexit_3 
*/
				fprintf(out,"i2f\n");
				fprintf(out,"ldc %d\n",$7);
				fprintf(out,"i2f\n");
				fprintf(out,"fcmpl\n");
			  	fprintf(out,"ifle Ltrue_%d\n",lC[lCtop]);
			  	fprintf(out,"iconst_0 \n");
			  	fprintf(out,"goto Lfalse_%d\n",lC[lCtop]);
			  	fprintf(out,"Ltrue_%d:\n",lC[lCtop]);
			  	fprintf(out,"iconst_1\n",lC[lCtop]);
			  	fprintf(out,"Lfalse_%d:\n",lC[lCtop]);
			  	fprintf(out,"ifeq Lexit_%d\n",lC[lCtop]);
			}
			  DO
			  opt_stmt_list
			  END DO
			{
			  popLoopVar( symbolTable );
			  			int shoot=-1;
			  			  for(i=LocalCount;i>=0;i--)
 						{
 								
 								if( LocalItem[i]!=0 &&ItemScope[i]==scope && strcmp($2,LocalItem[i])==0)
 								{
 									shoot=i;
 									break;
 								}
 						}
 						if(shoot!=-1)fprintf(out,"iload %d\n",shoot);
 						fprintf(out,"ldc 1\n");
 						fprintf(out,"iadd\n");
 						fprintf(out,"istore %d\n",shoot);
 						fprintf(out,"goto Lbegin_%d\n",lC[lCtop]);
 						fprintf(out,"Lexit_%d:\n",lC[lCtop]);
 						lCtop--;

			}
			;

loop_param		: INT_CONST { $$ = $1; }
			;

return_stmt		: RETURN boolean_expr MK_SEMICOLON
			{
			  verifyReturnStatement( $2, funcReturn );
			  fprintf(out,"%s",ExprStack[--StackCount]);
			  superfree(&ExprStack[StackCount]);
			  switch($2->pType->type)
			  {
			  
			  	case INTEGER_t:
			  		fprintf(out,"ireturn\n");
			  		break;
			  	case BOOLEAN_t:
			  		fprintf(out,"ireturn\n");
			  		break;
			  	case REAL_t:
			  		fprintf(out,"freturn\n");
			  		break;
			  }
			}
			;

opt_boolean_expr_list	: boolean_expr_list { $$ = $1; }
			| /* epsilon */ { $$ = 0; }	// null
			;

boolean_expr_list	: boolean_expr_list MK_COMMA boolean_expr
			{
			  struct expr_sem *exprPtr;
			  for( exprPtr=$1 ; (exprPtr->next)!=0 ; exprPtr=(exprPtr->next) );
			  exprPtr->next = $3;
			  $$ = $1;
			}
			| boolean_expr
			{
			  $$ = $1;
			}
			;

boolean_expr		: boolean_expr OP_OR boolean_term
			{
			  verifyAndOrOp( $1, OR_t, $3 );
			  $$ = $1;
			  	char* tmp=(char*)calloc(1000,sizeof(char));
				int ChangeType=0;

				strcat(tmp,ExprStack[StackCount-2]);

				strcat(tmp,ExprStack[StackCount-1]);

				strcat(tmp,"ior\n");
				superfree(&ExprStack[--StackCount]);
				superfree(&ExprStack[--StackCount]);
				ExprStack[StackCount]=tmp;
				StackCount++;
			}
			| boolean_term { $$ = $1; }
			;

boolean_term		: boolean_term OP_AND boolean_factor
			{
			  verifyAndOrOp( $1, AND_t, $3 );
			  $$ = $1;
				  char* tmp=(char*)calloc(1000,sizeof(char));
				int ChangeType=0;

				strcat(tmp,ExprStack[StackCount-2]);

				strcat(tmp,ExprStack[StackCount-1]);

				strcat(tmp,"iand\n");
				superfree(&ExprStack[--StackCount]);
				superfree(&ExprStack[--StackCount]);
				ExprStack[StackCount]=tmp;
				StackCount++;
			  
			}
			| boolean_factor { $$ = $1; }
			;

boolean_factor		: OP_NOT boolean_factor 
			{
			  verifyUnaryNOT( $2 );
			  $$ = $2;
			  strcat(ExprStack[StackCount-1],"ldc 0\n");
			  strcat(ExprStack[StackCount-1],"ixor\n");
			}
			| relop_expr { $$ = $1; }
			;

relop_expr		: expr rel_op expr
			{
			//fprintf(out,"[relop_expr %d %d]\n",$1->pType->type,$3->pType->type);
				char buf[10];
				int CT_1=0;
				int CT_2=0;
				if(($3->pType->type!=REAL_t))
				{
					CT_2=1;
				}
				if(($1->pType->type!=REAL_t))
				{
					CT_1=1;
				}

				//itoa(lCount++,buf,10);
			  verifyRelOp( $1, $2, $3 );
			  $$ = $1;
			  	
			 	char* tmp=(char*)calloc(1000,sizeof(char));
			 	//fprintf(out,"[!%d %d!]\n",$1->pType->type,$3->pType->type);
			  	strcat(tmp,ExprStack[StackCount-2]);
			  	if(CT_1)strcat(tmp,"i2f\n");
				strcat(tmp,ExprStack[StackCount-1]);
				if(CT_2)strcat(tmp,"i2f\n");
				strcat(tmp,"fcmpl \n"); 
				switch($2)
				{		
					case LT_t:strcat(tmp,"iflt");break;
					case LE_t:strcat(tmp,"ifle");break;
					case EQ_t:strcat(tmp,"ifeq");break;
					case GE_t:strcat(tmp,"ifge");break;
					case GT_t:strcat(tmp,"ifgt");break;
					case NE_t:strcat(tmp,"ifne");break;
				}
				lC[++lCtop]=lCount++;
				strcat(tmp," L1_");
				for(i=0;i<lC[lCtop];i++)strcat(tmp,"fuck");
				strcat(tmp,"\n iconst_0 \n goto L2_");
				for(i=0;i<lC[lCtop];i++)strcat(tmp,"fuck");
				strcat(tmp,"\n L1_");
				for(i=0;i<lC[lCtop];i++)strcat(tmp,"fuck");
				strcat(tmp,":\n iconst_1 \n L2_");
				for(i=0;i<lC[lCtop];i++)strcat(tmp,"fuck");
				strcat(tmp,":\n");
				
				lCtop--;
				superfree(&ExprStack[--StackCount]);
				superfree(&ExprStack[--StackCount]);
				ExprStack[StackCount]=tmp;
				StackCount++;
			  
			}
			| expr { $$ = $1; }
			;

rel_op			: OP_LT { $$ = LT_t; }
			| OP_LE { $$ = LE_t; }
			| OP_EQ { $$ = EQ_t; }
			| OP_GE { $$ = GE_t; }
			| OP_GT { $$ = GT_t; }
			| OP_NE { $$ = NE_t; }
			;

expr			: expr add_op term
			{
			  verifyArithmeticOp( $1, $2, $3 );
			  $$ = $1;
			  
			  	char* tmp=(char*)calloc(1000,sizeof(char));
				int ChangeType=0;
				if(($1->pType->type==REAL_t && $3->pType->type!=REAL_t))
				{
					ChangeType=2;
				}
				else if(($1->pType->type!=REAL_t && $3->pType->type==REAL_t))
				{
					ChangeType=1;
				}
				else if(($1->pType->type==REAL_t || $3->pType->type==REAL_t))
				{
					ChangeType=3;
				}
				strcat(tmp,ExprStack[StackCount-2]);
				//sprintf(tmp,"%s",ExprStack[StackCount-2]);
				if(ChangeType==1)strcat(tmp,"i2f\n");
				strcat(tmp,ExprStack[StackCount-1]);
				//sprintf(tmp,"%s",ExprStack[StackCount-1]);
				if(ChangeType==2)strcat(tmp,"i2f\n");
				if(ChangeType==0)
				{
					switch($2)
					{
						case ADD_t:
							strcat(tmp,"iadd\n");	break;
						case SUB_t:
							strcat(tmp,"isub\n");	break;
					}
				}
				else
				{
					switch($2)
					{
						case ADD_t:
							strcat(tmp,"fadd\n");	break;
						case SUB_t:
							strcat(tmp,"fsub\n");	break;
					}
				}
				superfree(&ExprStack[--StackCount]);
				superfree(&ExprStack[--StackCount]);
				ExprStack[StackCount]=tmp;
				StackCount++;
			}
			| term { $$ = $1; }
			;

add_op			: OP_ADD { $$ = ADD_t; }
			| OP_SUB { $$ = SUB_t; }
			;

term			: term mul_op factor
			{
				dumpEStack("term");
				char* tmp=(char*)calloc(1000,sizeof(char));
				//for(int i=0;i<1000;i++)tmp[i]=0;
				int ChangeType=0;
				if(($1->pType->type==REAL_t && $3->pType->type!=REAL_t))
				{
					ChangeType=2;
				}
				else if(($1->pType->type!=REAL_t && $3->pType->type==REAL_t))
				{
					ChangeType=1;
				}
				else if(($1->pType->type==REAL_t || $3->pType->type==REAL_t))
				{
					ChangeType=3;
				}
			//	if(StackCount<2)
				strcat(tmp,ExprStack[StackCount-2]);
				if(ChangeType==1)strcat(tmp,"i2f\n");
				strcat(tmp,ExprStack[StackCount-1]);
				if(ChangeType==2)strcat(tmp,"i2f\n");
				if(ChangeType==0)
				{
				 //fprintf(out,"[mul!!]\n");
					switch($2)
					{
						case MOD_t:
							strcat(tmp,"irem\n");	break;
						case DIV_t:
							strcat(tmp,"idiv\n");	break;
						case MUL_t:
							strcat(tmp,"imul\n");	break;
						
					}
				}
				else
				{
					switch($2)
					{
						case DIV_t:
							strcat(tmp,"fdiv\n");	break;
						case MUL_t:
							strcat(tmp,"fmul\n");	break;
					}
				
				}
				superfree(&ExprStack[--StackCount]);
				superfree(&ExprStack[--StackCount]);
				ExprStack[StackCount]=tmp;
				StackCount++;
				
			  if( $2 == MOD_t ) {
				verifyModOp( $1, $3 );
				
			  }
			  else {
				verifyArithmeticOp( $1, $2, $3 );
			  }
			  $$ = $1;
			}
			| factor { $$ = $1; }
			;

mul_op			: OP_MUL { $$ = MUL_t; }
			| OP_DIV { $$ = DIV_t; }
			| OP_MOD { $$ = MOD_t; }
			;

factor			: var_ref
			{
			  verifyExistence( symbolTable, $1, scope, __FALSE );
			  $$ = $1;
			  $$->beginningOp = NONE_t;
			 // fprintf(out,"[factor %d %s]\n",$$->pType->type,$$->varRef->id);
			  		struct expr_sem*v=$1;
					int shoot=-1;
					struct SymNode*glo=0;
					//fprintf(out,";[expr]\n");
					if(ExprStack[StackCount]==NULL)
					{
						ExprStack[StackCount]=(char*)calloc(1000,sizeof(char));
						//for(int i=0;i<1000;i++)tmp[i]=0;
					}
					if(glo=lookupSymbol(symbolTable, v->varRef->id, 0, __TRUE ),glo!=0)
					{
						//fprintf(out,"typ=%d  %s\n",glo->type->type,glo->name);fflush(out);
						//fprintf(out,"typ=%d  %s\n",glo->type->type,glo->name);fflush(out);
						if(glo->category==CONSTANT_t)
						{
						if(glo->type->type==INTEGER_t)
 						{
 							sprintf(ExprStack[StackCount],"ldc %d\n",glo->type->value.integerVal);
 							//strcat(ExprStack[StackCount],"ineg\n");
																	
 						}
 						else if(glo->type->type==BOOLEAN_t)
 						{
 							sprintf(ExprStack[StackCount],"ldc %d\n",glo->type->value.booleanVal);
							//strcat(ExprStack[StackCount],"ineg\n");									
 						}
 						else if(glo->type->type==REAL_t)
 						{
							sprintf(ExprStack[StackCount],"ldc %f\n",glo->type->value.realVal);
							//strcat(ExprStack[StackCount],"fneg\n");
 						}							
						}
						else
						{
						if(glo->type->type==INTEGER_t)
 						{
 							sprintf(ExprStack[StackCount],"getstatic %s/%s I\n",fileName,glo->name);
 							
																	
 						}
 						else if(glo->type->type==BOOLEAN_t)
 						{
 							sprintf(ExprStack[StackCount],"getstatic %s/%s I\n",fileName,glo->name);
																
 						}
 						else if(glo->type->type==REAL_t)
 						{
							sprintf(ExprStack[StackCount],"getstatic %s/%s F\n",fileName,glo->name);
 						}			
 						}					
 					}
 					else
 					{
 						for(i=LocalCount;i>=0;i--)
 						{
 								
 								if( LocalItem[i]!=0 &&v->varRef!=0 && v->varRef->id!=0 && ItemScope[i]<=scope && strcmp(v->varRef->id,LocalItem[i])==0)
 								{
 									shoot=i;
 									break;
 								}
 						}
 						
 						if(shoot!=-1)
 						{
 							if(v->pType->type==INTEGER_t)
 							{
 								sprintf(ExprStack[StackCount],"iload %d\n",shoot);
 							}	
 							else if(v->pType->type==BOOLEAN_t)
 							{
 								sprintf(ExprStack[StackCount],"iload %d\n",shoot);
																
 							}
 							else if(v->pType->type==REAL_t)
 							{
								sprintf(ExprStack[StackCount],"fload %d\n",shoot);
 																
 							}
 															
						}	
					}
					StackCount++;
					 dumpEStack("expr");
			}
			| OP_SUB var_ref
			{
			  if( verifyExistence( symbolTable, $2, scope, __FALSE ) == __TRUE )
				verifyUnaryMinus( $2 );
			  $$ = $2;
			  $$->beginningOp = SUB_t;
					if(ExprStack[StackCount]==NULL)ExprStack[StackCount]=(char*)calloc(1000,sizeof(char));
					//if(ExprStack[StackCount+1]==NULL)ExprStack[StackCount+1]=(char*)malloc(sizeof(char)*100);
			  		struct expr_sem*v=$2;
					int shoot=-1;
					struct SymNode*glo=0;
					if(glo=lookupSymbol(symbolTable, v->varRef->id, 0, __TRUE ),glo!=0)
					{
						//fprintf(out,"typ=%d  %s\n",glo->type->type,glo->name);fflush(out);
						if(glo->category==CONSTANT_t)
						{
						if(glo->type->type==INTEGER_t)
 						{
 							sprintf(ExprStack[StackCount],"ldc %d\n",glo->type->value.integerVal);
 							strcat(ExprStack[StackCount],"ineg\n");
																	
 						}
 						else if(glo->type->type==BOOLEAN_t)
 						{
 							sprintf(ExprStack[StackCount],"ldc %d\n",glo->type->value.booleanVal);
							strcat(ExprStack[StackCount],"ineg\n");									
 						}
 						else if(glo->type->type==REAL_t)
 						{
							sprintf(ExprStack[StackCount],"ldc %f\n",glo->type->value.realVal);
							strcat(ExprStack[StackCount],"fneg\n");
 						}							
						}
						else
						{
						if(glo->type->type==INTEGER_t)
 						{
 							sprintf(ExprStack[StackCount],"getstatic %s/%s I\n",fileName,glo->name);
 							strcat(ExprStack[StackCount],"ineg\n");
																	
 						}
 						else if(glo->type->type==BOOLEAN_t)
 						{
 							sprintf(ExprStack[StackCount],"getstatic %s/%s I\n",fileName,glo->name);
							strcat(ExprStack[StackCount],"ineg\n");									
 						}
 						else if(glo->type->type==REAL_t)
 						{
							sprintf(ExprStack[StackCount],"getstatic %s/%s F\n",fileName,glo->name);
							strcat(ExprStack[StackCount],"fneg\n");
 						}		
 						}						
 					}
 					else
 					{
 						for(i=LocalCount;i>=0;i--)
 						{
 								
 								if(LocalItem[i]!=0&&v->varRef!=0 && v->varRef->id!=0 && ItemScope[i]<=scope && strcmp(v->varRef->id,LocalItem[i])==0)
 								{
 									shoot=i;
 									break;
 								}
 						}
 						
 						if(shoot>=0)
 						{
 							if(v->pType->type==INTEGER_t)
 							{
 								sprintf(ExprStack[StackCount],"iload %d\n",shoot);
 								strcat(ExprStack[StackCount],"ineg\n");
 							}	
 							else if(v->pType->type==BOOLEAN_t)
 							{
 								sprintf(ExprStack[StackCount],"iload %d\n",shoot);
								strcat(ExprStack[StackCount],"ineg\n");								
 							}
 							else if(v->pType->type==REAL_t)
 							{
								sprintf(ExprStack[StackCount],"fload %d\n",shoot);
 								strcat(ExprStack[StackCount],"fneg\n");								
 							}
 															
						}	
					}	
					StackCount++;
					  
			}
			| MK_LPAREN boolean_expr MK_RPAREN 
			{
			  $2->beginningOp = NONE_t;
			  $$ = $2; 
			}
			| OP_SUB MK_LPAREN boolean_expr MK_RPAREN
			{
			  verifyUnaryMinus( $3 );
			  $$ = $3;
			  $$->beginningOp = SUB_t;
			  if($3->pType->type==REAL_t)
			  {
			  	strcat(ExprStack[StackCount-1],"fneg\n");
			  }
			  else
			  {
			  	strcat(ExprStack[StackCount-1],"ineg\n");
			  }
			}
			| ID MK_LPAREN opt_boolean_expr_list MK_RPAREN
			{
				struct expr_sem*ret;
			  $$ = verifyFuncInvoke( $1, $3, symbolTable, scope );
			  $$->beginningOp = NONE_t;
			  ret=$$;
			  //ExprStack[StackCount]=
			  char*tmp=(char*)calloc(1000,sizeof(char));
			  	int count=0;
				 struct expr_sem*ha;
				for(ha=$3;ha!=NULL;ha=ha->next)count++;
				
				for(i=count;i>0;i--)
				{
				strcat(tmp,ExprStack[StackCount-i]);
					superfree(&ExprStack[StackCount-i]);
				}
				
				strcat(tmp,"invokestatic ");
				strcat(tmp,fileName);
				strcat(tmp,"/");
				strcat(tmp,$1);
				strcat(tmp,"(");
				//fprintf(out,"invokestatic %s/%s(",fileName,$1);
				struct expr_sem*t;
				for(t=$3;t!=NULL;t=t->next)
				{
					switch(t->pType->type)
						{
							case INTEGER_t: strcat(tmp,"I"); break;
							case BOOLEAN_t: strcat(tmp,"Z"); break;
							case REAL_t:    strcat(tmp,"F"); break;
							//default:	fprintf(out,"?");
						}
				}
				switch(ret->pType->type)
				{
							case INTEGER_t: strcat(tmp,")I\n"); break;
							case BOOLEAN_t: strcat(tmp,")Z\n"); break;
							case REAL_t:    strcat(tmp,")F\n"); break;
							case VOID_t:    strcat(tmp,")V\n"); break;
				}
				StackCount-=count;
				ExprStack[StackCount]=tmp;
				StackCount++;
			}
			| OP_SUB ID MK_LPAREN opt_boolean_expr_list MK_RPAREN
			{
			  $$ = verifyFuncInvoke( $2, $4, symbolTable, scope );
			  $$->beginningOp = SUB_t;
			  			  
			}
			| literal_const
			{
			  $$ = (struct expr_sem *)malloc(sizeof(struct expr_sem));
			  $$->isDeref = __TRUE;
			  $$->varRef = 0;
			  if(ExprStack[StackCount]==NULL)ExprStack[StackCount]=(char*)calloc(1000,sizeof(char));
			if( ($1->category == INTEGER_t) || ($1->category == BOOLEAN_t) || ($1->category == REAL_t) )
			{	$$->pType = createPTypeWithValue($1->category, &($1->value));
			//fprintf(out,";[expr]\n");
			  	if(($1->category == INTEGER_t) )
			  		sprintf(ExprStack[StackCount],"ldc %d\n",$1->value);
				else if($1->category==BOOLEAN_t)
				{
					if($1->value.booleanVal ==__TRUE)
					{
						sprintf(ExprStack[StackCount],"iconst_1\n");
					}
					else
					{
						sprintf(ExprStack[StackCount],"iconst_0\n");
					}

				}
			  	else if(($1->category == REAL_t))
			  		sprintf(ExprStack[StackCount],"ldc %f\n",$1->value.realVal);

			  }
			  else if($1->category == STRING_t)
			  {
			  $$->pType = createPType( $1->category );
			$$->pType->strv=(char*)calloc(1000,sizeof(char));
			strcpy($$->pType->strv,$1->value.stringVal);
			sprintf(ExprStack[StackCount],"ldc \"%s\"\n",$$->pType->strv);
			  }
			  else 
			  	$$->pType = createPType( $1->category );
			  $$->next = 0;
			  
			  StackCount++;
			  dumpEStack("const");
			  if( $1->hasMinus == __TRUE ) {
			  	$$->beginningOp = SUB_t;
			  }
			  else {
				$$->beginningOp = NONE_t;
			  }
			}
			;

var_ref			: ID
			{
			  $$ = createExprSem( $1 );
			}
			| var_ref dim
			{
			  increaseDim( $1, $2 );
			  $$ = $1;
			}
			;

dim			: MK_LB boolean_expr MK_RB
			{
			  $$ = verifyArrayIndex( $2 );
			}
			;

%%

int yyerror( char *msg )
{
	fprintf( stderr, "\n|--------------------------------------------------------------------------\n" );
	fprintf( stderr, "| Error found in Line #%d: %s\n", linenum, buf );
	fprintf( stderr, "|\n" );
	fprintf( stderr, "| Unmatched token: %s\n", yytext );
	fprintf( stderr, "|--------------------------------------------------------------------------\n" );
	exit(-1);
}
int dumpEStack(const char* ha)
{
	fprintf(out," ");
	fflush(out);
	return 0;
	fprintf(out,";////////////////////%s/////////////////////%d\n",ha,StackCount);
	for(i=0;i<StackCount;i++)
	{
		
		fprintf(out,"       -------stack %d------\n",i);
		fprintf(out,"%s\n",ExprStack[i]);
		
	}
	fprintf(out,";//////////////////end_%s///////////////////////\n",ha);
	fflush(out);
}
void superfree(char** str)
{
	char *tmp=*str;
	free(tmp);
	*str=0;
}