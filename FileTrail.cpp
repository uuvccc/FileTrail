// FileTrail.cpp : Implementation of DLL Exports.



#include "pch.h"
#include "framework.h"
#pragma comment(lib, "comctl32.lib")

#include "resource.h"
#include "FileTrail_i.h"
#include "dllmain.h"
#include <shobjidl.h>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <prsht.h>
//#include "wnd.h"
//#include "MainDialog.h"




//   template <typename T>
//   INT_PTR CALLBACK DlgProcClassBinder(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
//     T* p;
//     if (uMsg == WM_INITDIALOG) {
//       p = new T(hDlg, (void*)lParam);
//       SetWindowLongPtrW(hDlg, GWLP_USERDATA, (LONG_PTR)p);
//     } else {
//       p = (T*)GetWindowLongPtrW(hDlg, GWLP_USERDATA);
//     }
//     // there are some unimportant messages sent before WM_INITDIALOG
//     const INT_PTR ret = p ? p->DlgProc(uMsg, wParam, lParam) : (INT_PTR)FALSE;
//     if (uMsg == WM_NCDESTROY) {
//       delete p;
//       // even if we were to somehow receive messages after WM_NCDESTROY make sure we dont call invalid ptr
//       SetWindowLongPtrW(hDlg, GWLP_USERDATA, 0);
//     }
//     return ret;
//   }

//   namespace detail {
//     template <typename Dialog>
//     INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
//       if (uMsg == WM_INITDIALOG)
//         lParam = ((LPPROPSHEETPAGEW)lParam)->lParam;
//       return DlgProcClassBinder<Dialog>(hDlg, uMsg, wParam, lParam);
//     };

//     template <typename PropPage>
//     UINT CALLBACK Callback(HWND hwnd, UINT msg, LPPROPSHEETPAGEW ppsp) {
//       const auto object = (PropPage*)ppsp->lParam;
//       UINT ret = 1;

//       static const char* const msgname[] = {"ADDREF", "RELEASE", "CREATE"};
//       DebugMsg("%s %p object %p ppsp %p\n", msgname[msg], hwnd, object, ppsp->lParam);

//       switch (msg) {
//       case PSPCB_ADDREF:
//         object->AddRef(hwnd, ppsp);
//         break;
//       case PSPCB_RELEASE:
//         object->Release(hwnd, ppsp);
//         break;
//       case PSPCB_CREATE:
//         ret = object->Create(hwnd, ppsp);
//         break;
//       default:
//         break;
//       }

//       return ret;
//     };
//   } // namespace detail


//     template </*typename PropPage,*/ typename Dialog, typename... Args>
//   HPROPSHEETPAGE MakePropPage(PROPSHEETPAGEW psp, Args&&... args) {
//     // Things are generally called in the following order:
//     // name           dlg   when
//     // -----------------------------------------------
//     // ADDREF         no    on opening properties
//     // CREATE         no    on opening properties
//     // *WM_INITDIALOG yes   on first click on sheet
//     // *WM_*          yes   window messages
//     // *WM_NCDESTROY  yes   on closing properties
//     // RELEASE        no    after properties closed
//     //
//     // Ones marked with * won't get called when the user never selects our prop sheet page

//     //const auto object = new PropPage(std::forward<Args>(args)...);

//     psp.pfnDlgProc = &detail::DlgProc<Dialog>;
//     //psp.pfnCallback = &detail::Callback<PropPage>;

//     //psp.lParam = (LPARAM)object;

//     const auto page = CreatePropertySheetPageW(&psp);

//     //if (!page)
//     //  delete object;

//     return page;
//   }

//class CFileTrailShlExt;

// 修改对话框处理函数
// 修改对话框处理函数
static INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
static UINT CALLBACK PropSheetPageProc(HWND hwnd, UINT uMsg, LPPROPSHEETPAGEW ppsp);

// 修改 AddPages 方法中的 PROPSHEETPAGEW 初始化


using namespace ATL;

// {1B12EB41-A8BD-42F4-8EF6-A3E1BE205172}
DEFINE_GUID(CLSID_FileTrailShlExt,
	0x1b12eb41, 0xa8bd, 0x42f4, 0x8e, 0xf6, 0xa3, 0xe1, 0xbe, 0x20, 0x51, 0x72);

