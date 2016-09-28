#include "Module.h"

#include <dlfcn.h>
#include "Logger.h"

Module::Module(const std::string& path)
{
	m_path = path;
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
}

void* Module::FindSymbol(const std::string& name)
{
	void* symbol = dlsym(m_handler, name.c_str());
	if (symbol == NULL) {
		LOG_E("Failed to look up %s\n", name.c_str());
	}
	
	return symbol;
}
