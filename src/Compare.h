#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <Windows.h>
#include "DirObject.h"

namespace SimpleXTree
{
	class Compare
	{
	public:
		Compare();
		~Compare();

		void Activate();
		void Toggle();
		void Render(int start);
		bool IsActivated() const
		{
			return m_activated;
		}
		void CheckKeys(DirObject* dirObject);
		void KeyEvent(WCHAR ch);
		void VK(DWORD vk);
		void SelectDir(DirObject* dirObject);
	private:
		bool m_checkingForKeys;
		bool m_activated;
		bool m_lPressed;
		bool m_escPressed;
		bool m_show;
		long long m_timePassed;
		bool m_timeSet;
		bool m_renderCursor;
		int m_waitForKeyLetGo;
		bool m_lastShown;
		bool m_showAvail;
		std::wstring m_avail;
		std::wstring m_drive;
		DirObject* m_dirObject;
		DirObject* m_initial;
	public:
		DirObject* m_selected;
		std::wstring m_typed;
		bool m_step2;
	public:
		bool m_browse;
	};
}
