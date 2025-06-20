模运算下的数值难以比较大小，且这道题涉及加法，不是乘法，不能使用对数放缩，只能用高精度，但直接使用则时空复杂度爆炸。

#### 考虑优化高精度

注意到边权全为 $2$ 的幂次，这提示我们用二进制表示数值。

这里设数值 $val$ 二进制表示下第 $i$ 位为 $val(i)$，每次给数值 $val$ 加 $2^x$ 相当于找最小的 $y$ 使得 $y>=x$ 且 $val(y)=0$，然后让 $x$ 到 $y$ 之间的 $1$ 归零，$val(y)$ 置为 $1$。

归零操作相当于区间覆盖，置 $1$ 的操作相当于单点修改，于是考虑每个数值均用一颗权值线段树维护，寻找 $y$ 则可以通过线段树二分完成，三种操作都可以单次 $O(\log x)$ 完成。

比较的话就相当于字典序比较，先找到两个数值最高的不相同的位，然后比较那一位的大小即可，这一步就是个经典的字符串算法，用 二分+ 进制 $\text{hash}$ 就可以以单次 $O(\log x)$ 完成。

进制 $hash$ 满足结合律且可以 $O(1)$ 快速合并两个子区间的进制 ，于$\text{hash}$是可以在权值线段树上多加一个变量维护每个节点对应区间的 $\text{hash}$ 值。 

进一步的，因为在 $\text{dijkstra}$ 中涉及到 $O(n+m)$ 个数值，如果每个数值都用一颗权值线段树则炸时空，而单次给数值加上 $2$ 的幂次只会影响权值线段树上 $O(\log x)$ 个节点，显然可以用可持久化优化时空，总时间复杂度为 $O(m\log m \log x)$ 。

思路还是很好想的，是道假黑题。

#### 具体实现

直接用 $2$ 作为进制 $\text{hash}$ 的进制，$1000000007$ 作为模数，这样在输出答案时直接输出 $\text{hash}$ 值就行了。

初步判读线段树每个节点需要保存左儿子指针，右儿子指针，$\text{hash}$ 值，区间覆盖懒标记，区间内是否全为 $1$ 标记。

一般来说支持区间覆盖操作需要带上懒标记，但这题是区间归零操作，且用的是动态开点线段树，完全没必要带这一懒标记，因为区间归零操作就相当于让节点变成 $none$，所以可以省去区间覆盖的懒标记。

单点修改也很套路。

寻找 $y$ 这一步用线树二分时需要快速判断某一节点所对应的区间是否全为 $1$，考虑到我们都已经维护了区间的  值$\text{hash}$了，何不好好利用它，没必要再多加一个判断区间内是否为全 $1$ 的标记了，预处理出每个长度的全 $1$ 的区间对应的 $\text{hash}$ 值，每次判断某个区间是否全为 $1$ 直接比较 $\text{hash}$ 值就行了。

比较操作直接在线段树上二分就行了。

总结一下，线段树的每个节点只需保存左儿子指针，右儿子指针和 $\text{hash}$ 值。

在 $\text{dijkstra}$ 时，用 $dis(u)$ 来代表节点 $u$ 到源点的最短路长度数值（实际上存的是权值线段树的根节点），枚举边时也会生成一个临时数值 $tmp = dis(u)+2^{x_{(u,v)}}$ （也是一颗权值线段树）。

当 $tmp$ 与 $dis(v)$ 比较完后，其中一个数值就废弃了，因为目前肯定没有任何数值是通过 $tmp$ 或 $dis(v)$生成的，生成那个废弃数值所新用的节点都回收后不会影响到其它数值，可以回收再利用，考虑如何回收这些节点。

一开始我考虑的是为 $tmp$ 和每个 $dis(*)$ 各开一个 ```vector``` 保存生成它所新用的节点，然后在它废弃时再将对应 ```vector``` 中所有节点回收。

然后 $\text{RE}$ 了，```debug``` 后发现原因在于若废弃的数值是 $dis(v)$，尽管我回收了 $dis(v)$，但优先队列中仍存在 $(v,dis(v))$ 二元组，但关于那个被废弃的数值的所有比较已经失效，于是这基于比较实现的 ```STL``` 就导致 $\text{RE}$ 了。

解决办法有两个，一是放弃回收节点的想法，二是用 ```set``` 代替优先队列，然后每次废弃 $dis(v)$ 前先从 ```set``` 里删掉对应的二元组 $(v,dis(v))$。

