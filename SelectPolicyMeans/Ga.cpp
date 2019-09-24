#include "Ga.h"
#include <stdio.h>

#define GENERATION_NUM	360
#define INDIVIDUAL_NUM	40
#define WorldNum 6

Para para;

int ga_facility[INDIVIDUAL_NUM][100][100];
int ga_housePop[INDIVIDUAL_NUM][100][100];
int ga_facilityNum[INDIVIDUAL_NUM];
int ga_subsidy[INDIVIDUAL_NUM];
double ga_moveProb[INDIVIDUAL_NUM];
int ga_moveNum[INDIVIDUAL_NUM];
double ga_fitness[INDIVIDUAL_NUM];
double ga_facilityPop[INDIVIDUAL_NUM][100][100];
int NowGene = 0;

int best_facility[100][100];
int best_housePop[100][100];
double best_facilityPop[100][100];
int best_subsidy;
double best_fitness;

//コンストラクタ
Ga::Ga()
{
	Ga::centerZoneNum = (para.centerZoneVID2 - para.centerZoneVID1 + 1) * (para.centerZoneHID2 - para.centerZoneHID1 + 1);
	Ga::surroundingZoneNum = para.zoneVNum * para.zoneHNum - Ga::centerZoneNum;
	Ga::rep = para.transTimeLimit / para.transTimeUnit;
	Ga::fit = para.transCostUnit * para.transTimeUnit * para.receiveRatio * 0.73;
}
//デストラクタ
Ga::~Ga()
{

}

void Ga::Ga_SampleInit(void)
{

	for (int i = 0; i < para.zoneVNum; i++)
	{
		for (int j = 0; j < para.zoneHNum; j++)
		{
			if ((i == 0 && j == 3) || (i == 0 && j == 6) || (i == 2 && j == 6) || (i == 2 && j == 8) || (i == 3 && j == 2) || (i == 5 && j == 3) || (i == 5 && j == 8) || (i == 6 && j == 4) || (i == 7 && j == 0 || (i == 7 && j == 4 || (i == 8 && j == 6))))
			{
				ga_facility[0][i][j] = 1;
				ga_housePop[0][i][j] = para.initialZonePopNum;
				ga_facilityPop[0][i][j] = 0.0;
			}
			else
			{
				ga_facility[0][i][j] = 0;
			}

		}
	}
	ga_subsidy[0] = 10;
	ga_moveNum[0] = 0;
	ga_facilityNum[0] = 0;

	Ga_Calc(0);

}

void Ga::Ga_Print(void) {
	//printf("centerzonenum=%d\n surroundingzonenum=%d\n rep=%d\n fit=%lf\n", centerZoneNum, surroundingZoneNum, rep, fit);
	
	//printf("%d世代　ga_fitness[0]=%lf\n", NowGene, ga_fitness[0]);
		for (int h = 0; h < INDIVIDUAL_NUM; h++)
		{
			//printf("ga_facility[%d][%d]\n", NowGene,h);
			//printf("ga_housePop[%d][%d]\n",g, h);
			for (int i = 0; i < para.zoneVNum; i++)
			{
				for (int j = 0; j < para.zoneHNum; j++)
				{
					//printf("%d", ga_facility[h][i][j]);
					//printf(" %d", ga_housePop[h][i][j]);
				}
				//printf(" \n");//ga_facility用の区切り
			}
			//printf("\n");
			//printf("ga_subsidy[%d][%d]=%d", g,h, ga_subsidy[g][h]);
			//printf("ga_facilityNum[%d]=%d\n", h, ga_facilityNum[h]);
			//printf("ga_moveProb[%d][%d]=%lf\n", g, h, ga_moveProb[g][h]);
			printf("%d世代　ga_fitness[%d]=%lf\n\n\n", NowGene, h, ga_fitness[h]);
			//printf("%d世代　fitness=%lf \n",NowGene, ga_fitness[h]);
			
		}
		//printf("fitness=%lf\n", ga_fitness[0]);
	/*
		for (int i = 0; i < para.zoneVNum; i++)
		{
			for (int j = 0; j < para.zoneHNum; j++)
			{
				printf("%d", ga_facility[0][i][j]);
				//printf(" %d", ga_housePop[h][i][j]);
			}
			//printf(" ");//ga_facility用の区切り
		}
		printf("\n");
		printf("fitness=%lf\n", ga_fitness[0]);
		*/
}

