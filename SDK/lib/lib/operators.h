/*
 * lib/operators.h
 */

#ifndef LIB_LIB_OPERATORS_H_
#define LIB_LIB_OPERATORS_H_

void* operator new(size_t sz) throw();
void operator delete(void* ptr) throw();

/*#####################################################*/
#ifdef USE_VISUAL_STUDIO
#include "operators.cpp"
#endif
/*#####################################################*/

#endif /* LIB_LIB_OPERATORS_H_ */
