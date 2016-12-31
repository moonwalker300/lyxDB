/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         SQL_STYPE
#define YYLTYPE         SQL_LTYPE
/* Substitute the variable and function names.  */
#define yyparse         sql_parse
#define yylex           sql_lex
#define yyerror         sql_error
#define yydebug         sql_debug
#define yynerrs         sql_nerrs


/* Copy the first part of user declarations.  */
#line 1 "parser.y" /* yacc.c:339  */

#include "parser.h"
#include "lexer.h"
#include <stdio.h>
int yyerror(YYLTYPE* llocp, AST** result, yyscan_t scanner, const char *msg) {
	AST* list = new AST();
	list->isValid = false;
	list->errorMsg = _strdup(msg);
	list->errorLine = llocp->first_line;
	list->errorColumn = llocp->first_column;
	*result = list;
	return 0;
}

#line 89 "parser.cpp" /* yacc.c:339  */

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.h".  */
#ifndef YY_SQL_PARSER_H_INCLUDED
# define YY_SQL_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef SQL_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define SQL_DEBUG 1
#  else
#   define SQL_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define SQL_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined SQL_DEBUG */
#if SQL_DEBUG
extern int sql_debug;
#endif
/* "%code requires" blocks.  */
#line 16 "parser.y" /* yacc.c:355  */

// %code requires block	
#include "AST.h"
#include <vector>
typedef void* yyscan_t;
#define YYSTYPE SQL_STYPE
#define YYLTYPE SQL_LTYPE
struct SQL_CUST_LTYPE {
	int first_line;
	int first_column;
	int last_line;
	int last_column;
	int total_column;
};
#define SQL_LTYPE SQL_CUST_LTYPE
#define SQL_LTYPE_IS_DECLARED 1
// Auto update column and line number
#define YY_USER_ACTION \
    yylloc->first_line = yylloc->last_line; \
    yylloc->first_column = yylloc->last_column; \
    for(int i = 0; yytext[i] != '\0'; i++) { \
    	yylloc->total_column++; \
        if(yytext[i] == '\n') { \
            yylloc->last_line++; \
            yylloc->last_column = 0; \
        } \
        else { \
            yylloc->last_column++; \
        } \
    }
AST* parseSQLString(const char* sql);
AST* parseSQLString(const std::string& sql);

#line 161 "parser.cpp" /* yacc.c:355  */

/* Token type.  */
#ifndef SQL_TOKENTYPE
# define SQL_TOKENTYPE
  enum sql_tokentype
  {
    SQL_IDENTIFIER = 258,
    SQL_STRING = 259,
    SQL_FLOATVAL = 260,
    SQL_INTVAL = 261,
    SQL_NOTEQUALS = 262,
    SQL_LESSEQ = 263,
    SQL_GREATEREQ = 264,
    SQL_DATABASES = 265,
    SQL_DISTINCT = 266,
    SQL_DATABASE = 267,
    SQL_BETWEEN = 268,
    SQL_VARCHAR = 269,
    SQL_CASCADE = 270,
    SQL_COLUMNS = 271,
    SQL_CONTROL = 272,
    SQL_DEFAULT = 273,
    SQL_DECIMAL = 274,
    SQL_EXPLAIN = 275,
    SQL_HISTORY = 276,
    SQL_INTEGER = 277,
    SQL_NATURAL = 278,
    SQL_PRIMARY = 279,
    SQL_SCHEMAS = 280,
    SQL_SPATIAL = 281,
    SQL_VIRTUAL = 282,
    SQL_BEFORE = 283,
    SQL_COLUMN = 284,
    SQL_CREATE = 285,
    SQL_DELETE = 286,
    SQL_DIRECT = 287,
    SQL_DOUBLE = 288,
    SQL_ESCAPE = 289,
    SQL_EXCEPT = 290,
    SQL_EXISTS = 291,
    SQL_GLOBAL = 292,
    SQL_HAVING = 293,
    SQL_INSERT = 294,
    SQL_ISNULL = 295,
    SQL_OFFSET = 296,
    SQL_RENAME = 297,
    SQL_SCHEMA = 298,
    SQL_SELECT = 299,
    SQL_SORTED = 300,
    SQL_TABLES = 301,
    SQL_UNIQUE = 302,
    SQL_UNLOAD = 303,
    SQL_UPDATE = 304,
    SQL_VALUES = 305,
    SQL_AFTER = 306,
    SQL_ALTER = 307,
    SQL_CROSS = 308,
    SQL_DELTA = 309,
    SQL_GROUP = 310,
    SQL_INDEX = 311,
    SQL_INNER = 312,
    SQL_LOCAL = 313,
    SQL_MERGE = 314,
    SQL_MINUS = 315,
    SQL_ORDER = 316,
    SQL_CHECK = 317,
    SQL_OUTER = 318,
    SQL_RIGHT = 319,
    SQL_TABLE = 320,
    SQL_UNION = 321,
    SQL_USING = 322,
    SQL_WHERE = 323,
    SQL_CALL = 324,
    SQL_DATE = 325,
    SQL_DESC = 326,
    SQL_DROP = 327,
    SQL_FILE = 328,
    SQL_FROM = 329,
    SQL_FULL = 330,
    SQL_HASH = 331,
    SQL_HINT = 332,
    SQL_INTO = 333,
    SQL_JOIN = 334,
    SQL_LEFT = 335,
    SQL_LIKE = 336,
    SQL_LOAD = 337,
    SQL_NULL = 338,
    SQL_PART = 339,
    SQL_PLAN = 340,
    SQL_SHOW = 341,
    SQL_TEXT = 342,
    SQL_TIME = 343,
    SQL_VIEW = 344,
    SQL_WITH = 345,
    SQL_CHAR = 346,
    SQL_ADD = 347,
    SQL_ALL = 348,
    SQL_AND = 349,
    SQL_ASC = 350,
    SQL_CSV = 351,
    SQL_FOR = 352,
    SQL_INT = 353,
    SQL_KEY = 354,
    SQL_NOT = 355,
    SQL_OFF = 356,
    SQL_SET = 357,
    SQL_TBL = 358,
    SQL_TOP = 359,
    SQL_USE = 360,
    SQL_AS = 361,
    SQL_BY = 362,
    SQL_IF = 363,
    SQL_IN = 364,
    SQL_IS = 365,
    SQL_OF = 366,
    SQL_ON = 367,
    SQL_OR = 368,
    SQL_TO = 369,
    SQL_EQUALS = 370,
    SQL_LESS = 371,
    SQL_GREATER = 372,
    SQL_NOTNULL = 373,
    SQL_UMINUS = 374
  };
