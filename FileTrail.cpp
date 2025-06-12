// FileTrail.cpp : Implementation of DLL Exports.


#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "FileTrail_i.h"
#include "dllmain.h"
#include <shobjidl.h>
#include <list>


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
	HRESULT STDMETHODCALLTYPE Initialize(
		_In_opt_ PCIDLIST_ABSOLUTE folder,
		_In_opt_ IDataObject* data,
		_In_opt_ HKEY prog_id
	) override
	{
		return S_OK;
	}

	// IShellPropSheetExt
	HRESULT STDMETHODCALLTYPE AddPages(
		LPFNSVADDPROPSHEETPAGE add_page_proc,
		LPARAM lparam
	) override
	{
		PROPSHEETPAGEW psp{};
		psp.dwSize = sizeof(PROPSHEETPAGEW);
		psp.dwFlags = PSP_USEREFPARENT | PSP_USETITLE;
		psp.hInstance = _AtlBaseModule.GetResourceInstance();
		psp.pszTemplate = MAKEINTRESOURCEW(IDD_DIALOG1); // 你的资源ID
		psp.pszTitle = L"File Trail";
		psp.pcRefParent = reinterpret_cast<UINT*>(&_AtlModule.m_nLockCnt);

		HPROPSHEETPAGE hPage = CreatePropertySheetPageW(&psp);
		if (hPage) {
			if (!add_page_proc(hPage, lparam)) {
				DestroyPropertySheetPage(hPage);
			}
		}
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


class DECLSPEC_UUID("1b12eb41-a8bd-42f4-8ef6-a3e1be205172") CFileTrailShlExt;

OBJECT_ENTRY_AUTO(__uuidof(CFileTrailShlExt), CFileTrailShlExt)

// Used to determine whether the DLL can be unloaded by OLE.
_Use_decl_annotations_
STDAPI DllCanUnloadNow(void)
{
	return _AtlModule.DllCanUnloadNow();
}

// Returns a class factory to create an object of the requested type.
_Use_decl_annotations_
STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID* ppv)
{
	return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

// DllRegisterServer - Adds entries to the system registry.
_Use_decl_annotations_
STDAPI DllRegisterServer(void)
{
    // 1. 调用 ATL 自动注册
    HRESULT hr = _AtlModule.DllRegisterServer();
    if (FAILED(hr))
        return hr;

    // 2. 注册到 Approved 列表（推荐，防止被系统屏蔽）
    HKEY hKey = nullptr;
    LPCWSTR szApprovedKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved";
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, szApprovedKey, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS)
    {
        const wchar_t* szClsid = L"{1B12EB41-A8BD-42F4-8EF6-A3E1BE205172}";
        const wchar_t* szDesc = L"FileTrail Shell Extension";
        RegSetValueExW(hKey, szClsid, 0, REG_SZ, (const BYTE*)szDesc, (DWORD)((wcslen(szDesc) + 1) * sizeof(wchar_t)));
        RegCloseKey(hKey);
    }

    // 3. 注册为所有文件的属性页扩展（可根据需要调整为特定类型）
    HKEY hSubKey = nullptr;
    if (RegCreateKeyExW(HKEY_CLASSES_ROOT, L"*\\shellex\\PropertySheetHandlers\\FileTrail", 0, nullptr, 0, KEY_WRITE, nullptr, &hSubKey, nullptr) == ERROR_SUCCESS)
    {
        const wchar_t* szClsid = L"{1B12EB41-A8BD-42F4-8EF6-A3E1BE205172}";
        RegSetValueExW(hSubKey, nullptr, 0, REG_SZ, (const BYTE*)szClsid, (DWORD)((wcslen(szClsid) + 1) * sizeof(wchar_t)));
        RegCloseKey(hSubKey);
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


