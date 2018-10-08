
#include <stdio.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
#define MAX_SIZE 9

void file_open_and_generate_formula();
void solve_formula();

void file_open_and_generate_formula() {
	int a[MAX_SIZE][MAX_SIZE] = { 0 };
	int el[MAX_SIZE][MAX_SIZE] = { 0 };
	int i, j;
 
	//fopen() - file read
	FILE *f;
	f = fopen("3inarowinput.txt", "r");
	
	printf("reads an input form input.txt...\n");

	
	if (f == NULL) {
		fputs("Cannot open input file...\n", stderr);
		return;
	}
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			a[i][j] = (int)fgetc(f);
			if (a[i][j] == 'o')
				a[i][j] = 1;
			if (a[i][j] == 'x')
				a[i][j] = 0;

			if (isspace(a[i][j]))
				j--;
			if (a[i][j] == '?') {
				a[i][j] = 2;
				el[i][j] = 1; //The part that contains zero is the variable that prevents you from assigning a value.
			}
		}
	}


	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (a[i][j] == 1)
				printf("o ");
			if (a[i][j] == 0)
				printf("x ");
			if (a[i][j] == 2)
				printf("? ");
		}
		printf("\n");

	}


	//for (i = 0;i < 8;i++) {
	//	for (j = 0;j < 8;j++) {
	//		printf("%c", a[i][j]);
	//	}
	//	printf("\n");
	//}
	fclose(f);

	//generate formula
	int x, y;
	
 printf("generaties a corresponding formula as formula.txt...\n");

	FILE *fl;
	fl = fopen("formula2.txt", "w");
	for (y = 1; y <= 8; y++)
		for (x = 1; x <= 8; x++)
			fprintf(fl, "(declare-const a%d%d Int)\n", y, x);

	for (y = 1; y <= 8; y++)
		for (x = 1; x <= 8; x++) {
			if (el[y - 1][x - 1] == 0)
				fprintf(fl, "(assert (= a%d%d %d))\n", y, x, a[y - 1][x - 1]);
		}

	for (y = 1; y <= 8; y++)
		for (x = 1; x <= 8; x++)
			fprintf(fl, "(assert (and (<= a%d%d 1) (<= 0 a%d%d)))\n", y, x, y, x);
	//in each row, the number of x and the number of o should be the same           
	for (y = 1; y <= 8; y++)
	{
		fprintf(fl, "(assert (= 4 (+  ");
		for (x = 1; x <= 8; x++)
			fprintf(fl, " a%d%d", y, x);

		fprintf(fl, " )))\n  ");
	}

	//in each column, the number of x and the number of o should be the same           

	for (y = 1; y <= 8; y++)
	{
		fprintf(fl, "(assert (= 4 (+  ");
		for (x = 1; x <= 8; x++)
			fprintf(fl, " a%d%d", x, y);

		fprintf(fl, " )))\n  ");
	}



	int i_h1, i_h2;
	int i_y, i_x;
	//to prevent three consecutive cells in a column from having all 'o' or 'x'
	for (x = 1; x <= 8; x++) {

		for (i_y = 3; i_y <= 8; i_y++) {

			i_h1 = i_y - 1;
			i_h2 = i_y - 2;
			fprintf(fl, "(assert(and (< (+ a%d%d ", i_y, x);
			fprintf(fl, "(+ a%d%d ", i_h1, x);
			fprintf(fl, "a%d%d)) 3) ", i_h2, x);

			fprintf(fl, "(> (+ a%d%d", i_y, x);
			fprintf(fl, "(+ a%d%d ", i_h1, x);
			fprintf(fl, "a%d%d)) 0)))  \n ", i_h2, x);
		}


	}

	//to prevent three consecutive cells in a row from having all 'o' or 'x'
	for (y = 1; y <= 8; y++) {

		for (i_x = 3; i_x <= 8; i_x++) {
			i_h1 = i_x - 1;
			i_h2 = i_x - 2;
			fprintf(fl, "(assert(and (< (+ a%d%d ", y, i_x);
			fprintf(fl, "(+ a%d%d ", y, i_h1);
			fprintf(fl, "a%d%d)) 3) ", y, i_h2);

			fprintf(fl, "(> (+ a%d%d", y, i_x);
			fprintf(fl, "(+ a%d%d ", y, i_h1);
			fprintf(fl, "a%d%d)) 0))) \n ", y, i_h2);
		}

	}



	fprintf(fl, "(check-sat)\n(get-model)\n");
	fclose(fl);
}


void solve_formula() {
   system("z3 formula2.txt>solution2.txt"); 
   printf("runs Z3 to solve formula.txt and then receives the result...\n");
}



void interprete_Z3_output() {
char gar1[45];
char gar2[45];

  int table1[MAX_SIZE][MAX_SIZE]={0} ;
	int table2[MAX_SIZE][MAX_SIZE]={0} ;
int num=0, bit=0;
int i;
int x,y;
FILE *fll;
  fll = fopen("solution2.txt", "r");
   printf("interprets the Z3 output and print out the solution to output.txt...\n");
   fscanf(fll, "%s",gar1);
   fscanf(fll, "%s",gar2);
  for (i = 0; i < 64; i++)
   {
   	 num=0;
	bit=0;
	fscanf(fll, "  (define-fun a%d () Int\n %d)", &num, &bit);
   for (y = 0; y < 8; y++)
	for (x = 0; x < 8; x++)
   {
   if(num==10*(x+1)+y+1)
   {
   table1[x][y]=num;
   table2[x][y]=bit;
   }
   }
    }
    fclose(fll);
     printf("print out the solution to output.txt...\n");

   	for (x = 0; x < 8; x++) {
		for (y = 0; y < 8; y++) {
			if (table2[x][y] == 1)
				printf("o ");
			if (table2[x][y] == 0)
				printf("x ");
	}
		printf("\n");

	}

 FILE *fl;
   fl = fopen("output2.txt", "w");
   for (x = 0; x < 8; x++)
   {
      for (y = 0; y < 8; y++)
      {
        
		if (table2[x][y] == 1)
				fprintf(fl,"o ");
			if (table2[x][y] == 0)
				fprintf(fl,"x ");
		
		
      }
      fprintf(fl,"\n");
   }
   fclose(fl);



}


int main()
{
	

  file_open_and_generate_formula();
   solve_formula();
   interprete_Z3_output();

}
