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

    ofstream cout("log.txt"); //Archivo que dice paso a paso la ejecución.

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
    string tipoResultado, archCal;
    if (outputFile == "") archCal = "calificacion.txt";
    else archCal = outputFile;

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
            tipoResultado = "";
            string casoActual, codigoActual, correctoActual;
            //casoActual = correctoActual = problem + "/";

            cout << "El problema actual es: " << problem << endl;

            codigoActual += *itSF;
            cout << endl << endl << "Evaluando el archivo " << codigoActual << endl;
            calificaciones << "Programa " << codigoActual << "\t";

            //TODO Agregar la compilación para los otros lenguajes.
            string nombrePuro = removeExtension(*itSF);
            //nombrePuro.replace(nombrePuro.end()-4, nombrePuro.end(), "");

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
                    casoActual = problem + "/" + *itTC + "." CASE_EXTENSION;
                    codigoActual = problem + "/" + *itSF;
                    nombrePuro = removeExtension(codigoActual);
                    //nombrePuro.replace(nombrePuro.end()-4, nombrePuro.end(), "");

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
                        //comando.replace(comando.end()-4, comando.end(), "");
                        comando = removeExtension(comando);
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
                            string salidaCorr = problem + "/" + *itTC + "." + OUTPUT_EXTENSION;
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
                    tipoResultado = (((double)casosCorrectos/testCases.size()*100.0) >= 60.0)?"AC":"WA";
                    cout << "La calificación es: " << ((double)casosCorrectos/testCases.size()*100.0) << endl;
                }
            }
            if(tipoResultado == "AC")
            {
                calificaciones << "AC\tCalificación " << ((double)casosCorrectos/testCases.size()*100.0) << endl;
            }
            else if(tipoResultado == "WA")
            {
                calificaciones << "WA\tCalificación " << ((double)casosCorrectos/testCases.size()*100.0) << endl;
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
