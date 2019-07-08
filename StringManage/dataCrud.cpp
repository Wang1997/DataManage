#include "dataCrud.h"

//���ݽڵ�
typedef struct node
{
    int index = 0; //��Ӧ����Դ����
    int length = 0;//��ռ�ռ䳤��
    int size = 0; //�ڵ��ܴ�С
}Node, *PNode;

//���ݿ���Ϣ
typedef struct chunkInfo
{
    int totalSize = 0; //����ܴ�С
    int nodeNum = 0; //���нڵ����
    PNode nodeArr = NULL; //���нڵ�����
}ChunkInfo, *PChunkInfo;

//���ݴ��ָ������
static Type g_dataBuf[BUF_LENGTH];
//������Ϣ��¼����
static ChunkInfo g_chunkInfoArr[BUF_LENGTH];
//������Ϣ��¼�����
static int g_chunkNum = 0;

//���ݱ�����������
static int g_iteator_chunkIndex = 0;
static int g_iteator_nodeIndex = 0;

//ͳ�ƽ������
static int statisticalArr[LETTERS_NUM*2];

/*************************static function*****************************/

// ���һ���µĴ洢��
static int addNewChunk(int dataLength)
{
    int newIndex = 0;
    if (g_chunkNum != 0)
    {
        newIndex = g_chunkInfoArr[g_chunkNum - 1].nodeArr[0].index +
            g_chunkInfoArr[g_chunkNum - 1].totalSize;
    }

    if (dataLength > BUF_LENGTH - newIndex)
    {// ���ݿռ䲻��
        return 0;
    }
    //��ʼ����
    g_chunkInfoArr[g_chunkNum].totalSize = dataLength;
    g_chunkInfoArr[g_chunkNum].nodeNum = 1;
    g_chunkInfoArr[g_chunkNum].nodeArr = (PNode)malloc(sizeof(Node) * dataLength);
    if (!g_chunkInfoArr[g_chunkNum].nodeArr)
    {// �ڴ�������
        return 0;
    }
    //��ʼ���ڵ�
    memset(g_chunkInfoArr[g_chunkNum].nodeArr, 0, sizeof(Node) * dataLength);
    g_chunkInfoArr[g_chunkNum].nodeArr[0].index = newIndex;
    g_chunkInfoArr[g_chunkNum].nodeArr[0].size = dataLength;
    g_chunkNum++;

    return 1;
}

// ��������
static void updateIndex(int chunkIndex, int nodeIndex)
{
    if (nodeIndex <= 0)
    {
        nodeIndex = 1;
    }
    //��������
    for (; nodeIndex < g_chunkInfoArr[chunkIndex].nodeNum; ++nodeIndex)
    {
        g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].index =
            g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex-1].index +
            g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex-1].size;
    }
}

//����ֵ����1
static void addIteatorIndex()
{
    g_iteator_nodeIndex++;
    if (g_iteator_nodeIndex >= g_chunkInfoArr[g_iteator_chunkIndex].nodeNum)
    {
        g_iteator_chunkIndex++;
        g_iteator_nodeIndex = 0;
    }
}

//����ƥ��
static int dataCompare(int dataIndex, int dataLen, PType content, int conLen)
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



/*************************interface*****************************/


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

// �������������Ч��
int checkIndexInfo(PIndexInfo pIndexInfo)
{
    int chunkIndex = pIndexInfo->chunkIndex;
    int nodeIndex = pIndexInfo->nodeIndex;
    if (chunkIndex < 0 || chunkIndex >= g_chunkNum)
    {
        return 0;
    }

    int nodeNum = g_chunkInfoArr[chunkIndex].nodeNum;
    if (nodeIndex < 0 || nodeIndex >= nodeNum)
    {
        return 0;
    }

    return 1;
}

// �������
int addInputData(PIndexInfo pIndexInfo, PType pData, int dataLength)
{
    int chunkIndex = pIndexInfo->chunkIndex;
    int nodeIndex = pIndexInfo->nodeIndex;
    //���������Ч��
    if (!checkIndexInfo(pIndexInfo))
    {
        return 0;
    }
    //��鳤��
    if (dataLength > g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].size)
    {
        return 0;
    }

    int index = g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].index;
    memcpy(g_dataBuf+index, pData, sizeof(Type)*dataLength);//��������
    g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].length = dataLength;

    return 1;
}

