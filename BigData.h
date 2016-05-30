#ifndef BIG_DATA_H
#define BIG_DATA_H
#include<string>
//��������
//"1234567890123456789"  //�����ַ��������� 
typedef long long INT64;
#define UN_INIT  0xcccccccccccccccc
#define MAX_INT64 0x7FFFFFFFFFFFFFFF
#define MIN_INT64 0x8000000000000000
class BigData 
{
public:
	BigData(INT64 value=0xcccccccccccccccc);
	BigData(const char* pData);
	friend std::ostream& operator<<( std::ostream& _cout,const BigData& bigdata);
	BigData operator+(const BigData& bigdata);//���ڱȺ������ڳ���ʱ��
	BigData operator-(const BigData& bigdata);
	BigData operator*(const BigData& bigdata);
	BigData operator/(const BigData& bigdata);
private:
	bool ISINT64overflow()const;
    std::string Add(std::string left,std::string right);
	std::string Sub(std::string left,std::string right);
	std::string Mul(std::string left,std::string right);
	std::string Dev(std::string left,std::string right);
	void INT64ToString();
private:
	INT64 _value;
	std::string  _strData;                //char* pData;
};

#endif