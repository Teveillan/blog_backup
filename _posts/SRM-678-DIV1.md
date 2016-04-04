title: "SRM 678 DIV1"
date: 2016-04-01 21:53:40
categories: 红名之路
tags: [SRM]
description: 
---

##Easy: A New Hope
题意：假设一个星期有n天，一个人有n件不同的衣服（标号1~n），每件衣服每周穿一件。由此有N！种穿的方式。
但是洗一件衣服需要D天（第x天洗了，x+D天才能穿）。
现已知第一周和最后一周穿衣服的顺序，问中间最少隔了几个星期。

<!--more-->
用数学方法表示，每周穿衣服的顺序可标示为长度为n的排序。
如{1, 2, 3, 4}。假设D＝3，则4号衣服下周最早可以在周三穿，提前了一天(4 - 3 ＝ 1)。

由此便清楚了，本质上就是第一周的排序$P_1$，移动某些数字后，使其状态等于最后一周的状态$P_m$。而每个数字一周最多可以向前移动`n-D`位。

枚举每个数字，求移动周数的最大值。

```c++
class ANewHope
{
        public:
        int count(vector <int> firstWeek, vector <int> lastWeek, int D)
        {
            int n = firstWeek.size();
            D = n - D;
            map<int, int>first_p, last_p;
            REP(i, n) first_p[ firstWeek[i] ] = i, last_p[ lastWeek[i] ] = i;
            int ret = 0;
            FOR(i, 1, n) {
                if(last_p[i] < first_p[i]) 
                    ret = max(ret, (first_p[i] - last_p[i] + D - 1) / D);//上取整
            }
            return ret + 1; //加上第一周
        }
};

```

##Middle: The Empire Strikes Back
题意：有n颗行星，坐标$(x_i, y_i)$。有m发子弹，当子弹威力为w时，对准一颗行星，可以消灭(0, 0)~$(x_i, y_i)$矩形内的行星。
问子弹威力最小应为多少。

这种题上来不用多说先二分枚举子弹威力mid...

行星坐标$(x_i, y_i)$以及矩形右上角的坐标$(x_i+mid, y_i+mid)$分开存。
都对y从大到小进行排序。
贪心。保存此时能毁灭的x的最大值max_x，随着行星坐标$y_i$的减小，求矩形坐标中$y_j+mid > y_i$中最大的$x_j+mid$，并更新为max_x

```c++
const int MOD = 1e9 + 7;
bool solve(vector<pii>&vec, int n, int m, int mid) {
    vector<pii>f(vec);
    for(pii &tmp: f) tmp.first += mid, tmp.second += mid;

    int max_x = -1, id = n - 1;
    int ret = 0;
    DWN(i, n-1, 0) {
        if(max_x < vec[i].second) {
            ++ret;
            while(id >= 0 && f[id].first >= vec[i].first) {
                max_x = max(max_x, f[id].second);
                id --;
            }
        }
    }
    return ret <= m;
}

class TheEmpireStrikesBack
{
        public:
        int find(int AX, int BX, int CX, int AY, int BY, int CY, int n, int m)
        {
            vector<pii>vec;
            LL x = AX, y = AY;
            vec.pb(pii(y, x));
            FOR(i, 1, n-1) {
                x = (x * BX + CX) % MOD;
                y = (y * BY + CY) % MOD;
                vec.pb(pii(y, x));
            }
            sort(vec.begin(), vec.end());
            int l = 0, r = MOD, ans;
            while(l <= r) {
                int mid = (l + r) >> 1;
                if(solve(vec, n, m, mid)) r = mid - 1, ans = mid;
                else l = mid + 1;
            }
            return ans;
        }
};
```

