#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Component.h"

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

	class OpenWith : public Component
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
		bool IsOtherActivatedAndNotBrowse() const
		{
			return IsActivated();
		}
		bool IsOtherActivated() const
		{
			return IsActivated();
		}
		void CheckKeys();

	private:
		void Run(Item const& item);
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
