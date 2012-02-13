#include <stdlib.h>
#include <stdio.h>

//
// PERFORMANCE_MAX
//

#define WIDTH 24
#define HEIGHT 24

#define PITCH 24
#define KERNEL_SIZE 8

// Das Eingabearray muss als 24x24 Short-Array unverändert bleiben.
int signed input_array[HEIGHT][WIDTH] =
{
	{-639040, -507968, -262144, 1151616, 1151552, 16384, 16448, 1179648,
	1179648, 0, -589888, 1097728, -131072, 98624, 98752, 131392,
	98752, 78400, -131584, 197184, 197824, 180992, -197312, 280064},
	
	{230528, 459968, 476672, 444224, -443904, 509568, 723136, 510272,
	690240, 428096, 624320, 642304, 756352, 805824, 806016, 608128,
	953728, -789888, -724608, 805888, -418048, -12288, 131776, 1605632},
	
	{1343488, 16448, 1088, -966720, 458752, 0, 32896, 16576,
	64, 0, 0, 0, 0, 81920, 384, 16640,
	114880, 131200, -966720, 901120, 1024, 32896, 128, 82176},
	
	{49280, -82304, -320, 0, 16384, 49280, 82176, 1088,
	541824, 101440, 328896, 1067136, 1328448, 1854592, 122944, 1083584,
	1594496, 843904, 1363200, -1874048, -1817216, -1547072, -3008, 13952},
	
	{16896, 64, 1032192, -391552, 1294080, -1126656, 1747712, -649664,
	-1439104, 603712, 31232, -492096, -534656, 1696448, 208576, -1001024,
	532992, 1451200, -891840, 585280, 1400384, 4736, 1312256, -1648768},
	
	{1047168, 286720, -491648, 1241088, -531328, 1602048, -1590848, 16320,
	-1334784, -1725632, 1625856, 1435264, -1560128, 305856, 1354304, -405504,
	-443776, -1548096, -1966592, -952000, -1730816, 248256, 415680, -1858496},
	
	{1577728, 1879488, 831360, -833984, 829120, -815360, 636288, 1145216,
	-489600, 1384320, -669760, -1711296, -644096, 872064, -1225600, 1579584,
	846400, 1487936, 1768704, 267008, -1662528, -1902336, -1267648, -602816},
	
	{829312, 2006912, 372160, -1138944, -1655296, -1997376, 675264, -1890688,
	997504, 2076288, -1255424, -1345088, 618176, -290368, 619840, 96640,
	-1675712, -683968, -1200512, 257920, -130176, -1139392, -1137152, -1742144},
	
	{681088, 134080, 793152, 1713664, -189760, 558400, 1251264, 2045120,
	-485696, 924736, -1758528, -708096, 1323328, -153984, 1659008, 1105536,
	849216, 2014080, -312640, 475136, 643648, 13312, -1788672, -507904},
	
	{-686976, -339776, -1335424, -1933888, -791872, 967552, 1673152, 2032896,
	1707712, -1516416, -432832, 1633344, -1086528, -582400, -2033920, -1132928,
	-422208, -342016, 564032, -1116672, 660736, 1027648, 644992, -128768},
	
	{1553792, 55424, -1305280, -990976, -644032, -1689088, -68160, 5056,
	-698752, 1446528, -175488, 1452800, 906304, 1831936, 366784, 1420736,
	-564160, 1378624, 169280, -979584, 965888, -1624064, 1783168, -878912},
	
	{-1971712, 111616, 1419712, -1938240, -1894592, 1858944, 1562560, -57408,
	1301440, -677696, -1666176, 1603008, 577664, 1227904, -1775936, 1278912,
	113984, -2059008, -832128, 159744, 1778752, 1123968, 1506816, -716608},
	
	{-1948416, 213696, 566080, -340928, -1560704, 1934144, 128448, 740352,
	1483712, 1653824, -1173056, -2038848, -1380288, 1494144, -221376, -1198400,
	833216, -35904, 1449216, -333184, 1695424, 1940288, 837376, 1703424},
	
	{-176320, -1200832, -742272, -2021760, -1477312, 1573376, -1841408, -1815360,
	828672, 52800, -197632, -606016, -758272, -1441344, -788480, -1899328,
	-1555264, 946048, -835776, -1153280, -1621248, 303040, -1960192, -503104},
	
	{1876608, 1076352, -382080, -1083584, 482368, 26176, 1341824, -1556736,
	-1573312, -1907328, -1868416, 629312, -467200, -743168, -2083776, -887360,
	-1233536, -234048, 306304, -1047168, -1474368, -1772544, 977920, -254080},
	
	{-982080, 159488, -432320, 1082432, -1114560, 1426624, 692800, 1207040,
	38528, -555904, 954752, 392320, -990336, -785024, -1561728, 728384,
	504064, -659648, 1026048, -820608, 151296, -746048, -476032, -857408},
	
	{-2061248, -1308160, 185408, 1770880, -437696, -1319360, -1783552, 617984,
	-1986432, -1752832, 640704, 646784, 2021568, -652288, -794368, 1751360,
	-1233728, -1919872, -463616, 1236672, 580544, -894976, -864576, 1654784},
	
	{1351552, -493632, 606592, 729792, -1050816, -755392, -1840576, 1323456,
	1073408, 1348352, -1241600, -1415424, -1241344, -902976, -1822656, 959872,
	-2051776, 1680832, -1293184, -874496, -342528, -654144, 1170048, -743296},
	
	{-1190784, -875136, 1894016, 988096, 1697216, 1728384, 883776, 699200,
	304192, 1149888, 1044160, 1312768, 134912, -1142272, -606080, 1491392,
	1461952, 210944, 860928, 1265408, 1385216, -724480, -168512, 2008832},
	
	{-1105216, 1270528, 1484736, -1634752, -528640, -1097024, 666752, -307584,
	862208, 1920640, 648064, -2065088, 1667520, 372160, 2027456, -944960,
	895936, -1665472, -1687104, 102912, 21504, -1661568, 1298432, 1552896},
	
	{-40960, 1848832, 1147008, 1485888, 2006784, -1392192, -92544, 708864,
	-313152, -740928, 1417600, 1902400, -2088896, 1160704, -1308480, -1508672,
	1242432, -868224, -184192, 1600896, 696448, 79424, 747968, 532992},
	
	{-168960, 928768, -1758656, 113344, -1318720, 158720, -1145920, -1686016,
	660416, -1279168, -1608704, -962112, 1388288, 1498304, 533696, -902144,
	-432576, 1188224, 2046336, 51904, 2048704, 1907008, -1929280, 1609152},
	
	{354624, 545728, -1465664, 581696, -170752, 374976, -1189504, -879744,
	750272, -1483008, 632320, -1790720, 263872, 1782848, -1762048, 694656,
	1895488, -1777280, -910848, 1077440, 958528, 1554368, -1083584, -747264},
	
	{-917952, -1837120, 854336, 1406912, -1954880, 1513216, -677504, -734976,
	-1608128, -55744, -2086080, 610368, 1000576, 777152, -933760, 452608,
	-706368, -1067968, 708480, -1382912, -1121280, -1104640, -298496, -1353792}
};

