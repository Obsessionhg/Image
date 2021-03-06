// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// ImageDoc.cpp : implementation of the CImageDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Image.h"
#endif

#include "ImageDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImageDoc

IMPLEMENT_DYNCREATE(CImageDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageDoc, CDocument)
END_MESSAGE_MAP()


// CImageDoc construction/destruction

CImageDoc::CImageDoc() noexcept
{
	// TODO: add one-time construction code here
	m_pImage	= NULL;
}

CImageDoc::~CImageDoc()
{
	if ( m_pImage )
		delete m_pImage;
}

BOOL CImageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CImageDoc serialization

void CImageDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CImageDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CImageDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CImageDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImageDoc diagnostics

#ifdef _DEBUG
void CImageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageDoc commands


BOOL CImageDoc::OnOpenDocument ( LPCTSTR lpszPathName )
{
	//if (!CDocument::OnOpenDocument ( lpszPathName ))
	//	return FALSE;

	// TODO:  Add your specialized creation code here
	HRESULT	hr;

	m_pImage	= new CImage;
	hr	= m_pImage->Load ( lpszPathName );

	if ( S_OK != hr )
		return	FALSE;

	theApp.m_iCnt_pix++;

	return TRUE;
}

void CImageDoc::mergePix ( CImage *pImage_1, CImage *pImage_2 )
{
	int		nWidth, nHeight, nBPP;
	HDC		hDC;

	nBPP = pImage_1->GetBPP ( );
	nWidth = pImage_1->GetWidth ( );
	nHeight = pImage_1->GetHeight ( ) + pImage_2->GetHeight ( );

	if (nWidth < pImage_2->GetWidth ( ))
	{
		nWidth = pImage_2->GetWidth ( );
	}

	m_pImage = new CImage;

	m_pImage->Create ( nWidth, nHeight, nBPP );

	hDC = m_pImage->GetDC ( );
	pImage_1->BitBlt ( hDC, 0, 0, SRCCOPY );
	pImage_2->BitBlt ( hDC, 0, pImage_1->GetHeight ( ), SRCCOPY );

	m_pImage->ReleaseDC ( );

	UpdateAllViews ( NULL );
}


BOOL CImageDoc::OnSaveDocument ( LPCTSTR lpszPathName )
{
	HRESULT	hr;
	// TODO: Add your specialized code here and/or call the base class
	hr	= m_pImage->Save ( lpszPathName, Gdiplus::ImageFormatJPEG );

	if ( S_OK == hr )
		return	TRUE;

	return	FALSE;

//	return CDocument::OnSaveDocument ( lpszPathName );
}
