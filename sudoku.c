// ConsoleApplication1.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define _CRT_SECURE_NO_WARNINGS
#define MAX_SIZE 9

void file_open_and_generate_formula();
void solve_formula();
void interprete_Z3_output();

void file_open_and_generate_formula() {
	int a[MAX_SIZE][MAX_SIZE] = { 0 };
	int el[MAX_SIZE][MAX_SIZE] = { 0 };
	int i, j;

	//fopen() - file read
	FILE *f;
	f = fopen("input.txt", "r");
	printf("reads an input form input.txt...\n");
	if (f == NULL) {
		fputs("Cannot open input file...\n", stderr);
		return;
	}
	for (i = 0;i < 9;i++) {
		for (j = 0;j < 9;j++) {
			a[i][j] = (int)fgetc(f);
			if (isspace(a[i][j]))
				j--;
			if (a[i][j] == '?') {
				a[i][j] = '0';
				el[i][j] = 1; //The part that contains zero is the variable that prevents you from assigning a value.
			}
		}
	}
	fclose(f);

	//generate formula
	int x, y;
	int v = 0; //A variable that prevents the vertical assertion functions from being de-duplified.
	int t = 0; //A variable that prevents the horizontal assertion functions from being de-duplified.
	int r = 0; int s = 0; //3x3 part variable
	int e = 2; //A variable that prevents the 3x3 area assertion functions from being de-duplified.

	FILE *fl;
	fl = fopen("formula.txt", "w");
	printf("generaties a corresponding formula as formula.txt...\n");
	for (y = 1; y <= 9; y++)
		for (x = 1; x <= 9; x++)
			fprintf(fl, "(declare-const a%d%d Int)\n", y, x);

	for (y = 1; y <= 9; y++)
		for (x = 1; x <= 9; x++) {
			if (el[y - 1][x - 1] == 0)
				fprintf(fl, "(assert (= a%d%d %c))\n", y, x, a[y - 1][x - 1]);
		}

	for (y = 1; y <= 9; y++)
		for (x = 1; x <= 9; x++)
			fprintf(fl, "(assert (and (<= a%d%d 9) (<= 1 a%d%d)))\n", y, x, y, x);

	for (y = 1; y <= 9; y++) {
		if (y % 3 == 1) e -= 2;
		for (x = 1; x <= 9; x++) {
			int i_y, i_x;

			//row
			if (v == 0)
			{
				fprintf(fl, "(assert(distinct ");
				for (i_y = 1; i_y <= 9; i_y++)
					fprintf(fl, "a%d%d ", i_y, x);
				fprintf(fl, "))\n");
			}

			//column
			if (t == 0)
			{
				fprintf(fl, "(assert(distinct ");
				for (i_x = 1; i_x <= 9; i_x++)
					fprintf(fl, "a%d%d ", y, i_x);
				fprintf(fl, "))\n");
				t = 1; //column assertion off
			}

			//3x3 area
			r = ((x % 3 == 1) && (y % 3 == 1)) ? x / 3 : r;
			s = (((y % 3 == 1) && (x % 3 == 1)) && (y >= x)) ? y / 3 : s;

			if (e == (r + s))
			{
				fprintf(fl, "(assert(distinct ");
				for (i_y = 1; i_y <= 3; i_y++)
				{
					for (i_x = 1; i_x <= 3; i_x++)
					{
						fprintf(fl, "a%d%d ", (3 * s + i_y), (3 * r + i_x));
					}
				}
				fprintf(fl, "))\n");
				e++;
			}
		}
		v = 1; //row assertion off
		t = 0; //column assertion on
		r = 0;
	}
	fprintf(fl, "(check-sat)\n(get-model)\n");
	fclose(fl);
}
void solve_formula() {
	system("z3 formula.txt>solution.txt");	
	printf("runs Z3 to solve formula.txt and then receives the result...\n");
}
void interprete_Z3_output() {
	char gar1[45];
	char gar2[45];
	int table[MAX_SIZE][MAX_SIZE] = { 0 };
	int temp_x[82] = { 0 };
	int temp_y[82] = { 0 };
	int num[82] = { 0 }, bit[82] = { 0 };
	FILE *fll;
	fll = fopen("solution.txt", "r");
	printf("interprets the Z3 output and print out the solution to output.txt...\n");

	fscanf(fll, "%s\n", gar1);
	fscanf(fll, "%s\n", gar2);
	int i, j;
	for (i = 0; i < 81; i++)
	{
		fscanf(fll, "  (define-fun a%d () Int\n %d)", &num[i], &bit[i]);
		temp_x[i] = num[i] / 10;
		temp_y[i] = num[i] % 10;
	}
	fclose(fll);
	int t = 0;
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			for (t = 0; t < 81; t++)
			{
				if ((temp_x[t] - 1 == i) && (temp_y[t] - 1 == j)) {
					table[i][j] = bit[t];
				}
			}
		}
	}
	printf("print out the solution to output.txt...\n");
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			printf("%d ", table[i][j]);
		}
		printf("\n");
	}
	FILE *fl;
	fl = fopen("output.txt", "w");
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			fprintf(fl,"%d ", table[i][j]);
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

// ���α׷� ����: <Ctrl+F5> �Ǵ� [�����] > [��������� �ʰ� ����] �޴�
// ���α׷� �����: <F5> Ű �Ǵ� [�����] > [����� ����] �޴�

// ������ ���� ��: 
//   1. [�ַ�� Ž����] â�� ����Ͽ� ������ �߰�/�����մϴ�.
//   2. [�� Ž����] â�� ����Ͽ� �ҽ� ��� �����մϴ�.
//   3. [���] â�� ����Ͽ� ���� ��� �� ��Ÿ �޽����� Ȯ���մϴ�.
//   4. [���� ���] â�� ����Ͽ� ������ ���ϴ�.
//   5. [������Ʈ] > [�� �׸� �߰�]�� �̵��Ͽ� �� �ڵ� ������ ����ų�, [������Ʈ] > [���� �׸� �߰�]�� �̵��Ͽ� ���� �ڵ� ������ ������Ʈ�� �߰��մϴ�.
//   6. ���߿� �� ������Ʈ�� �ٽ� ������ [����] > [����] > [������Ʈ]�� �̵��ϰ� .sln ������ �����մϴ�.