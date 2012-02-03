/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

import java.io.*;
/**
 *
 * @author jmh
 */
public class sumame20 {

    /**
     * @param args the command line arguments
     */
    static int V[] = new int[100];
    
    static int msumasi(int n, int k, int p, int q) {
       int i, j = 0;
       if( n > 0 ) {
          if( k >= n && V[n] != 0) 
              return V[n];
          for(i = 1; i <= k && i <= n; i++) {
            if(p <= i && i <= q)
                j += msumasi(n-i, i-1, p, q);
            if( k >= n ) 
                V[n] = j;
          }
       }
       else return 1;
       return j;
    }
    
    public static void main(String[] args) {
        BufferedReader entrada = new BufferedReader(new InputStreamReader(System.in));
        String datos = "";
        try{
            datos = entrada.readLine();
        }
        catch(Exception e){
            System.out.println(e.getMessage());
        }
        String data[] = datos.split(" ");
        int n = Integer.parseInt(data[0]);
        int p = Integer.parseInt(data[1]);
        int q = Integer.parseInt(data[2]);
        for(int i=0; i<n; i++)
            V[i]=0;
        System.out.println(msumasi(n, n, p, q));
    }
}
