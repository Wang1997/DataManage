#include "stringManage.h"
#include "dataCrud.h"


int main(int argc, char *argv[])
{
    init();
    menuView();
    quit();
    system("pause");
    return 0;
}


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
                updateInput();
                break;
            }
            case 4:
            {
                queryView();
                break;
            }
            case 5:
            {
                statisticalWordsView();
                break;
            }
            case 6:
            {
                storeInfoView();
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
    rewind(stdin);
    printf("添加成功\r\n");
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

//更新
void updateInput()
{
    int chunkIndex = 0;
    int nodeIndex = 0;
    if (!findByIdInput(&chunkIndex, &nodeIndex))
    {
        return;
    }

    int inputNum = 0;
    printf("请输入修改后的字节个数：");
    scanf("%d", &inputNum);
    if (!checkAddInput(inputNum))
    {
        printf("对不起,输入的范围有误\r\n");
        return;
    }
    printf("请输入数据:");
    Type data[MAX_INPUT_LENGTH];
    int i = 0;
    rewind(stdin);
    while (i < inputNum)
    {
        data[i++] = getchar();
    }
    rewind(stdin);
    int length = getDataBufLength(chunkIndex, nodeIndex);
    if (length < inputNum) //需获取新的存储空间
    {
        int newChunkIndex = 0;
        int newNodeIndex = 0;
        //获取新的存储信息
        if (!getStorageInfo(inputNum, &newChunkIndex, &newNodeIndex))
        {
            printf("对不起,删除失败,剩余空间不够\r\n");
            return;
        }
        //删除旧数据
        deleteDataBuf(chunkIndex, nodeIndex);
        chunkIndex = newChunkIndex;
        nodeIndex = newNodeIndex;
    }

    if (updateDataBuf(chunkIndex, nodeIndex, data, inputNum))
    {
        printf("更新成功\r\n");
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
    printf("请输入需要查找内容的字符个数：");
    int inputNum;
    scanf("%d", &inputNum);
    if (!checkAddInput(inputNum))
    {
        printf("对不起,输入的范围有误\r\n");
        return;
    }

    printf("请输入需匹配的数据: ");
    Type data[MAX_INPUT_LENGTH];
    int i = 0;
    rewind(stdin);
    while (i < inputNum)
    {
        data[i++] = getchar();
    }

    printf("查找结果：\r\n");
    int findNum = findByContent(data, inputNum);
    if (!findNum)
    {
        printf("  对不起,匹配不到数据\r\n");
    }
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

//统计单词界面
void statisticalWordsView()
{
    system("cls");
    int statisticalArr[LETTERS_NUM * 2] = {0};
    int totalLetters = statisticalWords(statisticalArr);
    
    if (!totalLetters)
    {
        printf("无统计结果\r\n");
        return;
    }
    
    printf("统计结果如下: \r\n");
    printf("字符总计为%d个\r\n", totalLetters);
    for (int i = 0; i < LETTERS_NUM * 2; ++i)
    {
        if (statisticalArr[i])
        {
            if (i < LETTERS_NUM)
            {
                printf("%c 出现了%d次,比例为%.2lf%%\r\n", i + 'a', statisticalArr[i],
                    statisticalArr[i] * 100.0 / totalLetters);
            }
            else
            {
                printf("%c 出现了%d次,比例为%.2lf%%\r\n", i - LETTERS_NUM + 'A', 
                    statisticalArr[i], statisticalArr[i] * 100.0 / totalLetters);
            }
        }
    }
    printf("\r\n以下字母尚未出现：\r\n");
    int flag = 0;
    for (int i = 0; i < LETTERS_NUM * 2; ++i)
    {
        if (!statisticalArr[i])
        {
            if(flag)
                printf(",");
            if (i < LETTERS_NUM)
            {
                printf("%c", i + 'a');
            }
            else
            {
                printf("%c", i - LETTERS_NUM + 'A');
            }
            flag = 1;
        }
        if (i == LETTERS_NUM - 1)
        {
            printf("\r\n");
            flag = 0;
        }
    }
    printf("\r\n");
}

//存储信息界面
void storeInfoView()
{
    while (1)
    {
        system("cls");
        printf("  存储信息  \r\n");
        printf("1.存储资源分布情况\r\n");
        printf("2.碎片整理\r\n");
        printf("请选择: ");
        int choose;
        scanf("%d", &choose);
        switch (choose)
        {
            case 1:
            {
                system("cls");
                
                return;
            }
            case 2:
            {
                
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

//打印某一条数据
void printData(int dataId, int index, int length)
{
    printf("[%d]. ", dataId);
    for(int i = 0;i < length;++i)
        //printf("%c",g_dataBuf[index+i]);
    printf("\r\n");
}
