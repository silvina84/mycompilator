typedef struct terceto{
	char operacion[35];
	char t1 [30] , t2[30]  ;
	int numeroTerceto;
} t_terceto;

struct node
{
    char data[100];
    struct node *link;
};

#define OPERADOR_AND 301
#define OPERADOR_OR 302

#define ERROR_MEMORIA -116
#define ERROR_STACK_VACIA -113

#define STACK_EXPRESION 208
#define STACK_FACTOR 209
#define STACK_CONDICION 200
#define STACK_OPERADOR 201
#define STACK_POSICION 202
#define STACK_FACTORIAL 203
#define STACK_AND 204
#define STACK_OR 205
#define STACK_COND_ASSEM 206
#define STACK_WHILE 207
#define STACK_IF 208
#define STACK_COMBINATORIA 209

t_terceto aux_condicion_if_terceto,
aux_expresion_logica_terceto,
aux_comparacion_terceto,
aux_cond_if_operador_logico;

t_terceto aux_condicion_while_terceto,
aux_expresion_logica_terceto,
aux_comparacion_terceto,
aux_cond_while_operador_logico;

struct node *stackCondicion = NULL, *stackOperador = NULL,*stackPosicion = NULL, *stackAnd=NULL,*stackOr=NULL, *stackCondAssem=NULL,*stackWhile=NULL,*stackIf=NULL;
//estructura del terceto operacion, tiene los valores de los numeros de tercetos que se estan usando
typedef struct tercetoOperacion{
	int expresion, 
		termino, 
		factor;
} t_operacion;

char aux4[40];
char aux5[40];

int indiceTerceto = 0;			// Cantidad de elementos e indice de tercetos
t_terceto *vectorTercetos;
t_terceto 	factor_terceto;
t_terceto auxExpresion_terceto;
t_terceto auxTermino_terceto;
#define TERCETOS "intermedia.txt"
t_terceto *vectorTercetosAlias;
t_terceto expresion_terceto, termino_terceto, expresion_terceto_aux, concatenacion_terceto, factor_terceto_aux;


//Busca un terceto en el vector de tercetos
t_terceto* buscarTerceto(t_terceto t){
	int i;
	for (i = 0; i < indiceTerceto; ++i) 
		if(strcmp(vectorTercetos[i].operacion, t.operacion) == 0 && strcmp(vectorTercetos[i].t1, "-1")==0 && strcmp(vectorTercetos[i].t2,"-1")==0){
			return &(vectorTercetos[i]); 
			}
	return NULL;
}

//Crea el terceto con los indices de los tercetos. Si no existen tiene -1
t_terceto crearTerceto(char* operacion,char* t1,char* t2){

//printf("Terceto: %s-%s-%s\n",operacion,t1,t2);
	t_terceto result;
	t_terceto *tmp;
	strcpy(result.operacion, operacion);
	
	strcpy(result.t1,t1);   // En vez de sprintf
	strcpy(result.t2,t2);   // En vez del los sprintf
	
	t_terceto *aux = buscarTerceto(result);
	if(indiceTerceto > 0 && aux != NULL)
		result = *aux;
	else{
		result.numeroTerceto = indiceTerceto++;
		vectorTercetos = (t_terceto*) realloc(vectorTercetos, sizeof(t_terceto) * indiceTerceto);
		if(vectorTercetos == NULL)
			printf("realloc del vectorTercetos en crearTerceto()");
		vectorTercetos[indiceTerceto-1] = result;
	}
	return result;
}

//Escribe un terceto en el archivo
void escribirTerceto(t_terceto t, FILE *arch){	

	
	if(strcmp(t.operacion, "BI")==0) //salto no condicional
		fprintf(arch, "[%d] (%s, [%d], -)\n", t.numeroTerceto, t.operacion, t.t2);
	else if(strcmp(t.t1, "-1")==0 && strcmp(t.t2 ,"-1")==0) //terceto de asignacion de memoria
		fprintf(arch, "[%d] (%s, _, _)\n", t.numeroTerceto, t.operacion);
	else if(strcmp(t.t2 , "-1")==0) //terceto en el caso cuando se escriben los cmp 
		fprintf(arch, "[%d] (%s, %s, _)\n", t.numeroTerceto, t.operacion, t.t1);
	else //terceto completo sin problemas
		fprintf(arch, "[%d] (%s, %s, %s)\n", t.numeroTerceto, t.operacion, t.t1, t.t2);	
}