inline void printArray(int* chunk) {
	int i, j;
	for (i=0; i<8; i++) {
		for (j=0; j<8; j++) {
			printf("%d\t", chunk[i*8+j]);
		}
		printf("\n");
	}
	printf("\n");
}

inline void printArray2d() {
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[0][0], input_array[0][1], input_array[0][2], input_array[0][3], input_array[0][4], input_array[0][5], input_array[0][6], input_array[0][7], input_array[0][8], input_array[0][9], input_array[0][10], input_array[0][11], input_array[0][12], input_array[0][13], input_array[0][14], input_array[0][15], input_array[0][16], input_array[0][17], input_array[0][18], input_array[0][19], input_array[0][20], input_array[0][21], input_array[0][22], input_array[0][23]); 
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[1][0], input_array[1][1], input_array[1][2], input_array[1][3], input_array[1][4], input_array[1][5], input_array[1][6], input_array[1][7], input_array[1][8], input_array[1][9], input_array[1][10], input_array[1][11], input_array[1][12], input_array[1][13], input_array[1][14], input_array[1][15], input_array[1][16], input_array[1][17], input_array[1][18], input_array[1][19], input_array[1][20], input_array[1][21], input_array[1][22], input_array[1][23]); 	
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[2][0], input_array[2][1], input_array[2][2], input_array[2][3], input_array[2][4], input_array[2][5], input_array[2][6], input_array[2][7], input_array[2][8], input_array[2][9], input_array[2][10], input_array[2][11], input_array[2][12], input_array[2][13], input_array[2][14], input_array[2][15], input_array[2][16], input_array[2][17], input_array[2][18], input_array[2][19], input_array[2][20], input_array[2][21], input_array[2][22], input_array[2][23]);
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[3][0], input_array[3][1], input_array[3][2], input_array[3][3], input_array[3][4], input_array[3][5], input_array[3][6], input_array[3][7], input_array[3][8], input_array[3][9], input_array[3][10], input_array[3][11], input_array[3][12], input_array[3][13], input_array[3][14], input_array[3][15], input_array[3][16], input_array[3][17], input_array[3][18], input_array[3][19], input_array[3][20], input_array[3][21], input_array[3][22], input_array[3][23]);
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[4][0], input_array[4][1], input_array[4][2], input_array[4][3], input_array[4][4], input_array[4][5], input_array[4][6], input_array[4][7], input_array[4][8], input_array[4][9], input_array[4][10], input_array[4][11], input_array[4][12], input_array[4][13], input_array[4][14], input_array[4][15], input_array[4][16], input_array[4][17], input_array[4][18], input_array[4][19], input_array[4][20], input_array[4][21], input_array[4][22], input_array[4][23]);
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[5][0], input_array[5][1], input_array[5][2], input_array[5][3], input_array[5][4], input_array[5][5], input_array[5][6], input_array[5][7], input_array[5][8], input_array[5][9], input_array[5][10], input_array[5][11], input_array[5][12], input_array[5][13], input_array[5][14], input_array[5][15], input_array[5][16], input_array[5][17], input_array[5][18], input_array[5][19], input_array[5][20], input_array[5][21], input_array[5][22], input_array[5][23]);
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[6][0], input_array[6][1], input_array[6][2], input_array[6][3], input_array[6][4], input_array[6][5], input_array[6][6], input_array[6][7], input_array[6][8], input_array[6][9], input_array[6][10], input_array[6][11], input_array[6][12], input_array[6][13], input_array[6][14], input_array[6][15], input_array[6][16], input_array[6][17], input_array[6][18], input_array[6][19], input_array[6][20], input_array[6][21], input_array[6][22], input_array[6][23]);
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[7][0], input_array[7][1], input_array[7][2], input_array[7][3], input_array[7][4], input_array[7][5], input_array[7][6], input_array[7][7], input_array[7][8], input_array[7][9], input_array[7][10], input_array[7][11], input_array[7][12], input_array[7][13], input_array[7][14], input_array[7][15], input_array[7][16], input_array[7][17], input_array[7][18], input_array[7][19], input_array[7][20], input_array[7][21], input_array[7][22], input_array[7][23]);
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[8][0], input_array[8][1], input_array[8][2], input_array[8][3], input_array[8][4], input_array[8][5], input_array[8][6], input_array[8][7], input_array[8][8], input_array[8][9], input_array[8][10], input_array[8][11], input_array[8][12], input_array[8][13], input_array[8][14], input_array[8][15], input_array[8][16], input_array[8][17], input_array[8][18], input_array[8][19], input_array[8][20], input_array[8][21], input_array[8][22], input_array[8][23]);
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[9][0], input_array[9][1], input_array[9][2], input_array[9][3], input_array[9][4], input_array[9][5], input_array[9][6], input_array[9][7], input_array[9][8], input_array[9][9], input_array[9][10], input_array[9][11], input_array[9][12], input_array[9][13], input_array[9][14], input_array[9][15], input_array[9][16], input_array[9][17], input_array[9][18], input_array[9][19], input_array[9][20], input_array[9][21], input_array[9][22], input_array[9][23]);
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[10][0], input_array[10][1], input_array[10][2], input_array[10][3], input_array[10][4], input_array[10][5], input_array[10][6], input_array[10][7], input_array[10][8], input_array[10][9], input_array[10][10], input_array[10][11], input_array[10][12], input_array[10][13], input_array[10][14], input_array[10][15], input_array[10][16], input_array[10][17], input_array[10][18], input_array[10][19], input_array[10][20], input_array[10][21], input_array[10][22], input_array[10][23]);
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[11][0], input_array[11][1], input_array[11][2], input_array[11][3], input_array[11][4], input_array[11][5], input_array[11][6], input_array[11][7], input_array[11][8], input_array[11][9], input_array[11][10], input_array[11][11], input_array[11][12], input_array[11][13], input_array[11][14], input_array[11][15], input_array[11][16], input_array[11][17], input_array[11][18], input_array[11][19], input_array[11][20], input_array[11][21], input_array[11][22], input_array[11][23]);
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[12][0], input_array[12][1], input_array[12][2], input_array[12][3], input_array[12][4], input_array[12][5], input_array[12][6], input_array[12][7], input_array[12][8], input_array[12][9], input_array[12][10], input_array[12][11], input_array[12][12], input_array[12][13], input_array[12][14], input_array[12][15], input_array[12][16], input_array[12][17], input_array[12][18], input_array[12][19], input_array[12][20], input_array[12][21], input_array[12][22], input_array[12][23]);
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[13][0], input_array[13][1], input_array[13][2], input_array[13][3], input_array[13][4], input_array[13][5], input_array[13][6], input_array[13][7], input_array[13][8], input_array[13][9], input_array[13][10], input_array[13][11], input_array[13][12], input_array[13][13], input_array[13][14], input_array[13][15], input_array[13][16], input_array[13][17], input_array[13][18], input_array[13][19], input_array[13][20], input_array[13][21], input_array[13][22], input_array[13][23]);
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[14][0], input_array[14][1], input_array[14][2], input_array[14][3], input_array[14][4], input_array[14][5], input_array[14][6], input_array[14][7], input_array[14][8], input_array[14][9], input_array[14][10], input_array[14][11], input_array[14][12], input_array[14][13], input_array[14][14], input_array[14][15], input_array[14][16], input_array[14][17], input_array[14][18], input_array[14][19], input_array[14][20], input_array[14][21], input_array[14][22], input_array[14][23]);
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[15][0], input_array[15][1], input_array[15][2], input_array[15][3], input_array[15][4], input_array[15][5], input_array[15][6], input_array[15][7], input_array[15][8], input_array[15][9], input_array[15][10], input_array[15][11], input_array[15][12], input_array[15][13], input_array[15][14], input_array[15][15], input_array[15][16], input_array[15][17], input_array[15][18], input_array[15][19], input_array[15][20], input_array[15][21], input_array[15][22], input_array[15][23]);
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[16][0], input_array[16][1], input_array[16][2], input_array[16][3], input_array[16][4], input_array[16][5], input_array[16][6], input_array[16][7], input_array[16][8], input_array[16][9], input_array[16][10], input_array[16][11], input_array[16][12], input_array[16][13], input_array[16][14], input_array[16][15], input_array[16][16], input_array[16][17], input_array[16][18], input_array[16][19], input_array[16][20], input_array[16][21], input_array[16][22], input_array[16][23]);
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[17][0], input_array[17][1], input_array[17][2], input_array[17][3], input_array[17][4], input_array[17][5], input_array[17][6], input_array[17][7], input_array[17][8], input_array[17][9], input_array[17][10], input_array[17][11], input_array[17][12], input_array[17][13], input_array[17][14], input_array[17][15], input_array[17][16], input_array[17][17], input_array[17][18], input_array[17][19], input_array[17][20], input_array[17][21], input_array[17][22], input_array[17][23]);
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[18][0], input_array[18][1], input_array[18][2], input_array[18][3], input_array[18][4], input_array[18][5], input_array[18][6], input_array[18][7], input_array[18][8], input_array[18][9], input_array[18][10], input_array[18][11], input_array[18][12], input_array[18][13], input_array[18][14], input_array[18][15], input_array[18][16], input_array[18][17], input_array[18][18], input_array[18][19], input_array[18][20], input_array[18][21], input_array[18][22], input_array[18][23]);
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[19][0], input_array[19][1], input_array[19][2], input_array[19][3], input_array[19][4], input_array[19][5], input_array[19][6], input_array[19][7], input_array[19][8], input_array[19][9], input_array[19][10], input_array[19][11], input_array[19][12], input_array[19][13], input_array[19][14], input_array[19][15], input_array[19][16], input_array[19][17], input_array[19][18], input_array[19][19], input_array[19][20], input_array[19][21], input_array[19][22], input_array[19][23]);
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[20][0], input_array[20][1], input_array[20][2], input_array[20][3], input_array[20][4], input_array[20][5], input_array[20][6], input_array[20][7], input_array[20][8], input_array[20][9], input_array[20][10], input_array[20][11], input_array[20][12], input_array[20][13], input_array[20][14], input_array[20][15], input_array[20][16], input_array[20][17], input_array[20][18], input_array[20][19], input_array[20][20], input_array[20][21], input_array[20][22], input_array[20][23]);
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[21][0], input_array[21][1], input_array[21][2], input_array[21][3], input_array[21][4], input_array[21][5], input_array[21][6], input_array[21][7], input_array[21][8], input_array[21][9], input_array[21][10], input_array[21][11], input_array[21][12], input_array[21][13], input_array[21][14], input_array[21][15], input_array[21][16], input_array[21][17], input_array[21][18], input_array[21][19], input_array[21][20], input_array[21][21], input_array[21][22], input_array[21][23]);
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[22][0], input_array[22][1], input_array[22][2], input_array[22][3], input_array[22][4], input_array[22][5], input_array[22][6], input_array[22][7], input_array[22][8], input_array[22][9], input_array[22][10], input_array[22][11], input_array[22][12], input_array[22][13], input_array[22][14], input_array[22][15], input_array[22][16], input_array[22][17], input_array[22][18], input_array[22][19], input_array[22][20], input_array[22][21], input_array[22][22], input_array[22][23]);
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
input_array[23][0], input_array[23][1], input_array[23][2], input_array[23][3], input_array[23][4], input_array[23][5], input_array[23][6], input_array[23][7], input_array[23][8], input_array[23][9], input_array[23][10], input_array[23][11], input_array[23][12], input_array[23][13], input_array[23][14], input_array[23][15], input_array[23][16], input_array[23][17], input_array[23][18], input_array[23][19], input_array[23][20], input_array[23][21], input_array[23][22], input_array[23][23]);

}

