struct pt {
	const int LEN = 1e4 + 100;
    int num[LEN], len[LEN], cnt[LEN], ch[LEN][26], fail[LEN], S[LEN];
    int tot, len, last;

    int creat(int l) {
        memset(ch[tot], 0, sizeof ch[tot]);
        cnt[tot] = num[tot] = 0;
        len[tot] = l;
        return tot++;
    }
    void init() {
        tot = 0, last = 0, len = 0, S[len] = -1, fail[0] = 1;
        creat(0);
        creat(-1);
    }
    int get_fail(int x) {
        while(S[len-len[x]-1] != S[len]) {
            x = fail[x];
        }
        return x;
    }
    void add(int c) {
        S[++len] = c;
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
