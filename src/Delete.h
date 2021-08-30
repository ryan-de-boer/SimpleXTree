#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <Windows.h>
#include "DirObject.h"
#include "Component.h"

namespace SimpleXTree
{
	/*
	*/
	class Delete : public Component
	{
	public:
		Delete();
		~Delete();

		void Activate();
		void Toggle();
		void Render(int start);
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
		void CheckKeys(DirObject* dirObject);
		void KeyEvent(WCHAR ch);
		void VK(DWORD vk);
		void SelectDir(DirObject* dirObject);
	private:
		std::wstring YesNo(bool value);
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
		bool m_yesNo;
	public:
//		bool m_browse;
	private:
		bool m_identical;
		bool m_unique;
		bool m_newer;
		bool m_older;
		bool m_binary;
		bool m_subs;
    bool m_useRecycleBin;
	public:
//		bool m_otherActive;
	};
}
