// ConsoleApplication1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
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

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.