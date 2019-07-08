#pragma once
#include <stdio.h>
#include <stdlib.h>

// 菜单界面
void menuView();

// 增加数据输入
void addInput();

//删除输入
void deleteInput();

//修改输入
void updateInput();

//查询界面
void queryView();

//内容查找 输入
void findByContentInput();

//Id查找 输入
int findByIdInput(int *chunkIndex, int *nodeIndex);

//统计单词界面
void statisticalWordsView();

//存储信息界面
void storeInfoView();

//打印某一条数据
void printData(int dataId,int index,int length);
