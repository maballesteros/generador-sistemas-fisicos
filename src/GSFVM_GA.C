/*-------------------------------------------------------------------------
   GSFVM_GA - Este modulo procesa los argumentos que se pasan al programa
  -------------------------------------------------------------------------
   Incluye las funciones:
	   GSFVMf_vis_help( void );
	   GSFVMf_getargs( int GSFVMv_num_args, char *GSFVMm_args[] );
   Incluye las variables:
	   --- char *GSFVMv_GSFname;
	       Puntero a la cadena, en mayusculas, del nombre actual de este
	       programa. El nombre lo sacamos del argumento 0.
  -------------------------------------------------------------------------*/
#include "GSFVMinc.h"
#include <string.h>

char *GSFVMv_GSFname;


/*-------------------------------------------------------------------------
   GSFVMf_vis_help() - Muestra por el canal estandard de salida las
		       opciones posibles de GSFVISMOV
  -------------------------------------------------------------------------
   23-7-1994, Muestra el formato de la llamada mediante el DOS, y las
   opciones que gestiona.
  -------------------------------------------------------------------------*/
void GSFVMf_vis_help( void )
       {
	puts(" Modo de empleo: ");
	printf("    %s <fichero de movimientos> <opciones>", GSFVMv_GSFname);
	puts(" Opciones:");
	puts("      /h  - Muestra este mensaje");
	puts("      /v  - Modo de video VGA");
	puts("      /e  - Modo de video EGA");
	puts("      /c  - Modo de video CGA");
	puts("      /t  - Pasa por pantalla mensajes tecnicos");
       }




/*-------------------------------------------------------------------------
   GSFVMf_getargs() - Procesa los argumentos que se pasan al GSFVISMOV
  -------------------------------------------------------------------------
   23-7-1994, el modo de chequear la entrada es el siguiente:
	     - Hacemos el siguiente chequeo para todos los argumentos
	       - Si el argumento empieza con '/' o '-', entonces
		 tomamos su segunda letra. Esta determinara la opcion.
	       - Si no se cumple se chequea la variable GSFVMv_args_OK
		   - Si vale 0, se toma el argumento como fichero de
		     movimientos
		     Se pone la variable GSFVMv_args_OK a 1
		   - Si vale 1, se ha excedido introducido un argumento
		     no valido, ya que los ficheros ya han sido tomados.
		     Muestra un mensaje diciendo que este argumento
		     sera desestimado.
	     - Tras el chequeo de los argumentos se chequea la variable
	       GSFVMv_args_OK.
	       - Si vale 0, se visualiza un mensaje de error y se retorna
		 un 1 (error)
		 Se retorna un 0 (correcto)
	       - Si vale 1, se retorna 0 (correcto)

  -------------------------------------------------------------------------*/
int GSFVMf_getargs( int GSFVMv_num_args, char *GSFVMm_args[] )
       {
	int  GSFVMv_arg_num;
	char GSFVMv_firstargchar;
	char GSFVMv_opcionchar;
	int  GSFVMv_args_OK = 0;
	int  GSFVMv_dot_pos = 0;



	GSFVMv_tech=0;
	GSFVMv_vmode=0;

	GSFVMv_GSFname = strupr( GSFVMm_args[0] );

	for( GSFVMv_arg_num=1;
	     GSFVMv_arg_num < GSFVMv_num_args;
	     GSFVMv_arg_num++ )
	 {
	  GSFVMv_firstargchar = *GSFVMm_args[ GSFVMv_arg_num ];

	  if( (GSFVMv_firstargchar == '/')||
	      (GSFVMv_firstargchar == '-') )
	    {
	     GSFVMv_opcionchar = tolower(*(GSFVMm_args[ GSFVMv_arg_num ]+1));
	     switch( GSFVMv_opcionchar )
	       {
		case 'h': if(GSFVMv_args_OK==0)
			    {GSFVMf_vis_help(); return 1;}
			  else break;
		case 't': GSFVMv_tech=1; break;
		case 'v': GSFVMv_vmode=3;
			  puts(" Seleccionada VGA\n");
			  break;
		case 'e': GSFVMv_vmode=2;
			  puts(" Seleccionada EGA\n");
			  break;
		case 'c': GSFVMv_vmode=1;
			  puts(" Seleccionada CGA\n");
			  break;
	       }
	    }
	  else if( GSFVMv_args_OK == 0 )
		 {
		  strncpy( GSFVMv_cmovimientos,
			   GSFVMm_args[ GSFVMv_arg_num ],
			   12);
		  printf(" Selecionado fichero de movimientos: %s\n",
			 GSFVMv_cmovimientos);
		  GSFVMv_args_OK = 1;
		 }
	  else   {
		  puts(" El ficheros de movimientos ya ha sido tomado\n");
		  printf(" El fichero: %s sera desechado.\n",
			 GSFVMm_args[ GSFVMv_arg_num ] );
		 }
	 }

	switch( GSFVMv_args_OK )
	 {
	  case 0: puts(" ERROR - Necesito fichero de programa");
		  GSFVMf_vis_help();
		  return 1;

	  case 1: if(GSFVMv_vmode==0){
		    puts(" Modo de video no seleccionado. Se adopta: VGA\n");
		    GSFVMv_vmode=3;}
		  return 0;
	 }

       }

