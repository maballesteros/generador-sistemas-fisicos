/*-------------------------------------------------------------------------
   GSFVM_FI - Funciones para ficheros fuente y destino (open-close)
  -------------------------------------------------------------------------
   Incluye las funciones:
	  int GSFVMf_openfiles( void );
	  int GSFVMf_closefiles( void );
  -------------------------------------------------------------------------*/
#include "GSFVMinc.h"
#include "fcntl.h"
#include "errno.h"

/*-------------------------------------------------------------------------
   GSFVMf_openfiles() - Abre fichero de movimientos
  -------------------------------------------------------------------------
   23-7-1994, sigue el siguiente esquema:
	      - Intenta abrir el fichero fuente en modo lectura binaria.
		 - Si no lo consigue da un mensaje de error.
		   Retorna un 1
		 - Si lo consigue, continua el proceso
	      - La apertura de ficheros ha sido correcta.
		Retorna un 0
  -------------------------------------------------------------------------*/
int GSFVMf_openfiles( void )
      {
       if( (GSFVMv_fmovimientos=fopen(GSFVMv_cmovimientos,"rb"))==NULL )
	   {
	    printf(" ERROR - No puedo abrir el fichero movimientos: %s",
		   GSFVMv_cmovimientos);
	    return 1;
	   }
       return 0;
      }


/*-------------------------------------------------------------------------
   GSFVMf_closefiles() - Cierra los ficheros fuente
  -------------------------------------------------------------------------
   23-7-1994, el esquema es sencillo:
	       - Intenta cerrar el fichero que se ha abierto
		  - Si no se ha cerrado, mostrar un mensaje
		    de error. Retornar 1
	       - El fichero se ha cerrado correctamente.
		 Retornar 0

  -------------------------------------------------------------------------*/
int GSFVMf_closefiles( void )
      {
       if( fclose(GSFVMv_fmovimientos)!=0 )
	 {
	  puts(" ERROR - No he podido cerrar el fichero de movimientos");
	  return 1;
	 }
       return 0;
      }
