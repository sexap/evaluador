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
        for (list<string>::iterator itSF = sourceFiles.begin(); itSF != sourceFiles.end(); itSF++)  //Ciclo para cada programa de alumno. (Fuentes)
        {
            int programa;
            pid_t pID;
            int status;
            int fd_pipe[2];
            int compilacion;
            string lang;
            string comando;

            unsigned int casosCorrectos = 0;
            tipoResultado = "";
            string casoActual, codigoActual, correctoActual;

            codigoActual += *itSF;
            clog << "Evaluando el codigo " << codigoActual << endl;
            calificaciones << "Programa " << codigoActual << "\t";

            string nombrePuro = removeExtension(*itSF);


            if(!forceValidLang(lang, codigoActual))
            {
                    cerr << "La extensión de " << codigoActual << " no coincide con un lenguaje conocido." << endl;
            }

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

                    if(pID == 0)
                    {
                        close(fd_pipe[0]);
                        dup2(fd_pipe[1], STDOUT_FILENO);    //Salida al pipe.
                        close(fd_pipe[1]);

                        freopen(casoActual.c_str(), "r", stdin);   //Entrada del problema.

                        //Ejecución
                        comando = "exec_alumno";
                        clog << "  Ejecuto el programa " << comando << endl;
                        programa = execl(comando.c_str(), comando.c_str(), (char *)NULL);
                        if(programa == -1)
                        {
                            cerr << "Error de ejecución" << endl;
                            return 0;
                        }
                    }
                    else
                    {
                        close(fd_pipe[1]);
                        waitpid(pID, &status, 0);

                        bool exito = true;

                        if(exito)
                        {
                            //el programa ya fue compilado y esta listo para ejecutarse
                            string salidaCorr = problem + "/" + *itTC + "." + OUTPUT_EXTENSION;
                            clog << "  Comparo con el archivo: " << salidaCorr << endl;

                            /*
                            *   Llamo al juez normal
                            */
                            if(juezNormal(strictEval, salidaCorr, fd_pipe[0]))
                            {
                                clog << "  Caso " << salidaCorr << " estuvo bien en modo ";
                                if(strictEval)
                                    clog << "estricto." << endl;
                                else
                                    clog << "normal." << endl;
                                casosCorrectos++;
                            }
                            else
                            {
                                clog << "  Falla el caso " << salidaCorr << " en modo ";
                                if(strictEval)
                                    clog << "estricto." << endl;
                                else
                                    clog << "normal." << endl;
                            }
                            clog << endl;
                        }
                        close(fd_pipe[0]);
                    }   //Cierra el Padre
                }   //Cierra el ciclo TC
                if(strictEval)
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
