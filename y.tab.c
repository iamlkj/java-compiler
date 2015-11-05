#include <stdlib.h>
#include <string.h>
#ifndef lint
#ifdef __unused
__unused
#endif
static char const 
yyrcsid[] = "$FreeBSD: src/usr.bin/yacc/skeleton.c,v 1.39.2.2.2.1 2010/12/21 17:09:25 kensmith Exp $";
#endif
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYLEX yylex()
#define YYEMPTY -1
#define yyclearin (yychar=(YYEMPTY))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING() (yyerrflag!=0)
#if defined(__cplusplus) || __STDC__
static int yygrowstack(void);
#else
static int yygrowstack();
#endif
#define YYPREFIX "yy"
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

/*#include "test.h"*/

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
struct SymTable *symbolTable;	/* main symbol table*/
int lCount=0;
int lC[20];
int lCtop=0;
__BOOLEAN paramError;			/* indicate is parameter have any error?*/

struct PType *funcReturn;		/* record function's return type, used at 'return statement' production rule*/
int dumpEStack(const char*);
void superfree(char**);
typedef union {
	int intVal;
	float realVal;
	/*__BOOLEAN booleanVal;*/
	char *lexeme;
	struct idNode_sem *id;
	/*SEMTYPE type;*/
	struct ConstAttr *constVal;
	struct PType *ptype;
	struct param_sem *par;
	struct expr_sem *exprs;
	/*struct var_ref_sem *varRef; */
	struct expr_sem_node *exprNode;
} YYSTYPE;
#define YYERRCODE 256
#define ARRAY 257
#define BEG 258
#define BOOLEAN 259
#define DEF 260
#define DO 261
#define ELSE 262
#define END 263
#define FALSE 264
#define FOR 265
#define INTEGER 266
#define IF 267
#define OF 268
#define PRINT 269
#define READ 270
#define REAL 271
#define RETURN 272
#define STRING 273
#define THEN 274
#define TO 275
#define TRUE 276
#define VAR 277
#define WHILE 278
#define OP_ADD 279
#define OP_SUB 280
#define OP_MUL 281
#define OP_DIV 282
#define OP_MOD 283
#define OP_ASSIGN 284
#define OP_EQ 285
#define OP_NE 286
#define OP_GT 287
#define OP_LT 288
#define OP_GE 289
#define OP_LE 290
#define OP_AND 291
#define OP_OR 292
#define OP_NOT 293
#define MK_COMMA 294
#define MK_COLON 295
#define MK_SEMICOLON 296
#define MK_LPAREN 297
#define MK_RPAREN 298
#define MK_LB 299
#define MK_RB 300
#define ID 301
#define INT_CONST 302
#define FLOAT_CONST 303
#define SCIENTIFIC 304
#define STR_CONST 305
const short yylhs[] = {                                        -1,
   26,    0,   31,   27,   28,   28,   32,   32,   33,   33,
   33,    2,    2,    2,    2,    2,    2,    2,    2,    2,
   29,   29,   34,   34,   36,   37,   35,    9,    9,    8,
    8,    7,    1,    1,    6,    6,    3,    3,    4,    4,
    4,    4,    5,   24,   24,   38,   38,   38,   38,   38,
   38,   38,   45,   30,   46,   46,   47,   47,   39,   39,
   39,   44,   49,   40,   40,   48,   51,   41,   50,   52,
   53,   42,   25,   43,   19,   19,   18,   18,   11,   11,
   12,   12,   13,   13,   14,   14,   23,   23,   23,   23,
   23,   23,   15,   15,   22,   22,   16,   16,   21,   21,
   21,   17,   17,   17,   17,   17,   17,   17,   10,   10,
   20,
};
const short yylen[] = {                                         2,
    0,    6,    0,    4,    1,    0,    2,    1,    5,    5,
    5,    1,    2,    1,    2,    1,    2,    1,    1,    1,
    1,    0,    2,    1,    0,    0,   11,    1,    0,    3,
    1,    3,    3,    1,    2,    0,    1,    1,    1,    1,
    1,    1,    6,    1,    2,    1,    1,    1,    1,    1,
    1,    1,    0,    5,    1,    0,    2,    1,    4,    3,
    3,    5,    0,    9,    6,    1,    0,    7,    1,    0,
    0,   12,    1,    3,    1,    0,    3,    1,    3,    1,
    3,    1,    2,    1,    3,    1,    1,    1,    1,    1,
    1,    1,    3,    1,    1,    1,    3,    1,    1,    1,
    1,    1,    2,    3,    4,    4,    5,    1,    1,    2,
    3,
};
const short yydefred[] = {                                      0,
    1,    0,    0,    0,    0,    0,    0,    0,    8,   34,
    0,    0,    0,    3,    0,   24,    7,    0,    0,    2,
    0,   53,   23,   33,    0,   41,   20,   39,   40,   42,
   19,    0,   12,   14,   16,   18,    0,    0,    0,    0,
   31,    0,   25,    4,    0,    0,   44,    0,   13,   15,
   17,   11,    9,   10,    0,    0,    0,    0,   45,    0,
   32,   37,   38,   30,    0,    0,    0,    0,   26,    0,
    0,    0,    0,    0,    0,    0,    0,   46,   58,   47,
   48,   49,   50,   51,   52,    0,    0,    0,   35,    0,
   70,    0,    0,    0,    0,  108,    0,    0,    0,   82,
   84,    0,    0,   98,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  110,   54,   57,   43,   53,    0,    0,
    0,    0,   83,    0,    0,    0,    0,   95,   96,   89,
   92,   91,   87,   90,   88,    0,    0,   99,  100,  101,
    0,    0,   60,   61,   74,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  104,    0,    0,   81,    0,
    0,   97,    0,   67,    0,    0,   59,  111,    0,   73,
    0,  105,    0,  106,   63,    0,    0,    0,   62,   27,
    0,  107,    0,   65,    0,   71,    0,   68,    0,    0,
    0,   64,    0,    0,   72,
};
const short yydgoto[] = {                                       2,
   40,   96,   61,   62,   63,   69,   41,   42,   43,   97,
  147,   99,  100,  101,  102,  103,  104,  148,  149,  114,
  141,  136,  137,   48,  171,    3,    6,    7,   14,   78,
   22,    8,    9,   15,   16,   57,   90,   79,   80,   81,
   82,   83,   84,   85,   45,   86,   87,  105,  183,  110,
  177,  119,  189,
};
const short yysindex[] = {                                   -287,
    0,    0, -279, -258, -273, -232, -259, -258,    0,    0,
 -155, -256, -203,    0, -259,    0,    0, -248, -244,    0,
 -273,    0,    0,    0, -264,    0,    0,    0,    0,    0,
    0, -202,    0,    0,    0,    0, -193, -177, -175, -142,
    0, -126,    0,    0, -129, -130,    0,  -97,    0,    0,
    0,    0,    0,    0,   39, -273, -105, -258,    0, -264,
    0,    0,    0,    0,  -96,   22,  -73, -236,    0,  -85,
 -169, -169, -169, -169, -169,  -79, -266,    0,    0,    0,
    0,    0,    0,    0,    0,  -62,   22,   39,    0,  -74,
    0, -189, -169, -169,  -68,    0,  -75,  -51,  -46,    0,
    0,   28, -134,    0,  -27, -230, -214, -207,  -51,   -8,
 -169, -169, -169,    0,    0,    0,    0,    0,  -34, -169,
  -29,  -75,    0, -211, -169, -169, -169,    0,    0,    0,
    0,    0,    0,    0,    0, -160, -160,    0,    0,    0,
 -160,   22,    0,    0,    0,   22,  -51,  -28,  -25, -166,
 -251,    8,  -23, -200, -169,    0,  -18,  -46,    0, -134,
 -125,    0,  -35,    0, -169,  -19,    0,    0,  -16,    0,
   27,    0,  -15,    0,    0,   36,   41,  -51,    0,    0,
  -23,    0,   22,    0,   45,    0,   46,    0,   50,   52,
   22,    0,   57,   60,    0,
};
const short yyrindex[] = {                                      0,
    0,    0,    0, -249,    0,    0,   64, -195,    0,    0,
    0,    0,    0,    0,   66,    0,    0,    0,    0,    0,
   29,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   30,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -179,    0,    0,
    0,    0,    0,    0,   33, -140,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -260,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, -208,    0,    0,    0,
    0,    0,    0,    0, -123,    0, -100,   51,  -22,    0,
    0, -227,  -54,    0,    0,    0,    0,    0,   65,    0,
   32,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -123,  -77,    0,    0,   32,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -153,    0,    0,    0, -140, -167,   34,    0,    0,
    0,    0,    0,    0,   32,    0,    0,    1,    0,  -31,
  -10,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -148,    0,    0,
    0,    0, -140,    0,    0,    0,    0,    0,    0,    0,
 -140,    0,    0,    0,    0,
};
const short yygindex[] = {                                      0,
  326,  314,  246,  -11,  316,    0,  280,    0,    0,  -66,
  -69,  211,  -81,    0,  201,  203,  199,    0, -115,    0,
    0,    0,    0,  281,  161,    0,    0,  285,    0,  -21,
    0,    0,  336,    0,  330,    0,    0,  259,    0,    0,
    0,    0,    0,    0,    0, -135,    0,    0,    0,    0,
    0,    0,    0,
};
#define YYTABLESIZE 346
const short yytable[] = {                                      77,
   44,   98,  106,  107,  108,  109,  163,   38,    6,  157,
  164,  123,   25,    1,   26,   46,    4,  112,    5,   27,
   77,   28,   26,  109,  124,  122,   29,   10,   30,   28,
   12,   31,  113,   86,   29,   32,   30,   47,  109,  173,
  126,   13,  150,  151,   20,  159,   86,  187,  168,   53,
  154,    6,   24,   55,   55,  193,   89,   33,   34,   35,
   36,  126,    5,   86,   86,  143,   86,    5,   86,    5,
   86,    5,   86,    5,    5,   77,    5,  126,    6,   77,
  126,  144,    5,    6,  126,    6,  156,    6,  145,    6,
    6,  126,    6,   21,   27,  178,  152,  172,    6,   49,
   50,   51,   52,   27,   53,    5,   31,  120,   56,   56,
   92,  121,   49,   50,   51,   31,   77,   53,   53,   92,
   54,    6,   56,   93,   77,  126,   78,   94,   58,  167,
   78,   95,   33,   34,   35,   36,   94,  109,   18,   19,
   95,   33,   34,   35,   36,   77,  138,  139,  140,   77,
  109,   18,   55,  128,  129,  109,  109,  109,  109,  109,
  102,  109,  109,  109,  109,  109,  109,  109,  109,   56,
  109,   59,  109,  102,  109,  109,  109,   60,  102,  102,
  102,  102,  102,  103,  102,  102,  102,  102,  102,  102,
  102,  102,   65,  102,   88,  102,  103,  102,   68,  102,
  115,  103,  103,  103,  103,  103,   94,  103,  103,  103,
  103,  103,  103,  103,  103,   91,  103,  111,  103,   94,
  103,  118,  103,  113,   94,   94,  175,  176,  125,   93,
   94,   94,   94,   94,   94,   94,   94,   94,   80,   94,
  126,   94,   93,   94,  127,   94,  142,   93,   93,  153,
   85,   80,  146,   93,   93,   93,   93,   93,   93,   93,
   93,   79,   93,   85,   93,  165,   93,  155,   93,   80,
  169,   80,  166,   80,   79,   80,  179,   80,  170,  174,
   85,   85,  182,   85,  180,   85,   70,   85,   71,   85,
   72,   73,   79,   74,   79,   25,   79,   26,   79,   75,
   79,  181,  184,  185,   28,  188,  128,  129,  190,   29,
  191,   30,  130,  131,  132,  133,  134,  135,  192,  194,
  195,   22,   76,   21,   66,   69,   29,   28,   36,   76,
   11,   75,   37,  117,   39,   64,  158,  161,  160,  162,
   67,  186,   66,   17,   23,  116,
};
const short yycheck[] = {                                      66,
   22,   71,   72,   73,   74,   75,  142,   19,  258,  125,
  146,   93,  257,  301,  259,  280,  296,  284,  277,  264,
   87,  266,  259,  284,   94,   92,  271,  301,  273,  266,
  263,  276,  299,  261,  271,  280,  273,  302,  299,  155,
  292,  301,  112,  113,  301,  127,  274,  183,  300,  258,
  120,  301,  301,  262,  263,  191,   68,  302,  303,  304,
  305,  292,  258,  291,  292,  296,  294,  263,  296,  265,
  298,  267,  300,  269,  270,  142,  272,  292,  258,  146,
  292,  296,  278,  263,  292,  265,  298,  267,  296,  269,
  270,  292,  272,  297,  264,  165,  118,  298,  278,  302,
  303,  304,  296,  264,  258,  301,  276,  297,  262,  263,
  280,  301,  302,  303,  304,  276,  183,  258,  296,  280,
  296,  301,  263,  293,  191,  292,  294,  297,  258,  296,
  298,  301,  302,  303,  304,  305,  297,  261,  294,  295,
  301,  302,  303,  304,  305,  294,  281,  282,  283,  298,
  274,  294,  295,  279,  280,  279,  280,  281,  282,  283,
  261,  285,  286,  287,  288,  289,  290,  291,  292,  296,
  294,  302,  296,  274,  298,  299,  300,  275,  279,  280,
  281,  282,  283,  261,  285,  286,  287,  288,  289,  290,
  291,  292,  298,  294,  268,  296,  274,  298,  295,  300,
  263,  279,  280,  281,  282,  283,  261,  285,  286,  287,
  288,  289,  290,  291,  292,  301,  294,  297,  296,  274,
  298,  296,  300,  299,  279,  280,  262,  263,  297,  261,
  285,  286,  287,  288,  289,  290,  291,  292,  261,  294,
  292,  296,  274,  298,  291,  300,  274,  279,  280,  284,
  261,  274,  261,  285,  286,  287,  288,  289,  290,  291,
  292,  261,  294,  274,  296,  294,  298,  297,  300,  292,
  263,  294,  298,  296,  274,  298,  296,  300,  302,  298,
  291,  292,  298,  294,  301,  296,  265,  298,  267,  300,
  269,  270,  292,  272,  294,  257,  296,  259,  298,  278,
  300,  275,  267,  263,  266,  261,  279,  280,  263,  271,
  261,  273,  285,  286,  287,  288,  289,  290,  267,  263,
  261,  258,  301,  258,  274,  261,  298,  298,  296,  298,
    5,  298,   19,   88,   19,   56,  126,  137,  136,  141,
   60,  181,   58,    8,   15,   87,
};
#define YYFINAL 2
#ifndef YYDEBUG
#define YYDEBUG 1
#endif
#define YYMAXTOKEN 305
#if YYDEBUG
const char * const yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"ARRAY","BEG","BOOLEAN","DEF",
"DO","ELSE","END","FALSE","FOR","INTEGER","IF","OF","PRINT","READ","REAL",
"RETURN","STRING","THEN","TO","TRUE","VAR","WHILE","OP_ADD","OP_SUB","OP_MUL",
"OP_DIV","OP_MOD","OP_ASSIGN","OP_EQ","OP_NE","OP_GT","OP_LT","OP_GE","OP_LE",
"OP_AND","OP_OR","OP_NOT","MK_COMMA","MK_COLON","MK_SEMICOLON","MK_LPAREN",
"MK_RPAREN","MK_LB","MK_RB","ID","INT_CONST","FLOAT_CONST","SCIENTIFIC",
"STR_CONST",
};
const char * const yyrule[] = {
"$accept : program",
"$$1 :",
"program : ID $$1 MK_SEMICOLON program_body END ID",
"$$2 :",
"program_body : opt_decl_list opt_func_decl_list $$2 compound_stmt",
"opt_decl_list : decl_list",
"opt_decl_list :",
"decl_list : decl_list decl",
"decl_list : decl",
"decl : VAR id_list MK_COLON scalar_type MK_SEMICOLON",
"decl : VAR id_list MK_COLON array_type MK_SEMICOLON",
"decl : VAR id_list MK_COLON literal_const MK_SEMICOLON",
"literal_const : INT_CONST",
"literal_const : OP_SUB INT_CONST",
"literal_const : FLOAT_CONST",
"literal_const : OP_SUB FLOAT_CONST",
"literal_const : SCIENTIFIC",
"literal_const : OP_SUB SCIENTIFIC",
"literal_const : STR_CONST",
"literal_const : TRUE",
"literal_const : FALSE",
"opt_func_decl_list : func_decl_list",
"opt_func_decl_list :",
"func_decl_list : func_decl_list func_decl",
"func_decl_list : func_decl",
"$$3 :",
"$$4 :",
"func_decl : ID MK_LPAREN opt_param_list $$3 MK_RPAREN ret_type $$4 MK_SEMICOLON compound_stmt END ID",
"opt_param_list : param_list",
"opt_param_list :",
"param_list : param_list MK_SEMICOLON param",
"param_list : param",
"param : id_list MK_COLON type",
"id_list : id_list MK_COMMA ID",
"id_list : ID",
"ret_type : MK_COLON scalar_type",
"ret_type :",
"type : scalar_type",
"type : array_type",
"scalar_type : INTEGER",
"scalar_type : REAL",
"scalar_type : BOOLEAN",
"scalar_type : STRING",
"array_type : ARRAY array_index TO array_index OF type",
"array_index : INT_CONST",
"array_index : OP_SUB INT_CONST",
"stmt : compound_stmt",
"stmt : simple_stmt",
"stmt : cond_stmt",
"stmt : while_stmt",
"stmt : for_stmt",
"stmt : return_stmt",
"stmt : proc_call_stmt",
"$$5 :",
"compound_stmt : $$5 BEG opt_decl_list opt_stmt_list END",
"opt_stmt_list : stmt_list",
"opt_stmt_list :",
"stmt_list : stmt_list stmt",
"stmt_list : stmt",
"simple_stmt : var_ref OP_ASSIGN boolean_expr MK_SEMICOLON",
"simple_stmt : PRINT boolean_expr MK_SEMICOLON",
"simple_stmt : READ boolean_expr MK_SEMICOLON",
"proc_call_stmt : ID MK_LPAREN opt_boolean_expr_list MK_RPAREN MK_SEMICOLON",
"$$6 :",
"cond_stmt : IF condition THEN opt_stmt_list ELSE $$6 opt_stmt_list END IF",
"cond_stmt : IF condition THEN opt_stmt_list END IF",
"condition : boolean_expr",
"$$7 :",
"while_stmt : WHILE condition_while DO opt_stmt_list $$7 END DO",
"condition_while : boolean_expr",
"$$8 :",
"$$9 :",
"for_stmt : FOR ID $$8 OP_ASSIGN loop_param TO loop_param $$9 DO opt_stmt_list END DO",
"loop_param : INT_CONST",
"return_stmt : RETURN boolean_expr MK_SEMICOLON",
"opt_boolean_expr_list : boolean_expr_list",
"opt_boolean_expr_list :",
"boolean_expr_list : boolean_expr_list MK_COMMA boolean_expr",
"boolean_expr_list : boolean_expr",
"boolean_expr : boolean_expr OP_OR boolean_term",
"boolean_expr : boolean_term",
"boolean_term : boolean_term OP_AND boolean_factor",
"boolean_term : boolean_factor",
"boolean_factor : OP_NOT boolean_factor",
"boolean_factor : relop_expr",
"relop_expr : expr rel_op expr",
"relop_expr : expr",
"rel_op : OP_LT",
"rel_op : OP_LE",
"rel_op : OP_EQ",
"rel_op : OP_GE",
"rel_op : OP_GT",
"rel_op : OP_NE",
"expr : expr add_op term",
"expr : term",
"add_op : OP_ADD",
"add_op : OP_SUB",
"term : term mul_op factor",
"term : factor",
"mul_op : OP_MUL",
"mul_op : OP_DIV",
"mul_op : OP_MOD",
"factor : var_ref",
"factor : OP_SUB var_ref",
"factor : MK_LPAREN boolean_expr MK_RPAREN",
"factor : OP_SUB MK_LPAREN boolean_expr MK_RPAREN",
"factor : ID MK_LPAREN opt_boolean_expr_list MK_RPAREN",
"factor : OP_SUB ID MK_LPAREN opt_boolean_expr_list MK_RPAREN",
"factor : literal_const",
"var_ref : ID",
"var_ref : var_ref dim",
"dim : MK_LB boolean_expr MK_RB",
};
#endif
#if YYDEBUG
#include <stdio.h>
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH 10000
#endif
#endif
#define YYINITSTACKSIZE 200
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short *yyss;
short *yysslim;
YYSTYPE *yyvs;
int yystacksize;

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
/* allocate initial stack or double stack size, up to YYMAXDEPTH */
#if defined(__cplusplus) || __STDC__
static int yygrowstack(void)
#else
static int yygrowstack()
#endif
{
    int newsize, i;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = yystacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;
    i = yyssp - yyss;
    newss = yyss ? (short *)realloc(yyss, newsize * sizeof *newss) :
      (short *)malloc(newsize * sizeof *newss);
    if (newss == NULL)
        return -1;
    yyss = newss;
    yyssp = newss + i;
    newvs = yyvs ? (YYSTYPE *)realloc(yyvs, newsize * sizeof *newvs) :
      (YYSTYPE *)malloc(newsize * sizeof *newvs);
    if (newvs == NULL)
        return -1;
    yyvs = newvs;
    yyvsp = newvs + i;
    yystacksize = newsize;
    yysslim = yyss + newsize - 1;
    return 0;
}

