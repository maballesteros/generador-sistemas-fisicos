/*-------------------------------------------------------------------------
   GSFMS_FI - Funciones para ficheros fuente y destino (open-close)
  -------------------------------------------------------------------------
   Incluye las funciones:
	  int GSFMSf_openfiles( void );
	  int GSFMSf_closefiles( void );
  -------------------------------------------------------------------------*/
#include "gsfmsinc.h"


/*-------------------------------------------------------------------------
   GSFMSf_openfiles() - Abre ficheros fuente y destino
  -------------------------------------------------------------------------
   20-7-1994, sigue el siguiente esquema:
	      - Intenta abrir el fichero fuente en modo lectura binaria.
		 - Si no lo consigue da un mensaje de error.
		   Retorna un 1
		 - Si lo consigue, continua el proceso
	      - Intenta abrir el fichero destino en modo escritura binaria.
		 - Si no lo consigue da un mensaje de error.
		   Retorna un 1
		 - Si lo consigue, continua el proceso
	      - La apertura de ficheros ha sido correcta.
		Retorna un 0
  -------------------------------------------------------------------------*/
int GSFMSf_openfiles( void )
      {
       if( (GSFMSv_ffuente=fopen(GSFMSv_cfuente,"rb"))==NULL )
	   {
	    printf(" ERROR - No puedo abrir el fichero fuente: %s",
		   GSFMSv_cfuente);
	    return 1;
	   }
       else if( (GSFMSv_fdestino=fopen(GSFMSv_cdestino,"wb"))==NULL )
	   {
	    printf(" ERROR - No puedo abrir el fichero destino: %s",
		   GSFMSv_cdestino);
	    return 1;
	   }
       return 0;
      }


/*-------------------------------------------------------------------------
   GSFMSf_closefiles() - Cierra los ficheros fuente y destino
  -------------------------------------------------------------------------
   20-7-1994, el esquema es sencillo:
	       - Intenta cerrar el fichero fuente y destino
		  - Si no se han cerrado los dos, mostrar un mensaje
		    de error. Retornar 1
	       - Los ficheros se han cerrado correctamente.
		 Retornar 0

  -------------------------------------------------------------------------*/
int GSFMSf_closefiles( void )
      {
       if( fcloseall()!=2 )
	 {
	  puts(" ERROR - No he podido cerrar todos los ficheros");
	  return 1;
	 }
       return 0;
      }
