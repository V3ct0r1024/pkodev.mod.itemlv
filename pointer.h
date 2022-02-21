#pragma once
#include "address.h"
#include "structure.h"

namespace pkodev
{
	namespace pointer
	{
		// void CItemCommand::Render(int x, int y)
		typedef void(__thiscall* CItemCommand__Render__Ptr)(void*, int, int);
		CItemCommand__Render__Ptr CItemCommand__Render = (CItemCommand__Render__Ptr)(void*)(address::MOD_EXE_VERSION::CItemCommand__Render);

		// void CItemCommand::SaleRender(int x, int y, int nWidth, int nHeight)
		typedef void(__thiscall* CItemCommand__SaleRender__Ptr)(void*, int, int, int, int);
		CItemCommand__SaleRender__Ptr CItemCommand__SaleRender = (CItemCommand__SaleRender__Ptr)(void*)(address::MOD_EXE_VERSION::CItemCommand__SaleRender);

		// inline CItemRecord* GetItemRecordInfo( int nTypeID )
		typedef void* (__cdecl* GetItemRecordInfo__Ptr)(int);
		GetItemRecordInfo__Ptr GetItemRecordInfo = (GetItemRecordInfo__Ptr)(void*)(address::MOD_EXE_VERSION::GetItemRecordInfo);

		// SItemForge& SItemForge::Convert(DWORD v, int nItemID)
		typedef void*(__cdecl* SItemForge__Convert__Ptr)(unsigned int, int);
		SItemForge__Convert__Ptr SItemForge__Convert = (SItemForge__Convert__Ptr)(void*)(address::MOD_EXE_VERSION::SItemForge__Convert);

		// void CGuiFont::Render( const char* str, int x, int y, DWORD color,float size )
		typedef void(__thiscall* CGuiFont_Render_Ptr)(void*, const char*, int, int, unsigned int);
		CGuiFont_Render_Ptr CGuiFont_Render = (CGuiFont_Render_Ptr)(void*)(address::MOD_EXE_VERSION::CGuiFont__Render);

		// SIZE* CMPFont::GetTextSize(char* szText, SIZE* pSize, float fScale = 1.0f);
		typedef tagSIZE* (__thiscall* CMPFont__GetTextSize__Ptr)(void*, char*, tagSIZE*, float);
		CMPFont__GetTextSize__Ptr CMPFont__GetTextSize = reinterpret_cast<CMPFont__GetTextSize__Ptr>(*reinterpret_cast<unsigned int*>(address::MOD_EXE_VERSION::CMPFont__GetTextSize));

		// void UIRender::FillFrame(int left, int top, int right, int bottom, DWORD color=0x90000000)
		typedef void(__thiscall* UIRender__FillFrame__Ptr)(void*, int, int, int, int, unsigned int);
		UIRender__FillFrame__Ptr UIRender__FillFrame = (UIRender__FillFrame__Ptr)(void*)(address::MOD_EXE_VERSION::UIRender__FillFrame);
	}
}