void Ga::Ga_Init(void) {

	
	double r;

	for (int h = 0; h < INDIVIDUAL_NUM; h++)
	{
		for (int i = 0; i < para.zoneVNum; i++)
		{
			for (int j = 0; j < para.zoneHNum; j++)
			{
				r = (double)rand() / RAND_MAX;
				if (r > 0.8)//あえて多くするとよい結果になった
				{
					ga_facility[h][i][j] = 0;
				}

				else
				{
					ga_facility[h][i][j] = 1;
					ga_housePop[h][i][j] = para.initialZonePopNum;
					ga_facilityPop[h][i][j] = 0.0;
				}
				
			}
		}
		ga_subsidy[h] = rand() % (para.subsidyLevelNum + 1);
		ga_moveNum[h] = 0;
		ga_facilityNum[h] = 0;
	}
}

void Ga::Ga_ReGenerate(int h)
{
	double r;

	for (int i = 0; i < para.zoneVNum; i++)
	{
		for (int j = 0; j < para.zoneHNum; j++)
		{
			r = (double)rand() / RAND_MAX;
			if (r > 0.7)
			{
				ga_facility[h][i][j] = 0;
			}

			else
			{
				ga_facility[h][i][j] = 1;
				ga_housePop[h][i][j] = para.initialZonePopNum;
				ga_facilityPop[h][i][j] = 0.0;
			}

		}
	}
	ga_subsidy[h] = rand() % (para.subsidyLevelNum + 1);
	ga_moveNum[h] = 0;
	ga_facilityNum[h] = 0;
}

void Ga::Ga_PareCalc(void)
{
	
	
		for (int h = 0; h < INDIVIDUAL_NUM; h++)
		{

			Ga_Calc(h);

			if (NowGene == 0)
			{
			
				if (ga_fitness[h] >= DBL_MAX)
				{
					while (ga_fitness[h] >= DBL_MAX)
					{
						Ga_ReGenerate(h);
						Ga_Calc(h);
					}
				}
			}
		}
	
}

void Ga::Ga_Calc(int h) 
{
	
	int i, j, k, x, y, neighbour, fromv, fromh, to, tov, toh;
	double oneDayPop;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
			ga_facilityNum[h] = 0;
		
			for (i = 0; i < para.zoneVNum; i++) 
			{
				for (j = 0; j < para.zoneHNum; j++) 
				{
					ga_facilityPop[h][i][j] = 0.0;
					ga_facilityNum[h] += ga_facility[h][i][j];
					ga_housePop[h][i][j] = para.initialZonePopNum;//初期人数を代入
				}
			}
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		

			ga_moveProb[h] = 1 / (1 + exp(-0.1 * ((double)ga_subsidy[h] / (double)para.subsidyLevelNum * para.subsidyMax) + 5));
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
			ga_moveNum[h] = 0;
			for (fromv = 0; fromv < para.centerZoneVID1; fromv++) {
				for (fromh = 0; fromh < para.zoneHNum; fromh++) {
					for (i = 0; i < para.initialZonePopNum; i++) {
						if ((double)rand() / (double)RAND_MAX < ga_moveProb[h]) {
							ga_housePop[h][fromv][fromh]--;
							to = rand() % centerZoneNum;
							toh = to % (para.centerZoneHID2 - para.centerZoneHID1 + 1) + para.centerZoneHID1;
							tov = to / (para.centerZoneHID2 - para.centerZoneHID1 + 1) + para.centerZoneVID1;
							ga_housePop[h][tov][toh]++;
							ga_moveNum[h]++;
						}
					}
				}
			}
			for (; fromv <= para.centerZoneVID2; fromv++) {
				for (fromh = 0; fromh < para.centerZoneHID1; fromh++) {
					for (i = 0; i < para.initialZonePopNum; i++) {
						if ((double)rand() / (double)RAND_MAX < ga_moveProb[h]) {
							ga_housePop[h][fromv][fromh]--;
							to = rand() % centerZoneNum;
							toh = to % (para.centerZoneHID2 - para.centerZoneHID1 + 1) + para.centerZoneHID1;
							tov = to / (para.centerZoneHID2 - para.centerZoneHID1 + 1) + para.centerZoneVID1;
							ga_housePop[h][tov][toh]++;
							ga_moveNum[h]++;
						}
					}
				}
				for (fromh = para.centerZoneHID2 + 1; fromh < para.zoneHNum; fromh++) {
					for (i = 0; i < para.initialZonePopNum; i++) {
						if ((double)rand() / (double)RAND_MAX < ga_moveProb[h]) {
							ga_housePop[h][fromv][fromh]--;
							to = rand() % centerZoneNum;
							toh = to % (para.centerZoneHID2 - para.centerZoneHID1 + 1) + para.centerZoneHID1;
							tov = to / (para.centerZoneHID2 - para.centerZoneHID1 + 1) + para.centerZoneVID1;
							ga_housePop[h][tov][toh]++;
							ga_moveNum[h]++;
						}
					}
				}

				for (; fromv < para.zoneVNum; fromv++) {
					for (fromh = 0; fromh < para.zoneHNum; fromh++) {
						for (i = 0; i < para.initialZonePopNum; i++) {
							if ((double)rand() / (double)RAND_MAX < ga_moveProb[h]) {
								ga_housePop[h][fromv][fromh]--;
								to = rand() % centerZoneNum;
								toh = to % (para.centerZoneHID2 - para.centerZoneHID1 + 1) + para.centerZoneHID1;
								tov = to / (para.centerZoneHID2 - para.centerZoneHID1 + 1) + para.centerZoneVID1;
								ga_housePop[h][tov][toh]++;
								ga_moveNum[h]++;
							}
						}
					}
				}

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				ga_fitness[h] = para.runningCost * ga_facilityNum[h] + ((double)ga_subsidy[h] / (double)para.subsidyLevelNum * para.subsidyMax) * ga_moveNum[h];
				//printf("ga_fitness[%d][%d]=%lf\n\n\n", g, h, ga_fitness[g][h]);

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				Ga_SubCalc(h);
			}
	
}

