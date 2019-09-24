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
	void Ga_Mating();//�g��Ȃ�
	void Ga_Model();//�g��Ȃ�
	void Ga_SampleInit();//�z�z�����̎{�ݔz�u*�g��Ȃ�
	///////////////////////////////////////////

	//////////////////
	//�����f������
	//////////////////
	void Ga_IslandModel();
	void Ga_IslandMating(int island);
	void Ga_IslandSort(int island);
	void Ga_IslandImi(void);
	void Ga_Reset(void);
	void Ga_BestofGroup(void);
	///////////////////

	//�ł��ǂ��œK����\��
	///////////////////
	void Ga_LastPrint();
};

