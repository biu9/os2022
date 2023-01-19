
void puti(int x) {
    // unimplemented
    int reverse = 0;
    while(x > 0) {
        reverse += x%10;
        x /= 10;
        reverse *= 10;
    }
    reverse /= 10;
    while(reverse > 0) {
        int tmp = reverse % 10;
        sbi_ecall(0x1, 0x0, tmp+48, 0, 0, 0, 0, 0);
        reverse /= 10;
    }
}