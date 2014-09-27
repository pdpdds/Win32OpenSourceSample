#pragma once

class BreakPad
{
public:
	BreakPad(void);
	virtual ~BreakPad(void);

	bool Install();

protected:

private:
	void* m_pBreakPadHandler;
};