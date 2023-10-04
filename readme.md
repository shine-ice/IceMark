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