/*
 * demangle.h
 *
 *  Created on: 13.03.2015
 *      Author: Klemens
 */

#ifndef UTIL_DEMANGLE_H_
#define UTIL_DEMANGLE_H_

#include <cxxabi.h>
#include <memory>

namespace util {

std::shared_ptr<char> demangle(const char *abiName)
{
  int status;
  char *ret = abi::__cxa_demangle(abiName, 0, 0, &status);

  /* NOTE: must free() the returned char when done with it! */
  std::shared_ptr<char> retval;
  retval.reset( (char *)ret, [](char *mem) { if (mem) free((void*)mem); } );
  return retval;
};

}
#endif /* UTIL_DEMANGLE_H_ */
