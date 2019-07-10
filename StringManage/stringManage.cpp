#include "stringManage.h"


int main(int argc, char *argv[])
{
    manageInit();
    menuView();
    manageQuit();
    system("pause");
    return 0;
}

//该程序初始化
void manageInit()
{
    init();
    srand(time(NULL));
}

//该程序退出
void manageQuit()
{
    quit(); //调用dataCrud的quit，进行释放内存
    printf("感谢您的使用\r\n");
}

// 添加输入检查
int checkAddInput(int inputNum)
{
    if (inputNum <= 0 || inputNum > MAX_INPUT_LENGTH)
        return 0;
    return 1;
}

//确认核对检查
int checkConfirmInput(char confirm)
{
    if (confirm != 'y' && confirm != 'Y')
        return 0;
    return 1;
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
        printf("7.自动扩容功能\r\n");
        printf("8.退出\r\n");
        printf("请选择: ");
        int choose;
        rewind(stdin);
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
                autoExpansion();
                break;
            }
            case 8:
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
    printf("请输入需要添加的字符串：");
    Type data[MAX_INPUT_LENGTH+1] = {0};
    char input[8] = {0};
    sprintf(input, "%%%ds", MAX_INPUT_LENGTH);

    rewind(stdin);
    scanf(input , &data); //输入数据

    int dataLength = strlen(data);

    if (!checkAddInput(dataLength))
    {
        printf("对不起,输入的范围有误\r\n");
        return;
    }

    //获取存储信息
    IndexInfo indexInfo = getStorageInfo(dataLength);
    if (!checkIndexInfo(&indexInfo))
    {
        printf("对不起,剩余空间不够\r\n");
        return;
    }

    if (addInputData(&indexInfo, data, dataLength))
    {
        printf("添加成功\r\n");
    }
    else
    {
        printf("添加失败\r\n");
    }
    
    return;
}

//删除输入
void deleteInput()
{
    printf("请输入要删除的Id:");
    int dataId = 0;
    rewind(stdin);
    scanf("%d",&dataId);

    //获取存储信息
    IndexInfo indexInfo = findById(dataId);
    if (!checkIndexInfo(&indexInfo))
    {
        printf("对不起,查找不到该数据\r\n");
        return;
    }
    printf("需删除数据如下: \r\n");
    printData(&indexInfo);

    printf("请确认是否删除(y or n)：");
    rewind(stdin);
    char confirm;
    confirm = getchar();
    if (!checkConfirmInput(confirm))
    {//不删除
        printf("该数据未删除\r\n");
        return; 
    }
        
    if (deleteDataBuf(&indexInfo))
    {
        printf("删除成功\r\n");
    }
    else
    {
        printf("删除失败\r\n");
    }
}

