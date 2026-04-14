

namespace glm
{
	
	GLM_FUNC_QUALIFIER int pow(int x, uint y)
	{
		if(y == 0)
			return x >= 0 ? 1 : -1;

		int result = x;
		for(uint i = 1; i < y; ++i)
			result *= x;
		return result;
	}

	
	GLM_FUNC_QUALIFIER int sqrt(int x)
	{
		if(x <= 1) return x;

		int NextTrial = x >> 1;
		int CurrentAnswer;

		do
		{
			CurrentAnswer = NextTrial;
			NextTrial = (NextTrial + x / NextTrial) >> 1;
		} while(NextTrial < CurrentAnswer);

		return CurrentAnswer;
	}


namespace detail
{
	GLM_FUNC_QUALIFIER unsigned int ones32(unsigned int x)
	{
		/* 32-bit recursive reduction using SWAR...
		but first step is mapping 2-bit values
		into sum of 2 1-bit values in sneaky way
		*/
		x -= ((x >> 1) & 0x55555555);
		x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
		x = (((x >> 4) + x) & 0x0f0f0f0f);
		x += (x >> 8);
		x += (x >> 16);
		return(x & 0x0000003f);
	}
}

	
/*
	GLM_FUNC_QUALIFIER unsigned int floor_log2(unsigned int x)
	{
		x |= (x >> 1);
		x |= (x >> 2);
		x |= (x >> 4);
		x |= (x >> 8);
		x |= (x >> 16);

		return _detail::ones32(x) >> 1;
	}
*/
	
	GLM_FUNC_QUALIFIER int mod(int x, int y)
	{
		return ((x % y) + y) % y;
	}

	
	template<typename genType>
	GLM_FUNC_QUALIFIER genType factorial(genType const& x)
	{
		genType Temp = x;
		genType Result;
		for(Result = 1; Temp > 1; --Temp)
			Result *= Temp;
		return Result;
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<2, T, Q> factorial(
		vec<2, T, Q> const& x)
	{
		return vec<2, T, Q>(
			factorial(x.x),
			factorial(x.y));
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<3, T, Q> factorial(
		vec<3, T, Q> const& x)
	{
		return vec<3, T, Q>(
			factorial(x.x),
			factorial(x.y),
			factorial(x.z));
	}

	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<4, T, Q> factorial(
		vec<4, T, Q> const& x)
	{
		return vec<4, T, Q>(
			factorial(x.x),
			factorial(x.y),
			factorial(x.z),
			factorial(x.w));
	}

	GLM_FUNC_QUALIFIER uint pow(uint x, uint y)
	{
		if (y == 0)
			return 1u;

		uint result = x;
		for(uint i = 1; i < y; ++i)
			result *= x;
		return result;
	}

	GLM_FUNC_QUALIFIER uint sqrt(uint x)
	{
		if(x <= 1) return x;

		uint NextTrial = x >> 1;
		uint CurrentAnswer;

		do
		{
			CurrentAnswer = NextTrial;
			NextTrial = (NextTrial + x / NextTrial) >> 1;
		} while(NextTrial < CurrentAnswer);

		return CurrentAnswer;
	}

	GLM_FUNC_QUALIFIER uint mod(uint x, uint y)
	{
		return x - y * (x / y);
	}



	GLM_FUNC_QUALIFIER unsigned int nlz(unsigned int x)
	{
		return 31u - static_cast<unsigned int>(findMSB(x));
	}
/*
#else

	
	GLM_FUNC_QUALIFIER unsigned int nlz(unsigned int x)
	{
		int y, m, n;

		y = -int(x >> 16);      
		m = (y >> 16) & 16;  
		n = 16 - m;          
		x = x >> m;          
							
		y = x - 0x100;       
		m = (y >> 16) & 8;   
		n = n + m;
		x = x << m;

		y = x - 0x1000;      
		m = (y >> 16) & 4;   
		n = n + m;
		x = x << m;

		y = x - 0x4000;      
		m = (y >> 16) & 2;   
		n = n + m;
		x = x << m;

		y = x >> 14;         
		m = y & ~(y >> 1);   
		return unsigned(n + 2 - m);
	}

#endif
*/
}