//��ȡһ��洢λ��
IndexInfo getStorageInfo(int dataLength)
{
    int findFlag = 0;
    int space = 0;
    IndexInfo retIndex;
    retIndex.chunkIndex = -1;
    retIndex.nodeIndex = -1;

    if (dataLength <= 0)
    {
        return retIndex;
    }

    //������ 
    for (int i = 0; i < g_chunkNum; ++i)
    {
        //�������нڵ�
        for (int j = 0; j < g_chunkInfoArr[i].nodeNum; ++j)
        {

            space = g_chunkInfoArr[i].nodeArr[j].size -
                g_chunkInfoArr[i].nodeArr[j].length; //��������
            if (space >= dataLength)// �ҵ����������Ľڵ�
            {
                retIndex.chunkIndex = i;
                retIndex.nodeIndex = j;
                findFlag = 1;
                break;
            }
        }
        if (findFlag)
        { //�ҵ��������˳�
            break;
        }
    }

    int chunkIndex = retIndex.chunkIndex;
    int nodeIndex = retIndex.nodeIndex;

    if (!findFlag)
    {//�����¿�
        if (!addNewChunk(dataLength))
        {
            return retIndex;
        }
        retIndex.chunkIndex = g_chunkNum - 1;
        retIndex.nodeIndex = 0;
    }
    else if (g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].length)
    {//ԭ��������,�迪���½ڵ� ��չ����
        int cpySize = (g_chunkInfoArr[chunkIndex].nodeNum - nodeIndex - 1)
                                                        * sizeof(Node);
        if (cpySize > 0) //Խ�翼��
        {
            memmove(&g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex + 2],
                &g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex + 1],
                cpySize); //���������ƶ�һ��
        }

        //��ռ�
        g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].size =
            g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].length;
        //��ʼ���½ڵ�
        nodeIndex += 1;
        g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].size = space;
        g_chunkInfoArr[chunkIndex].nodeNum++; //�ڵ����++;
        updateIndex(chunkIndex, nodeIndex); //��������
        retIndex.nodeIndex = nodeIndex;
    }

    return retIndex;
}

//��������������
void resetIteatorIndex()
{
    g_iteator_chunkIndex = 0;
    g_iteator_nodeIndex = 0;
}

//�ж��Ƿ�������
int hasNextIndexInfo()
{
    if (g_iteator_chunkIndex < 0 || g_iteator_chunkIndex >= g_chunkNum)
    {
        return 0;
    }
    if (g_iteator_nodeIndex < 0 || g_iteator_nodeIndex >= 
                                        g_chunkInfoArr[g_iteator_chunkIndex].nodeNum)
    {
        return 0;
    }
    return 1;
}

//��ȡ�����ṹ������
IndexInfo getNextIndexInfo()
{
    IndexInfo retIndex;
    retIndex.chunkIndex = -1;
    retIndex.nodeIndex = -1;

    if (!hasNextIndexInfo())
    {
        return retIndex;
    }
    retIndex.chunkIndex = g_iteator_chunkIndex;
    retIndex.nodeIndex = g_iteator_nodeIndex;

    //��һ��
    addIteatorIndex();
    
    return retIndex;
}

// ��ȡԴ���ݵĳ���
int getDataBufLength(PIndexInfo pIndexInfo)
{
    int chunkIndex = pIndexInfo->chunkIndex;
    int nodeIndex = pIndexInfo->nodeIndex;

    //���������Ч��
    if (!checkIndexInfo(pIndexInfo))
    {
        return 0;
    }

    return g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].length;
}

// ��ȡԴ���ݵ�ռ�ÿռ�
int getDataBufSize(PIndexInfo pIndexInfo)
{
    int chunkIndex = pIndexInfo->chunkIndex;
    int nodeIndex = pIndexInfo->nodeIndex;

    //���������Ч��
    if (!checkIndexInfo(pIndexInfo))
    {
        return 0;
    }

    return g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].size;
}

//����������ȡԴ����
Type getDataByIndex(PIndexInfo pIndexInfo, int index)
{
    Type data = (Type)0;
    int chunkIndex = pIndexInfo->chunkIndex;
    int nodeIndex = pIndexInfo->nodeIndex;

    //���������Ч��
    if (!checkIndexInfo(pIndexInfo))
    {
        return data;
    }

    if (index < 0 || index >= g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].length)
    {
        return data;
    }

    return g_dataBuf[g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].index + index];
}

