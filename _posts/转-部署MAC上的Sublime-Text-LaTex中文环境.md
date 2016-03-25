title: "[转]部署MAC上的Sublime Text+LaTeX中文环境"
date: 2015-05-11 14:35:22
categories: Tech
tags: [MacOS, LaTeX]
description: 
---

原文链接：http://www.readern.com/sublime-text-latex-chinese-under-mac.html
作者：Read ERN

LaTex就不多做介绍了，其实一般人用到的机会也不多（包括我）。只是这段时间在Coursera上新课程的时候，偶尔需要，所以从零开始摸索了一下，也顺便进一步发现了Sublime Text的强大。

MAC上的LaTex部署，一般是两条思路，一个是直接安装MacTex，自带编写环境什么的；另一个是Sublime Text（或者Textmate等其他编辑器）+LaTexTools插件。方式二其实也是需要依赖MacTex的，但相对来说体量小、不需要经常切换编辑器，所以果断的介绍方式二。

亲测：MacOS 10.10, Sublime Text 3可用。（2015年5月）

<!-- more -->
###安装步骤
1. 首先需要安装Sublime Text 3和[Package Control](https://packagecontrol.io/installation)，这个有大把的帖子。
2. MacTex现在是一个2G+的大包子，其实里面很多东西我们不需要，所以本着节约精神，我们安装[MacTex_Basic](http://mirror.ctan.org/systems/mac/mactex/mactex-basic.pkg)包就行了，现在的版本大概是100M以内。这个安装也是傻瓜的。
3. 在Sublime Text里`Command+Shift+P`调出命令窗口，输入`Install`，之后选择LaTexTools，网络OK的话，很快就完成了插件安装。
4. LaTexTools插件会在编译你的Tex文件后，调用Skim这个PDF阅读器打开编译出的PDF文件，因此你还需要安装[Skim](http://skim-app.sourceforge.net/).
5. 运行一下Skim，进入`偏好设置——同步`，在`PDF-Tex同步支持`那里选择`Sublime Text`。这样，当你在Sublime Text里修改tex文件时，Skim预览也会相应变更。
6. 完成上面所有步骤，其实就已经搭建完成基本环境。创建一个test.tex文档，贴上我后面附的测试代码，保存一下。(原Po说这里可以开始编译了，亲测是不行的，等完成第七步再编译。)
7. 让我们最后来修改编译和中文环境:
打开编辑`~/Library/Application\ Support/Sublime\ Text/Packages/Users/LaTeXTools.sublime-settings`，将其中"builder"条目改为：
`"builder": "simple"`
接着打开编辑`~/Library/Application\ Support/Sublime\ Text\ 3/Packages/LaTeXTools/builders/simpleBuilder.py`
找到其中大概第41行
`pdflatex = ["dflatex", "-interaction=nonstopmode", "-synctex=1"]`
修改为
`pdflatex = ["xelatex", "-interaction=nonstopmode", "-synctex=1"]`
保存后关闭，`Command + B`编译一下，一切OK！

**补充**:
配置好后，如果编译的时候提示:
> LaTeXTools: you need to migrate your preferences. See the README file for instructions.

调出命令窗口(`Command+Shift+P`), 输入如下命令: `LaTeXTools: Reconfigure and migrate setting`.

最后再给一个个性化设置。上面写了，LaTeXTools默认调用Skim，这一方面是因为LaTeXTools是跨平台的插件，Skim也同样跨平台开源，另一方面Skim确实对LaTeX的PDF支持很好。但是用OS X的网友们其实还有个选择，就是调用系统默认的PDF程序来打开编译后的成果，特别是“预览”还是很好用的。目前LaTeXTools的开发者还没有给出设置方法，所以，仍然要靠改代码：
修改`~/Library/Application\ Support/Sublime\ Text\ 3/Packages/LaTeXTools/jumpToPDF.py`, 将其中`if plat == 'darwin':` 之后开始的两段直接注释掉，替换为：
`subprocess.Popen([‘open’] + [pdffile])`
同样保存后就生效啦。目前用起来，系统自带的预览还是能满足我的需求滴。

测试代码：
```latex
\documentclass{article}
\usepackage{fontspec, xunicode, xltxtra}  
\setmainfont{Hiragino Sans GB}  

\title{Title}
\author{}

\begin{document}

\maketitle{}

\section{Introduction}

This is where you will write your content. 在这里写上内容。

\end{document}
```

###LaTex的其他选择

如果只是偶尔需要输入公式，这里介绍一个在线的LaTex公式编辑器http://www.codecogs.com/latex/eqneditor.php

有iPad的，你们有福了，还有个手写公式后自动识别、生成LaTex公式的App：[Script MathPad](https://itunes.apple.com/us/app/myscript-mathpad-handwriting/id674996719?mt=8)。简单的使用已经足够了，如果需要经常使用，导出功能需要内购（4.99刀）。顺便说一句，这家公司的手写App都很强大，手写计算器、手写Notes都挺好用，而且不贵。

####参考链接：
MacTex页面：http://tug.org/mactex/morepackages.html

Skim官网：http://skim-app.sourceforge.net

LaTexTools官网：https://github.com/SublimeText/LaTeXTools

前六步骤的具体介绍：http://economistry.com/2013/01/installing-and-using-latex-for-mac/ 不过需要注意，这里是用了完整的MacTex

发现xelatex是受了这里的启发：http://www.teeboneding.com/blog/2013/06/01/write-ntu-master-thesis-with-xelatex-template-on-mac/ 但需要注意，这里的编译代码测试无效。

第七步受了这里的启发：https://github.com/SublimeText/LaTeXTools/issues/303

