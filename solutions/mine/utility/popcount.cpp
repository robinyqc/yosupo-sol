// reference: https://nimrod.blog/posts/algorithms-behind-popcount/

int popcount2(unsigned int n) {
    n = n - ((n >>1) & 0x55555555);
    n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
    n = (n + (n >> 4)) & 0x0F0F0F0F;
    return (n * 0x01010101) >> 24;
}

int popcountll(unsigned long long n) {
    n = n - ((n >> 1) & 0x5555555555555555);
    n = (n & 0x3333333333333333) + ((n >> 2) & 0x3333333333333333);
    n = (n + (n >> 4)) & 0xF0F0F0F0F0F0F0F;
    return (n * 0x101010101010101) >> 56;
}