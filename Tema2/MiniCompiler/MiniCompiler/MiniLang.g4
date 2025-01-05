grammar MiniLang;

program: function* EOF;

function: type IDENTIFIER '(' parameterList? ')' block;

parameterList: parameter (',' parameter)*;
parameter: type IDENTIFIER;

block: '{' statement* '}';

statement:
	varDeclaration
	| assignment
	| ifStatement
	| whileStatement
	| forStatement
	| returnStatement
	| expression ';';

varDeclaration: type IDENTIFIER ( '=' expression)? ';';

assignment: IDENTIFIER '=' expression ';';

ifStatement: 'if' '(' expression ')' block ('else' block)?;

whileStatement: 'while' '(' expression ')' block;

forStatement:
	'for' '(' (varDeclaration | assignment)? ';' expression? ';' assignment? ')' block;

returnStatement: 'return' expression? ';';

expression:
	expression ('||' expression)									# LogicalOr
	| expression ('&&' expression)									# LogicalAnd
	| expression ('==' | '!=' | '<' | '<=' | '>' | '>=') expression	# Relational
	| expression ('+' | '-') expression								# Additive
	| expression ('*' | '/' | '%') expression						# Multiplicative
	| ('!' | '++' | '--') expression								# Unary
	| '(' expression ')'											# Parens
	| IDENTIFIER '(' (expression (',' expression)*)? ')'			# FunctionCall
	| IDENTIFIER													# Identifier
	| NUMBER														# Number
	| STRING														# String;

type: 'int' | 'float' | 'double' | 'string' | 'void';

IDENTIFIER: [a-zA-Z_][a-zA-Z0-9_]*;
NUMBER: [0-9]+ ('.' [0-9]+)?;
STRING: '"' (~["\\] | '\\' .)* '"';

ADD: '+';
SUB: '-';
MUL: '*';
DIV: '/';
MOD: '%';
INCREMENT: '++';
DECREMENT: '--';
ASSIGN: '=';
ADD_ASSIGN: '+=';
SUB_ASSIGN: '-=';
MUL_ASSIGN: '*=';
DIV_ASSIGN: '/=';
MOD_ASSIGN: '%=';

LT: '<';
LE: '<=';
GT: '>';
GE: '>=';
EQ: '==';
NEQ: '!=';

AND: '&&';
OR: '||';
NOT: '!';

LPAREN: '(';
RPAREN: ')';
LBRACE: '{';
RBRACE: '}';
COMMA: ',';
SEMI: ';';

WS: [ \t\r\n]+ -> skip;
LINE_COMMENT: '//' ~[\r\n]* -> skip;
BLOCK_COMMENT: '/*' .*? '*/' -> skip;