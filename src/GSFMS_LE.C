/*-------------------------------------------------------------------------
   GSFMS_LE - Captura del entorno y definicion del sistema
  -------------------------------------------------------------------------
   Incluye las funciones:
	   int    GSFMSf_getentorno( void );
	   int    GSFMSf_getsistema( void );
	   int    GSFMSf_match( int token_to_match );
	   int    GSFMSf_seek_tok( char *str_token );
	   int    GSFMSf_lexan( void );
	   int    GSFMSf_sentencia( void );
	   void   GSFMSf_get_body( int GSFMSv_partic );
	   void   GSFMSf_get_pos( int GSFMSv_partic );
	   void   GSFMSf_get_vel( int GSFMSv_partic );
	   void   GSFMSf_get_msc( int GSFMSv_partic );
	   void   GSFMSf_get_campo( int GSFMSv_partic );
	   void   GSFMSf_get_no_part( int GSFMSv_partic );
  -------------------------------------------------------------------------*/
   #include "gsfmsinc.h"
   #include <ctype.h>
   #include <math.h>

/*-------------------------------------------------------------------------
   DEFINICIONES
  -------------------------------------------------------------------------
   28-7-1994, Estas definiciones asocian un valor numerico a cada token
  -------------------------------------------------------------------------*/
   #define DEFENTORNO       255
   #define DEFSISTEMA       254
   #define NUM_BODYS        253
   #define NUM_CAMPOS       252
   #define SPC_DIM          251
   #define INC_TEMPORAL     250
   #define TIEMPO           249
   #define NUM              248
   #define SIN_INT          247
   #define TODOS            246
   #define SI                 1
   #define NO                 0

/*-------------------------------------------------------------------------
   CABECERAS - Cabeceras de las funciones usadas en este modulo
  -------------------------------------------------------------------------*/
   int    GSFMSf_getentorno( void );
   int    GSFMSf_getsistema( void );
   int    GSFMSf_match( int token_to_match );
   int    GSFMSf_seek_tok( char *str_token );
   int    GSFMSf_lexan( void );
   int    GSFMSf_sentencia( void );
   void   GSFMSf_get_body( int GSFMSv_partic );
   void   GSFMSf_get_pos( int GSFMSv_partic );
   void   GSFMSf_get_vel( int GSFMSv_partic );
   void   GSFMSf_get_msc( int GSFMSv_partic );
   void   GSFMSf_get_campo( int GSFMSv_partic );
   void   GSFMSf_get_no_part( int GSFMSv_partic );



/*-------------------------------------------------------------------------
   VARIABLES - A continuacion defino las variables que se utilizan en
	       este modulo. La decision de no incluirlas en el fichero
	       gsfmsinc.h es de claridad. Solo las funciones presentes
	       en este modulo van a hacer uso de ellas.
  -------------------------------------------------------------------------

   --- int    GSFMSv_token_type;
       Esta variable guarda el valor numerico asociado al siguiente token
       en el fichero fuente. Indica si el token es un numero, un keyword,
       unas llaves, etc.

   --- float GSFMSv_token_val;
       Algunos token requieren de un valor numerico asociado. Es p.e. el
       caso, cuando el token es un numero. En este caso, el valor numerico
       de la cadena que representa al numero es guardado en esta variable

   --- struct GSFMSs_token_struct { ... }
       Esta estructura define una entrada a la tabla de simbolos.

   --- struct GSFMSs_token_struct GSFMSm_token_mat[]

  -------------------------------------------------------------------------*/
   int    GSFMSv_token_type;
   float  GSFMSv_token_val;
   struct GSFMSs_token_struct {
	     char *key;
	     int   tok;
	     double atr;
	    };
   struct GSFMSs_token_struct GSFMSm_token_mat[] =
	    {
	     "DefEntorno",DEFENTORNO,0,
	     "DefSistema",DEFSISTEMA,0,
	     "Tiempo",TIEMPO,0,
	     "IncTemporal",INC_TEMPORAL,0,
	     "SpaceDimension",SPC_DIM,0,
	     "NumCampos",NUM_CAMPOS,0,
	     "NumBodys",NUM_BODYS,0,
	     "SinInteraccion",SIN_INT,0,
	     "Null",NUM,NOCAMPO,
	     "Grav",NUM,GRAVITATORIO,
	     "Coulomb",NUM,COULOMB,
	     "Elast",NUM,ELASTICO,
	     "UnitR",NUM,UNITR,
	     "UnitA",NUM,UNITA,
	     "Si",NUM,SI,
	     "No",NUM,NO,
	     "Todos",TODOS,0
	    };




