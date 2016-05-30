#include"BigData.h"
#include<assert.h>
BigData::BigData(INT64 value)
	:_value(value)
{
	INT64ToString();
}

BigData::BigData(const char* pData)
{
	//"1234567890" "456789qwe" "+""0002345"
	// "+123456789""+"
	if(NULL == pData)
	{
		assert(false);
		return;
	}
	char *pStr = (char*)pData;
	char cSymbol = pData[0];
	if('+'==cSymbol || '-'==cSymbol)
	{
		pStr++;
	}
	else if(cSymbol >= '0'&& cSymbol <='9')
	{
		cSymbol = '+';
	}
	else  //无效的情况
	{
		return;
	}
	//"0002345"去掉前置0
	while('0'==*pStr )
		pStr++;
	_strData.resize(strlen(pData)+1);
	_strData[0]=cSymbol;
	//"456789qwe
	_value=0;
	int iCount = 1;
	while(*pStr>='0'&& *pStr<='9')
	{
		//转换
		_value=_value*10+(*pStr-'0');
		_strData[iCount++]=*pStr;
		pStr++;
	}
	_strData.resize(iCount);
	//将结果保存至_pData中
	if(cSymbol=='-')
	{
		_value=0-_value;
	}
}
bool BigData::ISINT64overflow()const
	{
		//923372036854775808
		//-923372036854775808
		std::string temp("+9223372036854775807");
		if(_strData[0]=='-')
		{
			temp="-9223372036854775808";
		}
		//return _strData<temp; //在这里不可以这么比
		if(_strData.size()<temp.size())
		{
			return true;
		}
		else if((_strData.size()==temp.size())&&_strData<=temp)
		{
			return true;
		}
	return false;
	}
std::ostream& operator<<( std::ostream& _cout,const BigData& bigdata)
{
	if(bigdata.ISINT64overflow())
	{
		_cout<<bigdata._value<<std::endl;
	}
	else
	{
		char* pData=(char*)bigdata ._strData .c_str ();
		if('+'==pData[0])
		{
			pData++;
		}
		_cout<<pData<<std::endl;
	}
	//_cout<<bigdata._value  <<std::endl;
	return _cout;
}
BigData BigData::operator-(const BigData& bigdata)
{
	if(ISINT64overflow()&&bigdata.ISINT64overflow())
	{
		if(_strData[0]==bigdata._strData[0])
		{
			return BigData (_value -bigdata._value );
		}
		else
		{
			if((_value >0&& MAX_INT64 +bigdata ._value >=_value )
				||(_value <0&&MIN_INT64 +bigdata ._value<=_value  ))
			{
				return BigData (_value +bigdata ._value );
			}
		}
	}
	if(_strData[0]==bigdata._strData [0]) //异号相当于调用加法
	{
		return BigData (Add(_strData ,bigdata ._strData ).c_str());
	}
	return BigData (Sub(_strData ,bigdata ._strData ).c_str());

}
BigData BigData::operator+(const BigData& bigdata)
{
	
	if(ISINT64overflow() && bigdata.ISINT64overflow ())
	{
		//+-
		if(_strData[0]!=bigdata ._strData[0])
		{
			 BigData (_value+bigdata._value );
		}
		
		else
		{
			//8+3 = 11 10-3 =7<8 //溢出
			//-7 -4 = -11
			INT64 temp =  MIN_INT64 -_value;
			if((_value >= 0 && MAX_INT64 -_value >=bigdata ._value) 
		||(_value< 0 && temp <= bigdata._value ))
			{
				return BigData(_value+bigdata._value);
			}
		}		
	}
	//至少一个溢出
	//结果溢出
	if(_strData[0]==bigdata._strData[0])
	{
		return BigData(Add(_strData,bigdata._strData).c_str());//c_str()返回字符串首地址
	}
	return BigData(Sub(_strData,bigdata._strData).c_str());
}

BigData BigData :: operator*(const BigData& bigdata)
{
	if(_value == 0||bigdata ._value == 0)
	{
		return BigData (INT64 (0));
	}
	if(ISINT64overflow ()&&bigdata.ISINT64overflow ())
	{
		if(_strData[0]==bigdata ._strData[0])
		{
			//10 /-2 = -5
			if((_value >0&&MAX_INT64/_value >=bigdata ._value )
				||(_value <0&&MAX_INT64 /_value >=bigdata ._value ))
			{
				return BigData(_value *bigdata ._value );
			}

		}
	}
	return BigData(Mul(_strData ,bigdata._strData ).c_str ());
}

