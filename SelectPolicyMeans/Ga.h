#pragma once
class Para;
#include "Para.h"


class Ga
{
public:
	
	int centerZoneNum;
	int surroundingZoneNum;
	int rep;
	double fit;

	Ga();
	~Ga();
	void Ga_Init();
	void Ga_Print();
	void Ga_Calc(int h);
	void Ga_SubCalc(int h);
	void Ga_PareCalc();
	void Ga_ReGenerate(int h);
	void Ga_Sort();

	///////////////////////////////////////////
	void Ga_Mating();//使わない
	void Ga_Model();//使わない
	void Ga_SampleInit();//配布資料の施設配置*使わない
	///////////////////////////////////////////

	//////////////////
	//島モデル実装
	//////////////////
	void Ga_IslandModel();
	void Ga_IslandMating(int island);
	void Ga_IslandSort(int island);
	void Ga_IslandImi(void);
	void Ga_Reset(void);
	void Ga_BestofGroup(void);
	///////////////////

	//最も良い最適解を表示
	///////////////////
	void Ga_LastPrint();
};

