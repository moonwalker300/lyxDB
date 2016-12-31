%{
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
%}
// Specify code that is included in the generated .h and .c files
%code requires {
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
}
// Define the names of the created files
%output "parser.cpp"
%defines "parser.h"
// Tell bison to create a reentrant parser
%define api.pure full
// Prefix the parser
%define api.prefix {sql_}
%define api.token.prefix {SQL_}
%define parse.error verbose
%locations
%initial-action {
	// Initialize
	@$.first_column = 0;
	@$.last_column = 0;
	@$.first_line = 0;
	@$.last_line = 0;
	@$.total_column = 0;
};
// Define additional parameters for yylex (http://www.gnu.org/software/bison/manual/html_node/Pure-Calling.html)
%lex-param   { yyscan_t scanner }
// Define additional parameters for yyparse
%parse-param { AST** result }
%parse-param { yyscan_t scanner }
/*********************************
 ** Define all data-types (http://www.gnu.org/software/bison/manual/html_node/Union-Decl.html)
 *********************************/
%union {
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
}
/*********************************
 ** Token Definition
 *********************************/
%token <sval> IDENTIFIER STRING
%token <fval> FLOATVAL
%token <ival> INTVAL
%token <uval> NOTEQUALS LESSEQ GREATEREQ
/* SQL Keywords */
%token DATABASES DISTINCT DATABASE BETWEEN VARCHAR
%token CASCADE COLUMNS CONTROL DEFAULT DECIMAL EXPLAIN
%token HISTORY INTEGER NATURAL PRIMARY SCHEMAS
%token SPATIAL VIRTUAL BEFORE COLUMN CREATE DELETE DIRECT
%token DOUBLE ESCAPE EXCEPT EXISTS GLOBAL HAVING
%token INSERT ISNULL OFFSET RENAME SCHEMA SELECT SORTED
%token TABLES UNIQUE UNLOAD UPDATE VALUES AFTER ALTER CROSS
%token DELTA GROUP INDEX INNER LOCAL MERGE MINUS ORDER CHECK
%token OUTER RIGHT TABLE UNION USING WHERE CALL DATE DESC
%token DROP FILE FROM FULL HASH HINT INTO JOIN LEFT LIKE
%token LOAD NULL PART PLAN SHOW TEXT TIME VIEW WITH CHAR ADD ALL
%token AND ASC CSV FOR INT KEY NOT OFF SET TBL TOP USE AS BY IF
%token IN IS OF ON OR TO
%type <stmt_list> statement_list
%type <statement> statement
%type <create_database_stmt> create_database_statement
%type <drop_database_stmt> drop_database_statement
%type <show_databases_stmt> show_databases_statement
%type <use_stmt> use_statement
%type <show_tables_stmt> show_tables_statement
%type <desc_stmt> desc_statement
%type <create_index_stmt> create_index_statement
%type <drop_index_stmt> drop_index_statement
%type <select_stmt> select_statement
%type <create_stmt> create_statement
%type <insert_stmt> insert_statement
%type <delete_stmt> delete_statement
%type <update_stmt> update_statement
%type <drop_stmt> drop_statement
%type <sval> table_name
%type <bval> opt_distinct opt_not_null
%type <uval> column_type
%type <table> from_clause table_ref table_ref_name
%type <expr> expr scalar_expr unary_expr binary_expr function_expr star_expr
%type <expr> column_name literal int_literal num_literal string_literal
%type <expr> opt_where opt_having
%type <column_t> column_def
%type <update_t> update_clause
%type <group_t> opt_group
%type <expr_vec> expr_list select_list literal_list
%type <expr_vecs> literal_lists
%type <table_vec> table_ref_commalist
%type <update_vec> update_clause_commalist
%type <column_vec> column_def_commalist
/******************************
 ** Token Precedence and Associativity
 ** Precedence: lowest to highest
 ******************************/
%left		OR
%left		AND
%right		NOT
%right		'=' EQUALS NOTEQUALS LIKE
%nonassoc	'<' '>' LESS GREATER LESSEQ GREATEREQ
%nonassoc	NOTNULL
%nonassoc	ISNULL
%nonassoc	IS				/* sets precedence for IS NULL, etc */
%left		'+' '-'
%left		'*' '/' '%'
%left		'^'
/* Unary Operators */
%right 		UMINUS
%left		'[' ']'
%left		'(' ')'
%left		'.'
%%
input:
		statement_list opt_semicolon {
			*result = $1;
		}
	;
