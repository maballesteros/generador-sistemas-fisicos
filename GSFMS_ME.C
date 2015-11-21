/*-------------------------------------------------------------------------
   GSFMS_ME - Manejo de memoria
  -------------------------------------------------------------------------
   28-7-1994, En este bloque se incluyen las funciones:
       int    GSFMSf_getmemoria( void );
       int    GSFMSf_putmemoria( void );
  -------------------------------------------------------------------------*/
#include "gsfmsinc.h"
#include <malloc.h>




/*-------------------------------------------------------------------------
   GSFMSf_getmemoria() - Reserva un bloque de memoria
  -------------------------------------------------------------------------
   28-7-1994, El esquema es el siguiente:
	      - Pedimos un bloque de memoria.
		 - Si se produce un error, emitimos un mensaje.
		   Retornamos un 1
		 - Si no se produce error, seguimos
	      - Peticion de bloque sin problemas.
		Retornamos un 0
  -------------------------------------------------------------------------*/
int    GSFMSf_getmemoria( void )
	 {
	  float needmem=((float)GSFMSv_bodys*BODYLENGHT)/1024;
	  if( GSFMSv_tech ) printf(" TEC: Memoria requerida %.1f Kb\n",
				    needmem );
	  if( (GSFMSm_bodys
	       =(struct GSFMSs_body *)malloc(
		  (size_t)(GSFMSv_bodys*BODYLENGHT) )
	      )
	      == NULL) {
			puts(" ERROR - No puedo conseguir memoria");
			return 1;
		       }
	  return 0;
	 }



/*-------------------------------------------------------------------------
   GSFMSf_putmemoria() - Devolvemos bloque de memoria
  -------------------------------------------------------------------------
   28-7-1994, El esquema es:
	      - Liberamos el bloque de memoria
	      - Retornamos 0
  -------------------------------------------------------------------------*/
int    GSFMSf_putmemoria( void )
	 {
	  free( GSFMSm_bodys );
	  return 0;
	 }
