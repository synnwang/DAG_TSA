/*
本著作係採用創用 CC 姓名標示-非商業性 3.0 台灣 授權條款授權.
Contributor: Sheng-Wei Wang
*/

#ifndef site_h
#define site_h


using namespace std;
#include <list>
class site{
	public:
		site();
		site(int,int,double,list<int>);
		void show();
		bool operator<(const site&) const; //重定義事件排序
		bool getType();
		list<int> getTips();
		int getID();
		double getTime();
	private:
		int id;
		int type; //0: create  1:confirmed
		list<int> selectTip;
		double timeStamp;
}; 

#endif

