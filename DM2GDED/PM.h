
#pragma once

#define PM_V_RELOAD 0x01
#define PM_V_SEL_TREE_ITEM 0x02
#define PM_V_SET_OPT 0x03
#define PM_V_UPDATE_TREE_ITEM 0x04
#define PM_V_UPDATE_ENTRY 0x05
#define PM_V_REFRESH 0x06

#define PM_V_SIIV_SCENE_UPDATED 0x07

// wParam : (BOOL)fActivate
// lParam : (UINT)nID
// LRESULT: (BOOL)fOk
#define PM_WM_ACTIVATE_VW (WM_APP +0x0100)
// wParam : x
// lParam : y
// LRESULT: -
#define PM_WM_CLICK_PIXEL (WM_APP +0x0101)