//����Id����
IndexInfo findById(int dataId)
{
    IndexInfo retIndex;
    retIndex.chunkIndex = -1;
    retIndex.nodeIndex = -1;
    int calcId = 0;

    if (dataId <= 0) //�������ݼ��
        return retIndex;

    //������ 
    for (int i = 0; i < g_chunkNum; ++i)
    {
        calcId += g_chunkInfoArr[i].nodeNum;
        if (calcId >= dataId)
        {
            int nodeIndex = dataId - (calcId - g_chunkInfoArr[i].nodeNum) - 1;//�±���-1
            // �ж�������Ч��
            if (!g_chunkInfoArr[i].nodeArr[nodeIndex].length)
                return retIndex;

            retIndex.chunkIndex = i;
            retIndex.nodeIndex = nodeIndex;
            
            return retIndex;
        }
    }
    return retIndex;
}

//����������ȡ����Id
int getDataIdByIndexInfo(PIndexInfo pIndexInfo)
{
    int dataId = 0;
    int chunkIndex = pIndexInfo->chunkIndex;
    int nodeIndex = pIndexInfo->nodeIndex;

    //���������Ч��
    if (!checkIndexInfo(pIndexInfo))
    {
        return dataId;
    }

    //������ 
    for (int i = 0; i < chunkIndex; ++i)
    {
        dataId += g_chunkInfoArr[i].nodeNum;
    }
    dataId += nodeIndex +1;

    return dataId;
}

//�������ݲ���
IndexInfo getNextFindByContent(PType content, int length)
{
    IndexInfo retIndex;
    retIndex.chunkIndex = -1;
    retIndex.nodeIndex = -1;

    //����
    while (hasNextIndexInfo())
    {
        int dataIndex = g_chunkInfoArr[g_iteator_chunkIndex].nodeArr[g_iteator_nodeIndex].index;
        int dataLength = g_chunkInfoArr[g_iteator_chunkIndex].nodeArr[g_iteator_nodeIndex].length;
        if (dataCompare(dataIndex, dataLength, content, length))
        {
            retIndex.chunkIndex = g_iteator_chunkIndex;
            retIndex.nodeIndex = g_iteator_nodeIndex;
            addIteatorIndex(); //�����ȵ�����++
            return retIndex;  //���ҵ��˳�
        }
        addIteatorIndex();
    }

    return retIndex;
}

// ɾ������
int deleteDataBuf(PIndexInfo pIndexInfo)
{
    int chunkIndex = pIndexInfo->chunkIndex;
    int nodeIndex = pIndexInfo->nodeIndex;

    //���������Ч��
    if (!checkIndexInfo(pIndexInfo))
    {
        return 0;
    }

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
                updateIndex(chunkIndex, nextIndex); //��������
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
                updateIndex(chunkIndex, nodeIndex); //��������
            }
            g_chunkInfoArr[chunkIndex].nodeNum--;
        }
    }

    return 1;
}

//��������
int updateDataBuf(PIndexInfo pIndexInfo, PType content, int conLen)
{

    int chunkIndex = pIndexInfo->chunkIndex;
    int nodeIndex = pIndexInfo->nodeIndex;

    //���������Ч��
    if (!checkIndexInfo(pIndexInfo))
    {
        return 0;
    }

    int index = g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].index;
    g_chunkInfoArr[chunkIndex].nodeArr[nodeIndex].length = conLen;
    //����Դ����
    memcpy(&g_dataBuf[index], content, sizeof(Type) * conLen);

    return 1;
}

//ͳ�Ƶ���
int* statisticalWords()
{

    memset(statisticalArr,0,sizeof(int) * LETTERS_NUM * 2);

    int totalLetters = 0;
    //������ 
    for (int i = 0; i < g_chunkNum; ++i)
    {
        
        //�������нڵ�
        for (int j = 0; j < g_chunkInfoArr[i].nodeNum; ++j)
        {
            int index = g_chunkInfoArr[i].nodeArr[j].index;
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
        }
    }

    return statisticalArr;
}

//��ȡ�����ܹ�����
int getDataTotalNum()
{
    int totalNum = 0;
    //������ 
    for (int i = 0; i < g_chunkNum; ++i)
    {
        //�������нڵ�
        for (int j = 0; j < g_chunkInfoArr[i].nodeNum; ++j)
        {
            totalNum += g_chunkInfoArr[i].nodeArr[j].length;
        }
    }

    return totalNum;
}
