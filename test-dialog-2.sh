#!/bin/bash

# 使用 dialog 创建菜单
result=$(dialog --menu "选择一个操作" 10 30 3 1 "查看版本" 2 "运行" 3 "激活" --stdout)

# 根据用户选择执行相应的操作
case $result in
    1)
        echo "查看版本被选择"
    ;;
    2)
        echo "运行被选择"
    ;;
    3)
        echo "激活被选择"
    ;;
    *)
        echo "未知选择"
    ;;
esac
