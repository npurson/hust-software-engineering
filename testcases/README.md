## 自动测试框架

### 环境准备

1. 安装python

2. 安装依赖

```bash
pip install -r requirements.txt -i https://pypi.tuna.tsinghua.edu.cn/simple
```

### 开始测试

```bash
python3 autotest.py -d basecases -n ./richman
```

参数：

 - -d 测试文件.in .out 所在文件夹
 - -n 可执行文件
