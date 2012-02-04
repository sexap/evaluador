//TODO: Crear variables con la ruta de los archivos y otras con los nombres de los archivos.

//TODO: Recibir a partir de aquí la variable que indica el lenguaje.

// Variables accesibles desde el exterior
// -- AQUI
{
	// Variables internas (no existen fuera de esta sección)
	// -- AQUI

    /*
    *   Compilación y Evaluación
    */
    string rutaFuentes, rutaCasos;
    string tipoResultado, archCal;
    if (outputFile == "") archCal = "calificacion.txt";
    else archCal = outputFile;

    int calificacion;

    ofstream calificaciones(archCal.c_str());         //Archivo de los resultados en txt

    clog << "Iniciando evaluación..." << endl;
    clog << "El problema es: " << problem << endl;
    clog << "El tipo de juez es: " << judgeType << endl;
    clog << "El modo de evaluación (indicado) es: " << (strictEval?"estricto":"normal") << endl;
    clog << "Resultados se guardarán en: " << archCal << endl;
    clog << endl;

    calificaciones << "Calificaciones de " << problem << "\n\n";
    /**
    *   Juez Normal
    **/
    if(judgeType == "standard")
    {
    	// ********************************
    	// no debería ser la variable strictEval?
    	// (leida desde las configuraciones)
        bool estricto = false;

        for (list<string>::iterator itSF = sourceFiles.begin(); itSF != sourceFiles.end(); itSF++)  //Ciclo para cada programa de alumno. (Fuentes)
        {
            int programa;
            pid_t pID;
            int status;
            int fd_pipe[2];
            int leidos;
            int compilacion;

            int casosCorrectos = 0;
            tipoResultado = "";
            string casoActual, codigoActual, correctoActual;

            codigoActual += *itSF;
            clog << "Evaluando el codigo " << codigoActual << endl;
            calificaciones << "Programa " << codigoActual << "\t";

            //TODO Agregar la compilación para los otros lenguajes.
            string nombrePuro = removeExtension(*itSF);

            string lang;
            if(!forceValidLang(lang, codigoActual))
            {
                    cerr << "La extensión de " << codigoActual << " no coincide con un lenguaje conocido." << endl;
            }
            string comando;


            if(lang == "c")
            {
                comando = "gcc -lm " + *itSF + " -o exec_alumno";
            }
            else if(lang == "c++")
            {
                comando = "g++ -lm " + *itSF + " -o exec_alumno";
            }
            else if(lang == "java")
            {
                //gcj --main=divtor26 divtor26.java -o divtor26.class
                size_t found;
                string nombreSinRuta;
                found=nombrePuro.find_last_of("/");
                nombreSinRuta = nombrePuro.substr(found+1, nombrePuro.length());
                nombrePuro += ".class";
                comando = "gcj --main=" + nombreSinRuta + " " + *itSF + " -o exec_alumno";
            }

            clog << "Compilando con el comando " << comando << endl;
            compilacion = system(comando.c_str());

            if(compilacion == 0)
            {
                clog << "Compilación correcta...." << endl;
            }
            else
            {
                tipoResultado = "CE";
                clog << "Falló la compilación." << endl;
            }
            clog << endl;

            if(tipoResultado != "CE")
            {
                for (list<string>::iterator itTC = testCases.begin(); itTC != testCases.end(); itTC++)  //Ciclo para cada caso de prueba. (Casos)
                {
                    casoActual = problem + "/" + *itTC + "." CASE_EXTENSION;
                    codigoActual = problem + "/" + *itSF;

                    clog << "  Probando con caso " << casoActual << endl;
                    if (pipe(fd_pipe) < 0)
                    {
                        cerr << "No se pudo hacer pipe" << endl;
                        return 1;
                    }

                    pID = fork();

                    //**************** Hijo *******************/
                    if(pID == 0)
                    {
                        //cerr << "Soy el hijo." << endl;

                        close(fd_pipe[0]);
                        dup2(fd_pipe[1], STDOUT_FILENO);    //Salida al pipe.
                        close(fd_pipe[1]);

                        freopen(casoActual.c_str(), "r", stdin);   //Entrada del problema.

                        //Ejecución
                        comando = "exec_alumno";
                        clog << "  Ejecuto el programa " << comando << endl;
                        programa = execl(comando.c_str(), comando.c_str(), NULL);
                        if(programa == -1)
                        {
                            cerr << "Error de ejecución" << endl;
                            return 0;
                        }
                    }   //Cierra el hijo.
                    //****************** Padre ********************/
                    else
                    {
                        close(fd_pipe[1]);
                        //es el padre
                        waitpid(pID, &status, 0);

                        bool exito =true;
                        /*if (WIFEXITED(status))
                        {
                            if (WEXITSTATUS(status) == 0)
                            {
                                exito = true;
                            }
                            else
                            {
                                cerr << "WEXITSTATUS(status) diferente de 0" << endl;
                                tipoResultado = "CE";
                            }
                        }
                        else if (WIFSIGNALED(status))
                        {
                            cerr << "Terminado con la señal" << WTERMSIG(status)<< endl;
                        }
                        else if (WIFSTOPPED(status))
                        {
                            cerr << "Detenido por la señal" << WSTOPSIG(status) << endl;
                        }
                        else if (WIFCONTINUED(status))
                        {
                            cerr << "wtf" << endl;
                        }
                        */
                        if(exito)
                        {
                            //el programa ya fue compilado y esta listo para ejecutarse
                            string salidaCorr = problem + "/" + *itTC + "." + OUTPUT_EXTENSION;
                            clog << "  Comparo con el archivo: " << salidaCorr << endl;
                            //Comparar entre el caso actual y la salida lectura de pipe desde el hijo.

                            /*
                            *   Llamo al juez normal
                            */
                            if(juezNormal(estricto, salidaCorr, fd_pipe[0]))
                            {
                                clog << "  Caso " << salidaCorr << " estuvo bien en modo ";
                                if(estricto)
                                    clog << "estricto." << endl;
                                else
                                    clog << "normal." << endl;
                                casosCorrectos++;
                            }
                            else
                            {
                                clog << "  Falla el caso " << salidaCorr << " en modo ";
                                if(estricto)
                                    clog << "estricto." << endl;
                                else
                                    clog << "normal." << endl;
                            }
                            clog << endl;
                        }
                        close(fd_pipe[0]);
                    }   //Cierra el Padre
                }   //Cierra el ciclo TC
                if(estricto)
                {
                    clog << "Tuvo " << casosCorrectos << " casos correctos de " << testCases.size() << " casos de prueba." << endl;
                    if(casosCorrectos == testCases.size())
                    {

                        clog << "ACCEPTED" << endl;
                        clog << "La calificación es: 10" << endl;
                        tipoResultado = "AC";
                    }
                    else
                    {
                        clog << "WRONG ANSWER" << endl;
                        tipoResultado = "WA";
                        clog << "La calificación es: 0" << endl;
                    }
                }
                else
                {
                    clog << "Tuvo " << casosCorrectos << " casos correctos de " << testCases.size() << " casos de prueba." << endl;
                    tipoResultado = (((double)casosCorrectos/testCases.size()*100.0) >= 60.0)?"AC":"WA";
                    clog << "La calificación es: " << ((double)casosCorrectos/testCases.size()*100.0) << endl;
                }
                clog << endl;
            }
            if(tipoResultado == "AC")
            {
                calificaciones << "AC\tCalificación\t" << ((double)casosCorrectos/testCases.size()*100.0) << endl;
            }
            else if(tipoResultado == "WA")
            {
                if(casosCorrectos == 0)
                    calificaciones << "WA\tCalificación\t0" << endl;
                else
                    calificaciones << "WA\tCalificación\t" << ((double)casosCorrectos/testCases.size()*100.0) << endl;
            }
            else if(tipoResultado == "CE")
            {
                calificaciones << "CE" << endl;
            }
            else if(tipoResultado == "TLE")
            {

            }
            else if(tipoResultado == "MLE")
            {

            }
        }
    }

    /**
    *   Juez Especial
    **/
    else if(judgeType == "especial")
    {

    }

    /**
    *   Juez Interactivo
    **/
    else if(judgeType == "interactive")
    {

    }
}
