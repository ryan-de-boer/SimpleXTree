#pragma once
#include <string>
#include <vector>
#include <fstream>

namespace SimpleXTree
{
	class Item
	{
	public:
		Item(std::wstring const& name, std::wstring const& path, std::wstring const& args);
		std::wstring Name;
		std::wstring Path;
		std::wstring Args;
	};

	class OpenWith
	{
	public:
		OpenWith();
		~OpenWith();

		void SetPath(std::wstring const& path)
		{
			m_path = path;
		}
		void Activate();
		void Toggle();
		void Render();
		std::vector<Item> Items;
		bool IsActivated() const
		{
			return m_activated;
		}
		void CheckKeys();

	private:
		void Run(Item const& item);
		bool m_activated;
		int m_selected;
		bool m_downPressed;
		bool m_upPressed;
		bool m_escPressed;
		bool m_enterPessed;
		std::wstring m_path;
	};
}
