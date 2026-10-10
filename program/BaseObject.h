#pragma once
class BaseObject
{
protected:
	VECTOR m_position = VGet(0.0f, 0.0f, 0.0f);
	bool m_isActive = true;
public:
	BaseObject() = default;
	virtual ~BaseObject() = default;

	virtual void  Initialize() = 0;
	virtual void  Update()	   = 0;
	virtual void  Draw()	   = 0;

	void SetPosition(const VECTOR& pos) { m_position = pos; }
	VECTOR GetPosition() const { return m_position;}
	bool IsActive() const { return m_isActive; }
};