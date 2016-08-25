#include<iostream>
#include"BigData.h"
using namespace std;
void AddTest()
{
	BigData left(1234);
	BigData right(4321);
	cout<<left+right<<endl;

	BigData left1(9223372036854775807);
	BigData right1(2);
	cout<<left1+right1<<endl;

	BigData left2(-9223372036854775808);
	BigData right2(-3);
	cout<<left2+right2<<endl;
}
void SubTest()
{
	//BigData left1(1234);
	//BigData right1(34);
	//cout<<left1-right1<<endl;
	BigData left1("-9223372036854775807");  
	BigData right1("999");
	cout<<left1-right1<<endl;
}
void MulTest()
{
	BigData left1("9999");
	BigData right1("79988888889999999");
	cout<<left1*right1<<endl;

	BigData left2("9999");
	BigData right2("1");
	cout<<left2*right2<<endl;
}

void DivTest()
{
	BigData left2("222222222222222222222222222222");
	BigData right2("33");
	cout << left2 / right2 << endl;

}
int main()
{
	 DivTest();
	
	///*int a=10;
	//int b;*/
	////"1234567890" "+123456789" "456789qwe" "+""0002345"
	//cout<<(atoi("1234567890"))<<endl;
	//cout<<(atoi("+123456789"))<<endl;
	//cout<<(atoi("456789qwe"))<<endl;
	//cout<<(atoi("0002345"))<<endl;
	/*BigData b1(23456);
	BigData b2("12345678");
	BigData b3("+1245656");
	BigData b4("+");
	BigData b5("    ");
	BigData b6("-21324444");
	BigData b7("-0000758435qwe");
	cout<<b1<<endl;
	cout<<b2<<endl;
	cout<<b3<<endl;
	cout<<b4<<endl;
	cout<<b5<<endl;
	cout<<b6<<endl;*/
	system("pause");
	return 0;
}