/*-------------------------------------------------------------------------
   GSFMSf_getentorno() - Lee del fichero fuente las variables y
			 preliminares a la definicion del sistema
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
int    GSFMSf_getentorno( void )
	 {
	   GSFMSv_num_lines=1;  /* Linea 1. Cuando encuentre \n, 2 */
	   GSFMSv_errors=0;     /* ­­Menos mal!! */
	   GSFMSv_no_interact=NO; /* Por defecto todas interactuan con
				     todas. Esto se debe, principalmente,
				     a que las anteriores versiones no
				     contaban con la opcion de no interaccion
				   */

	   GSFMSv_token_type = GSFMSf_lexan();
	   if( GSFMSf_match( DEFENTORNO ) ) return 1;
	   if( GSFMSf_match( '=' ) ) return 1;
	   if( GSFMSf_match( '{' ) ) return 1;
	   while( GSFMSv_token_type!='}' ) GSFMSf_sentencia();
	   if( GSFMSf_match( '}' ) ) return 1;
	   if( GSFMSv_errors )
	     {
	      puts(" Lectura de entorno con errores");
	      printf(" QUIT. Se cometieron %d errores\n",GSFMSv_errors);
	      return 1;
	     }
	   puts(" Lectura de entorno OK.");
	   return 0;
	 }




/*-------------------------------------------------------------------------
   GSFMSf_getsistema() - Lee la definicion del sistema y la guarda
			 en memoria
  -------------------------------------------------------------------------
   28-7-1994, El esquema es el siguiente:
	      - Inicializamos el puntero a atributos de msc
	      - Se comprueba que es la definicion del sistema
	      - Le debe seguir un '='
	      - Abrimos llaves
	      - Leemos todos los datos relativos a las particulas
		- Leemos los datos de una particula
		- Si no es la ultima particula, le debe seguir una coma
		- Siguiente particula
	      - Cerramos llaves
	      - Si se produce algun error, retornamos 1
	      - El proceso ha sido correcto.
		Retornamos un 0
  -------------------------------------------------------------------------*/
int    GSFMSf_getsistema( void )
	 {
	    int n;

	    GSFMSv_msc_atr_p=0;
	    if( GSFMSf_match(DEFSISTEMA) ) return 1;
	    if( GSFMSf_match('=') ) return 1;
	    if( GSFMSf_match('{') ) return 1;
	    for( n=0; n<GSFMSv_bodys; n++)
	       {
		printf(" Leyendo sistema fisico. Hecho %d %c\r",
		       (int)(n*100/GSFMSv_bodys),'%');
		GSFMSf_get_body( n );
		if( n<(GSFMSv_bodys-1) ) if( GSFMSf_match(',') ) return 1;
	       }
	    if( GSFMSf_match('}') ) return 1;
	    if( GSFMSv_errors )
	      {
	       puts(" Lectura de sistema con errores          ");
	       printf(" QUIT. Se cometieron %d errores\n",GSFMSv_errors);
	       return 1;
	      }
	    puts(" Lectura de sistema OK.                  ");
	    return 0;
	 }




