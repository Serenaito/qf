# Quiet

#### 介绍
windows10 64位系统桌面看板娘

#### 软件架构
QT + live2d


#### 安装教程

1.  下载
2.  解压
3.  点击quiet.exe

#### 添加模型
~~只支持V3的live2d模型~~
引用库已更新
1. 将下载好的模型的文件夹放入Resources路径
2. 修改config.json文件，例如新添加模型的文件夹名为ModelA
3. 原始的config.json内容为
```
	{
		"systemtray":	"icon.PNG",
		"module":	[ {
				"name":	"LSS",
				"window_x":	45,
				"window_y":	640
			},{
				"name":	"Hiyori",
				"window_x":	1659,
				"window_y":	668
			}],
		"userdata":	{
			"current_model":	"LSS",
			"top":	false,
			"move":	false
		}
	}
```	
4. 修改后为
```
	{
		"systemtray":	"icon.PNG",
		"module":	[ {
				"name":	"LSS",
				"window_x":	45,
				"window_y":	640
			},{
				"name":	"Hiyori",
				"window_x":	1659,
				"window_y":	668
			},
			{
				"name":	"ModelA",
				"window_x":	-1,
				"window_y":	-1
			}],
		"userdata":	{
			"current_model":	"LSS",
			"top":	false,
			"move":	false
		}
	}
```

#### 相关文档
https://zhuanlan.zhihu.com/p/511077879

#### 效果演示视频
https://www.bilibili.com/video/BV13q4y1m7ax/

#### 模型来源说明
1. Hiyori模型来源于live2d官方网站(https://www.live2d.com/)
2. LSS模型来源于B站up主(原帖 https://www.bilibili.com/video/BV1KU4y1x7ep)


#### 最后
该软件没经过充分测试,肯定会有很多BUG，就是个玩具级别的应用，图一乐。
