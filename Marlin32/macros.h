
// Remove compiler warning on an unused variable
#define UNUSED(x) (void) (x)

// Macros to make a string from a macro
#define STRINGIFY_(M) #M
#define STRINGIFY(M) STRINGIFY_(M)

// Macros for bit masks
#define TEST(n,b) (((n)&_BV(b))!=0)
#ifndef MACROS_H
#define MACROS_H

#define SBI(n,b) (n |= _BV(b))
#define CBI(n,b) (n &= ~_BV(b))
#define SET_BIT(n,b,value) (n) ^= ((-value)^(n)) & (_BV(b))

// Macros for maths shortcuts
#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif
#define RADIANS(d) ((d)*M_PI/180.0)
#define DEGREES(r) ((r)*180.0/M_PI)
#define HYPOT2(x,y) (sq(x)+sq(y))
#define HYPOT(x,y) sqrt(HYPOT2(x,y))

// Macros to contrain values
#define NOLESS(v,n) do{ if (v < n) v = n; }while(0)
#define NOMORE(v,n) do{ if (v > n) v = n; }while(0)

// Macros to support option testing
#define _CAT(a, ...) a ## __VA_ARGS__
#define SWITCH_ENABLED_false 0
#define SWITCH_ENABLED_true  1
#define SWITCH_ENABLED_0     0
#define SWITCH_ENABLED_1     1
#define SWITCH_ENABLED_      1
#define ENABLED(b) _CAT(SWITCH_ENABLED_, b)
#define DISABLED(b) (!_CAT(SWITCH_ENABLED_, b))

#define WITHIN(V,L,H) ((V) >= (L) && (V) <= (H))
#define NUMERIC(a) WITHIN(a, '0', '9')
#define DECIMAL(a) (NUMERIC(a) || a == '.')
#define NUMERIC_SIGNED(a) (NUMERIC(a) || (a) == '-' || (a) == '+')
#define DECIMAL_SIGNED(a) (DECIMAL(a) || (a) == '-' || (a) == '+')
#define COUNT(a) (sizeof(a)/sizeof(*a))
#define ZERO(a) memset(a,0,sizeof(a))
#define COPY(a,b) memcpy(a,b,min(sizeof(a),sizeof(b)))

#define PIN_EXISTS(PN) (defined(PN ##_PIN) && PN ##_PIN >= 0)

#endif // __MACROS_H
