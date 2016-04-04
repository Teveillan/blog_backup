title: "Mac OS Sublime Text 3 C/C++ 配置"
date: 2016-03-30 16:57:59
tags: [MacOS, SublimeText]
---


## **安装sublime text 3 插件**

**1.安装 Package Control组件：**
*  按 control+` (键盘上1左边的按钮)调出console控制台;
*  粘贴以下代码到底部命令行并回车:

<!--more-->
```python
import urllib.request,os; pf = 'Package Control.sublime-package'; ipp = sublime.installed_packages_path(); urllib.request.install_opener( urllib.request.build_opener( urllib.request.ProxyHandler()) ); open(os.path.join(ipp, pf), 'wb').write(urllib.request.urlopen( 'http://sublime.wbond.net/' + pf.replace(' ','%20')).read())
```

**2.用Package Control安装插件的方法**
* `Preference -> Package Control`打开控制栏，输入`install Package`回车。
 
**3.常用插件**
1. `SublimeCodeIntel`:  代码提示及自动补全
2. `SideBar Enhancements`: 改进侧边栏
3. `SideBar Folders`: 增加了侧边栏对于文件夹的支持

暂时没用到这么多..以后慢慢补吧...

<br /> 


##  **内置输入数据**

本以为上面的弄完就万事大吉，但在编译运行的时候发现一个问题：运行时无法在控制台输入数据。愁。

解决办法：
`Tools -> Build System -> New Build System` 新建编译系统。在新打开的窗口中替换为以下代码

```json
{
    "cmd": ["bash", "-c", "g++ '${file}' -o '${file_path}/${file_base_name}' && osascript -e 'tell application \"Terminal\" to activate do script \"clear&&${file_path}/${file_base_name} && read -p \\\"Press Enter to exit.\\\"&&exit\"'"],
    "file_regex": "^(..[^:]*):([0-9]+):?([0-9]+)?:? (.*)$",
    "working_dir": "${file_path}",
    "selector": "source.c, source.c++",

    "variants":
    [
        {
            "name": "Run",
            "cmd": ["bash", "-c", "g++ '${file}' -o '${file_path}/${file_base_name}' && '${file_path}/${file_base_name}'"]
        }
    ]
}
```
Command + s 保存，命名为MyC++。
在`Tools -> Build System`选中`MyC++`。
`Command + Shift + B`编译，选`MyC++`（不加Run的那个）。

搞定~



