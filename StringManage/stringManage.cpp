#include "stringManage.h"


//���ݴ��ָ������
Type g_dataBuf[BUF_LENGTH];
//������Ϣ��¼����
ChunkInfo g_chunkInfoArr[BUF_LENGTH];

int g_chunkNum = 0;//������Ϣ��¼�����

int main(int argc, char *argv[])
{
    init();
    menuView();
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
    //�ͷŶ��ڴ�
    for (int i = 0; i < g_chunkNum; ++i)
    {
        free(g_chunkInfoArr[i].nodeArr);
    }
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

// ��ȡ��������ĳ���
int getDataBufLength(int chunkIndex, int nodeIndex)
{
    return g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].length;
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
        if (!addNewChunk(inputNum, totalSize))
        {
            return 0;
        }
        *chunkIndex = g_chunkNum - 1;
        *nodeIndex = 0;
        //��ʼ���½ڵ�
        g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].length = inputNum;
        g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].size = inputNum;
    }
    else if(g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].length)
    {//ԭ��������,�迪���½ڵ� ��չ����
        int cpySize = (g_chunkInfoArr[*chunkIndex].nodeNum - *nodeIndex - 1) 
                                                    * sizeof(Node);
        if (cpySize > 0) //Խ�翼��
        {
            memmove(&g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex + 2],
                &g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex + 1],
                cpySize); //���������ƶ�һ��
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
    else
    {
        g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].length = inputNum;
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
            if (g_chunkInfoArr[i].nodeArr[j].length) //����Ч���ݴ�ӡ
            {
                printData(dataId, index, g_chunkInfoArr[i].nodeArr[j].length);
            }
            index += g_chunkInfoArr[i].nodeArr[j].size;
        }
    }
    
}

//����Id����
int findById(int dataId, int *chunkIndex, int *nodeIndex)
{
    if(dataId <= 0) //�������ݼ��
        return 0;

    int calcId = 0;
    //������ 
    for (int i = 0; i < g_chunkNum; ++i)
    {
        calcId += g_chunkInfoArr[i].nodeNum;
        if (calcId >= dataId)
        {
            *chunkIndex = i;
            *nodeIndex = dataId - (calcId - g_chunkInfoArr[i].nodeNum) - 1; //�±���-1
            // �ж�������Ч��
            if (!g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].length)
                return 0;
            return 1;
        }
    }
    return 0;
}

// ����ƥ��
int dataCompare(int dataIndex, int dataLen, PType content, int conLen)
{
    if (conLen > dataLen)
    {
        return 0;
    }

    for (int i = dataIndex; i < dataIndex+dataLen; ++i)
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
        if(flag) //ƥ��ɹ�
            return 1;
    }

    return 0;
}

//�������ݲ���
int findByContent(PType content, int length)
{
    int dataId = 0;
    int findNum = 0;
    //������ 
    for (int i = 0; i < g_chunkNum; ++i)
    {
        int index = g_chunkInfoArr[i].startIndex;
        //�������нڵ�
        for (int j = 0; j < g_chunkInfoArr[i].nodeNum; ++j)
        {
            ++dataId;
            if (dataCompare(index, g_chunkInfoArr[i].nodeArr[j].length, content, length)) //ƥ������
            {
                printData(dataId, index, g_chunkInfoArr[i].nodeArr[j].length);
                ++findNum;
            }
            index += g_chunkInfoArr[i].nodeArr[j].size;
        }
    }
    return findNum;
}

//ȷ�Ϻ˶Լ��
int checkConfirmInput(char confirm) 
{
    if(confirm != 'y' && confirm != 'Y')
        return 0;
    return 1;
}

// ɾ������
int deleteDataBuf(int chunkIndex, int nodeIndex)
{
    //ɾ��
    g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].length = 0;

    //��������ڵ������ϲ�(Լ����ǰ)
    //�жϺ�һ���ϲ�
    int nextIndex = nodeIndex + 1;
    if (nextIndex < g_chunkInfoArr[chunkIndex].nodeNum)
    {
        if (g_chunkInfoArr[chunkIndex].nodeArr[nextIndex].length == 0)
        {
            g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].size +=
                g_chunkInfoArr[chunkIndex].nodeArr[nextIndex].size;
            //��������
            int cpySize = (g_chunkInfoArr[chunkIndex].nodeNum - nextIndex - 1) 
                                                        * sizeof(Node);
            if (cpySize > 0) //Խ�翼��
            {
                memmove(&g_chunkInfoArr[chunkIndex].nodeArr[nextIndex],
                    &g_chunkInfoArr[chunkIndex].nodeArr[nextIndex + 1],
                    cpySize); //������ǰ�ƶ�һ��
            }
            g_chunkInfoArr[chunkIndex].nodeNum--;
        }
    }

    //�ж�ǰһ���ϲ�
    int preIndex = nodeIndex - 1;
    if (preIndex >= 0)
    {
        if (g_chunkInfoArr[chunkIndex].nodeArr[preIndex].length == 0)
        {
            g_chunkInfoArr[chunkIndex].nodeArr[preIndex].size +=
                g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].size;
            //��������
            int cpySize = (g_chunkInfoArr[chunkIndex].nodeNum - nodeIndex - 1) 
                                                        * sizeof(Node);
            if (cpySize > 0) //Խ�翼��
            {
                memmove(&g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex],
                    &g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex + 1],
                    cpySize); //������ǰ�ƶ�һ��
            }
            g_chunkInfoArr[chunkIndex].nodeNum--;
        }
    }
    
    return 1;
}

//��������
int updateDataBuf(int chunkIndex, int nodeIndex, PType content, int conLen)
{
    int index = getDataBufIndex(chunkIndex, nodeIndex);
    g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].length = conLen;
    //����Դ����
    memcpy(&g_dataBuf[index], content, sizeof(Type) * conLen);
    return 1; 
}

//ͳ�Ƶ���
int statisticalWords(int *statisticalArr)
{
    int totalLetters = 0;
    //������ 
    for (int i = 0; i < g_chunkNum; ++i)
    {
        int index = g_chunkInfoArr[i].startIndex;
        //�������нڵ�
        for (int j = 0; j < g_chunkInfoArr[i].nodeNum; ++j)
        {
            //������ʵ����
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

/**************************view*****************************/

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


//��ӡĳһ������
void printData(int dataId, int index, int length)
{
    printf("[%d]. ", dataId);
    for(int i = 0;i < length;++i)
        printf("%c",g_dataBuf[index+i]);
    printf("\r\n");
}
