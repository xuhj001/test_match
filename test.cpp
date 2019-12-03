#include <cstring>
#include <string>
#include <vector>
#include <atomic>
#include <iostream>
#include <algorithm>

void ReplaceStr(std::string& strContent, const char* strSrc, const char* strDest,bool ignoreCase=true)
{
	std::string strCopy(strContent);
	std::string strSrcCopy(strSrc);

	std::string::size_type pos1 = 0;
	std::string::size_type pos2 = 0;
	std::string::size_type srclen = strlen(strSrc);
	std::string::size_type dstlen = strlen(strDest);
	int diff = dstlen - srclen;

	//忽略大小写时，先都转换成小写，然后定位位置，再对源字符串进行替换
	if(ignoreCase)
	{
		//源字符串转化成小写
		std::transform(strCopy.begin(),strCopy.end(),strCopy.begin(),tolower);
		std::transform(strSrcCopy.begin(),strSrcCopy.end(),strSrcCopy.begin(),tolower);
	}
    std::cout<<"diff "<<diff <<std::endl;

	while( (pos1=strCopy.find(strSrcCopy, pos1)) != std::string::npos)
	{
	    std::cout<<"find strSrcCopy: "<<strSrcCopy<<" pos1: "<<pos1 <<std::endl;
	    pos2 = pos1 + diff;
	    std::cout<<"strContent: "<<strContent<<" pos2: "<<pos2 <<std::endl;
		strContent.replace(pos2, srclen, strDest);
//	    std::cout<<"strContent1:"<<strContent <<std::endl;

	    pos1 += srclen;
//		pos2 += dstlen;
	}

	return;
}

//std::string ReplaceStr(std::string s,std::string a, std::string b){
//   std::string lower_s;
//   std::transform(s.begin(), s.end(), lower_s.begin(), ::tolower);
//   std::transform(a.begin(), a.end(), a.begin(), ::tolower);
//   auto position=lower_s.find(a);
//   while(position!=std::string::npos){
//        s.replace(position, a.size(), b);
//        position=lower_s.find(a);
//   }
//
//   return s;
//}

int main()
{
    std::string text = "Abc 123 abc 456 Def";
    std::string strSrc = "ABC";
    std::string strDest = "**";
    std::cout<<"text "<<text <<std::endl;
    ReplaceStr(text, strSrc.c_str(), strDest.c_str());
    std::cout<<"text "<<text <<std::endl;

    return 0;
}

