#include "BindsList.h"
#include "../CFG.h"
#include "../Menu/Menu.h"
#include "../VisualUtils/VisualUtils.h"

#define LIST_WIDTH CFG::BindsList_Width 

void CBindsList::Drag()
{
    const int nMouseX = H::Input->GetMouseX();
    const int nMouseY = H::Input->GetMouseY();

    static bool bDragging = false;

    if (!bDragging && F::Menu->IsMenuWindowHovered())
        return;

    static int nDeltaX = 0;
    static int nDeltaY = 0;

    const int nListX = CFG::BindsList_Pos_X;  
    const int nListY = CFG::BindsList_Pos_Y; 

    const bool bHovered = nMouseX > nListX && nMouseX < nListX + LIST_WIDTH && nMouseY > nListY && nMouseY < nListY + CFG::Menu_Drag_Bar_Height;

    // Start dragging
    if (bHovered && H::Input->IsPressed(VK_LBUTTON))
    {
        nDeltaX = nMouseX - nListX;
        nDeltaY = nMouseY - nListY;
        bDragging = true;
    }

    // Stop dragging
    if (!H::Input->IsPressed(VK_LBUTTON) && !H::Input->IsHeld(VK_LBUTTON))
        bDragging = false;

    // Update the location
    if (bDragging)
    {
        CFG::BindsList_Pos_X = nMouseX - nDeltaX;  
        CFG::BindsList_Pos_Y = nMouseY - nDeltaY; 
    }
}

