%{
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <string.h>
#include "y.tab.h"
#include "ts_funciones.h"

FILE  *yyin;
char *yytext;
void yyerror(char *msg);
//-----------------------Funciones para validacion de tipos
int validarInt(char entero[]);
int validarFloat(char flotante[]);
int validarString(char cadena[]);
//-----------------------Fin

%}

%union{
 char s[20];
}

%token DEFVAR
%token ENDDEF

%token <s>ID
%token MAYOR MAYOR_IGUAL MENOR MENOR_IGUAL IGUAL DISTINTO   
%token ASIG SUMA RESTA MUL DIV
%token P_A P_C LL_A LL_C PA_A PA_C P_Y_C DP COMA

%token ENTERO
%token REAL
%token CADENA
%token INT
%token FLOAT
%token STRING

%token AND
%token OR

%token IF
%token ELSE
%token WHILE
%token AVG

%%
programa
		:sentencias																{printf("COMPILACION OK\n");}
		;								
sentencias								
			:sentencias sent													{printf("GRUPO DE SENTENCIAS\n");}
			|sent																{printf("SENTENCIA INDIVIDUAL\n");}
			;								
sent								
	:asignacion																	{printf("ASIG\n");}
	|decision																	{printf("DESI\n");}
	|declaracion																{printf("DECLA\n");}
	|iteracion																	{printf("ITERA\n");}
	|promedio																	{printf("AVG\n");}
	;

asignacion
			:ID ASIG formato_asignacion
			;
formato_asignacion
					:expresion													{printf("ASIGNACION SIMPLE\n");}
					|ID ASIG formato_asignacion									{printf("ASIGNACION MULTIPLE\n");}
					;
decision
		:IF P_A condiciones P_C LL_A sentencias LL_C							{printf("IF\n");}
		|IF P_A condiciones P_C LL_A sentencias LL_C ELSE LL_A sentencias LL_C	{printf("IF-ELSE\n");}
		;
		
declaracion
			:DEFVAR declaraciones ENDDEF										{printf("BLOQUE DECLARACION\n");}
			;	
declaraciones
				:declaraciones formato_declaracion
				|formato_declaracion
				;
formato_declaracion
					:ID DP tipo_dato											{printf("DECLARACION SIMPLE\n");}	
					|ID COMA formato_declaracion								{printf("DECLARACION MULTIPLE\n");}
					;
tipo_dato
		:INT																	{printf("INT\n");}
		|FLOAT																	{printf("FLOAT\n");}
		|STRING																	{printf("STRING\n");}
		;			
		
iteracion							
			:WHILE P_A condiciones P_C LL_A sentencias LL_C 					{printf("WHILE\n");}
			;						
condiciones									
			:condiciones operador condicion										{printf("Condiciones Multiples\n");}
			|condicion															{printf("Condicion Individual\n");}
			;									
operador									
		:AND																	{printf("&&\n");}
		|OR																		{printf("||\n");}
		;									
condicion									
		:expresion MAYOR expresion												{printf(">\n", yytext);}
		|expresion MAYOR_IGUAL expresion										{printf(">=\n", yytext);}
		|expresion MENOR expresion												{printf("<\n", yytext);}
		|expresion MENOR_IGUAL  expresion										{printf("<=\n", yytext);}
		|expresion IGUAL  expresion												{printf("==\n", yytext);}
		|expresion DISTINTO  expresion											{printf("!=\n", yytext);}
		;									

promedio
		:AVG P_A PA_A formato_promedio PA_C P_C									{printf("FUNCION PROMEDIO (AVG)\n");}
		;
formato_promedio
				:expresion
				|expresion COMA formato_promedio
				;
expresion									
		:expresion RESTA termino												{printf("RESTA\n");}
		|expresion SUMA termino													{printf("SUMA\n");}
		|termino																{printf("TERMINO\n");}
		;									

termino									
		:termino MUL factor														{printf("MULTIPLICACION\n");}
		|termino DIV factor														{printf("DIVISION\n");}
		|factor																	{printf("FACTOR\n");}
		;									

factor									
		:ID																		{printf("ID\n");}
		|tipo																	{printf("CTE\n");}
		|P_A expresion P_C														{printf("EXPRESION\n");}
		;									

tipo									
	:ENTERO																		{validarInt(yytext);printf("ENTERO\n");}
	|REAL																		{validarFloat(yytext);printf("FLOAT\n");}
	|CADENA																		{validarString(yytext);printf("CADENA\n");}
	;

%%

int main(int argc,char *argv[])
{
  if ((yyin = fopen(argv[1], "rt")) == NULL)
  {
	printf("\nNo se puede abrir el archivo: %s\n", argv[1]);
  }
  else
  {
	yyparse();
  }
  fclose(yyin);
  return 0;
}

void yyerror(char *msg){
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

//-----------------------Funciones para validacion de tipos-----------------------//
int validarInt(char entero[]) {
    int casteado = atoi(entero);
    char msg[100];
    if(casteado < -32768 || casteado > 32767) {
        sprintf(msg, "ERROR: Entero %d fuera de rango. Debe estar entre [-32768; 32767]\n", casteado);
        yyerror(msg);
    } else {
        //guardarenTS
        //saveSymbol(entero,"cFloat", NULL);
        //insertarTipo("cFloat");

        //printf solo para pruebas:
        //printf("Entero ok! %d \n", casteado);
        return 0;
    }
}

int validarFloat(char flotante[]) {
    double casteado = atof(flotante);
    casteado = fabs(casteado);
    char msg[300];
    if(casteado < pow(-1.17549,-38) || casteado >  pow(3.40282,38)){
    //if(casteado < 2 || casteado >  10){
        sprintf(msg, "ERROR: Float %f fuera de rango. Debe estar entre [1.17549e-38; 3.40282e38]\n", casteado);
        yyerror(msg);
    } 
    else {
        //saveSymbol(flotante,"cFloat", NULL);
        //insertarTipo("cFloat");
        //guardarenTS
        //printf solo para pruebas:
        //printf("Float ok! %f \n", casteado);
        return 0;
    }
}

int validarString(char cadena[]) {
    char msg[100];
    int longitud = strlen(cadena);

    if( strlen(cadena) > 32){ //en lugar de 30 verifica con 32 porque el string viene entre comillas
        sprintf(msg, "ERROR: Cadena %s demasiado larga. Maximo 30 caracteres\n", cadena);
        yyerror(msg);
    }
    char sincomillas[31];
    int i;
    for(i=0; i< longitud - 2 ; i++) {
            sincomillas[i]=cadena[i+1];
    }
    sincomillas[i]='\0';
    //guardarenTS();
    //saveSymbol(sincomillas,"cString", NULL);
    //insertarTipo("string");
    //reemplazarBlancos(sincomillas);
    //apilarPolaca(sincomillas);
    /*
    // Bloque para debug
    printf("***************************\n");
    printf("%d\n",strlen(sincomillas));
    for ( i = 0; i < strlen(sincomillas)+1; i++) {
        printf("%d : %c , %d \n",i,sincomillas[i],sincomillas[i]);
    }
    printf("***************************\n");
    */

    //guardarenTS;
    return 0;
}

//-----------------------Fin de funciones para validacion-----------------------//
