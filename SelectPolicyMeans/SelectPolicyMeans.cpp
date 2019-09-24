#include "Ga.h"
#include <time.h>

using namespace std;
double sec;//一回の実行で何秒かを計測

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));

	Ga g;
	g.~Ga();
	
	for (int p = 0; p < 1; p++)//計測回数
	{
		clock_t start = clock();//時間の計測はじめ
		printf("*****計測中*****\n");
		g.Ga_BestofGroup();
		clock_t end = clock();//時間計測終了
		sec = (double)(end - start) / CLOCKS_PER_SEC;//秒数に変換
		printf("処理時間=%lf\n", sec);
		g.Ga_LastPrint();

		g.Ga_Reset();
	}
	
		return 0;
	
}