void Ga::Ga_SubCalc(int h)
{

	int i, j, k, x, y, neighbour, fromv, fromh, to, tov, toh;
	double oneDayPop;

	for (y = 0; y < para.zoneVNum; y++) 
	{
		for (x = 0; x < para.zoneHNum; x++) 
		{
			if (ga_housePop[h][y][x]) 
			{
				if (ga_facility[h][y][x]) 
				{
					oneDayPop = ga_housePop[h][y][x] * para.receiveRatio;
					ga_facilityPop[h][y][x] += oneDayPop;
					if (ga_facilityPop[h][y][x] > para.facilityCapacity) 
					{
						
						ga_fitness[h] = DBL_MAX;
						
						return;
					}
					else 
					{
						ga_fitness[h] += (para.transCostUnit) * para.transTimeUnit * oneDayPop * 0.365;
					}
				}
				else 
				{
					neighbour = 0;
					for (i = 1; i <= rep; i++) 
					{
						for (j = y - i, k = x; j < y; j++, k++)
							if ((0 <= j) && (k < para.zoneHNum) && ga_facility[h][j][k])
								neighbour++;
						for (; j < y + i; j++, k--)
							if ((j < para.zoneVNum) && (k < para.zoneHNum) && ga_facility[h][j][k])
								neighbour++;
						for (; j > y; j--, k--)
							if ((j < para.zoneVNum) && (0 <= k) && ga_facility[h][j][k])
								neighbour++;
						for (; j > y - i; j--, k++)
							if ((0 <= j) && (0 <= k) && ga_facility[h][j][k])
								neighbour++;
						if (neighbour) 
						{
							oneDayPop = ga_housePop[h][y][x] * para.receiveRatio / neighbour;
							for (j = y - i, k = x; j < y; j++, k++) 
							{
								if ((0 <= j) && (k < para.zoneHNum) && ga_facility[h][j][k]) 
								{
									ga_facilityPop[h][j][k] += oneDayPop;
									if (ga_facilityPop[h][j][k] > para.facilityCapacity) 
									{
										ga_fitness[h] = DBL_MAX;
										
										return;
									}
								}
							}
							for (; j < y + i; j++, k--) 
							{
								if ((j < para.zoneVNum) && (k < para.zoneHNum) && ga_facility[h][j][k]) 
								{
									ga_facilityPop[h][j][k] += oneDayPop;
									if (ga_facilityPop[h][j][k] > para.facilityCapacity) 
									{
										ga_fitness[h] = DBL_MAX;
										return;
									}
								}
							}
							for (; j > y; j--, k--) 
							{
								if ((j < para.zoneVNum) && (0 <= k) && ga_facility[h][j][k]) 
								{
									ga_facilityPop[h][j][k] += oneDayPop;
									if (ga_facilityPop[h][j][k] > para.facilityCapacity) {

										ga_fitness[h] = DBL_MAX;
										return;
									}
								}
							}
							for (; j > y - i; j--, k++) 
							{
								if ((0 <= j) && (0 <= k) && ga_facility[h][j][k]) 
								{
									ga_facilityPop[h][j][k] += oneDayPop;
									if (ga_facilityPop[h][j][k] > para.facilityCapacity) 
									{
										ga_fitness[h] = DBL_MAX;
										return;
									}
								}
							}
							ga_fitness[h] += i * (ga_housePop[h][y][x]) * fit;
							break;
						}
					}
					if (!neighbour) {
						ga_fitness[h] = DBL_MAX;
						return;
					}
				}
			}
		}
	}
}

