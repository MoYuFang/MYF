

int tmp[maxw];

int dfs(int u){
	////dp����ĳ�ʼ�������� 
	 
	////
	int v
		szu = 1, //u�����Ĵ�С
		//szu = dot_weight[u] //������С���Բ��������ڵ�����Ҳ�����������ڵ�Ȩֵ�� 
		szv, //v�����Ĵ�С
		;
	_fev(p, u){
		v = to[p];
		szu += dfs(v);
		//���ӽڵ� v ���� f[u][i] 
		_for(i, beg1, end1){//��������ķ�Χ 
			//tmp[i] ��������������¹���� f[u][i] 
			tmp[i] = init; //��ʼֵ
			//���ܻ��������ĳ�ʼ����
			
			_for(j, beg1, end2){
				tmp[i] = better(tmp[i], func(f[u][i-j], f[v][j])) //���� 
				//���ܻ��������ĸ��²���				
			}
		}

		_for(i, beg1, end1){
			f[u][i] = tmp[i]; //���ĸ��� 
		} 
	}
	
	return szu; //����������С 
}

//ע�⣺�е���Ŀ���Խ� tmp ����ʡ�ԣ�ֱ���� f[u] �����������²���
//�� tmp ��������� 
 
 
