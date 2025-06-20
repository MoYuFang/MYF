

首先手玩样例，将 $a_i$ 从左到右一个一个地插入，在这个过程中动态维护使得答案最小的序列 $b_i$。

手玩前两个样例后可以发现 $b$ 序列会分成若干个段，每个段里的 $b_i$ 一定是一个以 $k$ 为公差的等差数列，原因可以从下面的例子看出。

比如当 $k=2$ 时，对于下面的 $a$ 序列

```
a: 1 1 4 5 9 10 11 16 17 18 19
```

 我们可以构造出这样的一个 $b$ 序列使得差异值最小

```
b: -1 1 3 5 | 8 10 12 | 15 17 19 21
```

其中 ```|``` 将不同的段分了开来。

现在我们在 $a$ 序列的末尾加入数字 ```20```，$a$ 序列变成

```
a: 1 1 4 5 | 9 10 11 | 16 17 18 19 20
```

因为 $21+k=23>20$，我们让 $b$ 序列的下一个数尽可能的小，也就是 ```23```。

```
b: -1 1 3 5 | 8 10 12 | 15 17 19 21 23
```

但是差异值不再是最小的了，我们需要对 $b$ 序列作一些改动使得差异值减小。

对比一下 $a$ 序列的最后一段和 $b$ 序列的最后一段。

```
a: 16 17 18 19 20
b: 15 17 19 21 23
```

因为 $b$ 序列中的每一段一定是一个以 $k$ 为公差的等差数列，若想修改一段中的数字必然使这一段整体加或减一个相同的数值，对段的修改必然是整体加减。

考虑到最后一段中，若整体减的话会有 $2$ 个数字 ```15 17``` 会使得差异值增大，会有 $3$ 个数字 ```19 21 23``` 会使差异值减小。

我们称使差异值增大的数为**升数**，使差异值减小的数为**降数**，显然升数和降数的个数决定了整体减是否会使差异值减小。

最后一段中，升数的个数小于降数的个数，所以我们应该使最后一段整体减以使差异值减小，那么要减多少合适呢？

显然减到降数个数小于等于升数个数时最为合适，在这里我们需要整体减 $1$，使得 $b$ 序列的最后一段变成

```
b: 14 16 18 20 22
```

于是新的 $a$ 序列和 $b$ 序列就是

```
a: 1 1 4 5 | 9 10 11 | 16 17 18 19 20
b: -1 1 3 5 | 8 10 12 | 14 16 18 20 22
```

然后我们再在 $a$ 中加入一个数字 ```21```，注意到 ```b``` 中的升数个数大于等于降数个数，不需要整体减，所以两个序列变为

```
a: 1 1 4 5 | 9 10 11 | 16 17 18 19 20 21
b: -1 1 3 5 | 8 10 12 | 14 16 18 20 22 24
```

再在 $a$ 中加入一个数字 ```22```，这时候 ```b``` 中的降数个数大于升数个数，所以需要对 $b$ 序列中的最后一段进行整体减。

```
a: 1 1 4 5 | 9 10 11 | 16 17 18 19 20 21 22
b: -1 1 3 5 | 8 10 12 | 13 15 17 19 21 23 25
```

但是这样有问题，注意到 $b$ 序列中第 $2$ 段末尾的数字 ```12``` 和第 $3$ 段开头的数字 ```13``` 差距小于 $k$，这样的 $b$ 序列是非法的。

于是我们将 $b$ 序列倒退回整体减之前的状态，也就是

```
b: -1 1 3 5 | 8 10 12 | 14 16 18 20 22 24 26
```

发现第 $2$ 段和 $3$ 段之间已经是一个等差数列了，应该将两段合并。

```
b: -1 1 3 5 | 8 10 12 14 16 18 20 22 24 26
```

再与 $a$ 序列对比一下。

```
a: 1 1 4 5 | 9 10 11 16 17 18 19 20 21 22
```

升数有 ```8 10 14 16 18```，降数有 ```12 20 22 24 26```，降数与升数数量一致，不需要再整体减了。

再在 $a$ 中加入数字 ```40```，因为 $b$ 序列中最后一个数字加 $k$ 后的值 $26+2=28<40$，所以可以让 $b$ 的下一个数字为 $40$，这样就需要新开一个段。

```
b: -1 1 3 5 | 8 10 12 14 16 18 20 22 24 26 | 40
```

