显然，要将 $S$ 集中的单词插入 Trie 树，这样方便查询编号为 $i$ 的单词的出现次数。

定义失配树上每个节点 $u$ 一个域 $cnt(u)$，为目前 $T$ 中包含该节点对应字符串的个数，这样查询 $T$ 中包含 $S$ 中某单词的字符串的个数就相当于在失配树上单点查询。

然后考虑往 $T$ 集加入字符串 $str$ 时对失配树的影响，这相当于在 AC 自动机上匹配 $str$。在普通 AC 自动机的匹配过程中，每到达一个节点就将该节点到根节点的链上每个节点的 $cnt$ 加 $1$ （指失配树上的路径），相当于前缀链的加操作，这样每个节点可能同时在若干点的前缀链上，它的 $cnt$ 会也被加若干次。

但本题稍微有点不一样，在一次匹配当中，每个 $cnt$ 被加的节点只能加一次。于是，先把匹配中所有经过的点放在 $stk$ （栈）里，栈大小 $top$ 就等于字符串 $str$ 的长度，然后以节点对应的 dfn 序为关键字，对 $stk$ 排序并去重（用 sort 和 unique），去重后 $top$ 可能会减小。

然后

$\forall$ $0 \leq i \leq top - 1$，让 $stk_i$ 到 $rt$ 的前缀链上每个节点的 $cnt$ 加 $1$。

$\forall$ $1 \leq i \leq top - 1$，让 $LCA(stk_i,stk_{i-1})$ 到 $rt$ 的前缀链上每个节点的 $cnt$ 减 $1$。

这样就能保证 $stk_i$ 与 $stk_{i-1}$ 的前缀链的公共链上的每个节点的只被加一次。

代码实现上不是直接前缀链修改和单点查询，这样得用树剖，较慢。

更快的是方法是树上差分，将前缀链修改和单点查询变为单点加与子树和查询，然后用 dfn 序和树状数组维护。

此外，因为算法中求 LCA 的部分仅仅只是求 LCA，可以用欧拉序与 st 表实现 $O(N \log N)$ 预处理，$O(1)$ 查询。

总时间复杂度为 $O(N \log N + M \log M)$，$N$ 为 $S$ 集中字符串总长，$M$ 为 $T$ 集中字符串总长。

