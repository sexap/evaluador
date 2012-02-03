// Variables accesibles desde el exterior
string judgeType, judgeExe, action, problem, outputFile;
int maxSourceSize, maxCompTime, maxCompMem, maxOutSize, maxRunTime, maxRunMem;
bool verbose, strictEval, compareWhite;
list<string> testCases, sourceFiles;

{
	// Variables internas (no existen fuera de esta sección)
	Config confArg, confFile;
	bool hasError;
	list<string> tmpList;
	stack<list<string>::iterator> toErase;

	// Registra variables
	confArg.registerArgFixVar("action", Config::T_STRING);
	confArg.registerArgFixVar("problem", Config::T_STRING);

	confArg.registerArgVar("f", Config::T_LIST, true);
	confArg.registerArgVar("s", Config::T_INT, false);
	confArg.registerArgVar("T", Config::T_INT, false);
	confArg.registerArgVar("M", Config::T_INT, false);
	confArg.registerArgVar("S", Config::T_INT, false);
	confArg.registerArgVar("c", Config::T_LIST, false);
	confArg.registerArgVar("v", Config::T_BOOL, false);
	confArg.registerArgVar("o", Config::T_STRING, false);

	confFile.registerFileVar("max_time", Config::T_INT, true);
	confFile.registerFileVar("max_mem", Config::T_INT, false);
	confFile.registerFileVar("judge_type", Config::T_STRING, true);
	confFile.registerFileVar("judge_exe", Config::T_STRING, false);
	confFile.registerFileVar("strict_eval", Config::T_BOOL, false);
	confFile.registerFileVar("compare_white", Config::T_BOOL, false);

	// Valores por default
	confArg.setValue("s", 24); // 24kB de código
	confArg.setValue("T", 5000); // 5s para compilar
	confArg.setValue("M", 4096); // 4MB para compilar (revisar)
	confArg.setValue("S", 8); // 8KB de salida
	confArg.setValue("v", false); // Es callado
	confArg.setValue("o", "");

	confFile.setValue("max_mem", 32768); // 32MB para ser ejecutado (revisar)
	confFile.setValue("judge_exe", "judge"); // Ejecutable del juez
	confFile.setValue("strict_eval", false); // No es estricto
	confFile.setValue("compare_white", false); // Ignora espacios extra

	// Analizar y valida argumentos
	confArg.parseArgs(argc, argv);
	if (!confArg.validate())
	{
		cerr << "Parametros incorrectos. Consulte el manual" << endl;
		return 1;
	}

	confArg.getValue("action", action);
	confArg.getValue("problem", problem);

	confArg.getValue("f", sourceFiles);
	confArg.getValue("s", maxSourceSize);
	confArg.getValue("T", maxCompTime);
	confArg.getValue("M", maxCompMem);
	confArg.getValue("S", maxOutSize);
	confArg.getValue("c", testCases);
	confArg.getValue("v", verbose);
	confArg.getValue("o", outputFile);

	hasError = false;
	if (!isValidAction(action))
	{
		cerr << action << " no es una acción válida" << endl;
		hasError = true;
	}
	if (!isDir(problem))
	{
		cerr << "No existe el problema " << problem << endl;
		hasError = true;
	}
	if (!isFile(problem + "/eval.conf"))
	{
		cerr << "No se encuentra " << problem << "/eval.conf" << endl;
		hasError = true;
	}
	if (!isBetween(maxSourceSize, 1, 512))
	{
		cerr << "el parametro -s debe estar entre 1kB y 512kB" << endl;
		hasError = true;
	}
	if (!isBetween(maxCompTime, 1, 90000))
	{
		cerr << "el parametro -T debe estar entre 1ms y 90s" << endl;
		hasError = true;
	}
	if (!isBetween(maxCompMem, 1, 262144))   // (revisar)
	{
		cerr << "el parametro -M debe estar entre 1kB y 256MB" << endl;
		hasError = true;
	}
	if (!isBetween(maxOutSize, 1, 256))
	{
		cerr << "el parametro -S debe estar entre 1kB y 256kB" << endl;
		hasError = true;
	}
	if (hasError) return 1;

	// Analizar y valida archivo de configureción
	confFile.parseFile(problem + "/eval.conf");
	if (!confFile.validate())
	{
		cerr << "Archivo erroneo. Consulta el manual" << endl;
		return 1;
	}

	confFile.getValue("max_time", maxRunTime);
	confFile.getValue("max_mem", maxRunMem);
	confFile.getValue("judge_type", judgeType);
	confFile.getValue("judge_exe", judgeExe);
	confFile.getValue("strict_eval", strictEval);
	confFile.getValue("compare_white", compareWhite);

	hasError = false;
	if (!isBetween(maxRunTime, 1, 60000))
	{
		cerr << "max_time debe estar entre 1ms y 60s" << endl;
		hasError = true;
	}
	if (!isBetween(maxRunMem, 1, 262144))
	{
		cerr << "max_mem debe estar entre 1kB y 256MB" << endl;
		hasError = true;
	}
	if (!isValidJudgeType(judgeType))
	{
		cerr << judgeType << " no es un tipo de juez válido" << endl;
		hasError = true;
	}
	else if (judgeNeedsExe(judgeType))
	{
		if (!isFile(problem + "/" + judgeExe))
		{
			cerr << "No se encuentra el ejecutable " << judgeExe << endl;
			hasError = true;
		}
	}
	if (hasError) return 1;

	//TODO: Posible loop infinito si un archivo .set se contiene a sí mismo

	/// ** Genera lista de casos de prueba **
	hasError = false;
	// Modo automático
	if (testCases.size() == 0)
	{
		// Obtiene los archivos .case de la carpeta
		testCases = Config::getDir(problem, CASE_EXTENSION);

		for (list<string>::iterator it = testCases.begin(); it != testCases.end(); it++)
		{
			// Quita extensión
			*it = removeExtension(*it);

			// Verifica existencia de salida esperada
			if (judgeNeedsOutput(judgeType) && !isFile(problem + "/" + *it + "." + OUTPUT_EXTENSION))
			{
				cerr << "No se encuentra la salida experada para el caso " << *it << endl;
				hasError = true;
			}
		}
	}
	// Modo manual
	else
	{
		for (list<string>::iterator it = testCases.begin(); it != testCases.end(); it++)
		{
			// Indicaron un archivo .set
			if (hasExtension(*it, "set"))
			{
				// Verifica existencia
				if (!isFile(problem + "/" + *it))
				{
					cerr << "No existe el archivo " << *it << endl;
					hasError = true;
				}
				// Agregar contenidos
				else
				{
					tmpList = Config::getSet(problem + "/" + *it);
					testCases.insert(testCases.end(), tmpList.begin(), tmpList.end());
					toErase.push(it);
				}
			}
			// Indicaron un caso normal
			else
			{
				// Verificamos existencia
				if (!isFile(problem + "/" + *it + "." + CASE_EXTENSION))
				{
					cerr << "No existe el caso " << *it << endl;
					hasError = true;
				}
				// Verificamos existencia de salida esperada
				else if (judgeNeedsOutput(judgeType) && !isFile(problem + "/" + *it + "." + OUTPUT_EXTENSION))
				{
					cerr << "No se encuentra la salida experada para el caso " << *it << endl;
					hasError = true;
				}
			}
		}

		while (!toErase.empty())
		{
			testCases.erase(toErase.top());
			toErase.pop();
		}

		// Quitar repetidos
		testCases.sort();
		testCases.unique();
	}
	if (hasError) return 1;

	/// ** Genera lista de codigos fuente ***
	hasError = false;
	if (sourceFiles.size() == 1 && isDir(*sourceFiles.begin()))
	{
		string baseName = *sourceFiles.begin();
		sourceFiles = Config::getDir(baseName);
		for (list<string>::iterator it = sourceFiles.begin(); it != sourceFiles.end(); it++)
		{
			*it = baseName + "/" + (*it);
		}
	}
	else
	{
		for (list<string>::iterator it = sourceFiles.begin(); it != sourceFiles.end(); it++)
		{
			if (!isFile(*it))
			{
				cerr << "No existe el archivo " << *it << endl;
				toErase.push(it);
			}
			else if (hasExtension(*it, "set"))
			{
				tmpList = Config::getSet(*it);
				sourceFiles.insert(sourceFiles.end(), tmpList.begin(), tmpList.end());
				toErase.push(it);
			}
			else if (!isFileSmaller(*it, maxSourceSize))
			{
				cerr << "El archivo " << *it << " es muy grande" << endl;
				toErase.push(it);
			}
		}
		while (!toErase.empty())
		{
			sourceFiles.erase(toErase.top());
			toErase.pop();
		}
		sourceFiles.sort();
		sourceFiles.unique();
	}
	if (sourceFiles.size() == 0)
	{
		cerr << "No hay casos por evaluar" << endl;
		hasError = true;
	}
	if (hasError) return 1;
}
