/*
* Copyright (c) 2003-2013 Rony Shapiro <ronys@users.sourceforge.net>.
* All rights reserved. Use of the code is allowed under the
* Artistic License 2.0 terms, as specified in the LICENSE file
* distributed with this code, or available from
* http://www.opensource.org/licenses/artistic-license-2.0.php
*/
// SHCTHotKey.cpp : implementation file
//

#include "stdafx.h"
#include "SHCTHotKey.h"
#include "SHCTListCtrl.h"

#include "resource.h"

// SHCTHotKey

IMPLEMENT_DYNAMIC(CSHCTHotKey, CHotKeyCtrl)

CSHCTHotKey::CSHCTHotKey()
: m_pParent(NULL), m_bHandled(false)
{
}

CSHCTHotKey::~CSHCTHotKey()
{
}

BEGIN_MESSAGE_MAP(CSHCTHotKey, CHotKeyCtrl)
  ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

// SHCTHotKey message handlers

void CSHCTHotKey::OnKillFocus(CWnd *pWnd)
{
  UNREFERENCED_PARAMETER(pWnd);
  if (m_pParent != NULL) {
    WORD wVK, wMod;
    GetHotKey(wVK, wMod);
    m_pParent->OnHotKeyKillFocus(wVK, wMod);
  }
}

BOOL CSHCTHotKey::PreTranslateMessage(MSG* pMsg)
{
  if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP) {
    const UINT_PTR nChar = pMsg->wParam;
    if ((nChar == VK_RETURN && 
            (GetKeyState(VK_CONTROL) < 0 || GetKeyState(VK_MENU) < 0)) ||
        (nChar == VK_TAB &&
            (GetKeyState(VK_CONTROL) < 0 || GetKeyState(VK_MENU) < 0)) ||
        (nChar == VK_NUMLOCK &&
            (GetKeyState(VK_CONTROL) < 0 || GetKeyState(VK_MENU) < 0)) ||
        (nChar == VK_SCROLL &&
            (GetKeyState(VK_CONTROL) < 0 || GetKeyState(VK_MENU) < 0)) ||
        (nChar == VK_CANCEL &&
            (GetKeyState(VK_CONTROL) < 0 || GetKeyState(VK_MENU) < 0)) ||
        (nChar == VK_DELETE) ||
        (nChar == VK_SPACE)  ||
        (nChar == VK_BACK)) {
    if (pMsg->message == WM_KEYUP)
      return TRUE;

      WORD wVK, wMod;
      GetHotKey(wVK, wMod);

      // Enter sets the Hotkey unless user did Ctrl+Enter, or
      // Alt+Enter or Ctrl+Alt+Enter, in which is taken as a HotKey
      if (nChar == VK_RETURN && 
          ((wMod & HOTKEYF_CONTROL) != HOTKEYF_CONTROL &&
           (wMod & HOTKEYF_ALT    ) != HOTKEYF_ALT    )) {
        CHotKeyCtrl::PreTranslateMessage(pMsg);
      }

      // No current key means don't want to use the current modifiers.
      if (wVK != 0)
        wMod = 0;

      // Set extended to make sure we get DEL and not NUM DECIMAL
      if (nChar == VK_DELETE)
        wMod |= HOTKEYF_EXT;

      wVK = (WORD)nChar;
      SetHotKey(wVK, wMod);

      m_pParent->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_SHORTCUTHOTKEY, EN_CHANGE), 0);
      return TRUE;
    }
  }
  return CHotKeyCtrl::PreTranslateMessage(pMsg);
}
