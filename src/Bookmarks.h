#pragma once
#include <string>
#include <vector>
#include <fstream>

namespace SimpleXTree
{
	class BItem
	{
	public:
		BItem(std::wstring const& name, std::wstring const& path);
		std::wstring Name;
		std::wstring Path;
	};

	class Bookmarks
	{
	public:
		Bookmarks();
		~Bookmarks();

		void SetPath(std::wstring const& path)
		{
			m_path = path;
		}
		void Activate();
		void Toggle();
		void Render();
		std::vector<BItem> Items;
		bool IsActivated() const
		{
			return m_activated;
		}
		void CheckKeys();

	private:
		void GoToL(BItem const& item);
		bool m_activated;
		int m_selected;
		bool m_downPressed;
		bool m_upPressed;
		bool m_escPressed;
		bool m_enterPessed;
		std::wstring m_path;
	};
}