#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab

#ifndef YYPARSE_PARAM
#if defined(__cplusplus) || __STDC__
#define YYPARSE_PARAM_ARG void
#define YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif	/* ANSI-C/C++ */
#else	/* YYPARSE_PARAM */
#ifndef YYPARSE_PARAM_TYPE
#define YYPARSE_PARAM_TYPE void *
#endif
#if defined(__cplusplus) || __STDC__
#define YYPARSE_PARAM_ARG YYPARSE_PARAM_TYPE YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL YYPARSE_PARAM_TYPE YYPARSE_PARAM;
#endif	/* ANSI-C/C++ */
#endif	/* ! YYPARSE_PARAM */

int
yyparse (YYPARSE_PARAM_ARG)
    YYPARSE_PARAM_DECL
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    if (yyss == NULL && yygrowstack()) goto yyoverflow;
    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if ((yyn = yydefred[yystate])) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yysslim && yygrowstack())
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#if defined(lint) || defined(__GNUC__)
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#if defined(lint) || defined(__GNUC__)
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yysslim && yygrowstack())
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yyvsp[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 1:
{
				
				out=fopen("haha.j","w");
		/*	if(ExprStack[StackCount]==NULL)ExprStack[StackCount]=(char*)malloc(sizeof(char)*10000);*/
			  struct PType *pType = createPType( VOID_t );
			  struct SymNode *newNode = createProgramNode( yyvsp[0].lexeme, scope, pType );
			  insertTab( symbolTable, newNode );
			  
			  if( strcmp(fileName,yyvsp[0].lexeme) ) {
				fprintf( stdout, "<Error> found in Line %d: program beginning ID inconsist with file name  \n", linenum );
			  }
			  fprintf(out,"; %s.j \n",yyvsp[0].lexeme);
			  fprintf(out,".class public %s \n",yyvsp[0].lexeme);
			  fprintf(out,".super java/lang/Object\n");
			  fprintf(out,".field public static _sc Ljava/util/Scanner;\n");

			}
break;
case 2:
{
			  if( strcmp(yyvsp[-5].lexeme, yyvsp[0].lexeme) ) { fprintf( stdout, "<Error> found in Line %d: %s", linenum,"Program end ID inconsist with the beginning ID  \n"); }
			  if( strcmp(fileName,yyvsp[0].lexeme) ) {
				 fprintf( stdout, "<Error> found in Line %d: program end ID inconsist with file name  \n", linenum );
			  }
			  /* dump symbol table*/
			  if( Opt_D == 1 )
				printSymTable( symbolTable, scope );
				fprintf(out,"return \n\
.end method");
			}
break;
case 3:
{
fprintf(out,".method public static main([Ljava/lang/String;)V\n\
.limit stack 100\n\
.limit locals 100 \n\
new java/util/Scanner \n\
dup \n\
getstatic java/lang/System/in Ljava/io/InputStream; \n\
invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V \n\
putstatic %s/_sc Ljava/util/Scanner; \n ",fileName);
								
								}
break;
case 9:
{
			  /* insert into symbol table*/
			  struct idNode_sem *ptr;
			  struct SymNode *newNode;
			  for( ptr=yyvsp[-3].id ; ptr!=0 ; ptr=(ptr->next) ) {
			  	if( verifyRedeclaration( symbolTable, ptr->value, scope ) ==__FALSE ) { }
				else {
					newNode = createVarNode( ptr->value, scope, yyvsp[-1].ptype );
					insertTab( symbolTable, newNode );
					/*fprintf(out,"[%d haha]\n",scope);*/
					if(scope==0)
					{
							if(yyvsp[-1].ptype->type==INTEGER_t)
										{fprintf(out,".field public static %s I\n",ptr->value);}
						else if(BOOLEAN_t==yyvsp[-1].ptype->type)
										{fprintf(out,".field public static %s Z\n",ptr->value);}
						else if(REAL_t==yyvsp[-1].ptype->type)
										{fprintf(out,".field public static %s F\n",ptr->value);}
					}
					else
					{
					/*	fprintf(stdout,"[local %s]",ptr->value);*/
						LocalItem[++LocalCount]=(char*)calloc(40,sizeof(char));
						/*for(int i=0;i<1000;i++)tmp[i]=0;*/
						ItemScope[LocalCount]=scope;
						ItemType[LocalCount]=yyvsp[-1].ptype->type;
						strcpy(LocalItem[LocalCount],ptr->value);
						/*fprintf(stdout,"[%d\t%s]\n",LocalCount,LocalItem[LocalCount-1]);*/
					}
				}
			  }
			  deleteIdList( yyvsp[-3].id );
			}
break;
case 10:
{
			  verifyArrayType( yyvsp[-3].id, yyvsp[-1].ptype );
			  /* insert into symbol table*/
			  struct idNode_sem *ptr;
			  struct SymNode *newNode;
			  for( ptr=yyvsp[-3].id ; ptr!=0 ; ptr=(ptr->next) ) {
			  	if( yyvsp[-1].ptype->isError == __TRUE ) { }
				else if( verifyRedeclaration( symbolTable, ptr->value, scope ) ==__FALSE ) { }
				else {
					newNode = createVarNode( ptr->value, scope, yyvsp[-1].ptype );
					insertTab( symbolTable, newNode );
				}
			  }
			  deleteIdList( yyvsp[-3].id );
			}
break;
case 11:
{
			  struct PType *pType = createPType( yyvsp[-1].constVal->category );
			  /* insert constants into symbol table*/
			  struct idNode_sem *ptr;
			  struct SymNode *newNode;
			  for( ptr=yyvsp[-3].id ; ptr!=0 ; ptr=(ptr->next) ) {
			  	if( verifyRedeclaration( symbolTable, ptr->value, scope ) ==__FALSE ) { }
				else {
					newNode = createConstNode( ptr->value, scope, pType, yyvsp[-1].constVal );
					insertTab( symbolTable, newNode );
				}
			  }
			  
			  deleteIdList( yyvsp[-3].id );
			}
break;
case 12:
{
			  int tmp = yyvsp[0].intVal;
			  yyval.constVal = createConstAttr( INTEGER_t, &tmp );
			}
break;
case 13:
{
			  int tmp = -yyvsp[0].intVal;
			  yyval.constVal = createConstAttr( INTEGER_t, &tmp );
			}
break;
case 14:
{
			  float tmp = yyvsp[0].realVal;
			  yyval.constVal = createConstAttr( REAL_t, &tmp );
			}
break;
case 15:
{
			  float tmp = -yyvsp[0].realVal;
			  yyval.constVal = createConstAttr( REAL_t, &tmp );
			}
break;
case 16:
{
			  float tmp = yyvsp[0].realVal;
			  yyval.constVal = createConstAttr( REAL_t, &tmp );
			}
break;
case 17:
{
			  float tmp = -yyvsp[0].realVal;
			  yyval.constVal = createConstAttr( REAL_t, &tmp );
			}
break;
case 18:
{
			  yyval.constVal = createConstAttr( STRING_t, yyvsp[0].lexeme );
			}
break;
case 19:
{
			  SEMTYPE tmp = __TRUE;
			  yyval.constVal = createConstAttr( BOOLEAN_t, &tmp );
			}
break;
case 20:
{
			  SEMTYPE tmp = __FALSE;
			  yyval.constVal = createConstAttr( BOOLEAN_t, &tmp );
			}
break;
case 25:
{
			  /* check and insert parameters into symbol table*/
			  paramError = insertParamIntoSymTable( symbolTable, yyvsp[0].par, scope+1 );
			  /*fprintf(out,"");*/
			  								/*struct idNode_sem*ptr;
											for(ptr=$1;ptr->next!=NULL;ptr=ptr->next)
											{
												LocalItem[++LocalCount]=(char*)calloc(40,sizeof(char));
												ItemScope[LocalCount]=scope;
											}*/
			}
break;
case 26:
{
			  /* check and insert function into symbol table*/
			  if( paramError == __TRUE ) {
			  	printf("--- param(s) with several fault!! ---\n");
			  } else {
				insertFuncIntoSymTable( symbolTable, yyvsp[-5].lexeme, yyvsp[-3].par, yyvsp[0].ptype, scope );
			  }
			  funcReturn = yyvsp[0].ptype;
				fprintf(out,".method public static %s(",yyvsp[-5].lexeme);
				char*ptmp=calloc(100,sizeof(char));
				/*strcat(ptmp);*/
				struct param_sem* list;
				/*fprintf(out,"[: )]\n");fflush(out);*/
				for(list=yyvsp[-3].par;list!=NULL;list=list->next)
				{
				/*fprintf(out,"[! %d]",list->pType->type);fflush(out);*/
					struct idNode_sem*ptr;
					for(ptr=list->idlist;ptr!=NULL;ptr=ptr->next)
					{
						if(LocalCount==0)LocalCount=-1;
						LocalItem[++LocalCount]=(char*)calloc(40,sizeof(char));
						ItemScope[LocalCount]=scope;
						strcpy(LocalItem[LocalCount],ptr->value);
						/*fprintf(out,"[inhere %d]\n",list->pType->type);fflush(out);*/
						switch(list->pType->type)
						{
							case INTEGER_t: fprintf(out,"I"); break;
							case BOOLEAN_t: fprintf(out,"Z"); break;
							case REAL_t:    fprintf(out,"F"); break;
							/*default:	fprintf(out,"?");*/
						}
					}		
				}
				fprintf(out,")");
				char rettp;
				switch(yyvsp[0].ptype->type)
				{
							case INTEGER_t: fprintf(out,"I\n"); break;
							case BOOLEAN_t: fprintf(out,"Z\n"); break;
							case REAL_t:    fprintf(out,"F\n"); break;
							case VOID_t:	fprintf(out,"V\n"); break;				
				
				}
			  
			   fprintf(out,".limit stack 100\n");
			   fprintf(out,".limit locals 100\n"); 
 
			}
break;
case 27:
{
			  if( strcmp(yyvsp[-10].lexeme,yyvsp[0].lexeme) ) {
				fprintf( stdout, "<Error> found in Line %d: the end of the functionName mismatch  \n", linenum );
			  }
			  funcReturn = 0;
			  	switch(yyvsp[-5].ptype->type)
				{
							case INTEGER_t: fprintf(out,"ireturn \n"); break;
							case BOOLEAN_t: fprintf(out,"ireturn \n"); break;
							case REAL_t:    fprintf(out,"freturn \n"); break;
							case VOID_t:	fprintf(out,"return \n"); break;				
				
				}
			  fprintf(out,".end method \n");
			}
break;
case 28:
{ yyval.par = yyvsp[0].par; }
break;
case 29:
{ yyval.par = 0; }
break;
case 30:
{
			  param_sem_addParam( yyvsp[-2].par, yyvsp[0].par );
			  yyval.par = yyvsp[-2].par;
			}
break;
case 31:
{ yyval.par = yyvsp[0].par; }
break;
case 32:
{ yyval.par = createParam( yyvsp[-2].id, yyvsp[0].ptype ); 
											
											
											
										}
break;
case 33:
{
			  idlist_addNode( yyvsp[-2].id, yyvsp[0].lexeme );
			  yyval.id = yyvsp[-2].id;
			}
break;
case 34:
{ yyval.id = createIdList(yyvsp[0].lexeme); }
break;
case 35:
{ yyval.ptype = yyvsp[0].ptype; }
break;
case 36:
{ yyval.ptype = createPType( VOID_t ); }
break;
case 37:
{ yyval.ptype = yyvsp[0].ptype; }
break;
case 38:
{ yyval.ptype = yyvsp[0].ptype; }
break;
case 39:
{ yyval.ptype = createPType( INTEGER_t ); }
break;
case 40:
{ yyval.ptype = createPType( REAL_t ); }
break;
case 41:
{ yyval.ptype = createPType( BOOLEAN_t ); }
break;
case 42:
{ yyval.ptype = createPType( STRING_t ); }
break;
case 43:
{
				verifyArrayDim( yyvsp[0].ptype, yyvsp[-4].intVal, yyvsp[-2].intVal );
				increaseArrayDim( yyvsp[0].ptype, yyvsp[-4].intVal, yyvsp[-2].intVal );
				yyval.ptype = yyvsp[0].ptype;
			}
break;
case 44:
{ yyval.intVal = yyvsp[0].intVal; }
break;
case 45:
{ yyval.intVal = -yyvsp[0].intVal; }
break;
case 53:
{ 
			  scope++;
			}
break;
case 54:
{ 
			  /* print contents of current scope*/
			  if( Opt_D == 1 )
			  	printSymTable( symbolTable, scope );
			  deleteScope( symbolTable, scope );	/* leave this scope, delete...*/
			  scope--; 
			}
break;
case 59:
{
			  /* check if LHS exists*/
			  __BOOLEAN flagLHS = verifyExistence( symbolTable, yyvsp[-3].exprs, scope, __TRUE );
			  /* id RHS is not dereferenced, check and deference*/
			  __BOOLEAN flagRHS = __TRUE;
			  if( yyvsp[-1].exprs->isDeref == __FALSE ) {
				/*flagRHS = verifyExistence( symbolTable, $3, scope, __FALSE );*/
				flagRHS = verifyAndDerefenced( symbolTable, yyvsp[-1].exprs, scope, __FALSE );	
			  }
			  /* if both LHS and RHS are exists, verify their type*/
			  if( flagLHS==__TRUE && flagRHS==__TRUE )
				verifyAssignmentTypeMatch( yyvsp[-3].exprs, yyvsp[-1].exprs );
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
			  
			  
			  
					struct expr_sem*v=yyvsp[-3].exprs;
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
 															/*fprintf(out,";[finish]\n");fflush(out);*/
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
 							/*fprintf(stdout,"glo=%d\n",glo->category);					*/
 						}
					}										
				/*	superfree(&ExprStack[--StackCount]);*/
			}
