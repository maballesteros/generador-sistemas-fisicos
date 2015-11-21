/*-------------------------------------------------------------------------
   GSFVM_LE - Captura del entorno y definicion del sistema
  -------------------------------------------------------------------------
   Incluye las funciones:
	int    GSFVMf_getentorno( void );
	int    GSFVMf_getsistema( void );
	int    GSFVMf_match( int token_to_match );
	int    GSFVMf_seek_tok( char *str_token );
	int    GSFVMf_lexan( void );
	int    GSFVMf_sentencia( void );
	void   GSFVMf_get_move( void );
	void   GSFVMf_skip_move( void )
	void   GSFVMf_get_ppos( int GSFVMv_partic );
  -------------------------------------------------------------------------*/
   #include "GSFVMinc.h"
   #include <ctype.h>
   #include <math.h>

/*-------------------------------------------------------------------------
   DEFINICIONES
  -------------------------------------------------------------------------
   28-7-1994, Estas definiciones asocian un valor numerico a cada token
  -------------------------------------------------------------------------*/
   #define DEFENTORNO       255
   #define MOVSISTEMA       254
   #define NUM_BODYS        253
   #define NUM_CAMPOS       252
   #define SPC_DIM          251
   #define INC_TEMPORAL     250
   #define CICLOS           249
   #define NUM              248
   #define STEEP            247

/*-------------------------------------------------------------------------
   CABECERAS - Cabeceras de las funciones usadas en este modulo
  -------------------------------------------------------------------------*/
   int    GSFVMf_getentorno( void );
   int    GSFVMf_getsistema( void );
   int    GSFVMf_match( int token_to_match );
   int    GSFVMf_seek_tok( char *str_token );
   int    GSFVMf_lexan( void );
   int    GSFVMf_sentencia( void );
   void   GSFVMf_get_move( void );
   void   GSFVMf_skip_move( void );
   void   GSFVMf_get_ppos( int GSFVMv_partic );


/*-------------------------------------------------------------------------
   VARIABLES - A continuacion defino las variables que se utilizan en
	       este modulo. La decision de no incluirlas en el fichero
	       GSFVMinc.h es de claridad. Solo las funciones presentes
	       en este modulo van a hacer uso de ellas.
  -------------------------------------------------------------------------

   --- long   GSFVMv_ciclo;
       Esta variable contiene el numero del ciclo que se esta procesando.

   --- long   GSFVMv_mciclo;
       Esta variable contiene el numero del ciclo que se mete en memoria.

   --- int    GSFVMv_token_type;
       Esta variable guarda el valor numerico asociado al siguiente token
       en el fichero fuente. Indica si el token es un numero, un keyword,
       unas llaves, etc.

   --- float  GSFVMv_token_val;
       Algunos token requieren de un valor numerico asociado. Es p.e. el
       caso, cuando el token es un numero. En este caso, el valor numerico
       de la cadena que representa al numero es guardado en esta variable

   --- struct GSFVMs_token_struct { ... }
       Esta estructura define una entrada a la tabla de simbolos.

   --- struct GSFVMs_token_struct GSFVMm_token_mat[]

  -------------------------------------------------------------------------*/
   long   GSFVMv_ciclo;
   long   GSFVMv_mciclo;
   int    GSFVMv_token_type;
   float  GSFVMv_token_val;
   struct GSFVMs_token_struct {
	     char *key;
	     int   tok;
	    };
   struct GSFVMs_token_struct GSFVMm_token_mat[] =
	    {
	     "DefEntorno",DEFENTORNO,
	     "MovSistema",MOVSISTEMA,
	     "Ciclos",CICLOS,
	     "IncTemporal",INC_TEMPORAL,
	     "SpaceDimension",SPC_DIM,
	     "NumBodys",NUM_BODYS,
	     "Steep",STEEP,
	     "",0
	    };




