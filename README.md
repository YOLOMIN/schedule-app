# schedule-app（日程规划程序）

> 用 C++ / Qt 6 开发的桌面日程规划与提醒工具。

## 项目说明

- 核心功能：待办/日程清单 + 到期提醒弹窗 + 系统托盘常驻。
- 数据存储：先用 SQLite（Qt 内置驱动），后续可迁移到 MySQL。

## 技术栈

- 语言：C++
- GUI 框架：Qt 6.11.2（Widgets）
- 构建：CMake + MinGW 13.1.0 (64-bit)
- IDE：Qt Creator 20.0.1

## 环境

- Qt 安装路径：`F:\ProgramFiles\qt\`
- Kit：Desktop Qt 6.11.2 MinGW 64-bit

## 目录结构

TODO

## 进度记录

| 日期 | 内容 | 状态 |
| --- | --- | --- |
| 2026-09-05 | 确定 C++ Qt 方案，安装 Qt 6.11.2 + MinGW | ✅ |
| 2026-09-06 | 新建 Qt Widgets 项目，迁移到英文路径 | 进行中 |

## 构建

用 Qt Creator 打开 `CMakeLists.txt`，选择 MinGW 64-bit kit，点击运行。