void Ga::Ga_Sort(void)
{
	double temp_fitness;
	int    temp_subsidy;
	int    temp_moveNum;
	int    temp_facilityNum;
	int    temp_facility;
	int    temp_housePop;
	double temp_facilityPop;

	for (int i = 0; i < (INDIVIDUAL_NUM); ++i)
	{
		for (int j = i + 1; j < (INDIVIDUAL_NUM); ++j)
		{
			if (ga_fitness[i] > ga_fitness[j])
			{
				temp_fitness = ga_fitness[i];
				ga_fitness[i] = ga_fitness[j];
				ga_fitness[j] = temp_fitness;

				temp_subsidy = ga_subsidy[i];
				ga_subsidy[i] = ga_subsidy[j];
				ga_subsidy[j] = temp_subsidy;

				temp_moveNum = ga_moveNum[i];
				ga_moveNum[i] = ga_moveNum[j];
				ga_moveNum[j] = temp_moveNum;

				temp_facilityNum = ga_facilityNum[i];
				ga_facilityNum[i] = ga_facilityNum[j];
				ga_facilityNum[j] = temp_facilityNum;

				for (int q = 0; q < para.zoneVNum; q++)
				{
					for (int n = 0; n < para.zoneHNum; n++)
					{
						temp_facility = ga_facility[i][q][n];
						ga_facility[i][q][n] = ga_facility[j][q][n];
						ga_facility[j][q][n] = temp_facility;

						temp_housePop = ga_housePop[i][q][n];
						ga_housePop[i][q][n] = ga_housePop[j][q][n];
						ga_housePop[j][q][n] = temp_housePop;

						temp_facilityPop = ga_facilityPop[i][q][n];
						ga_facilityPop[i][q][n] = ga_facilityPop[j][q][n];
						ga_facilityPop[j][q][n] = temp_facilityPop;
					}
				}
			}
		}
	}
}

void Ga::Ga_Model(void)
{
	Ga_Sort();

	Ga_Mating();

	NowGene++;

	Ga_Sort();
}

void Ga::Ga_Mating(void)
{
	int pare1 = NULL;
	int pare2 = NULL;
	double r1, r2;
	int surviveRank[20] = { 10, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1 };

	double r_sub;

	int judge = 0;
	double r;
	double r_muta0;
	double r_muta1;

	for (int h = 1; h < (INDIVIDUAL_NUM); h++)
	{
		while ((pare2 == NULL))
		{

			for (int i = 0; i < 20; i++)
			{
				r1 = ((double)rand() / RAND_MAX) * 10;
				r2 = ((double)rand() / RAND_MAX) * 10;

				if ((surviveRank[i] > r1) && (pare1 == NULL))
				{
					pare1 = i;

				}

				if ((surviveRank[i] > r2) && (pare2 == NULL))
				{
					pare2 = i;
				}

				if (pare1 == pare2)
				{
					pare2 = NULL;
				}
			}

		}

		//printf("pare1 = %d, pare2 = %d\n", pare1, pare2);
		//////////////////////////////////////////////////

		r_sub = (double)rand() / RAND_MAX;
		if (r_sub >= 0.5)
		{
			ga_subsidy[h] = ga_subsidy[pare1];
		}

		else if (r_sub < 0.5)
		{
			ga_subsidy[h] = ga_subsidy[pare2];
		}
		//////////////////////////////////////////////////


		while (judge == 0)
		{
			for (int q = 0; q < para.zoneVNum; q++)
			{

				for (int n = 0; n < para.zoneHNum; n++)
				{
					r = (double)rand() / RAND_MAX;
					r_muta0 = (double)rand() / RAND_MAX;
					r_muta1 = (double)rand() / RAND_MAX;

					if (r > 0.5)
					{
						ga_facility[h][q][n] = ga_facility[pare1][q][n];
						ga_housePop[h][q][n] = para.initialZonePopNum;
						ga_facilityPop[h][q][n] = 0.0;
					}

					else
					{
						ga_facility[h][q][n] = ga_facility[pare2][q][n];
						ga_housePop[h][q][n] = para.initialZonePopNum;
						ga_facilityPop[h][q][n] = 0.0;
					}

					if (r_muta0 < 0.05)
					{
						if (ga_facility[h][q][n] == 1)
						{
							ga_facility[h][q][n] = 0;
							ga_housePop[h][q][n] = para.initialZonePopNum;
							ga_facilityPop[h][q][n] = 0.0;
						}
					}

					if (r_muta1 < 0.03)
					{
						if (ga_facility[h][q][n] == 0)
						{
							ga_facility[h][q][n] = 1;
							ga_housePop[h][q][n] = para.initialZonePopNum;
							ga_facilityPop[h][q][n] = 0.0;
						}
					}

				}
			}
			Ga_Calc(h);
			if (ga_fitness[h] < DBL_MAX)
			{
				judge = 1;
			}
		}
	}
	judge = 0;
	pare1 = NULL;
	pare2 = NULL;
}