/*-------------------------------------------------------------------------
   GSFVMf_getentorno() - Lee del fichero de movimientos las variables y
			 preliminares a los datos de movimiento.
  -------------------------------------------------------------------------
   28-7-1994, El esquema de la funcion es el siguiente:
	      - Dado que es la primera exploracion del fichero se
		busca el primer token
	      - Se comprueba que es la definicion del entorno.
	      - Le debe seguir un '='
	      - Le debe seguir un '{'
	      - Si el siguiente token no es '}', debe ser una sentencia.
		Llamamos a la funcion que gestiona sentencias.
	      - Se comprueba que el token es '}'
	      - Si se produce algun error durante el proceso,retornamos 1
	      - El proceso ha sido correcto.
		Retornamos un 0
  -------------------------------------------------------------------------*/
int    GSFVMf_getentorno( void )
	 {
	   GSFVMv_steep = 1;

	   GSFVMv_token_type = GSFVMf_lexan();
	   printf(" Leyendo entorno del sistema");
	   if( GSFVMf_match( DEFENTORNO ) ) return 1;
	   if( GSFVMf_match( '=' ) ) return 1;
	   if( GSFVMf_match( '{' ) ) return 1;
	   while( GSFVMv_token_type!='}' ) GSFVMf_sentencia();
	   if( GSFVMf_match( '}' ) ) return 1;
	   printf("\r Lectura del entorno OK.    \n");
	   return 0;
	 }




/*-------------------------------------------------------------------------
   GSFVMf_getsistema() - Lee los movimientos del sistema y los guarda
			 en memoria
  -------------------------------------------------------------------------
   28-7-1994, El esquema es el siguiente:
	      - Se comprueba que es la definicion del sistema
	      - Le debe seguir un '='
	      - Abrimos llaves
	      - Leemos las posiciones de todas las particulas en un ciclo
		Si le sigue una coma, no es el ultimo ciclo, por tanto se
		continua. Si no le sigue una coma, se acabo.
	      - Cerramos llaves
	      - Si se produce algun error, retornamos 1
	      - El proceso ha sido correcto.
		Retornamos un 0
  -------------------------------------------------------------------------*/
int    GSFVMf_getsistema( void )
	 {
	    if( GSFVMf_match(MOVSISTEMA) ) return 1;
	    if( GSFVMf_match('=') ) return 1;
	    if( GSFVMf_match('{') ) return 1;
	    if( GSFVMv_tech ) printf("\n TEC: Leyendo %d ciclos\n", GSFVMv_ciclos);
	    GSFVMv_mciclo=0;
	    for( GSFVMv_ciclo=0;GSFVMv_ciclo<GSFVMv_ciclos; GSFVMv_ciclo++)
	       {
		printf("\r Leyendo movimientos del sistema. Hecho: %d %c",
		       (int)(GSFVMv_ciclo*100/GSFVMv_ciclos),
		       '%');
		if( (GSFVMv_ciclo%GSFVMv_steep)==0 )
		  {
		    GSFVMf_get_move();GSFVMv_mciclo++;
		  }else{
		    GSFVMf_skip_move();
		  };
		if( GSFVMv_ciclo<(GSFVMv_ciclos-1) ) GSFVMf_match(',');
	       }
	    puts("\r Lectura de movimientos OK.                            ");
	    if( GSFVMf_match('}') ) return 1;
	    return 0;
	 }




/*-------------------------------------------------------------------------
   GSFVMf_match() - Chequea el token. Si es correcto, coge el siguiente
  -------------------------------------------------------------------------
   28-7-1994, El esquema es el siguiente:
	      - Se comprueba si coinciden los token
		- Si coinciden buscar el siguiente token. Retornar un 0.
		- No coinciden, continuar
	      - Mostrar mensaje de error.
		Retornar un 1
  -------------------------------------------------------------------------*/
int    GSFVMf_match( int token_to_match )
	 {
	  if( token_to_match==GSFVMv_token_type )
	      {GSFVMv_token_type=GSFVMf_lexan(); return 0;}
	  printf(" ERROR - Se esperaba '%c'",(char)token_to_match);
	  return 1;
	 }