//更新
void updateInput()
{
    printf("请输入要更新的Id:");
    int dataId = 0;
    rewind(stdin);
    scanf("%d", &dataId);

    //获取存储信息
    IndexInfo indexInfo = findById(dataId);
    if (!checkIndexInfo(&indexInfo))
    {
        printf("对不起,查找不到该数据\r\n");
        return;
    }
    printf("需更新数据如下: \r\n");
    printData(&indexInfo);

    printf("请输入更新后的字符串：");
    Type data[MAX_INPUT_LENGTH + 1] = { 0 };
    char input[8] = { 0 };
    sprintf(input, "%%%ds", MAX_INPUT_LENGTH);

    rewind(stdin);
    scanf(input, &data); //输入数据

    int length = getDataBufLength(&indexInfo);
    int updateLength = strlen(data);
    if (length < updateLength) //需获取新的存储空间
    {
        //获取新的存储信息
        IndexInfo updateIndexInfo = getStorageInfo(updateLength);
        if (!checkIndexInfo(&updateIndexInfo))
        {
            printf("对不起,更新失败,剩余空间不够\r\n");
            return;
        }
        //删除旧数据
        if (deleteDataBuf(&indexInfo))
        {
            indexInfo = updateIndexInfo;
        }
        else 
        {
            printf("更新失败,旧数据删除失败\r\n");
            return;
        }
    }

    if (updateDataBuf(&indexInfo, data, updateLength))
    {
        printf("更新成功\r\n");
    }
    else
    {
        printf("更新失败\r\n");
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
        rewind(stdin);
        scanf("%d", &choose);
        switch (choose)
        {
            case 1:
            {
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
                findByIdInput();
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

// 展示所有数据
void showAllData()
{
    system("cls");

    resetIteatorIndex();
    while (hasNextIndexInfo())
    {
        IndexInfo indexInfo = getNextIndexInfo();
        if (getDataBufLength(&indexInfo)) //删除数据不打印
        {
            printData(&indexInfo);
        }
    }
    
    return;
}


//内容查找 输入
void findByContentInput()
{
    printf("请输入需要匹配的字符串：");
    Type data[MAX_INPUT_LENGTH + 1] = { 0 };
    char input[8] = { 0 };
    sprintf(input, "%%%ds", MAX_INPUT_LENGTH);

    rewind(stdin);
    scanf(input, &data); //输入数据

    int dataLength = strlen(data);

    printf("查找结果：\r\n");
    int flag = 0;
    
    resetIteatorIndex();
    while (hasNextIndexInfo())
    {
        IndexInfo indexInfo = getNextFindByContent(data, dataLength);
        //可能查找不到,需判断数据有效性
        if (checkIndexInfo(&indexInfo)) 
        {
            printData(&indexInfo);
            flag = 1;
        }
    }

    if (!flag)
    {
        printf("  对不起,匹配不到数据\r\n");
    }
}


//Id查找 输入
void findByIdInput()
{
    
    printf("请输入需要查找的Id:");
    int dataId;
    rewind(stdin);
    scanf("%d",&dataId);
    
    //获取存储信息
    IndexInfo indexInfo = findById(dataId);
    if (!checkIndexInfo(&indexInfo))
    {
        printf("对不起,查找无结果\r\n");
        return;
    }

    printf("查找数据如下: \r\n");
    printData(&indexInfo);
}

//统计单词界面
void statisticalWordsView()
{
    system("cls");
    int totalLetters = getDataTotalNum();
    int *statisticalArr = statisticalWords();
    
    if (!totalLetters)
    {
        printf("对不起,暂无数据可统计\r\n");
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
                showStorageResource();
                return;
            }
            case 2:
            {
                if (defragment())
                {
                    printf("碎片整理成功\r\n");
                }
                else
                {
                    printf("碎片整理失败\r\n");
                }
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

//展示存储资源图
void showStorageResource()
{
    int totalCounts = 0;
    int totalSize = 0;
    resetIteatorIndex();
    while (hasNextIndexInfo())
    {
        IndexInfo indexInfo = getNextIndexInfo();
        int len = getDataBufLength(&indexInfo);
        int size = getDataBufSize(&indexInfo);
        for (int i = 0; i < size; ++i)
        {
            if (i < len)
            {
                printf("■"); //占有
            }
            else
            {
                printf("□"); //空闲
            }
            if(++totalCounts % 30 == 0)
                printf("\r\n");
        }
        totalSize += size;
    }
    printf("\r\n");
    printf("总共空间大小:0x%x,剩余0x%x未分配\r\n", BUF_LENGTH, BUF_LENGTH-totalSize);
}

//打印某一条数据
void printData(PIndexInfo pIndexInfo)
{
    int dataId = getDataIdByIndexInfo(pIndexInfo);
    printf("[%d]. ", dataId);
    
    int length = getDataBufLength(pIndexInfo);
    for (int i = 0; i < length; ++i)
    {
        printf("%c", getDataByIndex(pIndexInfo, i));
    }
    printf("\r\n");
}


//自动扩容
void autoExpansion()
{
    char padding[] = "qwertyuioopasdfghjklzxcvbnm1234567890QWERTYUIO\
        PASDFGHJKLZXCVBNM,.[;[]!@#%$^&*()))_+";
    int paddingLen = strlen(padding);
    //连续查找三次无结果则扩容结束
    int find = 0;
    Type data[MAX_INPUT_LENGTH] = {0};
    
    while (find < 3)
    {
        int dataLength = (rand() % MAX_INPUT_LENGTH) + 1; //[1,30]
                                                          //获取存储信息
        IndexInfo indexInfo = getStorageInfo(dataLength);
        if (!checkIndexInfo(&indexInfo))
        { //失败
            find++;
            continue;
        }
        for (int i = 0; i < dataLength; ++i)
        {
            data[i] = padding[rand() % paddingLen];
        }
        if (!addInputData(&indexInfo, data, dataLength))
        { //失败
            find++;
            continue;
        }
        find = 0;
    }
    printf("自动扩容成功\r\n");
}
