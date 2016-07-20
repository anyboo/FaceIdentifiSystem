[dog]
加密狗驱动安装目录

WkRuntime32.exe，加密狗驱动安装.首先安装该驱动一次，以后只需要插入加密狗即可使用SDK。
SDK的运行必须有合法的加密狗。

[include-mt]
SDK头文件目录

[lib-mt]
SDK编译库文件目录

[bin]
SDK运行库文件目录

SDK运行必要文件:
THFaceImage.dll（人脸检测运行库)
THFeature.dll(人脸特征运行库)
vcomp110.dll(并行计算运行库)
feadb.db3a(人脸检测模型库)

[doc]
SDK文档目录

[sample]
SDK示例工程目录

Test-cam工程说明：
1.VC2008编译
2.功能说明：调用THFaceImage和THFeature SDK,利用WDM摄像头进行图像捕获显示，进行人脸检测，特征提取，特征比对

PhotoCompare工程说明：
1.VS2008编译，默认配置依赖OpenCV2.3.1
2.功能说明：调用THFaceImage和THFeature SDK，打开一张图片进行人脸检测。打开两张图片特征检测，特征提取，特征比对并输出相似度

FaceVerify1024-768工程说明：
1.VS2008编译，默认配置依赖OpenCV2.3.1
2.功能说明：调用THFaceImage和THFeature SDK，读取身份证的图片信息，提取身份证图片的人脸特征，与摄像头的人脸特征进行验证。