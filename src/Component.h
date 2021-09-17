#pragma once
namespace SimpleXTree
{
  class DirObject;
}

class Component
{
public:
	Component()
	{
		m_isOtherActivatedAndNotBrowse = false;
		m_isOtherActivated = false;
	}
	virtual bool IsOtherActivatedAndNotBrowse() const
	{
		return m_isOtherActivatedAndNotBrowse;
	}
	virtual bool IsOtherActivated() const
	{
		return m_isOtherActivated;
	}
  virtual void CheckKeysDO(SimpleXTree::DirObject* dirObject, bool filesScreen)
  {
  }
protected:
	bool m_isOtherActivatedAndNotBrowse;
	bool m_isOtherActivated;
public:
	bool m_otherActive;
};