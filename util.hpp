#ifndef UTIL_HPP

#define UTIL_HPP

#define LINE_LENGTH 200

bool startsWith(const char *str, const char *prefix);

typedef int (*t_get_line)(char*, void*);

#endif
