vector<int> z_algorithm(const string& s) {
    vector<int> prefix(s.size());
    for (int i = 1, j = 0; i < s.size(); i++) {
        if (i + prefix[i - j] < j + prefix[j]) {
            prefix[i] = prefix[i - j];
        } else {
            int k = max(0LL, j + prefix[j] - i);
            while (i + k < s.size() && s[k] == s[i + k]) ++k;
            prefix[i] = k;
            j = i;
        }
    }
    prefix[0] = (int) s.size();
    return prefix;
}


struct RollingHash {
    vector<int64_t> hash;
    vector<int64_t> base_pow;
    int64_t base, mod;
    const int64_t DEFAULT_BASE = 1009, DEFAULT_MOD = 1e9 + 7;

    RollingHash(string S = "", int64_t base_ = -1, int64_t mod_ = -1) {
        base = base_ > 0 ? base_ : DEFAULT_BASE;
        mod = mod_ > 0 ? mod_ : DEFAULT_MOD;
        if (S.size() > 0) initialize(S);
    }

    void initialize(string& S) {
        int N = S.size();
        vector<int> s(N);
        for (int i = 0; i < N; i++) s[i] = S[i];
        initialize(s);
    }

    void initialize(vector<int>& S) {
        hash.resize(S.size() + 1);
        base_pow.resize(S.size() + 1);
        hash[0] = 0;
        base_pow[0] = 1;
        for (int i = 1; i <= S.size(); i++) {
            hash[i] = (hash[i - 1] * base + S[i - 1]) % mod;
            base_pow[i] = base_pow[i - 1] * base % mod;
        }
    }

    // 0-indexed, [a, b)
    int64_t between(int a, int b) {
        return (mod + hash[b] - hash[a] * base_pow[b - a] % mod) % mod;
    }
};
