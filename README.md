# 基于 Qt 的 GEC6818 Linux 嵌入式智能点餐系统

本项目基于 **GEC6818 开发板（ARM）**、**Linux** 与 **Qt**，实现了一个嵌入式的智能点餐终端与上位机（Qt HMI）的完整原型。系统包含触摸屏点餐界面、订单管理、上位机管理、LED 控制以及内置的 Pinyin 输入法（EasyInput）,以及科大讯飞的语音识别。  

---

## 一、项目简介

- 嵌入式端（GEC6818）负责：发送点餐信息、控制 LED、驱动按键等硬件交互（`led.ko` 驱动）。
- Qt 端（上位机 / HMI）负责：接收并显示订单、管理订单队列、确认完成操作、提供更友好的界面与输入法。
- 通信方式：TCP Socket（JSON 或自定义文本协议）。
- 支持：触摸屏优化（800×480），内置中文输入（EasyInput），可扩展语音识别模块（如需）。
- Linux终端运行科大讯飞的识别代码

---


## 三、主要功能

- GUI：主界面（MainPage）、购物车（CartPage）、订单列表、菜品展示与交互。
- 订单管理：接收、显示、完成、移除订单；支持上位机确认完成操作。
- 网络：嵌入式端通过 TCP 将订单发送到 Qt 端，上位机监听并解析订单。
- 硬件控制：通过 `led.ko` 驱动控制开发板 LED；Qt 端可发送控制命令。
- 输入法：内置 EasyInput（基于 GooglePinyin 的字典与算法）便于触摸屏中文输入。
- 资源管理：所有图片和 UI 资源由 `tupian.qrc` 管理。
- 科大讯飞的离线语音指令识别

---

## 五、关于在linux 系统下，搭建ARM qt开发环境


对于ARM平台的qt环境，开发板上的qt环境只提供程序运行环境支持，不支持编译

<img width="370" height="146" alt="Image" src="https://github.com/user-attachments/assets/6f70b5f4-c30d-4814-97fa-fe8a90fef7b8" />


第一步：将Qt-Embedded-5.7.0.tar.xz拷贝到linux下/usr/local

<img width="554" height="92" alt="Image" src="https://github.com/user-attachments/assets/dda4fc9e-fece-4fc1-8b4c-360c088a8ce5" />


第二步：进入/usr/local目录执行解压命令

<img width="554" height="34" alt="Image" src="https://github.com/user-attachments/assets/9acc24c6-f9a1-4272-b249-e56c132b3b15" />


第三步：进入qt所在目录 ，查看qmake文件

<img width="554" height="79" alt="Image" src="https://github.com/user-attachments/assets/d848ce6f-0131-466d-8ef6-9998e1b3a067" />


第四步：在qt工程目录下，以绝对路径的方式来执行qmake命令，不要在任意的路径下来执行qmake，因为，linux操作系统下，本身也支持qt

<img width="554" height="139" alt="Image" src="https://github.com/user-attachments/assets/1f9a0de5-1b80-4f29-8596-f82b73155196" />


第五步：在工程下，执行make命令来编译工程

<img width="554" height="78" alt="Image" src="https://github.com/user-attachments/assets/a154a88c-12b4-4c96-8a84-cd9001ffe193" />


(QT修改界面与结果不同步的问题：两种编译相互影响)

第一步：

make clean

第二步：

重新构建(不是重新编译)

第三步：

QT编译 

编译工程时报错：

<img width="554" height="151" alt="Image" src="https://github.com/user-attachments/assets/5021052a-ff31-42fa-b3d6-97d648104856" />


解决办法：

sudo ln -s /usr/lib/x86_64-linux-gnu/libmpfr.so.6 /usr/lib/x86_64-linux-gnu/libmpfr.so.4

第六步：在工程目录下，利用file命令来查看可执行文件的格式

<img width="554" height="46" alt="Image" src="https://github.com/user-attachments/assets/80c659df-be40-472c-9812-568828411607" />


第七步：将可执行程序利用串口工具上传到开发板

第八步：修改权限并运行程序

chmod 777 day2_homework2

./day2_homework2

在移植qt程序时，可能出现的问题：

<img width="554" height="69" alt="Image" src="https://github.com/user-attachments/assets/90fe8bd1-2482-4ad7-b3ba-99bf49c39e03" />


完成之后在qt的工程目录下/usr/local/Qt-Embedded-5.7.0/bin/qmake

会生成一个makefile，make即可(如果在qt软件编译过，需要重构之后再make clean，再make)


## 六、LED驱动安装
led灯显示的步骤如下：

1、将led_drv.ko驱动文件下载到开发板中

2、下载完成之后，在开发板中使用命令 安装驱动文件

insmod  led_drv.ko  --后面把这个安装的命令写入到/etc/profile脚本文件中

3、安装完成之后，使用lsmod去查看是否安装成功 或者 去根目录下/dev 里面去查看

[root@GEC6818 ~]#insmod  led_drv.ko
[root@GEC6818 ~]#lsmod
led_drv 2871 0 - Live 0xbf684000 (O)
cc2530_drv 5650 0 - Live 0xbf227000 (O)
rtl8723bu_wifi 1810467 0 - Live 0xbf021000 (O)
gec6818_humidity 3339 0 - Live 0xbf01d000 (O)
stepmotor 4252 0 - Live 0xbf018000 (O)
relay 1244 0 - Live 0xbf014000 (O)
led 1593 0 - Live 0xbf010000 (O)
gec6818_beep 1551 0 - Live 0xbf00c000 (O)
gas_drv 2257 0 - Live 0xbf008000 (O)
dc_motor 1802 0 - Live 0xbf004000 (O)
buttons_drv 3028 0 - Live 0xbf000000 (O)

## 七、科大讯飞使用方法

首先打开科大讯飞官网，获取自己的SDK，将我的SDK移植，不要用我的SDK，不知道什么时候会失效。

进入ubuntu

gec@ubuntu:/mnt/hgfs/GZ2523/qt/Project/111-科大讯飞/Linux_aitalk_exp1227_50e0c9
f8/samples/asr_offline_sample$ 
--------------------------------------------
f8/samples/asr_offline_sample$ ls
32bit_make.sh  asr_offline_sample.c        asr_offline_sample.o  msc
64bit_make.sh  asr_offline_sample.c新下载  Makefile
--------------------------------------------
因为我的Ubuntu是64位的，使用./64bit_make.sh

会报很多警告，是正常现象，只要没有错误就行。

然后链接动态库，每次都需要链接(非常关键，负责执行不了)
在这个路径下:
gec@ubuntu:/mnt/hgfs/GZ2523/qt/Project/111-科大讯飞/Linux_aitalk_exp1227_50e0c9
f8/samples/asr_offline_sample$ 

export LD_LIBRARY_PATH=$(pwd)/../../libs/x64/

链接成功之后会在这个路径下创建asr_offline_sample，这个就是我们需要的可执行文件

gec@ubuntu:/mnt/hgfs/GZ2523/qt/Project/111-科大讯飞/Linux_aitalk_exp1227_50e0c9
f8/bin$ ls
asr_offline_sample  call.bnf  call.bnf拷贝  call.bnf新下载  msc  wav


