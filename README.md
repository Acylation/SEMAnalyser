# SEMAnalyser
Circle recon for sem
## 开发Memo
### Features
- 导入 √
- 前处理框架 √
- 前处理通用参(Stage1: GaussianBlur+Canny; Stage2: SurfaceBLur+Sobel)
- 识别与识别调参
- 窗体模块
- 手动删除
- 导出数据 √
- 保存 √
### 优化
- 导入过程后缀筛选
- 前处理选项化、允许调参
- 手动调整
- 边缘点霍夫阈值调整
- 实时统计
- 导出log
- 识别文件头
- 模板匹配文件头（腐蚀膨胀->ROI->模板识别）
- 回退
- Resize
- SetDstDir
- SortPoints
- Back
- Prettier
- 读写配置文件/Qt配置选项
