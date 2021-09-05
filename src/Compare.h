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

  @CompareTree
  c           Compares this directory with another. The compare options
  are:
  o Identical, tags the files with the same name and file
  time.
  o Unique, tags the files which are not in the second
  list.
  o Newer, tags the files with the same name and a newer
  file time.
  o Older, tags the files with the same name and a older
  file time.
  o Binary, tags the files that are the same size and
  contain exactly the same content.
  o Subs, preforms the comparison operation into
  subdirectories aswell.

    */
	class Compare: public Component
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
	    bool IsOtherActivatedAndNotBrowse() const
		{
			return IsActivated() && !m_browse;
		}
		bool IsOtherActivated() const
		{
			return IsActivated();
		}
		void CheckKeys(DirObject* dirObject, bool filesScreen);
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
		bool m_step2;
	public:
		bool m_browse;
  private:
    bool m_identical;
    bool m_unique;
    bool m_newer;
    bool m_older;
    bool m_binary;
    bool m_subs;
  public:

	};
}