#endif

/* Value type.  */
#if ! defined SQL_STYPE && ! defined SQL_STYPE_IS_DECLARED
typedef union SQL_STYPE SQL_STYPE;
union SQL_STYPE
{
#line 75 "parser.y" /* yacc.c:355  */

	double fval;
	int64_t ival;
	char* sval;
	uint64_t uval;
	bool bval;
	SQLStatement* statement;
	CreateDatabaseStatement* create_database_stmt;
	DropDatabaseStatement* drop_database_stmt;
	ShowDatabasesStatement* show_databases_stmt;
	UseStatement* use_stmt;
	ShowTablesStatement* show_tables_stmt;
	DescStatement* desc_stmt;
	CreateIndexStatement* create_index_stmt;
	DropIndexStatement* drop_index_stmt;
	SelectStatement* select_stmt;
	CreateStatement* create_stmt;
	InsertStatement* insert_stmt;
	DeleteStatement* delete_stmt;
	UpdateStatement* update_stmt;
	DropStatement* drop_stmt;
	TableRef* table;
	Expr* expr;
	ColumnDefinition* column_t;
	GroupByDescription* group_t;
	UpdateClause* update_t;
	AST* stmt_list;
	std::vector<char*>* str_vec;
	std::vector<TableRef*>* table_vec;
	std::vector<ColumnDefinition*>* column_vec;
	std::vector<UpdateClause*>* update_vec;
	std::vector<Expr*>* expr_vec;
	std::vector<std::vector<Expr*>*>* expr_vecs;

#line 328 "parser.cpp" /* yacc.c:355  */
};
# define SQL_STYPE_IS_TRIVIAL 1
# define SQL_STYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined SQL_LTYPE && ! defined SQL_LTYPE_IS_DECLARED
typedef struct SQL_LTYPE SQL_LTYPE;
struct SQL_LTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define SQL_LTYPE_IS_DECLARED 1
# define SQL_LTYPE_IS_TRIVIAL 1
#endif



int sql_parse (AST** result, yyscan_t scanner);

#endif /* !YY_SQL_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 356 "parser.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#  define __attribute__(Spec) /* empty */
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined SQL_LTYPE_IS_TRIVIAL && SQL_LTYPE_IS_TRIVIAL \
             && defined SQL_STYPE_IS_TRIVIAL && SQL_STYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  44
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   210

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  136
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  100
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  179

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   374

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   126,     2,     2,
     131,   132,   124,   122,   135,   123,   133,   125,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   134,
     117,   115,   118,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   129,     2,   130,   127,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     116,   119,   120,   121,   128
};

