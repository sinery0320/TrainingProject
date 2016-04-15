// dllmain.h : Declaration of module class.

class CSignalGeneratorModule : public ATL::CAtlDllModuleT< CSignalGeneratorModule >
{
public :
	DECLARE_LIBID(LIBID_SignalGeneratorLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SIGNALGENERATOR, "{5464B8EE-0377-4410-897B-AE69D7EC86EA}")
};

extern class CSignalGeneratorModule _AtlModule;
