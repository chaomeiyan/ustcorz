#include <iostream>
#include <vector>
#include <fstream> 
#include "time.h" 
#include <stdlib.h>
constexpr auto NUM_ID = 7000; 
constexpr auto NUM_CIR = 230;
constexpr auto LEN = 5100;
using namespace std;
/*
����ͼ�洢�����򻷣�
*/

int visited[NUM_ID];
typedef struct{
	unsigned int data[7] = { 0 };
	unsigned int length=0;
}CIR;
CIR circle[NUM_CIR];

	bool DFS(unsigned int G[][9], unsigned int ID, int numcirl) {
	visited[ID] = numcirl;
	circle[numcirl].data[circle[numcirl].length++] = ID;
	if (circle[numcirl].length == 7) {//���ĳ���ҪС��8
		for (unsigned int k=1; k <= G[ID][0]; k++) {//ID��û�г����ǻ�ͷ
			if (G[ID][k] == circle[numcirl].data[0]) return 1;//�ҵ���
		}
		circle[numcirl].length = 0;
		visited[ID] = -1;
		return 0;//���ǻ�
	}
	for (unsigned int i = 1; i <= G[ID][0]; i++) {
		if (circle[numcirl].length > 2)//���ĳ���Ҫ����2
			for (unsigned int j = 1; j <= G[ID][0]; j++)
				if (G[ID][j] == circle[numcirl].data[0]) return 1;//�ҵ���
		if (visited[G[ID][i]] == numcirl) continue;
		if (DFS(G, G[ID][i], numcirl)) return 1;
		else {
			visited[ID] = -1;
			return 0;
		}
	}
	circle[numcirl].length = 0;
	return 0;
}

int main()
{
	long start = clock();  //��ʼʱ��
	long finish;//����ʱ�䣬���ڲ��Դ�������ʱ��
	long t;//ʱ����
   
	//�򿪶�ȡ���ݵ��ļ�
	FILE* fin = NULL;
	if (fopen_s(&fin, "test_data.txt", "rb") != 0) {
		printf("error open test_data.txt\n");
		return 0;
	}
	if (fin == 0) return 0;

	unsigned int* df1 = (unsigned int*)malloc(sizeof(unsigned int) * NUM_ID);
	unsigned int* df2 = (unsigned int*)malloc(sizeof(unsigned int) * NUM_ID);
	unsigned int n = 0;
	unsigned int max=0;//����ID
	while (fscanf_s(fin, "%u", df1 + n) != EOF) {
		fseek(fin, 1, 1);
		fscanf_s(fin, "%u", df2 + n);
		if (df1[n] > max) max = df1[n];
		if (df2[n] > max) max = df2[n];
		fseek(fin, 2, 1);
		while (fgetc(fin) != '\n');
		n++;
	}
	fclose(fin);
	
	unsigned int G[NUM_ID][9];//����ͼ 

	for (unsigned int i = 0; i <= max; i++){
		G[i][0] = 0;
		visited[i] = -1;
	}

	unsigned int ID;
	
	//��������ͼ���ڽӾ���
	for (unsigned int i = 0; i < n; i++) {
		ID = df1[i];
		G[ID][G[ID][0]+1] = df2[i];
		G[ID][0]++;//���ȼ�һ
	}

	int numcirl=0;//����������
	for (unsigned int i = 0; i <= max; i++) {
		if (i == 6000)
			printf("d");
		circle[numcirl].length = 0;//��ʼ��
		if (G[i][0] == 0) continue;
		circle[numcirl].data[circle[numcirl].length++] = i;
		visited[i] = numcirl;
		ID = G[i][1];
		if (i > 4000 && circle[112].length != 6)
			printf("d");

		if (DFS(G, ID, numcirl)) numcirl++;
		else {
			visited[i] = -1;
			circle[numcirl].length = 0;
		}
	}
	unsigned int tmp[7], len, minID, flag, *sort;
	for (unsigned int i = 0; i < numcirl; i++) {//��������
		sort = &circle[i].data[0];
		len = circle[i].length;
		minID = *sort;

		flag = 0;
		for (unsigned int j = 1; j < len; j++) {//����С
			if (*(sort+j) < minID) {
				minID = *(sort+j);
				flag = j;
			} 
		}
		if (flag == 0)continue;//˳����ȷ
		for (unsigned int j = 0; j < len; j++) {
			tmp[j] = *(sort + j);
		}
		for (unsigned int j = 0; j < len; j++) {
			*(sort + j) = tmp[(flag + j) % len];
		}
	}
	
	unsigned int* sortID = (unsigned int*)malloc(sizeof(unsigned int) * numcirl),head=0,rear=numcirl-1;
   //��������Ű����ĳ����ռ�����һ�ˣ�3��7���ڶ��ˣ�4��5�������ˣ�6��
	for (unsigned int j= 0; j < numcirl; j++) {//��������
		if (circle[j].length == 3) sortID[head++] = j;
		if (circle[j].length == 7) sortID[rear--] = j;
	}
	for (unsigned int j = 0; j < numcirl; j++) {//��������
		if (circle[j].length == 4) sortID[head++] = j;
		if (circle[j].length == 6) sortID[rear--] = j;
	}
	for (unsigned int j = 0; j < numcirl; j++) {//��������
		if (circle[j].length == 5) sortID[head++] = j;
	}

	//������ļ�
	FILE* fout = NULL;
	if (fopen_s(&fout, "myresult.txt", "wb") != 0) {
		printf("error open result.txt\n");
		return 0;
	}
	if (fout == 0) return 0;
	unsigned int idx;
	fprintf(fout, "%u\n", numcirl);
	for (unsigned int i = 0, j; i < numcirl; i++) {
		idx = sortID[i];//����ȷ��˳�������
		for (j = 0; j < circle[idx].length - 1; j++) {
			fprintf(fout, "%u,", circle[idx].data[j]);
		}
		fprintf(fout, "%u\n", circle[idx].data[j]);//ÿ������������
	}
	

	printf("%u", max);
	finish = clock();   //����ʱ��
	cout << "ʱ��: " << finish - start << "ms\n\n"; //���ʱ��
	t = finish - start;
}