/*-------------------------------------------------------------------------
   GSFMSf_match() - Chequea el token. Si es correcto, coge el siguiente
  -------------------------------------------------------------------------
   28-7-1994, El esquema es el siguiente:
	      - Se comprueba si coinciden los token
		- Si coinciden buscar el siguiente token. Retornar un 0.
		- No coinciden, continuar
	      - Mostrar mensaje de error.
		Retornar un 1
  -------------------------------------------------------------------------*/
int    GSFMSf_match( int token_to_match )
	 {
	  if( token_to_match==GSFMSv_token_type )
	      {GSFMSv_token_type=GSFMSf_lexan(); return 0;}
	  printf(" ERROR: %s(%d)  - Se esperaba '%c'\n",
		   GSFMSv_cfuente,
		   GSFMSv_num_lines,
		   (char)token_to_match);
	  ++GSFMSv_errors;
	  return 1;
	 }



/*-------------------------------------------------------------------------
   GSFMSf_seek_tok() - Esta funcion busca en la matriz de keywords una
		    cadena que coincida con su parametro. Retorna el
		    token asociado al keyword.
  -------------------------------------------------------------------------
   28-7-1994, funciona como sigue:
	     - Busca en la matriz una cadena que coincida. Dos posibilidades:
	     - Llega al final de la matriz. Esto lo sabe porque el ultimo
	       token es 0. El keyword es desconocido
	     - Encuentra la cadena coincidente. En este caso retorna el
	       valor numerico del token.
  -------------------------------------------------------------------------*/
int    GSFMSf_seek_tok( char *str_token )
	 {
	  int GSFMSv_mat_pos;
	  for( GSFMSv_mat_pos=0;
	       GSFMSm_token_mat[GSFMSv_mat_pos].tok!=0;
	       GSFMSv_mat_pos++ )
	    {
	     if(strcmpi(str_token,GSFMSm_token_mat[GSFMSv_mat_pos].key)==0)
	       {
		GSFMSv_token_val = GSFMSm_token_mat[GSFMSv_mat_pos].atr;
		return GSFMSm_token_mat[GSFMSv_mat_pos].tok;
	       }
	    }
	  printf(" ERROR %s(%d)  - Token '%s' desconocido\n ",
		   GSFMSv_cfuente,
		   GSFMSv_num_lines,
		   str_token);
	  ++GSFMSv_errors;
	  return 0;
	 }




/*-------------------------------------------------------------------------
   GSFMSf_lexan() - Lee el siguiente token presente en el fich. fuente
  -------------------------------------------------------------------------
   28-7-1994, El esquema es el siguiente:
	      - Lee el caracter en la entrada
	      - Si es un numero o '-',leerlo y guardarlo en GSFMSv_token_val
		Retornar NUM
	      - Si es una letra, buscar en la matriz de tokens, de que
		tipo se trata. Retornar el tipo de token
	      - Si es un caracter de separacion, desecharlo
	      - Si es un caracter aceptable, retornar el caracter
  -------------------------------------------------------------------------*/
