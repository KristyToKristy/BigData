#include"BigData.h"
#include<assert.h>
#include<string.h>
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
		char* pData=(char*)bigdata._strData.c_str();
		if('+'==pData[0])
		{
			pData++;
		}
		_cout<<pData<<std::endl;
	}
	//_cout<<bigdata._value  <<std::endl;
	return _cout;
}

BigData BigData::operator+(const BigData& bigdata)
{

	if (ISINT64overflow() && bigdata.ISINT64overflow())
	{
		//+-
		if (_strData[0] != bigdata._strData[0])
		{
			BigData(_value + bigdata._value);
		}

		else
		{
			//8+3 = 11 10-3 =7<8 //溢出
			//-7 -4 = -11    -10-（-4）=-6  -6 > -7 溢出
			//比最大临界值大，比最小临界值小就会溢出
			//不溢出的情况
			INT64 temp = MIN_INT64 - _value;
			if ((_value >= 0 && MAX_INT64 - _value >= bigdata._value)
				|| (_value< 0 && temp <= bigdata._value))
			{
				return BigData(_value + bigdata._value);
			}
		}
	}
	//至少一个溢出
	//结果溢出
	if (_strData[0] == bigdata._strData[0])
	{
		return BigData(Add(_strData, bigdata._strData).c_str());//c_str()返回字符串首地址
	}
	return BigData(Sub(_strData, bigdata._strData).c_str());
}

