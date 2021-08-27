#pragma once
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
protected:
	bool m_isOtherActivatedAndNotBrowse;
	bool m_isOtherActivated;
public:
	bool m_otherActive;
};