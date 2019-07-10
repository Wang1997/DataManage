#include "stringManage.h"


int main(int argc, char *argv[])
{
    manageInit();
    menuView();
    manageQuit();
    system("pause");
    return 0;
}

//�ó����ʼ��
void manageInit()
{
    init();
    srand(time(NULL));
}

//�ó����˳�
void manageQuit()
{
    quit(); //����dataCrud��quit�������ͷ��ڴ�
    printf("��л����ʹ��\r\n");
}

// ���������
int checkAddInput(int inputNum)
{
    if (inputNum <= 0 || inputNum > MAX_INPUT_LENGTH)
        return 0;
    return 1;
}

//ȷ�Ϻ˶Լ��
int checkConfirmInput(char confirm)
{
    if (confirm != 'y' && confirm != 'Y')
        return 0;
    return 1;
}

//�˵�����
void menuView()
{
    while (1)
    {
        system("cls");
        printf("  �ַ�������ϵͳ  \r\n");
        printf("1.���ӹ���\r\n");
        printf("2.ɾ������\r\n");
        printf("3.�޸Ĺ���\r\n");
        printf("4.��ѯ����\r\n");
        printf("5.ͳ�ƹ���\r\n");
        printf("6.�洢��Ϣ����\r\n");
        printf("7.�Զ����ݹ���\r\n");
        printf("8.�˳�\r\n");
        printf("��ѡ��: ");
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
                printf("�Բ�����������������ѡ��\r\n");
                break;
            }
                
        }
        system("pause");        
    }
}

// ������������
void addInput()
{
    int inputNum = 0;
    printf("��������Ҫ��ӵ��ַ�����");
    Type data[MAX_INPUT_LENGTH+1] = {0};
    char input[8] = {0};
    sprintf(input, "%%%ds", MAX_INPUT_LENGTH);

    rewind(stdin);
    scanf(input , &data); //��������

    int dataLength = strlen(data);

    if (!checkAddInput(dataLength))
    {
        printf("�Բ���,����ķ�Χ����\r\n");
        return;
    }

    //��ȡ�洢��Ϣ
    IndexInfo indexInfo = getStorageInfo(dataLength);
    if (!checkIndexInfo(&indexInfo))
    {
        printf("�Բ���,ʣ��ռ䲻��\r\n");
        return;
    }

    if (addInputData(&indexInfo, data, dataLength))
    {
        printf("��ӳɹ�\r\n");
    }
    else
    {
        printf("���ʧ��\r\n");
    }
    
    return;
}

//ɾ������
void deleteInput()
{
    printf("������Ҫɾ����Id:");
    int dataId = 0;
    rewind(stdin);
    scanf("%d",&dataId);

    //��ȡ�洢��Ϣ
    IndexInfo indexInfo = findById(dataId);
    if (!checkIndexInfo(&indexInfo))
    {
        printf("�Բ���,���Ҳ���������\r\n");
        return;
    }
    printf("��ɾ����������: \r\n");
    printData(&indexInfo);

    printf("��ȷ���Ƿ�ɾ��(y or n)��");
    rewind(stdin);
    char confirm;
    confirm = getchar();
    if (!checkConfirmInput(confirm))
    {//��ɾ��
        printf("������δɾ��\r\n");
        return; 
    }
        
    if (deleteDataBuf(&indexInfo))
    {
        printf("ɾ���ɹ�\r\n");
    }
    else
    {
        printf("ɾ��ʧ��\r\n");
    }
}

//����
void updateInput()
{
    printf("������Ҫ���µ�Id:");
    int dataId = 0;
    rewind(stdin);
    scanf("%d", &dataId);

    //��ȡ�洢��Ϣ
    IndexInfo indexInfo = findById(dataId);
    if (!checkIndexInfo(&indexInfo))
    {
        printf("�Բ���,���Ҳ���������\r\n");
        return;
    }
    printf("�������������: \r\n");
    printData(&indexInfo);

    printf("��������º���ַ�����");
    Type data[MAX_INPUT_LENGTH + 1] = { 0 };
    char input[8] = { 0 };
    sprintf(input, "%%%ds", MAX_INPUT_LENGTH);

    rewind(stdin);
    scanf(input, &data); //��������

    int length = getDataBufLength(&indexInfo);
    int updateLength = strlen(data);
    if (length < updateLength) //���ȡ�µĴ洢�ռ�
    {
        //��ȡ�µĴ洢��Ϣ
        IndexInfo updateIndexInfo = getStorageInfo(updateLength);
        if (!checkIndexInfo(&updateIndexInfo))
        {
            printf("�Բ���,����ʧ��,ʣ��ռ䲻��\r\n");
            return;
        }
        //ɾ��������
        if (deleteDataBuf(&indexInfo))
        {
            indexInfo = updateIndexInfo;
        }
        else 
        {
            printf("����ʧ��,������ɾ��ʧ��\r\n");
            return;
        }
    }

    if (updateDataBuf(&indexInfo, data, updateLength))
    {
        printf("���³ɹ�\r\n");
    }
    else
    {
        printf("����ʧ��\r\n");
    }
    
}

