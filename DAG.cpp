// poisson_distribution
#include <iostream>
#include <random>
#include <list>
#include <cstdlib>
#include <time.h>
using namespace std;

void initialization();
int Poisson(double);
void TipSelection(int,int,int,int);
int UpdateTips(int);
void show();

const int rounds=10000;
const double lambda=50;
const double K=2;
const int maxMessages=rounds*lambda*1.1;
int Approve[maxMessages][2];
int ConfirmTime[maxMessages][2];
int m[rounds+1];
int tips[maxMessages];
list<int> beDirectedApprove[maxMessages];
int sumTips=0;

int totalMessages=0;
default_random_engine generator;
int last_confirm=-1;

int main()
{
	initialization();
	int currentID=1;
	for(int i=1;i<=rounds;i++)
	{
		int numberOfMessages=Poisson(lambda);
	//	if(i%100==0)
	//		cout<<"Round "<<i<<" Messages: "<<numberOfMessages<<endl;
		m[i]=numberOfMessages;
		int numTips=UpdateTips(currentID);
		sumTips=sumTips+numTips;
		cout<<i<<" "<<numTips<<" "<<numberOfMessages<<endl;
		for(int j=currentID;j<currentID+numberOfMessages;j++)
		{
			ConfirmTime[j][0]=i;
			TipSelection(numTips,currentID,K,j);
		}
		
//		show();
		totalMessages=totalMessages+numberOfMessages;
		currentID=currentID+numberOfMessages;

	}
//	cout<<"Total: "<<totalMessages<<endl;
	int sumConfirmTime=0;
	int maxConfirmTime=0;
	int confirmedMessage=0;
	for(int i=0;i<totalMessages;i++)
	{
		if(ConfirmTime[i][1]!=-1)
		{
			int CT=ConfirmTime[i][1]-ConfirmTime[i][0];
			if(CT>maxConfirmTime)
				maxConfirmTime=CT;
			sumConfirmTime=sumConfirmTime+CT;
			confirmedMessage++;
		}
	}
	cout<<lambda<<" "<<K<<" "<<(double)sumTips/rounds<<" "<<sumConfirmTime/(double)confirmedMessage<<" "<<maxConfirmTime<<endl;
	return 0;
}

void initialization()
{
	m[0]=1;
	srand(0);
	for(int i=0;i<=maxMessages;i++)
	{
		Approve[i][0]=-1;
		Approve[i][1]=-1;
		ConfirmTime[i][0]=-1;
		ConfirmTime[i][1]=-1;
	}
	ConfirmTime[0][0]=0;
}
int Poisson(double mean)
{
  	poisson_distribution<int> distribution(mean);
    int number = distribution(generator);
    return number;
}

int UpdateTips(int currentID)
{
	int numberOfTips=0;
//	cout<<"Last Confirm "<<last_confirm<<endl;
	for(int i=0;i<currentID;i++)
	{
		if(beDirectedApprove[i].size()==0)
		{
			
			tips[numberOfTips]=i;
//			cout<<i<<" ";
			numberOfTips++;
		}
	}
//	cout<<endl;
//	cout<<"Number of Tips: "<<numberOfTips<<endl;
	return numberOfTips;
}

void TipSelection(int numTips,int currentID,int numberConfirm, int messageID)
{
//repetition
/*
	for(int i=0;i<numberConfirm;i++)
	{
		int randNumber=rand()%numTips;
		Approve[messageID][i]=tips[randNumber];
		ConfirmTime[tips[i]][1]=ConfirmTime[messageID][0];
		beDirectedApprove[tips[randNumber]].remove(messageID);
		beDirectedApprove[tips[randNumber]].push_back(messageID);
	}

*/		
// No repetition	
	if(numTips<=numberConfirm)
	{
		for(int i=0;i<numTips;i++)
		{
			Approve[messageID][i]=tips[i];
			ConfirmTime[tips[i]][1]=ConfirmTime[messageID][0];
//			cout<<"Message "<<messageID<<" approves message "<<tips[i]<<endl;
			beDirectedApprove[i].push_back(messageID);
		}

	}
	else
	{
		int confirm[numberConfirm];
		for(int i=0;i<numberConfirm;i++)
		{
			bool repeat = true;
			do{
				repeat=false;
				int n=rand()%numTips;
				confirm[i]=n;
				for(int j=0;j<i;j++)
				{
					if(confirm[j]==n)
						repeat=true;
				}
				
			}while(repeat);
		}
		for(int k=0;k<numberConfirm;k++)
		{
			Approve[messageID][k]=tips[confirm[k]];
			ConfirmTime[tips[confirm[k]]][1]=ConfirmTime[messageID][0];
//			cout<<"Message "<<messageID<<" approves message "<<tips[confirm[k]]<<endl;
			beDirectedApprove[tips[confirm[k]]].push_back(messageID);
		}
		
	}
	
}

void show()
{
	for(int i=0;i<maxMessages;i++)
	{
		cout<<i<<" "<<Approve[i][0]<<" "<<Approve[i][1]<<endl;
	}
}