void BigData::INT64ToString()
{
	INT64 tem = _value;
	char cSymbol = '+';
	if(_value<0)
	{
		cSymbol = '-';
		//tem = 0-tem;
	}
	//_str
	//1234567
	//7654321
	_strData.append(1,cSymbol);  //追加
	while(tem)
	{
		int c=tem%10;
		if(c<0)
		{
			c = 0-c;
		}
		_strData.append (1,c+'0');
		tem/=10;
	}
	char* pLeft = (char*)_strData.c_str()+1;
	char* pRight= pLeft+_strData.size()-2;
	while(pLeft<pRight)
	{
		char cTemp = *pLeft;
		*pLeft++ = *pRight++;
		*pRight-- = cTemp;
	}
}
 std::string BigData::Add(std::string left,std::string right)
 {
	int iLsize = left.size();
	int iRsize = right.size();
	char cSymbol = '+';
	if(iLsize<iRsize)
	{
		std::swap(left,right);
		std::swap(iLsize,iRsize);
	}
	std::string sRet;
	sRet.resize(iLsize+1);  //为结果分配空间
	sRet[0]=left[0];
	char Step = 0;
	//处理位数不同的时候
	for(int iIdx=1;iIdx<iLsize;iIdx++)
	{
		char cRet = left[iLsize-iIdx]-'0'+Step;
		if(iIdx<iRsize)
		{
			cRet+=(right[iRsize-iIdx]-'0');
		}
		sRet[iLsize-iIdx+1] = cRet%10+'0';
		Step=cRet/10;
	}
	sRet[1]= Step+'0';
	return sRet;
 }
std::string BigData::Sub(std::string left,std::string right)
{
	int iLsize = left.size();
	int iRsize = right.size();
	char cSymbol = left[0];

	cSymbol = '+';
	if(iLsize<iRsize||(iLsize ==iRsize &&left<right))
	{
		std::swap(left,right);
		std::swap(iLsize,iRsize);
		if(cSymbol == '+')
		{
			cSymbol = '-';
		}
		else
		{
			cSymbol = '+';
		}
	}
	//保存结果的，同样要放在string里
	std::string strRet;
	strRet.resize (left.size ());
	strRet[0]=cSymbol ;
	//1.从低到高，取left每一位
	//2.从低到高，取left每一位
	//3.判断是否借位
	for(int Idx = 1;Idx<iLsize;++Idx)
	{
		char cRet = left[iLsize - Idx]-'0';
		if(Idx<iRsize)
		{
			cRet -= (right[iRsize-Idx]-'0');
		}
		if(cRet<0)
		{
			left[iLsize-Idx-1]-=1;
			cRet += 10;
		}
		strRet[iLsize - Idx]=cRet +'0';
	}
	return strRet;
}
std::string BigData::Mul(std::string left,std::string right)
{
    char cSymbol = '+';
	if(left[0]!=right[0])
	{
		cSymbol = '-';
	}
	
	int iLsize = left.size();
	int iRsize = right.size();
	if(iLsize>iRsize)
	{
		std::swap(left,right);
		std::swap(iLsize,iRsize);
	}
	std::string sRet;
	//sRet.resize(iLsize+iRsize -1);
	sRet.assign (iLsize +iRsize -1,'0');
	sRet[0]=cSymbol ;
	int iDatalen = sRet.size ();
	int ioffset = 0;
	//取左边以为与右边相乘
	for(int iLdx=1;iLdx<iLsize;++iLdx )
	{
		char cLeft = left[iLsize-iLdx ]-'0';
		char cStep = 0;
		if(cLeft == 0)
		{
			ioffset ++;
			continue;
		}
		//99*999
		//9*999

		for(int iRdx=1;iRdx<iRsize;++iRdx)
		{
			char cRet = cLeft*(right [iRsize -iRdx ]-'0');
			cRet += sRet[iDatalen -iRdx-ioffset]-'0';
			sRet[iDatalen -iRdx-ioffset]=(cRet%10)+'0';
			cStep = cRet /10;
		}
		sRet[iDatalen -iRsize-ioffset ]+=cStep ;
		ioffset ++;
	}

	return sRet ;
}


