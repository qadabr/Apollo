#ifndef MODULE_H
#define MODULE_H

#include <string>
#include <dlfcn.h>

#include "Setting.h"
#include "Logger.h"

typedef int (*api_command)(void*);

class Module
{
 public:
	Module(const std::string& path);
	~Module();
 public:
	void Load();
	void Free();
	void* FindSymbol(const std::string& name);
	int ExecuteCommand(const std::string& command, void* arguments);
	int Start();
	int Stop();
	int LoadAPI();
 private:
	std::string           m_path;
	void*                 m_handler;
	api_command*          m_api;
	size_t                m_apiSize;
};

#endif /* MODULE_H */