inline void set(int* array, int row, int col, int val) {
	array[row*8 + col] = val;
}


#define mean(a, b) (a + b) >> 1

inline void calculateRow(int* array) {
	
	int a0 = array[0];
	int a1 = array[1];
	int a2 = array[2];
	int a3 = array[3];
	int a4 = array[4];
	int a5 = array[5];
	int a6 = array[6];
	int a7 = array[7];

	// first round
	int b0 = mean(a0, a1);
	int b1 = mean(a2, a3);
	int b2 = mean(a4, a5);
	int b3 = mean(a6, a7);
	
	array[4] = a0-b0;
	array[5] = a2-b1;
	array[6] = a4-b2;
	array[7] = a6-b3;
	
	// second round
	int c0 = mean(b0, b1);
	int c1 = mean(b2, b3);
	
	array[2] = b0 - c0;
	array[3] = b2 - c1;
	
	// third round
	int d0 = mean(c0, c1);
	
	array[0] = d0;
	array[1] = c0 - d0;

}

inline void calculateCol(int* array, int pitch) {

	int a0 = array[0];
	int a1 = array[pitch];
	int a2 = array[pitch << 1];
	int a3 = array[pitch * 3];
	int a4 = array[pitch << 2];
	int a5 = array[pitch * 5];
	int a6 = array[pitch * 6];
	int a7 = array[pitch * 7];

	// first round
	int b0 = mean(a0, a1);
	int b1 = mean(a2, a3);
	int b2 = mean(a4, a5);
	int b3 = mean(a6, a7);
	
	array[pitch << 2] = a0-b0;
	array[pitch * 5] = a2-b1;
	array[pitch * 6] = a4-b2;
	array[pitch * 7] = a6-b3;
	
	// second round
	int c0 = mean(b0, b1);
	int c1 = mean(b2, b3);
	
	array[pitch << 1] = b0 - c0;
	array[pitch * 3] = b2 - c1;
	
	// third round
	int d0 = mean(c0, c1);
	
	array[0] = d0;
	array[pitch] = c0 - d0;

}