#if SQL_DEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   183,   183,   188,   189,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   208,
     213,   218,   222,   227,   231,   236,   242,   248,   255,   256,
     259,   264,   269,   270,   272,   273,   274,   275,   276,   279,
     285,   292,   299,   303,   307,   315,   316,   319,   326,   336,
     337,   340,   343,   346,   347,   350,   355,   358,   359,   362,
     363,   366,   367,   370,   371,   372,   373,   374,   377,   378,
     379,   382,   383,   386,   387,   388,   389,   390,   391,   392,
     393,   396,   399,   400,   403,   404,   405,   408,   411,   412,
     415,   418,   421,   422,   430,   431,   434,   440,   441,   444,
     445
};
#endif

#if SQL_DEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "STRING", "FLOATVAL",
  "INTVAL", "NOTEQUALS", "LESSEQ", "GREATEREQ", "DATABASES", "DISTINCT",
  "DATABASE", "BETWEEN", "VARCHAR", "CASCADE", "COLUMNS", "CONTROL",
  "DEFAULT", "DECIMAL", "EXPLAIN", "HISTORY", "INTEGER", "NATURAL",
  "PRIMARY", "SCHEMAS", "SPATIAL", "VIRTUAL", "BEFORE", "COLUMN", "CREATE",
  "DELETE", "DIRECT", "DOUBLE", "ESCAPE", "EXCEPT", "EXISTS", "GLOBAL",
  "HAVING", "INSERT", "ISNULL", "OFFSET", "RENAME", "SCHEMA", "SELECT",
  "SORTED", "TABLES", "UNIQUE", "UNLOAD", "UPDATE", "VALUES", "AFTER",
  "ALTER", "CROSS", "DELTA", "GROUP", "INDEX", "INNER", "LOCAL", "MERGE",
  "MINUS", "ORDER", "CHECK", "OUTER", "RIGHT", "TABLE", "UNION", "USING",
  "WHERE", "CALL", "DATE", "DESC", "DROP", "FILE", "FROM", "FULL", "HASH",
  "HINT", "INTO", "JOIN", "LEFT", "LIKE", "LOAD", "NULL", "PART", "PLAN",
  "SHOW", "TEXT", "TIME", "VIEW", "WITH", "CHAR", "ADD", "ALL", "AND",
  "ASC", "CSV", "FOR", "INT", "KEY", "NOT", "OFF", "SET", "TBL", "TOP",
  "USE", "AS", "BY", "IF", "IN", "IS", "OF", "ON", "OR", "TO", "'='",
  "EQUALS", "'<'", "'>'", "LESS", "GREATER", "NOTNULL", "'+'", "'-'",
  "'*'", "'/'", "'%'", "'^'", "UMINUS", "'['", "']'", "'('", "')'", "'.'",
  "';'", "','", "$accept", "input", "statement_list", "statement",
  "create_database_statement", "drop_database_statement",
  "show_databases_statement", "use_statement", "show_tables_statement",
  "desc_statement", "create_index_statement", "drop_index_statement",
  "create_statement", "column_def_commalist", "column_def", "opt_not_null",
  "column_type", "drop_statement", "delete_statement", "insert_statement",
  "literal_lists", "update_statement", "update_clause_commalist",
  "update_clause", "select_statement", "opt_distinct", "select_list",
  "from_clause", "opt_where", "opt_group", "opt_having", "expr_list",
  "literal_list", "expr", "scalar_expr", "unary_expr", "binary_expr",
  "function_expr", "column_name", "literal", "string_literal",
  "num_literal", "int_literal", "star_expr", "table_ref",
  "table_ref_commalist", "table_ref_name", "table_name", "opt_semicolon", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,    61,   370,    60,    62,   371,
     372,   373,    43,    45,    42,    47,    37,    94,   374,    91,
      93,    40,    41,    46,    59,    44
};
# endif

