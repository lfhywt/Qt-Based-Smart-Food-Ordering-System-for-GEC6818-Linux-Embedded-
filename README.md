# 基于 Qt 的 GEC6818 Linux 嵌入式智能点餐系统

本项目基于 **GEC6818 开发板（ARM）**、**Linux** 与 **Qt**，实现了一个嵌入式的智能点餐终端与上位机（Qt HMI）的完整原型。系统包含触摸屏点餐界面、订单管理、上位机管理、LED 控制以及内置的 Pinyin 输入法（EasyInput）。  

此 README 为中文完整版，含完整目录结构、构建与运行说明、通信协议示例和常见问题解答，便于直接放到 GitHub 仓库首页。

---

## 一、项目简介

- 嵌入式端（GEC6818）负责：发送点餐信息、控制 LED、驱动按键等硬件交互（`led.ko` 驱动）。
- Qt 端（上位机 / HMI）负责：接收并显示订单、管理订单队列、确认完成操作、提供更友好的界面与输入法。
- 通信方式：TCP Socket（JSON 或自定义文本协议）。
- 支持：触摸屏优化（800×480），内置中文输入（EasyInput），可扩展语音识别模块（如需）。

---

## 二、项目目录（与你提供的 tree 完全对应）
├── 11.png
├── 1.jpg
├── 2.jpg
├── 3-removebg-preview.png
├── background.jpg
├── cartmanager.cpp
├── cartmanager.h
├── cartmanager.ui
├── cartpage.cpp
├── cartpage.h
├── cartpage.ui
├── easyInput
│ ├── dict_pinyin.dat
│ ├── dict_pinyin_user.dat
│ ├── easyInput.cpp
│ ├── easyinput.h
│ ├── easyInput.pri
│ ├── easyInput.qrc
│ ├── easyInput.ui
│ ├── googlepinyin
│ │ ├── atomdictbase.h
│ │ ├── dictbuilder.cpp
│ │ ├── dictbuilder.h
│ │ ├── dictdef.h
│ │ ├── dictlist.cpp
│ │ ├── dictlist.h
│ │ ├── dicttrie.cpp
│ │ ├── dicttrie.h
│ │ ├── lpicache.cpp
│ │ ├── lpicache.h
│ │ ├── matrixsearch.cpp
│ │ ├── matrixsearch.h
│ │ ├── mystdlib.cpp
│ │ ├── mystdlib.h
│ │ ├── ngram.cpp
│ │ ├── ngram.h
│ │ ├── pinyinime.cpp
│ │ ├── pinyinime.h
│ │ ├── searchutility.cpp
│ │ ├── searchutility.h
│ │ ├── spellingtable.cpp
│ │ ├── spellingtable.h
│ │ ├── spellingtrie.cpp
│ │ ├── spellingtrie.h
│ │ ├── splparser.cpp
│ │ ├── splparser.h
│ │ ├── sync.cpp
│ │ ├── sync.h
│ │ ├── userdict.cpp
│ │ ├── userdict.h
│ │ ├── utf16char.cpp
│ │ ├── utf16char.h
│ │ ├── utf16reader.cpp
│ │ └── utf16reader.h
│ ├── googlepinyin.cpp
│ ├── googlepinyin.h
│ ├── image
│ │ ├── delete_white.png
│ │ └── upper_white.png
│ └── ui_easyInput.h
├── led.ko
├── ledqt.cpp
├── ledqt.h
├── main.cpp
├── mainpage.cpp
├── mainpage.h
├── mainpage.ui
├── mainwindow.cpp
├── mainwindow.h
├── mainwindow.ui
├── Makefile
├── m.png
├── p1.png
├── p2.png
├── p3.png
├── p4.png
├── p5.png
├── p6.png
├── p7.png
├── p8.png
├── p9.png
├── page1.cpp
├── page1.h
├── page1.ui
├── Project
├── Project.pro
├── Project.pro.user
├── top.png
└── tupian.qrc



---

## 三、主要功能

- GUI：主界面（MainPage）、购物车（CartPage）、订单列表、菜品展示与交互。
- 订单管理：接收、显示、完成、移除订单；支持上位机确认完成操作。
- 网络：嵌入式端通过 TCP 将订单发送到 Qt 端，上位机监听并解析订单。
- 硬件控制：通过 `led.ko` 驱动控制开发板 LED；Qt 端可发送控制命令。
- 输入法：内置 EasyInput（基于 GooglePinyin 的字典与算法）便于触摸屏中文输入。
- 资源管理：所有图片和 UI 资源由 `tupian.qrc` 管理。

---

## 四、构建与运行（Qt 端）

**环境要求（开发机）**
- Qt 5.x 或 Qt 6.x（建议使用 Qt Creator）
- Linux / Windows 均可编译 Qt 程序（嵌入式需交叉编译）

**在开发机（主机）编译运行**
1. 使用 Qt Creator 打开 `Project.pro`。
2. 配置合适的 Kit（Desktop）并执行 `Build` → `Run`。
3. 可直接运行生成的可执行文件（例如 `./Project` 或 Qt Creator 运行按钮）。

**注意**
- `Project.pro.user` 为 Qt Creator 的本地配置文件，不用提交到远端仓库。
- 如果使用 Windows 开发但最终运行在嵌入式上，请使用交叉编译（见下）。

---

## 五、交叉编译与嵌入式端（GEC6818）

**交叉编译（示例）**
- 假设交叉工具链为 `arm-linux-gnueabi-g++` / `arm-linux-gnueabi-gcc`，并已配置 qmake 或 CMake 的交叉 Kit。
- 在交叉环境中编译 Qt 程序或仅编译嵌入式端的 C 程序（如 TCP 客户端）。

**部署**
```bash
# 交叉编译后，将二进制拷贝到开发板
scp Project root@<GEC6818_IP>:/home/root/

# 在开发板上运行（需可执行权限）
chmod +x /home/root/Project
/home/root/Project
