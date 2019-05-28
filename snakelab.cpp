/******************
***C����̰���� ***
***����:iimT*******
***2016/12/12****
*******************/

//ԭ������ַ�� https://github.com/tfh93121/C-Snake/blob/master/snake.c
//ȱ����ʹ�� cls ����������������Ļ��˸�ú�������
//����� gotoxy ������ʵ�ֿ��ٵ��ظ��������壬�����Ļ��˸���⡣
//--anbangli@qq.com   2019-4-25

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>	//Windowsϵͳ�й��ڴ��ڵ��������Ժͺ������õ�ͷ�ļ� 

const int MapWid = 38;//��ͼ���
const int MapHt = 22;//��ͼ�߶�

char map[MapHt][MapWid];//�����ͼ
char key;//���������û�����ʱ����ļ�ֵ

int direct = 4;//�ߵ�ǰ���˶������� 1���� 2���� 3���� 4 
int food[2] = {9,11};//food[0]ʳ�������  food[1]ʳ��������

int snake[400][3];//�ߵ�����������Լ��ߡ�[i][0]Ϊ����xֵ��[i][1]Ϊ����yֵ��[i][2]Ϊ��ͷ��������ַ�ֵ
//[0][]Ϊ��β������Ϊ��ͷ����
int head =0; //��ͷ�������е����꣨�ߵĳ���Ϊ head+1��

void Init();//��Ϸ��ʼ��ʱ���ʼ��
void MoveSnake(int direct);//���ߵ���������ƶ�
void MakeFood(int food[]);//�������������һ��ʳ������
void DrawMap();  //�������ͼ
void DrawSnake();  //����ߺ�ʳ��
int ifBump();//�ж��Ƿ����ײ(ײ�Լ���ײǽ)
void getKey();//��ȡ��ֵ�����������˶�����

