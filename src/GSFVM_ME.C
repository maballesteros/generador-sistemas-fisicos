/*-------------------------------------------------------------------------
   GSFVM_ME - Manejo de memoria
  -------------------------------------------------------------------------
   En este bloque se incluyen las funciones:
       int    GSFVMf_getmemoria( void );
       int    GSFVMf_putmemoria( void );
   Y las variables:
       float GSFVMv_maxmem;
       float GSFVMv_needmem;
  -------------------------------------------------------------------------*/
#include "GSFVMinc.h"
#include <malloc.h>

float GSFVMv_needmem;





/*-------------------------------------------------------------------------
   GSFVMf_getmemoria() - Reserva un bloque de memoria
  -------------------------------------------------------------------------
   28-7-1994, El esquema es el siguiente:
	      - Pedimos un bloque de memoria.
		 - Si se produce un error, emitimos un mensaje.
		   Retornamos un 1
		 - Si no se produce error, seguimos
	      - Peticion de bloque sin problemas.
		Retornamos un 0
  -------------------------------------------------------------------------*/
#define BFLOAT 4 /* Numero de bytes que ocupa un float */
int    GSFVMf_getmemoria( void )
	 {
	  GSFVMv_needmem = ((float)(BFLOAT*GSFVMv_bodys*
			   ((int)(GSFVMv_ciclos/GSFVMv_steep)+1)*
			    GSFVMv_spcdim))/1024;

	  if( GSFVMv_tech ){
	      printf("\n TEC: Memoria requerida: %.1f Kb",GSFVMv_needmem);
	    }

	  if( (GSFVMm_pos =(float huge *)halloc( (GSFVMv_bodys*
				 ((int)(GSFVMv_ciclos/GSFVMv_steep)+1)*
				 GSFVMv_spcdim),
				 BFLOAT
			       )
	      )
	      == (long)NULL ) {
		 puts(" ERROR - No puedo conseguir la memoria requerida.");
		 return 1;
		       }
	  return 0;
	 }



/*-------------------------------------------------------------------------
   GSFVMf_putmemoria() - Liberamos el bloque de memoria
  -------------------------------------------------------------------------
   28-7-1994, El esquema es:
	      - Liberamos el bloque de memoria
	      - Retornamos 0
  -------------------------------------------------------------------------*/
int    GSFVMf_putmemoria( void )
	 {
	  hfree( GSFVMm_pos );
	  return 0;
	 }