/*-------------------------------------------------------------------------
   GSFVMf_seek_tok() - Esta funcion busca en la matriz de keywords una
		    cadena que coincida con su parametro. Retorna el
		    token asociado al keyword.
  -------------------------------------------------------------------------
   28-7-1994, como funciona:
	     - Busca en la matriz. Dos posibilidades
	     - Llega al final de la matriz. Esto lo sabe porque el ultimo
	       token es 0
	     - Encuentra la cadena coincidente. En este caso retorna el
	       valor numerico del token.
  -------------------------------------------------------------------------*/
int    GSFVMf_seek_tok( char *str_token )
	 {
	  int GSFVMv_mat_pos;
	  for( GSFVMv_mat_pos=0;
	       GSFVMm_token_mat[GSFVMv_mat_pos].tok!=0;
	       GSFVMv_mat_pos++ )
	    {
	     if(strcmpi(str_token,GSFVMm_token_mat[GSFVMv_mat_pos].key)==0)
	       return GSFVMm_token_mat[GSFVMv_mat_pos].tok;
	    }
	  return 0;
	 }




/*-------------------------------------------------------------------------
   GSFVMf_lexan() - Lee el siguiente token presente en el fich. fuente
  -------------------------------------------------------------------------
   28-7-1994, El esquema es el siguiente:
	      - Lee el caracter en la entrada
	      - Si es un numero o '-',leerlo y guardarlo en GSFVMv_token_val
		Retornar NUM
	      - Si es una letra, buscar en la matriz de tokens, de que
		tipo se trata. Retornar el tipo de token
	      - Si es un caracter de separacion, desecharlo
	      - Si es un caracter aceptable, retornar el caracter
  -------------------------------------------------------------------------*/
int    GSFVMf_lexan( void )
	 {
	  int    GSFVMv_char;
	  int    GSFVMv_casepos=0;
	  int    GSFVMv_buff_pos=0;
	  char   GSFVMv_buffer[30];
	  while(1)
	  {
	   if( (GSFVMv_char = fgetc( GSFVMv_fmovimientos ))==EOF ) return EOF;
	   if( GSFVMv_char == 26 ) return EOF;
	   switch( GSFVMv_casepos )
	   {
	    case 0:  if( (GSFVMv_char=='=')||(GSFVMv_char=='{')||
			 (GSFVMv_char=='}')||(GSFVMv_char==';')||
			 (GSFVMv_char==',') )
			    {GSFVMv_casepos=1;
			     ungetc(GSFVMv_char,GSFVMv_fmovimientos);break;}
		     else if(isalpha(GSFVMv_char))
			    {GSFVMv_casepos=2;
			     ungetc(GSFVMv_char,GSFVMv_fmovimientos);break;}
		     else if( isdigit(GSFVMv_char) ||
			      (GSFVMv_char=='-') )
			    {
			     ungetc(GSFVMv_char,GSFVMv_fmovimientos);
			     fscanf(GSFVMv_fmovimientos, "%f", &GSFVMv_token_val);
			     return NUM;
			    }
		     else if( (GSFVMv_char=='\n')||(GSFVMv_char=='\r')||
			      (GSFVMv_char==' ')||(GSFVMv_char=='\t') )
			      break;
		     else {return 0;}

	    case 1:  return GSFVMv_char;

	    case 2:  while( isalnum(GSFVMv_char) )
		      {
		       GSFVMv_buffer[GSFVMv_buff_pos++]=GSFVMv_char;
		       GSFVMv_char = fgetc( GSFVMv_fmovimientos );
		      }
		     ungetc(GSFVMv_char, GSFVMv_fmovimientos);
		     GSFVMv_buffer[GSFVMv_buff_pos]='\0';
		     return GSFVMf_seek_tok( GSFVMv_buffer );
	   }
	  }
	 }




/*-------------------------------------------------------------------------
   GSFVMf_sentencia() - Lee una sentencia y la ejecuta
  -------------------------------------------------------------------------
   28-7-1994, El esquema es el siguiente:
	      - Chequea el tipo de token. Este determinara la sentencia
	      - Se leen los complementos de la sentencia, y se ejecutan
		las acciones correspondientes.
	      - Despues de la sentencia debe haber un ';'
  -------------------------------------------------------------------------*/
