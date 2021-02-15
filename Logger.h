#pragma once
#include "Singleton.h"
#include <array>
#include <chrono>

enum LogLevel
{
	Info = 0x1,
	Warning = 0x2,
	Error = 0x4,
	FixMe = 0x8
};

class Logger : public Singleton<Logger>
{

public:
	virtual ~Logger();
	Logger(const Logger &) = delete;
	Logger& operator=(const Logger& ) = delete;

	void Initialize();
	int StartPerformanceTimer();
	double StopPerformanceTimer(int timerId);

	void Log(LogLevel level, const std::wstring& msg, bool includeTimeStamp = false);
	void LogFormat(LogLevel level, const wchar_t* format, ...);
	void LogInfo(const std::wstring& msg, bool includeTimeStamp = false);
	void LogWarning(const std::wstring& msg, bool includeTimeStamp = false);
	void LogError(const std::wstring& msg, bool includeTimeStamp = false);
	void LogFixMe(const std::wstring& source, bool includeTimeStamp = false);
	bool LogHResult(HRESULT hr, const std::wstring& origin, bool includeTimeStamp = false);
	void StartFileLogging(const std::wstring& fileName);
	void StopFileLogging();

	void BreakOnLog(LogLevel level, bool doBreak) { doBreak ? m_BreakBitField |= level : m_BreakBitField &= ~level; }
	virtual void Log(const std::wstring& message) ;

protected:
	Logger();
	friend Singleton<Logger>;

private:

	std::wostream* m_os;
	std::wofstream* m_FileOutStream;	
	
	//TIMER 
	double m_PcFreq;
	static const int MAX_PERFORMANCE_TIMERS = 10;
	__int64 m_PerformanceTimerArr[MAX_PERFORMANCE_TIMERS];
	std::chrono::high_resolution_clock::time_point m_TimePoints[MAX_PERFORMANCE_TIMERS];

	char m_BreakBitField;

	static const size_t m_ConvertBufferSize{ 1024 };
	std::array<wchar_t, m_ConvertBufferSize> m_ConvertBuffer;
	
	HANDLE m_ConsoleHandle;

	friend class Game;
};
