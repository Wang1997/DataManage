#include "stringManage.h"


//���ݴ��ָ������
Type g_dataBuf[BUF_LENGTH];
//������Ϣ��¼����
ChunkInfo g_chunkInfoArr[BUF_LENGTH];

int g_chunkNum = 0;//������Ϣ��¼�����

int main(int argc, char *argv[])
{
    init();
    menuInput();
    quit();
    system("pause");
    return 0;
}

/*************************logic*****************************/

//��ʼ��
void init()
{
    memset(g_dataBuf,0,sizeof(Type) * BUF_LENGTH);
    memset(g_chunkInfoArr,0,sizeof(ChunkInfo) * BUF_LENGTH);
}

//�˳�
void quit()
{
    
}

// ���������
int checkAddInput(int inputNum)
{
    if(inputNum <= 0 || inputNum > MAX_INPUT_LENGTH)
        return 0;
    return 1;
}

// �������
void addInputData(int index,PType pData)
{
    memcpy(&g_dataBuf[index], pData, sizeof(Type));
}

// ��ȡ����������±�
int getDataBufIndex(int chunkIndex,int nodeIndex)
{
    int index = g_chunkInfoArr[chunkIndex].startIndex;
    for (int i = 0; i < nodeIndex; ++i)
    {
        index += g_chunkInfoArr[chunkIndex].nodeArr[i].size;
    }
    return index;
}

// ���һ���µĴ洢��
int addNewChunk(int inputNum,int startIndex)
{
    if (inputNum > BUF_LENGTH - startIndex)
    {// �ռ䲻��
        return 0;
    }

    g_chunkInfoArr[g_chunkNum].startIndex = startIndex;
    g_chunkInfoArr[g_chunkNum].totalSize = inputNum;
    g_chunkInfoArr[g_chunkNum].nodeNum = 1;
    g_chunkInfoArr[g_chunkNum].nodeArr = (PNode)malloc(sizeof(Node) * inputNum);
    if (!g_chunkInfoArr[g_chunkNum].nodeArr)
    {// �ڴ�������
        return 0;
    }

    memset(g_chunkInfoArr[g_chunkNum].nodeArr, 0, sizeof(Node) * inputNum);
    g_chunkNum++;

    return 1;
}

//��ȡһ��洢λ��
int getStorageInfo(int inputNum,int *chunkIndex,int *nodeIndex)
{
    int totalSize = 0;
    int findFlag = 0;
    int space = 0;
    //������ 
    for (int i = 0; i < g_chunkNum; ++i)
    {
        //�������нڵ�
        for (int j = 0; j < g_chunkInfoArr[i].nodeNum; ++j)
        {
            space = g_chunkInfoArr[i].nodeArr[j].size - 
                               g_chunkInfoArr[i].nodeArr[j].length; //��������
            if (space >= inputNum)// �ҵ����������Ľڵ�
            {
                *chunkIndex = i;
                *nodeIndex = j;
                findFlag = 1;
                break;
            }
        }
        totalSize += g_chunkInfoArr[i].totalSize;
        if (findFlag)
        { //�ҵ��������˳�
            break;
        }
    }
    
    if (!findFlag)
    {//�����¿�
        if (!addNewChunk(inputNum,totalSize))
        {
            return 0;
        }
        *chunkIndex = g_chunkNum - 1;
        *nodeIndex = 0;
        //��ʼ���½ڵ�
        g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].length = inputNum;
        g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].size = inputNum;
    }
    else
    {//�����½ڵ�
        for (int j = g_chunkInfoArr[*chunkIndex].nodeNum; j > *nodeIndex + 1; --j)
        {
            g_chunkInfoArr[*chunkIndex].nodeArr[j] = 
                            g_chunkInfoArr[*chunkIndex].nodeArr[j-1];
        }
        //��ռ�
        g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].size = 
                           g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].length;
        //��ʼ���½ڵ�
        *nodeIndex += 1;
        g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].length = inputNum;
        g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].size = space;
        g_chunkInfoArr[*chunkIndex].nodeNum++; //�ڵ����++;
    }
    return 1;
}

//չʾ��������
void showAllData()
{
    
    int dataId = 0;
    //������ 
    for (int i = 0; i < g_chunkNum; ++i)
    {
        int index = g_chunkInfoArr[i].startIndex;
        //�������нڵ�
        for (int j = 0; j < g_chunkInfoArr[i].nodeNum; ++j)
        {
            ++dataId;
            if (!g_chunkInfoArr[i].nodeArr[j].length) //��ɾ��
                continue;
            printData(dataId, index, g_chunkInfoArr[i].nodeArr[j].length);
            index += g_chunkInfoArr[i].nodeArr[j].size;
        }
    }
    
}



/**************************view*****************************/

//�˵�����
void menuInput()
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
    printf("��ӳɹ�\r\n");
    rewind(stdin);
    return;
}

//ɾ������
void deleteInput()
{
    int dataId = 0;
    printf("��������Ҫɾ��������Id��");
    scanf("%d", &dataId);

}

//��ѯ����
void queryInput()
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
                return;
            }
            default:
            {
                printf("�Բ�����������������ѡ��\r\n");
                break;
            }
        }
    }
}

//���ݲ��� ����
void findByContentInput()
{

}

//��ӡĳһ������
void printData(int dataId, int index, int length)
{
    printf("%d. ", dataId);
    for(int i = 0;i < length;++i)
        printf("%c",g_dataBuf[index+i]);
    printf("\r\n");
}
