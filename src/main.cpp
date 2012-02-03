// Librerias estándar
#include <iostream>
#include <list>
#include <stack>
#include <string>
using namespace std;

// Librerias propias
#include "build_params.h"
#include "config.h"
#include "validation.h"
#include "juezNormal.cpp"
#include "JE/set.cpp"
using namespace seap_implement;

// Librerias linux
#include <fstream>
#include <sys/types.h>
#include <unistd.h> //execl
#include <cstdlib> //execl
#include <sys/time.h> //setrlimit
#include <sys/resource.h> //setrlimit
#include <sys/wait.h>
#include <cstdio>

int main(int argc, char* argv[])
{
	// Variables que se pueden usar tras la validación
	// string judgeType, judgeExe, action, problem, outputFile;
	// int maxSourceSize, maxCompTime, maxCompMem, maxOutSize, maxRunTime, maxRunMem;
	// bool verbose, strictEval, compareWhite;
	// list<string> testCases, sourceFiles;
    #include "main_validation.cpp"

    ofstream cerr("log.txt"); //Archivo que dice paso a paso la ejecución.

	//Temporal (ernesto)
    cerr << "CASOS:" << endl;
    for (list<string>::iterator it = testCases.begin(); it != testCases.end(); it++) cerr << *it << endl;
    cerr << "FUENTES:" << endl;
    for (list<string>::iterator it = sourceFiles.begin(); it != sourceFiles.end(); it++) cerr << *it << endl;
    //TODO: Crear variables con la ruta de los archivos y otras con los nombres de los archivos.

    //TODO: Recibir a partir de aquí la variable que indica el lenguaje.

    /*
    *   Compilación y Evaluación
    */
    string rutaFuentes, rutaCasos;
    string tipoResultado, archCal;
    if (outputFile == "") archCal = "calificacion.txt";
    else archCal = outputFile;

    int calificacion;

    ofstream calificaciones(archCal.c_str());         //Archivo de los resultados en txt

    calificaciones << "Calificaciones de " << problem << "\n\n";
    cerr << endl << endl << "Evaluando Programas...." << endl << endl;
    cerr << "El tipo de juez es: " << judgeType << endl;
    /**
    *   Juez Normal
    **/
    if(judgeType == "standard")
    {
        bool estricto = false;
        cerr << "El modo de evaluación es: ";
        if(estricto) cerr << "estricto." << endl;
        else         cerr << "normal"    << endl;

        for (list<string>::iterator itSF = sourceFiles.begin(); itSF != sourceFiles.end(); itSF++)  //Ciclo para cada programa de alumno. (Fuentes)
        {
            int programa;
            pid_t pID;
            int status;
            int fd_pipe[2];
            int leidos;
            int compilacion;

            int casosCorrectos = 0;
            tipoResultado = "";
            string casoActual, codigoActual, correctoActual;

            cerr << "El problema actual es: " << problem << endl;

            codigoActual += *itSF;
            cerr << endl << endl << "Evaluando el archivo " << codigoActual << endl;
            calificaciones << "Programa " << codigoActual << "\t";

            //TODO Agregar la compilación para los otros lenguajes.
            string nombrePuro = removeExtension(*itSF);

            string lang;
            if(!forceValidLang(lang, codigoActual))
            {
                    cerr << "No es un lenguaje válido." << endl;
            }
            string comando;
            if(lang == "c")
            {
                comando = "gcc -lm " + *itSF + " -o " + nombrePuro;
            }
            else if(lang == "c++")
            {
                comando = "g++ -lm -include /usr/include/stdlib.h -Wno-deprecated " + *itSF + " -o " + nombrePuro;
            }
            else if(lang == "java")
            {
                //TODO Arreglar la compilación con Java.
                //comando = "gcj --main=" + nombrePuro + " -Wall " + *itSF + " -o " + nombrePuro;
                comando = "gcj --main=nombreClase -Wall " + *itSF + " -o nombreClase";
            }

            cerr << "Intento " << comando << endl;
            compilacion = system(comando.c_str());

            if(compilacion == 0)
            {
                cerr << "Compilación correcta...." << endl;
            }
            else
            {
                tipoResultado = "CE";
                cerr << "Falló la compilación." << endl;
            }

            if(tipoResultado != "CE")
            {
                for (list<string>::iterator itTC = testCases.begin(); itTC != testCases.end(); itTC++)  //Ciclo para cada caso de prueba. (Casos)
                {
                    casoActual = problem + "/" + *itTC + "." CASE_EXTENSION;
                    codigoActual = problem + "/" + *itSF;
                    nombrePuro = removeExtension(codigoActual);

                    cerr << "Probando con " << casoActual << endl;
                    if (pipe(fd_pipe) < 0)
                    {
                        cerr << "No se pudo hacer pipe" << endl;
                        return 1;
                    }

                    pID = fork();

                    //**************** Hijo *******************/
                    if(pID == 0)
                    {
                        //cerr << "Soy el hijo." << endl;

                        close(fd_pipe[0]);
                        dup2(fd_pipe[1], STDOUT_FILENO);    //Salida al pipe.
                        close(fd_pipe[1]);

                        freopen(casoActual.c_str(), "r", stdin);   //Entrada del problema.

                        //Ejecución
                        comando = "./" + *itSF;
                        comando = removeExtension(comando);
                        cerr << "Ejecuto el programa " << comando << endl;
                        programa = execl(comando.c_str(), comando.c_str(), NULL);
                        if(programa == -1)
                        {
                            cerr << "Error de ejecución" << endl;
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
                                cerr << "WEXITSTATUS(status) diferente de 0" << endl;
                                tipoResultado = "CE";
                            }
                        }
                        else if (WIFSIGNALED(status))
                        {
                            cerr << "Terminado con la señal" << WTERMSIG(status)<< endl;
                        }
                        else if (WIFSTOPPED(status))
                        {
                            cerr << "Detenido por la señal" << WSTOPSIG(status) << endl;
                        }
                        else if (WIFCONTINUED(status))
                        {
                            cerr << "wtf" << endl;
                        }

                        if(exito)
                        {
                            //el programa ya fue compilado y esta listo para ejecutarse
                            string salidaCorr = problem + "/" + *itTC + "." + OUTPUT_EXTENSION;
                            cerr << "Comparo con el archivo: " << salidaCorr << endl;
                            //Comparar entre el caso actual y la salida lectura de pipe desde el hijo.

                            /*
                            *   Llamo al juez normal
                            */
                            if(juezNormal(estricto, salidaCorr, fd_pipe[0]))
                            {
                                cerr << "Caso " << salidaCorr << " estuvo bien en modo ";
                                if(estricto)
                                    cerr << "estricto." << endl;
                                else
                                    cerr << "normal." << endl;
                                casosCorrectos++;
                            }
                            else
                            {
                                cerr << "Falla el caso " << salidaCorr << " en modo ";
                                if(estricto)
                                    cerr << "estricto." << endl;
                                else
                                    cerr << "normal." << endl;
                            }
                            cerr << endl;
                        }
                        close(fd_pipe[0]);
                    }   //Cierra el Padre
                }   //Cierra el ciclo TC
                if(estricto)
                {
                    cerr << endl << "Tuvo " << casosCorrectos << " casos correctos de " << testCases.size() << " casos de prueba." << endl << endl;
                    if(casosCorrectos == testCases.size())
                    {

                        cerr << "ACCEPTED" << endl;
                        cerr << "La calificación es: 10" << endl;
                        tipoResultado = "AC";
                    }
                    else
                    {
                        cerr << "WRONG ANSWER" << endl;
                        tipoResultado = "WA";
                        cerr << "La calificación es: 0" << endl;
                    }
                }
                else
                {
                    cerr << endl << "Tuvo " << casosCorrectos << " casos correctos de " << testCases.size() << " casos de prueba." << endl << endl;
                    tipoResultado = (((double)casosCorrectos/testCases.size()*100.0) >= 60.0)?"AC":"WA";
                    cerr << "La calificación es: " << ((double)casosCorrectos/testCases.size()*100.0) << endl;
                }
            }
            if(tipoResultado == "AC")
            {
                calificaciones << "AC\tCalificación\t" << ((double)casosCorrectos/testCases.size()*100.0) << endl;
            }
            else if(tipoResultado == "WA")
            {
                if(casosCorrectos == 0)
                    calificaciones << "WA\tCalificación\t0" << endl;
                else
                    calificaciones << "WA\tCalificación\t" << ((double)casosCorrectos/testCases.size()*100.0) << endl;
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