break;
case 60:
{ verifyScalarExpr( yyvsp[-1].exprs, "print" ); 
												/*struct expr_sem *expr=$2;*/
												/*fprintf(out,";[vref= %s]",)*/
												dumpEStack("print");
												fprintf(out,"getstatic java/lang/System/out Ljava/io/PrintStream; \n");
												fprintf(out,"%s",ExprStack[--StackCount]);
												superfree(&ExprStack[StackCount]);
												switch(yyvsp[-1].exprs->pType->type)
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
break;
case 61:
{ verifyScalarExpr( yyvsp[-1].exprs, "read" ); 
 												dumpEStack("read");
 												int shoot=-1;
 												for(i=LocalCount;i>=0;i--)
 												{
 													if(LocalItem[i]!=0 && ItemScope[i]<=scope && strcmp(yyvsp[-1].exprs->varRef->id,LocalItem[i])==0)
 													{
 														shoot=i;
 														break;
 													}
 												
 												}
 													fprintf(out,"; invoke java.util.Scanner.nextXXX() \n");
													fprintf(out,"getstatic %s/_sc Ljava/util/Scanner; \n",fileName);
													/*fprintf(out,"beforepass! %d\n",StackCount);fflush(out);*/
													superfree(&ExprStack[--StackCount]);
	
													/*fprintf(out,"pass! %d\n",StackCount);fflush(out);*/
 												if(shoot==-1)
 												{
 													struct SymNode*glo;
													if(glo=lookupSymbol(symbolTable, yyvsp[-1].exprs->varRef->id, 0, __TRUE ),glo!=0)
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
 														/*fprintf(stdout,"glo=%d\n",glo->category);					*/
 													}
 												}
 												else
 												{

 													if(yyvsp[-1].exprs->pType->type==INTEGER_t)
 													{
 														fprintf(out,"invokevirtual java/util/Scanner/nextInt()I\n");
 														fprintf(out,"istore %d\n",shoot);
 													}
 													else if(yyvsp[-1].exprs->pType->type==BOOLEAN_t)
 													{
 														fprintf(out,"invokevirtual java/util/Scanner/nextBoolean()Z\n");
														fprintf(out,"istore %d\n",shoot);
 													}
 													else if(yyvsp[-1].exprs->pType->type==REAL_t)
 													{
 														fprintf(out,"invokevirtual java/util/Scanner/nextFloat()F \n");
 														fprintf(out,"fstore %d\n",shoot);
 													}
 													
 												}
 												
 												dumpEStack("endread");
 												}
