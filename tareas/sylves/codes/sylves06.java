import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
public class sylves06 {
	/**
	 * @param args
	 * @throws IOException 
	 */
	public static void main(String[] args) throws IOException {
		// TODO Auto-generated method stub
		int n, m, syl;
		BufferedReader lee = new BufferedReader(new InputStreamReader(System.in));
	    String s = null;
	    s = lee.readLine();
	    n = Integer.parseInt(s.substring(0, s.indexOf(' ')));
	    m = Integer.parseInt(s.substring(s.lastIndexOf(' ')+1));	    	    
	    syl = sylvester(n,m);
	    syl = syl%m;
	    System.out.println(syl);
	}
	static int sylvester(int n, int m){
		int syl=0;
		if(n==0)
	    	return 2;
	    else{
	    	syl=sylvester(n-1,m);
	    	syl=syl*(syl-1)+1;
	    	return syl;	    	
	    }		
	}
}