通过上面的例子可以发现我们需要为 $b$ 序列的每个段维护降数和升数的个数，以及最快能减少成升数的降数是哪一个，同时每个段是一个公差为 $k$ 的等差数列，故需要为每个段维护等差数列的初值。

于是我们设计如下的结构体来维护 $b$ 序列中不同的段。

```cpp
#define ll long long
struct Section{
    int l, r; //这个段对应的区间为[l,r]
    ll 	base, //等差数列的初值
    	max_down; //这个段再整体减少多少就需要与上一个段合并(第一个段认为是正无穷)
    ll get_lst(){ return base + (ll)(r-l)*k; } //返回等差数列的末值
};
```

用线段树来维护每个区间内的降数个数、每个降数再减少多少就会变成升数、最快减少成升数的降数是哪一个。

```cpp
struct Seg{
    int cnt, //降数个数
    	pos; //最快减少成升数的降数在 b 序列中的位置
    ll	tmi, //pos 上的数再减少多少就会变成升数
    	tag; //区间加标记，用来完成整体减的操作
} seg[maxn<<2]
```

然后我们在给 $a$ 序列不断添加数字的过程中动态维护 $b$ 序列以及答案即可。



完整代码加注释。

```cpp
#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <assert.h>
using namespace std;

#define re register
#define sf scanf
#define pf printf
#define nl() putchar('\n')
#define ms(x, val) memset(x, val, sizeof(x))
#define ll long long
#define db double
#define ull unsigned long long
#define _for(i, a, b) for(re int i = (a); i < (b); ++i)
#define _rfor(i, a, b) for(re int i = (a); i <= (b); ++i)
#define _fev(p, u) for(re int p = head[u]; p; p = nex[p])
#define inf 0x7fffffffffffffffll
#define maxn 1000005
#define maxnn (10*maxn)

int rdnt(){
	re int x = 0, sign = 1;
	re char c = getchar();
	while (c < '0' || c > '9') { if (c == '-') sign = -1; c = getchar(); }
	while (c >= '0' && c <= '9') x = (x<<3) + (x<<1) + (c ^ 48), c = getchar();
	return x * sign;
}

template<class T> 
inline void umx(T &x, const T &y){ if (y > x) x = y; }
template<class T>
inline void umi(T &x, const T &y){ if (y < x) x = y; }

int rg; //线段树的值域
struct Seg{ int pos, cnt; ll tag, tmi; } seg[maxnn];
#define tmi(x) seg[x].tmi
#define pos(x) seg[x].pos
#define cnt(x) seg[x].cnt
#define tag(x) seg[x].tag
#define lx (x<<1)
#define rx (x<<1|1) 

//push up 操作
void ud(re int x){
	tmi(x) = min(tmi(lx), tmi(rx));
	pos(x) = tmi(lx) == tmi(x) ? pos(lx) : pos(rx);
	cnt(x) = cnt(lx) + cnt(rx);
}

void build(int x, int tl, int tr){
	tag(x) = 0;
	if (tl == tr){ cnt(x) = tag(x) = pos(x) = 0; tmi(x) = inf; return; }
	int mi = (tl+tr)>>1;
	build(lx, tl, mi);
	build(rx, mi+1, tr);
	ud(x);
}

//对线段树上的一个节点进行区间减操作
void spread(re int x, re ll vl){
	if (tmi(x) < inf){
		tmi(x) -= vl;
		tag(x) += vl;
		assert(tmi(x) >= 0);
	}
}

//push down 操作
void pd(re int x){
	spread(lx, tag(x));
	spread(rx, tag(x));
	tag(x) = 0;
}

//在位置 p 上插入一个降数，这个降数再减少 vl 就会变成升数
void addn(int x, ll vl, int p, int tl, int tr){
	if (tl == tr){ tmi(x) = vl; pos(x) = p; cnt(x) = 1; tag(x) = 0; return; }
	if (tag(x)) pd(x);
	int mi = (tl+tr)>>1;
	if (p <= mi) addn(lx, vl, p, tl, mi);
	else addn(rx, vl, p, mi+1, tr);
	ud(x);
}

//将位置 p 上的降数修改为升数
void deln(int x, int p, int tl, int tr){
	assert(x);
	if (tl == tr){
		assert(tmi(x)==0);
		tmi(x) = inf; pos(x) = cnt(x) = tag(x) = 0;
		return;
	}
	if (tag(x)) pd(x);
	int mi = (tl+tr)>>1;
	if (p <= mi) deln(lx, p, tl, mi);
	else deln(rx, p, mi+1, tr);
	ud(x); 
}

//区间减操作
void mdf(int x, ll vl, int l, int r, int tl, int tr){
	if (l == tl && r == tr){ spread(x, vl); return; }
	if (tag(x)) pd(x);
	int mi = (tl+tr)>>1;
	if (r <= mi) mdf(lx, vl, l, r, tl, mi);
	else if (l > mi) mdf(rx, vl, l, r, mi+1, tr);
	else mdf(lx, vl, l, mi, tl, mi), mdf(rx, vl, mi+1, r, mi+1, tr);
	ud(x);
}

//查询一个区间内降数的个数 c, 最快减少成升数的降数的位置 p, p 位置上的降数减少多少后会变成升数 vl
struct Par{ int p, c; ll vl; };
Par qry(int x, int l, int r, int tl, int tr){
	if (tl == l && tr == r){ return {pos(x), cnt(x), tmi(x)}; }
	if (tag(x)) pd(x);
	int mi = (tl+tr)>>1;
	if (r <= mi) return qry(lx, l, r, tl, mi);
	else if (l > mi) return qry(rx, l, r, mi+1, tr);
	else{
		Par p1 = qry(lx, l, mi, tl, mi), p2 = qry(rx, mi+1, r, mi+1, tr);
		return p1.vl <= p2.vl ? Par({p1.p, p1.c+p2.c, p1.vl}) : Par({p2.p, p1.c+p2.c, p2.vl});
	}
}

int k,
	top = 0, //段的数目, 也就是栈顶
	a[maxn]; //a 序列
ll ans = 0; //答案
struct Section{
	int l, r;
	ll updown, base;
	ll get_lst(){ return base+(ll)(r-l)*k; }
} stk[maxn]; //栈

//对一个段进行整体减 vl
//同时 p 代表那个最新变为升数的降数的位置
//c 是这一段中的降数个数减去升数个数
void decrease(Section &sec, re int c, re int p, re ll vl){
	ans -= (ll)c*vl;
	mdf(1, vl, sec.l, sec.r, 1, rg);
	if (vl == 0) assert(p != -1);
	if (p != -1) deln(1, p, 1, rg);
	sec.base -= vl;
	if (sec.updown < inf){
		assert(sec.updown >= vl);
		sec.updown -= vl;
	}
}

//尝试将最后一段整体减少
//这样一定至少使一个降数变为一个升数
//若成功使得至少一个降数变为升数则返回 true
//否则返回 false
bool maintain(){
	Section &sec = stk[top];
	
	Par par = qry(1, sec.l, sec.r, 1, rg);
	re int c = par.c*2-(sec.r-sec.l+1); //降数个数减去升数个数 
	if (c < 0) return false;
	
	re int p = par.p; re ll vl = par.vl;
	assert(vl < inf && p > 0);
	
	if (sec.updown == inf || vl < sec.updown){//还没减少完
		decrease(sec, c, p, vl);
	} 
	else{ //减少完了，要与上一段合并了
		decrease(sec, c, vl==sec.updown?p:-1, sec.updown);
		assert(sec.updown == 0);
		assert(top > 1);
		Section &pre_sec = stk[top-1];
		assert(pre_sec.get_lst()+k == sec.base);
		pre_sec.r = sec.r;
		--top;
	}
	return true;
}

signed main(){
	#ifndef ONLINE_JUDGE
	freopen("sample.in", "r", stdin);
	//freopen("sample.out", "w", stdout);
	#endif
	
	seg[0] = {0, 0, 0, inf};
	
	re int n = rg = rdnt(), T; k = rdnt(); ans = top = 0;
	_rfor(i, 1, n) a[i] = rdnt(); T = rdnt();
	build(1, 1, n);
	
	_rfor(i, 1, n){
		if (i == 1){
			Section &sec = stk[++top];
			sec.l = sec.r = 1; sec.updown = inf; sec.base = a[i];
		}
		else{
			re ll curb = stk[top].get_lst()+k;
			if (curb >= a[i]){
				ans += curb-a[i];
				++stk[top].r;
				addn(1, curb-a[i], i, 1, rg);
				while(maintain());
			}
			else{
				Section &sec = stk[++top];
				sec.l = sec.r = i; sec.updown = a[i]-curb; sec.base = a[i];
			}
		}
		if (T == 0 || i == n) pf("%lld\n", ans);
	}

	return 0;
}
```













