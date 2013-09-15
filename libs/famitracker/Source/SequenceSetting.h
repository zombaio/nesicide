/*
** FamiTracker - NES/Famicom sound tracker
** Copyright (C) 2005-2012  Jonathan Liss
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful, 
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
** Library General Public License for more details.  To obtain a 
** copy of the GNU Library General Public License, write to the Free 
** Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Any permitted reproduction of these routines, in whole or in part,
** must bear this legend.
*/

#pragma once

#include "cqtmfc.h"

class CSequence;

class CSequenceSetting : public CWnd
{
   Q_OBJECT
   // Qt interfaces
public slots:
   void menuAction_triggered(int id);

public:
   DECLARE_DYNAMIC(CSequenceSetting)
   // Qt stuff
protected:
   void paintEvent(QPaintEvent *);
   void mousePressEvent(QMouseEvent *event);
   
public:
   CSequenceSetting(CWnd *pParent);
	virtual ~CSequenceSetting();
public:
	void Setup(CFont *pFont);
	void SelectSequence(CSequence *pSequence, int Type, int InstrumentType);

private:
	CWnd *m_pParent;
	CMenu m_menuPopup;
	CFont *m_pFont;
	CSequence *m_pSequence;
	int m_iInstType;
	int m_iType;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuArpAbsolute();
	afx_msg void OnMenuArpRelative();
	afx_msg void OnMenuArpFixed();
};
