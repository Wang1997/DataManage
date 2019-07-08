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
        printf("7.�˳�\r\n");
        printf("��ѡ��: ");
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
    printf("��������Ҫ��ӵ��ֽڸ�����");
    scanf("%d", &inputNum);
    if (!checkAddInput(inputNum))
    {
        printf("�Բ���,����ķ�Χ����\r\n");
        return;
    }
    int chunkIndex = 0;
    int nodeIndex = 0;
    //��ȡ�洢��Ϣ
    if (!getStorageInfo(inputNum, &chunkIndex, &nodeIndex))
    {
        printf("�Բ���,ʣ��ռ䲻��\r\n");
        return;
    }

    rewind(stdin);
    printf("���������ݣ�");
    int i = 0;
    Type data;
    //��ȡ���ݴ洢��ʼ�±�
    int index = getDataBufIndex(chunkIndex, nodeIndex);
    while (i < inputNum)
    {
        data = getchar();
        addInputData(index+i,&data);
        i++;
    }
    rewind(stdin);
    printf("��ӳɹ�\r\n");
    return;
}

//ɾ������
void deleteInput()
{
    int chunkIndex = 0;
    int nodeIndex = 0;
    if (!findByIdInput(&chunkIndex, &nodeIndex))
    {
        return;
    }
    printf("��ȷ���Ƿ�ɾ��(y or n)��");
    rewind(stdin);
    char confirm;
    confirm = getchar();
    rewind(stdin);
    if (!checkConfirmInput(confirm))
    {//��ɾ��
        printf("������δɾ��\r\n");
        return; 
    }
        
    if (deleteDataBuf(chunkIndex, nodeIndex))
    {
        printf("ɾ���ɹ�\r\n");
    }
}

//����
void updateInput()
{
    int chunkIndex = 0;
    int nodeIndex = 0;
    if (!findByIdInput(&chunkIndex, &nodeIndex))
    {
        return;
    }

    int inputNum = 0;
    printf("�������޸ĺ���ֽڸ�����");
    scanf("%d", &inputNum);
    if (!checkAddInput(inputNum))
    {
        printf("�Բ���,����ķ�Χ����\r\n");
        return;
    }
    printf("����������:");
    Type data[MAX_INPUT_LENGTH];
    int i = 0;
    rewind(stdin);
    while (i < inputNum)
    {
        data[i++] = getchar();
    }
    rewind(stdin);
    int length = getDataBufLength(chunkIndex, nodeIndex);
    if (length < inputNum) //���ȡ�µĴ洢�ռ�
    {
        int newChunkIndex = 0;
        int newNodeIndex = 0;
        //��ȡ�µĴ洢��Ϣ
        if (!getStorageInfo(inputNum, &newChunkIndex, &newNodeIndex))
        {
            printf("�Բ���,ɾ��ʧ��,ʣ��ռ䲻��\r\n");
            return;
        }
        //ɾ��������
        deleteDataBuf(chunkIndex, nodeIndex);
        chunkIndex = newChunkIndex;
        nodeIndex = newNodeIndex;
    }

    if (updateDataBuf(chunkIndex, nodeIndex, data, inputNum))
    {
        printf("���³ɹ�\r\n");
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
                printf("�Բ�����������������ѡ��\r\n");
                system("pause");
                break;
            }
        }
    }
}


//���ݲ��� ����
void findByContentInput()
{
    printf("��������Ҫ�������ݵ��ַ�������");
    int inputNum;
    scanf("%d", &inputNum);
    if (!checkAddInput(inputNum))
    {
        printf("�Բ���,����ķ�Χ����\r\n");
        return;
    }

    printf("��������ƥ�������: ");
    Type data[MAX_INPUT_LENGTH];
    int i = 0;
    rewind(stdin);
    while (i < inputNum)
    {
        data[i++] = getchar();
    }

    printf("���ҽ����\r\n");
    int findNum = findByContent(data, inputNum);
    if (!findNum)
    {
        printf("  �Բ���,ƥ�䲻������\r\n");
    }
}


//Id���� ����
int findByIdInput(int *chunkIndex,int *nodeIndex)
{
    int dataId;
    printf("������Id:");
    scanf("%d",&dataId);
    
    if (!findById(dataId, chunkIndex, nodeIndex))
    {
        printf("�Բ���,�޸�Id����(������ɾ��),��ȷ��Id��Ϣ\r\n");
        return 0;
    }

    //��ȡ���ݴ洢�±�ͳ���
    int index = getDataBufIndex(*chunkIndex, *nodeIndex);
    int length = getDataBufLength(*chunkIndex, *nodeIndex);
    printf("���ҽ��: ");
    printData(dataId,index,length);
    return 1;
}

//ͳ�Ƶ��ʽ���
void statisticalWordsView()
{
    system("cls");
    int statisticalArr[LETTERS_NUM * 2] = {0};
    int totalLetters = statisticalWords(statisticalArr);
    
    if (!totalLetters)
    {
        printf("��ͳ�ƽ��\r\n");
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
                system("cls");
                
                return;
            }
            case 2:
            {
                
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

//��ӡĳһ������
void printData(int dataId, int index, int length)
{
    printf("[%d]. ", dataId);
    for(int i = 0;i < length;++i)
        //printf("%c",g_dataBuf[index+i]);
    printf("\r\n");
}