void Ga::Ga_IslandModel(void)
{
	double imi;
	imi = (double)rand() / RAND_MAX;

	for (int i = 0; i < 4; i++)
	{
		Ga_IslandSort(i);
	}

	if ((NowGene>100)&&(imi < 0.1))
	{
		Ga_IslandImi();
	}

	for (int i = 0; i < 4; i++)
	{
		if (i == 0)
		{
			Ga_IslandMating(0);
		}
		else if (i == 1)
		{
			Ga_IslandMating((INDIVIDUAL_NUM / 4));
		}

		else if (i == 2)
		{
			Ga_IslandMating((INDIVIDUAL_NUM / 2));
		}

		else if (i == 3)
		{
			Ga_IslandMating((INDIVIDUAL_NUM / 4)+ (INDIVIDUAL_NUM / 2));
		}
	}
	NowGene++;

	
	for (int i = 0; i < 4; i++)
	{
		Ga_IslandSort(i);
	}
	
	
}

void Ga::Ga_IslandImi(void)
{
	int king[4] = {1,10,20,30};//移住するエリートを選択
	int selectisland[4] = { 0,10,20,30 };//移住先の変数
	int selectgene[4] = { 9,19,29,39 };//移住先寄生先の変数
	double select[4] = { 0.25,0.25,0.25,0.25 };
	int elete = NULL;//移住者の決定
	int island=NULL;//移住先の決定
	double is;

	//移住者の決定
	while (elete == NULL)
	{
		
		for (int i = 0; i <=3; i++)
		{
			if (elete == NULL)
			{
				is = (double)rand() / RAND_MAX;
				if ((is < select[i]) && (elete == NULL))
				{
					elete = king[i];
					//printf("elete=%d\n", elete);
				}
			}
			else
			{
				break;
			}
		}
	}
	if (elete == 1)
	{
		elete = 0;
	}
	//移住先の決定
	while (island == NULL)
	{
		for (int i = 0; i < 4; i++)
		{
			if (island == NULL)
			{
				is = (double)rand() / RAND_MAX;
				if ((is < select[i]) && (island == NULL))
				{
					if (elete != selectisland[i])
					{
						island = selectgene[i];
					}
				}
			}
			else
			{
				break;
			}
		}
	}
	
	//printf("elete=%d \nisland=%d\n", elete, island);

	//////////////////////////////////////////////////////////////////////////////////////////
	//遺伝子の移し替え（移住者の遺伝子を移住先の最も評価が悪い個体に移し替える）
	
	ga_fitness[island] = ga_fitness[elete];
	ga_subsidy[island] = ga_subsidy[elete];
	ga_moveNum[island] = ga_moveNum[elete];
	ga_facilityNum[island] = ga_facilityNum[elete];
	

	for (int q = 0; q < para.zoneVNum; q++)
	{
		for (int n = 0; n < para.zoneHNum; n++)
		{
			
			ga_facility[island][q][n] = ga_facility[elete][q][n];
			ga_housePop[island][q][n] = ga_housePop[elete][q][n];
			ga_facilityPop[island][q][n] = ga_facilityPop[elete][q][n];
			
		}
	}

}

