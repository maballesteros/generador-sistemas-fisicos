/*-------------------------------------------------------------------------
   GSFMS_GA - Funciones para el proceso de argumentos
  -------------------------------------------------------------------------
   20-7-1994, las funciones en este momento son:
	GSFMSf_getargs()  - Procesa los argumentos de GSFMOVSIS
	GSFMSf_vis_help() - Muestra un mensaje de ayuda

  -------------------------------------------------------------------------*/
  #include "gsfmsinc.h"
  #include <string.h>
  #include <ctype.h>



/*-------------------------------------------------------------------------
   GSFMSf_vis_help() - Muestra por el canal estandard de salida las
		       opciones posibles de GSFMOVSIS
  -------------------------------------------------------------------------
   20-7-1994, tan solo saca por pantalla un mensaje de presentacion y las
   distintas opciones.
  -------------------------------------------------------------------------*/
void GSFMSf_vis_help( char *progname )
       {
	puts(" Modo de empleo: ");
	printf("     %s <fuente> <destino> <opciones>",
	       strupr( progname ));
	puts(" Opciones:");
	puts("      /h  - Muestra este mensaje");
	puts("      /t  - Mensajes tecnicos");
       }




/*-------------------------------------------------------------------------
   GSFMSf_getargs() - Procesa los argumentos que se pasan al GSFMOVSIS
  -------------------------------------------------------------------------
   20-7-1994, el modo de chequear la entrada es el siguiente:
	     - Hacemos el siguiente chequeo para todos los argumentos
	       - Si el argumento empieza con '/' o '-', entonces
		 tomamos su segunda letra. Esta determinara la opcion.
	       - Si no se cumple se chequea la variable GSFMSv_args_OK
		   - Si vale 0, se toma el argumento como fichero fuente
		     Se pone la variable GSFMSv_args_OK a 1
		   - Si vale 1, se toma el argumento como fichero destino
		     Se pone la variable GSFMSv_args_OK a 2
		   - Si vale 2, se ha excedido introducido un argumento
		     no valido, ya que los ficheros ya han sido tomados.
		     Muestra un mensaje diciendo que este argumento
		     sera desestimado.
	     - Tras el chequeo de los argumentos se chequea la variable
	       GSFMSv_args_OK.
	       - Si vale 0, se visualiza un mensaje de error y se retorna
		 un 1 (error)
	       - Si vale 1, se adopta un nombre para el fichero destino,
		 tomando el nombre del fichero fuente y a¤adiendo la
		 extension .fms
		 Se retorna un 0 (correcto)
	       - Si vale 2, se retorna 0 (correcto)

  -------------------------------------------------------------------------*/
int GSFMSf_getargs( int GSFMSv_num_args, char *GSFMSm_args[] )
       {
	int  GSFMSv_arg_num;
	char GSFMSv_firstargchar;
	char GSFMSv_opcionchar;
	int  GSFMSv_args_OK = 0;
	int  GSFMSv_dot_pos = 0;

	GSFMSv_tech = 0;

	for( GSFMSv_arg_num=1;
	     GSFMSv_arg_num < GSFMSv_num_args;
	     GSFMSv_arg_num++ )
	 {
	  GSFMSv_firstargchar = *GSFMSm_args[ GSFMSv_arg_num ];

	  if( (GSFMSv_firstargchar == '/')||
	      (GSFMSv_firstargchar == '-') )
	    {
	     GSFMSv_opcionchar = tolower(*(GSFMSm_args[ GSFMSv_arg_num ]+1));
	     switch( GSFMSv_opcionchar )
	       {
		case 'h': if(GSFMSv_args_OK==0)
			    {
			     GSFMSf_vis_help( GSFMSm_args[0] );
			     return 1;
			    }
			  else break;
		case 't': ++GSFMSv_tech;
			  break;
		default:  printf(" Parametro -%c desconocido. Sera saltado.\n",
				  GSFMSv_opcionchar);
			  break;
	       }
	    }
	  else if( GSFMSv_args_OK == 1 )
		 {
		  strncpy( GSFMSv_cdestino,
			   GSFMSm_args[ GSFMSv_arg_num ],
			   12);
		  printf(" Selecionado fichero destino: %s\n",
			 GSFMSv_cdestino);
		  GSFMSv_args_OK = 2;
		 }
	  else if( GSFMSv_args_OK == 0 )
		 {
		  strncpy( GSFMSv_cfuente,
			   GSFMSm_args[ GSFMSv_arg_num ],
			   12);
		  printf(" Selecionado fichero fuente: %s\n",
			 GSFMSv_cfuente);

		  GSFMSv_args_OK = 1;
		 }
	  else   {
		  puts(" Los ficheros fuente y destino ya han sido tomados");
		  printf(" El fichero: %s sera desechado.\n",
			 GSFMSm_args[ GSFMSv_arg_num ] );
		 }
	 }

	switch( GSFMSv_args_OK )
	 {
	  case 0: puts(" ERROR - No existe fichero de entrada");
		  GSFMSf_vis_help( GSFMSm_args[0] );
		  return 1;

	  case 1: while( (GSFMSv_cfuente[GSFMSv_dot_pos]!='.' )&&
			 (GSFMSv_cfuente[GSFMSv_dot_pos]!='\0')  )
			 ++GSFMSv_dot_pos;
		  strncpy(GSFMSv_cdestino,GSFMSv_cfuente,GSFMSv_dot_pos);
		  strcat(GSFMSv_cdestino,".fms");
		  printf(" Adoptado fichero destino: %s\n",GSFMSv_cdestino);
		  return 0;

	  case 2: return 0;
	 }

       }
