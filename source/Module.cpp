#include "Module.h"

Module::Module(const std::string& path)
{
	m_path = MODULES_DIR + path;
}

Module::~Module()
{
	this->Free();
}

void Module::Load()
{
	m_handler = dlopen(m_path.c_str(), RTLD_LAZY);
	if (m_handler == NULL) {
		LOG_E("Failed to open %s because %s\n",
		      m_path.c_str(), dlerror());
	}
	else {
		LOG_D("Module %s has been successfully loaded\n",
		      m_path.c_str());
	}
}

void Module::Free()
{
	dlclose(m_handler);
	LOG_D("Module %s has been successfully closed\n",
	      m_path.c_str());

	if (m_api != NULL) {
		delete[] m_api;
	}
}

void* Module::FindSymbol(const std::string& name)
{
	void* symbol = dlsym(m_handler, name.c_str());
	if (symbol == NULL) {
		LOG_E("Failed to look up %s\n", name.c_str());
	}
	
	return symbol;
}

int Module::ExecuteCommand(const std::string& command, void* arguments)
{
	api_command func;

	func = (typeof func)FindSymbol(command);
	if (func == NULL) {
		return -1;
	}

	return func(arguments);
}

int Module::Start()
{
	int result = ExecuteCommand("__module_init", NULL);
	if (result != 0) {
		LOG_E("Failed to start module %s, error code %d\n",
		      m_path.c_str(), result);
	}

	return result;
}

int Module::Stop()
{
	int result = ExecuteCommand("__module_exit", NULL);
	if (result != 0) {
		LOG_E("Failed to stop module %s, error code %d\n",
		      m_path.c_str(), result);
	}

	return result;
}