void Ga::Ga_IslandMating(int island)
{
	int pare1 = NULL;
	int pare2 = NULL;
	double r1, r2;
	int surviveRank[(INDIVIDUAL_NUM / 4)] = {0,8,8,6,4,3,2,2,1,1};

	double r_sub;

	int judge=0;
	double r;
	double r_muta0;
	double r_muta1;


	//親選択,,,MGGモデルによって最良解とランキング選択よって決められたもう一つの解のみでその世代の交配を行う
	pare1 = 0;
	while ((pare2 == NULL))
	{

		for (int i = 0; i < (INDIVIDUAL_NUM / 4); i++)
		{
			//r1 = ((double)rand() / RAND_MAX) * 10;
			r2 = ((double)rand() / RAND_MAX) * 10;
			/*
			if ((surviveRank[i] > r1) && (pare1 == NULL))
			{
				pare1 = i;

			}
			*/
			if ((surviveRank[i] > r2) && (pare2 == NULL))
			{
				pare2 = i;
			}
			/*
			if (pare1 == pare2)
			{
				pare2 = NULL;
			}
			*/
		}

	}



	for (int h = island+1; h < (INDIVIDUAL_NUM / 4) + island; h++)
	{

		r_sub = (double)rand() / RAND_MAX;
		if (r_sub >= 0.5)
		{
			ga_subsidy[h] = ga_subsidy[island];
		}

		else if (r_sub < 0.5)
		{
			ga_subsidy[h] = ga_subsidy[pare2 + island];
		}
		//////////////////////////////////////////////////

		
		while (judge == 0)
		{
			for (int q = 0; q < para.zoneVNum; q++)
			{

				for (int n = 0; n < para.zoneHNum; n++)
				{
					r = (double)rand() / RAND_MAX;
					r_muta0 = (double)rand() / RAND_MAX;
					r_muta1 = (double)rand() / RAND_MAX;

					if (r > 0.5)
					{
						ga_facility[h][q][n] = ga_facility[island][q][n];
						ga_housePop[h][q][n] = para.initialZonePopNum;
						ga_facilityPop[h][q][n] = 0.0;
					}

					else
					{
						ga_facility[h][q][n] = ga_facility[pare2+island][q][n];
						ga_housePop[h][q][n] = para.initialZonePopNum;
						ga_facilityPop[h][q][n] = 0.0;
					}
					////////////////////////////////////////////////////////////////////////////////////////////////////
					if (island == 0)
					{
						if (r_muta0 < 0.05)
						{
							if (ga_facility[h][q][n] == 1)
							{
								ga_facility[h][q][n] = 0;
								ga_housePop[h][q][n] = para.initialZonePopNum;
								ga_facilityPop[h][q][n] = 0.0;
							}
						}

						
							if (r_muta1 < 0.01)
							{
								if (ga_facility[h][q][n] == 0)
								{

									ga_facility[h][q][n] = 1;
									ga_housePop[h][q][n] = para.initialZonePopNum;
									ga_facilityPop[h][q][n] = 0.0;
								}
							}
						
						
					}
					if (island == (INDIVIDUAL_NUM / 4))
					{
						if (r_muta0 < 0.04)
						{
							if (ga_facility[h][q][n] == 1)
							{
								ga_facility[h][q][n] = 0;
								ga_housePop[h][q][n] = para.initialZonePopNum;
								ga_facilityPop[h][q][n] = 0.0;
							}
						}

						if (r_muta1 < 0.01)
						{
							if (ga_facility[h][q][n] == 0)
							{
								ga_facility[h][q][n] = 1;
								ga_housePop[h][q][n] = para.initialZonePopNum;
								ga_facilityPop[h][q][n] = 0.0;
							}
						}
					}
					if (island == (INDIVIDUAL_NUM / 2))
					{
						if (r_muta0 < 0.04)
						{
							if (ga_facility[h][q][n] == 1)
							{
								ga_facility[h][q][n] = 0;
								ga_housePop[h][q][n] = para.initialZonePopNum;
								ga_facilityPop[h][q][n] = 0.0;
							}
						}

						if (r_muta1 < 0.02)
						{
							if (ga_facility[h][q][n] == 0)
							{
								ga_facility[h][q][n] = 1;
								ga_housePop[h][q][n] = para.initialZonePopNum;
								ga_facilityPop[h][q][n] = 0.0;
							}
						}
					}
					if (island == (INDIVIDUAL_NUM / 4)+ (INDIVIDUAL_NUM / 2))
					{
						if (r_muta0 < 0.08)
						{
							if (ga_facility[h][q][n] == 1)
							{
								ga_facility[h][q][n] = 0;
								ga_housePop[h][q][n] = para.initialZonePopNum;
								ga_facilityPop[h][q][n] = 0.0;
							}
						}

						if (r_muta1 < 0.03)
						{
							if (ga_facility[h][q][n] == 0)
							{
								ga_facility[h][q][n] = 1;
								ga_housePop[h][q][n] = para.initialZonePopNum;
								ga_facilityPop[h][q][n] = 0.0;
							}
						}
					}
					////////////////////////////////////////////////////////////////////////////////////////////////////

				}
			}

			Ga_Calc(h);

			if (ga_fitness[h] < DBL_MAX)
			{
				judge = 1;
			}
		}

		judge = 0;
	}

	pare1 = NULL;
	pare2 = NULL;
}