//��ѯ����
void queryView()
{
    while (1)
    {
        system("cls");
        printf("  ��ѯ����  \r\n");
        printf("1.��ѯ��������\r\n");
        printf("2.�������ݲ���\r\n");
        printf("3.����Id����\r\n");
        printf("��ѡ��: ");
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
                printf("�Բ�����������������ѡ��\r\n");
                system("pause");
                break;
            }
        }
    }
}

// չʾ��������
void showAllData()
{
    system("cls");

    resetIteatorIndex();
    while (hasNextIndexInfo())
    {
        IndexInfo indexInfo = getNextIndexInfo();
        if (getDataBufLength(&indexInfo)) //ɾ�����ݲ���ӡ
        {
            printData(&indexInfo);
        }
    }
    
    return;
}


//���ݲ��� ����
void findByContentInput()
{
    printf("��������Ҫƥ����ַ�����");
    Type data[MAX_INPUT_LENGTH + 1] = { 0 };
    char input[8] = { 0 };
    sprintf(input, "%%%ds", MAX_INPUT_LENGTH);

    rewind(stdin);
    scanf(input, &data); //��������

    int dataLength = strlen(data);

    printf("���ҽ����\r\n");
    int flag = 0;
    
    resetIteatorIndex();
    while (hasNextIndexInfo())
    {
        IndexInfo indexInfo = getNextFindByContent(data, dataLength);
        //���ܲ��Ҳ���,���ж�������Ч��
        if (checkIndexInfo(&indexInfo)) 
        {
            printData(&indexInfo);
            flag = 1;
        }
    }

    if (!flag)
    {
        printf("  �Բ���,ƥ�䲻������\r\n");
    }
}


//Id���� ����
void findByIdInput()
{
    
    printf("��������Ҫ���ҵ�Id:");
    int dataId;
    rewind(stdin);
    scanf("%d",&dataId);
    
    //��ȡ�洢��Ϣ
    IndexInfo indexInfo = findById(dataId);
    if (!checkIndexInfo(&indexInfo))
    {
        printf("�Բ���,�����޽��\r\n");
        return;
    }

    printf("������������: \r\n");
    printData(&indexInfo);
}

//ͳ�Ƶ��ʽ���
void statisticalWordsView()
{
    system("cls");
    int totalLetters = getDataTotalNum();
    int *statisticalArr = statisticalWords();
    
    if (!totalLetters)
    {
        printf("�Բ���,�������ݿ�ͳ��\r\n");
        return;
    }
    
    printf("ͳ�ƽ������: \r\n");
    printf("�ַ��ܼ�Ϊ%d��\r\n", totalLetters);
    for (int i = 0; i < LETTERS_NUM * 2; ++i)
    {
        if (statisticalArr[i])
        {
            if (i < LETTERS_NUM)
            {
                printf("%c ������%d��,����Ϊ%.2lf%%\r\n", i + 'a', statisticalArr[i],
                    statisticalArr[i] * 100.0 / totalLetters);
            }
            else
            {
                printf("%c ������%d��,����Ϊ%.2lf%%\r\n", i - LETTERS_NUM + 'A', 
                    statisticalArr[i], statisticalArr[i] * 100.0 / totalLetters);
            }
        }
    }
    printf("\r\n������ĸ��δ���֣�\r\n");
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

//�洢��Ϣ����
void storeInfoView()
{
    while (1)
    {
        system("cls");
        printf("  �洢��Ϣ  \r\n");
        printf("1.�洢��Դ�ֲ����\r\n");
        printf("2.��Ƭ����\r\n");
        printf("��ѡ��: ");
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
                    printf("��Ƭ����ɹ�\r\n");
                }
                else
                {
                    printf("��Ƭ����ʧ��\r\n");
                }
                return;
            }
            default:
            {
                printf("�Բ�����������������ѡ��\r\n");
                system("pause");
                break;
            }
        }
    }
}

//չʾ�洢��Դͼ
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
                printf("��"); //ռ��
            }
            else
            {
                printf("��"); //����
            }
            if(++totalCounts % 30 == 0)
                printf("\r\n");
        }
        totalSize += size;
    }
    printf("\r\n");
    printf("�ܹ��ռ��С:0x%x,ʣ��0x%xδ����\r\n", BUF_LENGTH, BUF_LENGTH-totalSize);
}

//��ӡĳһ������
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


//�Զ�����
void autoExpansion()
{
    char padding[] = "qwertyuioopasdfghjklzxcvbnm1234567890QWERTYUIO\
        PASDFGHJKLZXCVBNM,.[;[]!@#%$^&*()))_+";
    int paddingLen = strlen(padding);
    //�������������޽�������ݽ���
    int find = 0;
    Type data[MAX_INPUT_LENGTH] = {0};
    
    while (find < 3)
    {
        int dataLength = (rand() % MAX_INPUT_LENGTH) + 1; //[1,30]
                                                          //��ȡ�洢��Ϣ
        IndexInfo indexInfo = getStorageInfo(dataLength);
        if (!checkIndexInfo(&indexInfo))
        { //ʧ��
            find++;
            continue;
        }
        for (int i = 0; i < dataLength; ++i)
        {
            data[i] = padding[rand() % paddingLen];
        }
        if (!addInputData(&indexInfo, data, dataLength))
        { //ʧ��
            find++;
            continue;
        }
        find = 0;
    }
    printf("�Զ����ݳɹ�\r\n");
}
