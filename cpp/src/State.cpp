#include "State.hpp"
#include "move.hpp"
#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>
#include<iterator>

int		State::tot = 0;

State::State() :
	g(0),
	instruction(EMPTY)
{
	for (char i = 0; i < 8; ++i)
	{
		this->compressed <<= 3;
		this->compressed |= i;
	}
	for (char i = 0; i < 12; ++i)
	{
		this->compressed <<= 4;
		this->compressed |= i;
	}
	tot++;
}

State::State(std::string input):
	g(0),
	instruction(EMPTY)
{
	// tokenize
	std::istringstream buf(input);
	std::istream_iterator<std::string> beg(buf), end;
	std::vector<std::string> tokens(beg, end);


	int i = 0;
	for (auto& s: tokens)
	{
		if (i >= 0 && i < 8)
		{
			this->compressed <<= 3;
			this->compressed |= atoi(s.c_str());
		}
		if (i >= 8 && i < 20)
		{
			this->compressed <<= 4;
			this->compressed |= atoi(s.c_str());
		}
		if (i >= 20 && i < 28)
		{
			this->corners_orientation <<= 2;
			this->corners_orientation |= atoi(s.c_str());
		}
		if (i >= 28 && i < 40)
		{
			this->edges_orientation <<= 1;
			this->edges_orientation |= atoi(s.c_str());
		}
		i++;
	}
	tot++;
}


State::State(State const & origin) :
	g(origin.g),
	instruction(origin.instruction),
	compressed(origin.compressed),
    corners_orientation(origin.corners_orientation),
    edges_orientation(origin.edges_orientation)
{
	tot++;
}

State::State(State const & origin, Instruction instruction) :
	g(origin.g + 1),
	instruction(instruction),
	compressed(origin.compressed),
    corners_orientation(origin.corners_orientation),
    edges_orientation(origin.edges_orientation)
{
	tot++;
}

State & State::operator=(State const & rhs)
{
	if (this != &rhs)
	{
		this->g = rhs.g;
		this->instruction = rhs.instruction;
		this->compressed = rhs.compressed;
        this->corners_orientation = rhs.corners_orientation;
        this->edges_orientation = rhs.edges_orientation;
	}
	tot++;
	return *this;
}

std::ostream & operator<<(std::ostream & o, State const & rhs)
{
	int to_print;

	o << "instruction : " << rhs.instruction << std::endl;
	o << "g: " << int(rhs.g) << std::endl;
	o << "corners position: ";
	for (auto i = 0; i < 8; i++)
	{
		to_print = 0;
		for (auto j = 2; j >= 0; j--)
		{
			to_print <<= 1;
			to_print |= rhs.compressed[48 +  j + ((7 - i)*3)];
		}
		o << to_print << " ";
	}
	o << std::endl;
	o << "edges position:   ";
	for (auto i = 0; i < 12; i++)
	{
		to_print = 0;
		for (auto j = 3; j >= 0; j--)
		{
			to_print <<= 1;
			to_print |= rhs.compressed[j + ((11 - i)*4)];
		}
		o << to_print << " ";
	}
    o << std::endl;
    o << "corners orientation: ";
    for (auto i = 0; i < 8; i++)
    {
        to_print = 0;
        to_print |= rhs.corners_orientation[1 + ((7 - i)*2)];
        to_print <<= 1;
        to_print |= rhs.corners_orientation[(7 - i)*2];
        o << to_print << " ";
    }
    o << std::endl;
    o << "edges orientation:   ";
    for (auto i = 0; i < 12; i++)
    {
        to_print = 0;
        to_print |= rhs.edges_orientation[11- i];
        o << to_print << " ";
    }
	o << std::endl;
	o << "compressed:" << std::endl;
	o << rhs.compressed << std::endl;
	o << "orientation:" << std::endl;
	o << rhs.corners_orientation << rhs.edges_orientation << std::endl;
	o << std::endl;
	return o;
}

// fix this
std::bitset<100> State::get_bitset()
{
	std::bitset<100> ret;
	for (auto i = 0; i < 16; ++i)
		ret[i] = corners_orientation[i];
	for (auto i = 0; i < 12; ++i)
		ret[i+16] = edges_orientation[i];
	for (auto i = 0; i < 72; ++i)
		ret[i+28] = compressed[i];
	return ret;
}

std::bitset<32>	State::get_full_orientation()
{
	std::bitset<32> ret;
	for (auto i = 0; i < 16; ++i)
		ret[i] = corners_orientation[i];
	for (auto i = 0; i < 12; ++i)
		ret[i+16] = edges_orientation[i];
	return ret;

}

std::bitset<16>		State::get_UD_slice_permutation(void)
{
	
	std::bitset<16> ret;
	char	index = 0;

	if (this->compressed[46] == 1) // pos 0
		index++;
	if (this->compressed[42] == 1) // pos 1
	{
		ret[(3 - index) * 4] = 1;
		index++;
	}
	if (this->compressed[38] == 1) // pos 2
	{
		ret[(3 - index) *4 + 1] = 1;
		index++;
	}
	if (this->compressed[34] == 1) // pos 3
	{
		ret[(3 - index) *4 + 1] = 1;
		ret[(3 - index) *4] = 1;
		index++;
	}
	if (this->compressed[30] == 1) // pos 4
	{
		ret[(3 - index) *4 + 2] = 1;
		index++;
	}
	if (this->compressed[26] == 1) // pos 5
	{
		ret[(3 - index) *4 + 2] = 1;
		ret[(3 - index) *4] = 1;
		index++;
	}
	if (this->compressed[22] == 1) // pos 6
	{
		ret[(3 - index) *4 + 2] = 1;
		ret[(3 - index) *4 + 1] = 1;
		index++;
	}
	if (this->compressed[18] == 1) // pos 7
	{
		ret[(3 - index) *4 + 1] = 1;
		ret[(3 - index) *4 + 2] = 1;
		ret[(3 - index) *4 + 3] = 1;
		index++;
	}
	if (this->compressed[14] == 1) // pos 8
	{
		ret[(3 - index) *4 + 3] = 1;
		index++;
	}
	if (this->compressed[10] == 1) // pos 9
	{
		ret[(3 - index) *4 + 3] = 1;
		ret[(3 - index) *4] = 1;
		index++;
	}
	if (this->compressed[6] == 1) // pos 10
	{
		ret[(3 - index) *4 + 3] = 1;
		ret[(3 - index) *4 + 1] = 1;
		index++;
	}
	if (this->compressed[2] == 1) // pos 11
	{
		ret[(3 - index) *4 + 3] = 1;
		ret[(3 - index) *4 + 1] = 1;
		ret[(3 - index) *4] = 1;
		index++;
	}

	return ret;
}
