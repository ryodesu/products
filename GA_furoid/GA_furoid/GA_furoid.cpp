// GA_furoid.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define NUM 64
#define LAST_GENE 1000

int now_gene=0;
int now_id = 0;

int now_num_a= 0;
int now_num_b = 0;

int a[LAST_GENE][NUM / 2][NUM / 2];
int b[LAST_GENE][NUM / 2][NUM / 2];

double a_ans[LAST_GENE][NUM / 2];
double b_ans[LAST_GENE][NUM / 2];
double a_sum;
double b_sum;
double total_score[LAST_GENE][NUM / 2];
double rank[LAST_GENE][NUM / 2];

double total_rank[LAST_GENE];
int total_a[LAST_GENE][NUM / 2];
int total_b[LAST_GENE][NUM / 2];



void init_origin_set(void)
{
	int a_count=0;
	int b_count=0;
	int random;

	for (now_id = 0 ; now_id < (NUM / 2) ; now_id++)
	{
		for (int i = 0; i < (NUM); i++)
		{
			random = rand() % 2 + 1;

			if (a_count >= (NUM / 2))
			{
				random = 2;
			}

			else if (b_count >= (NUM / 2))
			{
				random = 1;
			}


			if (random == 1)
			{
				if (a_count < (NUM / 2))
				{
					a[now_gene][now_id][now_num_a] = (i + 1);
					now_num_a += 1;
					a_count += 1;
				}
			}

			else if (random == 2)
			{
				if (b_count < (NUM / 2))
				{
					b[now_gene][now_id][now_num_b] = (i + 1);
					now_num_b += 1;
					b_count += 1;
				}
			}

		}
		now_num_a = 0;
		now_num_b = 0;
		a_count = 0;
		b_count = 0;
	}
	now_id = 0;
}





void next_set(void)
{

	int cross_point_1;
	int cross_point_2;
	int temp_omega;
	int lp = 0;

	if (now_gene <= (LAST_GENE - 2))
	{
		for (int i = 0; i < (NUM / 2); i++)
		{
			for (int j = 0; j < (NUM / 2); j++)
			{
				a[now_gene + 1][i][j] = a[now_gene][i][j];
				b[now_gene + 1][i][j] = b[now_gene][i][j];
			}
		}


		for (int j = (NUM/4); j < (NUM/2); j++)
		{
			cross_point_1 = rand() % (NUM / 2);
			cross_point_2 = rand() % (NUM / 2);
			temp_omega = a[now_gene + 1][j][cross_point_1];
			a[now_gene + 1][j][cross_point_1] = b[now_gene + 1][j][cross_point_2];
			b[now_gene + 1][j][cross_point_2] = temp_omega;
		}

		now_gene += 1;
	}
}

void calc(void)
{
	

	for (int j = 0; j <= now_gene; j++)
	{
		for (now_id = 0; now_id < (NUM / 2); now_id++)
		{


			for (int i = 0; i < (NUM / 2); i++)
			{
				a_sum = a_sum + (double)sqrt(a[now_gene][now_id][i]);
				b_sum = b_sum + (double)sqrt(b[now_gene][now_id][i]);
			}
			a_ans[now_gene][now_id] = a_sum;
			a_sum = 0;
			b_ans[now_gene][now_id] = b_sum;
			b_sum = 0;
			total_score[now_gene][now_id] = a_ans[now_gene][now_id] - b_ans[now_gene][now_id];
			if (total_score[now_gene][now_id] < 0)
			{
				total_score[now_gene][now_id] *= (-1);
			}


		}
	}
	now_id = 0;
}

void final_set(void)
{

	int temp_alfa;
	int temp_beta;

	for (int i = 0; i < LAST_GENE; i++)
	{
		total_rank[i] = total_score[i][0];
		
		for (int j = 0; j < (NUM / 2); j++)
		{
			total_a[i][j] = a[i][0][j];
			total_b[i][j] = b[i][0][j];

		}
	}

	for (int t = 0; t < LAST_GENE; t++)
	{
		for (int k = 0; k < (NUM / 2)-1; k++)
		{
			if (total_a[t][k] > total_a[t][k + 1])
			{
				temp_alfa = total_a[t][k];
				total_a[t][k] = total_a[t][k + 1];
				total_a[t][k + 1] = temp_alfa;
			}

			if (total_b[t][k] > total_b[t][k + 1])
			{
				temp_beta = total_b[t][k];
				total_b[t][k] = total_b[t][k + 1];
				total_b[t][k + 1] = temp_beta;
			}
		}
	}
}


void sort_score(void)
{
	double temp;
	double temp_2_a;
	double temp_2_b;
	int   temp_3_a;
	int   temp_3_b;

	

	for (int i = 0; i < (NUM / 2); ++i)
	{
		for (int j = i + 1; j < (NUM / 2); ++j)
		{
			if (total_score[now_gene][i] > total_score[now_gene][j])
			{
				temp = total_score[now_gene][i];
				total_score[now_gene][i] = total_score[now_gene][j];
				total_score[now_gene][j] = temp;
				
					temp_2_a = a_ans[now_gene][i];
					a_ans[now_gene][i]= a_ans[now_gene][j];
					a_ans[now_gene][j] = temp_2_a;

					temp_2_b = b_ans[now_gene][i];
					b_ans[now_gene][i] = b_ans[now_gene][j];
					b_ans[now_gene][j] = temp_2_b;

					for (int k = 0; k < (NUM / 2); k++)
					{
						temp_3_a = a[now_gene][i][k];
						a[now_gene][i][k]= a[now_gene][j][k];
						a[now_gene][j][k] = temp_3_a;

						temp_3_b = b[now_gene][i][k];
						b[now_gene][i][k] = b[now_gene][j][k];
						b[now_gene][j][k] = temp_3_b;
					}

				
			}
		}
	}
}


void print(void)
{
	

	printf("\n\n各世代の最も良い遺伝子は\n");
	for (int i = 0; i < LAST_GENE-1; i++)
	{
		printf("\n第%d世代 :%lf \n遺伝子データA(" , i+1 , total_rank[i]);
		for (int j = 0; j < (NUM / 2); j++)
		{
			printf("%d,", total_a[i][j]);
		}
		printf(")\n遺伝子データB(");
		for (int j = 0; j < (NUM / 2); j++)
		{
			printf("%d,", total_b[i][j]);
		}
		printf(")\n");
	}

	
}


int main()
{

	srand(time(NULL));

	init_origin_set();

	for (int i = 0; i < (LAST_GENE-1);i++)
	{
		
		calc();
		sort_score();
		

		next_set();
		
	}

	final_set();

	print();
	


	
    return 0;

}

