
#include "common.h"

#ifndef WIN32

void __debugbreak()
{

}

#endif // WIN32



uint64_t unixtime()
{
#ifdef WIN32
	struct __timeb64 tstruct;
	_ftime64_s( &tstruct );
#else
	struct timeb tstruct;
	ftime( &tstruct );
#endif
	return tstruct.millitm + tstruct.time*1000;
}


void a_swap(unsigned char * a, unsigned char * b)
{
	register char c, d;
	c = *a;
	d = *b;
	*b = c;
	*a = d;
}
void ByteSwap5(unsigned char * b, int n)
{
	register int i = 0;
	register int j = n-1;
	while (i<j)
	{
		a_swap(&b[i], &b[j]);
		i++, j--;
	}
}

int htoi(char hex)
{
	switch (hex)
	{
	case 49:
		return 1;
	case 50:
		return 2;
	case 51:
		return 3;
	case 52:
		return 4;
	case 53:
		return 5;
	case 54:
		return 6;
	case 55:
		return 7;
	case 56:
		return 8;
	case 57:
		return 9;
	case 48:
		return 0;
	case 97://a
		return 10;
	case 98://b
		return 11;
	case 99://c
		return 12;
	case 100://d
		return 13;
	case 101://e
		return 14;
	case 102://f
		return 15;
	}
	return 0;
}
char itoh(int num)
{
	switch (num)
	{
	case 1:
		return 49;
	case 2:
		return 50;
	case 3:
		return 51;
	case 4:
		return 52;
	case 5:
		return 53;
	case 6:
		return 54;
	case 7:
		return 55;
	case 8:
		return 56;
	case 9:
		return 57;
	case 0:
		return 48;
	case 10://a
		return 97;
	case 11://b
		return 98;
	case 12://c
		return 99;
	case 13://d
		return 100;
	case 14://e
		return 101;
	case 15://f
		return 102;
	}
	return 0;
}

#ifdef WIN32
int vasprintf(char **str, const char *fmt, va_list ap)
{
	int ret = -1;
	va_list ap2;
	char *string, *newstr;
	size_t len;

	VA_COPY(ap2, ap);
	if ((string = (char*)malloc(INIT_SZ)) == 0)
		goto fail;

	ret = vsnprintf(string, INIT_SZ, fmt, ap2);
	if (ret >= 0 && ret < INIT_SZ) { /* succeeded with initial alloc */
		*str = string;
	} else if (ret == INT_MAX || ret < 0) { /* Bad length */
		goto fail;
	} else {        /* bigger than initial, realloc allowing for nul */
		len = (size_t)ret + 1;
		if ((newstr = (char*)realloc(string, len)) == 0) {
			free(string);
			goto fail;
		} else {
			va_end(ap2);
			VA_COPY(ap2, ap);
			ret = vsnprintf(newstr, len, fmt, ap2);
			if (ret >= 0 && (size_t)ret < len) {
				*str = newstr;
			} else { /* failed with realloc'ed string, give up */
				free(newstr);
				goto fail;
			}
		}
	}
	va_end(ap2);
	return (ret);

fail:
	*str = 0;
	errno = ENOMEM;
	va_end(ap2);
	return (-1);
}

int asprintf(char **str, const char *fmt, ...)
{
	va_list ap;
	int ret;

	*str = 0;
	va_start(ap, fmt);
	ret = vasprintf(str, fmt, ap);
	va_end(ap);

	return ret;
}
#endif

extern bool ci_equal(char ch1, char ch2);


bool ci_equal(char ch1, char ch2)
{
	return toupper((unsigned char)ch1) == toupper((unsigned char)ch2);
}

size_t ci_find(const std::string& str1, const std::string& str2)
{
	std::string::const_iterator pos = std::search(str1. begin ( ), str1. end ( ), str2.
		begin ( ), str2. end ( ), ci_equal);
	if (pos == str1. end ( ))
		return std::string::npos;
	else
		return pos - str1. begin ( );
}

std::string makesafe(std::string in)
{
	std::stringstream ss;
	for (int i = 0; i < in.size(); ++i)
	{
		if (((in[i] >= '0') && (in[i] <= '9')) || ((in[i] >= 'A') && (in[i] <= 'Z')) || ((in[i] >= 'a') && (in[i] <= 'z')) || (in[i] == '_') || (in[i] == '.') || (in[i] == '/') || (in[i] == ':') || (in[i] == '@') || (in[i] == '|'))
			ss << in[i];
		else
			ss << ' ';
	}
	return ss.str();
}

char * strtolower(char * x)
{
	for(int i = 0; i < strlen(x); ++i)
	{
		if(x[i] >= 65 && x[i] <= 90)
		{
			x[i] += 32;
		}
	}
	return x;
}


double randn_notrig(double mu, double sigma)
{
	static bool deviateAvailable=false;	//	flag
	static double storedDeviate;			//	deviate from previous calculation
	double polar, rsquared, var1, var2;

	//	If no deviate has been stored, the polar Box-Muller transformation is
	//	performed, producing two independent normally-distributed random
	//	deviates.  One is stored for the next round, and one is returned.
	if (!deviateAvailable) {

		//	choose pairs of uniformly distributed deviates, discarding those
		//	that don't fall within the unit circle
		do {
			var1=2.0*( double(rand())/double(RAND_MAX) ) - 1.0;
			var2=2.0*( double(rand())/double(RAND_MAX) ) - 1.0;
			rsquared=var1*var1+var2*var2;
		} while ( rsquared>=1.0 || rsquared == 0.0);

		//	calculate polar transformation for each deviate
		polar=sqrt(-2.0*log(rsquared)/rsquared);

		//	store first deviate and set flag
		storedDeviate=var1*polar;
		deviateAvailable=true;

		//	return second deviate
		return var2*polar*sigma + mu;
	}

	//	If a deviate is available from a previous call to this function, it is
	//	returned, and the flag is set to false.
	else {
		deviateAvailable=false;
		return storedDeviate*sigma + mu;
	}
}

int32_t ndice(int _throw, int range)
{
	int32_t i, ret;

	if (range <= 0 || _throw <= 0) return 0;

	ret = 0;
	for (i = 1; i <= _throw; i++) {

		ret += (rand() % range) + 1;
	}

	return ret;
}

int32_t dice(int _throw, int range)
{
	if(_throw > 10)
		return (int32_t)(
		(_throw * range / 2) + (_throw * randn_notrig()/3) + (_throw / 2)
		);
	else
		return ndice(_throw, range);
	//for decimal mean values
	//return (_throw * range / 2) + (_throw * randn_notrig()/3) + ((range%2) / 2.0) + (_throw / 2);
}

#define GetNibble(var, index)		(  ( var & ( 0xF << (4*index) ) ) >> (4*index)  )
#define GetBit(var, index)			( (var & (1 << index)) >> index )
#define ToBit(index)					(1 << index)

void SetNibble(uint32_t &var, uint8_t index, uint8_t val)
{
	index *= 4;
	var &= ~(0xF << index);
	var |= val << index;	
}

void SetBit(uint32_t &var, uint8_t index, bool val)
{
	var &= 0xffffffff - (0x1 << index);
	var |= val << index;
}