[原文](https://zhuanlan.zhihu.com/p/637848680)

早期在markdown语法还没有推出来之前，编写blog是在网页上或olw写的，也就是文章是保存在对方的主机上。

最近计划把我在博客园的一些早期html文章转换成markdown的文件，因为md更好地保存，经过实践之后，我保留这两个工具。

> PS. 也尝试过python的脚本版本，但运行时会有报错，就省心地选用了相对稳定的工具。

我尝试的几种脚本 放在 [html2md](https://link.zhihu.com/?target=https%3A//github.com/zhaoqingqing/blog_samplecode/tree/master/build-tools/html2md)，本文提到的这两个工具是我推荐使用的。

  

## 最近更新

本文章写于2020年8月，但是最近2023年6月，我发现了一个更加好用的浏览器插件，支持把网页内容下载保存为markdown文件

edge扩展地址：[MarkDownload - Markdown Web Clipper - Microsoft Edge Addons](https://link.zhihu.com/?target=https%3A//microsoftedge.microsoft.com/addons/detail/markdownload-markdown-w/hajanaajapkhaabfcofdjgjnlgkdkknm)

那么现在我就直接使用edge的扩展，更加方便了。

  

## html2md

地址：[https://github.com/TruthHun/html2md](https://link.zhihu.com/?target=https%3A//github.com/TruthHun/html2md)

需要下载一个exe，在windows上使用，批量转换脚本如下：

```powershell
@echo on for /r %%i in (*.html) do html2md.exe %%~pi%%~ni.html %%~pi%%~ni.md pause
```

这个工具转换出来的html更加简洁，我目前是使用这个脚本进行转换

  

## pandoc

pandoc文档：[https://pandoc.org/installing.html](https://link.zhihu.com/?target=https%3A//pandoc.org/installing.html)

使用方法：

安装 pandoc，并配置环境变量，在dos下使用下列语句转换格式。

```powershell
pandoc text.html -o text.markdown
```

pandoc转换出来的html保留了原来的很多内容，比如一些特定的div #，比如这样子的

::: {.content} ::: {#outer\_postBodyPS} ::: {#postBodyPS}