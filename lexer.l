%{
#include "parser.h"
#include <stdio.h>
#define TOKEN(name) { return SQL_##name; }
char* substr(const char* source, int from, int to) {
    int len = to-from;
    char* copy = new char[len+1];
    strncpy(copy, source+from, len);
    copy[len] = '\0';
    return copy;
}
%}
%option header-file="lexer.h"
%option outfile="lexer.cpp"
%option reentrant
%option bison-bridge
%option never-interactive
%option batch
%option noyywrap
%option nounput
%option warn
%option case-insensitive
%option prefix="sql_"
%option bison-locations 
%s COMMENT
%%
--				BEGIN(COMMENT);
<COMMENT>[^\n]*	/* skipping comment content until a end of line is read */;
<COMMENT>\n 	BEGIN(INITIAL);
[ \t\n]+	/* skip whitespace */;
DATABASES	TOKEN(DATABASES)
DISTINCT	TOKEN(DISTINCT)
DATABASE	TOKEN(DATABASE)
BETWEEN		TOKEN(BETWEEN)
CASCADE		TOKEN(CASCADE)
COLUMNS		TOKEN(COLUMNS)
CONTROL		TOKEN(CONTROL)
DECIMAL		TOKEN(DECIMAL)
DEFAULT		TOKEN(DEFAULT)
EXPLAIN		TOKEN(EXPLAIN)
HISTORY		TOKEN(HISTORY)
INTEGER		TOKEN(INTEGER)
NATURAL		TOKEN(NATURAL)
PRIMARY		TOKEN(PRIMARY)
SCHEMAS		TOKEN(SCHEMAS)
SPATIAL		TOKEN(SPATIAL)
VIRTUAL		TOKEN(VIRTUAL)
VARCHAR		TOKEN(VARCHAR)
BEFORE		TOKEN(BEFORE)
COLUMN		TOKEN(COLUMN)
CREATE		TOKEN(CREATE)
DELETE		TOKEN(DELETE)
DIRECT		TOKEN(DIRECT)
DOUBLE		TOKEN(DOUBLE)
ESCAPE		TOKEN(ESCAPE)
EXCEPT		TOKEN(EXCEPT)
EXISTS		TOKEN(EXISTS)
GLOBAL		TOKEN(GLOBAL)
HAVING		TOKEN(HAVING)
INSERT		TOKEN(INSERT)
ISNULL		TOKEN(ISNULL)
OFFSET		TOKEN(OFFSET)
RENAME		TOKEN(RENAME)
SCHEMA		TOKEN(SCHEMA)
SELECT		TOKEN(SELECT)
SORTED		TOKEN(SORTED)
TABLES		TOKEN(TABLES)
UNIQUE		TOKEN(UNIQUE)
UNLOAD		TOKEN(UNLOAD)
UPDATE		TOKEN(UPDATE)
VALUES		TOKEN(VALUES)
AFTER		TOKEN(AFTER)
ALTER		TOKEN(ALTER)
CROSS		TOKEN(CROSS)
DELTA		TOKEN(DELTA)
GROUP		TOKEN(GROUP)
INDEX		TOKEN(INDEX)
INNER		TOKEN(INNER)
LOCAL		TOKEN(LOCAL)
MERGE		TOKEN(MERGE)
MINUS		TOKEN(MINUS)
ORDER		TOKEN(ORDER)
OUTER		TOKEN(OUTER)
RIGHT		TOKEN(RIGHT)
TABLE		TOKEN(TABLE)
UNION		TOKEN(UNION)
USING		TOKEN(USING)
WHERE		TOKEN(WHERE)
CHECK		TOKEN(CHECK)
CALL		TOKEN(CALL)
DATE		TOKEN(DATE)
DESC		TOKEN(DESC)
DROP		TOKEN(DROP)
FILE		TOKEN(FILE)
FROM		TOKEN(FROM)
FULL		TOKEN(FULL)
HASH		TOKEN(HASH)
HINT		TOKEN(HINT)
INTO		TOKEN(INTO)
JOIN		TOKEN(JOIN)
LEFT		TOKEN(LEFT)
LIKE		TOKEN(LIKE)
LOAD		TOKEN(LOAD)
NULL		TOKEN(NULL)
PART		TOKEN(PART)
PLAN		TOKEN(PLAN)
SHOW		TOKEN(SHOW)
TEXT		TOKEN(TEXT)
TIME		TOKEN(TIME)
VIEW		TOKEN(VIEW)
WITH		TOKEN(WITH)
CHAR		TOKEN(CHAR)
ADD			TOKEN(ADD)
ALL			TOKEN(ALL)
AND			TOKEN(AND)
ASC			TOKEN(ASC)
CSV			TOKEN(CSV)
FOR			TOKEN(FOR)
INT			TOKEN(INT)
KEY			TOKEN(KEY)
NOT			TOKEN(NOT)
OFF			TOKEN(OFF)
SET			TOKEN(SET)
TBL			TOKEN(TBL)
TOP			TOKEN(TOP)
USE			TOKEN(USE)
AS			TOKEN(AS)
BY			TOKEN(BY)
IF			TOKEN(IF)
IN			TOKEN(IN)
IS			TOKEN(IS)
OF			TOKEN(OF)
ON			TOKEN(ON)
OR			TOKEN(OR)
TO			TOKEN(TO)
"<>" 		TOKEN(NOTEQUALS)
"<="		TOKEN(LESSEQ)
">="		TOKEN(GREATEREQ)
[-+*/(){},.;<>=^%:?]	{ return yytext[0]; }
[0-9]+"."[0-9]* |
"."[0-9]*	{
	yylval->fval = atof(yytext);
	return SQL_FLOATVAL;
}
[0-9]+	{
	yylval->ival = atol(yytext);
	return SQL_INTVAL;
}
\"[^\"\n]+\" {
	// Crop the leading and trailing quote char
	yylval->sval = substr(yytext, 1, strlen(yytext)-1);
	return SQL_IDENTIFIER;
}
[A-Za-z][A-Za-z0-9_]* {
	yylval->sval = _strdup(yytext);
	return SQL_IDENTIFIER;
}
'[^'\n]*' {
	// Crop the leading and trailing quote char
	yylval->sval = substr(yytext, 1, strlen(yytext)-1);
	return SQL_STRING;
}
. { fprintf(stderr, "[SQL-Lexer-Error] Unknown Character: %c\n", yytext[0]); return 0; }
%%
int yyerror(const char *msg) {
    fprintf(stderr, "[SQL-Lexer-Error] %s\n",msg); return 0;
}