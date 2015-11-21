/*-------------------------------------------------------------------------
    main() - rutina principal del sistema

    20-7-1994, version 1.2 -  Anyadida la posibilidad de que algunas
			      particulas no sean sensibles a los campos
			      que otras generan. Se incluye como opcion,
			      asi que un fichero de alguna version
			      anterior sera generado con normalidad.
  -------------------------------------------------------------------------
   24-7-1994, main() recibe los argumentos que se pasan al programa y
   realiza las siguientes tareas:

	GSFMSf_getargs()    - En primer lugar procesa los argumentos.
			      El proceso incluye determinar los ficheros
			      de entrada y de salida.
			      Si el de entrada no se da, devuelve un error.
			      Si tiene el de entrada (.fsf) pero no el de
			      salida, adopta el nombre de la entrada y le da
			      la extension .fms
	GSFMSf_openfiles()  - Abre el fichero fuente y el de salida
	GSFMSf_getentorno() - Lee del fichero de entrada:
				- Numero de cuerpos
				- Tiempo de movimiento
				- Incremento del tiempo
				- Dimension del espacio
				- ....
	GSFMSf_getmemoria() - Consigue la memoria necesaria acorde con el
			      numero de cuerpos presentes
	GSFMSf_getsistema() - Lee del fichero de entrada los datos sobre
			      los cuerpos que forman el sistema
	GSFMSf_movsistema() - Mueve los cuerpos que forman el sistema,
			      generando un fichero de salida con los datos
			      relativos al movimiento
	GSFMSf_closefiles() - Cierra el fichero fuente y el de salida

  -------------------------------------------------------------------------*/

   #include "gsfmsinc.h"
   #include <stdio.h>

   main( int argc, char *argv[] )
       {
	puts("\n GSFMOVSIS - Motor de sistemas fisicos");
	puts(" Julio-1994, v1.2 by Mike Crossbowers\n");

	if( GSFMSf_getargs( argc, argv ) ) exit(1);
	if( GSFMSf_openfiles() ) exit(2);
	       if( GSFMSf_getentorno() ) exit(3);
	       if( GSFMSf_getmemoria() ) exit(4);
		      if( GSFMSf_getsistema() ) exit(5);
		      if( GSFMSf_movsistema() ) exit(6);
	       if( GSFMSf_putmemoria() ) exit(7);
	if( GSFMSf_closefiles() ) exit(8);
       }