//�������ն˴��ڵ���Ϸʱ����Ҫ����Ļ�Ͼ�ȷ��λ��������ݣ�
//������Ҫ�Զ�����Ӧ�ĺ��� gotoxy��HideCursor�� ShowCursor
//����������Ҫ�õ��ṹ���Windows����е�֪ʶ����ѧ�߲���Ū�� 
int gotoxy(int x, int y) { //��λ���λ��
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

//////////////////////////////////////////////////////////////////////////////
int main() {
	Init();	//��ʼ��
	srand(time(0));	//��ʼ�����������
	HideCursor();	//���ع��
	
	DrawMap();		//���Ƶ�ͼ
	DrawSnake();	//�����ߺ�ʳ�� 
	while (1) {
		getKey();	//��ȡ�û����� !!!
		MoveSnake(direct);//���߿�ʼ�ƶ� !!!
		
		if (!food[0] && !food[1]) {//ʳ���Ѿ����Ե�������������ʳ��
			MakeFood(food);//����ʳ������
		}
		DrawSnake();	//�����ߺ�ʳ�� 
		
		if (ifBump()) {	//�����ײ 
			gotoxy(MapWid/4, MapHt/2-3); 
			ShowCursor();
			printf("��Ϸ��������ĳɼ�Ϊ��%d", head);
			break;
		}
		Sleep(150);
		getKey();
	}
	getch(); 
	return 0;
}

void Init() {//��ʼ��
	//��ʼ���ߡ�//��βΪһ���հ��ַ����Ա����ʱ�����ԭ����β
	snake[0][0] = 5, snake[0][1] = 9, snake[0][2] = ' ';
	snake[1][0] = 6, snake[1][1] = 9, snake[1][2] = '#';
	snake[2][0] = 7, snake[2][1] = 9, snake[2][2] = '#';
	snake[3][0] = 8, snake[3][1] = 9, snake[3][2] = '@';//��ͷ
	head = 3;
	
	//��ʼ����ͼ
	for (int i = 0; i< MapHt; i++) {
		for (int j = 0; j< MapWid; j++) {
			if (i == 0 || j == 0 || i == MapHt - 1 || j == MapWid - 1) {
				map[i][j] = '*';
			} else {
				map[i][j] = ' ';
			}
		}
	}
	//�ڵ�ͼ������ʾ��Ϸ���� "SNAKE" 
	char title[] = "SNAKE";
	int n = sizeof(title)/sizeof(title[0])-1;
	int k = MapWid/2 - n/2;
	for (int i=0; i < n; i++)
		map[0][k+i] = title[i];
}

void DrawMap() {//����map�����д��ֵ��ӡ����
	for (int i = 0; i < MapHt; i++) {
		for (int j = 0; j< MapWid; j++) {
			//gotoxy(j,i);
			printf("%c%c", map[i][j],map[i][j]);
		}
		printf("\n");
	}
}

void DrawSnake() {//������
	int i;
	//���»�����
	for (i = 0; i <= head; i++) {	//�±�0Ϊ��β���±�����Ϊ��ͷ����
		gotoxy(snake[i][0]*2, snake[i][1]);
		switch(snake[i][2]){
			case '@': printf("��"); break;
			case '#': printf("��"); break;
			case ' ': printf("  "); break;
			default: break;
		}
		
	}
	//˳�����»���ʳ��
	gotoxy(food[0]*2, food[1]);
	printf("()");
}

void MoveSnake(int direct) {//���ߵ����꿪ʼ�ƶ�
	//�ȱ�����ͷ�����꣬�Ա����ʹ��
	int x = snake[head][0];
	int y = snake[head][1];

	switch (direct) {
		case 1://����
			snake[head][1]--;//�����겻�䣬������-1
			break;
		case 2://����
			snake[head][1]++;//�����겻�䣬������+1
			break;
		case 3://����
			snake[head][0]--;//�����겻�䣬������-1
			break;
		case 4://����
			snake[head][0]++;//�����겻�䣬������+1
			break;
	}
	//��ͷ�Ѿ�������һ��λ�ã����ж��Ƿ��Ե�ʳ���������֮ǰ���е�����ŵ�ǰһ����λ����

	if (snake[head][0] == food[0] && snake[head][1] == food[1] ) { //�Ե�ʳ�������1��������ͷ���߾�
		snake[head][2] = '#'; //ԭ������ͷ��Ϊ�߾�
		snake[head][0] = x;	//�߾����ԭ����ͷ��x��y����
		snake[head][1] = y;
		head ++;	//�߳���1��Ȼ���趨�µ���ͷ
		snake[head][0] = food[0];
		snake[head][1] = food[1];
		snake[head][2] = '@';
		food[0] = 0, food[1] = 0;//����ǰ��food�ÿ�
	} else {	//û�Ե�ʳ��������ƶ�
		for (int j = 0; j < head; j++) {//�����������
			snake[j][0] = snake[j+1][0];
			snake[j][1] = snake[j+1][1];
		}
		snake[head-1][0] = x;	//�߾�����Ϊԭ�ȵ���ͷλ��
		snake[head-1][1] = y;
	}
}

void MakeFood(int food[]) {//����һ��ʳ�������
	int x, y, k, good;
	do {	//�������һ��ʳ�����꣬������Ƿ��������ͻ���ظ�����ͻΪֹ��
		x = rand() % (MapWid -2) + 1;
		y = rand() % (MapHt -2) + 1;//����һ��1-50������Ϊ������  1-20������Ϊ������
		good = 1;
		for (k = 0; k <= head; k++) {
			if ((snake[k][0] == x && snake[k][1] == y)) //�г�ͻ
				good = 0;
		}
	} while (!good);

	food[0] = x;
	food[1] = y;
}

int ifBump() {	//�ж��Ƿ�ײǽ��ײ������ 
	if ((snake[head][0]==0|| snake[head][0] == MapWid-1)
	        || (snake[head][1] == 0|| snake[head][1] == MapHt-1))//ײǽ
		return 1;

	for (int i = 1; i < head-1; i++) {	//��Ϊ��β�ǿհ��ַ�������ֻ���ж� 1 ~ head-1
		if ((snake[i][0] == snake[head][0] && snake[i][1] == snake[head][1])) {//ײ������ 
			return 1;
		}
	}
	return 0;
}

void getKey() {
	if (_kbhit()) {	//����û����˼������ȡ�������жϡ������账�� 
		key = _getch();	
		switch (key) {
			case 'H'://�û��� �� ��
				if (direct != 2)
					direct = 1;
				break;
			case 'P'://�û��� �� ��
				if (direct != 1)
					direct = 2;
				break;
			case 'K'://�û��� �� ��
				if (direct != 4)
					direct = 3;
				break;
			case 'M'://�û��� �� ��
				if (direct != 3)
					direct = 4;
				break;
		}
	}
}
