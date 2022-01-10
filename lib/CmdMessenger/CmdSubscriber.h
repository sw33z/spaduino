#pragma once

#include <stdint.h>

class CmdSubscriber
{
	public:
		CmdSubscriber() = default;
		virtual ~CmdSubscriber() = default;

		virtual void onData(uint8_t cmdID) = 0;
		virtual void onUnhandledData(uint8_t /*cmdID*/){};
};