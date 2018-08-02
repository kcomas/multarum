
// Comment

fib: (n) {
    ? {
        n = 0 | n = 1 { n }
        { $(n - 1) + $(n - 2) }
    }
}

fib(30) >> 1
