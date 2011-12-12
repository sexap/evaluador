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

bool comparacion(bool estricto, string nomArchCorr, int dArchAlum)
{
    char buffer[128];
    int leidos;
    ifstream respCorrecta(nomArchCorr.c_str());
    if(!respCorrecta.good())
    {
        cout << "No pude abrir la respuesta correcta." << endl;
        return false;
    }
    else
    {
//       cout << "Abrí bien la respuesta correcta. " << endl;
    }

    bool accepted = true;
    char otroC;
    if(estricto)
    {
        cout << "Comparo en modo estricto con " << nomArchCorr << endl;

        while((leidos = read(dArchAlum, buffer, 127)))
        {
            buffer[leidos] = 0;
            //cout << buffer;

            //Comparo caracter por caracter
            for(int c = 0; c < leidos; c++)
            {
                if(respCorrecta.good())
                {
                    respCorrecta.get(otroC);
                    //cout << "Comparo " << otroC << " con " << buffer[c] << endl;
                    if(otroC != buffer[c])
                    {
                        accepted = false;
                    }
                }
                else
                {
                    accepted = false;
                    break;
                }
            }
        }
        if(respCorrecta.peek() != EOF)
            accepted = false;
    }
    else
    {
        cout << "Comparo en modo blando con: "  << nomArchCorr << endl;
        while((leidos = read(dArchAlum, buffer, 127)))
        {
            buffer[leidos] = 0;
            //cout << buffer;

            //Comparo caracter por caracter
            for(int c = 0; c < leidos; c++)
            {
                if(respCorrecta.good())
                {
                    respCorrecta.get(otroC);
                    //cout << "Comparo " << otroC << " con " << buffer[c] << endl;
                    if(otroC != buffer[c])
                    {
                        accepted = false;
                    }
                    else
                    {
                        bool huboSig = false;

                        while(otroC == ' ')
                        {
                            respCorrecta.get(otroC);
                        }
                        //respCorrecta.putback(otroC);

                        while(buffer[c] == ' ')
                        {
                            //Busco el siguiente caracter para ver si es un espacio
                            //Si hay más caracteres en la tanda actual, pues paso al siguiente.
                            if(c+1 < leidos)
                            {
                                c++;
                            }
                            //Si ya no hay más caracteres en la tanda actual, pues pido una nueva tanda, mientras haya nuevas tandas.
                            else
                            {
                                if(leidos = read(dArchAlum, buffer, 127))
                                {
                                    c = 0;
                                }
                                else
                                {
                                    c = leidos;
                                }
                            }
                        }
                    }
                }
                else
                {
                    accepted = false;
                    break;
                }
            }
        }
        if(respCorrecta.peek() != EOF)
            accepted = false;
    }
    respCorrecta.close();
    return accepted;
}

