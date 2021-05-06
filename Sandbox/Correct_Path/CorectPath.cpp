#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

string generate_moves(int position_x, int position_y, string& results)
{
	string moves;

	if(position_x + 1 <= 4 && ( results.empty() || results.back() != 'l')) // right
		moves.push_back('r');
	if(position_x - 1 >= 0 && ( results.empty() || results.back() != 'r')) // left
		moves.push_back('l');
	if(position_y - 1 >= 0 && ( results.empty() || results.back() != 'd')) // up
		moves.push_back('u');
	if(position_y + 1 <= 4 && ( results.empty() || results.back() != 'u')) // down
		moves.push_back('d');

	//cout << moves << " Pos: (" << position_x << ", " << position_y << endl;

	return moves;
}

bool recursive_solve(const string& str, unsigned position_string, unsigned position_x, unsigned position_y, string& results, bool * matrix)
{
	if(position_x > 4 || position_y > 4) // unsigned praps at negative values
		return false;
	if(matrix[position_y * 5 + position_x] == true)
		return false;

	matrix[position_y * 5 + position_x] = true;

	if(position_string == str.length()) // base case
	{
		if(position_x == 4 && position_y == 4)
			return true;
		matrix[position_y * 5 + position_x] = false;
		return false;
	}

	if((4-position_x) + (4-position_y) > str.length() - position_string)
	{
		matrix[position_y * 5 + position_x] = false;
		return false;
	}

	// solving
	string moves;

	if(str[position_string] == '?')
		moves = generate_moves(position_x, position_y, results);
	else
		moves = str[position_string];

	for(const auto& move : moves)
	{
		results.push_back(move);

		switch (move)
		{
			case 'r':
				if(recursive_solve(str, 1+position_string, 1+position_x, position_y, results, matrix))
					return true;
				break;
			case 'l':
				if(recursive_solve(str, 1+position_string, position_x - 1, position_y, results, matrix))
					return true;
				break;
			case 'u':
				if(recursive_solve(str, 1+position_string, position_x, position_y - 1, results, matrix))
					return true;
				break;
			case 'd':
				if(recursive_solve(str, 1+position_string, position_x, 1+position_y, results, matrix))
					return true;
				break;
		}

		results.pop_back();
	}

	matrix[position_y * 5 + position_x] = false;
	return false;
}

string CorrectPath(string str)
{
	string results;
	bool matrix[5][5] = {0};

	recursive_solve(str, 0, 0, 0, results, matrix[0]);

	return results;
}

int main()
{
	// keep this function call here
	cout << CorrectPath(gets(stdin));
	return 0;
}
