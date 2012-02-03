
import java.io.*;

public class sumame30 {
     static int p, q;
     static int v[] = new int[100] ;


    public static void main(String[] args) throws IOException{
        String a = "";

        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        a = br.readLine();
        int n = Integer.parseInt(a.substring(0,a.indexOf(" ")));
        a = a.replaceFirst(" ","-");
        p= Integer.parseInt(a.substring(a.indexOf("-")+1,a.indexOf(" ")));
        q = Integer.parseInt( a.substring(a.indexOf(" ")+1) );

          for(int i=0; i<n; i++)
          v[i]=0;

          System.out.println(msumasi(n,n));
       
    }

      public static int msumasi(int n, int k){
         int i, j = 0;
               if( n > 0 ) {
                  if( k >= n && v[n] != 0) return v[n];
                  for(i = 1; i <= k && i <= n; i++) {
                      if(p <= i && i <= q)
                        j += msumasi(n-i, i-1);
                  if( k >= n ) v[n] = j;
                  }
               }
               else return 1;
               return j;

      }

    }