//Escribe todos los Tercetos
void escribirArchivoTercetos(){
	int i;
	FILE* arch = fopen(TERCETOS, "w+");
	for(i = 0; i < indiceTerceto; i++)
		{
		 if(vectorTercetos[i].numeroTerceto==75)
			strcpy(vectorTercetos[i].t1,"main_else_1");
		 escribirTerceto(vectorTercetos[i], arch);		
		 
		}
	t_terceto t = crearTerceto("fin","-1","-1");
	escribirTerceto(t, arch);
	fclose(arch);
}

// to insert elements in stack
void push(int destino, char* i)
{
    struct node *temp;
    temp = (struct node*)malloc(sizeof(struct node));
    //if(temp == NULL)
	//	mostrarError(ERROR_MEMORIA, "malloc de temp en push())");
    strcpy(temp->data , i);
    switch(destino){
    	case STACK_CONDICION:
	    	temp->link = stackCondicion;
	    	stackCondicion = temp;	
    	break;
    	case STACK_OPERADOR:
	    	temp->link = stackOperador;
	    	stackOperador = temp;
    	break;
		case STACK_POSICION:
	    	temp->link = stackPosicion;
	    	stackPosicion = temp;
		break;
		case STACK_AND:
	    	temp->link = stackAnd;
	    	stackAnd = temp;
		break;
		case STACK_OR:
	    	temp->link = stackOr;
	    	stackOr = temp;
		break;
		
		case STACK_COND_ASSEM:
	    	temp->link = stackCondAssem;
	    	stackCondAssem = temp;
		break;
		
	case STACK_WHILE:
	    	temp->link = stackWhile;
	    	stackWhile = temp;
		break;
	case STACK_IF:
	    	temp->link = stackIf;
	    	stackIf = temp;
		break;
		
    }
}



// to delete elements from stack
int pop(int destino )
{
	int result;
    struct node *temp;
    switch(destino){
    	case STACK_CONDICION:
	    	//if (stackCondicion == NULL)
			//	mostrarError(ERROR_STACK_VACIA, "stackCondicion");
		    //else
		    //{
		        temp = stackCondicion;
		        stackCondicion = stackCondicion->link;
		    //}	
    	break;
		case STACK_AND:
					//if (stackAnd == NULL)
					//	mostrarError(ERROR_STACK_VACIA, "stackAnd");
					//else
					//{
						temp = stackAnd;
						stackAnd = stackAnd->link;
					//}	
				break;
    	case STACK_OPERADOR:
	    	//if (stackOperador == NULL)
		    //    mostrarError(ERROR_STACK_VACIA, "stackOperador");
		    //else
		    //{
		        temp = stackOperador;
		        stackOperador = stackOperador->link;
		    //}
	    	break;
		case STACK_POSICION:
	    	//if (stackPosicion == NULL)
			//	mostrarError(ERROR_STACK_VACIA, "stackPosicion");
		    //else
		    //{
		        temp = stackPosicion;
		        stackPosicion = stackPosicion->link;
		    //}
	    	break;
		case STACK_COND_ASSEM:
	    	//if (stackCondAssem == NULL)
			//	mostrarError(ERROR_STACK_VACIA, "stackCondAssem");
		    //else
		    //{
		        temp = stackCondAssem;
		        stackCondAssem = stackCondAssem->link;
		    //}	
    	break;
		
		case STACK_WHILE:
	    	//if (stackWhile == NULL)
			//	mostrarError(ERROR_STACK_VACIA, "stackWhile");
		    //else
		    //{
		        temp = stackWhile;
		        stackWhile = stackWhile->link;
		    //}	
    	break;
		case STACK_IF:
	    	//if (stackIf == NULL)
			//	mostrarError(ERROR_STACK_VACIA, "stackIf");
		    //else
		    //{
		        temp = stackIf;
		        stackIf = stackIf->link;
		    //}	
    	break;
		
    }
    result = atoi(temp->data);
    free(temp);
	return result;
}