int main(int argc, char* argv[])
{
    Config confArg, confFile;
    string lang, judgeType, judgeExe, action, problem;
    int maxSourceSize, maxCompTime, maxCompMem, maxOutSize, maxRunTime, maxRunMem;
    bool verbose, strictEval, compareWhite, hasError;
    list<string> testCases, tmpList, sourceFiles;
    stack<list<string>::iterator> toErase;

    // Registra variables
    confArg.registerArgVar("action", Config::T_STRING, 1);
    confArg.registerArgVar("problem", Config::T_STRING, 2);

    confArg.registerArgVar("f", Config::T_LIST, true);
    confArg.registerArgVar("l", Config::T_STRING, false);
    confArg.registerArgVar("s", Config::T_INT, false);
    confArg.registerArgVar("T", Config::T_INT, false);
    confArg.registerArgVar("M", Config::T_INT, false);
    confArg.registerArgVar("S", Config::T_INT, false);
    confArg.registerArgVar("c", Config::T_LIST, false);
    confArg.registerArgVar("v", Config::T_BOOL, false);

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
    confArg.getValue("v", verbose);
    confArg.getValue("c", testCases);

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

    //FIXME: Posible loop infinito si un archivo set se contiene a sí mismo
    // Genera lista de casos de prueba
    hasError = false;
    if (testCases.size() == 0)
    {
        testCases = Config::getDir(problem, "case");
        // Quita extensión
        for (list<string>::iterator it = testCases.begin(); it != testCases.end(); it++)
        {
            *it = it->substr(0, it->rfind('.'));
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
        sourceFiles = Config::getDir(*sourceFiles.begin());
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

    cout << "CASOS:" << endl;
    for (list<string>::iterator it = testCases.begin(); it != testCases.end(); it++) cout << *it << endl;
    cout << "FUENTES:" << endl;
    for (list<string>::iterator it = sourceFiles.begin(); it != sourceFiles.end(); it++) cout << *it << endl;

    /*cout << "La acción es " << action << " y el problema es " << problem << endl;
    cout << endl;
    cout << "Se compilará el archivo \"" << sourceFile << "\" en lenguaje " << lang << endl;
    cout << "El archivo debe pesar máximo " << maxSourceSize << "kB" << endl;
    cout << endl;
    cout << "Tiene " << maxCompTime << "ms y " << maxCompMem << "kB para que se compile" << endl;
    cout << "Durante su ejecución no podrá generar más de " << maxOutSize << "kB de salida" << endl;
    cout << endl;
    cout << "Se eveluará con el juez " << judgeType << " (ejecutable " << judgeExe << ")" << endl;
    cout << "Tiene " << maxRunTime << "ms y " << maxRunMem << "kB para la ejecución de cada caso" << endl;
    cout << "La salida " << (compareWhite?"":"NO ") << "tamará en cuenta espacios en blanco" << endl;
    cout << "La calificación será " << (strictEval?"todo o nada":"normal") << endl;
    cout << endl;
    cout << "¿Mostrar información extra? " << (verbose?"Sí":"No") << endl;
    cout << endl;*/

    //En este punto:
    //testCases: lista con todos los casos y verificada su existencia
    //sourceFiles: Todos los archivos por compilar, verificada su existencia y su tamaño

    ///////////////////////////////////////////////////////////////////////////
    ///////////// Rafael, se encarga de compilacion y ejecucion////////////////
    ///////////////////////////////////////////////////////////////////////////
//	#ifdef COMPREADY //TODO: Quitar al actualizar

    /*
    *   Hago la compilación y la ejecución para cada código de alumno.
    */
    //Para cada código
    /*   for (list<string>::iterator itSF = sourceFiles.begin(); itSF != sourceFiles.end(); itSF++)
       {*/
    for (list<string>::iterator itSF = sourceFiles.begin(); itSF != sourceFiles.end(); itSF++)
    {
        int programa;
        pid_t pID;
        int status;
        int fd_pipe[2];
        int leidos;
        int compilacion;
        bool estricto = false;
        int casosCorrectos = 0;
        string casoActual, codigoActual, correctoActual;
        casoActual = codigoActual = correctoActual = problem + "/";

        cout << "El problema actual es: " << problem << endl;

        codigoActual += *itSF;
        cout << endl << endl << "Evaluando el archivo " << codigoActual << endl;

        //Compilación
        string nombrePuro = *itSF;
        nombrePuro.replace(nombrePuro.end()-4, nombrePuro.end(), "");
        string comando = "g++ " + *itSF + " -o " + nombrePuro + " -lm";
        cout << comando << endl;
        compilacion = system(comando.c_str());
        cout << "Compilo: "<< codigoActual << " " << compilacion << endl << endl;

        if(compilacion == 0)
        {
            cout << "Compilación correcta...." << endl;
        }
        else
        {
            cout << "Error de compilación." << endl;
            return 0;
        }

        for (list<string>::iterator itTC = testCases.begin(); itTC != testCases.end(); itTC++)
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
            else
            {
                //cout << "Pipe creado correctamente...." << endl;
            }

            pID = fork();

            //cout << "Hago el fork..." << endl;

            //**************** Hijo *******************/
            if(pID == 0)
            {
                //cout << "Soy el hijo." << endl;

                close(fd_pipe[0]);
                dup2(fd_pipe[1], STDOUT_FILENO);    //Salida al pipe.
                close(fd_pipe[1]);

                freopen(casoActual.c_str(), "r", stdin);   //Entrada del problema.
//            freopen("log.log", "w", stdout);
//            freopen("error.log", "w", stderr);

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
                //freopen("hijo.sal", "w", stdout);
                close(fd_pipe[1]);
                //es el padre
                //cout << "Soy el Padre "<< getpid() << ", mi hijo es el proceso: " << pID << endl;
                waitpid(pID, &status, 0);

                bool exito =false;
                //cout<<"Ya acabo mi hijo."<<endl;
                if (WIFEXITED(status))
                {
                    /*cout << "Mi hijo dice:" << endl;
                    char buffer[128];
                    while((leidos = read(fd_pipe[0], buffer, 127)))
                    {
                        buffer[leidos] = 0;
                        cout << buffer;
                    }
                    cout << "Eso es todo lo que dijo." << endl;*/
                    if (WEXITSTATUS(status) == 0)
                    {
                        exito = true;
                    }
                    else cout << "Error de compilación" << endl;
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
//               cout << "Comparo con el archivo: " << salidaCorr << endl;
                    //Comparar entre el caso actual y la salida lectura de pipe desde el hijo.
                    if(comparacion(estricto, salidaCorr, fd_pipe[0]))
                    {
                        cout << "Caso " << salidaCorr << " estuvo bien en modo ";
                        if(estricto)
                            cout << "estricto." << endl;
                        else
                            cout << "blando." << endl;
                        casosCorrectos++;
                    }
                    else
                    {
                        cout << "Falla el caso " << salidaCorr << " en modo ";
                        if(estricto)
                            cout << "estricto." << endl;
                        else
                            cout << "blando." << endl;
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
            }
            else
            {
                cout << "WRONG ANSWER" << endl;
                cout << "La calificación es: 0" << endl;
            }
        }
        else
        {
            cout << endl << "Tuvo " << casosCorrectos << " casos correctos de " << testCases.size() << " casos de prueba." << endl << endl;
            cout << "La calificación es: " << ((double)casosCorrectos/testCases.size()*100.0) << endl;
        }
    }
//    } //Cierra el ciclo de SF

//	list<string>::iterator it;
//
//	for(it = testCases.begin(); it != testCases.end(); it++){
//		cout << "Compilando " << *it << endl;
//	}
//	#endif //TODO: Quitar al actualizar
    return 0;
}