#define YYPACT_NINF -105

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-105)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-1)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     105,     3,   -40,   -28,    33,    53,    55,    17,     9,    61,
      66,   -63,  -105,  -105,  -105,  -105,  -105,  -105,  -105,  -105,
    -105,  -105,  -105,  -105,  -105,  -105,  -105,    64,    72,    53,
      53,    53,  -105,    -3,   -56,   -24,  -105,  -105,    81,    82,
      53,  -105,  -105,  -105,  -105,   105,  -105,  -105,   -45,   -44,
      21,    38,   -57,  -105,  -105,  -105,  -105,  -105,    -3,    30,
     -41,    28,  -105,  -105,  -105,  -105,  -105,  -105,  -105,  -105,
    -105,  -105,    88,    95,  -105,   -29,  -105,  -105,   102,    24,
      -3,  -105,   -25,    33,   104,    -2,    53,    21,    -3,    -3,
      -3,    -3,    -3,    -3,   -51,    -3,    -3,    -3,  -105,    -5,
     -54,  -105,   108,   -18,    -1,    34,  -104,  -105,    28,    20,
      -8,    -3,  -105,  -105,  -105,     4,    85,    28,    14,     8,
       8,    14,    14,  -105,    54,    14,     8,     8,    20,    95,
    -105,    10,  -105,  -105,  -105,  -105,  -105,  -105,    16,    19,
    -105,    24,   -75,  -105,    22,     2,    53,    41,  -105,  -105,
    -105,  -105,  -105,   135,   148,  -105,  -105,    20,    20,  -105,
      25,  -105,    -3,    26,    27,  -105,   -70,    53,   -34,    52,
    -105,  -105,  -105,    -3,  -105,    73,  -105,    28,  -105
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,   100,     3,     5,     6,     7,     8,     9,    10,    11,
      12,    14,    18,    16,    15,    17,    13,     0,     0,     0,
       0,     0,    49,     0,    97,     0,    96,    24,     0,     0,
       0,    21,    23,    22,     1,    99,     2,    19,     0,     0,
      54,     0,    82,    87,    88,    90,    86,    91,     0,     0,
      51,    59,    64,    65,    66,    67,    68,    70,    84,    85,
      89,    69,     0,     0,    20,     0,    39,     4,     0,     0,
       0,    40,     0,    50,     0,     0,     0,    54,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
      54,    45,     0,     0,     0,     0,     0,    28,    53,     0,
      41,     0,    83,    63,    52,    92,    56,    60,    74,    77,
      78,    80,    79,    71,     0,    73,    75,    76,     0,     0,
      44,     0,    25,    36,    38,    37,    35,    34,     0,     0,
      27,     0,     0,    61,     0,     0,     0,     0,    48,    72,
      47,    46,    26,     0,     0,    29,    42,     0,     0,    81,
      93,    94,     0,     0,     0,    62,     0,     0,    58,    33,
      31,    43,    95,     0,    55,     0,    30,    57,    32
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -105,  -105,  -105,   110,  -105,  -105,  -105,  -105,  -105,  -105,
    -105,  -105,  -105,  -105,    29,  -105,  -105,  -105,  -105,  -105,
    -105,  -105,  -105,    32,  -105,    74,  -105,  -105,   -67,  -105,
    -105,     0,     5,   -50,  -105,  -105,  -105,  -105,  -105,   -58,
    -105,  -105,  -105,  -105,  -105,  -105,   -74,    23,  -105
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,   106,   107,   176,   138,    22,    23,    24,
     110,    25,   100,   101,    26,    33,    59,    87,    81,   148,
     174,    60,   142,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,   114,   160,    35,    36,    46
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      52,    53,    54,    55,   173,    89,    90,    91,    85,    89,
      90,    91,   115,   133,    80,    27,    -1,    -1,   134,    41,
     116,    89,    90,    91,    53,    54,    55,   104,   140,    38,
     108,   141,   123,   130,    30,    89,    90,    91,   117,   118,
     119,   120,   121,   122,    32,   125,   126,   127,   105,   124,
      31,   143,    49,    50,    51,    42,    34,   156,    37,    28,
     157,   145,   171,    76,    43,   157,    44,    47,    29,   135,
     150,    45,   161,    39,    83,    48,    84,    72,    73,    92,
      56,   129,    40,    92,    74,    75,    78,    79,    82,    80,
     136,    98,    93,   172,    88,    92,    93,   137,    99,   165,
     143,    88,   102,    56,    86,   103,   109,   112,    94,    92,
     128,   131,    94,    95,   132,    96,    97,    95,    94,    96,
      97,    57,    93,   177,    94,    -1,    -1,   144,    58,    95,
     113,    96,    97,   139,   159,     1,     2,   149,    94,   146,
     147,   163,   152,    95,     3,    96,    97,   153,   162,     4,
     154,   164,   175,   158,     5,    77,   178,   111,   169,   170,
     167,   151,   168,   166,     0,     0,     0,     0,     0,     0,
     155,     0,     0,     0,     0,     0,     6,     7,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     8,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       9
};