int    GSFVMf_sentencia( void )
	 {
	  int n;
	  switch( GSFVMv_token_type )
	    {
	      case EOF:            return 0;

	      case CICLOS:         GSFVMf_match(CICLOS);
				   GSFVMf_match('=');
				   GSFVMv_ciclos = GSFVMv_token_val;
				   GSFVMf_match(NUM);
				   break;

	      case INC_TEMPORAL:   GSFVMf_match(INC_TEMPORAL);
				   GSFVMf_match('=');
				   GSFVMv_inc_t = GSFVMv_token_val;
				   GSFVMf_match(NUM);
				   break;

	      case NUM_BODYS:      GSFVMf_match(NUM_BODYS);
				   GSFVMf_match('=');
				   GSFVMv_bodys = GSFVMv_token_val;
				   GSFVMf_match(NUM);
				   break;

	      case SPC_DIM:        GSFVMf_match(SPC_DIM);
				   GSFVMf_match('=');
				   GSFVMv_spcdim = (int)GSFVMv_token_val;
				   GSFVMf_match(NUM);
				   break;

	      case STEEP:          GSFVMf_match(STEEP);
				   GSFVMf_match('=');
				   GSFVMv_steep = (int)GSFVMv_token_val;
				   GSFVMf_match(NUM);
				   break;

	    }
	  GSFVMf_match(';');
	 }



/*-------------------------------------------------------------------------
   GSFVMf_get_move()  - Lee todas las posiciones en un ciclo
  -------------------------------------------------------------------------
   28-7-1994, El esquema es el siguiente:
	      - Abrimos llaves
		- Leemos las posiciones que ocupan todas las particulas
	      - Cerramos llaves
  -------------------------------------------------------------------------*/
void   GSFVMf_get_move( void )
	 {
	   int n;
	   GSFVMf_match('{');
	    for( n=0; n<GSFVMv_bodys; n++)
	       {
		GSFVMf_get_ppos( n );
		if( n<(GSFVMv_bodys-1) ) GSFVMf_match(',');
	       }
	   GSFVMf_match('}');
	 }





/*-------------------------------------------------------------------------
   GSFVMf_skip_move()  - Salta un ciclo
  -------------------------------------------------------------------------
   28-7-1994, El esquema es el siguiente:
	      - Leemos caracteres:
		- Si es {  incrementamos GSFVMv_llaves
		- Si es }  decrementamos GSFVMv_llaves
		- Si GSFVMv_llaves vale cero hemos saltado un ciclo
  -------------------------------------------------------------------------*/
void   GSFVMf_skip_move( void )
	 {
	   int n;
	   int  GSFVMv_llaves;

	   GSFVMv_llaves=1;
	   do{
	       GSFVMv_token_type=fgetc( GSFVMv_fmovimientos );
	       if( GSFVMv_token_type=='{' ) ++GSFVMv_llaves;
	       else if( GSFVMv_token_type=='}' ) --GSFVMv_llaves;

	   }while(GSFVMv_llaves);

	   GSFVMf_match('}');
	 }





/*-------------------------------------------------------------------------
   GSFVMf_get_ppos()  - Lee la posicion de la particula del parametro
  -------------------------------------------------------------------------
   28-7-1994, El esquema es el siguiente:
	      - Abrimos llaves
		- Leemos tantas coordenadas como dimensiones tenga el
		  espacio. Las coordenadas estan separadas por comas.
	      - Cerramos llaves
  -------------------------------------------------------------------------*/
void   GSFVMf_get_ppos( int GSFVMv_partic )
	 {
	   int n;
	   GSFVMf_match('{');
	    for( n=0; n<GSFVMv_spcdim; n++)
	       {
		*(GSFVMm_pos+GSFVMv_mciclo*GSFVMv_bodys*GSFVMv_spcdim
			  +GSFVMv_partic*GSFVMv_spcdim+n) =
		    GSFVMv_token_val;
		GSFVMf_match( NUM );
		if( n<(GSFVMv_spcdim-1) ) GSFVMf_match(',');
	       }
	   GSFVMf_match('}');
	 }
