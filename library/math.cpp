
int gcd(int a, int b) { return b ? gcd(b, a % b) : a; }

int lcm(int a, int b) { return a * (b / gcd(a, b)); }

// 素因数
vector<int> getPrimeFactors(int a) {
    vector<int> s;
    for (int i = 2; i * i <= a; i++) {
        if (a % i != 0) continue;
        while (a % i == 0) {
            s.push_back(i);
            a /= i;
        }
    }
    if (a > 1) s.push_back(a);
    return s;
}

// 素因数
set<int> getPrimeFactors(int a) {
    set<int> s;
    for (int i = 2; i * i <= a; i++) {
        if (a % i != 0) continue;
        s.insert(i);
        while (a % i == 0) a /= i;
    }
    if (a > 1) s.insert(a);
    return s;
}

vector<int> getPrimeFactors(int a) {
    vector<int> s;
    for (int i = 2; i * i <= a; i++) {
        if (a % i != 0) continue;
        s.push_back(i);
        while (a % i == 0) a /= i;
    }
    if (a > 1) s.push_back(a);
    return s;
}

// 約数列挙
vector<int> get(int n) {
    vector<int> ret;
    for (int i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            ret.push_back(i);
            if (i != 1 && i * i != n) {
                ret.push_back(n / i);
            }
        }
    }
    return ret;
}

// 高速素因数分解
template<typename T>
vector<T> smallest_prime_factors(T n) {
    vector<T> spf(n + 1);
    for (int i = 0; i <= n; i++) spf[i] = i;
    for (T i = 2; i * i <= n; i++) {
        if (spf[i] == i) {
            for (T j = i * i; j <= n; j += i) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }

    return spf;
}

template<typename T>
set<T> factolization(T x, vector<T>& spf) {
    set<T> ret;
    while (x != 1) {
        ret.insert(spf[x]);
        x /= spf[x];
    }
    return ret;
}


// mod
const int mod = 998244353;

struct mint {
    unsigned x;

    mint() : x(0) {}

    mint(signed sig) { x = sig < 0 ? sig % mod + mod : sig % mod; }

    mint(signed long long sig) { x = sig < 0 ? sig % mod + mod : sig % mod; }

    int get() const { return (int) x; }

    mint& operator+=(mint that) {
        if ((x += that.x) >= mod) x -= mod;
        return *this;
    }

    mint& operator-=(mint that) {
        if ((x += mod - that.x) >= mod) x -= mod;
        return *this;
    }

    mint& operator*=(mint that) {
        x = (unsigned long long) x * that.x % mod;
        return *this;
    }

    mint& operator/=(mint that) { return *this *= that.inverse(); }

    mint operator+(mint that) const { return mint(*this) += that; }

    mint operator-(mint that) const { return mint(*this) -= that; }

    mint operator*(mint that) const { return mint(*this) *= that; }

    mint operator/(mint that) const { return mint(*this) /= that; }

    mint inverse() const {
        long long a = x, b = mod, u = 1, v = 0;
        while (b) {
            long long t = a / b;
            a -= t * b;
            std::swap(a, b);
            u -= t * v;
            std::swap(u, v);
        }
        return mint(u);
    }

    bool operator==(mint that) const { return x == that.x; }

    bool operator!=(mint that) const { return x != that.x; }

    mint operator-() const {
        mint t;
        t.x = x == 0 ? 0 : mod - x;
        return t;
    }
};

istream& operator>>(istream& i, mint& a) {
    i >> a.x;
    return i;
}

ostream& operator<<(ostream& o, const mint& a) {
    o << a.x;
    return o;
}

struct combination {
    vector<mint> fact, ifact;

    combination(int n) : fact(n + 1), ifact(n + 1) {
        fact[0] = 1;
        for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i;
        ifact[n] = fact[n].inverse();
        for (int i = n; i >= 1; --i) ifact[i - 1] = ifact[i] * i;
    }

    mint operator()(int n, int k) {
        if (k < 0 || k > n) return 0;
        return fact[n] * ifact[k] * ifact[n - k];
    }
};

struct permutation {
    vector<mint> fact, ifact;

    permutation(int n) : fact(n + 1), ifact(n + 1) {
        fact[0] = 1;
        for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i;
        ifact[n] = fact[n].inverse();
        for (int i = n; i >= 1; --i) ifact[i - 1] = ifact[i] * i;
    }

    mint operator()(int n, int k) {
        if (k < 0 || k > n) return 0;
        return fact[n] * ifact[n - k];
    }
};

int binpow(int x, int p) {
    if (p == 0) return 1;

    if (p % 2 == 0)
        return binpow((x * x) % mod, p / 2);
    else
        return (x * binpow(x, p - 1)) % mod;
}

// 逆元
int modinv(int a, int m) {
    int b = m, u = 1, v = 0;
    while (b) {
        int t = a / b;
        a -= t * b;
        swap(a, b);
        u -= t * v;
        swap(u, v);
    }
    u %= m;
    if (u < 0) u += m;
    return u;
}


//　行列累乗

typedef vector<int> Vec;
typedef vector <Vec> Mat;


int add(int x, int y) { return (x += y) >= mod ? x - mod : x; }

int mul(int x, int y) { return 1LL * x * y % mod; }

Vec mulMatVec(Mat a, Vec b) {
    int n = b.size();
    Vec ret(n, 0);
    REP(i, n)
    REP(j, n)
    ret[i] = add(ret[i], mul(a[i][j], b[j]));
    return ret;
}

Vec mulVecMat(Vec a, Mat b) {
    int n = a.size();
    Vec ret(n, 0);
    REP(i, n)
    REP(j, n)
    ret[i] = add(ret[i], mul(a[j], b[j][i]));
    return ret;
}

Mat mulMatMat(Mat a, Mat b) {
    int n = a.size();
    Mat ret(n, Vec(n, 0));
    REP(i, n)
    REP(j, n)
    REP(k, n)
    ret[i][j] = add(ret[i][j], mul(a[i][k], b[k][j]));
    return ret;
}

Mat fastPow(Mat x, int n) {
    Mat ret(x.size(), Vec(x.size(), 0));
    REP(i, x.size())
    ret[i][i] = 1;
    while (0 < n) {
        if ((n % 2) == 0) {
            x = mulMatMat(x, x);
            n >>= 1;
        } else {
            ret = mulMatMat(ret, x);
            --n;
        }
    }
    return ret;
}

void Gauss(vector<long long>& A) {
    int rank = 0;
    for (int d = 0; d < 60; ++d) {
        int pivot = -1;
        for (int i = rank; i < (int) A.size(); ++i) {
            if (A[i] & (1LL << d)) {
                pivot = i;
                break;
            }
        }
        if (pivot == -1) continue;
        swap(A[rank], A[pivot]);
        for (int j = 0; j < (int) A.size(); ++j) {
            if (j == rank) continue;
            if (!(A[j] & (1LL << d))) continue;
            A[j] ^= A[rank];
        }
        ++rank;
    }
}

// bit操作系
int bitcount(int n) {
    int cnt = 0;
    REP(i, N) {
        if (n >> i & 1) cnt++;
    }
    return cnt;
}

int erase(int n, int i) {
    int up = n;
    up = up >> (i + 1);
    up = up << i;
    int low = n & ((1 << i) - 1);
    return up | low;
}

int push(int n, int v, int i) {
    int up = n;
    up = up >> i;
    up = up << (i + 1);
    int low = n & ((1 << i) - 1);
    return up | low | (v << i);
}

