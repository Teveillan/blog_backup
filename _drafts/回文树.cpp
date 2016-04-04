const int LEN = 1e4 + 10;
struct pt {
    int num[LEN], len[LEN], cnt[LEN], ch[LEN][26], fail[LEN], S[LEN];
    int tot, n, last;
    int creat(int l) {
        memset(ch[tot], 0, sizeof ch[tot]);
        cnt[tot] = num[tot] = 0;
        len[tot] = l;
        return tot++;
    }
    void init() {
        tot = last = n = 0;
        creat(0);
        creat(-1);
        S[n] = -1;
        fail[0] = 1;
    }
    int get_fail(int x) {
        while(S[n-len[x]-1] != S[n]) {
            x = fail[x];
        }
        return x;
    }
    void add(int c) {
        S[++n] = c;
        int cur = get_fail(last);
        if(ch[cur][c] == 0) {
            int now = creat(len[cur] + 2);
            fail[now] = ch[get_fail(fail[cur])][c];
            ch[cur][c] = now;
            num[now] = num[fail[now]] + 1;
        }
        last = ch[cur][c];
        cnt[last]++;
    }
    void calc() {
        for(int i = tot - 1; i >= 0; --i) cnt[fail[i]] += cnt[i];
    }
}A;
