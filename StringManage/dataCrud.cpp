#include "dataCrud.h"

//���ݴ��ָ������
static Type g_dataBuf[BUF_LENGTH];
//������Ϣ��¼����
static ChunkInfo g_chunkInfoArr[BUF_LENGTH];
//������Ϣ��¼�����
static int g_chunkNum = 0;


//��ʼ��
void init()
{
    memset(g_dataBuf, 0, sizeof(Type) * BUF_LENGTH);
    memset(g_chunkInfoArr, 0, sizeof(ChunkInfo) * BUF_LENGTH);
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
    if (inputNum <= 0 || inputNum > MAX_INPUT_LENGTH)
        return 0;
    return 1;
}

// �������
void addInputData(int index, PType pData)
{
    memcpy(&g_dataBuf[index], pData, sizeof(Type));
}

// ��ȡ����������±�
int getDataBufIndex(int chunkIndex, int nodeIndex)
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
int addNewChunk(int inputNum, int startIndex)
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
int getStorageInfo(int inputNum, int *chunkIndex, int *nodeIndex)
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
    else if (g_chunkInfoArr[*chunkIndex].nodeArr[*nodeIndex].length)
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

//չʾ����
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
                //printData(dataId, index, g_chunkInfoArr[i].nodeArr[j].length);
            }
            index += g_chunkInfoArr[i].nodeArr[j].size;
        }
    }

}

//����Id����
int findById(int dataId, int *chunkIndex, int *nodeIndex)
{
    if (dataId <= 0) //�������ݼ��
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
        if (flag) //ƥ��ɹ�
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
            //ƥ������
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

//ȷ�Ϻ˶Լ��
int checkConfirmInput(char confirm)
{
    if (confirm != 'y' && confirm != 'Y')
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
