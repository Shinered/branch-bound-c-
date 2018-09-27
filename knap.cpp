#include <iostream>
#include <algorithm>
#include <stack>
#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;
//分支定界算法,使用深度优先的栈实现

//节点类实现
class Node
{	
public:
	int curvalue; //当前节点的价值
	int level = -1;
	int room;
	double bestvalue; //bound值
	bool flag = 1;   
	vector<int> bit; //最优解集
};
//物体属性
class Item
{
public:
	int index;
	int value;
	int weight;
};

//自定义比较函数,对items进行排序
bool mycompare(const Item a, const Item b)
{
	double a_den = a.value * 1.0 / a.weight;
	double b_den = b.value * 1.0 / b.weight;
	return a_den > b_den;
}

//branch & bound ,dfs implementation
class Bound
{
public:
/***************初始化Item数据**************/
	Bound()
	{
		ifstream fin("./data/ks_10000_0");						
		string line;
		stringstream stream;
		if(getline(fin, line))
		{	
			stream.clear();
			stream << line;
			stream >> item_count;
			stream >> capacity;	
		}
//		maxprofit = 0;
		int cnt = 0;
		while(getline(fin, line))
		{
			int value;
			int weight;
			Item item;
			stream.clear();
			stream << line;
			stream >> value;
			stream >> weight;
			//maxprofit += value;
			item.index = cnt;
			item.value = value;
			item.weight = weight;
			items.push_back(item);
			cnt++;
		}
		//排序
		sort(items.begin(), items.end(), mycompare);

	}
	//计算bound,好的bound对有效地剪枝(prune)很重要
	double bound( Node a )
	{
		double sum = 0;
		double weight = 0;
		if(a.level == -1){
			for(int i = 0; i < item_count; i++)
			{
				if(weight + items[i].weight <= capacity)
				{
					sum += items[i].value;
					//cout << "sum :" << sum << endl;
					weight += items[i].weight;
				} else {
					sum += (capacity-weight)*1.0 / items[i].weight  * items[i].value;
					//cout << "sum:" << sum << endl;
					break;
				}	
			}
		} else {
			sum += a.curvalue;
			weight += capacity - a.room;
			for(int i = a.level+1; i < item_count; i++)
			{
				if(weight + items[i].weight <= capacity)
				{
					sum += items[i].value;
				//	cout << "sum :" << sum << endl;
					weight += items[i].weight;
				} else {
					sum += (capacity-weight)*1.0 / items[i].weight  * items[i].value;
				//	cout << "sum:" << sum << endl;
					break;
				}	
			}		
		}
		return sum;
	}


	void print(Node a)
	{
		cout   <<  "level:" << a.level <<  "curvalue:" << a.curvalue << ","  << "room:" << a.room << "," <<  "bestvalue:" << a.bestvalue << endl;
	
	}
	//深度优先的branch & bound
	int dfs()
	{
		Node u, v;
		u.curvalue = 0;
		u.room = capacity;
		u.bit.resize(item_count);	
		u.bestvalue = bound(u);
	//	print(u);
		myQ.push(u);
		int max = 0;
		int cnt = 0;
		while(!myQ.empty())
		{
			cout << "cnt:" << cnt ++ << endl;
			u = myQ.top();
			myQ.pop();
			if(u.level == -1)
			{
				v.level = 0;
			}

			if(u.level == item_count-1)
			{
				continue;
			}
			//not choose this item;
			v.level = u.level + 1;
			v.room = u.room;
			v.curvalue = u.curvalue;
			//cout << "curvalue:"	 << v.curvalue << endl;
			v.bestvalue = bound(v);
			v.bit = u.bit;
		
			if(v.bestvalue > max)
			{
				myQ.push(v);
				if(v.level == item_count - 1 )
				{
					max = v.curvalue;
					bit = v.bit;
				//	cout << "max value: " << max << endl;
				}
			}
			
			//choose this item;
			v.level = u.level + 1;
			v.room = u.room - items[v.level].weight;
			v.curvalue = u.curvalue +  items[v.level].value;
			v.bit = u.bit;
			v.bit[items[v.level].index] = 1;
			v.bestvalue = u.bestvalue;
			if((v.room >= 0) && (v.bestvalue > max))
			{
				myQ.push(v);
				if(v.level == item_count - 1 )
				{
					max = v.curvalue;
					bit = v.bit;
				}
			}
		}

		return max;
	}
	
	void printbit()
	{
		for(int i = 0; i < bit.size(); i++)
		{
			cout << bit[i] << " ";
		}
	}

public:
//	int maxprofit; 
	vector<int> bit; //保留最优解集
	vector<Item> items;  //物体数组
	int item_count; //物体数目
	int capacity;   //背包容量
	stack<Node> myQ; //stack 栈，后进先出
};

int main()
{	
	clock_t start, end;
	start = clock();
	Bound bound;
	cout << "maxprofit:" << bound.dfs() << endl;
	bound.printbit();
	end = clock();
	cout << "running time:" << (double)(end-start)/CLOCKS_PER_SEC << "s" << endl;
	return 0;
}










