#pragma once
#include <string.h>
#include <malloc.h>

//�洢��������
typedef char Type, *PType;

//�����ṹ��
typedef struct indexInfo
{
    int chunkIndex = -1; //������
    int nodeIndex = -1; //�ڵ�����
}IndexInfo,*PIndexInfo;

// ����Դ���洢����
#define BUF_LENGTH 0x5000
// ���ʸ���(ͳ��)
#define LETTERS_NUM 26


/*************************interface*****************************/


// �����ʼ������
void init();

// �˳���������,���ͷ��ڴ��
void quit();

// �������������Ч��
int checkIndexInfo(PIndexInfo pIndexInfo);

// �������
int addInputData(PIndexInfo pIndexInfo, PType pData,int dataLength);

// ��ȡһ��洢λ��
IndexInfo getStorageInfo(int dataLength);

//��������������
void resetIteatorIndex();

//�ж��Ƿ�������
int hasNextIndexInfo();

//��ȡ�����ṹ������
IndexInfo getNextIndexInfo();

// ��ȡԴ���ݵĳ���
int getDataBufLength(PIndexInfo pIndexInfo);

// ��ȡԴ���ݵ�ռ�ÿռ�
int getDataBufSize(PIndexInfo pIndexInfo);

//����������ȡԴ����
Type getDataByIndex(PIndexInfo pIndexInfo, int index);

//����Id����
IndexInfo findById(int dataId);

//����������ȡ����Id
int getDataIdByIndexInfo(PIndexInfo pIndexInfo);

//�������ݲ���
IndexInfo getNextFindByContent(PType content, int length);

//ɾ������
int deleteDataBuf(PIndexInfo pIndexInfo);

//��������
int updateDataBuf(PIndexInfo pIndexInfo, PType content, int conLen);

//ͳ�Ƶ���
int* statisticalWords();

//��ȡ�����ܹ�����
int getDataTotalNum();

//��Ƭ����
int defragment();
