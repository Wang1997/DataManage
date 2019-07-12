#include "dataCrud.h"

//数据节点
typedef struct node
{
    int index = 0; //对应数据源索引
    int length = 0;//所占空间长度
    int size = 0; //节点总大小
}Node, *PNode;

//数据块信息
typedef struct chunkInfo
{
    int totalSize = 0; //块的总大小
    int nodeNum = 0; //块中节点个数
    PNode nodeArr = NULL; //块中节点数据
}ChunkInfo, *PChunkInfo;

//数据存放指定数组
static Type g_dataBuf[BUF_LENGTH];
//数据信息记录数组
static ChunkInfo g_chunkInfoArr[BUF_LENGTH];
//数据信息记录块个数
static int g_chunkNum = 0;

//数据遍历迭代索引
static int g_iteator_chunkIndex = 0;
static int g_iteator_nodeIndex = 0;

//统计结果数组
static int statisticalArr[LETTERS_NUM*2];

/*************************static function*****************************/

// 添加一个新的存储块
static int addNewChunk(int dataLength)
{
    int newIndex = 0;
    if (g_chunkNum != 0)
    {
        newIndex = g_chunkInfoArr[g_chunkNum - 1].nodeArr[0].index +
            g_chunkInfoArr[g_chunkNum - 1].totalSize;
    }

    if (dataLength > BUF_LENGTH - newIndex)
    {// 数据空间不够
        return 0;
    }
    //初始化块
    g_chunkInfoArr[g_chunkNum].totalSize = dataLength;
    g_chunkInfoArr[g_chunkNum].nodeNum = 1;
    g_chunkInfoArr[g_chunkNum].nodeArr = (PNode)malloc(sizeof(Node) * dataLength);
    if (!g_chunkInfoArr[g_chunkNum].nodeArr)
    {// 内存分配错误
        return 0;
    }
    //初始化节点
    memset(g_chunkInfoArr[g_chunkNum].nodeArr, 0, sizeof(Node) * dataLength);
    g_chunkInfoArr[g_chunkNum].nodeArr[0].index = newIndex;
    g_chunkInfoArr[g_chunkNum].nodeArr[0].size = dataLength;
    g_chunkNum++;

    return 1;
}

// 更新索引
static void updateIndex(int chunkIndex, int nodeIndex)
{
    if (nodeIndex <= 0)
    {
        nodeIndex = 1;
    }
    //更新索引
    for (; nodeIndex < g_chunkInfoArr[chunkIndex].nodeNum; ++nodeIndex)
    {
        g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].index =
            g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex-1].index +
            g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex-1].size;
    }
}

//索引值递增1
static void addIteatorIndex()
{
    g_iteator_nodeIndex++;
    if (g_iteator_nodeIndex >= g_chunkInfoArr[g_iteator_chunkIndex].nodeNum)
    {
        g_iteator_chunkIndex++;
        g_iteator_nodeIndex = 0;
    }
}

//数据匹配
static int dataCompare(int dataIndex, int dataLen, PType content, int conLen)
{
    if (conLen > dataLen)
    {
        return 0;
    }

    for (int i = dataIndex; i < dataIndex + dataLen; ++i)
    {
        int flag = 1;
        int index = i;
        
        for (int j = 0; j < conLen; ++j)
        {
            int compareSize = sizeof(Type);
            //取类型后进行字节比较
            char *src = (char*)(g_dataBuf+index);
            char *compare = (char*)(content+j);
            while (compareSize > 0)
            {
                if (*src++ != *compare++)
                {
                    flag = 0;
                    break;
                }
                compareSize--;
            }
            ++index;
            if(!flag)
                break;
        }
        if (flag) //匹配成功
            return 1;
    }

    return 0;
}

// 获取有效的节点数量
static int getValidNodeNum()
{
    int totalNum = 0;
    //遍历块 
    for (int i = 0; i < g_chunkNum; ++i)
    {
        //遍历块中节点
        for (int j = 0; j < g_chunkInfoArr[i].nodeNum; ++j)
        {
            //长度不为0说明是有效数据
            if(g_chunkInfoArr[i].nodeArr[j].length)
                totalNum++;
        }
    }

    return totalNum;
    
}

