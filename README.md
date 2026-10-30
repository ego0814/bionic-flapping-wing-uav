# 高性能微型仿生扑翼无人机系统

本仓库归档“高性能微型仿生扑翼无人机系统研究”项目的控制程序与 SolidWorks 结构模型，便于检索、版本追溯、复现实验和后续迭代。

- **项目类型**：挂绳试飞版本
- **代码平台**：Arduino IDE / ESP8285
- **模型格式**：SolidWorks `.SLDPRT`、`.SLDASM`
- **交换格式**：STEP，作为 [v1.0.0 Release](https://github.com/ego0814/bionic-flapping-wing-uav/releases/tag/v1.0.0) 附件提供

## 内容索引

| 路径 | 内容 |
| --- | --- |
| [`code/flapping_controller_1_0/`](code/flapping_controller_1_0/) | 从结题报告附件提取的 161 行 Arduino 控制程序 |
| [`models/solidworks/`](models/solidworks/) | 原始 SolidWorks 零件、装配体与参数文本，共 50 个模型文件 |
| [`models/step/`](models/step/) | STEP 交换格式下载说明 |
| [`docs/SOURCE.md`](docs/SOURCE.md) | 文件来源、整理规则与未上传内容说明 |
| [`docs/MODEL_CATALOG.md`](docs/MODEL_CATALOG.md) | SolidWorks 模型清单与大小 |
| [`manifests/SHA256SUMS.txt`](manifests/SHA256SUMS.txt) | 本仓库文件及发行包校验值 |

## 控制程序

主程序：[`code/flapping_controller_1_0/flapping_controller_1_0.ino`](code/flapping_controller_1_0/flapping_controller_1_0.ino)

程序用于接收 A8S 接收机输出的 8 通道 PPM 信号，并驱动两路 FS-K08 舵机完成双翼扑动。代码中的主要控制关系为：

- 通道 1：左右翼差动/转向；
- 通道 2：升降偏置；
- 通道 3：油门、扑翼频率及幅度混控；
- 通道 5、6：两侧舵机中位微调；
- 通道 7：三档扑动幅度；
- 通道 8：升降倍率切换；
- PPM 输入：引脚 6；
- 舵机输出：A0、A1。

详细说明见 [`code/README.md`](code/README.md)。

## 模型与下载

SolidWorks 原生文件位于 [`models/solidworks/`](models/solidworks/)，保留原始目录层级，包括根目录的主模型和 `打印/` 下的打印版本。

STEP 文件不直接放入 Git 历史，避免仓库体积翻倍；21 个 STEP 文件已拆分为 7 个独立可解压的发行版附件（每个约 3.1-6.4 MB）：

- [下载 v1.0.0 发行版](https://github.com/ego0814/bionic-flapping-wing-uav/releases/tag/v1.0.0)
- 附件命名：`STEP-models-pack-01.zip` 至 `STEP-models-pack-07.zip`

打开装配体时，建议先在 SolidWorks 中设置模型搜索路径为本目录，或保持本仓库的目录结构不变。

## 可追溯标识

- 仓库：<https://github.com/ego0814/bionic-flapping-wing-uav>
- 发行标签：`v1.0.0`
- 默认分支：`main`
- 完整文件校验：见 [`manifests/SHA256SUMS.txt`](manifests/SHA256SUMS.txt)

## 来源与范围

代码和模型均来自本项目现有结题材料。结题报告正文包含个人身份信息，因此**未上传报告原文**；仅提取并发布报告附件中的控制代码，以及指定的 SolidWorks 模型目录。

## 使用提示

- 代码保持从报告附件提取时的行为与写法，未擅自修改控制参数。
- 首次上电或修改参数前，应拆除桨叶/翼面并做台架测试，确认舵机中位、方向和行程。
- SolidWorks 文件来自实际项目归档，部分文件可能依赖特定版本或外部参考；若提示参考丢失，请保持原目录结构或在 SolidWorks 中重新指定搜索路径。
