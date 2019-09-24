#include "Para.h"
// コンストラクタ
Para::Para()
{
	char* pos1, * pos2;
	char fName[256], line[1024], paraName[32];
	int paraValue;
	FILE* paraFP;

	strcpy(fName, "para.csv");
	if ((paraFP = fopen(fName, "r")) == NULL) {
		printf("Cannot open %s for input.\n", fName);
		exit(1);
	}
	while (fgets(line, 1024, paraFP)) {
		pos1 = line;
		pos2 = strchr(pos1, ',');
		*pos2 = '\0';
		strcpy(paraName, line);
		pos1 = pos2 + 1;
		pos2 = strchr(pos1, ',');
		if (!pos2)
			pos2 = strchr(pos1, '\n');
		*pos2 = '\0';
		if (!strcmp(paraName, "receiveRatio")) {
			receiveRatio = atof(pos1);
		}
		else {
			paraValue = atoi(pos1);
			if (!strcmp(paraName, "zoneHNum"))
				zoneHNum = paraValue;
			else if (!strcmp(paraName, "zoneVNum"))
				zoneVNum = paraValue;
			else if (!strcmp(paraName, "centerZoneHID1"))
				centerZoneHID1 = paraValue;
			else if (!strcmp(paraName, "centerZoneHID2"))
				centerZoneHID2 = paraValue;
			else if (!strcmp(paraName, "centerZoneVID1"))
				centerZoneVID1 = paraValue;
			else if (!strcmp(paraName, "centerZoneVID2"))
				centerZoneVID2 = paraValue;
			else if (!strcmp(paraName, "initialZonePopNum"))
				initialZonePopNum = paraValue;
			else if (!strcmp(paraName, "transTimeUnit"))
				transTimeUnit = paraValue;
			else if (!strcmp(paraName, "transTimeLimit"))
				transTimeLimit = paraValue;
			else if (!strcmp(paraName, "facilityCapacity"))
				facilityCapacity = paraValue;
			else if (!strcmp(paraName, "runningCost"))
				runningCost = paraValue;
			else if (!strcmp(paraName, "subsidyMax"))
				subsidyMax = paraValue;
			else if (!strcmp(paraName, "transCostUnit"))
				transCostUnit = paraValue;
			else if (!strcmp(paraName, "subsidyLevelNum"))
				subsidyLevelNum = paraValue;
		}
	}
	fclose(paraFP);

}

// デストラクタ
Para::~Para()
{
}