void Ga::Ga_IslandSort(int island) 
{
	double temp_fitness;
	int    temp_subsidy;
	int    temp_moveNum;
	int    temp_facilityNum;
	int    temp_facility;
	int    temp_housePop;
	double temp_facilityPop;

	if (island == 0)
	{
		for (int i = 0; i < (INDIVIDUAL_NUM/4); ++i)
		{
			for (int j = i + 1; j < (INDIVIDUAL_NUM/4); ++j)
			{
				if (ga_fitness[i] > ga_fitness[j])
				{
					temp_fitness = ga_fitness[i];
					ga_fitness[i] = ga_fitness[j];
					ga_fitness[j] = temp_fitness;

					temp_subsidy = ga_subsidy[i];
					ga_subsidy[i] = ga_subsidy[j];
					ga_subsidy[j] = temp_subsidy;

					temp_moveNum = ga_moveNum[i];
					ga_moveNum[i] = ga_moveNum[j];
					ga_moveNum[j] = temp_moveNum;

					temp_facilityNum = ga_facilityNum[i];
					ga_facilityNum[i] = ga_facilityNum[j];
					ga_facilityNum[j] = temp_facilityNum;

					for (int q = 0; q < para.zoneVNum; q++)
					{
						for (int n = 0; n < para.zoneHNum; n++)
						{
							temp_facility = ga_facility[i][q][n];
							ga_facility[i][q][n] = ga_facility[j][q][n];
							ga_facility[j][q][n] = temp_facility;

							temp_housePop = ga_housePop[i][q][n];
							ga_housePop[i][q][n] = ga_housePop[j][q][n];
							ga_housePop[j][q][n] = temp_housePop;

							temp_facilityPop = ga_facilityPop[i][q][n];
							ga_facilityPop[i][q][n] = ga_facilityPop[j][q][n];
							ga_facilityPop[j][q][n] = temp_facilityPop;
						}
					}
				}
			}
		}
	}

	else if (island == 1)
	{
		for (int i = (INDIVIDUAL_NUM / 4); i < (INDIVIDUAL_NUM / 2); ++i)
		{
			for (int j = i + 1; j < (INDIVIDUAL_NUM / 2); ++j)
			{
				if (ga_fitness[i] > ga_fitness[j])
				{
					temp_fitness = ga_fitness[i];
					ga_fitness[i] = ga_fitness[j];
					ga_fitness[j] = temp_fitness;

					temp_subsidy = ga_subsidy[i];
					ga_subsidy[i] = ga_subsidy[j];
					ga_subsidy[j] = temp_subsidy;

					temp_moveNum = ga_moveNum[i];
					ga_moveNum[i] = ga_moveNum[j];
					ga_moveNum[j] = temp_moveNum;

					temp_facilityNum = ga_facilityNum[i];
					ga_facilityNum[i] = ga_facilityNum[j];
					ga_facilityNum[j] = temp_facilityNum;

					for (int q = 0; q < para.zoneVNum; q++)
					{
						for (int n = 0; n < para.zoneHNum; n++)
						{
							temp_facility = ga_facility[i][q][n];
							ga_facility[i][q][n] = ga_facility[j][q][n];
							ga_facility[j][q][n] = temp_facility;

							temp_housePop = ga_housePop[i][q][n];
							ga_housePop[i][q][n] = ga_housePop[j][q][n];
							ga_housePop[j][q][n] = temp_housePop;

							temp_facilityPop = ga_facilityPop[i][q][n];
							ga_facilityPop[i][q][n] = ga_facilityPop[j][q][n];
							ga_facilityPop[j][q][n] = temp_facilityPop;
						}
					}
				}
			}
		}
	}

	else if (island == 2)
	{
		for (int i = (INDIVIDUAL_NUM / 2); i < ((INDIVIDUAL_NUM / 4)+ (INDIVIDUAL_NUM / 2)); ++i)
		{
			for (int j = i + 1; j < ((INDIVIDUAL_NUM / 4) + (INDIVIDUAL_NUM / 2)); ++j)
			{
				if (ga_fitness[i] > ga_fitness[j])
				{
					temp_fitness = ga_fitness[i];
					ga_fitness[i] = ga_fitness[j];
					ga_fitness[j] = temp_fitness;

					temp_subsidy = ga_subsidy[i];
					ga_subsidy[i] = ga_subsidy[j];
					ga_subsidy[j] = temp_subsidy;

					temp_moveNum = ga_moveNum[i];
					ga_moveNum[i] = ga_moveNum[j];
					ga_moveNum[j] = temp_moveNum;

					temp_facilityNum = ga_facilityNum[i];
					ga_facilityNum[i] = ga_facilityNum[j];
					ga_facilityNum[j] = temp_facilityNum;

					for (int q = 0; q < para.zoneVNum; q++)
					{
						for (int n = 0; n < para.zoneHNum; n++)
						{
							temp_facility = ga_facility[i][q][n];
							ga_facility[i][q][n] = ga_facility[j][q][n];
							ga_facility[j][q][n] = temp_facility;

							temp_housePop = ga_housePop[i][q][n];
							ga_housePop[i][q][n] = ga_housePop[j][q][n];
							ga_housePop[j][q][n] = temp_housePop;

							temp_facilityPop = ga_facilityPop[i][q][n];
							ga_facilityPop[i][q][n] = ga_facilityPop[j][q][n];
							ga_facilityPop[j][q][n] = temp_facilityPop;
						}
					}
				}
			}
		}
	}

	else if (island == 3)
	{
	for (int i = (INDIVIDUAL_NUM / 4) + (INDIVIDUAL_NUM / 2); i < (INDIVIDUAL_NUM); ++i)
	{
		for (int j = i + 1; j < (INDIVIDUAL_NUM); ++j)
		{
			if (ga_fitness[i] > ga_fitness[j])
			{
				temp_fitness = ga_fitness[i];
				ga_fitness[i] = ga_fitness[j];
				ga_fitness[j] = temp_fitness;

				temp_subsidy = ga_subsidy[i];
				ga_subsidy[i] = ga_subsidy[j];
				ga_subsidy[j] = temp_subsidy;

				temp_moveNum = ga_moveNum[i];
				ga_moveNum[i] = ga_moveNum[j];
				ga_moveNum[j] = temp_moveNum;

				temp_facilityNum = ga_facilityNum[i];
				ga_facilityNum[i] = ga_facilityNum[j];
				ga_facilityNum[j] = temp_facilityNum;

				for (int q = 0; q < para.zoneVNum; q++)
				{
					for (int n = 0; n < para.zoneHNum; n++)
					{
						temp_facility = ga_facility[i][q][n];
						ga_facility[i][q][n] = ga_facility[j][q][n];
						ga_facility[j][q][n] = temp_facility;

						temp_housePop = ga_housePop[i][q][n];
						ga_housePop[i][q][n] = ga_housePop[j][q][n];
						ga_housePop[j][q][n] = temp_housePop;

						temp_facilityPop = ga_facilityPop[i][q][n];
						ga_facilityPop[i][q][n] = ga_facilityPop[j][q][n];
						ga_facilityPop[j][q][n] = temp_facilityPop;
					}
				}
			}
		}
	}
	}
}

