bool juezNormal(bool strictEval, string archSalidaEsperada, string archAlum)
{
    ofstream cout("logJN.txt", fstream::app);
    bool accepted = true;

    ifstream respCorrecta(archSalidaEsperada.c_str());
    ifstream respAlumno(archAlum.c_str());

    cout << "******************************" << endl;
    if(!respAlumno.good())
    {
        cout << "No se puede leer el archivo de resultado del alumno." << endl;
    }
    else
    {
        cout << "Abrí el archivo del alumno." << endl;
    }
    if(!respCorrecta.good())
    {
        cout << "No se puede abrir la salida esperada." << endl;
        return false;
    }
    else
    {
        cout << "Abrí el archivo de salida esperada." << endl;
    }
    cout << "El archivo de respuesta esperada es:" << endl;
    cout << "---" << endl;
    while(respCorrecta.good())
    {
        char c;
        respCorrecta.get(c);
        cout << c;
    }
    cout << "---" << endl;

    cout << endl << "La salida del alumno fue:" << endl;
    cout << "---" << endl;
    while(respAlumno.good())
    {
        char c;
        respAlumno.get(c);
        cout << c;
    }
    cout << "---" << endl;

    respCorrecta.close();
    respAlumno.close();

    respCorrecta.open(archSalidaEsperada.c_str());
    respAlumno.open(archAlum.c_str());

    if(strictEval)
    {
        char caracterEsp, caracterAlum;
        cout << endl << endl << "Comparo en modo estricto " << archSalidaEsperada << " con " << archAlum << endl << endl;
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

        cout << endl << endl << "Comparo en modo normal " << archSalidaEsperada << " con " << archAlum << endl << endl;

        while(respCorrecta >> cadenaEsp)
        {
            if(respAlumno >> cadenaAlum)
            {
                cout << "Comparo " << cadenaAlum << " con " << cadenaEsp << "\t";
                if(cadenaAlum != cadenaEsp)
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
                cout << "La salida del alumno está incompleta." << endl;
            }
        }
        if(respAlumno >> cadenaAlum)
        {
            accepted = false;
            cout << "La salida del alumno tiene más cadenas de caracteres de las que debe." << endl;
        }
    }
    cout << endl;
    if(accepted) cout << "Caso " << archSalidaEsperada << " correcto" << endl;
    else         cout << "Caso " << archSalidaEsperada << " mal" << endl;

    respCorrecta.close();
    respAlumno.close();
    cout.close();

    return accepted;
}
