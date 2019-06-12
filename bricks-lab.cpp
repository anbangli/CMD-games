/*********************************************************************
�����ܣ���ש��
���ߣ�����
���ӣ�https://zhuanlan.zhihu.com/p/55963487
��Դ��֪��
����Ȩ���������С���ҵת������ϵ���߻����Ȩ������ҵת����ע��������

*********************************************************************/

#include <stdio.h>
#include <Windows.h>
#include <conio.h>

#define WIDTH 20//��Ϸ��ͼ���
#define HEIGHT 20//��Ϸ��ͼ�߶�

int map[HEIGHT][WIDTH];//��ͼ��ά����
//0-�հף�1-Χǽ��2-ש�飬3-�����壬4-С��

int y=HEIGHT-3, x=WIDTH/2;//С��ĳ�ʼλ��
int vy=-1, vx=1;	//С��ĳ�ʼ����
int ban=WIDTH/2-3;	//������ĳ�ʼλ��
DWORD old_time,new_time;//��ȡϵͳ����ʱ��

//�������ն˴��ڵ���Ϸʱ����Ҫ����Ļ�Ͼ�ȷ��λ��������ݣ�
//������Ҫ�Զ�����Ӧ�ĺ��� gotoxy��HideCursor�� ShowCursor
//����������Ҫ�õ��ṹ���Windows����е�֪ʶ����ѧ�߲���Ū��
//��Ļ���Ͻ�Ϊ(0,0)����������Ϊ X �ᣬ��������Ϊ Y ��
int gotoxy(int x, int y) { //��λ���λ�õ�(x,y)
	COORD coord;	//����ṹ��coord (����ϵcoord)
	coord.X = x;
	coord.Y = y;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	//����������handle����ȡ����̨������(ֵΪ-11)
	SetConsoleCursorPosition(handle, coord); //�ƶ���굽(x,y)
}

void HideCursor() { //���ع��
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}

void ShowCursor() { //��ʾ���
	CONSOLE_CURSOR_INFO cursor_info = {20, 1};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}

void init() { //��ʼ��
	for(int i=0; i<HEIGHT; i++) {	//��ͼ
		for(int j=0; j<WIDTH; j++) {
			if(j==0 || j==WIDTH-1 || i==HEIGHT-1 || i==0)
				map[i][j]=1;//1Χǽ
			else
				map[i][j]=0;//0�ո�
		}
	}

	for(int j=7; j<14; j++)
		map[HEIGHT-2][j]=3;//3������

	for(int i=2; i<7; i++)
		for(int j=1; j<WIDTH-1; j++)
			if(j%2 == 0)
				map[i][j]=2;//2ש��

	map[y][x]=4;//4С��
}

void showall() { //��ʾȫ��
	for(int i=0; i<HEIGHT; i++) {	//��
		for(int j=0; j<WIDTH; j++) {	//��
			switch(map[i][j]) {
				case 0:	//0-�հ�
					printf("  ");
					break;
				case 1: {	//1-Χǽ
					if(j==0)
						printf("||");
					else if(j==WIDTH-1)
						printf("||\n");
					else if(i==0||i==HEIGHT-1)
						printf("��");
					break;
				}
				break;
				case 2:	//2-ש��
				case 3:	//3-������
					printf("��");
					break;
				case 4:	//4-С��
					printf("��");
					break;
			}
		}
	}
}

void showxy(int x, int y, int v) { 	//��(x,y) ���� v ֵ��ʾ
	gotoxy(x*2, y);
	if (v == 0)	printf("  ");	//0-�հ�
	if (v == 2 || v == 3)	printf("��");	//2-ש��, 3-������
	if (v == 4) printf("��");	//4-С��
}

