#include "stringManage.h"


//数据存放指定数组
Type g_dataBuf[BUF_LENGTH];
//数据信息记录数组
ChunkInfo g_chunkInfoArr[BUF_LENGTH];

int g_chunkNum = 0;//数据信息记录块个数

int main(int argc, char *argv[])
{
    init();
    menuView();
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
    //释放堆内存
    for (int i = 0; i < g_chunkNum; ++i)
    {
        free(g_chunkInfoArr[i].nodeArr);
    }
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

// 获取数据数组的长度
int getDataBufLength(int chunkIndex, int nodeIndex)
{
    return g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].length;
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
    else if(g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].length)
    {//原先有数据,需开辟新节点 扩展往后
        int cpySize = (g_chunkInfoArr[*chunkIndex].nodeNum - *nodeIndex - 1) * sizeof(Node);
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
            if (g_chunkInfoArr[i].nodeArr[j].length) //非无效数据打印
            {
                printData(dataId, index, g_chunkInfoArr[i].nodeArr[j].length);
            }
            index += g_chunkInfoArr[i].nodeArr[j].size;
        }
    }
    
}

//根据Id查找
int findById(int dataId, int *chunkIndex, int *nodeIndex)
{
    if(dataId <= 0) //输入数据检查
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

//确认核对检查
int checkConfirmInput(char confirm) 
{
    if(confirm != 'y' && confirm != 'Y')
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
            int cpySize = (g_chunkInfoArr[chunkIndex].nodeNum - nextIndex - 1) * sizeof(Node);
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
            int cpySize = (g_chunkInfoArr[chunkIndex].nodeNum - nodeIndex - 1) * sizeof(Node);
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

/**************************view*****************************/

//菜单界面
void menuView()
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
                queryView();
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
    int chunkIndex = 0;
    int nodeIndex = 0;
    if (!findByIdInput(&chunkIndex, &nodeIndex))
    {
        return;
    }
    printf("请确认是否删除(y or n)：");
    rewind(stdin);
    char confirm;
    confirm = getchar();
    rewind(stdin);
    if (!checkConfirmInput(confirm))
    {//不删除
        printf("该数据未删除\r\n");
        return; 
    }
        
    if (deleteDataBuf(chunkIndex, nodeIndex))
    {
        printf("删除成功\r\n");
    }
}

//查询界面
void queryView()
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
                int chunkIndex = 0;
                int nodeIndex = 0;
                findByIdInput(&chunkIndex,&nodeIndex);
                return;
            }
            default:
            {
                printf("对不起，输入有误，请重新选择\r\n");
                system("pause");
                break;
            }
        }
    }
}


//内容查找 输入
void findByContentInput()
{
    printf("请输入需要查找内容");
}


//Id查找 输入
int findByIdInput(int *chunkIndex,int *nodeIndex)
{
    int dataId;
    printf("请输入Id:");
    scanf("%d",&dataId);
    
    if (!findById(dataId, chunkIndex, nodeIndex))
    {
        printf("对不起,无该Id数据(可能已删除),请确认Id信息\r\n");
        return 0;
    }

    //获取数据存储下标和长度
    int index = getDataBufIndex(*chunkIndex, *nodeIndex);
    int length = getDataBufLength(*chunkIndex, *nodeIndex);
    printf("查找结果: ");
    printData(dataId,index,length);
    return 1;
}


//打印某一条数据
void printData(int dataId, int index, int length)
{
    printf("(%d). ", dataId);
    for(int i = 0;i < length;++i)
        printf("%c",g_dataBuf[index+i]);
    printf("\r\n");
}
