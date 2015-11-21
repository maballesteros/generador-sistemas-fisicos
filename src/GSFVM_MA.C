/*-------------------------------------------------------------------------
   GSFVM_MA  - Este es el modulo principal de GSFVISMOV
  -------------------------------------------------------------------------
   Incluye las funciones:
	    main()
  -------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------
   main() - Funcion principal
  -------------------------------------------------------------------------
   24-7-1994, El esquema que sigue el programa es el siguiente:
	      - Procesamos los argumentos que se pasan al programa
	      - Abrimos ficheros
	      - Capturamos el entorno del sistema
		  - Obtenemos la memoria necesaria
		      - Leemos el sistema
		      - Leemos y ejecutamos el programa
		  - Devolvemos la memoria al sistema
	      - Cerramos ficheros. Bye
  -------------------------------------------------------------------------*/
  main(int nargs, char *args[])
  {
    puts("\n GSFVISMOV - Visualizador de movimientos de sistemas fisicos");
    puts(" Julio-1994, v1.1 by Mike Crossbowers\n");

    if( GSFVMf_getargs( nargs, args ) ) exit(1);
    if( GSFVMf_openfiles() ) exit(2);
    if( GSFVMf_getentorno() ) exit(3);
	if( GSFVMf_getmemoria() ) exit(4);
	    if( GSFVMf_getsistema() ) exit(5);
	    if( GSFVMf_runprograma() ) exit(6);
	if( GSFVMf_putmemoria() ) exit(7);
    if( GSFVMf_closefiles() ) exit(8);
  }
