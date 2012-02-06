// Variables accesibles desde el exterior
// -- AQUI
{
	// Variables internas (no existen fuera de esta sección)
	// -- AQUI

    /*
    *   Compilación y Evaluación
    */
    list<list <string> > ratingsList;
    list<string> rating;
    ostringstream strs;
    string str;
    string rutaFuentes, rutaCasos;
    string tipoResultado, archCal;
    if (outputFile == "") archCal = "calificaciones.txt";
    else archCal = outputFile;
	int realMaxTime;
    ofstream calificaciones(archCal.c_str());         //Archivo de los resultados en txt

    clog << "Iniciando evaluación..." << endl;
    clog << "El problema es: " << problem << endl;
    clog << "El tipo de juez es: " << judgeType << endl;
    clog << "El modo de evaluación (indicado) es: " << (strictEval?"estricto":"normal") << endl;
    clog << "Resultados se guardarán en: " << archCal << endl;
    clog << endl;

    calificaciones << "Calificaciones de " << problem << "\n\n";

    // Pasar restricción de tiempo a segundos enteros
	if (maxRunTime % 1000 == 0) realMaxTime = maxRunTime;
	else realMaxTime = maxRunTime + (1000 - maxRunTime % 1000);
	realMaxTime /= 1000;

    /**
    *   Juez Normal
    **/
    if(judgeType == "standard")
    {
        for (list<string>::iterator itSF = sourceFiles.begin(); itSF != sourceFiles.end(); itSF++)  //Ciclo para cada programa de alumno. (Fuentes)
        {
            int programa;
            pid_t pID;
            int fd_pipe[2];
            int compilacion;
            string lang;
            string comando;

            unsigned int casosCorrectos = 0;
            tipoResultado = "";
            string casoActual, codigoActual, correctoActual;

            //La lista de calificacion es borrada para cada alumno
            rating.clear();

            codigoActual += *itSF;
            clog << "Evaluando el codigo " << codigoActual << endl;

            calificaciones << "Programa " << codigoActual << "\t";
			rating.push_back (codigoActual);
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
                    	rlimit limite;

						close(fd_pipe[0]);
                        dup2(fd_pipe[1], STDOUT_FILENO);    //Salida al pipe.
                        close(fd_pipe[1]);

                        freopen(casoActual.c_str(), "r", stdin);   //Entrada del problema.

                        //Restricciones
						//Tiempo
						limite.rlim_cur = limite.rlim_max = realMaxTime;
						setrlimit(RLIMIT_CPU, &limite);
						// Memoria
						limite.rlim_cur = limite.rlim_max = maxRunMem*1024*1024;
						setrlimit(RLIMIT_AS, &limite);
						// No forks
						limite.rlim_cur = limite.rlim_max = 1;
						setrlimit(RLIMIT_NPROC, &limite);

                        //Ejecución
                        comando = "exec_alumno";
                        clog << "  Ejecuto el programa " << comando << endl;
                        programa = execl(comando.c_str(), comando.c_str(), (char *)NULL);
                        if(programa == -1)
                        {
                            //Si no se logra ejecutar correctamente el programa, se guarda un RE (runtime error)
                            rating.push_back ("RE");
                            cerr << "Error de ejecución" << endl;
                            return 0;
                        }
                    }
                    else
                    {
						tms tiempo;
						int execTime;
						int status, signalType;
						bool exito;

                        close(fd_pipe[1]);
                        waitpid(pID, &status, 0);

						exito = false;
                        if (WIFEXITED(status)) {
                        	clog << "Finalizó bien" << endl;
							// Calcular tiempo
							times(&tiempo);
							execTime = (tiempo.tms_cutime + tiempo.tms_cstime) / (sysconf(_SC_CLK_TCK) / 1000.0);

							if (execTime > maxRunTime) rating.push_back ("0 (TLE)");
							else exito = true;
						} else if (WIFSIGNALED(status)) {
							signalType = WTERMSIG(status);
							if (signalType == SIGKILL) {
								clog << "Matado por KILL" << endl;
								rating.push_back ("0 (TLE)");
							}
							else if (signalType == SIGSEGV) {
								clog << "Matado por SEGV" << endl;
								rating.push_back ("0 (MEM)");
							}
						}

						// Si merece la pena evaluarlo
                        if(exito)
                        {
                        	strs.str("");
							strs << execTime;
							str = strs.str();


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
                                rating.push_back ("1 (" + str +" ms)");
                            }
                            else
                            {
                                rating.push_back ("0 (" + str +" ms)");
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
            rating.push_back (tipoResultado);

            if(tipoResultado == "AC")
            {
                calificaciones << "AC\tCalificación\t" << ((double)casosCorrectos/testCases.size()*100.0) << endl;

                strs.str("");
                strs.clear();
                strs << ((double)casosCorrectos/testCases.size()*100.0);
                str = strs.str();

                rating.push_back (str);
            }
            else if(tipoResultado == "WA")
            {
                if(casosCorrectos == 0){
                    calificaciones << "WA\tCalificación\t0" << endl;
                    rating.push_back ("0");
                }
                else{
                    calificaciones << "WA\tCalificación\t" << ((double)casosCorrectos/testCases.size()*100.0) << endl;
                    strs.str("");
                    strs.clear();
                    strs << ((double)casosCorrectos/testCases.size()*100.0);
                    str = strs.str();

                    rating.push_back (str);
                }
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
            //TODO
            ratingsList.push_back(rating);
        }
        cout << "Estoy por abrir archivo";
        string resultsFile = "calificaciones.csv";
        ofstream outputResults(resultsFile.c_str());         //Archivo de los resultados en csv

        if(outputResults.fail()){
				cout << "No se pudo abrtir el archivo";
        }
		cout << "Se supone que ya abri el archivo";
        while (!ratingsList.empty()){

            rating = ratingsList.front();

            while(!rating.empty()){
            	cout << ".";
                outputResults << rating.front() << ","; //modificar la coma para que pueda ser otro operador
                rating.pop_front();
            }
            outputResults << endl;
            ratingsList.pop_front();
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
