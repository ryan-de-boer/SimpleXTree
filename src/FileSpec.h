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
	class FileSpec : public Component
	{
	public:
		FileSpec();
		~FileSpec();

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
    void CheckKeysDO(SimpleXTree::DirObject* dirObject, bool filesScreen)
    {
      CheckKeys(dirObject);
    }
		void KeyEvent(WCHAR ch);
		void VK(DWORD vk);
		void SelectDir(DirObject* dirObject);
	private:
		bool m_checkingForKeys;
	public:
		bool m_activated;
		bool m_lPressed;
		bool m_escPressed;
	private:
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
		std::wstring m_typedInProg;
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
