#include "Ga.h"
#include <time.h>

using namespace std;
double sec;//���̎��s�ŉ��b�����v��

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));

	Ga g;
	g.~Ga();
	
	for (int p = 0; p < 1; p++)//�v����
	{
		clock_t start = clock();//���Ԃ̌v���͂���
		printf("*****�v����*****\n");
		g.Ga_BestofGroup();
		clock_t end = clock();//���Ԍv���I��
		sec = (double)(end - start) / CLOCKS_PER_SEC;//�b���ɕϊ�
		printf("��������=%lf\n", sec);
		g.Ga_LastPrint();

		g.Ga_Reset();
	}
	
		return 0;
	
}