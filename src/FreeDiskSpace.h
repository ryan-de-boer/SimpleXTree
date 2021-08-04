#pragma once
#include <string>
#include <vector>
#include <fstream>

namespace SimpleXTree
{
	class FreeDiskSpace
	{
	public:
		static std::wstring GetFreeDiskSpace(std::wstring const& drive);
	};
}