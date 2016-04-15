// dllmain.h: 模块类的声明。

class CSignalGeneratorModule : public ATL::CAtlDllModuleT< CSignalGeneratorModule >
{
public :
	DECLARE_LIBID(LIBID_SignalGeneratorLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SIGNALGENERATOR, "{94C82023-185A-4006-AFD2-46B445A6C69C}")
};

extern class CSignalGeneratorModule _AtlModule;
