import java.io.*;
public class parsum06 {
	/**
	 * @param args
	 * @throws IOException 
	 */
	public static void main(String[] args) throws IOException {
		// TODO Auto-generated method stub
		  int i=0;
		  BufferedReader lee = new BufferedReader(new InputStreamReader(System.in));
	      String s = null;
	      s = lee.readLine();	      	     	     	      
	      for(int x=0; x<s.length(); x++){
	    	  if(s.charAt(x) == '(')
	    		  i++;
	      }
	      System.out.println(i);
	}
}