/*
Projet compilation - Thibaut B.
*/
%{
#include "svg.tab.hpp"
#include <string>
#include <iostream>
#include <algorithm>
#include <stdlib.h>

%}

blancs    		[ \t]+

debutcomment 	\/\*
midcomment 		(\*([^\/])|[^*])*
fincomment 		\*\/

chiffre   		[0-9]
entier    		{chiffre}+
exposant  		[eE][+-]?{entier}
reel    		{entier}("."{entier})?{exposant}?

string 			\"([^"]|[\n]|(\\\"))*\"

/* Aspect lexical */
variable 		[a-zA-Z]+[a-zA-Z0-9]*
constante_num	{entier}|{reel}
constante_str	{string}
/* separateur directement en dur */
/* operateur directement en dur */
commentaire 	{debutcomment}{midcomment}{fincomment}|%.*$
rgb 			RGB\({entier},{entier},{entier}\)
xy_point 		\({entier},{entier}\)

%%

{blancs}|°  { /* On ignore */ }

"fin" 	return(END);

(?i:canvas) { yylval.str=(char*)"canvas"; return(CANVAS); }
(?i:description) { yylval.str=(char*)"description"; return(DESCRIPTION); }
(?i:title) { yylval.str=(char*)"title"; return(TITLE); }
(?i:write) { yylval.str=(char*)"write"; return(WRITE); }

{xy_point} {
	/* (640,480) ->	640,480) -> 480) */
	yylval.point.x = atoi(yytext+1);
	char *y = strchr(yytext, ',');
	int idx_y = (int)(y-yytext)+1;
	yylval.point.y = atoi(yytext+idx_y);
	return(XY_POINT);
}

{rgb} {
	/* Parce que RGB en maj dans le svg n'est pas pris en compte */
	yytext[0] = 'r';
	yytext[1] = 'g';
	yytext[2] = 'b';
	yylval.const_str = yytext;
	return(RGB_CODE);
}

"RECT" return(RECT);
"LIGNE" return(LIGNE);
"CERCLE" return(CERCLE);
"TEXTE" return(TEXTE);

"remplissage" { yylval.const_str = (const char*)"fill"; return(ATTRIBUT); }
"couleur" { yylval.const_str = (const char*)"stroke"; return(ATTRIBUT); }
"rotation" { yylval.const_str = (const char*)"rotate"; return(ATTRIBUT); }
"translation" { yylval.const_str = (const char*)"translate"; return(ATTRIBUT); }
"scale" { yylval.const_str = (const char*)"scale"; return(ATTRIBUT); }

{variable}	{
	/* Si je ne fait pas ça j'ai des problèmes de taille en passant dans le ypp
	à défaut de solutions, je fais comme ça. */
	yylval.str = new char[strlen(yytext)];
	strcpy(yylval.str, yytext);
	return(VARIABLE);
}
{constante_num}	{ yylval.valeur_numerique = atof(yytext); return(CONSTANTE_NUM); }
{constante_str}	{
	/* Erase quotes */
	std::string s = yytext;
	s = s.substr(1, s.size() - 2);
	strcpy(yytext, s.c_str());
	yylval.const_str = yytext;
	return(CONSTANTE_STR);
}

","		return(COMMA);
"."		return(DOT);
";"		return(DOT_COMMA);
"("		return(PARENTHESE_GAUCHE);
")"		return(PARENTHESE_DROITE);
"["		return(CROCHET_GAUCHE);
"]" 	return(CROCHET_DROITE);
"{"		return(ACCOLADE_GAUCHE);
"}"		return(ACCOLADE_DROITE);

"+"		return(PLUS);
"-"		return(MOINS);
"="		return(EGAL);
"*"		return(FOIS);
"/"		return(DIVISE);

"<"		return(LE);
"<="	return(LEQ);
">"		return(GE);
">="	return(GEQ);
"=="	return(EQ);
"!="	return(NEQ);


"\n"  return(FIN);
