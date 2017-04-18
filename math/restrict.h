#ifndef RESTRICT_H
#define RESTRICT_H

#if __STDC_VERSION__ >= 199901L
#define RESTRICT restrict
#else
#define RESTRICT __restrict
#endif

#endif /* RESTRICT_H */
