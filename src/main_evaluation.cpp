// Variables accesibles desde el exterior
// -- AQUI
{
    //Comento las variables que use para CVS. (Eventualmente quitar)
    list<list <string> > ratingsList;
    list<string> rating;
    ostringstream strs;
    string str;

	// Variables internas (no existen fuera de esta sección)
    //string rutaFuentes, rutaCasos;
    string tipoResultado; //, archCal;
    //string SFsinRuta,  SFsinRutaNiExtension;
    int programa, status, fd_pipe_comp[2];
    pid_t pID;
    char buffer[512];
    string comando, lang;
    unsigned int casosCorrectos, califFinal;
    bool goodComp, goodRun;

    resource_t usedResources;
    enum {LIMIT_NONE, LIMIT_TIME, LIMIT_MEM};
    int limitExceded;

	//Archivo de los resultados en txt
    ofstream calificaciones((outputFile + ".txt").c_str());

    //Se inicializa el reporte
    Reporte reporte((outputFile + ".txt").c_str(),problem,judgeType);

    clog << "Iniciando evaluación..." << endl;
    clog << "El problema es: " << problem << endl;
    clog << "El tipo de juez es: " << judgeType << endl;
    if(judgeType == "standard")
        clog << "El modo de evaluación es: " << (strictEval?"estricto":"normal") << endl;
    if(judgeType == "special")
        clog << "El juez especial es: " << judgeExe << endl;
    clog << "Resultados se guardarán en: " << outputFile << ".xxx" << endl;
    clog << endl;

    calificaciones << "Calificaciones de " << problem << " con SEAP\n\n";

    //Ciclo para cada programa de alumno. (Fuentes con su ruta relativa)
    for (list<string>::iterator itSF = sourceFiles.begin(); itSF != sourceFiles.end(); itSF++) {

        if(showProgress) cout << "* " << flush;

        casosCorrectos = 0;
        tipoResultado = "";

        //La lista de calificacion es borrada para cada alumno
        rating.clear();

        clog << "Evaluando el codigo " << *itSF << endl;

        ///SFsinRuta = getFileName(*itSF);
        ///SFsinRutaNiExtension = removeExtension(SFsinRuta);

		if (ZARAGOZA_NAME_CONVENTION) {
			//TODO: Hacer solo muestre los numeros
			calificaciones << "Estudiante " << getFileName(removeExtension(*itSF)) << "  ";
			reporte.nuevoAlumno(getFileName(removeExtension(*itSF)));
		}
		else {
			calificaciones << "Programa " << *itSF << "  ";
			reporte.nuevoAlumno(*itSF);
		}

        rating.push_back (*itSF);

        forceValidLang(lang, *itSF);

        /*
        *   Compilación
        */
        if (pipe(fd_pipe_comp) < 0) {
            cerr << "No se pudo hacer pipe para la compilacion" << endl;
            return 1;
        }

        pID = fork();
        if (pID < 0) {
            cerr << "No se pudo hacer el fork para la compilacion" << endl;
        }
        else if (pID == 0) {
            close(fd_pipe_comp[0]);
            dup2(fd_pipe_comp[1], STDERR_FILENO);
            close(fd_pipe_comp[1]);

			//TODO: Agregar Werror=main
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

                string segundoParam = "--main=" + removeExtension(getFileName(*itSF));
                if (execl("/usr/bin/gcj", "gcj", (*itSF).c_str(), segundoParam.c_str(), "-o", "exec_alumno", (char *) 0) < 0)
                    perror("exec");
            }
        } // Termina el hijo

        close(fd_pipe_comp[1]); // El padre no necesita mandarle datos al hijo
        FILE* child_error = fdopen(fd_pipe_comp[0], "r");

		// Restricciones de compilación
		initResource(usedResources);
		limitExceded = LIMIT_NONE;
		while(waitpid(pID, &status, WNOHANG) == 0) {
			getMaxResourceUsage(pID, usedResources);
			if (usedResources.time / (sysconf(_SC_CLK_TCK) / 1000.0) > maxCompTime) {
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
			goodComp = true;
        }
        else {
			clog << "Falló la compilación." << endl;
			goodComp = false;

        	if (WIFEXITED(status)) {
        		// Volcar errores
        		clog << "Compilador dijo:" << endl;
        		for (int errCount = 0; errCount < MAX_ERR_MSG; errCount++) {
					if (fgets(buffer, sizeof(buffer), child_error) == NULL) break;
					clog << "  " << buffer;
        		}
        		if (fgets(buffer, sizeof(buffer), child_error) != NULL) clog << "  (Y sigue...)" << endl;
        	}
        	else if (WIFSIGNALED(status)) {
				if (limitExceded == LIMIT_TIME) clog << "Exceso de tiempo" << endl;
				else if (limitExceded == LIMIT_MEM) clog << "Exceso de memoria" << endl;
				else clog << "Matado misteriosamente por la señal " << WTERMSIG(status) << endl;
        	}
        } // Fin de las restricciones
        fclose(child_error);

        clog << endl;

		//Ciclo para cada caso de prueba. (Casos)
		for (list<string>::iterator itTC = testCases.begin(); itTC != testCases.end(); itTC++) {

			if (!goodComp) {
				rating.push_back ("CE");
				continue;
			}

			clog << "  Probando con caso " << *itTC << endl;

			//Fork de Evaluación
			pID = fork();
			if (pID < 0) {
				cerr << "No se pudo hacer el fork para la ejecución" << endl;
			}
			else if (pID == 0) {
				freopen("/dev/null", "w", stderr); //Redirigir error -> vacio
				freopen("salida_exec_alumno", "w", stdout); //Redirigir salida -> archivo
				freopen((*itTC + "." CASE_EXTENSION).c_str(), "r", stdin); //Redirigir caso -> entrada

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
			} // Termina el hijo de ejecución

			// Restricciones de ejecución
			initResource(usedResources);
			limitExceded = LIMIT_NONE;
			while(waitpid(pID, &status, WNOHANG) == 0) {
				getMaxResourceUsage(pID, usedResources);
				if (usedResources.time / (sysconf(_SC_CLK_TCK) / 1000.0) > maxRunTime) {
					limitExceded = LIMIT_TIME;
					kill(pID, SIGKILL);
				}
				else if (usedResources.mem > maxRunMem * 1024 * 1024) {
					limitExceded = LIMIT_MEM;
					kill(pID, SIGKILL);
				}
			}

			if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
				clog << "  Ejecución correcta...." << endl;
				goodRun = true;
			}
			else {
				goodRun = false;
				if (WIFEXITED(status)) {
					rating.push_back ("BAD RET");
					clog << "  Terminó misteriosamente con valor de retorno " << WEXITSTATUS(status) << endl;
				}
				else if (WIFSIGNALED(status)) {
					if (limitExceded == LIMIT_TIME) {
						rating.push_back ("LIM TIME");
						clog << "  Exceso de tiempo" << endl;
					}
					else if (limitExceded == LIMIT_MEM) {
						rating.push_back ("LIM MEM");
						clog << "  Exceso de memoria" << endl;
					}
					else if (WTERMSIG(status) == SIGSEGV) {
						rating.push_back ("ERR MEM");
						clog << "  Violación de segmento (SIGSEGV)" << endl;
					}
					else if (WTERMSIG(status) == SIGFPE) {
						rating.push_back ("ERR MATH");
						clog << "  Excepción de punto flotante (SIGFPE)" << endl;
					}
					else {
						rating.push_back ("SIG ?");
						clog << "  Matado misteriosamente por la señal " << WTERMSIG(status) << endl;
					}
				}
			} // Fin de restricciones


			// Si merece la pena evaluarlo
			if (goodRun) {
				strs.str("");
				strs << usedResources.time / sysconf(_SC_CLK_TCK);
				str = strs.str();

				//el programa ya fue compilado y esta listo para ejecutarse
				clog << "  Comparo con el archivo: " << (*itTC + "." + OUTPUT_EXTENSION) << endl;

				/**
				*   Juez Normal
				**/

				if (judgeType == "standard") {
					clog << "  Caso " << *itTC << " estuvo ";
					if (juezNormal(compareWhite, (*itTC + "." + OUTPUT_EXTENSION))) //Envío el pipe donde está la salida de la ejecución.
					{
						clog << "bien" << endl;
						casosCorrectos++;
						rating.push_back ("1 (" + str +" ms)");
						reporte.agregarResultadoCasoPrueba(1,usedResources.time / sysconf(_SC_CLK_TCK));
					} else {
						clog << "mal" << endl;
						rating.push_back ("0 (" + str +" ms)");
						reporte.agregarResultadoCasoPrueba(0,usedResources.time / sysconf(_SC_CLK_TCK));
					}
				}
				/**
				*   Juez Especial
				**/
				else if (judgeType == "special")
				{
					clog << "  Caso " << *itTC << " estuvo ";
					if (juezEspecial(*itTC + "." + OUTPUT_EXTENSION))
					{
						clog << "bien" << endl;
						casosCorrectos++;
						reporte.agregarResultadoCasoPrueba(1,usedResources.time / sysconf(_SC_CLK_TCK));
						rating.push_back ("1 (" + str +" ms)");
					} else
					{
						clog << "mal" << endl;
						reporte.agregarResultadoCasoPrueba(0,usedResources.time / sysconf(_SC_CLK_TCK));
						rating.push_back ("0 (" + str +" ms)");
					}
				}
				/**
				*   Juez Interactivo
				**/
				else if (judgeType == "interactive") {

				}
			}

			clog << endl;
		} // Termina for de casos de prueba
		remove("exec_alumno");

		clog << "Tuvo " << casosCorrectos << "/" << testCases.size() << " casos correctos." << endl;
		califFinal = (double)casosCorrectos / testCases.size() * 100.0;
		if (strictEval && califFinal < 100) califFinal = 0;

		strs.str("");
		strs.clear();
		strs << califFinal;
		str = strs.str();
		rating.push_back (str);
		ratingsList.push_back(rating);

		reporte.terminarEvaluacionUsuario(califFinal, "-");
		calificaciones << "  Calificación  " << califFinal;

        if(lang == "c")
            calificaciones << "\t(gcc)";
        else if(lang == "c++")
            calificaciones << "\t(g++)";
        else if(lang == "java")
            calificaciones << "\t(gcj)";
        calificaciones << endl;
    }

    //Archivo de los resultados en csv
    ofstream outputResults((outputFile + ".csv").c_str());

    if (outputResults.fail()) {
        cerr << "No se pudo abrtir el archivo";
    }

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
    //Salida de los resultados
    //reporte.imprimirResultadoAmigable();
}