break;
case 62:
{
			struct expr_sem*ret;
			  ret=verifyFuncInvoke( yyvsp[-4].lexeme, yyvsp[-2].exprs, symbolTable, scope );

				int count=0;
				 struct expr_sem*ha;
				for(ha=yyvsp[-2].exprs;ha!=NULL;ha=ha->next)count++;
				
				for(i=count;i>0;i--)
				{
					fprintf(out,"%s",ExprStack[StackCount-i]);
					superfree(&ExprStack[StackCount-i]);
				}
				StackCount-=count;
				
				fprintf(out,"invokestatic %s/%s(",fileName,yyvsp[-4].lexeme);
				struct expr_sem*t;
				for(t=yyvsp[-2].exprs;t!=NULL;t=t->next)
				{
					switch(t->pType->type)
						{
							case INTEGER_t: fprintf(out,"I"); break;
							case BOOLEAN_t: fprintf(out,"Z"); break;
							case REAL_t:    fprintf(out,"F"); break;
							/*default:	fprintf(out,"?");*/
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
break;
case 63:
{
			  	fprintf(out,"goto Lexit_%d\n",lC[lCtop]);
			  	fprintf(out,"Lfalse_%d:\n",lC[lCtop]);
			  }
break;
case 64:
{
			  		fprintf(out,"Lexit_%d:\n",lC[lCtop]);
			  		lCtop--;
			  }
break;
case 65:
{								  		
		  		fprintf(out,"Lfalse_%d:\n",lC[lCtop]);
			  			lCtop--;	  		
			}
break;
case 66:
{ verifyBooleanExpr( yyvsp[0].exprs, "if" ); 
								lC[++lCtop]=lCount++;
								fprintf(out,"%s",ExprStack[--StackCount]);
								fprintf(out,"ifeq Lfalse_%d\n",lC[lCtop]);								
								superfree(&ExprStack[StackCount]);
								}
break;
case 67:
{
			  		fprintf(out,"goto Lbegin_%d \n Lexit_%d:\n",lC[lCtop],lC[lCtop]);
			  		lCtop--;
			  		}
break;
case 69:
{ verifyBooleanExpr( yyvsp[0].exprs, "while" ); 
									lC[++lCtop]=lCount++;
									fprintf(out,"Lbegin_%d:\n",lC[lCtop]);  
									fprintf(out,"%s",ExprStack[--StackCount]);
									superfree(&ExprStack[StackCount]);
									fprintf(out,"ifeq Lexit_%d\n",lC[lCtop]);
									}
break;
case 70:
{ 
			  insertLoopVarIntoTable( symbolTable, yyvsp[0].lexeme );
			  LocalItem[++LocalCount]=(char*)calloc(40,sizeof(char));
			  ItemScope[LocalCount]=scope;
			  strcpy(LocalItem[LocalCount],yyvsp[0].lexeme);
			}
break;
case 71:
{
			  verifyLoopParam( yyvsp[-2].intVal, yyvsp[0].intVal );
			  lC[++lCtop]=lCount++;
			  fpritf(out,"ldc %d\n",yyvsp[-2].intVal);
			  int shoot=-1;
			   			for(i=LocalCount;i>=0;i--)
 						{
 								
 								if( LocalItem[i]!=0 &&ItemScope[i]==scope && strcmp(yyvsp[-5].lexeme,LocalItem[i])==0)
 								{
 									shoot=i;
 									break;
 								}
 						}
 						
 						if(shoot!=-1)
 						{

 								sprintf(ExprStack[StackCount],"istore %d\n",shoot);														
						}
				/*lC[lCtop]*/
				fprintf(out,"Lbegin_%d",lC[lCtop]);
						if(shoot!=-1)
 						{

 								sprintf(ExprStack[StackCount],"iload %d\n",shoot); 															
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
				fprintf(out,"ldc %d",yyvsp[0].intVal);
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
break;
case 72:
{
			  popLoopVar( symbolTable );
			  			int shoot=-1;
			  			  for(i=LocalCount;i>=0;i--)
 						{
 								
 								if( LocalItem[i]!=0 ItemScope[i]==scope && strcmp(yyvsp[-10].lexeme,LocalItem[i])==0)
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
break;
case 73:
{ yyval.intVal = yyvsp[0].intVal; }
break;
case 74:
{
			  verifyReturnStatement( yyvsp[-1].exprs, funcReturn );
			  fprintf(out,"%s",ExprStack[--StackCount]);
			  superfree(&ExprStack[StackCount]);
	/*		  switch($2->pType->type)
			  {
			  
			  	case INTEGER_t:
			  		fprintf(out,"ireturn");
			  		break;
			  	case BOOLEAN_t:
			  		fprintf(out,"ireturn");
			  		break;
			  	case REAL_t:
			  		fprintf(out,"freturn");
			  		break;
			  }*/
			}
break;
case 75:
{ yyval.exprs = yyvsp[0].exprs; }
break;
case 76:
{ yyval.exprs = 0; }
break;
case 77:
{
			  struct expr_sem *exprPtr;
			  for( exprPtr=yyvsp[-2].exprs ; (exprPtr->next)!=0 ; exprPtr=(exprPtr->next) );
			  exprPtr->next = yyvsp[0].exprs;
			  yyval.exprs = yyvsp[-2].exprs;
			}
break;
case 78:
{
			  yyval.exprs = yyvsp[0].exprs;
			}
break;
case 79:
{
			  verifyAndOrOp( yyvsp[-2].exprs, OR_t, yyvsp[0].exprs );
			  yyval.exprs = yyvsp[-2].exprs;
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
break;
case 80:
{ yyval.exprs = yyvsp[0].exprs; }
break;
case 81:
{
			  verifyAndOrOp( yyvsp[-2].exprs, AND_t, yyvsp[0].exprs );
			  yyval.exprs = yyvsp[-2].exprs;
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
break;
case 82:
{ yyval.exprs = yyvsp[0].exprs; }
break;
case 83:
{
			  verifyUnaryNOT( yyvsp[0].exprs );
			  yyval.exprs = yyvsp[0].exprs;
			  strcat(ExprStack[StackCount-1],"ldc 0\n");
			  strcat(ExprStack[StackCount-1],"ixor\n");
			}
break;
case 84:
{ yyval.exprs = yyvsp[0].exprs; }
break;
case 85:
{
			/*fprintf(out,"[relop_expr %d %d]\n",$1->pType->type,$3->pType->type);*/
				char buf[10];
				int CT_1=0;
				int CT_2=0;
				if((yyvsp[0].exprs->pType->type!=REAL_t))
				{
					CT_2=1;
				}
				if((yyvsp[-2].exprs->pType->type!=REAL_t))
				{
					CT_1=1;
				}

				/*itoa(lCount++,buf,10);*/
			  verifyRelOp( yyvsp[-2].exprs, yyvsp[-1].intVal, yyvsp[0].exprs );
			  yyval.exprs = yyvsp[-2].exprs;
			  	
			 	char* tmp=(char*)calloc(1000,sizeof(char));
			 	/*fprintf(out,"[!%d %d!]\n",$1->pType->type,$3->pType->type);*/
			  	strcat(tmp,ExprStack[StackCount-2]);
			  	if(CT_1)strcat(tmp,"i2f\n");
				strcat(tmp,ExprStack[StackCount-1]);
				if(CT_2)strcat(tmp,"i2f\n");
				strcat(tmp,"fcmpl \n"); 
				switch(yyvsp[-1].intVal)
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
				strcat(tmp,"\n iconst_0 ;\n goto L2_");
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
break;
case 86:
{ yyval.exprs = yyvsp[0].exprs; }
break;
case 87:
{ yyval.intVal = LT_t; }
break;
case 88:
{ yyval.intVal = LE_t; }
break;
case 89:
{ yyval.intVal = EQ_t; }
break;
case 90:
{ yyval.intVal = GE_t; }
break;
case 91:
{ yyval.intVal = GT_t; }
break;
case 92:
{ yyval.intVal = NE_t; }
break;
case 93:
{
			  verifyArithmeticOp( yyvsp[-2].exprs, yyvsp[-1].intVal, yyvsp[0].exprs );
			  yyval.exprs = yyvsp[-2].exprs;
			  
			  	char* tmp=(char*)calloc(1000,sizeof(char));
				int ChangeType=0;
				if((yyvsp[-2].exprs->pType->type==REAL_t && yyvsp[0].exprs->pType->type!=REAL_t))
				{
					ChangeType=2;
				}
				else if((yyvsp[-2].exprs->pType->type!=REAL_t && yyvsp[0].exprs->pType->type==REAL_t))
				{
					ChangeType=1;
				}
				else if((yyvsp[-2].exprs->pType->type==REAL_t || yyvsp[0].exprs->pType->type==REAL_t))
				{
					ChangeType=3;
				}
				strcat(tmp,ExprStack[StackCount-2]);
				/*sprintf(tmp,"%s",ExprStack[StackCount-2]);*/
				if(ChangeType==1)strcat(tmp,"i2f\n");
				strcat(tmp,ExprStack[StackCount-1]);
				/*sprintf(tmp,"%s",ExprStack[StackCount-1]);*/
				if(ChangeType==2)strcat(tmp,"i2f\n");
				if(ChangeType==0)
				{
					switch(yyvsp[-1].intVal)
					{
						case ADD_t:
							strcat(tmp,"iadd\n");	break;
						case SUB_t:
							strcat(tmp,"isub\n");	break;
					}
				}
				else
				{
					switch(yyvsp[-1].intVal)
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
break;
case 94:
{ yyval.exprs = yyvsp[0].exprs; }
break;
case 95:
{ yyval.intVal = ADD_t; }
break;
case 96:
{ yyval.intVal = SUB_t; }
break;
case 97:
{
				dumpEStack("term");
				char* tmp=(char*)calloc(1000,sizeof(char));
				/*for(int i=0;i<1000;i++)tmp[i]=0;*/
				int ChangeType=0;
				if((yyvsp[-2].exprs->pType->type==REAL_t && yyvsp[0].exprs->pType->type!=REAL_t))
				{
					ChangeType=2;
				}
				else if((yyvsp[-2].exprs->pType->type!=REAL_t && yyvsp[0].exprs->pType->type==REAL_t))
				{
					ChangeType=1;
				}
				else if((yyvsp[-2].exprs->pType->type==REAL_t || yyvsp[0].exprs->pType->type==REAL_t))
				{
					ChangeType=3;
				}
			/*	if(StackCount<2)*/
				strcat(tmp,ExprStack[StackCount-2]);
				if(ChangeType==1)strcat(tmp,"i2f\n");
				strcat(tmp,ExprStack[StackCount-1]);
				if(ChangeType==2)strcat(tmp,"i2f\n");
				if(ChangeType==0)
				{
				 /*fprintf(out,"[mul!!]\n");*/
					switch(yyvsp[-1].intVal)
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
					switch(yyvsp[-1].intVal)
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
				
			  if( yyvsp[-1].intVal == MOD_t ) {
				verifyModOp( yyvsp[-2].exprs, yyvsp[0].exprs );
				
			  }
			  else {
				verifyArithmeticOp( yyvsp[-2].exprs, yyvsp[-1].intVal, yyvsp[0].exprs );
			  }
			  yyval.exprs = yyvsp[-2].exprs;
			}
break;
case 98:
{ yyval.exprs = yyvsp[0].exprs; }
break;
case 99:
{ yyval.intVal = MUL_t; }
break;
case 100:
{ yyval.intVal = DIV_t; }
break;
case 101:
{ yyval.intVal = MOD_t; }
break;
case 102:
{
			  verifyExistence( symbolTable, yyvsp[0].exprs, scope, __FALSE );
			  yyval.exprs = yyvsp[0].exprs;
			  yyval.exprs->beginningOp = NONE_t;
			 /* fprintf(out,"[factor %d %s]\n",$$->pType->type,$$->varRef->id);*/
			  		struct expr_sem*v=yyvsp[0].exprs;
					int shoot=-1;
					struct SymNode*glo=0;
					/*fprintf(out,";[expr]\n");*/
					if(ExprStack[StackCount]==NULL)
					{
						ExprStack[StackCount]=(char*)calloc(1000,sizeof(char));
						/*for(int i=0;i<1000;i++)tmp[i]=0;*/
					}
					if(glo=lookupSymbol(symbolTable, v->varRef->id, 0, __TRUE ),glo!=0)
					{
						/*fprintf(out,"typ=%d  %s\n",glo->type->type,glo->name);fflush(out);*/
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
break;
case 103:
{
			  if( verifyExistence( symbolTable, yyvsp[0].exprs, scope, __FALSE ) == __TRUE )
				verifyUnaryMinus( yyvsp[0].exprs );
			  yyval.exprs = yyvsp[0].exprs;
			  yyval.exprs->beginningOp = SUB_t;
					if(ExprStack[StackCount]==NULL)ExprStack[StackCount]=(char*)calloc(1000,sizeof(char));
					/*if(ExprStack[StackCount+1]==NULL)ExprStack[StackCount+1]=(char*)malloc(sizeof(char)*100);*/
			  		struct expr_sem*v=yyvsp[0].exprs;
					int shoot=-1;
					struct SymNode*glo=0;
					if(glo=lookupSymbol(symbolTable, v->varRef->id, 0, __TRUE ),glo!=0)
					{
						/*fprintf(out,"typ=%d  %s\n",glo->type->type,glo->name);fflush(out);*/
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
break;
case 104:
{
			  yyvsp[-1].exprs->beginningOp = NONE_t;
			  yyval.exprs = yyvsp[-1].exprs; 
			}
break;
case 105:
{
			  verifyUnaryMinus( yyvsp[-1].exprs );
			  yyval.exprs = yyvsp[-1].exprs;
			  yyval.exprs->beginningOp = SUB_t;
			  if(yyvsp[-1].exprs->pType->type==REAL_t)
			  {
			  	strcat(ExprStack[StackCount-1],"fneg\n");
			  }
			  else
			  {
			  	strcat(ExprStack[StackCount-1],"ineg\n");
			  }
			}
break;
case 106:
{
				struct expr_sem*ret;
			  yyval.exprs = verifyFuncInvoke( yyvsp[-3].lexeme, yyvsp[-1].exprs, symbolTable, scope );
			  yyval.exprs->beginningOp = NONE_t;
			  ret=yyval.exprs;
			  /*ExprStack[StackCount]=*/
			  char*tmp=(char*)calloc(1000,sizeof(char));
			  	int count=0;
				 struct expr_sem*ha;
				for(ha=yyvsp[-1].exprs;ha!=NULL;ha=ha->next)count++;
				
				for(i=count;i>0;i--)
				{
				strcat(tmp,ExprStack[StackCount-i]);
					superfree(&ExprStack[StackCount-i]);
				}
				
				strcat(tmp,"invokestatic ");
				strcat(tmp,fileName);
				strcat(tmp,"/");
				strcat(tmp,yyvsp[-3].lexeme);
				strcat(tmp,"(");
				/*fprintf(out,"invokestatic %s/%s(",fileName,$1);*/
				struct expr_sem*t;
				for(t=yyvsp[-1].exprs;t!=NULL;t=t->next)
				{
					switch(t->pType->type)
						{
							case INTEGER_t: strcat(tmp,"I"); break;
							case BOOLEAN_t: strcat(tmp,"Z"); break;
							case REAL_t:    strcat(tmp,"F"); break;
							/*default:	fprintf(out,"?");*/
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
break;
case 107:
{
			  yyval.exprs = verifyFuncInvoke( yyvsp[-3].lexeme, yyvsp[-1].exprs, symbolTable, scope );
			  yyval.exprs->beginningOp = SUB_t;
			  			  
			}
break;
case 108:
{
			  yyval.exprs = (struct expr_sem *)malloc(sizeof(struct expr_sem));
			  yyval.exprs->isDeref = __TRUE;
			  yyval.exprs->varRef = 0;
			  if(ExprStack[StackCount]==NULL)ExprStack[StackCount]=(char*)calloc(1000,sizeof(char));
			if( (yyvsp[0].constVal->category == INTEGER_t) || (yyvsp[0].constVal->category == BOOLEAN_t) || (yyvsp[0].constVal->category == REAL_t) )
			{	yyval.exprs->pType = createPTypeWithValue(yyvsp[0].constVal->category, &(yyvsp[0].constVal->value));
			/*fprintf(out,";[expr]\n");*/
			  	if((yyvsp[0].constVal->category == INTEGER_t) )
			  		sprintf(ExprStack[StackCount],"ldc %d\n",yyvsp[0].constVal->value);
				else if(yyvsp[0].constVal->category==BOOLEAN_t)
				{
					if(yyvsp[0].constVal->value.booleanVal ==__TRUE)
					{
						sprintf(ExprStack[StackCount],"iconst_1\n");
					}
					else
					{
						sprintf(ExprStack[StackCount],"iconst_0\n");
					}

				}
			  	else if((yyvsp[0].constVal->category == REAL_t))
			  		sprintf(ExprStack[StackCount],"ldc %f\n",yyvsp[0].constVal->value.realVal);

			  }
			  else if(yyvsp[0].constVal->category == STRING_t)
			  {
			  yyval.exprs->pType = createPType( yyvsp[0].constVal->category );
			yyval.exprs->pType->strv=(char*)calloc(1000,sizeof(char));
			strcpy(yyval.exprs->pType->strv,yyvsp[0].constVal->value.stringVal);
			sprintf(ExprStack[StackCount],"ldc \"%s\"\n",yyval.exprs->pType->strv);
			  }
			  else 
			  	yyval.exprs->pType = createPType( yyvsp[0].constVal->category );
			  yyval.exprs->next = 0;
			  
			  StackCount++;
			  dumpEStack("const");
			  if( yyvsp[0].constVal->hasMinus == __TRUE ) {
			  	yyval.exprs->beginningOp = SUB_t;
			  }
			  else {
				yyval.exprs->beginningOp = NONE_t;
			  }
			}
break;
case 109:
{
			  yyval.exprs = createExprSem( yyvsp[0].lexeme );
			}
break;
case 110:
{
			  increaseDim( yyvsp[-1].exprs, yyvsp[0].intVal );
			  yyval.exprs = yyvsp[-1].exprs;
			}
break;
case 111:
{
			  yyval.intVal = verifyArrayIndex( yyvsp[-1].exprs );
			}
break;
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yysslim && yygrowstack())
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