/*************************interface*****************************/


//初始化
void init()
{
    memset(g_dataBuf, 0, sizeof(Type) * BUF_LENGTH);
    memset(g_chunkInfoArr, 0, sizeof(ChunkInfo) * BUF_LENGTH);
}

//退出
void quit()
{
    //释放堆内存
    for (int i = 0; i < g_chunkNum; ++i)
    {
        free(g_chunkInfoArr[i].nodeArr);
    }
}

// 检测索引数据有效性
int checkIndexInfo(PIndexInfo pIndexInfo)
{
    int chunkIndex = pIndexInfo->chunkIndex;
    int nodeIndex = pIndexInfo->nodeIndex;
    if (chunkIndex < 0 || chunkIndex >= g_chunkNum)
    {
        return 0;
    }

    int nodeNum = g_chunkInfoArr[chunkIndex].nodeNum;
    if (nodeIndex < 0 || nodeIndex >= nodeNum)
    {
        return 0;
    }

    return 1;
}

// 添加数据
int addInputData(PIndexInfo pIndexInfo, PType pData, int dataLength)
{
    int chunkIndex = pIndexInfo->chunkIndex;
    int nodeIndex = pIndexInfo->nodeIndex;
    //检查数据有效性
    if (!checkIndexInfo(pIndexInfo))
    {
        return 0;
    }
    //检查长度
    if (dataLength > g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].size)
    {
        return 0;
    }

    int index = g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].index;
    memcpy(g_dataBuf+index, pData, sizeof(Type)*dataLength);//拷贝数据
    g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].length = dataLength;

    return 1;
}

//获取一块存储位置
IndexInfo getStorageInfo(int dataLength)
{
    int findFlag = 0;
    int space = 0;
    IndexInfo retIndex;
    retIndex.chunkIndex = -1;
    retIndex.nodeIndex = -1;

    if (dataLength <= 0)
    {
        return retIndex;
    }

    //遍历块 
    for (int i = 0; i < g_chunkNum; ++i)
    {
        //遍历块中节点
        for (int j = 0; j < g_chunkInfoArr[i].nodeNum; ++j)
        {

            space = g_chunkInfoArr[i].nodeArr[j].size -
                g_chunkInfoArr[i].nodeArr[j].length; //空闲区域
            if (space >= dataLength)// 找到符合条件的节点
            {
                retIndex.chunkIndex = i;
                retIndex.nodeIndex = j;
                findFlag = 1;
                break;
            }
        }
        if (findFlag)
        { //找到了提早退出
            break;
        }
    }

    int chunkIndex = retIndex.chunkIndex;
    int nodeIndex = retIndex.nodeIndex;

    if (!findFlag)
    {//开辟新块
        if (!addNewChunk(dataLength))
        {
            return retIndex;
        }
        retIndex.chunkIndex = g_chunkNum - 1;
        retIndex.nodeIndex = 0;
    }
    else if (g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].length)
    {//原先有数据,需开辟新节点 扩展往后
        int cpySize = (g_chunkInfoArr[chunkIndex].nodeNum - nodeIndex - 1)
                                                        * sizeof(Node);
        if (cpySize > 0) //越界考虑
        {
            memmove(&g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex + 2],
                &g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex + 1],
                cpySize); //整体往后移动一个
        }

        //割空间
        g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].size =
            g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].length;
        //初始化新节点
        nodeIndex += 1;
        g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].size = space;
        g_chunkInfoArr[chunkIndex].nodeNum++; //节点个数++;
        updateIndex(chunkIndex, nodeIndex); //更新索引
        retIndex.nodeIndex = nodeIndex;
    }

    return retIndex;
}

//重置索引迭代器
void resetIteatorIndex()
{
    g_iteator_chunkIndex = 0;
    g_iteator_nodeIndex = 0;
}

//判断是否有数据
int hasNextIndexInfo()
{
    if (g_iteator_chunkIndex < 0 || g_iteator_chunkIndex >= g_chunkNum)
    {
        return 0;
    }
    if (g_iteator_nodeIndex < 0 || g_iteator_nodeIndex >= 
                                   g_chunkInfoArr[g_iteator_chunkIndex].nodeNum)
    {
        return 0;
    }
    return 1;
}

