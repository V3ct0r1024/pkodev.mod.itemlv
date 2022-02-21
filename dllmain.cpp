#include <Windows.h>
#include <detours.h>

#include <cstdio>

#include "loader.h"

#include "address.h"
#include "pointer.h"
#include "hook.h"
#include "structure.h"


namespace pkodev
{
    // Colors of smart icons
    enum class color : unsigned int
    {
        none  = 0x00FFFFFF,   // Transparent color
        gem   = 0xFFB7D1F8,   // Gem level
        fairy = 0xFFB7D1F8,   // Fairy level
        forge = 0xFFF9B8B8,   // Forge level
        app   = 0xFFB8F9D7    // Apparel
    };

    // Icon size in pixels
    const unsigned int icon_size{ 32 };

    // Render the smart icon
    void render_smart_icon(void* item_command, int x, int y);
}

// Entry point
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    // Nothing to do . . .
    return TRUE;
}

// Get mod information
void GetModInformation(mod_info& info)
{
    strcpy_s(info.name, TOSTRING(MOD_NAME));
    strcpy_s(info.version, TOSTRING(MOD_VERSION));
    strcpy_s(info.author, TOSTRING(MOD_AUTHOR));
    info.exe_version = MOD_EXE_VERSION;
}

// Start the mod
void Start(const char* path)
{
    // Enable hooks
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)pkodev::pointer::CItemCommand__Render, pkodev::hook::CItemCommand__Render);
    DetourAttach(&(PVOID&)pkodev::pointer::CItemCommand__SaleRender, pkodev::hook::CItemCommand__SaleRender);
    DetourTransactionCommit();
}

// Stop the mod
void Stop()
{
    // Disable hooks
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)pkodev::pointer::CItemCommand__Render, pkodev::hook::CItemCommand__Render);
    DetourDetach(&(PVOID&)pkodev::pointer::CItemCommand__SaleRender, pkodev::hook::CItemCommand__SaleRender);
    DetourTransactionCommit();
}

// void CItemCommand::Render(int x, int y)
void __fastcall pkodev::hook::CItemCommand__Render(void* This, void* NotUsed, int x, int y)
{
    // Call the original method void CItemCommand::Render(int x, int y)
    pkodev::pointer::CItemCommand__Render(This, x, y);

    // Render the smat icon
    pkodev::render_smart_icon(This, x, y);
}

// void CItemCommand::SaleRender(int x, int y, int nWidth, int nHeight)
void __fastcall pkodev::hook::CItemCommand__SaleRender(void* This, void* NotUsed, int x, int y,
    int nWidth, int nHeight)
{
    // Call the original method void CItemCommand::SaleRender(int x, int y, int nWidth, int nHeight)
    pkodev::pointer::CItemCommand__SaleRender(This, x, y, nWidth, nHeight);

    // Render the smat icon
    pkodev::render_smart_icon(This, (x + (nWidth - pkodev::icon_size) / 2), (y + (nHeight - pkodev::icon_size) / 2));
}

// Render the smart icon
void pkodev::render_smart_icon(void* item_command, int x, int y)
{
    // Read unsigned short int from memory
    auto read_uint16 = [](void* from, unsigned int offset) -> unsigned short int
    {
        return *reinterpret_cast<unsigned short int*>(reinterpret_cast<unsigned int>(from) + offset);
    };
    
    // Read unsigned int from memory
    auto read_uint32 = [](void* from, unsigned int offset) -> unsigned int
    {
        return *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned int>(from) + offset);
    };

    // Buffer for the label
    static char buf[16]{ 0x00 };

    // The label background color
    pkodev::color color = pkodev::color::none;

    // Do we need to render the smart icon?
    bool render = false;

    // Check that there is one item in the slot
    if ( read_uint16(item_command, 0x3E) > 1 )
    {
        // Exit the function . . .
        return;
    }

    // Get the item data from ItemInfo.txt
    void* record = pkodev::pointer::GetItemRecordInfo( read_uint16(item_command, 0x3C) );

    // Check that item is found
    if ( record == nullptr )
    {
        // Exit the function . . .
        return;
    }

    // Get the item type
    unsigned short int type = read_uint16(record, 0x0134);


    // Check that the item is a gem
    if ( (type == 49) || (type == 50) )
    {
        // Build the label
        sprintf_s(buf, sizeof(buf), "Lv%d", read_uint16(item_command, 0x46));
        color = pkodev::color::gem;
        render = true;
    }

    // Check that the item is a fairy
    if ( (render == false) && (type == 59) )
    {
        // Calculate fairy's level
        unsigned int level = ( read_uint16(item_command, 0x64) +  read_uint16(item_command, 0x5C) +
            read_uint16(item_command, 0x58) + read_uint16(item_command, 0x60) + read_uint16(item_command, 0x54) );

        // Build the label
        sprintf_s(buf, sizeof(buf), "Lv%d", level);
        color = pkodev::color::fairy;
        render = true;
    }

    // Check that the item has forge
    if ( render == false )
    {
        // Check the item type
        bool isWeapon = (type >= 1 && type <= 10);
        bool isDefenceType = (type == 11 || type == 22 || type == 27);
        bool isJewelery = (type == 25 || type == 26 || type == 81 || type == 82 || type == 83);
        bool isEquip = (type == 20 || type == 23 || type == 24 || type == 88 || type == 84);

        if ( (isWeapon == true) || (isDefenceType == true) || (isJewelery == true) || (isEquip == true) )
        {
            // Get forge parameter
            unsigned int param = read_uint32(item_command, 0x4A);

            // Check the parameter
            if (param > 0)
            {
                // Get forge level from the forge parameter
                unsigned int level = read_uint32(pkodev::pointer::SItemForge__Convert(param, -1), 0x08);
 
                // Check the level
                if (level > 0)
                {
                    // Build the label
                    sprintf_s(buf, sizeof(buf), "+%d", level);
                    color = pkodev::color::forge;
                    render = true;
                }
            }
        }
    }

    // Check that the item is the apparel
    if ((render == false) && (read_uint16(item_command, 0x42) == 20000) && (read_uint16(item_command, 0x46) == 0))
    {
        // Build the label
        sprintf_s(buf, sizeof(buf), "App");
        color = pkodev::color::app;
        render = true;
    }

    // Check that we need to render the smart icon
    if (render == true)
    {
        // Get the label text size
        tagSIZE size{ 0x00 };
        pkodev::pointer::CMPFont__GetTextSize
        (
            reinterpret_cast<void*>(
                *reinterpret_cast<unsigned int*>(
                    pkodev::address::MOD_EXE_VERSION::CMPFont
                )
            ),
            buf,
            &size,
            1.0f
        );

        // Calculate the label position
        x = pkodev::icon_size - size.cx + x;
        y = pkodev::icon_size - size.cy + y;

        // Fill label background with color
        pkodev::pointer::UIRender__FillFrame(
            reinterpret_cast<void*>(pkodev::address::MOD_EXE_VERSION::UIRender),
            x,
            y,
            x + size.cx,
            y + size.cy,
            static_cast<unsigned int>(color)
        );

        // Render the label
        pkodev::pointer::CGuiFont_Render(
            reinterpret_cast<void*>(pkodev::address::MOD_EXE_VERSION::CGuiFont),
            buf,
            x,
            y,
            0xFF000000
        );
    }
}