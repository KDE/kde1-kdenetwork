#include"Trace.h"

int Trace::traceIndent = 0;
int Trace::tabStop = 2;
bool Trace::traceOn = true;

Trace::Trace(const string& function_name)
{
	if( traceOn )
	{
		m_functionName = function_name;

		m_indent();
		cout << "Entering " << m_functionName << endl;
		traceIndent++;
	}
}

Trace::~Trace()
{
	if( traceOn )
	{
		traceIndent--;
		m_indent();
		cout << "Leaving " << m_functionName << endl;
		m_functionName = "";
	}
}

void Trace::outputf(const string& message ...)
{
	if( traceOn )
	{
		va_list arglist;

		va_start(arglist, message.c_str());

		char buffer[512];
		vsprintf(buffer, message.c_str(), arglist );

		va_end(arglist);

		m_indent();
		cout << m_functionName << " : " << buffer << endl;
	}
}

void Trace::output(const string& message)
{
	if( traceOn )
		outputf(message);
}

void Trace::output(const string& message, void* object )
{
	if( traceOn )
	{
		m_indent();
		cout << m_functionName << " : " << message << (void*)object << endl;
	}
}

void Trace::setTraceOn(const bool trace_on)
{
	traceOn = trace_on;
}

void Trace::setTabStop(const int tabstop)
{
	if( tabstop >= 0 )
		tabStop = tabstop;
	else
		tabStop = 2;
}

void Trace::m_indent()
{
	for(int i = 0; i < traceIndent; i++ )
	{
		for( int j = 0; j < tabStop; j++ )
			cout << " ";
	}
}