void Ga::Ga_LastPrint()
{
	FILE* resultFP;
	double subsidyOne;
	int i, j;


	printf("最適解 : %lf \n",best_fitness);
	printf("補助金 : %d \n", best_subsidy);

	for (int i = 0; i < para.zoneVNum; i++)
	{
		for (int j = 0; j < para.zoneHNum; j++)
		{
			printf("%d", best_facility[i][j]);
			
		}
		printf("\n");//ga_facility用の区切り
	}
	printf("\n");

	// ファイルオープン
	if ((resultFP = fopen("result.csv", "w")) == NULL) 
	{
		printf("Cannot open result.txt for output.\n");
		exit(1);
	}
	// 書込み
	subsidyOne = (double)ga_subsidy[0] / (double)para.subsidyLevelNum * para.subsidyMax;
	fprintf(resultFP, "%.0lf\n", subsidyOne);
	for (j = 0; j < para.zoneVNum; j++) 
	{
		for (i = 0; i < para.zoneHNum; i++)
		{
			fprintf(resultFP, "%d,", best_facility[j][i]);
		}
		fprintf(resultFP, "\n");
	}

	// ファイルクローズ
	fclose(resultFP);
}

void Ga::Ga_Reset()
{
	NowGene = 0;
}

void Ga::Ga_BestofGroup()
{
	

	best_fitness = DBL_MAX;
	for (int p = 0; p < WorldNum; p++)
	{
		
		Ga_Init();

		if (p >= 1)
		{
			ga_fitness[0] = best_fitness;
			ga_subsidy[0] = best_subsidy;

			for (int q = 0; q < para.zoneVNum; q++)
			{
				for (int n = 0; n < para.zoneHNum; n++)
				{
					ga_facility[0][q][n] = best_facility[q][n] ;
					ga_housePop[0][q][n] = best_housePop[q][n];
					ga_facilityPop[0][q][n] = best_facilityPop[q][n];
				}
			}
		}

		Ga_PareCalc();
		for (int i = 0; i < GENERATION_NUM; i++)
		{
			Ga_IslandModel();
			//g.Ga_Print();
		}
		Ga_Sort();
		Ga_Reset();
		
		if (best_fitness > ga_fitness[0])
		{
			best_fitness = ga_fitness[0];
			best_subsidy = ga_subsidy[0];

			for (int q = 0; q < para.zoneVNum; q++)
			{
				for (int n = 0; n < para.zoneHNum; n++)
				{

					best_facility[q][n] = ga_facility[0][q][n];
					best_housePop[q][n] = ga_housePop[0][q][n];
					best_facilityPop[q][n] = ga_facilityPop[0][q][n];

				}
			}
		}
		
		//printf("best=%lf\n", best_fitness);
	}
	
}