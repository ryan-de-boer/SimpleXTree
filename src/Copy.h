#pragma once
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <chrono>
#include "DirObject.h"
#include "Component.h"

namespace SimpleXTree
{
	class Copy : public Component
	{
	public:
		Copy();
		~Copy();

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
		void CheckKeys(DirObject* dirObject, bool filesScreen);
		void KeyEvent(WCHAR ch);

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
		std::wstring m_typed;
		std::map<std::wstring, long long> m_timePressed;
		std::map<int, bool> m_keyPressed;
	};
}
