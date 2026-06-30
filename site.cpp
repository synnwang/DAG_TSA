/*
本著作係採用創用 CC 姓名標示-非商業性 3.0 台灣 授權條款授權.
Contributor: Sheng-Wei Wang
*/

#include <iostream>
#include <list>
#include "site.h"

using namespace std;

site::site(){
	id=0;
	type=0;
	timeStamp=0;
	selectTip.clear();
}

site::site(int i, int p, double t, list<int> l)
{
	id=i;
	type=p;
	timeStamp=t;
	selectTip=l;
}

void site::show(){
	if(type==0)
		cout<<"Transaction "<<id<<" is created at timestamp "<<timeStamp<<endl;
	else
		cout<<"Transaction "<<id<<" is confirmed at timestamp "<<timeStamp<<endl;		
}



int site::getID()
{
	return id;
}

double site::getTime()
{
	return timeStamp;
}
		
bool site::getType()
{
	return type;
}

bool site::operator<(const site& s) const
{
    if(timeStamp<s.timeStamp)
        return true;
    else
        return false;
}

list<int> site::getTips()
{
	return selectTip;
}