int    GSFMSf_lexan( void )
	 {
	  int    GSFMSv_char;
	  int    GSFMSv_casepos=0;
	  int    GSFMSv_buff_pos=0;
	  char   GSFMSv_buffer[30];
	  while(1)
	  {
	   if( (GSFMSv_char = fgetc( GSFMSv_ffuente ))==EOF ) return EOF;
	   if( GSFMSv_char == 26 ) return EOF;
	   switch( GSFMSv_casepos )
	   {
	    case 0:  if( (GSFMSv_char=='=')||(GSFMSv_char=='{')||
			 (GSFMSv_char=='}')||(GSFMSv_char==';')||
			 (GSFMSv_char==',') )
			    {GSFMSv_casepos=1;
			     ungetc(GSFMSv_char,GSFMSv_ffuente);break;}
		     else if(isalpha(GSFMSv_char))
			    {GSFMSv_casepos=2;
			     ungetc(GSFMSv_char,GSFMSv_ffuente);break;}
		     else if( isdigit(GSFMSv_char) ||
			      (GSFMSv_char=='-') )
			    {
			     ungetc(GSFMSv_char,GSFMSv_ffuente);
			     fscanf(GSFMSv_ffuente, "%f", &GSFMSv_token_val);
			     return NUM;
			    }
		     else if( GSFMSv_char=='\n' ) {++GSFMSv_num_lines;break;}
		     else if( (GSFMSv_char==' ')||(GSFMSv_char=='\t')||
			      (GSFMSv_char=='\r') )
			      break;
		     else if( GSFMSv_char=='/' )
			    {
			     if( fgetc( GSFMSv_ffuente )!='*' )
			      puts(" '/' sin '*'; Comentario");
			     while(1)
			     {
			      GSFMSv_char = fgetc( GSFMSv_ffuente );
			      if( GSFMSv_char== EOF )
				puts(" ERROR - Comentario sin fin");
			      if( GSFMSv_char=='*' )
			       {if(fgetc(GSFMSv_ffuente)=='/')break;};
			     }
			     break;
			    }
		     else {return 0;}

	    case 1:  return GSFMSv_char;

	    case 2:  while( isalnum(GSFMSv_char) )
		      {
		       GSFMSv_buffer[GSFMSv_buff_pos++]=GSFMSv_char;
		       GSFMSv_char = fgetc( GSFMSv_ffuente );
		      }
		     ungetc(GSFMSv_char, GSFMSv_ffuente);
		     GSFMSv_buffer[GSFMSv_buff_pos]='\0';
		     return GSFMSf_seek_tok( GSFMSv_buffer );
	   }
	  }
	 }




/*-------------------------------------------------------------------------
   GSFMSf_sentencia() - Lee una sentencia y la ejecuta
  -------------------------------------------------------------------------
   28-7-1994, El esquema es el siguiente:
	      - Chequea el tipo de token. Este determinara la sentencia
	      - Se leen los complementos de la sentencia, y se ejecutan
		las acciones correspondientes.
	      - Despues de la sentencia debe haber un ';'
  -------------------------------------------------------------------------*/
int    GSFMSf_sentencia( void )
	 {
	  int n;
	  switch( GSFMSv_token_type )
	    {
	      case EOF:            return 0;

	      case TIEMPO:         GSFMSf_match(TIEMPO);
				   GSFMSf_match('=');
				   GSFMSv_movtime = GSFMSv_token_val;
				   GSFMSf_match(NUM);
				   break;

	      case INC_TEMPORAL:   GSFMSf_match(INC_TEMPORAL);
				   GSFMSf_match('=');
				   GSFMSv_inc_t = GSFMSv_token_val;
				   GSFMSf_match(NUM);
				   break;

	      case NUM_BODYS:      GSFMSf_match(NUM_BODYS);
				   GSFMSf_match('=');
				   GSFMSv_bodys = GSFMSv_token_val;
				   GSFMSf_match(NUM);
				   break;

	      case SPC_DIM:        GSFMSf_match(SPC_DIM);
				   GSFMSf_match('=');
				   GSFMSv_spcdim = (int)GSFMSv_token_val;
				   GSFMSf_match(NUM);
				   break;

	      case NUM_CAMPOS:     GSFMSf_match(NUM_CAMPOS);
				   GSFMSf_match('=');
				   GSFMSv_campos = (int)GSFMSv_token_val;
				   GSFMSf_match(NUM);
				   break;

	      case SIN_INT:        GSFMSf_match(SIN_INT);
				   GSFMSf_match('=');
				   GSFMSv_no_interact = (int)GSFMSv_token_val;
				   GSFMSf_match(NUM);
				   break;

	      default:             GSFMSv_token_type='}';
				   return;
	    }
	  GSFMSf_match(';');
	 }