class ATL_NO_VTABLE CFileTrailShlExt : public CComObjectRootEx<CComSingleThreadModel>
	, public CComCoClass<CFileTrailShlExt, &CLSID_FileTrailShlExt>
	, public IShellExtInit
	, public IShellPropSheetExt
	, public IContextMenu {

//protected:
//	std::list<std::wstring> _files_raw;
//
public:
	CFileTrailShlExt() = default;
//
	// IShellExtInit
	// 在 Initialize 方法中添加文件收集逻辑
	HRESULT STDMETHODCALLTYPE Initialize(
		_In_opt_ PCIDLIST_ABSOLUTE folder,
		_In_opt_ IDataObject* data,
		_In_opt_ HKEY prog_id) override
	{
		if (!data) return E_INVALIDARG;
		
		FORMATETC fmt = { CF_HDROP, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
		STGMEDIUM stg = { TYMED_HGLOBAL };
		
		if (SUCCEEDED(data->GetData(&fmt, &stg))) {
			HDROP hDrop = static_cast<HDROP>(stg.hGlobal);
			UINT fileCount = DragQueryFileW(hDrop, 0xFFFFFFFF, nullptr, 0);
			
			wchar_t path[MAX_PATH];
			for (UINT i = 0; i < fileCount; ++i) {
				if (DragQueryFileW(hDrop, i, path, MAX_PATH)) {
					OutputDebugStringW(L"FileTrail: Selected file - ");
					OutputDebugStringW(path);
					OutputDebugStringW(L"\n");
				}
			}
			ReleaseStgMedium(&stg);
		}
		return S_OK;
	}

	// IShellPropSheetExt
	// 修改 AddPages 方法
	// 修改 AddPages 方法
	// 修改 AddPages 方法
	HRESULT STDMETHODCALLTYPE AddPages(
		LPFNSVADDPROPSHEETPAGE add_page_proc,
		LPARAM lparam) override
	{
		OutputDebugStringW(L"FileTrail: AddPages called\n");
		
		// 增加引用计数
		this->InternalAddRef();
		
		PROPSHEETPAGEW psp = {};
		psp.dwSize = sizeof(PROPSHEETPAGEW);
		psp.dwFlags = PSP_USEREFPARENT | PSP_USETITLE | PSP_USECALLBACK;
		psp.hInstance = _AtlBaseModule.GetResourceInstance();
		psp.pszTemplate = MAKEINTRESOURCEW(IDD_DIALOG1);
		psp.pszTitle = L"File Trail";
		psp.pcRefParent = reinterpret_cast<UINT*>(&_AtlModule.m_nLockCnt);
		psp.pfnDlgProc = DialogProc;
		psp.pfnCallback = PropSheetPageProc;
		psp.lParam = reinterpret_cast<LPARAM>(this); // 传递 this 指针

		HPROPSHEETPAGE hPage = CreatePropertySheetPageW(&psp);
		if (!hPage) {
			DWORD err = GetLastError();
			wchar_t buf[256];
			swprintf_s(buf, L"FileTrail: CreatePropertySheetPage failed with error %d\n", err);
			OutputDebugStringW(buf);
			this->InternalRelease();
			return E_FAIL;
		}

		if (!add_page_proc(hPage, lparam)) {
			DestroyPropertySheetPage(hPage);
			OutputDebugStringW(L"FileTrail: add_page_proc failed\n");
			this->InternalRelease();
			return E_FAIL;
		}

		OutputDebugStringW(L"FileTrail: Page added successfully\n");
		return S_OK;
	}


	HRESULT STDMETHODCALLTYPE ReplacePage(
		_In_ EXPPS page_id,
		_In_ LPFNSVADDPROPSHEETPAGE replace_with_proc,
		_In_ LPARAM lparam
	) override
	{
		UNREFERENCED_PARAMETER(page_id);
		UNREFERENCED_PARAMETER(replace_with_proc);
		UNREFERENCED_PARAMETER(lparam);
		return E_NOTIMPL; // Not implemented, as we don't need to replace any pages
	}

	// IContextMenu
	HRESULT STDMETHODCALLTYPE QueryContextMenu(
		_In_ HMENU hmenu,
		_In_ UINT indexMenu,
		_In_ UINT idCmdFirst,
		_In_ UINT idCmdLast,
		_In_ UINT uFlags
	) override
	{
		UNREFERENCED_PARAMETER(hmenu);
		UNREFERENCED_PARAMETER(indexMenu);
		UNREFERENCED_PARAMETER(idCmdFirst);
		UNREFERENCED_PARAMETER(idCmdLast);
		UNREFERENCED_PARAMETER(uFlags);
		// Here you would add your context menu items
		return S_OK; // Return S_OK if successful
	}

	HRESULT STDMETHODCALLTYPE InvokeCommand(
		_In_ CMINVOKECOMMANDINFO* pici
	) override
	{
		UNREFERENCED_PARAMETER(pici);
		// Here you would handle the command invoked from the context menu
		return S_OK; // Return S_OK if successful
	}

	HRESULT STDMETHODCALLTYPE GetCommandString(
		_In_ UINT_PTR idCmd,
		_In_ UINT uType,
		_Reserved_ UINT* pReserved,
		_Out_writes_bytes_((uType& GCS_UNICODE) ? (cchMax * sizeof(wchar_t)) : cchMax) _When_(!(uType& (GCS_VALIDATEA | GCS_VALIDATEW)), _Null_terminated_) CHAR* pszName,
		_In_ UINT cchMax
	) override
	{
		UNREFERENCED_PARAMETER(idCmd);
		UNREFERENCED_PARAMETER(uType);
		UNREFERENCED_PARAMETER(pReserved);
		UNREFERENCED_PARAMETER(pszName);
		UNREFERENCED_PARAMETER(cchMax);
		// Here you would provide a string for the command
		return E_NOTIMPL; // Not implemented, as we don't need to provide a command string
	}

	static HRESULT WINAPI UpdateRegistry(_In_ BOOL bRegister) throw() {
		UNREFERENCED_PARAMETER(bRegister);
		return S_OK;
	}

	DECLARE_NOT_AGGREGATABLE(CFileTrailShlExt)

	BEGIN_COM_MAP(CFileTrailShlExt)
		COM_INTERFACE_ENTRY(IShellExtInit)
		COM_INTERFACE_ENTRY(IShellPropSheetExt)
		COM_INTERFACE_ENTRY(IContextMenu)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct() { return S_OK; }

	//void FinalRelease();
};

static INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static CFileTrailShlExt* pThis = nullptr;
    
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        pThis = reinterpret_cast<CFileTrailShlExt*>(((LPPROPSHEETPAGEW)lParam)->lParam);
        if (pThis)
        {
            // 初始化对话框
            std::wstring fileInfo = L"Selected files information:\r\n";
            SetDlgItemTextW(hDlg, IDC_EDIT_INFO, fileInfo.c_str());
        }
        return TRUE;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
        }
        break;
    case WM_DESTROY:
        if (pThis) {
            pThis->InternalRelease();
            pThis = nullptr;
        }
        break;
    }
    return FALSE;
}

