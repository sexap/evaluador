// Variables accesibles desde el exterior
// -- AQUI
{
	// Variables internas (no existen fuera de esta sección)
    string tipoResultado;
    int programa, status, fd_pipe_comp[2];
    pid_t pID;
    char buffer[512];
    string comando, lang, compiler;
    unsigned int casosCorrectos, califFinal;
    bool goodComp, goodRun;

    resource_t usedResources;
    enum {LIMIT_NONE, LIMIT_TIME, LIMIT_MEM};
    int limitExceded;

    // logJE
    ofstream clogJE("logJE.txt", fstream::app);

    // logJN
    ofstream clogJN("logJN.txt", fstream::app);

    //Se inicializa el reporte
    Reporte reporte(outputFile.c_str(),problem,judgeType);

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
        tipoResultado = "";

        clog << "Evaluando el codigo " << *itSF << endl;
        if(judgeType == "standard")
        	clogJN << "\tEvaluando el codigo " << *itSF << endl;
        else if(judgeType == "special")
			clogJE << "\tEvaluando el codigo " << *itSF << endl;

        forceValidLang(lang, *itSF);

        if(lang == "c")
            compiler = "gcc";
        else if(lang == "c++")
            compiler = "g++";
        else if(lang == "java")
            compiler = "gcj";

        //TODO: Hacer solo muestre los numeros
		if (ZARAGOZA_NAME_CONVENTION)   reporte.nuevoAlumno(getFileName(removeExtension(*itSF)),compiler);
		else reporte.nuevoAlumno(*itSF,compiler);

        /*******************
		 *   Compilación   *
		 *******************/
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
				reporte.terminarEvaluacionUsuario("CE");
				continue;
			}

			clog << "  Probando con caso " << *itTC << endl;

			/*****************
			 *   Ejecución   *
			 *****************/
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
					reporte.terminarEvaluacionUsuario("RE");
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
					reporte.agregarResultadoCasoPrueba("BAD RET");
					clog << "  Terminó misteriosamente con valor de retorno " << WEXITSTATUS(status) << endl;
				}
				else if (WIFSIGNALED(status)) {
					if (limitExceded == LIMIT_TIME) {
						reporte.agregarResultadoCasoPrueba("LIM TIME");
						clog << "  Exceso de tiempo" << endl;
					}
					else if (limitExceded == LIMIT_MEM) {
						reporte.agregarResultadoCasoPrueba("LIM MEM");
						clog << "  Exceso de memoria" << endl;
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


			// Si merece la pena evaluarlo
			if (goodRun) {
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
					clog << "  Caso " << *itTC << " estuvo ";
					if (juezEspecial(*itTC, judgeExe))
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
    reporte.imprimirResultadoCVS();
    reporte.imprimirResultadoAmigable();
}
