#include"math.hpp"
#include"errno.hpp"
#define ROOT_PRECISION 0.000001
double floor(double num)
{
	return static_cast<int>(num)-(num<0);
}
double root(double br, int t);
double pow(double br, double exponent)
{
	double r = 1;
	double t = floor(exponent);//For example 4^2.8 = 4^(2+0.8)=4^2 + 4^0.8.  floor(2.8)=2 >>>>
	if (t)
	{
		for (int i = 0; i < static_cast<int>(t); i++)r *= br;
	}
	double o = exponent - t; //2.8-floor(2.8)=0.8
	if (o)
	{
		//Convert to fraction with denominator of power of 10.
		double denominator=1,numerator=o;
		while ((numerator - floor(numerator)) != 0.0)
		{
			denominator *= 10;
			numerator *= 10;
		}
		//a^(b/c)=root_c(a^b)
		r *= root(pow(br, numerator), denominator);
	}
	return r;
}
double root(double br, int t)
{
	double ch = br / 2.0;
	double le = 0.0;
	double he = br;
	double pw;
	while (1)
	{
		pw = pow(ch, t);
		if (pw>br + ROOT_PRECISION)
		{
			he = ch;
			ch = (le + ch) / 2;
		}
		else if (pw<br - ROOT_PRECISION)
		{
			le = ch;
			ch = (ch + he) / 2;
		}
		else break;
	}
	return ch;
}
double sqrt(double num)
{
	double ch = num / 2.0;
	double le = 0.0;
	double he = num;
	double pw;
	while (1)
	{
		pw = pow(ch, 2);
		if (pw>num + ROOT_PRECISION)
		{
			he = ch;
			ch = (le + ch) / 2;
		}
		else if (pw<num - ROOT_PRECISION)
		{
			le = ch;
			ch = (ch + he) / 2;
		}
		else break;
	}
	return ch;
}
