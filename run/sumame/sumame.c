#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EQUIPOS  60
#define CADENAS 100
#define PRUEBAS  10

#define PROGRAMA "sumame"
#define PROG     "s"

int main(void)
{
  int i, j, k, ok;
  char codigo[CADENAS], prueba[CADENAS], lenguaje[CADENAS];
  FILE *evaluacion, *entrada, *datos, *salida;
  long long int rd, rs;

  evaluacion = fopen(PROGRAMA ".txt", "w");
  fprintf(evaluacion, "Calificaciones de " PROGRAMA "\n\n");
  for (i = 0; i <= EQUIPOS; i++) {
    k = 0;
    system("rm ./" PROG);
    sprintf(codigo, "gcc -lm " PROGRAMA "%02d.c -o " PROG, i);
    printf("%s\n", codigo);
    strcpy(lenguaje, "(gcc)");
    ok = system(codigo);
    if (ok) {
      sprintf(codigo, "g++ -lm -include /usr/include/stdlib.h -Wno-deprecated " PROGRAMA "%02d.cpp -o " PROG, i);
	printf("%s\n", codigo);
      strcpy(lenguaje, "(g++)");
      ok = system(codigo);
    }
    if (ok) {
      sprintf(codigo, "gcj --main=" PROGRAMA "%02d -Wall " PROGRAMA "%02d.java -o " PROG, i, i);
      printf("%s\n", codigo);
      strcpy(lenguaje, "(gcj)");
      ok = system(codigo);
    }
    if (ok) {
      sprintf(codigo, "fpc " PROGRAMA "%02d.pas -o" PROG, i);
      printf("%s\n", codigo);
      strcpy(lenguaje, "(fpc)");
      ok = system(codigo);
    }
    if (!ok) {
      printf("Evaluando estudiante %02d\n", i);
    for (j = 0; j < PRUEBAS; j++) {
      /* printf("Prueba %d\n", j); */
      sprintf(codigo, "./" PROG " < " PROGRAMA "%02d.ent > " PROGRAMA "%02d.sal", j, j);
      system(codigo);
      sprintf(prueba, PROGRAMA "%02d.ent", j);
      entrada = fopen(prueba, "r");
      sprintf(prueba, PROGRAMA "%02d.dat", j);
      datos = fopen(prueba, "r");
      sprintf(prueba, PROGRAMA "%02d.sal", j);
      if ((salida = fopen(prueba, "r")) != NULL) {
	fscanf(datos, "%lld", &rd);
	fscanf(salida, "%lld", &rs);
      if (rd == rs) k++;
      fclose(salida);}
      fclose(datos);
      fclose(entrada);
      sprintf(codigo, "rm " PROGRAMA "%02d.sal", j);
      system(codigo);
    }
    fprintf(evaluacion, "Estudiante %02d Calificacion %2d %s\n", i, k, lenguaje);
    }
  }
  fprintf(evaluacion, "\nSi tu calificacion no aparece es porque no recibi tu"
	  "\n programa o porque no compila con gcc, g++, gcj o fpc.\n\n"
	  "Si tu calificacion es menor a lo que esperabas es probable\n"
	  "que tu programa este mal o que imprima letreros o que no\n"
	  "separe la salida de la forma pedida (es decir, esta mal).\n");
  fclose(evaluacion);
  return 0;
}
