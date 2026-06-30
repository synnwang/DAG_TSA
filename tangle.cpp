#include <iostream>
#include <cstdlib>
#include "site.h"
#include <list>
#include <math.h>
#include <vector>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
using namespace std;

list<site> event_list;
list<int> tip_set;

const int numArrivals=1000000;
double timeStamp[numArrivals+1][2]={0};
double T0[numArrivals+1]={0};   // 新增：紀錄每筆交易的 t0（發送時間）

double lambda=5;
int current_id=1;
double current_time=0;
int k=2;
double confirmTime=1; // PoW time
double tsa_time=0; 
double t=0.5; // approving time 0 0.5 0.1 
long long sum_tips=0;
int TipDistribution[3*50]={0};

void initialization();
void generateFirstTransaction();
void simulation();
list<int> tsa();
double cTime();     // 原本：平均 (t5 - t3)
double cTime_05();  // 新增：平均 (t5 - t0)

int main(int argc, char** argv) {
	
	for(k=2;k<=5;k++)
	{
		for(lambda=5;lambda<=50;lambda=lambda+5)
		{
			initialization();
			generateFirstTransaction();
			simulation();
			cout << lambda << " "
			     << k << " "
			     << (double)k/(k-1)*lambda << " "
			     << sum_tips/(double)numArrivals << " "
			     << cTime() << " "        // 平均 (t5 - t3)
			     << cTime_05() << endl;  // 平均 (t5 - t0)
			
		/*	for(int j=0;j<150;j++)
				cout<<j<<" "<<TipDistribution[j]/(double)numArrivals<<endl;
		*/
		}
	}
	return 0;

}

void initialization()
{

	current_id=1;
	current_time=0;
	sum_tips=0;
	event_list.clear();
	tip_set.clear();
	site genesis;
	tip_set.push_back(genesis.getID());
	for(int i=0;i<numArrivals+1;i++)
	{
		timeStamp[i][0]=0;
		timeStamp[i][1]=0;
		T0[i]=0;              // 新增：重設 t0
	}
	for(int j=0;j<150;j++)
		TipDistribution[j]=0;
}



double exp_time(double rate)
{
	double r=(rand()+1)/(double)(RAND_MAX+2);
	return -1/rate*log(1-r);
}

void generateFirstTransaction()
{
	double interArrvialTime=exp_time(lambda);
	list<int> stip;
	site arrival(current_id,0,current_time+interArrvialTime,stip);
	current_id++;
	event_list.push_back(arrival);
	event_list.sort();
}

void simulation()
{
	// 
	while(!event_list.empty())
	{
		site s=event_list.front();
		event_list.pop_front();
//		s.show();
        //t0節點發送新交易
		current_time=s.getTime();
		//t1:選擇未被確認交易
		if(s.getType()==0)  //Create, tip_selection 
		{
			// 記錄這筆交易的 t0（發送時間）
			T0[s.getID()] = current_time;

			int numTips=tip_set.size();
			sum_tips=sum_tips+tip_set.size();
			if(numTips<3*lambda)
				TipDistribution[numTips]++;
		//	cout<<tip_set.size()<<endl;
			list<int> sTip=tsa();
			double kConfirmTime=0;
			
			
			//t1到t2批准未被確認交易並做PoW
			kConfirmTime=tsa_time+sTip.size()*t+exp_time(confirmTime); //exponential PoW only
			//add tsa time+tip size*approval time 
			 
		   //t3新交易被附加到DAG並成為未被確認的交易
			site confirmTip(s.getID(),1,s.getTime()+kConfirmTime,sTip);
			event_list.push_back(confirmTip);
			
			if(s.getID()<numArrivals)
			{
				list<int> stip;
				double interArrivalTime=exp_time(lambda);
				//t0:節點發送新交易
				site nextA(current_id,0,current_time+interArrivalTime,stip);
				current_id++;
				event_list.push_back(nextA);
			}			
		}
		else //confirmed, remove from tip_set, becomes tip  
		{
			//t5
			list<int> tip=s.getTips();
			timeStamp[s.getID()][0]=s.getTime();
			while(!tip.empty())
			{
				int tip_confirm=tip.front();
				if(timeStamp[tip_confirm][1]==0)
					timeStamp[tip_confirm][1]=s.getTime();
				tip_set.remove(tip_confirm);
//				cout<<tip_confirm<<" ";
				tip.pop_front();
			}
//			cout<<endl;
			tip_set.push_back(s.getID());
		}
		event_list.sort();
	}
}

list<int> tsa()
{
	list<int> temp=tip_set;
	if(temp.size()<=k)
		return temp;
	else
	{
		list<int> tip;
		while(tip.size()<k && temp.size()>0)
		{
			int nextRand=rand()%temp.size();
			for(int i=0;i<nextRand;i++)
			{
				int f=temp.front();
				temp.pop_front();
				temp.push_back(f);
			}
			
			tip.push_back(temp.front());
			temp.pop_front();
		}
		return tip;
	}
	
}

double cTime()
{
	double ctime=0;
	long long n=0;
	for(int i=1;i<numArrivals;i++)
	{
		if(timeStamp[i][1]>0)
		{
			ctime=ctime+timeStamp[i][1]-timeStamp[i][0]; // t5 - t3
			n++;
		}
	}
	return ctime/n;
}

// 新增：平均 (t5 - t0) = 從交易發送到第一次被批准完成
double cTime_05()
{
	double ctime=0;
	long long n=0;
	for(int i=1;i<numArrivals;i++)
	{
		// 有被批准過，且有紀錄 t0
		if(timeStamp[i][1]>0 && T0[i]>0)
		{
			ctime = ctime + timeStamp[i][1] - T0[i]; // t5 - t0
			n++;
		}
	}
	return ctime/n;
}








