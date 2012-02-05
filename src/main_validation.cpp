//TODO: Revisar Mensajes de error de los limites

// Variables accesibles desde el exterior
string judgeType, judgeExe, action, problem, outputFile;
int maxSourceSize, maxCompTime, maxCompMem, maxOutSize, maxRunTime, maxRunMem;
bool verbose, showProgress, strictEval, compareWhite;
list<string> testCases, sourceFiles;

{
	// Variables internas (no existen fuera de esta sección)
	Config confArg, confFile;
	bool hasError;
	string dumbString;
	list<string> tmpList;
	list<string>::iterator it, tmp_it;
	stack<list<string>::iterator> toErase;

	// Registra variables
	confArg.registerArgFixVar("action", Config::T_STRING);
	confArg.registerArgFixVar("problem", Config::T_STRING);

	confArg.registerArgVar("s", Config::T_INT, false);
	confArg.registerArgVar("T", Config::T_INT, false);
	confArg.registerArgVar("M", Config::T_INT, false);
	confArg.registerArgVar("S", Config::T_INT, false);

	confArg.registerArgVar("f", Config::T_LIST, true);
	confArg.registerArgVar("c", Config::T_LIST, false);
	confArg.registerArgVar("o", Config::T_STRING, false);
	confArg.registerArgVar("v", Config::T_BOOL, false);
	confArg.registerArgVar("nb", Config::T_BOOL, false);

	confFile.registerFileVar("max_time", Config::T_INT, true);
	confFile.registerFileVar("max_mem", Config::T_INT, false);
	confFile.registerFileVar("judge_type", Config::T_STRING, true);
	confFile.registerFileVar("judge_exe", Config::T_STRING, false);
	confFile.registerFileVar("strict_eval", Config::T_BOOL, false);
	confFile.registerFileVar("compare_white", Config::T_BOOL, false);

	// Valores por default
	// TODO: Revisar valores cuando las restricciones funcionen (también abajo en la validación)
	confArg.setValue("s", 24); // 24kB de código
	confArg.setValue("T", 5000); // 5s para compilar
	confArg.setValue("M", 4096); // 4MB para compilar (revisar)
	confArg.setValue("S", 8); // 8KB de salida
	confArg.setValue("o", "calificaciones.txt");
	confArg.setValue("v", false); // Es callado
	confArg.setValue("nb", false); // Muestra la barra deprogreso

	confFile.setValue("max_mem", 32768); // 32MB para ser ejecutado (revisar)
	confFile.setValue("judge_exe", "judge"); // Ejecutable del juez
	confFile.setValue("strict_eval", false); // No es estricto
	confFile.setValue("compare_white", false); // Ignora espacios extra

	// Analizar y valida argumentos
	confArg.parseArgs(argc, argv);
	if (!confArg.validate())
	{
		cerr << "Parametros incorrectos. Consulte el manual." << endl;
		return 1;
	}

	confArg.getValue("action", action);
	confArg.getValue("problem", problem);

	confArg.getValue("s", maxSourceSize);
	confArg.getValue("T", maxCompTime);
	confArg.getValue("M", maxCompMem);
	confArg.getValue("S", maxOutSize);

	confArg.getValue("f", sourceFiles);
	confArg.getValue("c", testCases);
	confArg.getValue("o", outputFile);
	confArg.getValue("v", verbose);
	confArg.getValue("nb",showProgress);
	showProgress = !showProgress;

	clog << "verboso: " << (verbose?"sí":"no") << endl;
	clog << "barra de progreso: " << (showProgress?"sí":"no") << endl;

	hasError = false;
	if (!isValidAction(action))
	{
		cerr << action << " no es una acción válida" << endl;
		hasError = true;
	}
	if (!isDir(problem))
	{
		cerr << "No se pudo abrir la carpeta del problema " << problem << endl;
		hasError = true;
	}
	else if (!isFile(problem + "/eval.conf"))
	{
		cerr << "No se pudo abrir '" << problem << "/eval.conf'" << endl;
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
		cerr << "Opciones incorrectas en '" << problem << "eval.conf'. Consulte el manual." << endl;
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
		if (!isExec(problem + "/" + judgeExe))
		{
			cerr << "No se pudo abrir '" << problem << "/" << judgeExe  << "' como ejecutable" << endl;
			hasError = true;
		}
	}
	if (hasError) return 1;

	//FIXME: Posible loop infinito si un archivo .set se contiene a sí mismo

	/// ** Genera lista de casos de prueba **
	hasError = false;
	// Modo automático
	if (testCases.size() == 0)
	{
		// Obtiene los archivos .case de la carpeta
		testCases = Config::getDir(problem, CASE_EXTENSION);

		for (it = testCases.begin(); it != testCases.end(); it++)
		{
			// Quita extensión
			*it = removeExtension(*it);

			// Verifica existencia de salida esperada
			if (judgeNeedsOutput(judgeType) && !isFile(problem + "/" + *it + "." + OUTPUT_EXTENSION))
			{
				cerr << "No se puede abrir el archivo de salida experada para el caso " << *it << endl;
				hasError = true;
			}
		}
	}
	// Modo manual
	else
	{
		for (it = testCases.begin(); it != testCases.end(); it++)
		{
			// Indicaron un archivo .set
			if (hasExtension(*it, "set"))
			{
				// Verifica existencia
				if (!isFile(problem + "/" + *it))
				{
					cerr << "No se puede abrir el archivo '" << *it << "'" << endl;
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
					cerr << "No se puede abrir el caso " << *it << endl;
					hasError = true;
				}
				// Verificamos existencia de salida esperada
				else if (judgeNeedsOutput(judgeType) && !isFile(problem + "/" + *it + "." + OUTPUT_EXTENSION))
				{
					cerr << "No se puede abrir el archivo de salida experada para el caso " << *it << endl;
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
	if (testCases.size() == 0)
	{
		cerr << "No hay casos de prueba para evaluar" << endl;
		hasError = true;
	}
	if (hasError) return 1;

	/// ** Genera lista de codigos fuente ***
	hasError = false;
	for (it = sourceFiles.begin(); it != sourceFiles.end(); it++)
	{
		if (isDir(*it)) {
			tmpList = Config::getDir(*it);
			for (tmp_it = tmpList.begin(); tmp_it != tmpList.end(); tmp_it++) {
				sourceFiles.push_back(*it + "/" + *tmp_it);
			}
			toErase.push(it);
		}
		else if (!isFile(*it)) {
			if (verbose) cerr << "Ignorando el archivo '" << *it << "' (no se puede abrir)" << endl;
			toErase.push(it);
		}
		//FIXME: Revisar no existencia del archivo
		else if (hasExtension(*it, "set")) {
			tmpList = Config::getSet(*it);
			sourceFiles.insert(sourceFiles.end(), tmpList.begin(), tmpList.end());
			toErase.push(it);
		}
		else if (!forceValidLang(dumbString, *it)) {
			if (verbose) cerr << "Ignorando el archivo '" << *it << "' (no se reconoce la extensión)" << endl;
			toErase.push(it);
		}
		else if (!isFileSmaller(*it, maxSourceSize))
		{
			if (verbose) cerr << "Ignorando el archivo '" << *it << "' (excede el tamaño límite)" << endl;
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

	if (sourceFiles.size() == 0)
	{
		cerr << "No hay códigos fuente que evaluar" << endl;
		hasError = true;
	}
	if (hasError) return 1;
}

// Debug
clog << "Los casos a evaluar serán:" << endl;
for (list<string>::iterator it = testCases.begin(); it != testCases.end(); it++) clog << *it << endl;
clog << endl;

clog << "Los archivos de código serán:" << endl;
for (list<string>::iterator it = sourceFiles.begin(); it != sourceFiles.end(); it++) clog << *it << endl;
clog << endl << endl;

// Zona de pruebas
//return 0;

