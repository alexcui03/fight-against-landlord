# 斗地主

SJTU CS1501 程序设计思想与方法（C++）小组大作业

Last Update: 2022.12.15

使用 cmake 构建本程序
```
mkdir build && cd build
cmake ..
cmake --build .
```

为了测试需要，显示其他玩家的牌可以在 `CMakeLists.txt` 中取消注释以下行：
```
add_definitions(-DSHOW_ALL_CARDS)
```
