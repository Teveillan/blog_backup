title: "[转]Manacher算法-回文串处理"
categories: Algorithm
date: 2015-01-09 11:45:39
tags:
description:
---
**Manacher**算法是查找一个字符串的最长回文子串的线性算法。
在介绍算法之前，首先介绍一下什么是回文串，所谓回文串，简单来说就是正着读和反着读都是一样的字符串，比如abba，noon等等，一个字符串的最长回文子串即为这个字符串的子串中，是回文串的最长的那个。
计算字符串的最长回文字串最简单的算法就是枚举该字符串的每一个子串，并且判断这个子串是否为回文串，这个算法的时间复杂度为O(n^3)的，显然无法令人满意，稍微优化的一个算法是枚举回文串的中点，这里要分为两种情况，一种是回文串长度是奇数的情况，另一种是回文串长度是偶数的情况，枚举中点再判断是否是回文串，这样能把算法的时间复杂度降为O(n^2)，但是当n比较大的时候仍然无法令人满意，Manacher算法可以在线性时间复杂度内求出一个字符串的最长回文字串，达到了理论上的下界。

<!-- more -->

#Manacher算法原理与实现

下面介绍Manacher算法的原理与步骤。
首先，Manacher算法提供了一种巧妙地办法，将长度为奇数的回文串和长度为偶数的回文串一起考虑，具体做法是，在原字符串的每个相邻两个字符中间插入一个分隔符，同时在首尾也要添加一个分隔符，分隔符的要求是不在原串中出现，一般情况下可以用#号。下面举一个例子：
![](http://7tszmu.com1.z0.glb.clouddn.com/Manacher-1.png)

## Len数组简介与性质
Manacher算法用一个辅助数组Len[i]表示以字符T[i]为中心的最长回文字串的最右字符到T[i]的长度，比如以T[i]为中心的最长回文字串是T[l,r],那么Len[i]=r-i+1。
对于上面的例子，可以得出Len[i]数组为:

![](http://7tszmu.com1.z0.glb.clouddn.com/Manacher-2.png)

Len数组有一个性质，那就是Len[i]-1就是该回文子串在原字符串S中的长度，至于证明，首先在转换得到的字符串T中，所有的回文字串的长度都为奇数，那么对于以T[i]为中心的最长回文字串，其长度就为2*Len[i]-1,经过观察可知，T中所有的回文子串，其中分隔符的数量一定比其他字符的数量多1，也就是有Len[i]个分隔符，剩下Len[i]-1个字符来自原字符串，所以该回文串在原字符串中的长度就为Len[i]-1。
有了这个性质，那么原问题就转化为求所有的Len[i]。下面介绍如何在线性时间复杂度内求出所有的Len。

## Len数组的计算

首先从左往右依次计算Len[i]，当计算Len[i]时，Len[j](0<=j<i)已经计算完毕。设P为之前计算中最长回文子串的右端点的最大值，并且设取得这个最大值的位置为po，分两种情况：
第一种情况：i<=P
那么找到i相对于po的对称位置，设为j，那么如果Len[j]<P-i，如下图：

![](http://7tszmu.com1.z0.glb.clouddn.com/Manacher-3.png)

那么说明以j为中心的回文串一定在以po为中心的回文串的内部，且j和i关于位置po对称，由回文串的定义可知，一个回文串反过来还是一个回文串，所以以i为中心的回文串的长度至少和以j为中心的回文串一样，即Len[i]>=Len[j]。因为Len[j]<P-i,所以说i+Len[j]<P。由对称性可知Len[i]=Len[j]。
如果Len[j]>=P-i,由对称性，说明以i为中心的回文串可能会延伸到P之外，而大于P的部分我们还没有进行匹配，所以要从P+1位置开始一个一个进行匹配，直到发生失配，从而更新P和对应的po以及Len[i]。

![](http://7tszmu.com1.z0.glb.clouddn.com/Manacher-4.png)

第二种情况: i>P
如果i比P还要大，说明对于中点为i的回文串还一点都没有匹配，这个时候，就只能老老实实地一个一个匹配了，匹配完成后要更新P的位置和对应的po以及Len[i]。

![](http://7tszmu.com1.z0.glb.clouddn.com/Manacher-5.png)

# 时间复杂度分析

Manacher算法的时间复杂度分析和Z算法类似，因为算法只有遇到还没有匹配的位置时才进行匹配，已经匹配过的位置不再进行匹配，所以对于T字符串中的每一个位置，只进行一次匹配，所以Manacher算法的总体时间复杂度为O(n)，其中n为T字符串的长度，由于T的长度事实上是S的两倍，所以时间复杂度依然是线性的。

```c++
const int MAX_LEN = 1e5 + 10;

int Len[MAX_LEN<<1]; //记录算法完成后以i点为中心最长回文串向左向右延伸的半径
struct Manacher {
    char tmp_str[MAX_LEN<<1]; //记录预处理过后的字符串
    int init(char *s) { //在solve中就直接调用了。对原串进行预处理。
        int len = strlen(s);
        int id = 0;
        tmp_str[id++] = '#';
        for(int i=0; i<len; ++i) {
            tmp_str[id++] = s[i];
            tmp_str[id++] = '#';
        }
        tmp_str[id] = '\0';
        return id; //此时的id即为处理过后的字符串长度
    }


    void solve(char *s) { //从这开始处理
        int len = init(s);
        int max_r = -1, max_id = -1; // 最右能到得地方，以及最右时的中心点id
        for(int i=0; i<len; ++i) {
            if(max_r > i)   Len[i] = min(max_r - i, Len[2 * max_id - i]);
            else Len[i] = 1;

            while(i - Len[i] >= 0 && i + Len[i] < len && tmp_str[i - Len[i]] == tmp_str[i + Len[i]])
                ++Len[i];

            if(i + Len[i] > max_r) {
                max_r = i + Len[i];
                max_id = i;
            }
        }
    }
};
```

转自：[http://blog.csdn.net/dyx404514/article/details/42061017](http://blog.csdn.net/dyx404514/article/details/42061017)
作者：dyx心心

