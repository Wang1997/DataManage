#pragma once
#include <stdio.h>
#include <stdlib.h>

// �˵�����
void menuView();

// ������������
void addInput();

//ɾ������
void deleteInput();

//�޸�����
void updateInput();

//��ѯ����
void queryView();

//���ݲ��� ����
void findByContentInput();

//Id���� ����
int findByIdInput(int *chunkIndex, int *nodeIndex);

//ͳ�Ƶ��ʽ���
void statisticalWordsView();

//�洢��Ϣ����
void storeInfoView();

//��ӡĳһ������
void printData(int dataId,int index,int length);