inline void reverseRow(int* array) {
	int a0 = array[0];
	int a1 = array[1];	
	
	// third round ^-1
	int b0 = a0 + a1;
	int b1 = a0 - a1;
	
	int b2 = array[2];
	int b3 = array[3];
	
	// second round ^-1
	int c0 = b0 + b2;
	int c1 = b0 - b2;
	
	int c2 = b1 + b3;
	int c3 = b1 - b3;
	
	int c4 = array[4];
	int c5 = array[5];
	int c6 = array[6];
	int c7 = array[7];
	
	// first round ^-1
	int d0 = c0 + c4;
	int d1 = c0 - c4;
	
	int d2 = c1 + c5;
	int d3 = c1 - c5;
	
	int d4 = c2 + c6;
	int d5 = c2 - c6;
	
	int d6 = c3 + c7;
	int d7 = c3 - c7;
	
	array[0] = d0;
	array[1] = d1;
	array[2] = d2;
	array[3] = d3;
	array[4] = d4;
	array[5] = d5;
	array[6] = d6;
	array[7] = d7;
}

inline void reverseCol(int* array, int pitch) {
	int a0 = array[0];
	int a1 = array[pitch];	
	
	// third round ^-1
	int b0 = a0 + a1;
	int b1 = a0 - a1;
	
	int b2 = array[pitch << 1];
	int b3 = array[pitch * 3];
	
	// second round ^-1
	int c0 = b0 + b2;
	int c1 = b0 - b2;
	
	int c2 = b1 + b3;
	int c3 = b1 - b3;
	
	int c4 = array[pitch << 2];
	int c5 = array[pitch * 5];
	int c6 = array[pitch * 6];
	int c7 = array[pitch * 7];
	
	// first round ^-1
	int d0 = c0 + c4;
	int d1 = c0 - c4;
	
	int d2 = c1 + c5;
	int d3 = c1 - c5;
	
	int d4 = c2 + c6;
	int d5 = c2 - c6;
	
	int d6 = c3 + c7;
	int d7 = c3 - c7;
	
	array[0] = d0;
	array[pitch] = d1;
	array[pitch << 1] = d2;
	array[pitch * 3] = d3;
	array[pitch << 2] = d4;
	array[pitch * 5] = d5;
	array[pitch * 6] = d6;
	array[pitch * 7] = d7;
}

