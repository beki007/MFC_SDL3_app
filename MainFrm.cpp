
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication2.h"

#include "MainFrm.h"
#include <SDL3/SDL.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame() noexcept
{
	m_bLeftMouseButtonPressed = false;
	m_pSDLWindow = nullptr;
}

CMainFrame::~CMainFrame()
{
	// Clean up SDL window and quit SDL
	if (m_pSDLWindow != nullptr)
	{
		SDL_DestroyWindow(m_pSDLWindow);
		m_pSDLWindow = nullptr;
	}
	SDL_Quit();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(nullptr, nullptr, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, nullptr))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// Handle left mouse button related commands
	if (m_bLeftMouseButtonPressed)
	{
		// Process command when left mouse button is pressed
		// You can add custom handling here based on nID and nCode
		if (nCode == CN_COMMAND || nCode == CN_UPDATE_COMMAND_UI)
		{
			// Custom handling for commands when left mouse button is pressed
			// For example, you might want to modify certain command behaviors
		}
	}

	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::CreateSDLWindow()
{
	// Create SDL window on left mouse click
	if (m_pSDLWindow == nullptr)
	{
		// Initialize SDL
		if (!SDL_Init(SDL_INIT_VIDEO))
		{
			TRACE("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
			return;
		}

		// Create SDL3 window
		m_pSDLWindow = SDL_CreateWindow(
			"SDL Window",                              // window title
			800,                                       // width, in pixels
			600,                                       // height, in pixels
			SDL_WINDOW_RESIZABLE                      // flags
		);

		if (m_pSDLWindow == nullptr)
		{
			TRACE("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			SDL_Quit();
		}
		else
		{
			TRACE("SDL window created successfully!\n");
		}
	}
}

void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLeftMouseButtonPressed = true;
	CreateSDLWindow();
	CFrameWnd::OnLButtonDown(nFlags, point);
}

void CMainFrame::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bLeftMouseButtonPressed = false;
	CFrameWnd::OnLButtonUp(nFlags, point);
}

