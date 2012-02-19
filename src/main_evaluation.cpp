// Variables accesibles desde el exterior
// -- AQUI
{
    // Variables internas (no existen fuera de esta sección)
    // -- AQUI

    list<list <string> > ratingsList;
    list<string> rating;
    ostringstream strs;
    string str;
    string rutaFuentes, rutaCasos;
    string tipoResultado, archCal;
    string SFsinRuta;
    size_t positionInString;
    int programa, status, fd_pipe_eval[2], fd_pipe_comp[2];
    pid_t pID;
    char buffer[512];
    string comando, lang, SFsinRutaNiExtension;
    unsigned int casosCorrectos;
    bool exito;

    resource_t usedResources;
    enum {LIMIT_NONE, LIMIT_TIME, LIMIT_MEM};
    int limitExceded;

    if (outputFile == "") archCal = "califSEAP" + problem + ".txt";
    else archCal = outputFile;
    int realMaxTime;

    ofstream calificaciones(archCal.c_str());         //Archivo de los resultados en txt

    clog << "Iniciando evaluación..." << endl;
    clog << "El problema es: " << problem << endl;
    clog << "El tipo de juez es: " << judgeType << endl;
    clog << "El modo de evaluación (indicado) es: " << (strictEval?"estricto":"normal") << endl;
    clog << "Resultados se guardarán en: " << archCal << endl;
    clog << endl;

    calificaciones << "Calificaciones de " << problem << " con SEAP\n\n";

    // Pasar restricción de tiempo a segundos enteros
    if (maxRunTime % 1000 == 0) realMaxTime = maxRunTime;
    else realMaxTime = maxRunTime + (1000 - maxRunTime % 1000);
    realMaxTime /= 1000;

    //Ciclo para cada programa de alumno. (Fuentes con su ruta relativa)
    for (list<string>::iterator itSF = sourceFiles.begin(); itSF != sourceFiles.end(); itSF++) {

        if(showProgress) cout << "* " << flush;

        casosCorrectos = 0;
        tipoResultado = "";

        //La lista de calificacion es borrada para cada alumno
        rating.clear();

        clog << "Evaluando el codigo " << *itSF << endl;

        SFsinRutaNiExtension = removeExtension(*itSF);
        positionInString=SFsinRutaNiExtension.find_last_of("/");
        SFsinRuta = SFsinRutaNiExtension.substr(positionInString+1, SFsinRutaNiExtension.length());
        if(NORMAL_NAME_CONVENTION)
        {
            SFsinRutaNiExtension = SFsinRutaNiExtension.substr(positionInString+1+(problem.length()), SFsinRutaNiExtension.length());
            calificaciones << "Estudiante " << SFsinRutaNiExtension << "\t";
        }
        else
        {
            SFsinRutaNiExtension = SFsinRutaNiExtension.substr(positionInString+1, SFsinRutaNiExtension.length());
            calificaciones << "Programa " << SFsinRutaNiExtension << "\t";
        }
        rating.push_back (*itSF);

        if (!forceValidLang(lang, *itSF)) {
            cerr << "La extensión de " << *itSF << " no coincide con un lenguaje conocido." << endl;
        }

        /*
        *   Compilación
        */
        if (pipe(fd_pipe_comp) < 0) {
            cerr << "No se pudo hacer pipe para la compilacion" << endl;
            return 1;
        }
        pID = fork();
        if (pID == 0) {
            close(fd_pipe_comp[0]);
            dup2(fd_pipe_comp[1], STDERR_FILENO);

            if (lang == "c") {
                comando = "execl(\"/usr/bin/gcc\", \"gcc\", \"" + *itSF + "\", \"-o\", \"exec_alumno\", \"-lm\", (char *) 0);";
                clog << "Compilando con el comando " << comando << endl;
                if (execl("/usr/bin/gcc", "gcc", (*itSF).c_str(), "-o", "exec_alumno", "-lm", (char *) 0) < 0)
                    perror("exec");
            } else if (lang == "c++") {
                comando = "execl(\"/usr/bin/g++\", \"g++\", \"" + *itSF + "\", \"-o\", \"exec_alumno\", \"-lm\", (char *) 0);";
                clog << "Compilando con el comando " << comando << endl;
                if (execl("/usr/bin/g++", "g++", (*itSF).c_str(), "-o", "exec_alumno", "-lm", (char *) 0) < 0)
                    perror("exec");
            } else if (lang == "java") {
                comando = "execl(\"/usr/bin/gcj\", \"gcj\", \"--main=\"" + *itSF + ").c_string(), (" + *itSF + ").c_string(), \"-o\", \"exec_alumno\", (char *) 0);";
                clog << "Compilando con el comando " << comando << endl;

                SFsinRutaNiExtension += ".class";
                comando = "gcj --main=" + SFsinRuta + " " + *itSF + " -o exec_alumno";
                string segundoParam = "--main=" + SFsinRuta;
                if (execl("/usr/bin/gcj", "gcj", segundoParam.c_str(), (*itSF).c_str(), "-o", "exec_alumno", (char *) 0) < 0)
                    perror("exec");
            }
        } else if (pID < 0) {
            cerr << "No se pudo hacer el fork para la compilacion" << endl;
        }

        close(fd_pipe_comp[1]);         //Cierro este, porque el padre de compilacion no necesita mandarle algo al hijo por el pipe.
        FILE* child_error = fdopen(fd_pipe_comp[0], "r");   //Lee desde el pipe de compilación toda la salida estandar enviado el hijo.

		initResource(usedResources);
		limitExceded = LIMIT_NONE;
		while(waitpid(pID, &status, WNOHANG) == 0) {
			getMaxResourceUsage(pID, usedResources);
			if (usedResources.time > 1000 * maxCompTime / sysconf(_SC_CLK_TCK)) {
				limitExceded = LIMIT_TIME;
				kill(pID, SIGKILL);
			}
			else if (usedResources.mem > maxCompMem * 1024 * 1024) {
				limitExceded = LIMIT_MEM;
				kill(pID, SIGKILL);
			}
		}

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
			clog << "Compilación correcta...." << endl;
        }
        else {
        	tipoResultado = "CE";
			clog << "Falló la compilación." << endl;

        	if (WIFEXITED(status)) {
        		// Volcar errores
        		if (fgets(buffer, sizeof(buffer), child_error) != NULL) {
					clog << "Compilador dijo:" << endl;
					clog << buffer;
					if (!feof(child_error)) clog << "(y sigue...)" << endl;
        		}
        	}
        	else if (WIFSIGNALED(status)) {
				if (limitExceded == LIMIT_TIME) clog << "Exceso de tiempo" << endl;
				else if (limitExceded == LIMIT_MEM) clog << "Exceso de memoria" << endl;
				else clog << "Matado misteriosamente por la señal " << WTERMSIG(status) << endl;
        	}
        }

        clog << endl;

        if (tipoResultado != "CE") {
            //Ciclo para cada caso de prueba. (Casos)
            for (list<string>::iterator itTC = testCases.begin(); itTC != testCases.end(); itTC++) {

                clog << "  Probando con caso " << *itTC << endl;

                pID = fork();   //Fork de Evaluación

                if (pID == 0) {     //Código que sólo ejecuta el hijo: Ejecutar los programas
                    freopen("/dev/null", "w", stderr);       //El error estandar del hijo se redirige a nulo.
                    freopen("salida_exec_alumno", "w", stdout);

                    freopen((*itTC + "." CASE_EXTENSION).c_str(), "r", stdin);   //El hijo de evaluación abre como entrada estandar el caso de prueba.

                    //Ejecución
                    comando = "exec_alumno";
                    clog << "  Ejecuto el programa " << comando << endl;
                    programa = execl(comando.c_str(), comando.c_str(), (char *)NULL);
                    if (programa == -1) {
                        //Si no se logra ejecutar correctamente el programa, se guarda un RE (runtime error)
                        rating.push_back ("RE");
                        cerr << "Error de ejecución" << endl;
                        return 0;
                    }
                    fclose(stdout);
                }
                else if (pID < 0) {
                    cerr << "No se pudo hacer el fork para la ejecución" << endl;
                }

                exito = false;
                initResource(usedResources);
				limitExceded = LIMIT_NONE;
				while(waitpid(pID, &status, WNOHANG) == 0) {
					getMaxResourceUsage(pID, usedResources);
					if (usedResources.time > 1000 * maxRunTime / sysconf(_SC_CLK_TCK)) {
						limitExceded = LIMIT_TIME;
						kill(pID, SIGKILL);
					}
					else if (usedResources.mem > maxRunMem * 1024 * 1024) {
						limitExceded = LIMIT_MEM;
						kill(pID, SIGKILL);
					}
				}

				if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
					exito = true;
				}
				else {

					if (WIFEXITED(status)) {
						tipoResultado = "RET";
						clog << "Terminado misteriosamente con estado" << WEXITSTATUS(status) << endl;
					}
					else if (WIFSIGNALED(status)) {
						if (limitExceded == LIMIT_TIME) {
							tipoResultado = "TLE";
							clog << "Exceso de tiempo" << endl;
						}
						else if (limitExceded == LIMIT_MEM) {
							tipoResultado = "MLE";
							clog << "Exceso de memoria" << endl;
						}
						else {
							tipoResultado = "SIG";
							clog << "Matado misteriosamente por la señal " << WTERMSIG(status) << endl;
						}
					}
				}

                // Si merece la pena evaluarlo
                if (exito) {
                    clog << "Ejecución correcta...." << endl;
                    strs.str("");
                    strs << usedResources.time / sysconf(_SC_CLK_TCK);
                    str = strs.str();


                    //el programa ya fue compilado y esta listo para ejecutarse
                    clog << "  Comparo con el archivo: " << (*itTC + "." + OUTPUT_EXTENSION) << endl;

                    /**
                    *   Juez Normal
                    **/

                    if (judgeType == "standard") {
                        if (juezNormal(strictEval, (*itTC + "." + OUTPUT_EXTENSION), "salida_exec_alumno")) //Envío el pipe donde está la salida de la ejecución.
                        {
                            clog << "  Caso " << (*itTC + "." + OUTPUT_EXTENSION) << " estuvo bien en modo ";
                            if (strictEval)
                                clog << "estricto." << endl;
                            else
                                clog << "normal." << endl;
                            casosCorrectos++;
                            rating.push_back ("1 (" + str +" ms)");
                        } else {
                            rating.push_back ("0 (" + str +" ms)");
                            clog << "  Falla el caso " << (*itTC + "." + OUTPUT_EXTENSION) << " en modo ";
                            if (strictEval)
                                clog << "estricto." << endl;
                            else
                                clog << "normal." << endl;
                        }
                    }
                    /**
                    *   Juez Especial
                    **/
                    else if (judgeType == "especial") {

                    }
                    /**
                    *   Juez Interactivo
                    **/
                    else if (judgeType == "interactive") {

                    }
                    clog << endl;
                }
                close(fd_pipe_eval[0]);
            }   //Cierra el ciclo TC
            if (strictEval) {
                clog << "Tuvo " << casosCorrectos << " casos correctos de " << testCases.size() << " casos de prueba." << endl;
                if (casosCorrectos == testCases.size()) {

                    clog << "ACCEPTED" << endl;
                    clog << "La calificación es: 10" << endl;
                    tipoResultado = "AC";
                } else {
                    clog << "WRONG ANSWER" << endl;
                    tipoResultado = "WA";
                    clog << "La calificación es: 0" << endl;
                }
            } else {
                clog << "Tuvo " << casosCorrectos << " casos correctos de " << testCases.size() << " casos de prueba." << endl;
                tipoResultado = (((double)casosCorrectos/testCases.size()*100.0) >= 60.0)?"AC":"WA";
                clog << "La calificación es: " << ((double)casosCorrectos/testCases.size()*100.0) << endl;
            }
            clog << endl;
        }
        rating.push_back (tipoResultado);
        remove("exec_alumno");

        if (tipoResultado == "AC") {
            calificaciones << "AC  Calificación  " << ((double)casosCorrectos/testCases.size()*100.0);

            strs.str("");
            strs.clear();
            strs << ((double)casosCorrectos/testCases.size()*100.0);
            str = strs.str();

            rating.push_back (str);
        } else if (tipoResultado == "WA") {
            if (casosCorrectos == 0) {
                calificaciones << "WA  Calificación  0";
                rating.push_back ("0");
            } else {
                calificaciones << "WA  Calificación  " << ((double)casosCorrectos/testCases.size()*100.0);
                strs.str("");
                strs.clear();
                strs << ((double)casosCorrectos/testCases.size()*100.0);
                str = strs.str();

                rating.push_back (str);
            }
        } else if (tipoResultado == "CE") {
            calificaciones << "CE  Calificación  0";
        } else if (tipoResultado == "TLE") {

        } else if (tipoResultado == "MLE") {

        }
        if(lang == "c")
            calificaciones << "\t(gcc)";
        else if(lang == "c++")
            calificaciones << "\t(g++)";
        else if(lang == "java")
            calificaciones << "\t(gcj)";
        calificaciones << endl;
        //TODO
        ratingsList.push_back(rating);
    }
    //cout << "Estoy por abrir archivo";
    string resultsFile = "calificaciones.csv";
    ofstream outputResults(resultsFile.c_str());         //Archivo de los resultados en csv

    if (outputResults.fail()) {
        cout << "No se pudo abrtir el archivo";
    }
    //cout << "Se supone que ya abri el archivo";
    while (!ratingsList.empty()) {

        rating = ratingsList.front();

        while (!rating.empty()) {
            //cout << ".";
            outputResults << rating.front() << ","; //modificar la coma para que pueda ser otro operador
            rating.pop_front();
        }
        outputResults << endl;
        ratingsList.pop_front();
    }
    cout << endl;
    system("rm salida_exec_alumno");
}
