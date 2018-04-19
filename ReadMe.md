## 特点 Feature:
windbg汇编和命令窗口文本高亮插件，支持x86和x64
A windbg plugin to hiligth text in Disassembly and Command windows. Support x86 and x64.

## 下载 Download:
http://pan.baidu.com/s/1gd3i8i3

## 安装&使用：
将hs.dll放入32位windbg的安装目录，hs64.dll放入64位(x64)windbg的安装目录，
运行windbg，输入命令
.load hs
64位是
.load hs64

please put the dll file in to windbg installed directory.
.load hs
or x64:
.load hs64

## 自定义 Customize:
如果你想自定义syntax颜色，只需要以管理员权限运行windbg并加载windbg_hilight插件，这样会在windbg的安装目录里产生一个windbg_hilight.ini，参考http://www.w3schools.com/tags/ref_colorpicker.asp 获取你想要的颜色，并转成十进值填写到ini中。
If you want to customize the color of syntax, please run windbg in administrator privileage and .load hs(64), then you will get windbg_hilight.ini in windbg installed diretory. Change the value in color section, please refer http://www.w3schools.com/tags/ref_colorpicker.asp to get the hex color value and write the corresponding decimal value.

## 更新记录 Changelog:
- 2014年5月10日：
add patch by goldenegg: 去除对boost的依赖，允许dll卸载(2013-4-30)
新的配置文件更名为windbg_hilight.ini，如果以前配置过asm.ini请复制配置到windbg_hilight.ini，但不要直接更名asm.ini为windbg_hilight.ini，因为keywords.registers那项有更改
修复内存泄露，添加x64寄存器高亮
fixed issues Memory Leak, add register syntax for x64
- 2013年4月26日：
增加地址、opcode、jumpcall指令的颜色设置
增加命令窗口用户输入的颜色设置(只是简单地对>后的文字进行加色)
- 2013年4月25日：
首个版本发布，暂时只有Disassembly窗口的高亮

### 支付宝赞助
![支付宝赞助](https://github.com/lynnux/lynnux.github.io/blob/master/alipay.png)

### 微信赞助
![微信赞助](https://github.com/lynnux/lynnux.github.io/blob/master/weixin.png)

### 感谢 Greets:
感谢linx2008，开发高亮插件，但没有x64版本，http://hi.baidu.com/linx2008/item/0bee3aedc6d49e275b2d6441
感谢goldenegg，解决.unload崩溃，以及去掉boost和windbg sdk依赖


by lynnux
CopyRight (C) 2013-2018
started from 2013/4/25