// 完善属性页回调函数
// 完善属性页回调函数
static UINT CALLBACK PropSheetPageProc(HWND hwnd, UINT uMsg, LPPROPSHEETPAGEW ppsp)
{
    switch (uMsg)
    {
    case PSPCB_CREATE:
        return TRUE; // 返回TRUE允许创建页面
    case PSPCB_RELEASE:
        // 释放COM对象引用
        if (ppsp->lParam) {
            reinterpret_cast<CFileTrailShlExt*>(ppsp->lParam)->InternalRelease();
        }
        break;
    }
    return 0;
}


class DECLSPEC_UUID("1b12eb41-a8bd-42f4-8ef6-a3e1be205172") CFileTrailShlExt;

OBJECT_ENTRY_AUTO(__uuidof(CFileTrailShlExt), CFileTrailShlExt)

// Used to determine whether the DLL can be unloaded by OLE.
_Use_decl_annotations_
// 修改 DllCanUnloadNow 函数
STDAPI DllCanUnloadNow(void)
{
    // 添加调试输出
    OutputDebugStringW(L"FileTrail: DllCanUnloadNow called\n");
    return (_AtlModule.GetLockCount() == 0) ? S_OK : S_FALSE;
}

// Returns a class factory to create an object of the requested type.
_Use_decl_annotations_
STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID* ppv)
{
	return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

// DllRegisterServer - Adds entries to the system registry.
_Use_decl_annotations_
// 修改 DllRegisterServer 函数
// 修改 DllRegisterServer 函数
STDAPI DllRegisterServer(void)
{
    // 1. 调用 ATL 自动注册
    HRESULT hr = _AtlModule.DllRegisterServer();
    if (FAILED(hr)) {
        OutputDebugStringW(L"FileTrail: ATL registration failed\n");
        return hr;
    }

    // 2. 注册到 Approved 列表
    HKEY hKey = nullptr;
    if (RegCreateKeyExW(HKEY_LOCAL_MACHINE, 
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved",
        0, nullptr, 0, KEY_WRITE, nullptr, &hKey, nullptr) == ERROR_SUCCESS)
    {
        const wchar_t* szDesc = L"FileTrail Shell Extension";
        RegSetValueExW(hKey, L"{1B12EB41-A8BD-42F4-8EF6-A3E1BE205172}", 0, REG_SZ,
            (const BYTE*)szDesc, (DWORD)((wcslen(szDesc) + 1) * sizeof(wchar_t)));
        RegCloseKey(hKey);
    }

    // 3. 注册为所有文件的属性页扩展
    const wchar_t* locations[] = {
        L"*\\shellex\\PropertySheetHandlers\\FileTrail",
        L"AllFileSystemObjects\\shellex\\PropertySheetHandlers\\FileTrail",
        L"Directory\\shellex\\PropertySheetHandlers\\FileTrail",
        L"Drive\\shellex\\PropertySheetHandlers\\FileTrail"
    };

    for (const auto& location : locations) {
        if (RegCreateKeyExW(HKEY_CLASSES_ROOT, 
            location,
            0, nullptr, 0, KEY_WRITE, nullptr, &hKey, nullptr) == ERROR_SUCCESS)
        {
            RegSetValueExW(hKey, nullptr, 0, REG_SZ, 
                (const BYTE*)L"{1B12EB41-A8BD-42F4-8EF6-A3E1BE205172}", 
                (DWORD)(38 * sizeof(wchar_t)));
            RegCloseKey(hKey);
        }
    }

    return S_OK;
}

// DllUnregisterServer - Removes entries from the system registry.
_Use_decl_annotations_
STDAPI DllUnregisterServer(void)
{
    // 1. 调用 ATL 自动卸载
    HRESULT hr = _AtlModule.DllUnregisterServer();

    // 2. 删除 Approved 注册表项
    HKEY hKey = nullptr;
    LPCWSTR szApprovedKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved";
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, szApprovedKey, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS)
    {
        RegDeleteValueW(hKey, L"{1B12EB41-A8BD-42F4-8EF6-A3E1BE205172}");
        RegCloseKey(hKey);
    }

    // 3. 删除 PropertySheetHandlers 注册表项
    RegDeleteKeyW(HKEY_CLASSES_ROOT, L"*\\shellex\\PropertySheetHandlers\\FileTrail");

    return hr;
}

// DllInstall - Adds/Removes entries to the system registry per user per machine.
STDAPI DllInstall(BOOL bInstall, _In_opt_  LPCWSTR pszCmdLine)
{
	HRESULT hr = E_FAIL;
	static const wchar_t szUserSwitch[] = L"user";

	if (pszCmdLine != nullptr)
	{
		if (_wcsnicmp(pszCmdLine, szUserSwitch, _countof(szUserSwitch)) == 0)
		{
			ATL::AtlSetPerUserRegistration(true);
		}
	}

	if (bInstall)
	{
		hr = DllRegisterServer();
		if (FAILED(hr))
		{
			DllUnregisterServer();
		}
	}
	else
	{
		hr = DllUnregisterServer();
	}

	return hr;
}


