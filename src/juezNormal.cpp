bool juezNormal(bool strictEval, string archSalidaEsperada, string archAlum)
{
    ofstream cout("logJN.txt", fstream::app);
    bool accepted = true;

    ifstream respCorrecta(archSalidaEsperada.c_str());
    ifstream respAlumno(archAlum.c_str());

    cout << "******************************" << endl;
    if(!respAlumno.good())
    {
        cout << "No puedo leer el archivo de resultado del alumno." << endl;
    }
    else
    {
        cout << "Abrí el archivo del alumno." << endl;
    }
    if(!respCorrecta.good())
    {
        cout << "No pude abrir la respuesta correcta." << endl;
        return false;
    }
    else
    {
        cout << "Abrí la respuesta correcta." << endl;
    }
    /*cout << "El archivo de respuesta correcta es:" << endl;
    cout << "--" << endl;
    while(respCorrecta.good())
    {
        char c;
        respCorrecta.get(c);
        cout << c;
    }
    cout << "--" << endl;

    cout << "La salida del alumno fue:" << endl;
    cout << "--" << endl;
    while(respAlumno.good())
    {
        char c;
        respAlumno.get(c);
        cout << c;
    }
    cout << "--" << endl;
    respAlumno.seekg (0, ios::beg);
    respCorrecta.seekg (0, ios::beg);*/
    if(strictEval)
    {
        char caracterEsp, caracterAlum;
        cout << "Comparo en modo estricto con " << archSalidaEsperada << " con " << archAlum << endl;
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
                if(caracterEsp != caracterAlum)
                {
                    accepted = false;
                    cout << "\tMal" << endl;
                }
                else
                {
                    cout << "\tBien" << endl;
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
    else
    {
        string cadenaAlum, cadenaEsp;

        cout << "Comparo en modo normal con: "  << archSalidaEsperada << endl;

        while(respCorrecta >> cadenaEsp)
        {
            if(respAlumno >> cadenaAlum)
            {
                cout << "Comparo " << cadenaAlum << " con " << cadenaEsp << endl;
                if(cadenaAlum != cadenaEsp)
                {
                    accepted = false;
                    cout << "\tMal" << endl;
                }
                else
                {
                    cout << "\tBien" << endl;
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
    respCorrecta.close();
    respAlumno.close();
    if(accepted) cout << "Caso " << archSalidaEsperada << " correcto" << endl;
    else         cout << "Caso " << archSalidaEsperada << " mal" << endl;
    cout.close();
    return accepted;
}