void game() { //��Ϸ��Ҫ�߼�����
	int flag=0;//�жϱ�־λ
	int i, j;
	gotoxy(0,0);//�������ƶ���0��0��

	for(j=0; j<7; j++) {
		map[HEIGHT-2][j+ban]=0;//3���
		//	showxy(j+ban, HEIGHT-2, 0);	//���ԭλ�����
	}

	char ch;
	if(kbhit()) { //�ж��Ƿ��м������룬������ch ����
		ch = getch();
		while (kbhit()) 	//�û���������ʱ����ȡ�����ַ�
			ch = getch();
		switch(ch) {
			case 75:	//left
			case 'a':
			case 'A':
				if(ban>1)
					ban--;
				break;
			case 77:	//right
			case 'd':
			case 'D':
				if(ban+7 < WIDTH-1)
					ban++;
				break;
		}
	}

	//Sleep(100);
	new_time=GetTickCount();
	if(new_time - old_time >= 150) {
		old_time = new_time;
		new_time = GetTickCount();

		for(j=1; j<ban; j++) 	//������Ϊ�հ�
			showxy(j, HEIGHT-2, 0);
		for(j=ban; j<ban+7; j++) {	//���
			map[HEIGHT-2][j] = 3;//3-���
			showxy(j, HEIGHT-2, 3);
		}
		for(j=ban+7; j<WIDTH-1; j++) 	//����ұ�Ϊ�հ�
			showxy(j, HEIGHT-2, 0);

//		for(j=0; j<7; j++){	//���º��
//			map[HEIGHT-2][j+ban]=3;//3-���
//			showxy(j+ban, HEIGHT-2, 3);	//�ػ���
//		}

		//flag=0, ���ж����ϻ�������ײ����������ж������ж�б��
		if(vy==-1) { //С�����Ϸ��˶�
			if(map[y-1][x]==2) { //����Ϸ���ש��
				map[y-1][x]=0;//��ש��λ����Ϊ0
				showxy(x, y-1, 0);	//���ԭ��ש��
				vy=-vy;//�ı��˶�����
				flag=1;//����־λ��Ϊ1
			}
			if(map[y-1][x]==1) { //����Ϸ���Χǽ
				vy=-vy;//�ı��˶�����
			}
		}
		if(vx==-1) { //С��������˶�
			if(map[y][x-1]==2) {
				map[y][x-1]=0;
				showxy(x-1, y, 0);	//���ԭ��ש��
				vx=-vx;
				flag=1;
			}
			if(map[y][x-1]==1) {
				vx=-vx;
			}

		}
		if(vx==1) { //С�����Ҳ��˶�
			if(map[y][x+1]==2) {
				map[y][x+1]=0;
				showxy(x+1, y, 0);	//���ԭ��ש��
				vx=-vx;
				flag=1;
			}
			if(map[y][x+1]==1) {
				vx=-vx;
			}

		}
		if(vy==1) { //С�����·��˶�
			if(map[y+1][x]==2) {
				map[y+1][x]=0;
				showxy(x, y+1, 0);	//���ԭ��ש��
				vx=-vx;
				flag=1;
			}
			if(map[y+1][x]==3) { //����·��Ƿ�����
				vy=-vy;
			}
			if(y+1==HEIGHT-1) { //����·��ǵ��±߽磬����Ϸ�������˳�
				gotoxy (WIDTH/2, HEIGHT+1);
				printf("Game Over!");
				getchar();
				exit(0);
			}
		}

		//δ�������ϻ�������ײ�����ж��Ƿ���б��
		if(flag==0 && map[y+vy][x+vx]==2) {
			map[y+vy][x+vx]=0;
			showxy(x+vx, y+vy, 0);	//���ԭ��ש��
			vy=-vy;
			vx=-vx;
		}

		//ball moves
		map[y][x]=0;//��С��ԭλ����Ϊ0
		showxy(x, y, 0);	//���ԭλ�õ�С��
		y = y + vy;
		x = x + vx;
		map[y][x]=4;
		showxy(x, y, 4);	//����λ�û���С��

		//showall();
	}
}

int main() {
	HideCursor();
	init();
	showall();
	old_time=GetTickCount();
	while(1) {
		game();
	}
	ShowCursor();
	return 0;
}
