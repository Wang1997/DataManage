#pragma once
#include <string.h>
#include <malloc.h>

//存储数据类型
typedef char Type, *PType;

//索引结构体
typedef struct indexInfo
{
    int chunkIndex = -1; //块索引
    int nodeIndex = -1; //节点索引
}IndexInfo,*PIndexInfo;

// 数据源最大存储长度
#define BUF_LENGTH 0x5000
// 单词个数(统计)
#define LETTERS_NUM 26


/*************************interface*****************************/


// 程序初始化工作
void init();

// 退出程序处理工作,如释放内存等
void quit();

// 检测索引数据有效性
int checkIndexInfo(PIndexInfo pIndexInfo);

// 添加数据
int addInputData(PIndexInfo pIndexInfo, PType pData,int dataLength);

// 获取一块存储位置
IndexInfo getStorageInfo(int dataLength);

//重置索引迭代器
void resetIteatorIndex();

//判断是否有数据
int hasNextIndexInfo();

//获取索引结构体数据
IndexInfo getNextIndexInfo();

// 获取源数据的长度
int getDataBufLength(PIndexInfo pIndexInfo);

// 获取源数据的占用空间
int getDataBufSize(PIndexInfo pIndexInfo);

//根据索引获取源数据
Type getDataByIndex(PIndexInfo pIndexInfo, int index);

//根据Id查找
IndexInfo findById(int dataId);

//根据索引获取数据Id
int getDataIdByIndexInfo(PIndexInfo pIndexInfo);

//根据内容查找
IndexInfo getNextFindByContent(PType content, int length);

//删除数据
int deleteDataBuf(PIndexInfo pIndexInfo);

//更新数据
int updateDataBuf(PIndexInfo pIndexInfo, PType content, int conLen);

//统计单词
int* statisticalWords();

//获取数据总共个数
int getDataTotalNum();

//碎片整理
int defragment();
