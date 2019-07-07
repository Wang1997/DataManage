#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

//�洢��������
typedef char Type,*PType;

//���ݽڵ�
typedef struct node
{
    int length = 0; //��ռ�ռ䳤��
    int size = 0; //�ڵ��ܴ�С
}Node,*PNode;

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
#define SIZE_DIFF 5


/*************************logic*****************************/


// �����ʼ������,������ڴ��
void init();

// �˳�����������,���ͷ��ڴ��
void quit();

// ����������
int checkAddInput(int inputNum);

// ��������
void addInputData(int index, PType pData);

// ��ȡ����������±�
int getDataBufIndex(int chunkIndex, int nodeIndex);

// ����һ���µĴ洢��
int addNewChunk(int inputNum, int startIndex);

// ��ȡһ��洢λ��
int getStorageInfo(int inputNum, int *chunkIndex, int *nodeIndex);

//չʾ��������
void showAllData();

/**************************view*****************************/


// �˵�����
void menuInput();

// ������������
void addInput();

//ɾ������
void deleteInput();

//��ѯ����
void queryInput();

//���ݲ��� ����
void findByContentInput();

//��ӡĳһ������
void printData(int dataId,int index,int length);