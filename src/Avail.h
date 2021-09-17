#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include "Component.h"

namespace SimpleXTree
{
	class Avail : public Component
	{
	public:
		Avail();
		~Avail();

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
		void CheckKeys();
    void CheckKeysDO(SimpleXTree::DirObject* dirObject, bool filesScreen)
    {
      CheckKeys();
    }
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
	};
}
