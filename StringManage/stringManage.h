#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataCrud.h"

#define MAX_INPUT_LENGTH 30

// �˵�����
void menuView();

// ���������
int checkAddInput(int inputNum);

//ȷ�Ϻ˶Լ��
int checkConfirmInput(char confirm);

// ������������
void addInput();

//ɾ������
void deleteInput();

//�޸�����
void updateInput();

//��ѯ����
void queryView();

// չʾ��������
void showAllData();

//���ݲ��� ����
void findByContentInput();

//Id���� ����
void findByIdInput();

//ͳ�Ƶ��ʽ���
void statisticalWordsView();

//�洢��Ϣ����
void storeInfoView();

//չʾ�洢��Դͼ
void showStorageResource();

//��ӡĳһ������
void printData(PIndexInfo pIndexInfo);
