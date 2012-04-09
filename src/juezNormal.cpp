#include "juezNormal.h"

bool juezNormal(bool compareWhite, string archSalidaEsperada)
{
    ofstream cout("logJN.txt", fstream::app);
    bool accepted = true;
    string archAlum = "salida_exec_alumno";
    char c;

    ifstream respCorrecta(archSalidaEsperada.c_str());
    ifstream respAlumno(archAlum.c_str());

    if(!respAlumno.good())
    {
        cout << "No se puede leer el archivo de resultado del alumno." << endl;
    }
    else
    {
        //cout << "Abrí el archivo del alumno." << endl;
    }
    if(!respCorrecta.good())
    {
        cout << "No se puede abrir la salida esperada." << endl;
        return false;
    }
    else
    {
        //cout << "Abrí el archivo de salida esperada." << endl;
    }
    cout << "\nLa respuesta esperada es:" << endl;
    cout << "-----" << endl;
    while(respCorrecta.get(c))
    {
        cout << c;
    }
    cout << "-----" << endl;

    cout << endl << "La respuesta del alumno fue:" << endl;
    cout << "-----" << endl;
    while(respAlumno.get(c))
    {
        cout << c;
    }
    cout << "-----" << endl << endl;

    respCorrecta.close();
    respAlumno.close();

    respCorrecta.open(archSalidaEsperada.c_str());
    respAlumno.open(archAlum.c_str());

    if(compareWhite)
    {
        char caracterEsp, caracterAlum;
        //cout << endl << "\tComparo en modo estricto " << archSalidaEsperada << " con " << archAlum << endl;
        while(respCorrecta.good())
        {
            respCorrecta.get(caracterEsp);
            //Comparo caracter por caracter
            if(respAlumno.good())
            {
                respAlumno.get(caracterAlum);

                cout << "Comparo ";
                switch(caracterEsp)
                {
                    case '\n':
                        cout << "\\n";
                    case '\r':
                        cout << "\\r";
                    case '\t':
                        cout << "\\t";
                    default:
                        cout << caracterEsp;
                }
                cout << " con ";
                switch(caracterAlum)
                {
                    case '\n':
                        cout << "\\n";
                    case '\r':
                        cout << "\\r";
                    case '\t':
                        cout << "\\t";
                    default:
                        cout << caracterEsp;
                }
                cout << "\t";
                if(caracterEsp != caracterAlum)
                {
                    accepted = false;
                    cout << "Mal" << endl;
                }
                else
                {
                    cout << "Bien" << endl;
                }
            }
            else
            {
                accepted = false;

                cout << "El archivo de correcto es más largo que el de respuesta del alumno." << endl;
                cout << "Sugerencia: Agregue salto de linea al final del archivo del alumno." << endl;

                break;
            }
        }
        if(respAlumno.peek() != EOF)
        {
            accepted = false;

            cout << "El archivo de respuesta del alumno es más largo que el correcto." << endl;
            cout << "Sugerencia: Quite un salto de linea o espacio al final del archivo del alumno." << endl;
        }
        cout << "Terminé de comparar." << endl;
    }
    else        // Modo normal.
    {
        string cadenaAlum, cadenaEsp;

        //cout << endl << "\tComparo en modo normal " << archSalidaEsperada << " con " << archAlum << endl;

        while(respCorrecta >> cadenaEsp)
        {
            if(respAlumno >> cadenaAlum)
            {
                //cout << "Comparo " << cadenaAlum << " con " << cadenaEsp << "\t";
                if(cadenaAlum != cadenaEsp)
                {
                    accepted = false;
                    //cout << "Mal" << endl;
                }
                else
                {
                    //cout << "Bien" << endl;
                }
            }
            else
            {
                accepted = false;
                cout << "La salida del alumno está incompleta." << endl;
            }
        }
        if(respAlumno >> cadenaAlum)
        {
            accepted = false;
            cout << "La salida del alumno tiene más cadenas de caracteres de las que debe." << endl;
        }
    }

    if(accepted) cout << "\tCaso " << archSalidaEsperada << " CORRECTO" << endl;
    else         cout << "\n\tCaso " << archSalidaEsperada << " MAL" << endl;

    respCorrecta.close();
    respAlumno.close();
    cout.close();

    return accepted;
}
