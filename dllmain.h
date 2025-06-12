// dllmain.h : Declaration of module class.

class CFileTrailModule : public ATL::CAtlDllModuleT< CFileTrailModule >
{
public :
	DECLARE_LIBID(LIBID_FileTrailLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_FILETRAIL, "{2ff28472-41da-4aeb-9995-f933d8b85d62}")
};

extern class CFileTrailModule _AtlModule;