//获取索引结构体数据
IndexInfo getNextIndexInfo()
{
    IndexInfo retIndex;
    retIndex.chunkIndex = -1;
    retIndex.nodeIndex = -1;

    if (!hasNextIndexInfo())
    {
        return retIndex;
    }
    retIndex.chunkIndex = g_iteator_chunkIndex;
    retIndex.nodeIndex = g_iteator_nodeIndex;

    //下一个
    addIteatorIndex();
    
    return retIndex;
}

// 获取源数据的长度
int getDataBufLength(PIndexInfo pIndexInfo)
{
    int chunkIndex = pIndexInfo->chunkIndex;
    int nodeIndex = pIndexInfo->nodeIndex;

    //检查数据有效性
    if (!checkIndexInfo(pIndexInfo))
    {
        return 0;
    }

    return g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].length;
}

// 获取源数据的占用空间
int getDataBufSize(PIndexInfo pIndexInfo)
{
    int chunkIndex = pIndexInfo->chunkIndex;
    int nodeIndex = pIndexInfo->nodeIndex;

    //检查数据有效性
    if (!checkIndexInfo(pIndexInfo))
    {
        return 0;
    }

    return g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].size;
}

//根据索引获取源数据
Type getDataByIndex(PIndexInfo pIndexInfo, int index)
{
    Type data = (Type)0;
    int chunkIndex = pIndexInfo->chunkIndex;
    int nodeIndex = pIndexInfo->nodeIndex;

    //检查数据有效性
    if (!checkIndexInfo(pIndexInfo))
    {
        return data;
    }

    if (index < 0 || index >= g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].length)
    {
        return data;
    }

    return g_dataBuf[g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].index + index];
}

//根据Id查找
IndexInfo findById(int dataId)
{
    IndexInfo retIndex;
    retIndex.chunkIndex = -1;
    retIndex.nodeIndex = -1;
    int calcId = 0;

    if (dataId <= 0) //输入数据检查
        return retIndex;

    //遍历块 
    for (int i = 0; i < g_chunkNum; ++i)
    {
        calcId += g_chunkInfoArr[i].nodeNum;
        if (calcId >= dataId)
        {
            //下标需-1
            int nodeIndex = dataId - (calcId - g_chunkInfoArr[i].nodeNum) - 1;
            // 判断数据有效性
            if (!g_chunkInfoArr[i].nodeArr[nodeIndex].length)
                return retIndex;

            retIndex.chunkIndex = i;
            retIndex.nodeIndex = nodeIndex;
            
            return retIndex;
        }
    }
    return retIndex;
}

//根据索引获取数据Id
int getDataIdByIndexInfo(PIndexInfo pIndexInfo)
{
    int dataId = 0;
    int chunkIndex = pIndexInfo->chunkIndex;
    int nodeIndex = pIndexInfo->nodeIndex;

    //检查数据有效性
    if (!checkIndexInfo(pIndexInfo))
    {
        return dataId;
    }

    //遍历块 
    for (int i = 0; i < chunkIndex; ++i)
    {
        dataId += g_chunkInfoArr[i].nodeNum;
    }
    dataId += nodeIndex +1;

    return dataId;
}

//根据内容查找
IndexInfo getNextFindByContent(PType content, int length)
{
    IndexInfo retIndex;
    retIndex.chunkIndex = -1;
    retIndex.nodeIndex = -1;

    //遍历
    while (hasNextIndexInfo())
    {
        int dataIndex = g_chunkInfoArr[g_iteator_chunkIndex].nodeArr[g_iteator_nodeIndex].index;
        int dataLength = g_chunkInfoArr[g_iteator_chunkIndex].nodeArr[g_iteator_nodeIndex].length;
        if (dataCompare(dataIndex, dataLength, content, length))
        {
            retIndex.chunkIndex = g_iteator_chunkIndex;
            retIndex.nodeIndex = g_iteator_nodeIndex;
            addIteatorIndex(); //必须先迭代器++
            return retIndex;  //查找到退出
        }
        addIteatorIndex();
    }

    return retIndex;
}

