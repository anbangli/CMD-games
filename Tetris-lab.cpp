/***********************************************************************
������˹���顷��Tetris�� ���ģ����֧��ڧ㣩��һ���ɶ���˹�˰��п�л��
������ŵ����1984��6�·�����������Ϸ�������������ƶ�����ת�Ͱڷ���Ϸ�Զ�
����ĸ��ַ��飬ʹ֮���г�������һ�л���в��������÷֡�

��������ɫ����1��ʹ��C���Ա�д��������ʹ�������飬��δʹ��ָ��ͽṹ�壻
��2���ն˴����д�ӡ���������ַ���������ۡ�
ԭ���ߣ�LiuTianyong   https://github.com/LiuTianyong/didactic-octo-waffle

ԭ�����ǰѶ���˹�����������Ϸ�ϲ���һ����ɾ��Ϊ����˹���顣
�ѳ����и���С�����ϲ����������У�һЩȫ�ֱ�����Ϊ�ֲ�����������������
�޸��ߣ���� ��anbangli@qq.com��  2019��5�� 
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

//����ȫ�ֱ���
const int HGT = 19;	//�߶�����
const int WID = 20;	//�������
int Map[HGT][WID];	//��ͼ�Ĵ�С

const int VARIANTS = 28;	//����������7�ַ��飬ÿ����4�ֱ��Σ���28��
//ÿ�ַ����� 4 ��С���飬����������ĳ��С����Ϊ���㣬����Ϊ(0, 0)�� Ȼ��
//ֻ�������г�����3��С��������ڸÿ�� x �� y ֵ���� 6 ��ֵ�����ɡ�
int tetris[VARIANTS][6] = {	//�������෽�����Ϣ����һά�ȱ�ʾ���࣬�ڶ�ά�ȱ�ʾ����
	{ 0,-1,0,-2,1,0 },{ 0,1,1,0,2,0 },{ -1,0,0,1,0,2 },{ 0,-1,-1,0,-2,0 },
//  ��                ����
//  ��      ������      ��        ��
//  ����    ��          ��    ������
	{ 0,-1,0,1,-1,0 },{ 0,1,1,0,-1,0 },{ 1,0,0,-1,0,1 },{ 1,0,-1,0,0,-1 },
//    ��              ��        ��
//  ����    ������ 	  ����    ������
//    ��      ��      ��
	{ -1,1,0,1,1,0 },{ 0,-1,1,0,1,1 },{ -1,0,0,-1,1,-1 },{ -1,-1,-1,0,0,1 },
//	          ��        ����    ��
//	  ����    ���� 	  ����      ����
//	����        ��                ��
	{ -1,0,0,1,1,1 },{ 0,1,1,-1,1,0 },{ -1,0,0,1,1,1 },{ 0,1,1,-1,1,0 },
//	            ��                ��
//	����      ���� 	  ����      ����
//	  ����    ��        ����    ��
	{ -1,0,0,-1,0,-2 },{ -1,0,-2,0,0,1 },{ 0,1,0,2,1,0 },{ 0,-1,1,0,2,0 },
//	  ��                ����    ��
//	  ��    ������ 	    ��      ������
//	����        ��      ��
	{ 0,1,1,0,1,1 },{ 0,-1,1,0,1,-1 },{ -1,0,0,-1,-1,-1 },{ -1,0,-1,1,0,1 },
//	����      ����      ����    ����
//	����      ���� 	    ����    ����
	{ 0,-1,0,1,0,2 },{ -1,0,1,0,2,0 },{ 0,1,0,-1,0,-2 },{ 1,0,-1,0,-2,0 }
//	  ��                ��
//	  ��    �������� 	��    ��������
//	  ��                ��
//    ��                ��
};

//����ԭ������
void gotoxy(int x, int y);	//��λ���
void HideCursor();	// ���ع��
void ShowCursor();	//��ʾ���

void DrawMap(int next, int score);	//���Ƶ�ͼ
void DrawTetris(int cur, int tx, int ty, int show);	//���Ʒ���(showΪ0ʱ������

int Moveable(int x, int y, int cur);		//�жϷ����Ƿ�����ƶ�
void CheckScore(int next, int &score, int &Gamespeed);	//���÷�

//�������ն˴��ڵ���Ϸʱ����Ҫ����Ļ�Ͼ�ȷ��λ��������ݣ�
//������Ҫ�Զ�����Ӧ�ĺ��� gotoxy��HideCursor�� ShowCursor
//����������Ҫ�õ��ṹ���Windows����е�֪ʶ����ѧ�߲���Ū��
void gotoxy(int x, int y) { //��λ���λ��
	COORD coord;	//����ṹ��coord (����ϵcoord)
	coord.X = x;
	coord.Y = y;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	//����������handle����ȡ����̨������(ֵΪ-11)
	SetConsoleCursorPosition(handle, coord); //�ƶ����
}

void HideCursor() { //���ع��
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}

void ShowCursor() { //��ʾ���
	CONSOLE_CURSOR_INFO cursor_info = {20, 1};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}


void DrawMap(int next, int score) {	//���Ƶ�ͼ����������
	int x, y;
	//��̬���Ƶ�ͼ��ÿ��Ԫ��ռ2��
	gotoxy(WID-5, 0);	 //����(��0��)����
	printf("����˹����");//��ʾ��Ϸ����
	gotoxy(0, 1);
	for (y = 0; y<HGT; y++) {	//��
		for (x = 0; x<WID; x++) {	//��
			if (y == 0 || y == HGT - 1 	//���л����
			        || x == 0 || x == WID - 1) //��ǽ����ǽ
				printf("##");
			else if (Map[y][x] == 0) 	//�հ�
				printf("  ");
			else if (Map[y][x] == 2) 	//С����ռ��
				printf("��");
			else if (Map[y][x] == 3)	//����������
				printf("��");

		}
		puts("");
	}

	//�����ұ��������Ϣ����һ���飬�÷֣�����
	gotoxy(WID*2+2, 2);
	printf("����:");
	//�����һ����
	for (y = 2; y <= 10; y++) {
		for (x = WID*2; x <= WID*2+10; x++) {
			gotoxy(x + 1, y + 1);
			printf("  ");
		}
	}
	//�ػ���һ����
	gotoxy(WID*2+5, 6);
	printf("��");
	for (y = 0; y<6; y = y + 2) {
		gotoxy(WID*2+5 + 2 * tetris[next][y], 6 + tetris[next][y+1]);
		printf("��");
	}
	gotoxy(WID*2+4, 12);
	printf("�÷� :   %d", score);
	gotoxy(WID*2+4, 14);
	printf("���� :   %d", score / 100 + 1);
}

void DrawTetris(int cur, int tx, int ty, int show) {//���Ʒ��� (show Ϊ 0 ʱΪ������
	int x = tx * 2;	//ÿ���������ռ2������x=tx*2
	int y = ty + 1;	//���ݵ�ͼ���ƺ�������Ļ����������д��Ϸ���⣬����Ҫ +1
	if (y >= 2) {
		gotoxy(x, y);
		printf(show ==0? "  " : "��");	//"��");
	}
	for (int i = 0; i < 6; i = i + 2) {
		x = (tx + tetris[cur][i])*2;
		y = ty +1 + tetris[cur][i+1];
		if ( y >= 2 ) {
			gotoxy(x, y);
			printf(show == 0? "  " : "��");
		}
	}
}

int Moveable(int y, int x, int cur) {	//�жϷ����Ƿ�����ƶ�
	if (Map[y][x] != 0)return 0;
	int x2, y2;
	for (int i = 0; i<6; i = i + 2) {
		x2 = x + tetris[cur][i];
		y2 = y + tetris[cur][i+1];
		//��� x ������ͼ�������꣬�� y ���������·����꣬�򲻿��ƶ�
		if ( x2 < 0 || x2 > WID-2 || y2 >= HGT-1)
			return 0;
		//���(x2,y2)�����ڵ�ͼ���귶Χ�ڣ��ҵ�ͼ������С�飬�򲻿��ƶ�
		if ( x2 >= 0 && x2 < WID-2 && y2 >=0 && y2 < HGT-1 && Map[y2][x2] != 0)
			return 0;
	}
	return 1;
}


int main() {	//����˹����
	srand(time(0));
	int cur = rand() % VARIANTS;	//��ǰ��������
	int next = rand() % VARIANTS;	//��һ����������
	int tmp;
	//tx��ty: �ƶ��еķ���(tetris)��x��y����
	int tx = WID/2;		//����Ӷ������뿪ʼ����
	int ty = 0;

	int key;	//�û�����ļ�ֵ��¼
	int score = 0;	//��Ϸ�÷�
	int Gamespeed = 200;	//��Ϸ�ٶȣ������û�����ļ������λΪms��

	//��ʼ����ͼ
	int y, x;
	for (y = 0; y < HGT; y++) {
		for (x = 0; x<WID; x++)
			if (y == HGT - 1 || x == 0 || x == WID - 1)
				Map[y][x] = -1;	//��ǽ����ǽ�͵ײ�������Ϊǽ
		//������������Ϊǽ���Ա�����д��������£�
			else
				Map[y][x] = 0;	//�м�Ϊ��
	}
	HideCursor();	//���ع��
	DrawMap(next, score);	//�����ͼ���ұ߲���

	while(1) {
		DrawTetris(cur, tx, ty, 1);	//��ʾ����(�ڶ�����Ϊ1��
		if (kbhit()) {	//����û����˼������ȡ�������ж�
			key = getch();	//��ȡ��ֵ�����ƶ�
			while (kbhit()) 	//�û���������ʱ����ȡ�����ַ�
				key = getch();
			DrawTetris(cur, tx, ty, 0);	//��������(�ڶ�����Ϊ0��
			if (key == 'H') {	//up, ����
				tmp = cur;
				cur++;
				if (cur % 4 == 0 && cur != 0)
					cur = cur - 4;
				if (Moveable(ty, tx, cur) != 1)
					cur = tmp;
			} else if (key == 'P' && Moveable(ty + 1, tx, cur) == 1) //down
				ty++;
			else if (key == 'K' && Moveable(ty, tx - 1, cur) == 1)	//left
				tx--;
			else if (key == 'M' && Moveable(ty, tx + 1, cur) == 1)	//right
				tx++;
			else if (key == 27) {	//ESC
				gotoxy(0, HGT+1);
				ShowCursor();//��ʾ���
				printf("��Ϸ��ͣ���Ƿ����(Y/N)?");
				if ((key = toupper(getch())) == 'Y') {
					HideCursor();
					printf("\r                         ");
				} else
					return 0;
			}
			DrawTetris(cur, tx, ty, 1);	//���Ʒ��飨�ڶ�����Ϊ1��
		} else {	//�ް���
			if (Moveable(ty+1, tx, cur) == 1) {	//����λ�úϷ�����������
				DrawTetris(cur, tx, ty, 0);	//��������
				ty++;	//����
				DrawTetris(cur, tx, ty, 1);	//���Ʒ���
			} else { //������ײ����������λ�ö���ʵ����ͼλ��ֵȡΪ2��
				Map[ty][tx] = 2;
				for (int i = 0; i < 6; i = i + 2) {
					Map[ty + tetris[cur][i+1]][tx + tetris[cur][i]] = 2;
				}
				CheckScore(next, score, Gamespeed);	//��������������������

				if (ty <= 2) {	//����ֹͣ�ڵ�ͼ���У����ж�Ϊ��Ϸ����
					gotoxy(0, HGT+1);
					printf("Game Over��");
					return 0;
				} else {	//����Ҫ���������µķ���
					cur = next;
					next = abs(rand() % VARIANTS);
					tx = WID/2;
					ty = 0;
				}
				DrawMap(next, score);	//���Ƶ�ͼ���˴�ˢ����һ�����飩
			}
		}
		Sleep(Gamespeed);
	}
	ShowCursor();
	return 0;
}


/*************************************************
Function: CheckScore()
Description: ����Ƿ���ĳ�б�С������������������
*************************************************/
void CheckScore(int next, int &score, int &Gamespeed) {
	int y=HGT-1, x, full;
	while(y>=1) {	//��
		full = 1;	//��鱾���Ƿ�������
		for (x = 1; x< WID - 1; x++) {//��
			if (Map[y][x] != 2) { 	//�пտ�
				full = 0;
				break;
			}
		}
		if (full == 0) {	//����δ����
			y--;
		} else {	//����������
			for (x = 1; x < WID -1; x++)
				Map[y][x]=3;	//��־����������
			DrawMap(next, score); //�ػ��ͼ��������λ�ö�����ʾ��
			Sleep(200);
			//��������
			for (int y2 = y; y2>1; y2--) 
				for (x = 1; x < WID - 1; x++)
					Map[y2][x] = Map[y2 - 1][x];
			DrawMap(next, score);//�ػ��ͼ
			score += 10;
			if (score % 100 == 0 && score != 0)  
				Gamespeed -= 50;	//��Ϸ�ٶȼӿ� 
		}
	}
}

