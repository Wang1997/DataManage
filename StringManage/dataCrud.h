#pragma once
#include <string.h>
#include <malloc.h>

//�洢��������
typedef char Type, *PType;

//���ݽڵ�
typedef struct node
{
    int length = 0; //��ռ�ռ䳤��
    int size = 0; //�ڵ��ܴ�С
}Node, *PNode;

//���ݿ���Ϣ
typedef struct chunkInfo
{
    int startIndex = 0; //�����ʼ����
    int totalSize = 0; //����ܴ�С
    int nodeNum = 0; //���нڵ����
    PNode nodeArr = NULL; //���нڵ�����
}ChunkInfo, *PChunkInfo;

#define BUF_LENGTH 0x5000
#define MAX_INPUT_LENGTH 30
#define LETTERS_NUM 26
#define SIZE_DIFF 5


/*************************interface*****************************/


// �����ʼ������
void init();

// �˳���������,���ͷ��ڴ��
void quit();

// ���������
int checkAddInput(int inputNum);

// �������
void addInputData(int index, PType pData);

// ��ȡ����������±�
int getDataBufIndex(int chunkIndex, int nodeIndex);

// ��ȡ��������ĳ���
int getDataBufLength(int chunkIndex, int nodeIndex);

// ���һ���µĴ洢��
int addNewChunk(int inputNum, int startIndex);

// ��ȡһ��洢λ��
int getStorageInfo(int inputNum, int *chunkIndex, int *nodeIndex);

//����Id����
int findById(int dataId, int *chunkIndex, int *nodeIndex);

// ����ƥ��
int dataCompare(int dataIndex, int dataLen, PType content, int conLen);

//�������ݲ���
int findByContent(PType content, int length);

//ȷ�Ϻ˶Լ��
int checkConfirmInput(char confirm);

//ɾ������
int deleteDataBuf(int chunkIndex, int nodeIndex);

//��������
int updateDataBuf(int chunkIndex, int nodeIndex, PType content, int conLen);

//ͳ�Ƶ���
int statisticalWords(int *statisticalArr);

/*
*չʾ����
*�ӿ����������,Ӧ�÷�������
*/
void showAllData();
