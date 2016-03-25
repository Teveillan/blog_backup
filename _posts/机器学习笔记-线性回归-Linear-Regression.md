title: "机器学习笔记---线性回归(Linear Regression)"
date: 2015-7-28 20:36:26
categories: Tech
tags: 
description: 
---

作为机器学习的第一课，深刻理解线性回归将**非常**有助于我们学习后面的课程。

在上篇文章中我们将机器学习分为监督学习和非监督学习，线性回归就属于监督学习算法的一种。

依然拿上一章讲的房价预测作为我们学习线性回归的例子。
我们说过，影响房价的因素可能有很多种，如：房屋大小、周围均价、使用年限等等。
我们将这么一种因素称之为**特征（Feature）**。
对于每一组数据的每个特征值我们分别记为$x_1,x_2,x_3$...$x_n$，n为特征数。

<!--more-->
则我们建立一个数学模型，也就是假说（Hypothesis）为：
> $$h(x) = \sum_{i=0}^n \theta_ix_i = \theta^Tx$$

这里的$\theta$即为假说的参数，我们赋予它一个初始值，然后再在机器学习的过程中不断调整。（所以说机器学习的过程其实也就是个调参的过程）

至于有n个特征，i为什么从0开始，我们后面再讲。

##1.1 单变量线性回归(Linear Regression with One Variable)

最开始，我们假设影响房价的因素就一个：房屋面积。

| Size in feet$^2$(x)| Price in 1000's(y)|
|:----------:| :-----:|
|852|178|
|1534|315|
|1416|232|
|2104|460|

则对于一组参数$\theta$,我们Hypothesis定义为：
> $$h_\theta(x) = \theta_0 + \theta_1x_1$$

$\theta_0$是我们加的一个辅助变量，没有实际意义。为了规范，我们令$x_0 = 1$，则：
> $$ h\_\theta(x) = \theta\_0x\_0 + \theta\_1x\_1  = \sum\_{i=0}^n\theta\_ix\_i = \theta^Tx $$


##1.2 成本函数(CostFunction)
如何衡量这一组参数的好坏呢？计算它预测结果的误差大小就好了。
在课程中，我们用$J(\theta)$来表示一组$\theta$的Cost。
> $$ J(\theta) = \frac{1}{2m} \sum\_{i=0}^{m} Cost(x^{(i)}, y^{(i)}) = \frac{1}{2m} \sum\_{i=0}^m(h_\theta(x^{(i)})    - y^{(i)})^2   $$ 

有了成本函数，也就知道如何衡量$(\theta_0,\theta_1)$的好坏。剩下的只用调整$\theta$使得$J(\theta)$最小就好了。

##1.3 梯度下降(Gradient descent)

如何最小化 $J(\theta)$？

我们首先来直观地感受一下对于不同的$(\theta_0,\theta_1)$，$J(\theta)$的大小是怎么变化的。

{% qnimg teveillan/images/machine-learning2-1.png title:machine-learning alt:machine-learning extend:?imageView2/2/w/600 %}

如果我们最初设定的$\theta$使得$J(\theta)$恰好在山谷的位置，则皆大欢喜，此时的$J(\theta)$就是最小值，是最优解。

如果不幸选在山峰的位置，那么我们就要对$\theta$一点一点进行修改，使得$J(\theta)$一步一步走向山谷。这种一步一步修改$\theta$使得$J(\theta)$逐步走下山的过程我们称之为**梯度下降**，下降$J(\theta)$的值。

就像下图：

{% qnimg teveillan/images/machine-learning2-1.png title:machine-learning alt:machine-learning extend:?imageView2/2/w/600 %}


怎么下降？

想要下降，首先得确定下降的方向。然后确定下降的幅度。

从数学上讲，假如我们确定了下降的幅度$\lambda$，则对于每个$\theta$，最佳的下降值为：
> $$ \theta_j :=  \theta_j - \lambda \frac{\delta J(\theta)}{\delta \theta_j} $$


> $$ \frac{\delta J(\theta)} {\delta \theta\_j} =  \frac{1}{m} \sum\_{i=1}^m( h_\theta(x^{(i)}) - y^{(i)}) x_j^{(i)} $$

所以展开就是：

> $$ \theta\_j :=  \theta\_j - \lambda  \frac{1}{m} \sum\_{i=1}^m( h_\theta(x^{(i)}) - y^{(i)}) x_j^{(i)}    $$


就这样，一步一步更新$\theta$，使得$J(\theta)$一步一步变小。
到什么时候停止?

我们比较更新前后的$J(\theta)$, 当更新后的$J(\theta)$比较小时，肯定是可以更新的。
但是当更新后的$J(\theta)$比较大的时候，也不代表不能更新。
我们知道，在我们更新$\theta$的时候，有两个因素，一个是幅度（$\lambda$），一个是方向。
如果更新后的$J(\theta)$比较大，也有可能是更新的幅度比较大，一步迈过了谷底。这时候，我们需要减小$\lambda$来试试。

###NOTICE

1.更新$\theta$的时候是所有$\theta$一起更新的。
虽然对于每个$\theta$的导数值是一个一个计算的，但是更新的时候是最后一起更新。
如果是计算一个导数值更新一个$\theta$，我们会发现，由于我们先更新了$\theta_1$，再计算$\frac{\delta J(\theta)} {\theta_2}$的值就和预期的不符了。

2.对于不同的初始$\theta$，我们最终走到的谷底的位置也可能不同。
也就是说，我们最终计算的最小值可能只是局部最小值，而不是全局最小值。


##1.4学习算法
当我们有了任意一个初始的$\theta$，有了梯度下降算法。
接下来就是用梯度下降算法不断递归更新$\theta$就好了。

