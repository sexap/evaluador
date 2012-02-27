/*Problema 2: Tarea 1
NOMBRE: Angel Antonio Santos Palacios
MATRICULA: 204360519
NUMERO DE LISTA 27:
*/

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
public class parsum27 {
int rep=0;
	public parsum27() throws IOException{
	String cadena = "";
	InputStreamReader texto = new InputStreamReader(System.in);
    BufferedReader br = new BufferedReader(texto);
    cadena= br.readLine();
    
    	 System.out.println(separa(cadena.length(),1));

	}
	public static void main(String[] args) throws IOException {
		new parsum27();

	}
	public int separa(int x, int p)
	{	
		if(x==2)
			return 1;
		return p+separa(x-2,p++);	
	}
}
