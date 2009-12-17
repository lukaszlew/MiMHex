#ifndef CONDITIONAL_ASSERT_H_
#define CONDITIONAL_ASSERT_H_

#ifdef INSERT_ASSERTIONS
#include <cassert>
#define ASSERT(x) assert ((x))
#else
#define ASSERT(x)
#endif

#endif /* CONDITIONAL_ASSERT_H_ */
