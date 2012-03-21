// Variables accesibles desde el exterior
string judgeType, judgeExe, action, problem, outputFile;
unsigned maxSourceSize, maxCompTime, maxCompMem, maxOutSize, maxRunTime, maxSleepTime, maxRunMem;
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

	confArg.registerArgVar("f", Config::T_LIST, true);
	confArg.registerArgVar("c", Config::T_LIST, false);
	confArg.registerArgVar("o", Config::T_STRING, false);
	confArg.registerArgVar("v", Config::T_BOOL, false);
	confArg.registerArgVar("nb", Config::T_BOOL, false);

	confFile.registerFileVar("comp_time", Config::T_INT, false);
	confFile.registerFileVar("comp_mem", Config::T_INT, false);
	confFile.registerFileVar("run_time", Config::T_INT, false);
	confFile.registerFileVar("sleep_time", Config::T_INT, false);
	confFile.registerFileVar("run_mem", Config::T_INT, false);
	confFile.registerFileVar("source_size", Config::T_INT, false);
	confFile.registerFileVar("output_size", Config::T_INT, false);

	confFile.registerFileVar("judge_type", Config::T_STRING, false);
	confFile.registerFileVar("judge_exe", Config::T_STRING, false);
	confFile.registerFileVar("strict_eval", Config::T_BOOL, false);
	confFile.registerFileVar("compare_white", Config::T_BOOL, false);

	// Valores por default
	confArg.setValue("o", "");
	confArg.setValue("v", false); // Es callado
	confArg.setValue("nb", false); // Muestra la barra deprogreso

	confFile.setValue("comp_time", 10000); // 10s para compilar
	confFile.setValue("comp_mem", 256); // 256MB para compilar
	confFile.setValue("run_time", 5000); // 5 s para ejecutarse
	confFile.setValue("sleep_time", 50); // 50 kCiclos para estar dormido
	confFile.setValue("run_mem", 128); // 128 MiB para ser ejecutado
	confFile.setValue("source_size", 32); // 32 KiB de código
	confFile.setValue("output_size", 256); // 256 KiB de salida

	confFile.setValue("judge_type", "standard"); // Juez estándar
	confFile.setValue("judge_exe", ""); // Ejecutable del juez
	confFile.setValue("strict_eval", false); // No es estricto
	confFile.setValue("compare_white", false); // Ignora espacios extra

	// ** Analizar y valida argumentos **
	confArg.parseArgs(argc, argv);
	if (!confArg.validate())
	{
		cerr << "Parametros incorrectos. Consulte el manual." << endl;
		return 1;
	}

	confArg.getValue("action", action);
	confArg.getValue("problem", problem);

	confArg.getValue("f", sourceFiles);
	confArg.getValue("c", testCases);
	confArg.getValue("o", outputFile);
	confArg.getValue("v", verbose);
	confArg.getValue("nb",showProgress);

	problem = cleanDirName(problem);
	if(outputFile == "") outputFile = getFileName(problem);
	showProgress = !showProgress;

	clog << "verboso: " << (verbose?"sí":"no") << endl;
	clog << "barra de progreso: " << (showProgress?"sí":"no") << endl;
	clog << endl;

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
	if (hasError) return 1;

	// ** Analizar y valida archivo de configureción **
	if (!isFile(problem + "/eval.conf")) {
		if (verbose) cerr << "No se pudo abrir '" << problem << "/eval.conf'. Usando opciones por default." << endl;
	}
	else {
		confFile.parseFile(problem + "/eval.conf");
		if (!confFile.validate())
		{
			cerr << "Opciones incorrectas en '" << problem << "/eval.conf'. Consulte el manual." << endl;
			return 1;
		}
	}

	confFile.getValue("comp_time", maxCompTime);
	confFile.getValue("comp_mem", maxCompMem);
	confFile.getValue("run_time", maxRunTime);
	confFile.getValue("sleep_time", maxSleepTime);
	confFile.getValue("run_mem", maxRunMem);
	confFile.getValue("source_size", maxSourceSize);
	confFile.getValue("output_size", maxOutSize);

	confFile.getValue("judge_type", judgeType);
	confFile.getValue("judge_exe", judgeExe);
	confFile.getValue("strict_eval", strictEval);
	confFile.getValue("compare_white", compareWhite);

	if (judgeExe == "") judgeExe = "_judges_/" + getFileName(problem);
	else judgeExe = "_judges_/" + judgeExe;

	hasError = false;
	if (!isBetween(maxCompTime, 1, 90000))
	{
		cerr << "En '" << problem << "/eval.conf': comp_time debe estar entre 1 ms y 90 000 ms" << endl;
		hasError = true;
	}
	if (!isBetween(maxCompMem, 1, 2048))
	{
		cerr << "En '" << problem << "/eval.conf': comp_mem debe estar entre 1 MiB y 2048 MiB" << endl;
		hasError = true;
	}
	if (!isBetween(maxRunTime, 1, 90000))
	{
		cerr << "En '" << problem << "/eval.conf': run_time debe estar entre 1 ms y 90 000 ms" << endl;
		hasError = true;
	}
	if (!isBetween(maxSleepTime, 1, 10000))
	{
		cerr << "En '" << problem << "/eval.conf': sleep_time debe estar entre 1 kCiclos y 10 000 kCiclos" << endl;
		hasError = true;
	}
	if (!isBetween(maxRunMem, 1, 2048))
	{
		cerr << "En '" << problem << "/eval.conf': run_mem debe estar entre 1 MiB y 2048 MiB" << endl;
		hasError = true;
	}
	if (!isBetween(maxSourceSize, 1, 1024*1024))
	{
		cerr << "En '" << problem << "/eval.conf': source_size debe estar entre 1 KiB y 1 048 576 KiB" << endl;
		hasError = true;
	}
	if (!isBetween(maxOutSize, 1, 1024*1024))
	{
		cerr << "En '" << problem << "/eval.conf': output_size debe estar entre 1 KiB y 1 048 576 KiB" << endl;
		hasError = true;
	}

	if (!isValidJudgeType(judgeType))
	{
		cerr << "En '" << problem << "/eval.conf': " << judgeType << " no es un tipo de juez válido" << endl;
		hasError = true;
	}
	else if (judgeNeedsExe(judgeType))
	{
		if (!isExec(judgeExe)) {
			cerr << "No se pudo abrir '" << judgeExe  << "' como ejecutable" << endl;
			hasError = true;
		}
	}
	if (hasError) return 1;

	/// ** Genera lista de casos de prueba **
	hasError = false;
	// Modo automático
	if (testCases.size() == 0) {
		// Obtiene los casos de la carpeta del problema
		testCases = Config::getDirFiles(problem, CASE_EXTENSION);

		for (it = testCases.begin(); it != testCases.end(); ++it)
		{
			// Quita extensión y convierte a path completo
			*it = problem + "/" + removeExtension(*it);

			// Verifica existencia de salida esperada
			if (judgeNeedsOutput(judgeType) && !isFile(*it + "." + OUTPUT_EXTENSION))
			{
				cerr << "No se pudo abrir el archivo de salida esperada para el caso " << *it << endl;
				hasError = true;
			}
		}
	}
	// Modo manual
	else {
		//Pasada 1: Expandimos expresiones y completamos el path
		for (it = testCases.begin(); it != testCases.end(); ++it) {
			tmpList = Config::getExpansion(problem + "/" + *it + "." + CASE_EXTENSION);
			// Quitar extensiones
			for (tmp_it = tmpList.begin(); tmp_it != tmpList.end(); ++tmp_it) {
				*tmp_it = removeExtension(*tmp_it);
			}
			// Agregar a la lista
			testCases.insert(it, tmpList.begin(), tmpList.end());
			toErase.push(it);
		}
		while (!toErase.empty()) {
			testCases.erase(toErase.top());
			toErase.pop();
		}

		//Pasada 2: Procesamos cada elemento
		for (it = testCases.begin(); it != testCases.end(); ++it){
			if (isFile(*it + "." + CASE_EXTENSION)) {
				// Verificamos existencia de salida esperada
				if (judgeNeedsOutput(judgeType) && !isFile(*it + "." + OUTPUT_EXTENSION))
				{
					cerr << "No se pudo abrir el archivo de salida esperada para el caso " << *it << endl;
					hasError = true;
				}
			}
			else if (isDir(*it)) {
				tmpList = Config::getDirFiles(*it, CASE_EXTENSION);
				for (tmp_it = tmpList.begin(); tmp_it != tmpList.end(); ++tmp_it) {
					testCases.push_back(cleanDirName(*it) + "/" + removeExtension(*tmp_it));
				}
				toErase.push(it);
			}
			else {
				cerr << "No se pudo abrir el caso " << *it << endl;
				hasError = true;
			}
		}
		while (!toErase.empty()) {
			testCases.erase(toErase.top());
			toErase.pop();
		}
	}
	// Quitar repetidos
	testCases.sort();
	testCases.unique();

	if (testCases.size() == 0)
	{
		cerr << "No hay casos de prueba para evaluar" << endl;
		hasError = true;
	}
	if (hasError) return 1;

	/// ** Genera lista de codigos fuente ***
	hasError = false;
	for (it = sourceFiles.begin(); it != sourceFiles.end(); ++it)
	{
		if (isDir(*it)) {
			tmpList = Config::getDirFiles(*it);
			for (tmp_it = tmpList.begin(); tmp_it != tmpList.end(); ++tmp_it) {
				sourceFiles.push_back(cleanDirName(*it) + "/" + *tmp_it);
			}
			toErase.push(it);
		}
		else if (!isFile(*it)) {
			if (verbose) cerr << "Ignorando el archivo '" << *it << "' (no se pudo abrir)" << endl;
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
clog << "Los casos a evaluar son:" << endl;
for (list<string>::iterator it = testCases.begin(); it != testCases.end(); ++it) clog << *it << endl;
clog << endl;

clog << "Los archivos de código son:" << endl;
for (list<string>::iterator it = sourceFiles.begin(); it != sourceFiles.end(); ++it) clog << *it << endl;
clog << endl << endl;

clog << "Restricciones compilación" << endl;
clog << "  Tiempo:  " << maxCompTime << " ms" << endl;
clog << "  Memoria: " << maxCompMem << " MiB" << endl;
clog << "Restricciones ejecución" << endl;
clog << "  Tiempo:  " << maxRunTime << " ms" << endl;
clog << "  Memoria: " << maxRunMem << " MiB" << endl;
clog << "  Salida:  " << maxOutSize << " KiB" << endl;
clog << endl;
