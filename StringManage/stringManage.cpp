#include "stringManage.h"


//数据存放指定数组
Type g_dataBuf[BUF_LENGTH];
//数据信息记录数组
ChunkInfo g_chunkInfoArr[BUF_LENGTH];

int g_chunkNum = 0;//数据信息记录块个数

int main(int argc, char *argv[])
{
    init();
    menuInput();
    quit();
    system("pause");
    return 0;
}

/*************************logic*****************************/

//初始化
void init()
{
    memset(g_dataBuf,0,sizeof(Type) * BUF_LENGTH);
    memset(g_chunkInfoArr,0,sizeof(ChunkInfo) * BUF_LENGTH);
}

//退出
void quit()
{
    
}

// 添加输入检查
int checkAddInput(int inputNum)
{
    if(inputNum <= 0 || inputNum > MAX_INPUT_LENGTH)
        return 0;
    return 1;
}

// 添加数据
void addInputData(int index,PType pData)
{
    memcpy(&g_dataBuf[index], pData, sizeof(Type));
}

// 获取数据数组的下标
int getDataBufIndex(int chunkIndex,int nodeIndex)
{
    int index = g_chunkInfoArr[chunkIndex].startIndex;
    for (int i = 0; i < nodeIndex; ++i)
    {
        index += g_chunkInfoArr[chunkIndex].nodeArr[i].size;
    }
    return index;
}

// 添加一个新的存储块
int addNewChunk(int inputNum,int startIndex)
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
int getStorageInfo(int inputNum,int *chunkIndex,int *nodeIndex)
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
        if (!addNewChunk(inputNum,totalSize))
        {
            return 0;
        }
        *chunkIndex = g_chunkNum - 1;
        *nodeIndex = 0;
        //初始化新节点
        g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].length = inputNum;
        g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].size = inputNum;
    }
    else
    {//开辟新节点
        for (int j = g_chunkInfoArr[*chunkIndex].nodeNum; j > *nodeIndex + 1; --j)
        {
            g_chunkInfoArr[*chunkIndex].nodeArr[j] = 
                            g_chunkInfoArr[*chunkIndex].nodeArr[j-1];
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
    return 1;
}

//展示所有数据
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
            if (!g_chunkInfoArr[i].nodeArr[j].length) //已删除
                continue;
            printData(dataId, index, g_chunkInfoArr[i].nodeArr[j].length);
            index += g_chunkInfoArr[i].nodeArr[j].size;
        }
    }
    
}



/**************************view*****************************/

//菜单界面
void menuInput()
{
    while (1)
    {
        system("cls");
        printf("  字符串管理系统  \r\n");
        printf("1.增加功能\r\n");
        printf("2.删除功能\r\n");
        printf("3.修改功能\r\n");
        printf("4.查询功能\r\n");
        printf("5.统计功能\r\n");
        printf("6.存储信息功能\r\n");
        printf("7.退出\r\n");
        printf("请选择: ");
        int choose;
        scanf("%d",&choose);
        switch (choose)
        {
            case 1:
            {
                addInput();
                break;
            }
            case 2:
            {
                deleteInput();
                break;
            }
            case 3:
            {
                break;
            }
            case 4:
            {
                queryInput();
                break;
            }
            case 5:
            {
                break;
            }
            case 6:
            {
                break;
            }
            case 7:
            {
                return;
                break;
            }
            default:
            {
                printf("对不起，输入有误，请重新选择\r\n");
                break;
            }
                
        }
        system("pause");        
    }
}

// 增加数据输入
void addInput()
{
    int inputNum = 0;
    printf("请输入需要添加的字节个数：");
    scanf("%d", &inputNum);
    if (!checkAddInput(inputNum))
    {
        printf("对不起,输入的范围有误\r\n");
        return;
    }
    int chunkIndex = 0;
    int nodeIndex = 0;
    //获取存储信息
    if (!getStorageInfo(inputNum, &chunkIndex, &nodeIndex))
    {
        printf("对不起,剩余空间不够\r\n");
        return;
    }
    rewind(stdin);
    printf("请输入数据：");
    int i = 0;
    Type data;
    //获取数据存储开始下标
    int index = getDataBufIndex(chunkIndex, nodeIndex);
    while (i < inputNum)
    {
        data = getchar();
        addInputData(index+i,&data);
        i++;
    }
    printf("添加成功\r\n");
    rewind(stdin);
    return;
}

//删除输入
void deleteInput()
{
    int dataId = 0;
    printf("请输入需要删除的数据Id：");
    scanf("%d", &dataId);

}

//查询界面
void queryInput()
{
    while (1)
    {
        system("cls");
        printf("  查询功能  \r\n");
        printf("1.查询所有数据\r\n");
        printf("2.根据内容查找\r\n");
        printf("3.根据Id查找\r\n");
        printf("请选择: ");
        int choose;
        scanf("%d", &choose);
        switch (choose)
        {
            case 1:
            {
                system("cls");
                showAllData();
                return;
            }
            case 2:
            {
                findByContentInput();
                return;
            }
            case 3:
            {
                return;
            }
            default:
            {
                printf("对不起，输入有误，请重新选择\r\n");
                break;
            }
        }
    }
}

//内容查找 输入
void findByContentInput()
{

}

//打印某一条数据
void printData(int dataId, int index, int length)
{
    printf("%d. ", dataId);
    for(int i = 0;i < length;++i)
        printf("%c",g_dataBuf[index+i]);
    printf("\r\n");
}
