#ifndef MODULEMACROS_H
#define MODULEMACROS_H

#define MODULE_INIT extern "C" int __module_init(void* ARGS)
#define MODULE_EXIT extern "C" int __module_exit(void* ARGS)

#endif /* MODULEMACROS_H */