static const yytype_int16 yycheck[] =
{
       3,     4,     5,     6,    38,     7,     8,     9,    58,     7,
       8,     9,    86,    14,    68,    12,     8,     9,    19,    10,
      87,     7,     8,     9,     4,     5,     6,     3,   132,    12,
      80,   135,    83,   100,    74,     7,     8,     9,    88,    89,
      90,    91,    92,    93,    11,    95,    96,    97,    24,   100,
      78,   109,    29,    30,    31,    46,     3,   132,     3,    56,
     135,   111,   132,    40,     3,   135,     0,     3,    65,    70,
     128,   134,   146,    56,   131,     3,   133,   133,   102,    81,
      83,   135,    65,    81,     3,     3,   131,   131,    50,    68,
      91,     3,    94,   167,   135,    81,    94,    98,     3,   157,
     158,   135,   131,    83,    74,     3,   131,     3,   110,    81,
     115,     3,   110,   115,   132,   117,   118,   115,   110,   117,
     118,   124,    94,   173,   110,   117,   118,   135,   131,   115,
     132,   117,   118,    99,   132,    30,    31,    83,   110,   135,
      55,     6,   132,   115,    39,   117,   118,   131,   107,    44,
     131,     3,   100,   131,    49,    45,    83,    83,   132,   132,
     135,   129,   162,   158,    -1,    -1,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    30,    31,    39,    44,    49,    71,    72,    86,   105,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   153,   154,   155,   157,   160,    12,    56,    65,
      74,    78,    11,   161,     3,   182,   183,     3,    12,    56,
      65,    10,    46,     3,     0,   134,   184,     3,     3,   183,
     183,   183,     3,     4,     5,     6,    83,   124,   131,   162,
     167,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   133,   102,     3,     3,   183,   139,   131,   131,
      68,   164,    50,   131,   133,   169,    74,   163,   135,     7,
       8,     9,    81,    94,   110,   115,   117,   118,     3,     3,
     158,   159,   131,     3,     3,    24,   149,   150,   169,   131,
     156,   161,     3,   132,   180,   182,   164,   169,   169,   169,
     169,   169,   169,    83,   100,   169,   169,   169,   115,   135,
     164,     3,   132,    14,    19,    70,    91,    98,   152,    99,
     132,   135,   168,   175,   135,   169,   135,    55,   165,    83,
     175,   159,   132,   131,   131,   150,   132,   135,   131,   132,
     181,   182,   107,     6,     3,   175,   168,   135,   167,   132,
     132,   132,   182,    38,   166,   100,   151,   169,    83
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   136,   137,   138,   138,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   149,
     150,   150,   151,   151,   152,   152,   152,   152,   152,   153,
     154,   155,   156,   156,   157,   158,   158,   159,   160,   161,
     161,   162,   163,   164,   164,   165,   165,   166,   166,   167,
     167,   168,   168,   169,   169,   169,   169,   169,   170,   170,
     170,   171,   171,   172,   172,   172,   172,   172,   172,   172,
     172,   173,   174,   174,   175,   175,   175,   176,   177,   177,
     178,   179,   180,   180,   181,   181,   182,   183,   183,   184,
     184
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     2,     2,     2,     2,     6,     6,     6,     1,     3,
       6,     5,     2,     0,     1,     1,     1,     1,     1,     3,
       4,     5,     3,     5,     5,     1,     3,     3,     6,     1,
       0,     1,     2,     2,     0,     4,     0,     2,     0,     1,
       3,     1,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     3,     4,     3,     3,     3,     3,     3,     3,     3,
       3,     5,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     3,     1,     1,     3,     1,
       0
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (&yylloc, result, scanner, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if SQL_DEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined SQL_LTYPE_IS_TRIVIAL && SQL_LTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

__attribute__((__unused__))
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, result, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, AST** result, yyscan_t scanner)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (result);
  YYUSE (scanner);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, AST** result, yyscan_t scanner)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, result, scanner);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, AST** result, yyscan_t scanner)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , result, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, result, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !SQL_DEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !SQL_DEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, AST** result, yyscan_t scanner)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (result);
  YYUSE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (AST** result, yyscan_t scanner)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined SQL_LTYPE_IS_TRIVIAL && SQL_LTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

/* User initialization code.  */
#line 59 "parser.y" /* yacc.c:1429  */
{
	// Initialize
	yylloc.first_column = 0;
	yylloc.last_column = 0;
	yylloc.first_line = 0;
	yylloc.last_line = 0;
	yylloc.total_column = 0;
}

#line 1515 "parser.cpp" /* yacc.c:1429  */
  yylsp[0] = yylloc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 183 "parser.y" /* yacc.c:1646  */
    {
			*result = (yyvsp[-1].stmt_list);
		}
#line 1706 "parser.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 188 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new AST((yyvsp[0].statement)); }
#line 1712 "parser.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 189 "parser.y" /* yacc.c:1646  */
    { (yyvsp[-2].stmt_list)->addStatement((yyvsp[0].statement)); (yyval.stmt_list) = (yyvsp[-2].stmt_list); }
