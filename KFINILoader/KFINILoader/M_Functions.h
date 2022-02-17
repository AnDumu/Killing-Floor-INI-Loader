#pragma once

#include <iostream>
#include <Windows.h>

class SystemKF
{
public:
	enum eFunctions
	{
		_begin,
		None,

		fGetPerObjectNames,

		_end
	};

	SystemKF()
	{

	}

	~SystemKF()
	{

	}

	DWORD ProcessFunctions(eFunctions Function);
};

extern SystemKF MiserySKF;