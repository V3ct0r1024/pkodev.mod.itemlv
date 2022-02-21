#pragma once
#include "structure.h"

namespace pkodev
{
	namespace hook
	{
		// void CItemCommand::Render(int x, int y)
		void __fastcall CItemCommand__Render(void* This, void* NotUsed, int x, int y);

		// void CItemCommand::SaleRender(int x, int y, int nWidth, int nHeight)
		void __fastcall CItemCommand__SaleRender(void* This, void* NotUsed, int x, int y, int nWidth, int nHeight);
	}
}