#line 1718 "parser.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 192 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].create_database_stmt); }
#line 1724 "parser.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 193 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].drop_database_stmt); }
#line 1730 "parser.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 194 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].show_databases_stmt); }
#line 1736 "parser.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 195 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].use_stmt); }
#line 1742 "parser.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 196 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].show_tables_stmt); }
#line 1748 "parser.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 197 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].desc_stmt); }
#line 1754 "parser.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 198 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].create_index_stmt); }
#line 1760 "parser.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 199 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].drop_index_stmt); }
#line 1766 "parser.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 200 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].select_stmt); }
#line 1772 "parser.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 201 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].create_stmt); }
#line 1778 "parser.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 202 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].insert_stmt); }
#line 1784 "parser.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 203 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].delete_stmt); }
#line 1790 "parser.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 204 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].update_stmt); }
#line 1796 "parser.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 205 "parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].drop_stmt); }
#line 1802 "parser.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 208 "parser.y" /* yacc.c:1646  */
    {
			(yyval.create_database_stmt) = new CreateDatabaseStatement();
			(yyval.create_database_stmt)->databaseName = (yyvsp[0].sval);
		}
#line 1811 "parser.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 213 "parser.y" /* yacc.c:1646  */
    {
			(yyval.drop_database_stmt) = new DropDatabaseStatement();
			(yyval.drop_database_stmt)->databaseName = (yyvsp[0].sval);
		}
#line 1820 "parser.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 218 "parser.y" /* yacc.c:1646  */
    { 
			(yyval.show_databases_stmt) = new ShowDatabasesStatement();
		}
#line 1828 "parser.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 222 "parser.y" /* yacc.c:1646  */
    {
			(yyval.use_stmt) = new UseStatement();
			(yyval.use_stmt)->databaseName = (yyvsp[0].sval);
		}
#line 1837 "parser.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 227 "parser.y" /* yacc.c:1646  */
    {
			(yyval.show_tables_stmt) = new ShowTablesStatement();
		}
#line 1845 "parser.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 231 "parser.y" /* yacc.c:1646  */
    {
			(yyval.desc_stmt) = new DescStatement();
			(yyval.desc_stmt)->tableName = (yyvsp[0].sval);
		}
#line 1854 "parser.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 236 "parser.y" /* yacc.c:1646  */
    {
			(yyval.create_index_stmt) = new CreateIndexStatement();
			(yyval.create_index_stmt)->tableName = (yyvsp[-3].sval);
			(yyval.create_index_stmt)->columnName = (yyvsp[-1].sval);
		}
#line 1864 "parser.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 242 "parser.y" /* yacc.c:1646  */
    {
			(yyval.drop_index_stmt) = new DropIndexStatement();
			(yyval.drop_index_stmt)->tableName = (yyvsp[-3].sval);
			(yyval.drop_index_stmt)->columnName = (yyvsp[-1].sval);
		}
#line 1874 "parser.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 248 "parser.y" /* yacc.c:1646  */
    {
			(yyval.create_stmt) = new CreateStatement();
			(yyval.create_stmt)->tableName = (yyvsp[-3].sval);
			(yyval.create_stmt)->columns = (yyvsp[-1].column_vec);
		}
#line 1884 "parser.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 255 "parser.y" /* yacc.c:1646  */
    { (yyval.column_vec) = new std::vector<ColumnDefinition*>(); (yyval.column_vec)->push_back((yyvsp[0].column_t)); }
#line 1890 "parser.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 256 "parser.y" /* yacc.c:1646  */
    { (yyvsp[-2].column_vec)->push_back((yyvsp[0].column_t)); (yyval.column_vec) = (yyvsp[-2].column_vec); }
#line 1896 "parser.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 259 "parser.y" /* yacc.c:1646  */
    {
			(yyval.column_t) = new ColumnDefinition((yyvsp[-5].sval), (DataType) (yyvsp[-4].uval));
			(yyval.column_t)->length = (yyvsp[-2].ival);
			(yyval.column_t)->notnull = (yyvsp[0].bval);
		}
#line 1906 "parser.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 264 "parser.y" /* yacc.c:1646  */
    {
			(yyval.column_t) = new ColumnDefinition((yyvsp[-1].sval), (DataType)DTPRIMARY);
		}
#line 1914 "parser.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 269 "parser.y" /* yacc.c:1646  */
    { (yyval.bval) = true; }
#line 1920 "parser.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 270 "parser.y" /* yacc.c:1646  */
    { (yyval.bval) = false; }