statement_list:
		statement { $$ = new AST($1); }
	|	statement_list ';' statement { $1->addStatement($3); $$ = $1; }
	;
statement:
		create_database_statement { $$ = $1; }
	|	drop_database_statement { $$ = $1; }
	|	show_databases_statement { $$ = $1; }
	|	use_statement { $$ = $1; }
	|	show_tables_statement { $$ = $1; }
	|	desc_statement { $$ = $1; }
	|	create_index_statement { $$ = $1; }
	|	drop_index_statement { $$ = $1; }
	|	select_statement { $$ = $1; }
	|	create_statement { $$ = $1; }
	|	insert_statement { $$ = $1; }
	|	delete_statement { $$ = $1; }
	|	update_statement { $$ = $1; }
	|	drop_statement { $$ = $1; }
	;
create_database_statement:
		CREATE DATABASE IDENTIFIER {
			$$ = new CreateDatabaseStatement();
			$$->databaseName = $3;
		}
drop_database_statement:
		DROP DATABASE IDENTIFIER {
			$$ = new DropDatabaseStatement();
			$$->databaseName = $3;
		}
show_databases_statement:
		SHOW DATABASES { 
			$$ = new ShowDatabasesStatement();
		}
use_statement:
		USE IDENTIFIER{
			$$ = new UseStatement();
			$$->databaseName = $2;
		}
show_tables_statement:
		SHOW TABLES {
			$$ = new ShowTablesStatement();
		}
desc_statement:
		DESC IDENTIFIER{
			$$ = new DescStatement();
			$$->tableName = $2;
		}
create_index_statement:
		CREATE INDEX IDENTIFIER '(' IDENTIFIER ')' {
			$$ = new CreateIndexStatement();
			$$->tableName = $3;
			$$->columnName = $5;
		}
drop_index_statement:
		DROP INDEX IDENTIFIER '(' IDENTIFIER ')' {
			$$ = new DropIndexStatement();
			$$->tableName = $3;
			$$->columnName = $5;
		}
create_statement:
		CREATE TABLE table_name '(' column_def_commalist ')' {
			$$ = new CreateStatement();
			$$->tableName = $3;
			$$->columns = $5;
		}
	;
column_def_commalist:
		column_def { $$ = new std::vector<ColumnDefinition*>(); $$->push_back($1); }
	|	column_def_commalist ',' column_def { $1->push_back($3); $$ = $1; }
	;
column_def:
		IDENTIFIER column_type '(' INTVAL ')' opt_not_null {
			$$ = new ColumnDefinition($1, (DataType) $2);
			$$->length = $4;
			$$->notnull = $6;
		}
	|	PRIMARY KEY '(' IDENTIFIER ')' {
			$$ = new ColumnDefinition($4, (DataType)DTPRIMARY);
		}
	;
opt_not_null:
		NOT NULL{ $$ = true; }
	|	{ $$ = false; }
column_type:
		INT { $$ = DTINT; }
	|	CHAR { $$ = DTCHAR; }
	|	VARCHAR { $$ = DTVARCHAR; }
	|	DATE { $$ = DTDATE; }
	|	DECIMAL { $$ = DTDECIMAL; }
	;
drop_statement:
		DROP TABLE table_name {
			$$ = new DropStatement();
			$$->name = $3;
		}
	;
delete_statement:
		DELETE FROM table_name opt_where {
			$$ = new DeleteStatement();
			$$->tableName = $3;
			$$->expr = $4;
		}
	;
insert_statement:
		INSERT INTO table_name VALUES literal_lists {
			$$ = new InsertStatement();
			$$->tableName = $3;
			$$->valuelists = $5;
		}
	;
literal_lists:
		'(' literal_list ')' {
			$$ = new std::vector<std::vector<Expr*>*>();
			$$->push_back($2);
		}
	|	literal_lists ',' '(' literal_list ')' {
			$$->push_back($4);
		}
update_statement:
	UPDATE table_ref_name SET update_clause_commalist opt_where {
		$$ = new UpdateStatement();
		$$->table = $2;
		$$->updates = $4;
		$$->where = $5;
	}
	;
update_clause_commalist:
		update_clause { $$ = new std::vector<UpdateClause*>(); $$->push_back($1); }
	|	update_clause_commalist ',' update_clause { $1->push_back($3); $$ = $1; }
	;
