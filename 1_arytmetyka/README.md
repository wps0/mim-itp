## Problem: Arytmetyka
(C)
### Problem statement
Where we measure physical quantities, the results are subject to some error, e.g. 5m ± 10%. We treat each such approximation as a set of possible values. Implement a package of arithmetic operations on such approximate values conforming to the attached ary.h interface. In particular, the package is to contain:

    a definition of type struct value,

    constructors:

        value_approximation(x, p) returns x ± p% (for p > 0),

        value_from_to(x, y) returns (x+y)/2 ± (y-x)/2 (for x < y),

        value_to(x) returns x ± 0

    selectors:

        in_value(x, y) ⇔ value of x can be equal to y,

        min_value(x) = the lower bound of the possible values of x (or -∞ if the possible values of x are not bounded from below),

        max_value(x) = upper bound of the possible values of x (or ∞ if the possible values of x are not bounded from above),

        sr_value(x) = average (arithmetic) of min_value(x) and max_value(x) (or nan if min_value(x) and max_value(x) are not finite),

    modifiers:

        plus(a, b) = { x + y : in_value(a, x) ∧ in_value(b, y) },

        minus(a, b) = { x - y : in_value(a, x) ∧ in_value(b, y) },

        times(a, b) = { x - y : in_value(a, x) ∧ in_value(b, y) },

        divide(a,b) = {x/y : in_value(a, x) ∧ in_value(b, y) }.

In doing so, we implicitly assume that all arguments of type double are real numbers (i.e. are different from HUGE_VAL, -HUGE_VAL and NAN).
On the other hand, if the result is not a real number, it should be the appropriate one of the values: HUGE_VAL, -HUGE_VAL or NAN.

In solving this task, you can adopt the following rules to facilitate reasoning:

    Assume that modifiers enclose the resulting sets of values - that is, if the result is an open interval, assume that it is converted to a closed interval. 

    Operations on approximations are monotonic due to the containment of sets of possible values.
    That is, if the approximations x, y and z satisfy, as sets of possible values, x ⊆ y, then:

    plus(x, z) ⊆ plus(y, z)

    plus(z, x) ⊆ plus(z, y)

    and similarly for other arithmetic operations.

You can find some examples based on the above rule in the file example.c. Compile command (ary.c and ary.h files must be in the directory):
    gcc @options example.c ary.c -o example.e -lm
    The -lm option appends math.h. It must necessarily be at the end of the compilation command.

Floating-point numbers and operations on them can be surprising. For example, the IEEE standard provides two representations of zero (+0.0 and -0.0), with 1.0 /. 0.0 = HUGE_VAL, and 1.0 /. (-0.0) = -HUGE_VAL. 
    This can be helpful, for example, if you are dividing by an approximation that contains a one-sided neighborhood of zero.
    But it can also prove to be a pitfall when you are considering dividing by a value exactly equal to zero.
    Remember that in the definition of the divide operation there is a 'mathematical' division, which is not specified when dividing by zero. 
    You can assume that a number of type double is equal to zero if the following function outputs true for it:
    bool iszero(double x) { return fabs(x) < 1e-10; }

Translated with www.DeepL.com/Translator (free version)