// 删除数据
int deleteDataBuf(PIndexInfo pIndexInfo)
{
    int chunkIndex = pIndexInfo->chunkIndex;
    int nodeIndex = pIndexInfo->nodeIndex;

    //检查数据有效性
    if (!checkIndexInfo(pIndexInfo))
    {
        return 0;
    }

    //删除
    g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].length = 0;
    
    // 节点间进行合并
    {
        //块内最近节点索引合并(约定往前)
        //判断后一个合并
        int nextIndex = nodeIndex + 1;
        if (nextIndex < g_chunkInfoArr[chunkIndex].nodeNum)
        {
            if (g_chunkInfoArr[chunkIndex].nodeArr[nextIndex].length == 0)
            {
                g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].size +=
                    g_chunkInfoArr[chunkIndex].nodeArr[nextIndex].size;
                //拷贝数据
                int cpySize = (g_chunkInfoArr[chunkIndex].nodeNum - nextIndex - 1)
                    * sizeof(Node);
                if (cpySize > 0) //越界考虑
                {
                    memmove(&g_chunkInfoArr[chunkIndex].nodeArr[nextIndex],
                        &g_chunkInfoArr[chunkIndex].nodeArr[nextIndex + 1],
                        cpySize); //整体往前移动一个
                    updateIndex(chunkIndex, nextIndex); //更新索引
                }
                g_chunkInfoArr[chunkIndex].nodeNum--;
            }
        }

        //判断前一个合并
        int preIndex = nodeIndex - 1;
        if (preIndex >= 0)
        {
            if (g_chunkInfoArr[chunkIndex].nodeArr[preIndex].length == 0)
            {
                g_chunkInfoArr[chunkIndex].nodeArr[preIndex].size +=
                    g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].size;
                //拷贝数据
                int cpySize = (g_chunkInfoArr[chunkIndex].nodeNum - nodeIndex - 1)
                    * sizeof(Node);
                if (cpySize > 0) //越界考虑
                {
                    memmove(&g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex],
                        &g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex + 1],
                        cpySize); //整体往前移动一个
                    updateIndex(chunkIndex, nodeIndex); //更新索引
                }
                g_chunkInfoArr[chunkIndex].nodeNum--;
            }
        }
    }

    //块间进行合并
    {
        //如果当前块只有一个节点，并且该节点无内容 考虑块间合并
        if (g_chunkInfoArr[chunkIndex].nodeNum == 1 &&
            g_chunkInfoArr[chunkIndex].nodeArr[0].length == 0)
        {
            // 判断后一个进行合并
            int nextChunkIndex = chunkIndex + 1;
            if (nextChunkIndex < g_chunkNum &&
                g_chunkInfoArr[nextChunkIndex].nodeNum == 1 &&
                g_chunkInfoArr[nextChunkIndex].nodeArr[0].length == 0)
            {
                g_chunkInfoArr[chunkIndex].totalSize +=
                    g_chunkInfoArr[nextChunkIndex].totalSize;
                g_chunkInfoArr[chunkIndex].nodeArr[0].size +=
                    g_chunkInfoArr[nextChunkIndex].nodeArr[0].size;
                free(g_chunkInfoArr[nextChunkIndex].nodeArr);//释放内存
                for (int i = nextChunkIndex + 1; i < g_chunkNum; ++i)
                { //拷贝
                    g_chunkInfoArr[i-1] = g_chunkInfoArr[i];
                }
                --g_chunkNum;
            }

            //判断前一个合并
            int preIndex = chunkIndex - 1;
            if (preIndex >= 0 &&
                g_chunkInfoArr[preIndex].nodeNum == 1 &&
                g_chunkInfoArr[preIndex].nodeArr[0].length == 0)
            {
                g_chunkInfoArr[preIndex].totalSize +=
                    g_chunkInfoArr[chunkIndex].totalSize;
                g_chunkInfoArr[preIndex].nodeArr[0].size +=
                    g_chunkInfoArr[chunkIndex].nodeArr[0].size;
                free(g_chunkInfoArr[chunkIndex].nodeArr);//释放内存
                for (int i = chunkIndex + 1; i < g_chunkNum; ++i)
                { //拷贝
                    g_chunkInfoArr[i - 1] = g_chunkInfoArr[i];
                }
                --g_chunkNum;
            }
        }
    }


    return 1;
}

