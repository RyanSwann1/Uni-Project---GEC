#pragma once

class Timer
{
public:
	Timer(float expirationTime, bool active = false);
	
	bool isExpired() const;

	void deactive();
	void activate();
	void update(float deltaTime);
	void reset();

private:
	float m_expirationTime;
	float m_elaspedTime;
	bool m_active;
};