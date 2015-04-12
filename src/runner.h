/*
 * The MIT License (MIT)
 *
 * Copyright © 2015 Franklin "Snaipe" Mathieu <http://snai.pe/>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef CRITERION_RUNNER_H_
# define CRITERION_RUNNER_H_

# include "criterion/types.h"

DECL_SECTION_LIMITS(struct criterion_test, criterion_tests);
DECL_SECTION_LIMITS(struct criterion_suite, crit_suites);

struct criterion_test_set *criterion_init(void);

# ifndef FOR_MSVC
#  define FOREACH_TEST_SEC(Test)                                        \
    for (struct criterion_test *Test = SECTION_START(criterion_tests);  \
            Test < SECTION_END(criterion_tests);                        \
            ++Test)

#  define FOREACH_SUITE_SEC(Suite)                                      \
    for (struct criterion_suite *Suite = SECTION_START(crit_suites);    \
            Suite < SECTION_END(crit_suites);                           \
            ++Suite)
# else
__attribute__((pure,always_inline))
inline struct criterion_test *get_tsec_start(struct criterion_test *start) {
    static struct criterion_test *ptr = NULL;
    if (!ptr)
        for (ptr = start; !ptr->category; ++ptr);
    return ptr;
}

__attribute__((pure,always_inline))
inline struct criterion_suite *get_ssec_start(struct criterion_suite *start) {
    static struct criterion_suite *ptr = NULL;
    if (!ptr)
        for (ptr = start; !ptr->name; ++ptr);
    return ptr;
}

#  define FOREACH_TEST_SEC(Test)                                        \
    for (struct criterion_test *Test                                    \
                = get_tsec_start(SECTION_START(criterion_tests));       \
            Test->category;                                             \
            ++Test)

#  define FOREACH_SUITE_SEC(Suite)                                      \
    for (struct criterion_suite *Suite                                  \
                = get_ssec_start(SECTION_START(crit_suites));           \
            Suite->name;                                                \
            ++Suite)
# endif

#endif /* !CRITERION_RUNNER_H_ */