//更新数据
int updateDataBuf(PIndexInfo pIndexInfo, PType content, int conLen)
{

    int chunkIndex = pIndexInfo->chunkIndex;
    int nodeIndex = pIndexInfo->nodeIndex;

    //检查数据有效性
    if (!checkIndexInfo(pIndexInfo))
    {
        return 0;
    }

    int index = g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].index;
    g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].length = conLen;
    //更新源数据
    memcpy(&g_dataBuf[index], content, sizeof(Type) * conLen);

    return 1;
}

//统计单词
int* statisticalWords()
{

    memset(statisticalArr,0,sizeof(int) * LETTERS_NUM * 2);

    int totalLetters = 0;
    //遍历块 
    for (int i = 0; i < g_chunkNum; ++i)
    {
        
        //遍历块中节点
        for (int j = 0; j < g_chunkInfoArr[i].nodeNum; ++j)
        {
            int index = g_chunkInfoArr[i].nodeArr[j].index;
            //遍历真实数据
            for (int k = 0; k < g_chunkInfoArr[i].nodeArr[j].length; ++k)
            {
                Type data = g_dataBuf[index + k];
                if (data >= 'a' && data <= 'z')
                {
                    statisticalArr[data - 'a']++;
                }
                else if (data >= 'A' && data <= 'Z')
                {
                    statisticalArr[data - 'A' + LETTERS_NUM]++;
                }
            }
        }
    }

    return statisticalArr;
}

//获取数据总共个数
int getDataTotalNum()
{
    int totalNum = 0;
    //遍历块 
    for (int i = 0; i < g_chunkNum; ++i)
    {
        //遍历块中节点
        for (int j = 0; j < g_chunkInfoArr[i].nodeNum; ++j)
        {
            totalNum += g_chunkInfoArr[i].nodeArr[j].length;
        }
    }

    return totalNum;
}

//碎片整理
int defragment()
{
    //获取有效节点数量,用于分配空间
    int validNodeNum = getValidNodeNum();
    PNode nodeArr = NULL;
    if (validNodeNum > 0)
    {
        nodeArr = (PNode)malloc(sizeof(Node) * validNodeNum);
        if (nodeArr == NULL)
        { //内存申请失败
            return 0;
        }
    }

    //拷贝数据和释放旧数据内存
    int nodeId = 0;
    //遍历块 
    for (int i = 0; i < g_chunkNum; ++i)
    {
        //遍历块中节点
        for (int j = 0; j < g_chunkInfoArr[i].nodeNum; ++j)
        {
            if (g_chunkInfoArr[i].nodeArr[j].length)
            {//进行复制有效数据
                nodeArr[nodeId++] = g_chunkInfoArr[i].nodeArr[j];
            }
        }
        free(g_chunkInfoArr[i].nodeArr); //释放内存
    }

    //初始化索引表
    memset(g_chunkInfoArr, 0, sizeof(ChunkInfo) * BUF_LENGTH);
    g_chunkNum = 0;
    //根据节点数据移动源数据并更新索引表
    int dataIndex = 0;
    for (nodeId = 0; nodeId < validNodeNum; ++nodeId)
    {
        //移动源数据
        int cpySize = nodeArr[nodeId].length * sizeof(Type);
        memmove(&g_dataBuf[dataIndex],&g_dataBuf[nodeArr[nodeId].index]
                                                ,cpySize);
        dataIndex += nodeArr[nodeId].length;
        //更新对应索引表
        addNewChunk(nodeArr[nodeId].length);
        g_chunkInfoArr[g_chunkNum-1].nodeArr[0].length = nodeArr[nodeId].length;
    }

    if (validNodeNum > 0)
    {
        free(nodeArr); //释放内存
    }
   
    return 1;
}