/*-------------------------------------------------------------------------
   GSFMSf_get_body()  - Lee la descripcion de un body
  -------------------------------------------------------------------------
   28-7-1994, El nuevo esquema es el siguiente:
	      - Abrimos llaves
		- Leemos la posicion de la particula. Coma.
		- Leemos la velocidad de la particula. Coma.
		- Si se permite deshabilitar campos
		  - Abrimos llaves.
		    - Tomamos campos que genera. Coma
		    - Tomamos campos y particulas a las que es insensible.
		  - Cerramos llaves.
		- Si no se permite deshabilitar campos (por defecto)
		  - Leemos los campos que genera la particula. coma
		- Leemos las msc de la particula. Coma.
	      - Cerramos llaves
  -------------------------------------------------------------------------*/
void   GSFMSf_get_body( int GSFMSv_partic )
	 {
	   GSFMSf_match('{');
	     GSFMSf_get_pos( GSFMSv_partic );
	     GSFMSf_match(',');
	     GSFMSf_get_vel( GSFMSv_partic );
	     GSFMSf_match(',');
	     if( GSFMSv_no_interact )
	       {GSFMSf_get_no_part( GSFMSv_partic );GSFMSf_match(',');}
	     else
	       GSFMSm_bodys[GSFMSv_partic].nopart=0;
	     GSFMSf_get_campo( GSFMSv_partic );
	     GSFMSf_match(',');
	     GSFMSf_get_msc( GSFMSv_partic );
	   GSFMSf_match('}');
	 }


/*-------------------------------------------------------------------------
   GSFMSf_get_pos() - Lee y guarda la posicion de la particula
  -------------------------------------------------------------------------
   28-7-1994, El esquema es el siguiente:
	      - Abrimos llaves
		- Leemos tantos numeros como dimensiones tenga el espacio
		- Se guarda en la posicion de memoria correspondiente
		- Si no es el ultimo dato, se espera una coma
	      - Cerramos llaves
  -------------------------------------------------------------------------*/
void   GSFMSf_get_pos( int GSFMSv_partic )
	 {
	  int dir;

	  GSFMSf_match('{');
	   for( dir=0; dir<GSFMSv_spcdim; dir++ )
	    {
	     GSFMSm_bodys[GSFMSv_partic].pos[0][dir]=(float)GSFMSv_token_val;
	     GSFMSm_bodys[GSFMSv_partic].pos[1][dir]=(float)0;
	     GSFMSf_match(NUM);
	     if( dir<(GSFMSv_spcdim-1) ) GSFMSf_match(',');
	    }
	  GSFMSf_match('}');
	 }



/*-------------------------------------------------------------------------
   GSFMSf_get_vel() - Lee y guarda la velocidad de las particulas
  -------------------------------------------------------------------------
   28-7-1994, El esquema es el siguiente:
	      - Abrimos llaves
		- Leemos tantos numeros como dimensiones tenga el espacio
		- Se guarda en la posicion de memoria correspondiente
		- Si no es el ultimo dato, se espera una coma
	      - Cerramos llaves
  -------------------------------------------------------------------------*/
void   GSFMSf_get_vel( int GSFMSv_partic )
	 {
	  int dir;
	  GSFMSf_match('{');
	   for( dir=0; dir<GSFMSv_spcdim; dir++ )
	    {
	     GSFMSm_bodys[GSFMSv_partic].vel[0][dir]=(float)GSFMSv_token_val;
	     GSFMSm_bodys[GSFMSv_partic].vel[1][dir]=(float)0;
	     GSFMSf_match(NUM);
	     if( dir<(GSFMSv_spcdim-1) ) GSFMSf_match(',');
	    }
	  GSFMSf_match('}');
	 }



/*-------------------------------------------------------------------------
   GSFMSf_get_msc() - Lee y guarda en memoria las magnitudes sensibles a
		      los campos que genera.
  -------------------------------------------------------------------------
   28-7-1994, El esquema es el siguiente:
	      - Abrimos llaves
		- Leemos tantos numeros como campos se manejen
		- Se guarda en la posicion de memoria correspondiente
		- Si no es el ultimo dato, se espera una coma
	      - Cerramos llaves
  -------------------------------------------------------------------------*/