update_clause:
		IDENTIFIER '=' literal {
			$$ = new UpdateClause();
			$$->column = $1;
			$$->value = $3;
		}
	;
select_statement:
		SELECT opt_distinct select_list from_clause opt_where opt_group{
			$$ = new SelectStatement();
			$$->selectDistinct = $2;
			$$->selectList = $3;
			$$->fromTable = $4;
			$$->whereClause = $5;
			$$->groupBy = $6;
		}
	;
opt_distinct:
		DISTINCT { $$ = true; }
	|	/* empty */ { $$ = false; }
	;
select_list:
		expr_list
	;
from_clause:
		FROM table_ref { $$ = $2; }
	;
opt_where:
		WHERE expr { $$ = $2; }
	|	/* empty */ { $$ = NULL; }
	;
opt_group:
		GROUP BY expr_list opt_having {
			$$ = new GroupByDescription();
			$$->columns = $3;
			$$->having = $4;
		}
	|	/* empty */ { $$ = NULL; }
	;
opt_having:
		HAVING expr { $$ = $2; }
	|	/* empty */ { $$ = NULL; }
	;
expr_list:
		expr { $$ = new std::vector<Expr*>(); $$->push_back($1); }
	|	expr_list ',' expr { $1->push_back($3); $$ = $1; }
	;
literal_list:
		literal { $$ = new std::vector<Expr*>(); $$->push_back($1); }
	|	literal_list ',' literal { $1->push_back($3); $$ = $1; }
	;
expr:
		'(' expr ')' { $$ = $2; }
	|	scalar_expr
	|	unary_expr
	|	binary_expr
	|	function_expr
	;
scalar_expr:
		column_name
	|	star_expr
	|	literal
	;
unary_expr:
		expr IS NULL { $$ = Expr::makeOpUnary(Expr::ISNULL, $1); }
	|	expr IS NOT NULL { $$ = Expr::makeOpUnary(Expr::ISNOTNULL, $1); }
	;
binary_expr:
		expr '=' expr		{ $$ = Expr::makeOpBinary($1, Expr::EQ, $3); }
	|	expr NOTEQUALS expr	{ $$ = Expr::makeOpBinary($1, Expr::NE, $3); }
	|	expr '<' expr		{ $$ = Expr::makeOpBinary($1, Expr::LT, $3); }
	|	expr '>' expr		{ $$ = Expr::makeOpBinary($1, Expr::GT, $3); }
	|	expr LESSEQ expr	{ $$ = Expr::makeOpBinary($1, Expr::LE, $3); }
	|	expr GREATEREQ expr	{ $$ = Expr::makeOpBinary($1, Expr::GE, $3); }
	|	expr AND expr		{ $$ = Expr::makeOpBinary($1, Expr::AND, $3); }
	|	expr LIKE expr		{ $$ = Expr::makeOpBinary($1, Expr::LIKE, $3); }
	;
function_expr:
		IDENTIFIER '(' opt_distinct expr ')' { $$ = Expr::makeFunctionRef($1, $4, $3); }
	;
column_name:
		IDENTIFIER { $$ = Expr::makeColumnRef($1); }
	|	IDENTIFIER '.' IDENTIFIER { $$ = Expr::makeColumnRef($1, $3); }
	;
literal:
		string_literal
	|	num_literal
	|	NULL { $$ = NULL; }
	;
string_literal:
		STRING { $$ = Expr::makeLiteral($1); }
	;
num_literal:
		FLOATVAL { $$ = Expr::makeLiteral($1); }
	|	int_literal
	;
int_literal:
		INTVAL { $$ = Expr::makeLiteral($1); }
	;
star_expr:
		'*' { $$ = new Expr(kExprStar); }
	;
table_ref:
		table_ref_name
	|	table_ref_name ',' table_ref_commalist {
			$3->push_back($1);
			auto tbl = new TableRef(kTableCrossProduct);
			tbl->list = $3;
			$$ = tbl;
		}
	;
table_ref_commalist:
		table_ref_name { $$ = new std::vector<TableRef*>(); $$->push_back($1); }
	|	table_ref_commalist ',' table_ref_name { $1->push_back($3); $$ = $1; }
	;
table_ref_name:
		table_name {
			$$ = new TableRef(kTableName);
			$$->name = $1;
		}
	;
table_name:
		IDENTIFIER
	|	IDENTIFIER '.' IDENTIFIER
	;
opt_semicolon:
		';'
	|	/* empty */
	;
%%
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