#line 1926 "parser.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 272 "parser.y" /* yacc.c:1646  */
    { (yyval.uval) = DTINT; }
#line 1932 "parser.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 273 "parser.y" /* yacc.c:1646  */
    { (yyval.uval) = DTCHAR; }
#line 1938 "parser.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 274 "parser.y" /* yacc.c:1646  */
    { (yyval.uval) = DTVARCHAR; }
#line 1944 "parser.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 275 "parser.y" /* yacc.c:1646  */
    { (yyval.uval) = DTDATE; }
#line 1950 "parser.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 276 "parser.y" /* yacc.c:1646  */
    { (yyval.uval) = DTDECIMAL; }
#line 1956 "parser.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 279 "parser.y" /* yacc.c:1646  */
    {
			(yyval.drop_stmt) = new DropStatement();
			(yyval.drop_stmt)->name = (yyvsp[0].sval);
		}
#line 1965 "parser.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 285 "parser.y" /* yacc.c:1646  */
    {
			(yyval.delete_stmt) = new DeleteStatement();
			(yyval.delete_stmt)->tableName = (yyvsp[-1].sval);
			(yyval.delete_stmt)->expr = (yyvsp[0].expr);
		}
#line 1975 "parser.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 292 "parser.y" /* yacc.c:1646  */
    {
			(yyval.insert_stmt) = new InsertStatement();
			(yyval.insert_stmt)->tableName = (yyvsp[-2].sval);
			(yyval.insert_stmt)->valuelists = (yyvsp[0].expr_vecs);
		}
#line 1985 "parser.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 299 "parser.y" /* yacc.c:1646  */
    {
			(yyval.expr_vecs) = new std::vector<std::vector<Expr*>*>();
			(yyval.expr_vecs)->push_back((yyvsp[-1].expr_vec));
		}
#line 1994 "parser.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 303 "parser.y" /* yacc.c:1646  */
    {
			(yyval.expr_vecs)->push_back((yyvsp[-1].expr_vec));
		}
#line 2002 "parser.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 307 "parser.y" /* yacc.c:1646  */
    {
		(yyval.update_stmt) = new UpdateStatement();
		(yyval.update_stmt)->table = (yyvsp[-3].table);
		(yyval.update_stmt)->updates = (yyvsp[-1].update_vec);
		(yyval.update_stmt)->where = (yyvsp[0].expr);
	}
#line 2013 "parser.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 315 "parser.y" /* yacc.c:1646  */
    { (yyval.update_vec) = new std::vector<UpdateClause*>(); (yyval.update_vec)->push_back((yyvsp[0].update_t)); }
#line 2019 "parser.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 316 "parser.y" /* yacc.c:1646  */
    { (yyvsp[-2].update_vec)->push_back((yyvsp[0].update_t)); (yyval.update_vec) = (yyvsp[-2].update_vec); }
#line 2025 "parser.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 319 "parser.y" /* yacc.c:1646  */
    {
			(yyval.update_t) = new UpdateClause();
			(yyval.update_t)->column = (yyvsp[-2].sval);
			(yyval.update_t)->value = (yyvsp[0].expr);
		}
#line 2035 "parser.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 326 "parser.y" /* yacc.c:1646  */
    {
			(yyval.select_stmt) = new SelectStatement();
			(yyval.select_stmt)->selectDistinct = (yyvsp[-4].bval);
			(yyval.select_stmt)->selectList = (yyvsp[-3].expr_vec);
			(yyval.select_stmt)->fromTable = (yyvsp[-2].table);
			(yyval.select_stmt)->whereClause = (yyvsp[-1].expr);
			(yyval.select_stmt)->groupBy = (yyvsp[0].group_t);
		}
#line 2048 "parser.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 336 "parser.y" /* yacc.c:1646  */
    { (yyval.bval) = true; }
#line 2054 "parser.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 337 "parser.y" /* yacc.c:1646  */
    { (yyval.bval) = false; }
#line 2060 "parser.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 343 "parser.y" /* yacc.c:1646  */
    { (yyval.table) = (yyvsp[0].table); }
#line 2066 "parser.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 346 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 2072 "parser.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 347 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = NULL; }
#line 2078 "parser.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 350 "parser.y" /* yacc.c:1646  */
    {
			(yyval.group_t) = new GroupByDescription();
			(yyval.group_t)->columns = (yyvsp[-1].expr_vec);
			(yyval.group_t)->having = (yyvsp[0].expr);
		}
#line 2088 "parser.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 355 "parser.y" /* yacc.c:1646  */
    { (yyval.group_t) = NULL; }
