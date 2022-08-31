#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <io.h>
#include "opencv.hpp"
#include "highgui.hpp"
#include "fileList.h"
#include "SEMImage.h"
using namespace std;
using namespace cv;

/*开发Memo
    Features
        导入 √
        前处理框架 √
        前处理通用参 √
        识别与标记 √
        窗体与调参
        手动删除
        导出数据 √
        保存识别后图片和log √
    优化
        导入过程后缀筛选
        前处理选项化、允许调参
        手动调整
        边缘点霍夫阈值调整
        前处理提升
            selectiveBlur:
            https://www.cnblogs.com/Imageshop/p/4694540.html
        实时统计
        导出log
        识别文件头
        模板匹配文件头（腐蚀膨胀->ROI->模板识别）
            https://blog.csdn.net/weixin_41709536/article/details/100628973
            https://blog.csdn.net/wsp_1138886114/article/details/118991374
        回退
        Resize
        SetDstDir
        SortPoints
        Back
        Prettier
        UI
        读写配置文件/Qt配置选项
            https://zhuanlan.zhihu.com/p/535018905
            https://blog.csdn.net/weixin_44342705/article/details/120554362
    识别算法
        Demo: Canny + Hough
        v1: 直方图归一化 + Sobel + Hough
        v2: SurfaceBlur + Sobel + Hough
        v3: 手动阈值二值化 + 主动轮廓
            https://blog.csdn.net/weixin_40054912/article/details/82658934
        v4: 模板匹配（不推荐）
*/

//分割线不需要双斜线，大小写不敏感，兼容文件、文件夹
//本文件夹用".",上级文件夹输入".."即可，不用全部输入
//输入文件夹/文件字符串，返回vector<SEMImage> imageList
void import(string srcString, vector<SEMImage> &imageList)
{
    fileList srcList(srcString);
    vector<string> temp = srcList.exportList();
    for (int i = 0; i < temp.size(); i++)
        imageList.push_back(SEMImage(temp.at(i)));
}

int main()
{
    //导入模块，输入目标文件夹字符串，传入所有照片信息
    string srcStr;
    cout << "请输入目标路径：";
    cin >> srcStr;

    vector<SEMImage> imgList;
    import(srcStr, imgList);
    for(int i=0; i<imgList.size(); i++)
    {
        imgList.at(i).preprocess();
        imgList.at(i).recon();
        imgList.at(i).drawCircles();
        imgList.at(i).viewImg(3);
        imgList.at(i).exportData();
        imgList.at(i).exportImage();
        imgList.at(i).exportLog();
    }
    //will be called after waitKey
    return 0;
}