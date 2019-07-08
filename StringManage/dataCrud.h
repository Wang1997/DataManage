#pragma once
#include <string.h>
#include <malloc.h>

//存储数据类型
typedef char Type, *PType;

//数据节点
typedef struct node
{
    int length = 0; //所占空间长度
    int size = 0; //节点总大小
}Node, *PNode;

//数据块信息
typedef struct chunkInfo
{
    int startIndex = 0; //块的起始索引
    int totalSize = 0; //块的总大小
    int nodeNum = 0; //块中节点个数
    PNode nodeArr = NULL; //块中节点数据
}ChunkInfo, *PChunkInfo;

#define BUF_LENGTH 0x5000
#define MAX_INPUT_LENGTH 30
#define LETTERS_NUM 26
#define SIZE_DIFF 5


/*************************interface*****************************/


// 程序初始化工作
void init();

// 退出程序处理工作,如释放内存等
void quit();

// 添加输入检查
int checkAddInput(int inputNum);

// 添加数据
void addInputData(int index, PType pData);

// 获取数据数组的下标
int getDataBufIndex(int chunkIndex, int nodeIndex);

// 获取数据数组的长度
int getDataBufLength(int chunkIndex, int nodeIndex);

// 添加一个新的存储块
int addNewChunk(int inputNum, int startIndex);

// 获取一块存储位置
int getStorageInfo(int inputNum, int *chunkIndex, int *nodeIndex);

//根据Id查找
int findById(int dataId, int *chunkIndex, int *nodeIndex);

// 数据匹配
int dataCompare(int dataIndex, int dataLen, PType content, int conLen);

//根据内容查找
int findByContent(PType content, int length);

//确认核对检查
int checkConfirmInput(char confirm);

//删除数据
int deleteDataBuf(int chunkIndex, int nodeIndex);

//更新数据
int updateDataBuf(int chunkIndex, int nodeIndex, PType content, int conLen);

//统计单词
int statisticalWords(int *statisticalArr);

/*
*展示数据
*接口设计有问题,应该返回数据
*/
void showAllData();
