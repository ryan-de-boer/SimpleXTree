#pragma once
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <fstream>
#include <chrono>
#include <thread>
#include "DirObject.h"
#include "Component.h"

namespace SimpleXTree
{
  class CopyItem
  {
  public:
    CopyItem() {}
    ~CopyItem() {}

    void DoCopy();

    std::wstring Name;
    std::wstring Path;
    std::wstring DestPath;
    long long SizeInBytes;
  };

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
			return IsActivated() && !m_browse;
		}
		bool IsOtherActivated() const
		{
			return IsActivated();
		}
		void CheckKeys(DirObject* dirObject, bool filesScreen);
		void CheckKeysDO(SimpleXTree::DirObject* dirObject, bool filesScreen)
		{
			CheckKeys(dirObject, filesScreen);
		}
		void KeyEvent(WCHAR ch);
		void VK(DWORD vk);
		void SelectDir(DirObject* dirObject);
	private:
		bool m_checkingForKeys;
		bool m_activated;
		bool m_lPressed;
		bool m_escPressed;
	public:
		bool m_enterPressed;
		int m_numEnterPress;
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
		std::wstring m_typed;
	public:
    std::wstring m_typed2;
	std::wstring m_destinationFolder;
  private:
    std::map<std::wstring, long long> m_timePressed;
		std::map<int, bool> m_keyPressed;
    double m_percent;
    double m_timeSecondsLeft;
    long long m_bytesLeft;
    bool m_calculating;
    int m_numLeft;
    int m_numItems;
    std::wstring m_from;
    std::wstring m_to;

    void ThreadFn();
    std::thread m_member_thread;
    bool m_exitThread;
    std::vector<CopyItem> m_copyItems;
    bool m_threadReadyToCopy;
    std::wstring m_currentName;
    void StartCopy();
    bool m_selectStep;
    bool m_toStep;
	bool m_createDirStep;
	public:
	bool m_copyStep;
	private:
    std::wstring m_fileSpec;

	public:
		DirObject* m_selected;
		bool m_browse;
    bool m_refreshDest;
	};
}