void CBindsList::Run()
{
    // Anti screenshot?
    if (CFG::Misc_Clean_Screenshot && I::EngineClient->IsTakingScreenshot())
    {
        return;
    }

    // In menu?
    if (!F::Menu->IsOpen() && (I::EngineVGui->IsGameUIVisible() || SDKUtils::BInEndOfMatch()))
        return;

    if (F::Menu->IsOpen())
        Drag();

    // Check if the binds list is active
    if (!CFG::BindsList_Active)
        return;

    const auto outlineColor = F::VisualUtils->GetAlphaColor(CFG::Menu_Accent_Secondary, CFG::BindsList_Outline_Alpha);
    const auto bgColor = F::VisualUtils->GetAlphaColor(CFG::Menu_Background, CFG::BindsList_Background_Alpha);


    H::Draw->Rect(
        CFG::BindsList_Pos_X,
        CFG::BindsList_Pos_Y,
        CFG::BindsList_Width, 
        CFG::Menu_Drag_Bar_Height,
        bgColor
    );


    H::Draw->String(
        H::Fonts->Get(EFonts::Menu),
        CFG::BindsList_Pos_X + (CFG::BindsList_Width / 2),
        CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height / 2),
        CFG::Menu_Text,
        POS_CENTERXY,
        "Binds List"
    );


    H::Draw->OutlinedRect(
        CFG::BindsList_Pos_X,
        CFG::BindsList_Pos_Y,
        CFG::BindsList_Width, 
        CFG::Menu_Drag_Bar_Height,
        outlineColor
    );

    int iPos = 1; 

    if (H::Input->IsDown(CFG::Aimbot_Key))
    {
       
        H::Draw->Rect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width, 
            CFG::Menu_Drag_Bar_Height + 1,
            bgColor
        );

  
        H::Draw->String(
            H::Fonts->Get(EFonts::Menu),
            CFG::BindsList_Pos_X + (CFG::BindsList_Width / 2),  
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) + (CFG::Menu_Drag_Bar_Height / 2),
            CFG::Menu_Text_Inactive,
            POS_CENTERXY,
            "Aimbot"
        );

       
        H::Draw->OutlinedRect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            outlineColor
        );

   
        iPos++;
    }

 
    if (H::Input->IsDown(CFG::Triggerbot_Key)) 
    {
       
        H::Draw->Rect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width, 
            CFG::Menu_Drag_Bar_Height + 1,
            bgColor
        );

        H::Draw->String(
            H::Fonts->Get(EFonts::Menu),
            CFG::BindsList_Pos_X + (CFG::BindsList_Width / 2),  
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) + (CFG::Menu_Drag_Bar_Height / 2),
            CFG::Menu_Text_Inactive,
            POS_CENTERXY,
            "Triggerbot"
        );

        H::Draw->OutlinedRect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            outlineColor
        );

        iPos++;
    }


    if (H::Input->IsDown(CFG::Exploits_Crits_Force_Crit_Key) || H::Input->IsDown(CFG::Exploits_Crits_Force_Crit_Key_Melee)) 
    {
       
        H::Draw->Rect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width, 
            CFG::Menu_Drag_Bar_Height + 1,
            bgColor
        );


        H::Draw->String(
            H::Fonts->Get(EFonts::Menu),
            CFG::BindsList_Pos_X + (CFG::BindsList_Width / 2),  
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) + (CFG::Menu_Drag_Bar_Height / 2),
            CFG::Menu_Text_Inactive,
            POS_CENTERXY,
            "Crithack"
        );

  
        H::Draw->OutlinedRect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            outlineColor
        );

        
        iPos++;
    }

    if (H::Input->IsDown(CFG::Visuals_Thirdperson_Key))
    {
       
        H::Draw->Rect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width, 
            CFG::Menu_Drag_Bar_Height + 1,
            bgColor
        );

  
        H::Draw->String(
            H::Fonts->Get(EFonts::Menu),
            CFG::BindsList_Pos_X + (CFG::BindsList_Width / 2),  
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) + (CFG::Menu_Drag_Bar_Height / 2),
            CFG::Menu_Text_Inactive,
            POS_CENTERXY,
            "Thirdperson"
        );

       
        H::Draw->OutlinedRect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            outlineColor
        );

   
        iPos++;
    }

    if (H::Input->IsDown(CFG::SequenceFreezeKey))
    {

        H::Draw->Rect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            bgColor
        );


        H::Draw->String(
            H::Fonts->Get(EFonts::Menu),
            CFG::BindsList_Pos_X + (CFG::BindsList_Width / 2),
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) + (CFG::Menu_Drag_Bar_Height / 2),
            CFG::Menu_Text_Inactive,
            POS_CENTERXY,
            "Desync"
        );


        H::Draw->OutlinedRect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            outlineColor
        );


        iPos++;
    }


    if (H::Input->IsDown(CFG::Visuals_Paint_Key))
    {
       
        H::Draw->Rect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width, 
            CFG::Menu_Drag_Bar_Height + 1,
            bgColor
        );

  
        H::Draw->String(
            H::Fonts->Get(EFonts::Menu),
            CFG::BindsList_Pos_X + (CFG::BindsList_Width / 2),  
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) + (CFG::Menu_Drag_Bar_Height / 2),
            CFG::Menu_Text_Inactive,
            POS_CENTERXY,
            "Paint"
        );

       
        H::Draw->OutlinedRect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            outlineColor
        );

   
        iPos++;
    }

    if (H::Input->IsDown(CFG::Visuals_Paint_Erase_Key))
    {
       
        H::Draw->Rect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width, 
            CFG::Menu_Drag_Bar_Height + 1,
            bgColor
        );

  
        H::Draw->String(
            H::Fonts->Get(EFonts::Menu),
            CFG::BindsList_Pos_X + (CFG::BindsList_Width / 2),  
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) + (CFG::Menu_Drag_Bar_Height / 2),
            CFG::Menu_Text_Inactive,
            POS_CENTERXY,
            "Paint Erase"
        );

       
        H::Draw->OutlinedRect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            outlineColor
        );

   
        iPos++;
    }

    if (H::Input->IsDown(CFG::Misc_Taunt_Spin_Key))
    {
       
        H::Draw->Rect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width, 
            CFG::Menu_Drag_Bar_Height + 1,
            bgColor
        );

  
        H::Draw->String(
            H::Fonts->Get(EFonts::Menu),
            CFG::BindsList_Pos_X + (CFG::BindsList_Width / 2),  
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) + (CFG::Menu_Drag_Bar_Height / 2),
            CFG::Menu_Text_Inactive,
            POS_CENTERXY,
            "Taunt Spin"
        );

       
        H::Draw->OutlinedRect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            outlineColor
        );

   
        iPos++;
    }

 

    if (H::Input->IsDown(CFG::Misc_Edge_Jump_Key))
    {
       
        H::Draw->Rect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width, 
            CFG::Menu_Drag_Bar_Height + 1,
            bgColor
        );

  
        H::Draw->String(
            H::Fonts->Get(EFonts::Menu),
            CFG::BindsList_Pos_X + (CFG::BindsList_Width / 2),  
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) + (CFG::Menu_Drag_Bar_Height / 2),
            CFG::Menu_Text_Inactive,
            POS_CENTERXY,
            "Edge Jump"
        );

       
        H::Draw->OutlinedRect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            outlineColor
        );

   
        iPos++;
    }

    if (H::Input->IsDown(CFG::Misc_MVM_Instant_Respawn_Key))
    {
       
        H::Draw->Rect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width, 
            CFG::Menu_Drag_Bar_Height + 1,
            bgColor
        );

  
        H::Draw->String(
            H::Fonts->Get(EFonts::Menu),
            CFG::BindsList_Pos_X + (CFG::BindsList_Width / 2),  
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) + (CFG::Menu_Drag_Bar_Height / 2),
            CFG::Menu_Text_Inactive,
            POS_CENTERXY,
            "MvM Instant Respawn"
        );

       
        H::Draw->OutlinedRect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            outlineColor
        );

   
        iPos++;
    }

    if (H::Input->IsDown(CFG::FakeDuck_Key))
    {

        H::Draw->Rect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            bgColor
        );


        H::Draw->String(
            H::Fonts->Get(EFonts::Menu),
            CFG::BindsList_Pos_X + (CFG::BindsList_Width / 2),
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) + (CFG::Menu_Drag_Bar_Height / 2),
            CFG::Menu_Text_Inactive,
            POS_CENTERXY,
            "Fake Duck"
        );


        H::Draw->OutlinedRect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            outlineColor
        );


        iPos++;
    }

    if (H::Input->IsDown(CFG::Misc_Auto_Medigun_Key))
    {
       
        H::Draw->Rect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width, 
            CFG::Menu_Drag_Bar_Height + 1,
            bgColor
        );

  
        H::Draw->String(
            H::Fonts->Get(EFonts::Menu),
            CFG::BindsList_Pos_X + (CFG::BindsList_Width / 2),  
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) + (CFG::Menu_Drag_Bar_Height / 2),
            CFG::Menu_Text_Inactive,
            POS_CENTERXY,
            "Auto Medigun"
        );

       
        H::Draw->OutlinedRect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            outlineColor
        );

   
        iPos++;
    }

    if (H::Input->IsDown(CFG::AutoPeek_Key))
    {

        H::Draw->Rect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            bgColor
        );


        H::Draw->String(
            H::Fonts->Get(EFonts::Menu),
            CFG::BindsList_Pos_X + (CFG::BindsList_Width / 2),
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) + (CFG::Menu_Drag_Bar_Height / 2),
            CFG::Menu_Text_Inactive,
            POS_CENTERXY,
            "Auto Peek"
        );


        H::Draw->OutlinedRect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            outlineColor
        );


        iPos++;
    }

    if (H::Input->IsDown(CFG::Misc_Movement_Lock_Key))
    {
       
        H::Draw->Rect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width, 
            CFG::Menu_Drag_Bar_Height + 1,
            bgColor
        );

  
        H::Draw->String(
            H::Fonts->Get(EFonts::Menu),
            CFG::BindsList_Pos_X + (CFG::BindsList_Width / 2),  
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) + (CFG::Menu_Drag_Bar_Height / 2),
            CFG::Menu_Text_Inactive,
            POS_CENTERXY,
            "Movement Lock"
        );

       
        H::Draw->OutlinedRect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            outlineColor
        );

   
        iPos++;
    }

    if (H::Input->IsDown(CFG::Misc_Auto_Rocket_Jump_Key))
    {
       
        H::Draw->Rect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width, 
            CFG::Menu_Drag_Bar_Height + 1,
            bgColor
        );

  
        H::Draw->String(
            H::Fonts->Get(EFonts::Menu),
            CFG::BindsList_Pos_X + (CFG::BindsList_Width / 2),  
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) + (CFG::Menu_Drag_Bar_Height / 2),
            CFG::Menu_Text_Inactive,
            POS_CENTERXY,
            "Auto Rocket Jump"
        );

       
        H::Draw->OutlinedRect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            outlineColor
        );

   
        iPos++;
    }


    if (H::Input->IsDown(CFG::CircleStraferKey))
    {

        H::Draw->Rect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            bgColor
        );


        H::Draw->String(
            H::Fonts->Get(EFonts::Menu),
            CFG::BindsList_Pos_X + (CFG::BindsList_Width / 2),
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) + (CFG::Menu_Drag_Bar_Height / 2),
            CFG::Menu_Text_Inactive,
            POS_CENTERXY,
            "Circle Strafer"
        );


        H::Draw->OutlinedRect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            outlineColor
        );


        iPos++;
    }

    if (H::Input->IsDown(CFG::Misc_Auto_Air_Pogo_Key))
    {
       
        H::Draw->Rect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width, 
            CFG::Menu_Drag_Bar_Height + 1,
            bgColor
        );

  
        H::Draw->String(
            H::Fonts->Get(EFonts::Menu),
            CFG::BindsList_Pos_X + (CFG::BindsList_Width / 2),  
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) + (CFG::Menu_Drag_Bar_Height / 2),
            CFG::Menu_Text_Inactive,
            POS_CENTERXY,
            "Auto Pogo"
        );

       
        H::Draw->OutlinedRect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            outlineColor
        );

   
        iPos++;
    }

    if (H::Input->IsDown(CFG::Exploits_RapidFire_Key))
    {
       
        H::Draw->Rect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width, 
            CFG::Menu_Drag_Bar_Height + 1,
            bgColor
        );

  
        H::Draw->String(
            H::Fonts->Get(EFonts::Menu),
            CFG::BindsList_Pos_X + (CFG::BindsList_Width / 2),  
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) + (CFG::Menu_Drag_Bar_Height / 2),
            CFG::Menu_Text_Inactive,
            POS_CENTERXY,
            "Doubletap"
        );

       
        H::Draw->OutlinedRect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            outlineColor
        );

   
        iPos++;
    }

    if (H::Input->IsDown(CFG::Exploits_Shifting_Recharge_Key))
    {
       
        H::Draw->Rect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width, 
            CFG::Menu_Drag_Bar_Height + 1,
            bgColor
        );

  
        H::Draw->String(
            H::Fonts->Get(EFonts::Menu),
            CFG::BindsList_Pos_X + (CFG::BindsList_Width / 2),  
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) + (CFG::Menu_Drag_Bar_Height / 2),
            CFG::Menu_Text_Inactive,
            POS_CENTERXY,
            "Recharge"
        );

       
        H::Draw->OutlinedRect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            outlineColor
        );

   
        iPos++;
    }

    if (H::Input->IsDown(CFG::Exploits_Warp_Key))
    {
       
        H::Draw->Rect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width, 
            CFG::Menu_Drag_Bar_Height + 1,
            bgColor
        );

  
        H::Draw->String(
            H::Fonts->Get(EFonts::Menu),
            CFG::BindsList_Pos_X + (CFG::BindsList_Width / 2),  
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos) + (CFG::Menu_Drag_Bar_Height / 2),
            CFG::Menu_Text_Inactive,
            POS_CENTERXY,
            "Warp"
        );

       
        H::Draw->OutlinedRect(
            CFG::BindsList_Pos_X,
            CFG::BindsList_Pos_Y + (CFG::Menu_Drag_Bar_Height * iPos),
            CFG::BindsList_Width,
            CFG::Menu_Drag_Bar_Height + 1,
            outlineColor
        );

   
        iPos++;
    }

    // Continue adding more binds in the same manner, ensuring to place iPos increment inside the respective if block.
}
