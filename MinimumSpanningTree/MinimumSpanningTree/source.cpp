#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <regex>

using namespace std;

// Define
#define MAX_NODE 1024

struct stNote {
    int index;
    string sNodeName;
};

struct stWeight {
    int firstIndex;
    int secondIndex;
    int sWeight;
};

// Global variable
vector<stNote> vtNodeData;
vector<stWeight> vtWeightData;
vector<stWeight> vtMST;
int iNodeNo = 0;
int iEdgeNo = 0;
int iTotalWeight = 0;
bool bChkEnableNode[MAX_NODE];

// Extern function
std::vector<std::string> splitStringByComma(const std::string& inputString);
string mapData(int iIndex);

bool getData(std::string sFIlePath)
{
    FILE*           iFile;
    char            cBuf[50];
    vector<string>  vtData;
    stNote          sTempNode;
    stWeight        stTempWeight;
    bool            bRet = false;
    int             iCnt = 0;
    try
    {
        iFile = fopen(sFIlePath.c_str(), "r");
        if (iFile == NULL)
        {
            throw std::exception();
        }
        fgets(cBuf, sizeof(cBuf), iFile);
        int iNo;
        if (sscanf(cBuf, "%d", &iNo) == 1)
        {
            iNodeNo = iNo;
        }
        else
        {
            throw std::exception();
        }

        while (fgets(cBuf, sizeof(cBuf), iFile) != NULL)
        {
            vtData.clear();
            size_t length = strlen(cBuf);

            if (length > 0 && cBuf[length - 1] == '\n')
            {
                cBuf[length - 1] = '\0';
            }
            if (iCnt < iNodeNo)
            {
                std::string sData           = cBuf;
                vtData                      = splitStringByComma(sData);
                sTempNode.index             = stoi(vtData[0]);
                sTempNode.sNodeName         = vtData[1];
                vtNodeData.push_back(sTempNode);
            }
            else
            {
                std::string sData           = cBuf;
                vtData                      = splitStringByComma(sData);
                stTempWeight.firstIndex     = stoi(vtData[0]);
                stTempWeight.secondIndex    = stoi(vtData[1]);
                stTempWeight.sWeight        = stoi(vtData[2]);
                vtWeightData.push_back(stTempWeight);
                iEdgeNo++;
            }
            iCnt++;
        }

        fclose(iFile);
        return true;
    }
    catch (...)
    {
        if (iFile != NULL)
        {
            fclose(iFile);
            iFile = NULL;
        }
        return false;
    }
}

std::vector<std::string> splitStringByComma(const std::string& inputString)
{
    std::vector<std::string> elements;
    std::istringstream iss(inputString);
    std::string strString;

    while (std::getline(iss, strString, ',')) {
        elements.push_back(strString);
    }

    return elements;
}

bool getMinSpanningTree(int iStartNode)
{
    bChkEnableNode[iStartNode] = true;
    try
    {
        while (vtMST.size() < size_t(iNodeNo - 1))
        {
            // Chon ra canh co trong so ngan nhat co x thuoc V va y thuoc V(MST)
            int min_w = INT_MAX;
            int X = 99;
            int Y = 99;
            for (int i = 1; i <= iNodeNo; i++)
            {
                if (bChkEnableNode[i] == true)
                {
                    // duyet danh sach ke cua dinh i
                    for (int k = 1; k <= iEdgeNo; k++)
                    {
                        if (vtWeightData[k - 1].firstIndex == i)
                        {
                            int j = vtWeightData[k - 1].secondIndex;
                            int trongso = vtWeightData[k - 1].sWeight;
                            if (bChkEnableNode[j] == false && trongso < min_w)
                            {
                                min_w = trongso;
                                X = j;
                                Y = i;
                            }
                        }
                        else if (vtWeightData[k - 1].secondIndex == i)
                        {
                            int j = vtWeightData[k - 1].firstIndex;
                            int iWeight = vtWeightData[k - 1].sWeight;
                            if (bChkEnableNode[j] == false && iWeight < min_w)
                            {
                                min_w = iWeight;
                                X = j;
                                Y = i;
                            }
                        }
                        else
                        {
                            // Do Nothing
                        }
                    }
                }
            }
            vtMST.push_back({ X, Y, min_w });
            iTotalWeight += min_w;
            bChkEnableNode[X] = true; // cho dinh X vao V[MST]
        }
         return true;
    }
    catch (...)
    {
        iTotalWeight = 0;
        return false;
    }
}

bool outputData(string sOutputFile)
{
    FILE* iFile = NULL;
    try
    {
        std::ifstream fileStream(sOutputFile);

        if (!fileStream.is_open())
        {
            std::ofstream newFileStream(sOutputFile);
        }
        else
        {
            // Do Nothing
        }

        iFile = fopen(sOutputFile.c_str(), "w");
        if (iFile == NULL)
        {
            throw std::exception();
        }
        std::string strTotalWeight = to_string(iTotalWeight);
        fputs(strTotalWeight.c_str(), iFile);
        fputs("\n", iFile);

        for (size_t i = 0; i < vtMST.size(); i++)
        {
            std::string sFirstIdex      = mapData(vtMST[i].firstIndex);
            std::string sSecondIndex    = mapData(vtMST[i].secondIndex);
            fputs(sFirstIdex.c_str(), iFile);
            fputs(",", iFile);
            fputs(sSecondIndex.c_str(), iFile);
            fputs("\n", iFile);
        }
        fclose(iFile);
        return true;
    }
    catch (...)
    {
        if (iFile != NULL)
        {
            fclose(iFile);
            iFile = NULL;
        }
        return false;
    }
}
string mapData(int iIndex)
{
    string sName;
    for (size_t i = 0; i < vtNodeData.size(); i++)
    {
        if (iIndex == vtNodeData[i].index)
        {
            sName = vtNodeData[i].sNodeName;
        }
    }
    return sName;
}

int main(int argc, char* argv[])
{
    bool bRet = false;
    std::string sInputFile = argv[1];
    std::string sOutputFile = argv[2];
    //std::string sInputFile = "C:\\test\\Input.txt";
    //std::string sOutputFile = "C:\\test\\Output.txt";
    memset(bChkEnableNode, false, sizeof(bChkEnableNode));

    std::regex backslashRegex("\\\\");
    sInputFile = std::regex_replace(sInputFile, backslashRegex, "\\\\");
    sOutputFile = std::regex_replace(sOutputFile, backslashRegex, "\\\\");

    if (argc != 3)
    {
        std::cerr << "Check parameter again!!" << std::endl;
        return 1;
    }

    // Get data from input file
    bRet = getData(sInputFile);
    if (bRet == false)
    {
        std::cerr << "Get data false" << std::endl;
        return 0;
    }

    // 1 is first node
    bRet = getMinSpanningTree(1);
    if (bRet == false)
    {
        std::cerr << "Calulate MST false" << std::endl;
        return 0;
    }
    // Output data to file
    bRet = outputData(sOutputFile);
    if (bRet == false)
    {
        std::cerr << "Output data false" << std::endl;
        return 0;
    }
    std::cerr << "Success" << std::endl;
    return bRet;
}