void   GSFMSf_get_msc( int GSFMSv_partic )
	 {
	  int dir;
	  GSFMSf_match('{');
	   for( dir=0; dir<GSFMSv_campos; dir++ )
	    {
	     if( GSFMSv_token_type =='{' ){
		GSFMSf_match('{');
		GSFMSm_bodys[GSFMSv_partic].msc[dir]=(float)GSFMSv_msc_atr_p;
		while(1)
		{
		 GSFMSm_msc_atr[GSFMSv_msc_atr_p++]=(float)GSFMSv_token_val;
		 GSFMSf_match(NUM);
		 if( GSFMSv_token_type==',' ) GSFMSf_match(',');
		 else break;
		}
		GSFMSm_msc_atr[GSFMSv_msc_atr_p++]=GSFMSv_bodys;
		GSFMSf_match('}');
	       }
	     else{
		GSFMSm_bodys[GSFMSv_partic].msc[dir]=(float)GSFMSv_token_val;
		GSFMSf_match(NUM);
	       }
	     if( dir<(GSFMSv_campos-1) ) GSFMSf_match(',');
	    }
	  GSFMSf_match('}');
	 }



/*-------------------------------------------------------------------------
   GSFMSf_get_campo() - Lee y guarda el campo que genera cada particula
  -------------------------------------------------------------------------
   28-7-1994, El esquema es el siguiente:
	      - Abrimos llaves
		- Leemos tantos numeros como campos se manejen
		- Se guarda en la posicion de memoria correspondiente
		- Si no es el ultimo dato, se espera una coma
	      - Cerramos llaves
  -------------------------------------------------------------------------*/
void   GSFMSf_get_campo( int GSFMSv_partic )
	 {
	  int dir;

	  GSFMSf_match('{');
	   for( dir=0; dir<GSFMSv_campos; dir++ )
	    {
	     GSFMSm_bodys[GSFMSv_partic].campo[dir]=(long)GSFMSv_token_val;
	     GSFMSf_match(NUM);
	     if( dir<(GSFMSv_campos-1) ) GSFMSf_match(',');
	    }
	  GSFMSf_match('}');
	 }

/*-------------------------------------------------------------------------
   GSFMSf_get_no_part() - Lee y guarda particulas a las que es insensible
			   la particula GSFMSv_partic
  -------------------------------------------------------------------------
   28-7-1994, version 1.2. El esquema es el siguiente:
	      - El siguiente token, ¨es NO?
		- Efectivamente. GSFMSv_partic no excluye a ninguna particula
		  - nopart valdra 0
		  - Salimos
		- El token no es NO. GSFMSv_partic debe excluir la
		  interaccion de alguna particula.
		  - Abrimos llaves
		  - nopart es ahora un puntero a las particulas
		    - Leemos la particula con la que no debe
		      interaccionar.
		    - Si no es el ultimo dato, se espera una coma.
		    - Repetimos.
		  - Cerramos llaves
  -------------------------------------------------------------------------*/
void   GSFMSf_get_no_part( int GSFMSv_partic )
	 {
	  int dir;

	  if( (GSFMSv_token_type==NUM)&&(GSFMSv_token_val==NO) )
	    {
	     GSFMSm_bodys[GSFMSv_partic].nopart=0;
	     GSFMSf_match(NUM);
	    }
	  else
	    {
	     GSFMSf_match('{');
	     GSFMSm_bodys[GSFMSv_partic].nopart=GSFMSv_msc_atr_p;
	     while(1)
	       {
		GSFMSm_msc_atr[GSFMSv_msc_atr_p++]=(float)GSFMSv_token_val;
		GSFMSf_match(NUM);
		if( GSFMSv_token_type==',' ) GSFMSf_match(',');
		else break;
	       }
	      GSFMSm_msc_atr[GSFMSv_msc_atr_p++]=(float)GSFMSv_bodys;
	      GSFMSf_match('}');
	     }
	 }