```cpp
#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <iostream>
using namespace std;

#define re register
#define sf scanf
#define pf printf
#define nl() putchar('\n')
#define ms(x, val) memset(x, val, sizeof(x))
#define _for(i, a, b) for(re int (i) = (a); (i) < (b); ++(i))
#define _rfor(i, a, b) for(re int (i) = (a); (i) <= (b); ++(i))
#define _fev(p, u) for(re int (p) = head[(u)]; (p); (p) = nex[(p)])
#define maxn 2001000
#define lbmaxn 26
#define maxm 2001000
#define maxc 26

template <class T>
void print(string name, T arr[], int n, int flag = 1){
	cout<<name<<":";
	if (flag) _rfor(i, 1, n)cout<<" "<<arr[i];
	else _for(i, 0, n)cout<<" "<<arr[i];
	cout<<endl;
}

char str[maxn];

int 
	//AC 自动机相关 
	ncnt = 1, //Trie 节点数 
	mp[maxn], //S 集中编号为 i 的字符串在 Trie 上对应的节点 
	fail[maxn],
	nxt[maxn][maxc],
	que[maxn],
	
	//失配树图 
	ecnt = 0, 
	head[maxn],  
	to[maxn],
	nex[maxn],
	deep[maxn], 
	
	//dfn 序相关 
	dfn_clock = 0,
	pre[maxn], //dfn 序首 
	pst[maxn], //dfn 序尾 
	ori[maxn], //dfn 序首对应的节点编号 
	
	//树状数组(Fenwick 树) 
	fwk[maxn],
	
	//欧拉序相关 
	euler_clock = 0,
	fir[maxn], //欧拉序首 
	lst[maxn], //欧拉序尾 
	id[2*maxn], //欧拉序首对应的节点 
	
	//st 表相关 
	lb[2*maxn], //以 2 为底的对数 
	st[lbmaxn+2][2*maxn],
	
	//往 T 集中加入字符串时用的 stk 
	stk[maxn];

//在失配树中加边 
inline void add_edge(re int u, re int v){ to[++ecnt] = v, nex[ecnt] = head[u], head[u] = ecnt; }

//在 Trie 中插入 S 集中编号为 i 的字符串 str 
void ins(re char *str, re int i){
	re int u = 1, c;
	while(c = *str++){
		c -= 'a';
		if (!nxt[u][c]) nxt[u][c] = ++ncnt;
		u = nxt[u][c];
	}
	mp[i] = u;
}

//建 fail 
void build(){
	re int fro = 0, bac = 0, u, v;
	fail[1] = 1;
	_for(i, 0, maxc){
		v = nxt[1][i];
		if (v) que[bac++] = v, fail[v] = 1, add_edge(1, v);
		else nxt[1][i] = 1;
	}
	
	while(bac > fro){
		u = que[fro++];
		_for(i, 0, maxc){
			v = nxt[u][i];
			if (v) que[bac++] = v, fail[v] = nxt[fail[u]][i], add_edge(fail[v], v);
			else nxt[u][i] = nxt[fail[u]][i];
		}
	}
}

//求出 dfn 序和欧拉序 
void dfs(int u){
	ori[pre[u] = ++dfn_clock] = u;
	id[fir[u] = ++euler_clock] = u;
	int v;
	_fev(p, u){
		v = to[p];
		deep[v] = deep[u] + 1;
		dfs(v);
		id[++euler_clock] = u;
	}
	pst[u] = dfn_clock;
	lst[u] = euler_clock;
}

//st 表预处理 
void prepare(){
	lb[0] = -1;
	_rfor(i, 1, euler_clock) lb[i] = lb[i>>1]+1;
	_rfor(i, 1, euler_clock) st[0][i] = id[i];
	re int tmp = lb[euler_clock], x, y;
	_rfor(j, 1, tmp){
		_rfor(i, 1, euler_clock + 1 - (1<<j)){
			x = st[j-1][i]; y = st[j-1][i+(1<<(j-1))];
			if (deep[x] <= deep[y]) st[j][i] = x;
			else st[j][i] = y;
		}
	}
	
}

//st 表中查询 
int query(re int l, re int r){
	re int lbl = lb[r-l+1], len = 1<<lbl, x, y;
	x = st[lbl][l]; y = st[lbl][r-len+1];
	if (deep[x] <= deep[y]) return x;
	else return y;
}

int lca(re int x, re int y){
	if (x == y) return x;
	if (fir[x] > fir[y]) x^=y^=x^=y;
	if (lst[x] < fir[y]) return query(lst[x], fir[y]);
	else if (lst[x] > lst[y]) return x;
	else pf("Wrong in lst[x] <= lst[y]\n");
}

//树状数组加与树状数组查询 
void add(re int x, re int val){ for(; x <= dfn_clock; fwk[x] += val, x += (x&(-x))); }
int qry(re int x){ re int val = 0; for(; x; val += fwk[x], x -= (x&(-x))); return val; }

//往 T 集中加入 str
//注意实际上 stk 里装的是节点的 dfn 序首
//ori[dfn 序首] 是对应的节点 
void handle(re char *str){
	re int u = 1, top = 0; stk[top++] = u;
	while(*str) stk[top++] = pre[u = nxt[u][(*str++)-'a']];
	sort(stk, stk+top);
	top = unique(stk, stk+top) - stk;
	re int x, y;
	add(stk[0], 1);
	_for(i, 1, top){
		x = ori[stk[i-1]], y = ori[stk[i]];
		int z = lca(x,y);
		add(pre[y], 1);
		add(pre[lca(x, y)], -1);
	}
}

int main(){
	#ifndef ONLINE_JUDGE
	freopen("in.txt", "r", stdin);
	//freopen("out.txt", "w", stdout);
	#endif
	
	re int n, m, opt, x;
	sf("%d", &n);
	_rfor(i, 1, n){
		sf("%s", str);
		ins(str, i);
	}
	build();
	deep[1] = 0;
	dfs(1);
	prepare();
	
	sf("%d", &m);
	_rfor(i, 1, m){
		sf("%d", &opt);
		if (opt == 1){
			sf("%s", str);
			handle(str);
		}
		else{
			sf("%d", &x);
			pf("%d\n", qry(pst[mp[x]]) - qry(pre[mp[x]]-1));
		}
	}
	
	return 0;
}


```