int main() {
	
	//int* chunk = input_array[0];
	
	
	// int i;
	// for (i=0; i<8; i++) {
		// int j;
		// for (j=0; j<8; j++) {
			// set(chunk, i, j, input_array[i][j]);
		// }
	// }
	
	// //set with example data
	// int testData[8] =  {7,1,6,6,3,-5,4,2};
	// for (i=0; i<8; i++)
		// chunk[i*8] = testData[i];
	
	 // printArray2d(input_array);
//	int col, row;
	
#define CELLS WIDTH*HEIGHT
#define COL_JUMP (WIDTH * KERNEL_SIZE)
#define COL_JUMP_2 (WIDTH * KERNEL_SIZE * 2)
	printArray2d();
	/*int* base = &input_array[0][0];
	
	int cell;
	for(cell = 0; cell < CELLS; cell+=KERNEL_SIZE) {
		calculateRow(&base[cell]);
	}
	
	for(cell = 0; cell < WIDTH; cell++) {
		calculateCol(&base[cell], PITCH);
	}
	
	for(cell = 0; cell < WIDTH; cell++) {
		calculateCol(&base[cell + COL_JUMP], PITCH);
	}
	
	for(cell = 0; cell < WIDTH; cell++) {
		calculateCol(&base[cell + COL_JUMP_2], PITCH);
	}
	printArray2d(input_array);
	for(cell = 0; cell < WIDTH; cell++) {
		reverseCol(&base[cell], PITCH);
	}
	
	for(cell = 0; cell < WIDTH; cell++) {		
		reverseCol(&base[cell + COL_JUMP], PITCH);
	}
	
	for(cell = 0; cell < WIDTH; cell++) {
		reverseCol(&base[cell + COL_JUMP_2], PITCH);
	}
	
	for(cell = 0; cell < CELLS; cell+=KERNEL_SIZE) {
		reverseRow(&base[cell]);
	}
	
	printArray2d(input_array);
		
*/
	return 0;
}



