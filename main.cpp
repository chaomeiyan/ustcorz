#include <iostream>
#include <fstream> 
#include <stdlib.h>
#include <time.h>
constexpr auto NUM_ID = 7000;//NUM_IDӦ�ô�������ID��max
constexpr auto NUM_CIR = 230;//���������Ŀ�����Ǵ����ظ��Ļ���
constexpr auto LEN = 5100;//LENӦ�ô����û�������
using namespace std;

//����ṹ�壬ȫ�ֱ���
int visited[NUM_ID];
typedef struct{
	unsigned int data[7];//��һ����
	unsigned int length;//���ĳ���
}CIR;//һ��������Ϣ
CIR circle[NUM_CIR];//Ԫ��Ϊ���ṹ�������

//��DFS�㷨���ҳ����л�
bool DFS(unsigned int G[][9], unsigned int ID, int numcirl) {
	visited[ID] = numcirl;
	circle[numcirl].data[circle[numcirl].length++] = ID;
	if (circle[numcirl].length == 7) {//���ĳ���ҪС��8
		for (unsigned int k=1; k <= G[ID][0]; k++) //ID��û�г����ǻ�ͷ
			if (G[ID][k] == circle[numcirl].data[0]) return 1;//�ҵ���
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
	circle[numcirl].length = 0;//�˳��ݹ�ʱ��
	return 0;
}

int main()
{
	long start = clock();  //��ʼʱ��
	long finish;//����ʱ�䣬���ڲ��Դ�������ʱ��
	long t;//ʱ����
   
	//�򿪶�ȡ���ݵ��ļ��������df��data of file��������
	FILE* fin = NULL;
	unsigned int* df1 = (unsigned int*)malloc(sizeof(unsigned int) * NUM_ID);
	unsigned int* df2 = (unsigned int*)malloc(sizeof(unsigned int) * NUM_ID);
	unsigned int n = 0;
	unsigned int max = 0;//����ͼ������ID
	if (fopen_s(&fin, "test_data.txt", "rb") != 0) {
		printf("error open test_data.txt\n");
		return 0;
	}
	if (fin == 0) return 0;

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


	//��������ͼ(�ڽӾ���)
	/*�ڽӾ���
	��ţ����û�ID��������ʼ���ʱ�俪��
	��0�У��ö���ĳ��ȣ����ID�˺Ÿ����ٸ���ת�ˣ�
	��n�У�n>=1�����ö���ĳ����ڽӽ��
	*/
	unsigned int G[NUM_ID][9];
	unsigned int ID;
	for (unsigned int i = 0; i <= max; i++) {
		G[i][0] = 0;//���г��ȳ�ʼ��Ϊ0
		visited[i] = -1;//���Ҳ��ʼ�����ʼ�¼����
	}
	for (unsigned int i = 0; i < n; i++) {
		ID = df1[i];
		G[ID][G[ID][0]+1] = df2[i];
		G[ID][0]++;//���ȼ�һ
	}
	free(df1);
	free(df2);

	//�ҳ����л�
	unsigned int numcirl=0;//����������
	for (unsigned int i = 0; i <= max; i++) {
		circle[numcirl].length = 0;//��ʼ������
		if (G[i][0] == 0) continue;
		circle[numcirl].data[circle[numcirl].length++] = i;
		visited[i] = numcirl;//���ʼǺ�
		if (DFS(G, G[i][1], numcirl)) numcirl++;//���ɻ���������һ
		else {
			visited[i] = -1;//���ɻ���ɾ�����ʼǺ�
			circle[numcirl].length = 0;
		}
	}

	//��������
	unsigned int tmp[7], len, minID, flag, *sort;
	for (unsigned int i = 0; i < numcirl; i++) {
		//�ø��Ӽ�̵ı������棬������Ӽ��
		sort = &circle[i].data[0];
		len = circle[i].length;
		minID = *sort;
		flag = 0;
		for (unsigned int j = 1; j < len; j++)//����С
			if (*(sort+j) < minID) {
				minID = *(sort+j);
				flag = j;//flag����СID�ڻ��ڵ�λ��
			} 
		if (flag == 0)continue;//��˳����ȷ���õ���
		//��˳����ȷ������λ�ƣ�flag+j������λ
		for (unsigned int j = 0; j < len; j++)
			tmp[j] = *(sort + j);
		for (unsigned int j = 0; j < len; j++)
			*(sort + j) = tmp[(flag + j) % len];
	}

	//��������Ű����ĳ����ռ���sortidx���ô��ǣ����öԻ������ƶ���ֻ��Ҫ������Ӧ�����(idx)�ź��򼴿�;
	unsigned int* sortidx, head = 0, rear = numcirl - 1;
	unsigned int num_3, num_4, num_5, num_6, num_7;//����Ϊn(3,4...7)�Ļ��ĸ���
	sortidx = (unsigned int*)malloc(sizeof(unsigned int) * numcirl);
	for (unsigned int j= 0; j < numcirl; j++) {//�������򣬳���Ϊ3��7
		if (circle[j].length == 3) sortidx[head++] = j;
		if (circle[j].length == 7) sortidx[rear--] = j;
	}
	num_3 = head;
	num_7 = numcirl - 1 - rear;
	for (unsigned int j = 0; j < numcirl; j++) {//�������򣬳���Ϊ4��5
		if (circle[j].length == 4) sortidx[head++] = j;
		if (circle[j].length == 6) sortidx[rear--] = j;
	}
	num_4 = head - num_3;
	num_6 = numcirl - 1 - rear - num_7;
	for (unsigned int j = 0; j < numcirl; j++)  //�������򣬳���Ϊ6
		if (circle[j].length == 5) sortidx[head++] = j;
	num_5 = head - num_3 - num_4;
	//for (unsigned int j = 0; j < numcirl; j++) cout << sortidx[j] << endl;
	//cout << num_3 << endl << num_4 << endl << num_5 << endl << num_6 << endl << num_7 << endl;

	//��sortidx�е�������ν���������ļ�
	FILE* fout = NULL;
	if (fopen_s(&fout, "myresult.txt", "wb") != 0) {
		printf("error open result.txt\n");
		return 0;
	}
	if (fout == 0) return 0;
	unsigned int idx;
	fprintf(fout, "%u\n", numcirl);
	for (unsigned int i = 0, j; i < numcirl; i++) {
		idx = sortidx[i];
		for (j = 0; j < circle[idx].length - 1; j++) {
			fprintf(fout, "%u,", circle[idx].data[j]);
		}
		fprintf(fout, "%u\n", circle[idx].data[j]);//ÿ������������
	}
	                     
	//��������ʱ��
	finish = clock();
	cout << "ʱ��: " << finish - start << "ms\n"; 
}

