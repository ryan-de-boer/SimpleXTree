#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Component.h"

namespace SimpleXTree
{
	class BItem
	{
	public:
		BItem(std::wstring const& name, std::wstring const& path);
		std::wstring Name;
		std::wstring Path;
	};

	class Bookmarks : public Component
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
		bool IsOtherActivatedAndNotBrowse() const
		{
			return IsActivated();
		}
		bool IsOtherActivated() const
		{
			return IsActivated();
		}
		void CheckKeys();

    void CheckKeysDO(SimpleXTree::DirObject* dirObject, bool filesScreen)
    {
      CheckKeys();
    }

	private:
		void GoToL(BItem const& item);
		bool m_activated;
		int m_selected;
		bool m_downPressed;
		bool m_upPressed;
		bool m_escPressed;
  public:
		bool m_enterPessed;
  private:
		std::wstring m_path;
	};
}
