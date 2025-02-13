# Static and Dynamic Links

## Dynamic Link: The Caller's Bookmark

The dynamic link functions as a biological memory for the call stack. When function A calls function B:

1. B's frame stores A's frame address as its dynamic link
2. During B's execution, this link remains dormant
3. When B completes, the VM follows the dynamic link to:  
   - Restore A's stack frame as current  
   - Resume execution at the instruction following B's call  
   - Pass B's return value back to A's context

This mechanism enables seamless nesting of function calls to arbitrary depth while maintaining perfect recall of the call chain.

## Static Link: The Lexical Chain of Being

The static link implements lexical scoping through a persistent connection to the function's birth environment. Consider:

```argonaut
proc outer() {
    var x := 10;
    proc inner() {
        print("%d\n", x); // Accesses outer's x via static link
    }
}
```

When `inner` is called:

1. `inner`'s static link points to `outer`'s frame
2. Variable lookup for `x` traverses the static link chain
3. The lookup stops at the first frame containing `x`

This chain remains fixed regardless of where `inner` is called from, preserving lexical scoping rules.
