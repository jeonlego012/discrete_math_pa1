#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define _CRT_SECURE_NO_WARNINGS
void file_open_and_generate_formula();
void solve_formula();
void interprete_Z3_output();
int sum_y[9];
int sum_x[9];
void file_open_and_generate_formula() {
   char a[3][8] = { 0 };
   int sum_y[8];
   int sum_x[8];
   int i, j;

   //fopen() - file read
   FILE *f;
   f = fopen("input.txt", "r");
   printf("reads an input form input.txt...\n");
   if (f == NULL) {
      fputs("Cannot open input file...\n", stderr);
      return;
   }
   for (i = 0;i < 2;i++) {
      for (j = 0;j < 8;j++) {
         fscanf(f,"%d ", &a[i][j]);
      }
   }
   fclose(f);
   for (i = 0;i < 2;i++) {
      for (j = 0;j < 8;j++) {
         printf("%d ", a[i][j]);
      }
      printf("\n");
   }

   //generate formula
   int x, y;
   FILE *f2;
   f2 = fopen("formula2.txt", "wt");
   for (y = 1; y <= 8; y++) {
      for (x = 1; x <= 8; x++) {
         fprintf(f2, "(declare-const a%d%d Int)\n", y, x);
         /*fprintf(f2, "(declare-const b%d%d Int)\n", y, x);*/
      }
   }
   //for (y = 1; y < 8; y++) {
   //   for (x = y+1; x <= 8; x++) {
   //      fprintf(f2, "(assert (= a%d%d a%d%d))\n", y, x, x, y);
   //   }
   //}
   for (y = 1; y <= 8; y++) {
      for (x = 1; x <= 8; x++) {
         fprintf(f2, "(assert (or (= a%d%d 0) (= a%d%d %d) (= a%d%d %d)))\n", y, x, y, x, y, x, y, x);
      }
   }
   //for (y = 1; y <= 8; y++) {
   //   for (x = y; x <= y; x++) {
   //      fprintf(f2, "(assert (or (= a%d%d 0) (= a%d%d %d)))\n", y, x, y, x, x);
   //   }
   //}

   //row합
   for (y = 1; y <= 8; y++) {
      fprintf(f2, "(assert(= (+");
      for (x = 1; x <= 8; x++) {
         fprintf(f2, " a%d%d", y, x);
      }
      fprintf(f2, ") %d)", a[0][y - 1]);
      //fprintf(f2, "(and");
      //for (x = 1; x <= 8; x++) {
      //   fprintf(f2, " (or (= a%d%d 0) (= a%d%d %d))", y, x, y, x, x);
      //}
      fprintf(f2, ")\n");
   }
   //column합
   for (x = 1; x <= 8; x++) {
      fprintf(f2, "(assert(= (+");
      for (y = 1; y <= 8; y++) {
         fprintf(f2, " a%d%d", y, x);
      }
      fprintf(f2, ") %d)", a[1][x - 1]);
      //fprintf(f2, "(and");
      //for (y = 1; y <= 8; y++) {
      //   fprintf(f2, " (or (= a%d%d 0) (= a%d%d %d))", y, x, y, x, y);
      //}
      fprintf(f2, ")\n");
   }
   fprintf(f2, "(check-sat)\n");
   fprintf(f2, "(get-model)\n");
   fclose(f2);
}
void solve_formula()
{
   system("z3 formula2.txt>solution.txt");
   printf("runs Z3 to solve formula.txt and then receives the result...\n");
}
void interprete_Z3_output()
{
   char gar1[45];
   char gar2[45];
   char table[8][8] = { 0 };
   int temp_x[65] = { 0 };
   int temp_y[65] = { 0 };
   int num[65] = { 0 };
   int bit[65] = { 0 };
   FILE *fll;
   fll = fopen("solution.txt", "r");
   printf("interprets the Z3 output and print out the solution to output.txt...\n");

   fscanf(fll, "%s\n", gar1);
   fscanf(fll, "%s\n", gar2);
   int i, j;
   for (i = 0; i < 64; i++)
   {
      fscanf(fll, "  (define-fun a%d () Int\n %d)", &num[i], &bit[i]);
      temp_x[i] = num[i] / 10;
      temp_y[i] = num[i] % 10;
   }
   for (i = 0; i < 64; i++)
      printf("%d, %d:  %d", temp_x[i], temp_y[i], bit[i]);
   fclose(fll);
   int t = 0;
   for (i = 0; i < 8; i++)
   {
      for (j = 0; j < 8; j++)
      {
         for (t = 0; t < 64; t++)
         {
            if ((temp_x[t] - 1 == i) && (temp_y[t] - 1 == j)) {
               if (bit[t] == 0)
                  table[i][j] = 'o';
               else
                  table[i][j] = 'x';
               //table[i][j] = bit[t];
            }
         }
      }
   }
   printf("print out the solution to output.txt...\n");
   for (i = 0; i < 8; i++)
   {
      for (j = 0; j < 8; j++)
      {
         printf("%d ", table[i][j]);
      }
      printf("\n");
   }
   FILE *f1;
   f1 = fopen("output2.txt", "w");
   for (i = 0; i < 8; i++)
   {
      for (j = 0; j < 8; j++)
      {
         fprintf(f1, "%d ", table[i][j]);
      }
      fprintf(f1, "\n");
   }
   fclose(f1);
}
int main(void) {
   file_open_and_generate_formula();
   solve_formula();
   interprete_Z3_output();
}
