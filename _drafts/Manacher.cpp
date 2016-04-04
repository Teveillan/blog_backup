const int MAX_LEN = 1e5 + 10;

int Len[MAX_LEN<<1]; //记录算法完成后以i点为中心最长回文串向左向右延伸的半径
struct Manacher {
    char tmp_str[MAX_LEN<<1]; //记录预处理过后的字符串
    int init(char *s) { //在solve中就直接调用了。对原串进行预处理。
        int len = strlen(s);
        int id = 0;
        tmp_str[id++] = '#';
        for(int i=0; i<len; ++i) {
            tmp_str[id++] = s[i];
            tmp_str[id++] = '#';
        }
        tmp_str[id] = '\0';
        return id; //此时的id即为处理过后的字符串长度
    }


    void solve(char *s) { //从这开始处理
        int len = init(s);
        int max_r = -1, max_id = -1; // 最右能到得地方，以及最右时的中心点id
        for(int i=0; i<len; ++i) {
            if(max_r > i)   Len[i] = min(max_r - i, Len[2 * max_id - i]);
            else Len[i] = 1;

            while(i - Len[i] >= 0 && i + Len[i] < len && tmp_str[i - Len[i]] == tmp_str[i + Len[i]])
                ++Len[i];

            if(i + Len[i] > max_r) {
                max_r = i + Len[i];
                max_id = i;
            }
        }
    }
};
