#include <iostream>
#include <list>
#include <stack>
#include <string>
using namespace std;

#include "config.h"
#include "validation.h"
using namespace seap_implement;

#include <fstream>
#include <sys/types.h>
#include <unistd.h> //execl
#include <cstdlib> //execl
#include <sys/time.h> //setrlimit
#include <sys/resource.h> //setrlimit
#include <sys/wait.h>
#include <cstdio>
#include "juezNormal.cpp"
#include "JE/set.cpp"

int main(int argc, char* argv[])
{
    Config confArg, confFile;
    string lang, judgeType, judgeExe, action, problem, outputFile;
    int maxSourceSize, maxCompTime, maxCompMem, maxOutSize, maxRunTime, maxRunMem;
    bool verbose, strictEval, compareWhite, hasError;
    list<string> testCases, tmpList, sourceFiles;
    stack<list<string>::iterator> toErase;

    // Registra variables
    confArg.registerArgFixVar("action", Config::T_STRING);
    confArg.registerArgFixVar("problem", Config::T_STRING);

    confArg.registerArgVar("f", Config::T_LIST, true);
    confArg.registerArgVar("l", Config::T_STRING, false);
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
    confArg.setValue("l", "auto"); // Lenguaje auto
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
        cout << "Parametros incorrectos. Consulte el manual" << endl;
        return 1;
    }

    confArg.getValue("action", action);
    confArg.getValue("problem", problem);

    confArg.getValue("f", sourceFiles);
    confArg.getValue("l", lang);
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
        cout << action << " no es una acción válida" << endl;
        hasError = true;
    }
    if (!isDir(problem))
    {
        cout << "No existe el problema " << problem << endl;
        hasError = true;
    }
    if (!isFile(problem + "/eval.conf"))
    {
        cout << "No se encuentra " << problem << "/eval.conf" << endl;
        hasError = true;
    }
    if (!isBetween(maxSourceSize, 1, 512))
    {
        cout << "el parametro -s debe estar entre 1kB y 512kB" << endl;
        hasError = true;
    }
    if (!isBetween(maxCompTime, 1, 90000))
    {
        cout << "el parametro -T debe estar entre 1ms y 90s" << endl;
        hasError = true;
    }
    if (!isBetween(maxCompMem, 1, 262144))   // (revisar)
    {
        cout << "el parametro -M debe estar entre 1kB y 256MB" << endl;
        hasError = true;
    }
    if (!isBetween(maxOutSize, 1, 256))
    {
        cout << "el parametro -S debe estar entre 1kB y 256kB" << endl;
        hasError = true;
    }
    if (hasError) return 1;

    // Analizar y valida archivo de configureción
    confFile.parseFile(problem + "/eval.conf");
    if (!confFile.validate())
    {
        cout << "Archivo erroneo. Consulta el manual" << endl;
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
        cout << "max_time debe estar entre 1ms y 60s" << endl;
        hasError = true;
    }
    if (!isBetween(maxRunMem, 1, 262144))
    {
        cout << "max_mem debe estar entre 1kB y 256MB" << endl;
        hasError = true;
    }
    if (!isValidJudgeType(judgeType))
    {
        cout << judgeType << " no es un tipo de juez válido" << endl;
        hasError = true;
    }
    else if (judgeNeedsExe(judgeType))
    {
        if (!isFile(problem + "/" + judgeExe))
        {
            cout << "No se encuentra el ejecutable " << judgeExe << endl;
            hasError = true;
        }
    }
    if (hasError) return 1;

    //TODO: Posible loop infinito si un archivo set se contiene a sí mismo
    // Genera lista de casos de prueba
    hasError = false;
    if (testCases.size() == 0)
    {
        testCases = Config::getDir(problem, "case");
        // Quita extensión
        for (list<string>::iterator it = testCases.begin(); it != testCases.end(); it++)
        {
            *it = removeExtension(*it);
        }
    }
    else
    {
        for (list<string>::iterator it = testCases.begin(); it != testCases.end(); it++)
        {
            // Es archivo set
            if (hasExtension(*it, "set"))
            {
                if (!isFile(problem + "/" + *it))
                {
                    cout << "No existe el archivo " << *it << endl;
                    hasError = true;
                }
                else
                {
                    tmpList = Config::getSet(problem + "/" + *it);
                    testCases.insert(testCases.end(), tmpList.begin(), tmpList.end());
                    toErase.push(it);
                }
            }
            // Es caso normal
            else
            {
                if (!isFile(problem + "/" + *it + ".case"))
                {
                    cout << "No existe el caso " << *it << endl;
                    hasError = true;
                }
                else if (judgeNeedsOutput(judgeType) && !isFile(problem + "/" + *it + ".out"))
                {
                    cout << "No se encuentra la salida experada para el caso " << *it << endl;
                    hasError = true;
                }
            }
        }
        while (!toErase.empty())
        {
            testCases.erase(toErase.top());
            toErase.pop();
        }
        testCases.sort();
        testCases.unique();
    }
    if (hasError) return 1;

    // Genera lista de codigos fuente
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
                cout << "No existe el archivo " << *it << endl;
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
                cout << "El archivo " << *it << " es muy grande" << endl;
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
        cout << "No hay casos por evaluar" << endl;
        hasError = true;
    }
    if (hasError) return 1;

    ofstream cout("log.txt");       //Archivo que dice paso a paso la ejecución.

	//Temporal (ernesto)
    cout << "CASOS:" << endl;
    for (list<string>::iterator it = testCases.begin(); it != testCases.end(); it++) cout << *it << endl;
    cout << "FUENTES:" << endl;
    for (list<string>::iterator it = sourceFiles.begin(); it != sourceFiles.end(); it++) cout << *it << endl;
    //TODO: Crear variables con la ruta de los archivos y otras con los nombres de los archivos.

    //TODO: Recibir a partir de aquí la variable que indica el lenguaje.

    /*
    *   Compilación y Evaluación
    */
    string rutaFuentes, rutaCasos;
    string tipoResultado, archCal = "cal_" + problem + ".txt";
    int calificacion;

    ofstream calificaciones(archCal.c_str());         //Archivo de los resultados en txt

    calificaciones << "Calificaciones de " << problem << "\n\n";
    cout << endl << endl << "Evaluando Programas...." << endl << endl;
    cout << "El tipo de juez es: " << judgeType << endl;
    /**
    *   Juez Normal
    **/
    if(judgeType == "standard")
    {
        bool estricto = false;
        cout << "El modo de evaluación es: ";
        if(estricto) cout << "estricto." << endl;
        else         cout << "normal"    << endl;

        for (list<string>::iterator itSF = sourceFiles.begin(); itSF != sourceFiles.end(); itSF++)  //Ciclo para cada programa de alumno. (Fuentes)
        {
            int programa;
            pid_t pID;
            int status;
            int fd_pipe[2];
            int leidos;
            int compilacion;

            int casosCorrectos = 0;
            string casoActual, codigoActual, correctoActual;
            casoActual = codigoActual = correctoActual = problem + "/";

            cout << "El problema actual es: " << problem << endl;

            codigoActual += *itSF;
            cout << endl << endl << "Evaluando el archivo " << codigoActual << endl;
            calificaciones << "Programa " << codigoActual << "\t";

            //TODO Agregar la compilación para los otros lenguajes.
            string nombrePuro = *itSF;
            nombrePuro.replace(nombrePuro.end()-4, nombrePuro.end(), "");

            string comando = "g++ " + *itSF + " -o " + nombrePuro + " -lm";
            cout << "Intento " << comando << endl;
            compilacion = system(comando.c_str());

            if(compilacion == 0)
            {
                cout << "Compilación correcta...." << endl;
            }
            else
            {
                tipoResultado = "CE";
                cout << "Falló la compilación." << endl;
            }

            if(tipoResultado != "CE")
            {
                for (list<string>::iterator itTC = testCases.begin(); itTC != testCases.end(); itTC++)  //Ciclo para cada caso de prueba. (Casos)
                {
                    casoActual = problem + "/" + *itTC + ".case";
                    codigoActual = problem + "/" + *itSF;
                    nombrePuro = codigoActual;
                    nombrePuro.replace(nombrePuro.end()-4, nombrePuro.end(), "");

                    cout << "Probando con " << casoActual << endl;
                    if (pipe(fd_pipe) < 0)
                    {
                        cout << "No se pudo hacer pipe" << endl;
                        return 1;
                    }

                    pID = fork();

                    //**************** Hijo *******************/
                    if(pID == 0)
                    {
                        //cout << "Soy el hijo." << endl;

                        close(fd_pipe[0]);
                        dup2(fd_pipe[1], STDOUT_FILENO);    //Salida al pipe.
                        close(fd_pipe[1]);

                        freopen(casoActual.c_str(), "r", stdin);   //Entrada del problema.

                        //Ejecución
                        comando = "./" + *itSF;
                        comando.replace(comando.end()-4, comando.end(), "");
                        //cout << "Ejecuto el programa C++ " << comando << endl;
                        programa = execl(comando.c_str(), comando.c_str(), NULL);
                        if(programa == -1)
                        {
                            cout << "Error de ejecución" << endl;
                            return 0;
                        }
                    }   //Cierra el hijo.
                    //****************** Padre ********************/
                    else
                    {
                        close(fd_pipe[1]);
                        //es el padre
                        waitpid(pID, &status, 0);

                        bool exito =false;
                        if (WIFEXITED(status))
                        {
                            if (WEXITSTATUS(status) == 0)
                            {
                                exito = true;
                            }
                            else
                            {
                                tipoResultado = "CE";
                            }
                        }
                        else if (WIFSIGNALED(status))
                        {
                            cout << "Terminado con la señal" << WTERMSIG(status)<< endl;
                        }
                        else if (WIFSTOPPED(status))
                        {
                            cout << "Detenido por la señal" << WSTOPSIG(status) << endl;
                        }
                        else if (WIFCONTINUED(status))
                        {
                            cout << "wtf" << endl;
                        }

                        if(exito)
                        {
                            //el programa ya fue compilado y esta listo para ejecutarse
                            string salidaCorr = problem + "/" + *itTC + ".out";
                            cout << "Comparo con el archivo: " << salidaCorr << endl;
                            //Comparar entre el caso actual y la salida lectura de pipe desde el hijo.

                            /*
                            *   Llamo al juez normal
                            */
                            if(juezNormal(estricto, salidaCorr, fd_pipe[0]))
                            {
                                cout << "Caso " << salidaCorr << " estuvo bien en modo ";
                                if(estricto)
                                    cout << "estricto." << endl;
                                else
                                    cout << "normal." << endl;
                                casosCorrectos++;
                            }
                            else
                            {
                                cout << "Falla el caso " << salidaCorr << " en modo ";
                                if(estricto)
                                    cout << "estricto." << endl;
                                else
                                    cout << "normal." << endl;
                            }
                            cout << endl;
                        }
                        close(fd_pipe[0]);
                    }   //Cierra el Padre
                }   //Cierra el ciclo TC
                if(estricto)
                {
                    cout << endl << "Tuvo " << casosCorrectos << " casos correctos de " << testCases.size() << " casos de prueba." << endl << endl;
                    if(casosCorrectos == testCases.size())
                    {

                        cout << "ACCEPTED" << endl;
                        cout << "La calificación es: 10" << endl;
                        tipoResultado = "AC";
                    }
                    else
                    {
                        cout << "WRONG ANSWER" << endl;
                        tipoResultado = "WA";
                        cout << "La calificación es: 0" << endl;
                    }
                }
                else
                {
                    cout << endl << "Tuvo " << casosCorrectos << " casos correctos de " << testCases.size() << " casos de prueba." << endl << endl;
                    tipoResultado = (((double)casosCorrectos/testCases.size()*100.0) >= 60)?"AC":"WA";
                    cout << "La calificación es: " << ((double)casosCorrectos/testCases.size()*100.0) << endl;
                }
            }
            if(tipoResultado == "AC")
            {
                calificaciones << "AC\tCalificación " << ((int)casosCorrectos/testCases.size()*100.0) << endl;
            }
            else if(tipoResultado == "WA")
            {
                calificaciones << "WA\tCalificación " << ((int)casosCorrectos/testCases.size()*100.0) << endl;
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

//	#endif //TODO: Quitar al actualizar
    return 0;
}
