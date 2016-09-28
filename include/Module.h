#ifndef MODULE_H
#define MODULE_H

#include <string>

class Module
{
 public:
	Module(const std::string& path);
	~Module();
 public:
	void Load();
	void Free();
	void* FindSymbol(const std::string& name);
 private:
	std::string           m_path;
	void*                 m_handler;
};

#endif /* MODULE_H */
