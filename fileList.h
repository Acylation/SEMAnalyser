#include <iostream>
#include <string>
#include <vector>
#include <io.h>
using namespace std;

//写class解决递归使用vector的问题 https://zhuanlan.zhihu.com/p/464734574 
class fileList
{
private:
    vector<string> m_pathList; //每个元素记录一个文件的路径
    string m_folderDir;        //需要导入的文件夹
    bool m_recursion = 1;      //参数，是否递归，1为递归获取文件夹下的所有文件，0是仅读取本层文件
    // load 配置项

    // criterions
    bool isFile(string);
    bool isSubFolder(_finddata_t);    //通过attrib参数判断是否为sub文件夹
    bool validSubFolder(_finddata_t); //排除文件夹名"."（本层文件夹）和".."(上层文件夹)，其他文件夹有效
    void getFilePath(string);         // core

public:
    fileList(string); //构造时初始化
    ~fileList() {}
    vector<string> exportList();
};

fileList::fileList(string srcDir) //构造时初始化
{
    m_folderDir = srcDir;
    getFilePath(m_folderDir);
}

vector<string> fileList::exportList()
{
    return m_pathList;
}

bool fileList::isFile(string Str)
{
    _finddata_t item;
    intptr_t HANDLE = _findfirst(Str.c_str(), &item);
    if (HANDLE != -1L)
        return true;
    _findclose(HANDLE);
    return false;
}

bool fileList::isSubFolder(_finddata_t item)
{
    if (item.attrib & _A_SUBDIR)
        return true;
    else
        return false;
}

bool fileList::validSubFolder(_finddata_t folder)
{
    if ((strcmp(folder.name, ".") != 0) && (strcmp(folder.name, "..") != 0))
        return true;
    else
        return false;
}

void fileList::getFilePath(string srcDir)
/*
    Reference: https://blog.csdn.net/xiexu911/article/details/79990774
    CSDN User: nancheng911
    使用io.h中的_findfirst/_findnext读母文件夹下的所有文件（递归）
    关键：_finddata_t结构体
*/
{
    _finddata_t item;
    intptr_t HANDLE;
    string specifiedDir = srcDir + "\\*.bmp";
    HANDLE = _findfirst(specifiedDir.c_str(), &item);
    if (HANDLE == -1L)
    {
        if (isFile(srcDir))
        {
            m_pathList.push_back(srcDir);
            return;
        }
        cout << "路径错误" << endl; //该路径下没有子项目，且不是文件
        return;
    }
    do
    {
        if (!isSubFolder(item)) //路径正确，又不是subFolder，所以是subFile或者".",".."
            m_pathList.push_back(srcDir + "\\" + item.name);
        else if (m_recursion && validSubFolder(item))
            getFilePath(srcDir + "\\" + item.name);
    } while (_findnext(HANDLE, &item) == 0);
    _findclose(HANDLE);
}