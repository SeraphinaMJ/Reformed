#include <iostream>
#include <vector>
using namespace std;

unsigned ClosestEnemyII(const std::vector<string>& strArr) {

	unsigned height = strArr.size();
	unsigned width = strArr[0].length();

	int x_p = 0;
	int y_p = 0;

	for(int y = 0; y < height; ++y)
	{
		for(int x = 0; x < width ; ++x)
		{
			if( strArr[y][x] == '1')
			{
					x_p = x;
					y_p = y;
					goto get_out;
			}
		}
	}
	get_out:

	unsigned min = -1; /* do not fix */

	for(int y = 0; y < height; ++y)
	{
		for(int x = 0; x < width ; ++x)
		{
			if( strArr[y][x] == '2')
			{
				int horz = 0;
				if(x > x_p)
				{
					int left  = x - x_p;
					int right = x_p + (width - x);

					horz = std::min(left, right);
				}
				else
				{
					int right = x_p - x;
					int left = x + (width - x_p);

					horz = std::min(left, right);
				}

				int vertz = 0;
				if(y > y_p)
				{
					int down  = y - y_p;
					int up = y_p + (height - y);

					vertz = std::min(down, up);
				}
				else
				{
					int up = y_p - y;
					int down = y + (height - y_p);

					vertz = std::min(down, up);
				}

				unsigned result = horz + vertz;
				if(result < min)
					min = result;
			}
		}
	}

	return min;
}

int main()
{
  /* Note: In C++ you first have to initialize an array and set
     it equal to the stdin to test your code with arrays. */

  std::vector<string> A = gets(stdin);
  cout << ClosestEnemyII(A);
  return 0;

}
