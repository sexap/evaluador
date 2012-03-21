// Variables accesibles desde el exterior
// -- AQUI
{
	// Variables internas (no existen fuera de esta sección)
    int status, fd_pipe_comp[2], pipe_run_in[2], pipe_run_out[2];
    pid_t pID, jID;
    char buffer[512];
    string comando, lang;
    unsigned int casosCorrectos, califFinal;
    bool goodComp, goodRun;
    ParamHolder compParams, runParams;

    resource_t usedResources;
    rlimit limitVar;
    enum {LIMIT_NONE, LIMIT_TIME, LIMIT_MEM, LIMIT_SLEEP};
    int limitExceded;

    // logJE
    ofstream clogJE("logJE.txt", fstream::app);

    // logJN
    ofstream clogJN("logJN.txt", fstream::app);

    //Se inicializa el reporte
    Reporte reporte(removeExtension(getFileName(outputFile.c_str())),problem,judgeType,testCases);

    clog << "Iniciando evaluación..." << endl;
    clog << "El problema es: " << problem << endl;
    clog << "El tipo de juez es: " << judgeType << endl;
    if(judgeNeedsExe(judgeType)) clog << "El ejecutable del juez es: " << judgeExe << endl;
    clog << "Evaluación estricta: " << (strictEval?"sí":"no") << endl;
    clog << "Comparar espacios: " << (compareWhite?"sí":"no") << endl;
    clog << "Resultados se guardarán en: " << outputFile << ".xxx" << endl;
    clog << endl;

    //Ciclo para cada programa de alumno. (Fuentes con su ruta relativa)
    for (list<string>::iterator itSF = sourceFiles.begin(); itSF != sourceFiles.end(); itSF++) {

        if(showProgress) cout << "* " << flush;

        casosCorrectos = 0;

        clog << "Evaluando el codigo " << *itSF << endl;
        if(judgeType == "standard")
        	clogJN << "\tEvaluando el codigo " << *itSF << endl;
        else if(judgeType == "special")
			clogJE << "\tEvaluando el codigo " << *itSF << endl;

		/******************
		 *   Parámetros   *
		 ******************/
		compParams.clear();
		runParams.clear();

		forceValidLang(lang, *itSF);
		if (lang == "c") {
			compParams.add("gcc");
			compParams.add(*itSF);
			compParams.add("-o");
			compParams.add("exec_alumno");
			compParams.add("-Werror=main");

			runParams.add("exec_alumno");
		} else if (lang == "c++") {
			compParams.add("g++");
			compParams.add(*itSF);
			compParams.add("-o");
			compParams.add("exec_alumno");
			compParams.add("-Werror=main");

			runParams.add("exec_alumno");
		} else if (lang == "java") {
			compParams.add("gcj");
			compParams.add(*itSF);
			compParams.add("--main=" + removeExtension(getFileName(*itSF)));
			compParams.add("-o");
			compParams.add("exec_alumno");

			runParams.add("exec_alumno");
		}

		//TODO: Hacer solo muestre los numeros
		if (ZARAGOZA_NAME_CONVENTION)   reporte.nuevoAlumno(getFileName(removeExtension(*itSF)), compParams.exe());
		else reporte.nuevoAlumno(*itSF, compParams.exe());

        /*******************
		 *   Compilación   *
		 *******************/
        if (pipe(fd_pipe_comp) < 0) {
            cerr << "Falló pipe() para la compilación" << endl;
            perror("Error");
            return 1;
        }

        pID = fork();
        if (pID < 0) {
            cerr << "Falló fork() para la compilación" << endl;
            perror("Error");
            return 1;
        }
        else if (pID == 0) {
        	// Cerrar pipe de lectura
            close(fd_pipe_comp[0]);
            //Enviar stderr a pipe escritura
            dup2(fd_pipe_comp[1], STDERR_FILENO);
            close(fd_pipe_comp[1]);

			// Compilación
			clog << "Compilando con comando '" << compParams << "'" << endl;
			if (execvp(compParams.exe(), compParams.params()) < 0) {
				cerr << "Falló execvp() para la compilación" << endl;
				perror("Error");
			}

            return 1;
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
        fclose(child_error); //TODO: Revisar documentación (por el pipe)
        clog << endl;

		//Ciclo para cada caso de prueba. (Casos)
		for (list<string>::iterator itTC = testCases.begin(); itTC != testCases.end(); itTC++) {

			if (!goodComp) {
				reporte.agregarResultadoCasoPrueba("CE");
				continue;
			}

			clog << "  Probando con caso " << *itTC << endl;

			/*****************
			 *   Ejecución   *
			 *****************/
			//Juez Interactivo
			if (judgeType == "interactive") {
				// Crear pipes
				if (pipe(pipe_run_in) < 0 || pipe(pipe_run_out) < 0) {
					cerr << "Falló pipe() para la ejecución" << endl;
					perror("Error");
					return 1;
				}
				// Ejecutar
				jID = juezInteractivoStart(*itTC + "." + CASE_EXTENSION, judgeExe, pipe_run_in[1], pipe_run_out[0]);
			}

			pID = fork();
			if (pID < 0) {
				cerr << "Falló fork() para la ejecución" << endl;
				perror("Error");
				return 1;
			}
			else if (pID == 0) {
				//Redirigir error -> al vacio
				freopen("/dev/null", "w", stderr);

				if (judgeType == "interactive") {
					// Redirige salida -> pipe out
					dup2(pipe_run_out[1], STDOUT_FILENO);
					// Redirige entrada <- pipe in
					dup2(pipe_run_in[0], STDIN_FILENO);
				}
				else {
					// Redirigir salida -> archivo
					freopen("salida_exec_alumno", "w", stdout);
					// Redirigir entrada <- caso
					freopen((*itTC + "." CASE_EXTENSION).c_str(), "r", stdin);
				}

				// Restricciones de ejecución (parte 1)
				limitVar.rlim_cur = limitVar.rlim_max = maxOutSize * 1024;
				setrlimit(RLIMIT_FSIZE, &limitVar);

				// Ejecución
				clog << "  Ejecutando con comando '" << runParams << "'" << endl;
				if (execv(runParams.exe(), runParams.params()) < 0) {
					//Si no se logra ejecutar correctamente el programa, se guarda un RE (runtime error)
					reporte.agregarResultadoCasoPrueba("RE");
					cerr << "Falló execv() para la ejecución" << endl;
					perror("Error");
				}
				return 1;
			} // Termina el hijo de ejecución

			initResource(usedResources);
			limitExceded = LIMIT_NONE;
			while(true) {
				// Espera a los hijos
				if (waitpid(pID, &status, WNOHANG) != 0) {
					cerr << "Terminó usuario. Matando JI" << endl; //!
					if (judgeType == "interactive") kill(jID, SIGKILL);
					break;
				}
				if (judgeType == "interactive" && waitpid(jID, &status, WNOHANG) != 0) {
					cerr << "Terminó JI. Matando a usuario" << endl; //!
					kill(pID, SIGKILL);
					break;
				}
				// Restricciones de ejecución (parte 2)
				getMaxResourceUsage(pID, usedResources);
				if (usedResources.time / (sysconf(_SC_CLK_TCK) / 1000.0) > maxRunTime) {
					limitExceded = LIMIT_TIME;
					kill(pID, SIGKILL);
				}
				if (usedResources.sleep_time > 50000) {
					limitExceded = LIMIT_SLEEP;
					kill(pID, SIGKILL);
				}
				else if (usedResources.mem > maxRunMem * 1024 * 1024) {
					limitExceded = LIMIT_MEM;
					kill(pID, SIGKILL);
				}

			}

			// Verifica estado final de programa alumno
			if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
				clog << "  Ejecución correcta...." << endl;
				goodRun = true;
			}
			else {
				goodRun = false;
				if (WIFEXITED(status)) {
					reporte.agregarResultadoCasoPrueba("BAD RET");
					clog << "  Terminó misteriosamente con valor de retorno " << WEXITSTATUS(status) << endl;
				}
				else if (WIFSIGNALED(status)) {
					if (limitExceded == LIMIT_TIME) {
						reporte.agregarResultadoCasoPrueba("LIM TIME");
						clog << "  Exceso de tiempo" << endl;
					}
					else if (limitExceded == LIMIT_SLEEP) {
						reporte.agregarResultadoCasoPrueba("LIM SLP");
						clog << "  Exceso de inactividad" << endl;
					}
					else if (limitExceded == LIMIT_MEM) {
						reporte.agregarResultadoCasoPrueba("LIM MEM");
						clog << "  Exceso de memoria" << endl;
					}
					else if (WTERMSIG(status) == SIGXFSZ) {
						reporte.agregarResultadoCasoPrueba("LIM OUT");
						clog << "  Exceso de salida (SIGXFSZ)" << endl;
					}
					else if (WTERMSIG(status) == SIGSEGV) {
						reporte.agregarResultadoCasoPrueba("ERR MEM");
						clog << "  Violación de segmento (SIGSEGV)" << endl;
					}
					else if (WTERMSIG(status) == SIGBUS) {
						reporte.agregarResultadoCasoPrueba("ERR MEM");
						clog << "  Error de bus (SIGBUS)" << endl;
					}
					else if (WTERMSIG(status) == SIGFPE) {
						reporte.agregarResultadoCasoPrueba("ERR MATH");
						clog << "  Excepción de punto flotante (SIGFPE)" << endl;
					}
					else {
						reporte.agregarResultadoCasoPrueba("SIG ?");
						clog << "  Matado misteriosamente por la señal " << WTERMSIG(status) << endl;
					}
				}
			} // Fin de restricciones

			// Finaliza y espera juez interactivo
			if (judgeType == "interactive") {
				if (!goodRun) kill(jID, SIGKILL);
				juezInteractivoEnd(jID);
			}

			/******************
			 *   Evaluación   *
			 ******************/
			// Si merece la pena evaluarlo
			if (goodRun) {

				/**
				*   Juez Normal
				**/
				if (judgeType == "standard") {
					clog << "  Comparo con el archivo: " << (*itTC + "." + OUTPUT_EXTENSION) << endl;
					if (juezNormal(compareWhite, (*itTC + "." + OUTPUT_EXTENSION) ))
					{
						clog << "bien" << endl;
						casosCorrectos++;
						reporte.agregarResultadoCasoPrueba(1,usedResources.time / (sysconf(_SC_CLK_TCK) / 1000.0));
					} else {
						clog << "mal" << endl;
						reporte.agregarResultadoCasoPrueba(0,usedResources.time / (sysconf(_SC_CLK_TCK) / 1000.0));
					}
				}

				/**
				*   Juez Especial
				**/
				else if (judgeType == "special")
				{
					clog << "  Ejecutando juez especial" << endl;
					//TODO: Jueces especiales califican de 0 a 100
					if (juezEspecial(*itTC, judgeExe) == 100)
					{
						clog << "bien" << endl;
						casosCorrectos++;
						reporte.agregarResultadoCasoPrueba(1,usedResources.time / (sysconf(_SC_CLK_TCK) / 1000.0));
					} else
					{
						clog << "mal" << endl;
						reporte.agregarResultadoCasoPrueba(0,usedResources.time / (sysconf(_SC_CLK_TCK) / 1000.0));
					}
				}

				/**
				*   Juez Interactivo
				**/
				else if (judgeType == "interactive") {
					//TODO: Jueces especiales califican de 0 a 100
					if (juezInteractivoResult() == 100)
					{
						clog << "bien" << endl;
						casosCorrectos++;
						reporte.agregarResultadoCasoPrueba(1,usedResources.time / (sysconf(_SC_CLK_TCK) / 1000.0));
					} else
					{
						clog << "mal" << endl;
						reporte.agregarResultadoCasoPrueba(0,usedResources.time / (sysconf(_SC_CLK_TCK) / 1000.0));
					}
				}
			}

			clog << endl;
		} // Termina for de casos de prueba
		remove("exec_alumno");
		remove("salida_exec_alumno");

		clog << "Tuvo " << casosCorrectos << "/" << testCases.size() << " casos correctos." << endl;
		califFinal = (double)casosCorrectos / testCases.size() * 100.0;
		if (strictEval && califFinal < 100) califFinal = 0;
		reporte.terminarEvaluacionUsuario(califFinal);
    }
    cout << endl;

    //Salida de los resultados

    if(hasExtension(outputFile,"txt"))  reporte.imprimirResultadoAmigable();
    else if(hasExtension(outputFile,"csv")) reporte.imprimirResultadoCSV();
    else if(hasExtension(outputFile,"html")) reporte.imprimirResultadoHTML();
    else{
        reporte.imprimirResultadoAmigable();
        reporte.imprimirResultadoCSV();
        reporte.imprimirResultadoHTML();
    }

}
