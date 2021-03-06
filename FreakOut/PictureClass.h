#pragma once
#include <windows.h>
#include <atlbase.h>

//////////////////
// Picture object--encapsulates IPicture
//
class CPicture 
{

public:
	CPicture();
	~CPicture();

	// Load frm various sosurces
	BOOL Load(HINSTANCE hInst, UINT nIDRes);
	BOOL Load(LPCTSTR pszPathName);
	BOOL Load(IStream* pstm);

	// render to device context
	BOOL Render(HDC dc, RECT* rc, LPCRECT prcMFBounds=NULL) const;

	SIZE GetImageSize(HDC dc=NULL) const;

	operator IPicture*() {
		return m_spIPicture;
	}

	void GetHIMETRICSize(OLE_XSIZE_HIMETRIC& cx, OLE_YSIZE_HIMETRIC& cy) const {
		cx = cy = 0;
		const_cast<CPicture*>(this)->m_hr = m_spIPicture->get_Width(&cx);
		const_cast<CPicture*>(this)->m_hr = m_spIPicture->get_Height(&cy);
	}

	void Free() {
		if (m_spIPicture) {
			m_spIPicture.Release();
		}
	}

protected:

	void SetHIMETRICtoDP(HDC hdc, SIZE* sz) const;

	CComQIPtr<IPicture>m_spIPicture;  // ATL smart pointer to IPicture
	HRESULT m_hr;  // last error code
	BOOL m_isLoaded;
};
