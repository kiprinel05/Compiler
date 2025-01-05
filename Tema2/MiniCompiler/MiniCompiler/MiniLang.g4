grammar MiniLang;

// Programul începe de la regula `program`
program: statement* EOF;

// Declarații și expresii
statement:
	varDeclaration
	| assignment
	| ifStatement
	| whileStatement
	| forStatement
	| returnStatement
	| expression ';';

// Declarații
varDeclaration: type IDENTIFIER ( '=' expression)? ';';

// Atribuire
assignment: IDENTIFIER '=' expression ';';

// Structuri de control
ifStatement:
	'if' '(' expression ')' '{' statement* '}' (
		'else' '{' statement* '}'
	)?;

whileStatement: 'while' '(' expression ')' '{' statement* '}';

forStatement:
	'for' '(' (varDeclaration | assignment)? ';' expression? ';' (
		assignment
	)? ')' '{' statement* '}';

returnStatement: 'return' expression? ';';

// Expresii
expression:
	expression ('||' expression)									# LogicalOr
	| expression ('&&' expression)									# LogicalAnd
	| expression ('==' | '!=' | '<' | '<=' | '>' | '>=') expression	# Relational
	| expression ('+' | '-') expression								# Additive
	| expression ('*' | '/' | '%') expression						# Multiplicative
	| ('!' | '++' | '--') expression								# Unary
	| '(' expression ')'											# Parens
	| IDENTIFIER													# Identifier
	| NUMBER														# Number
	| STRING														# String;

// Tipuri
type: 'int' | 'float' | 'double' | 'string' | 'void';

// Token-uri pentru identificatori, literali și altele
IDENTIFIER: [a-zA-Z_][a-zA-Z0-9_]*;
NUMBER: [0-9]+ ('.' [0-9]+)?;
STRING: '"' (~["\\] | '\\' .)* '"';

// Operatori și delimitatori
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

// Spații albe și comentarii
WS: [ \t\r\n]+ -> skip;
LINE_COMMENT: '//' ~[\r\n]* -> skip;
BLOCK_COMMENT: '/*' .*? '*/' -> skip;