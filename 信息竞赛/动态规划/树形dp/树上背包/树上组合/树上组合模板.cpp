

int f[maxn][maxw];
int tmp[maxw]; 

int dfs(int u){
	////dp����ĳ�ʼ�������� 
	
	////
	int v,
		szu = dot_weight[u],
		szv; 
	
	_fev(p, u){
		v = to[p];
		szu += dfs(v);
		
		_for(i, beg1, end1){
			
			tmp[i] = func(f[u][i], f[v][0]);
			//��ʼ������һ���ǳ˷�
			//tmp[i] = f[u][i] * f[v][0] 
			_for(j, beg2, end2){
				tmp[i] += func(f[u][i-j], f[v][j]);
				//���ܻ��������ĸ��²��� 
			}
		}
		
		_for(i, beg1, end1){
			f[u][i] = tmp[i];
		} 
	}
	
	return szu;
}

