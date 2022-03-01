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
  class Search
  {
  public:
    Search();
    ~Search();
    void StartSearch(std::wstring const& theSearchHex);
    void StartSearch(std::wstring const& theSearchHex, __int64 startBeforeSearch);
    bool Searching() const;
    void Render();
    void KeyEvent(WCHAR ch);
    bool IsActivated() const
    {
      return m_activated;
    }
    void VK(DWORD vk);
  private:
    void ThreadFn();
    void Search3(std::wstring theSearchHex);
    std::thread m_member_thread;
    bool m_exitThread;
    bool m_threadReadyToSearch;
    std::wstring m_theSearchHex;

  public:
    bool m_editing;
    bool m_activated;
    __int64 m_startBeforeSearch;
    int m_numFoundBeforeSearch;
    __int64 m_theSearchPosBeforeSearch;
  };
}