BigData BigData::operator-(const BigData& bigdata)
{
	if(ISINT64overflow() && bigdata.ISINT64overflow())
	{
		if(_strData[0]==bigdata._strData[0])
		{
			return BigData (_value -bigdata._value );
		}
		else
		{
			//17-9=8 10+9=19>17不溢出
			//-12-（-4）= 8  —10+（-4）=-14  -12>-14不溢出
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


BigData BigData :: operator*(const BigData& bigdata)
{
	if(_value == 0||bigdata ._value == 0)
	{
		return BigData (INT64 (0));
	}
	if(ISINT64overflow() && bigdata.ISINT64overflow())
	{
		if(_strData[0] == bigdata ._strData[0])
		{
			//8/2=4 10/2=5 5>4
			//
			if((_value>0 && MAX_INT64/_value >=bigdata ._value )
				||(_value<0 && MAX_INT64/_value >=bigdata ._value ))
			{
				return BigData(_value *bigdata ._value );
			}
			else
			{
				//-10/-2 = 5  -6
				if ((_value>0 && MIN_INT64 / _value <= bigdata._value) ||
					(_value < 0 && MIN_INT64 /_value >= bigdata._value))
				{
					return BigData(_value*bigdata._value);
				}
			}
		}
	}
	return BigData(Mul(_strData ,bigdata._strData ).c_str ());
}
BigData BigData::operator/(const BigData& bigdata)
{
	//先排除0的情况
	//"+0" _value
	if (bigdata._strData[1] == '0')
	{
		assert(false);
	}
	if (ISINT64overflow() && bigdata.ISINT64overflow())
	{
		return BigData(_value / bigdata._value);
	}
	if (_strData.size() < bigdata._strData.size()||
	(_strData.size() == bigdata._strData.size()&&
		strcmp(_strData.c_str()+1,bigdata._strData.c_str +1)<0))
	{
		return BigData(INT64(0));
	}
	if (bigdata._strData == "+1" || _strData == "-1")
	{
		std::string ret = _strData;
		if (_strData[0] != bigdata._strData[0])
		{
			ret[0] = '-';
		}
		return BigData(ret.c_str());
	}

	if (strcmp(_strData.c_str()+1,bigdata._strData.c_str()+1)==0)
	{
		std::string ret = "+1";
		if (_strData[0] != bigdata._strData[0])
		{
			ret[0] = '-';
		}
		return BigData(ret.c_str());
	}
	return BigData(Div(_strData, bigdata._strData).c_str());
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
	//2.从低到高，取right每一位
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
	
	int Lsize = left.size();
	int Rsize = right.size();
	if(Lsize>Rsize)
	{
		std::swap(left,right);
		std::swap(Lsize,Rsize);
	}
	std::string sRet;
	//sRet.resize(iLsize+iRsize -1);
	sRet.assign (Lsize +Rsize -1,'0');
	sRet[0]=cSymbol ;
	int iDatalen = sRet.size ();
	int ioffset = 0;
	//取左边一位与右边相乘
	for(int iLdx=1;iLdx<Lsize;++iLdx )
	{
		char cLeft = left[Lsize-iLdx ]-'0';
		char cStep = 0;
		if(cLeft == 0)
		{
			ioffset ++;
			continue;
		}
		//99*999
		//9*999

		for(int iRdx=1;iRdx<Rsize;++iRdx)
		{
			char cRet = cLeft*(right [Rsize -iRdx ]-'0');
			cRet += sRet[iDatalen -iRdx-ioffset]-'0';   //加上进位
			sRet[iDatalen -iRdx-ioffset]=(cRet%10)+'0';
			cStep = cRet /10;   //进位
		}
		sRet[iDatalen -Rsize-ioffset ]+=cStep ;
		ioffset ++;
	}
	return sRet ;
}

std::string BigData::Div(std::string left, std::string right)
{
	std::string sRet;
	sRet.append(1, '+');
	if (left[0] != right[0])
	{
		sRet[0] = '-';
	}
	char* pLeft = (char*)(left.c_str() + 1);
	char* pRight = (char*)(right.c_str() + 1);
	int DataLen = right.size() - 1;
	int LSize = left.size() - 1;
	for (int iIdx = 0; iIdx <LSize;)
	{
		if (*pLeft == '0')
		{
			sRet.append(1, '0');
			pLeft++;
			iIdx--;
			continue;
		}
		if (!IsLeftStrBig(pLeft, DataLen, pRight, right.size()-1))
		{
			sRet.append(1, '0');//追加一个0
			DataLen++;
			if (DataLen + iIdx > LSize)
			{
				break;
			}
		}
		else
		{
			sRet.append(1, SubLoop(pLeft, DataLen, pRight, right.size() - 1));
			while (*pLeft == '0' && DataLen >0)
			{
				pLeft++;  //把0跳过去   //如果用引用的话，实参形参是一个实体
				iIdx++;
				DataLen--;//长度减1，如090就是3减1
			}
			//"2422222222" 2
			DataLen++;  //数据长度加1下
			if (DataLen + iIdx > LSize)  //保证不超过Size
			{
				break;
			}
		}
	}
	return sRet;
}

bool BigData::IsLeftStrBig(const char* pLeft, int LSize, const char* pRight,int RSize)
{
	//"222222222222222222"
	//"33"
	if (LSize > RSize || LSize == RSize && strcmp(pLeft, pRight) >= 0)
	{
		return true;
	}
	return false;
}
char BigData::SubLoop(char* pLeft, int LSize, char* pRight, int RSize)
{
	assert(pLeft != NULL && pRight != NULL);
	char cRet = '0';
	while (true)
	{
		if (!IsLeftStrBig(pLeft, LSize, pRight, RSize))
		{
			break;
		}
		int LDataLen = LSize-1;
		int RDataLen = RSize-1;
		// 32 33  借位   43-33 LSize=1, 
		while (LDataLen > 0 && RSize > 0)
		{
				char ret = pLeft[LDataLen] - '0';  //左边的最低位
				ret -= pRight[RDataLen] - '0';

				if (ret < 0)
				{
					pLeft[LDataLen-1] -= 1;
					ret += 10;
				}
				pLeft[LDataLen] = ret + '0';
				LDataLen--;
				RDataLen--;
		}
		//要考虑33330000000000000的情况，不能无休止的转下去
		//3333333
		//090  需要跳过0
		while (*pLeft == '0' && LSize > 0)
		{
			pLeft++;
			LSize--;
		}
		cRet++;
	}
	return cRet;
}


