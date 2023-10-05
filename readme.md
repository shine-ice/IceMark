# 0. 前言

基于Qt5开发图片标注程序



# 1. 标注





# X. 参考

## x.1 Clion开发Qt，配置资源文件

> [使用CMake构建Qt项目](https://blog.csdn.net/qq_40181592/article/details/101623231)
>
> [Qt_CLion中配置资源文件 ](https://www.cnblogs.com/Lhh-9999/p/16884584.html)

一定要在CMake中配置qrc文件,需要再find_packages后面

```shell
set(QRC_SOURCE_FILE res.qrc)
qt_add_resources(${QRC_SOURCE_FILE})
```

还需要将QRC文件加入编译文件列表才可以

```shell
add_executable(xxx xxx.cpp ${QRC_SOURCE_FILE})
```



## x.2 读取不同后缀的文件

> [QImageWriter QPixmap QImage 不支持jpg和tiff的问题](https://blog.csdn.net/technologyleader/article/details/89031071)
>
> [QImage 通过load打开图片失败的一种特殊情况](https://blog.csdn.net/wkr2005/article/details/90072532)
>
> [QT Qimage、QPixmap等读取图片失败的几种可能情况](https://blog.csdn.net/qq_39295271/article/details/105755167)

QImageWriter/QPixmap/QImage不能读取jpg图像问题，是因为没有把支持相应格式的动态库放到程序运行目录；

从Qt的安装目录下的plugins文件夹下拷贝文件夹imageformats到程序当前的运行目录下，重新运行你的程序，就能够正常读取jpg图像。

