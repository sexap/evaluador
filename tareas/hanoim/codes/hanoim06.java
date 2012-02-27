import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
public class hanoim06 {
	/**
	 * @param args
	 * @throws IOException 
	 */
	public static void main(String[] args) throws IOException {
		// TODO Auto-generated method stub
		int n;
		int de=1, a=3;
		BufferedReader lee = new BufferedReader(new InputStreamReader(System.in));
	    String s = null;
	    s = lee.readLine();
	    n = Integer.parseInt(s);
	    han(n,de,a);
	    System.out.println("0 0");	    
	}	
	static void han(int n, int de, int a){
		if(n==1)
			mueve(de,a);
		else{
			han(n-1, 1, 3);
			mueve(1, 2);
			han(n-1, 3, 1);
			mueve(2,3);
			han(n-1,1,3);
		}			
	}
	static void mueve(int de, int a){
		if(a==2)
			System.out.println(de+" 2");
		else if(de==2)
			System.out.println("2 "+a);
		else{
			System.out.println(de+" 2");
			System.out.println("2 "+a);
		}
	}	
}
