#pragma once

#ifndef STR
# define STR(...) #__VA_ARGS__
#endif

#ifndef PRIM_CONCAT
# define PRIM_CONCAT(X,Y) X ## Y
#endif

#ifndef CONCAT
# define CONCAT(X,Y) PRIM_CONCAT(X,Y)
#endif

#ifndef EMPTY
# define EMPTY(...)
#endif

#ifndef DEFER
# define DEFER(...) __VA_ARGS__ EMPTY()
#endif

#ifndef OBSTRUCT
# define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()
#endif

#ifndef EXPAND
# define EXPAND(...) __VA_ARGS__
#endif

#ifndef NARG

//#define NARG_SEQ_N(...) 0, 1, 2, 3, 4
# define NARG_N(_0,_1,_2,_3,_4, N, ...) N
//#define NARG_IMPL(...) NARG_N(__VA_ARGS__)
# define NARG(...) EXPAND(NARG_N(__VA_ARGS__, 4, 3, 2, 1, 0))

#endif // !defined(NARG)