既然都打算回收节点了，那就选方法二吧，反正复杂度仍不变。

经过一番仔细分析后，发现自己脑残了，即便回收了节点空间复杂度也没怎么降，只不过是从 $O(m\log x)$ 变为了 $O(n \log x)$，但 $m$ 与 $n$ 同阶，相当于白忙活了，只收获了一个 $\text{RE}$ 技巧。

```cpp
#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <assert.h>
#include <vector>
#include <set>
using namespace std;

#define re register
#define sf scanf
#define pf printf
#define nl() putchar('\n')
#define ms(x, val) memset(x, val, sizeof(x))
#define ll long long
#define _for(i, a, b) for(re int (i) = (a); (i) < (b); ++(i))
#define _rfor(i, a, b) for(re int (i) = (a); (i) <= (b); ++(i))
#define _fev(p, u) for(re int (p) = head[(u)]; (p); (p) = nex[(p)])
#define inf 0x7fffffff
#define maxn 101005
#define maxnn 3100005
#define mod 1000000007ll

int rdnt(){
	re int x = 0, sign = 1;
	re char c = getchar();
	while (c < '0' || c > '9') { if (c == '-') sign = -1; c = getchar(); }
	while (c >= '0' && c <= '9') x = (x<<3) + (x<<1) + (c ^ 48), c = getchar();
	return x * sign;
}

bool vis[maxn]; //dijkstra 用的 
int	
	//模 1000000007 意义下 2 的幂次 
	epw[maxn],
	
	//模 1000000007 意义下全 1 区间的 hash 值 
	hsh_one[maxn], 

	//权值线段值域 
	rg = maxn-5, 

	//内存池，用于回收节点 
	pcnt = 0, 
	pool[maxnn], 
	
	//邻接表 
	ecnt = 1, 
	head[maxn],
	to[maxn*2], 
	nex[maxn*2],
	we[maxn*2],
	
	//单元最短路数组（实际上存的是权值线段树的根节点编号） 
	dis[maxn],
	
	//输出路径方案用的 
	top = 0,
	path[maxn], //path(v) 代表 v 是从哪转移过来的 
	stk[maxn];
vector<int>
	tmp_set, //用来装生成临时数值所新用的线段树节点 
	seg_set[maxn]; //用来装生成 dis(v) 所新用的线段树节点 
struct Seg{ int lc, rc, hsh; } seg[maxnn]; //线段树节点，分别代表左、右儿子指针和 hash 值 
#define lc(x) seg[x].lc
#define rc(x) seg[x].rc
#define hsh(x) seg[x].hsh
struct Par{ int u, d; };
bool cmp(int x, int y, int tl, int tr);
inline bool operator<(const Par &p1, const Par &p2){
	//因为删除二元组时要精确查找二元组，所以无向图节点的编号也要纳入比较范围 
	return (hsh(p1.d) != hsh(p2.d)) ? cmp(p1.d, p2.d, 1, rg) : p1.u < p2.u; 
}
set<Par> pq; //set 版优先队列 

void add_edge(re int u, re int v, re int w){
	to[++ecnt] = v; nex[ecnt] = head[u]; head[u] = ecnt; we[ecnt] = w;
	to[++ecnt] = u; nex[ecnt] = head[v]; head[v] = ecnt; we[ecnt] = w;
}

//申请一个新的线段树节点并从 nn 拷贝 
int new_node(re int nn){
	re int x = pool[pcnt--];
	seg[x] = seg[nn];
	return x;
}

//回收单个线段树节点 
void del(re int x){
	pool[++pcnt] = x;
}

//回收 vector 里的线段树节点 
void clr(vector<int> &vec){
	_for(i, 0, vec.size()) del(vec[i]);
	vec.clear();
}

//线段树 push up 
void ud(re int x, re int len){
	hsh(x) = (hsh(lc(x)) + (ll)hsh(rc(x))*epw[len]%mod)%mod;
}

//比较两个数值，即判断 x < y 
bool cmp(int x, int y, int tl, int tr){
	if (tl == tr) return hsh(x) < hsh(y);
	int mi = (tl+tr)>>1;
	return (hsh(rc(x)) == hsh(rc(y))) ? cmp(lc(x), lc(y), tl, mi) : cmp(rc(x), rc(y), mi+1, tr);
}

//线段树二分找到区间 [l,r] 内第一个不为 0 的位置 
int qry(int x, int l, int r, int tl, int tr){
	if (tl > r || tr < l || hsh(x) == hsh_one[tr-tl+1]) return -1;
	if (hsh(x) == 0) return max(l, tl);
	int mi = (tl+tr)>>1, tmp = qry(lc(x), l, r, tl, mi);
	return (tmp > 0) ? tmp : qry(rc(x), l, r, mi+1, tr);
}

//区间归零操作，并将新用的节点加入到 vec 里
void cover(int x, int &y, int l, int r, int tl, int tr, vector<int> &vec){
	if (l == tl && r == tr){ y = 0; return; }
	vec.push_back(y = new_node(x));
	int mi = (tl+tr)>>1;
	if (r <= mi) cover(lc(x), lc(y), l, r, tl, mi, vec);
	else if (l > mi) cover(rc(x), rc(y), l, r, mi+1, tr, vec);
	else cover(lc(x), lc(y), l, mi, tl, mi, vec), cover(rc(x), rc(y), mi+1, r, mi+1, tr, vec);
	ud(y, mi-tl+1);
}

//单点修改，并将新用的节点加入到 vec 里
void addn(int x, int &y, int p, int tl, int tr, vector<int> &vec){
	if (tl == tr){
		vec.push_back(y = new_node(0));
		hsh(y) = 1;
		return;
	}
	vec.push_back(y = new_node(x));
	int mi = (tl+tr)>>1;
	if (p <= mi) addn(lc(x), lc(y), p, tl, mi, vec);
	else addn(rc(x), rc(y), p, mi+1, tr, vec);
	ud(y, mi-tl+1);
}

//数值 x 上加入 2^(w-1)，只所以 w-1 是为了让权值线段树的值域大于等于 1 
//返回生成的数值对应的权值线段树的根节点 
int add(re int x, re int w, vector<int> &vec){
	vec.clear();
	re int p = qry(x, w, rg, 1, rg), tmp = 0;
	addn(x, tmp, p, 1, rg, vec);
	if (p > w) cover(tmp, tmp, w, p-1, 1, rg, vec);
	return tmp;
}

void dijkstra(re int n, re int s){
	re int u, v, d, tmp;
	_rfor(i, 1, n) dis[i] = -1;
	dis[s] = 0;
	pq.insert((Par){s, dis[s]});
	while(!pq.empty()){
		auto it = pq.begin();
		u = it->u; d = it->d; pq.erase(it);
		if (vis[u]) continue;
		vis[u] = true;
		_fev(p, u){
			if (vis[v = to[p]]) continue;
			tmp = add(d, we[p], tmp_set);
			if (dis[v] == -1 || cmp(tmp, dis[v], 1, rg)){
				if (dis[v] != -1){
					//从 set 里精确删除 
					it = pq.find((Par){v, dis[v]});
					pq.erase(it);
				}
				path[v] = u;
				dis[v] = tmp;
				//交换 vector 
				swap(tmp_set, seg_set[v]);
				pq.insert((Par){v, dis[v]});
			}
			//回收节点 
			clr(tmp_set);
		}
	}
}

//求出路径方案 
void dfs(int s, int u){
	stk[top++] = u;
	if (u != s) dfs(s, path[u]);
}

int main(){
	#ifndef ONLINE_JUDGE
	freopen("sample.in", "r", stdin);
	freopen("sample.out", "w", stdout);
	#endif
	
	//预处理 
	hsh_one[0] = lc(0) = rc(0) = hsh(0) = 0; epw[0] = 1;
	_for(i, 1, maxnn) pool[++pcnt] = maxnn-i;
	_for(i, 1, maxn) epw[i] = (ll)epw[i-1]*2%mod, hsh_one[i] = (epw[i]+mod-1)%mod;
	
	re int n = rdnt(), m = rdnt(), s, t, u, v, w;
	_rfor(i, 1, m){
		u = rdnt(), v = rdnt(), w = rdnt();
		add_edge(u, v, w+1);
	}
	s = rdnt(); t = rdnt();
	dijkstra(n, s);

	if (dis[t] == -1){ pf("-1\n"); return 0; }
	re int ans = hsh(dis[t]); top = 0;
	pf("%d\n", ans);
	dfs(s, t);
	pf("%d\n", top);
	while(top){
		pf("%d", stk[--top]);
		if (top)putchar(' ');
	}
	nl();
	
	return 0;
}
```

