#include "dataCrud.h"

//数据存放指定数组
static Type g_dataBuf[BUF_LENGTH];
//数据信息记录数组
static ChunkInfo g_chunkInfoArr[BUF_LENGTH];
//数据信息记录块个数
static int g_chunkNum = 0;


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

// 添加输入检查
int checkAddInput(int inputNum)
{
    if (inputNum <= 0 || inputNum > MAX_INPUT_LENGTH)
        return 0;
    return 1;
}

// 添加数据
void addInputData(int index, PType pData)
{
    memcpy(&g_dataBuf[index], pData, sizeof(Type));
}

// 获取数据数组的下标
int getDataBufIndex(int chunkIndex, int nodeIndex)
{
    int index = g_chunkInfoArr[chunkIndex].startIndex;
    for (int i = 0; i < nodeIndex; ++i)
    {
        index += g_chunkInfoArr[chunkIndex].nodeArr[i].size;
    }
    return index;
}

// 获取数据数组的长度
int getDataBufLength(int chunkIndex, int nodeIndex)
{
    return g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].length;
}

// 添加一个新的存储块
int addNewChunk(int inputNum, int startIndex)
{
    if (inputNum > BUF_LENGTH - startIndex)
    {// 空间不够
        return 0;
    }

    g_chunkInfoArr[g_chunkNum].startIndex = startIndex;
    g_chunkInfoArr[g_chunkNum].totalSize = inputNum;
    g_chunkInfoArr[g_chunkNum].nodeNum = 1;
    g_chunkInfoArr[g_chunkNum].nodeArr = (PNode)malloc(sizeof(Node) * inputNum);
    if (!g_chunkInfoArr[g_chunkNum].nodeArr)
    {// 内存分配错误
        return 0;
    }

    memset(g_chunkInfoArr[g_chunkNum].nodeArr, 0, sizeof(Node) * inputNum);
    g_chunkNum++;

    return 1;
}

//获取一块存储位置
int getStorageInfo(int inputNum, int *chunkIndex, int *nodeIndex)
{
    int totalSize = 0;
    int findFlag = 0;
    int space = 0;
    //遍历块 
    for (int i = 0; i < g_chunkNum; ++i)
    {
        //遍历块中节点
        for (int j = 0; j < g_chunkInfoArr[i].nodeNum; ++j)
        {

            space = g_chunkInfoArr[i].nodeArr[j].size -
                g_chunkInfoArr[i].nodeArr[j].length; //空闲区域
            if (space >= inputNum)// 找到符合条件的节点
            {
                *chunkIndex = i;
                *nodeIndex = j;
                findFlag = 1;
                break;
            }
        }
        totalSize += g_chunkInfoArr[i].totalSize;
        if (findFlag)
        { //找到了提早退出
            break;
        }
    }

    if (!findFlag)
    {//开辟新块
        if (!addNewChunk(inputNum, totalSize))
        {
            return 0;
        }
        *chunkIndex = g_chunkNum - 1;
        *nodeIndex = 0;
        //初始化新节点
        g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].length = inputNum;
        g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].size = inputNum;
    }
    else if (g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].length)
    {//原先有数据,需开辟新节点 扩展往后
        int cpySize = (g_chunkInfoArr[*chunkIndex].nodeNum - *nodeIndex - 1)
            * sizeof(Node);
        if (cpySize > 0) //越界考虑
        {
            memmove(&g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex + 2],
                &g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex + 1],
                cpySize); //整体往后移动一个
        }

        //割空间
        g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].size =
            g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].length;
        //初始化新节点
        *nodeIndex += 1;
        g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].length = inputNum;
        g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].size = space;
        g_chunkInfoArr[*chunkIndex].nodeNum++; //节点个数++;
    }
    else
    {
        g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].length = inputNum;
    }
    return 1;
}

//展示数据
void showAllData()
{

    int dataId = 0;
    //遍历块 
    for (int i = 0; i < g_chunkNum; ++i)
    {
        int index = g_chunkInfoArr[i].startIndex;
        //遍历块中节点
        for (int j = 0; j < g_chunkInfoArr[i].nodeNum; ++j)
        {
            ++dataId;
            if (g_chunkInfoArr[i].nodeArr[j].length) //非无效数据打印
            {
                //printData(dataId, index, g_chunkInfoArr[i].nodeArr[j].length);
            }
            index += g_chunkInfoArr[i].nodeArr[j].size;
        }
    }

}

//根据Id查找
int findById(int dataId, int *chunkIndex, int *nodeIndex)
{
    if (dataId <= 0) //输入数据检查
        return 0;

    int calcId = 0;
    //遍历块 
    for (int i = 0; i < g_chunkNum; ++i)
    {
        calcId += g_chunkInfoArr[i].nodeNum;
        if (calcId >= dataId)
        {
            *chunkIndex = i;
            *nodeIndex = dataId - (calcId - g_chunkInfoArr[i].nodeNum) - 1; //下标需-1
                                                                            // 判断数据有效性
            if (!g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].length)
                return 0;
            return 1;
        }
    }
    return 0;
}

// 数据匹配
int dataCompare(int dataIndex, int dataLen, PType content, int conLen)
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
            if (g_dataBuf[index] != content[j])
            {
                flag = 0;
                break;
            }
            ++index;
        }
        if (flag) //匹配成功
            return 1;
    }

    return 0;
}

//根据内容查找
int findByContent(PType content, int length)
{
    int dataId = 0;
    int findNum = 0;
    //遍历块 
    for (int i = 0; i < g_chunkNum; ++i)
    {
        int index = g_chunkInfoArr[i].startIndex;
        //遍历块中节点
        for (int j = 0; j < g_chunkInfoArr[i].nodeNum; ++j)
        {
            ++dataId;
            //匹配数据
            if (dataCompare(index, g_chunkInfoArr[i].nodeArr[j].length, content, length))
            {
                //printData(dataId, index, g_chunkInfoArr[i].nodeArr[j].length);
                ++findNum;
            }
            index += g_chunkInfoArr[i].nodeArr[j].size;
        }
    }
    return findNum;
}

//确认核对检查
int checkConfirmInput(char confirm)
{
    if (confirm != 'y' && confirm != 'Y')
        return 0;
    return 1;
}

// 删除数据
int deleteDataBuf(int chunkIndex, int nodeIndex)
{
    //删除
    g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].length = 0;

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
            }
            g_chunkInfoArr[chunkIndex].nodeNum--;
        }
    }

    return 1;
}

//更新数据
int updateDataBuf(int chunkIndex, int nodeIndex, PType content, int conLen)
{
    int index = getDataBufIndex(chunkIndex, nodeIndex);
    g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].length = conLen;
    //更新源数据
    memcpy(&g_dataBuf[index], content, sizeof(Type) * conLen);
    return 1;
}

//统计单词
int statisticalWords(int *statisticalArr)
{
    int totalLetters = 0;
    //遍历块 
    for (int i = 0; i < g_chunkNum; ++i)
    {
        int index = g_chunkInfoArr[i].startIndex;
        //遍历块中节点
        for (int j = 0; j < g_chunkInfoArr[i].nodeNum; ++j)
        {
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
            totalLetters += g_chunkInfoArr[i].nodeArr[j].length;
            index += g_chunkInfoArr[i].nodeArr[j].size;
        }
    }

    return totalLetters;
}