#line 2094 "parser.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 358 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 2100 "parser.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 359 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = NULL; }
#line 2106 "parser.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 362 "parser.y" /* yacc.c:1646  */
    { (yyval.expr_vec) = new std::vector<Expr*>(); (yyval.expr_vec)->push_back((yyvsp[0].expr)); }
#line 2112 "parser.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 363 "parser.y" /* yacc.c:1646  */
    { (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr)); (yyval.expr_vec) = (yyvsp[-2].expr_vec); }
#line 2118 "parser.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 366 "parser.y" /* yacc.c:1646  */
    { (yyval.expr_vec) = new std::vector<Expr*>(); (yyval.expr_vec)->push_back((yyvsp[0].expr)); }
#line 2124 "parser.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 367 "parser.y" /* yacc.c:1646  */
    { (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr)); (yyval.expr_vec) = (yyvsp[-2].expr_vec); }
#line 2130 "parser.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 370 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[-1].expr); }
#line 2136 "parser.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 382 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpUnary(Expr::ISNULL, (yyvsp[-2].expr)); }
#line 2142 "parser.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 383 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpUnary(Expr::ISNOTNULL, (yyvsp[-3].expr)); }
#line 2148 "parser.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 386 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), Expr::EQ, (yyvsp[0].expr)); }
#line 2154 "parser.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 387 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), Expr::NE, (yyvsp[0].expr)); }
#line 2160 "parser.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 388 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), Expr::LT, (yyvsp[0].expr)); }
#line 2166 "parser.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 389 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), Expr::GT, (yyvsp[0].expr)); }
#line 2172 "parser.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 390 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), Expr::LE, (yyvsp[0].expr)); }
#line 2178 "parser.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 391 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), Expr::GE, (yyvsp[0].expr)); }
#line 2184 "parser.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 392 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), Expr::AND, (yyvsp[0].expr)); }
#line 2190 "parser.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 393 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), Expr::LIKE, (yyvsp[0].expr)); }
#line 2196 "parser.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 396 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeFunctionRef((yyvsp[-4].sval), (yyvsp[-1].expr), (yyvsp[-2].bval)); }
#line 2202 "parser.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 399 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeColumnRef((yyvsp[0].sval)); }
#line 2208 "parser.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 400 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeColumnRef((yyvsp[-2].sval), (yyvsp[0].sval)); }
#line 2214 "parser.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 405 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = NULL; }
#line 2220 "parser.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 408 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeLiteral((yyvsp[0].sval)); }
#line 2226 "parser.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 411 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeLiteral((yyvsp[0].fval)); }
#line 2232 "parser.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 415 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeLiteral((yyvsp[0].ival)); }
#line 2238 "parser.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 418 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = new Expr(kExprStar); }
#line 2244 "parser.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 422 "parser.y" /* yacc.c:1646  */
    {
			(yyvsp[0].table_vec)->push_back((yyvsp[-2].table));
			auto tbl = new TableRef(kTableCrossProduct);
			tbl->list = (yyvsp[0].table_vec);
			(yyval.table) = tbl;
		}
#line 2255 "parser.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 430 "parser.y" /* yacc.c:1646  */
    { (yyval.table_vec) = new std::vector<TableRef*>(); (yyval.table_vec)->push_back((yyvsp[0].table)); }
#line 2261 "parser.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 431 "parser.y" /* yacc.c:1646  */
    { (yyvsp[-2].table_vec)->push_back((yyvsp[0].table)); (yyval.table_vec) = (yyvsp[-2].table_vec); }
#line 2267 "parser.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 434 "parser.y" /* yacc.c:1646  */
    {
			(yyval.table) = new TableRef(kTableName);
			(yyval.table)->name = (yyvsp[0].sval);
		}
#line 2276 "parser.cpp" /* yacc.c:1646  */
    break;


#line 2280 "parser.cpp" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, result, scanner, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, result, scanner, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, result, scanner);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, result, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, result, scanner, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, result, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, result, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 447 "parser.y" /* yacc.c:1906  */

AST* parseSQLString(const char *text) {
    AST* result = NULL;
    yyscan_t scanner;
    YY_BUFFER_STATE state;
    if (sql_lex_init(&scanner)) {
        // couldn't initialize
        fprintf(stderr, "[Error] SQLParser: Error when initializing lexer!\n");
        return NULL;
    }
    state = sql__scan_string(text, scanner);
    if (sql_parse(&result, scanner)) {
        // Returns an error stmt object
        return result;
    }
    sql__delete_buffer(state, scanner);
    sql_lex_destroy(scanner);
    return result;
}
AST* parseSQLString(const std::string& text) {
    return parseSQLString(text.c_str());
}
