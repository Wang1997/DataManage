#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataCrud.h"

#define MAX_INPUT_LENGTH 30

// 菜单界面
void menuView();

// 添加输入检查
int checkAddInput(int inputNum);

//确认核对检查
int checkConfirmInput(char confirm);

// 增加数据输入
void addInput();

//删除输入
void deleteInput();

//修改输入
void updateInput();

//查询界面
void queryView();

// 展示所有数据
void showAllData();

//内容查找 输入
void findByContentInput();

//Id查找 输入
void findByIdInput();

//统计单词界面
void statisticalWordsView();

//存储信息界面
void storeInfoView();

//展示存储资源图
void showStorageResource();

//打印某一条数据
void printData(PIndexInfo pIndexInfo);
