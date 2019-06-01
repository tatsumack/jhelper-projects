// Range Minimum Query
struct SegmentTree {
private:
    int n;
    vector<int> node;

public:
    SegmentTree(vector<int> v) {
        int sz = v.size();
        n = 1;
        while (n < sz) n *= 2;
        node.resize(2 * n - 1, INF);
        for (int i = 0; i < sz; i++) node[i + n - 1] = v[i];
        for (int i = n - 2; i >= 0; i--) node[i] = min(node[2 * i + 1], node[2 * i + 2]);
    }

    void update(int x, int val) {
        x += (n - 1);
        node[x] = val;
        while (x > 0) {
            x = (x - 1) / 2;
            node[x] = min(node[2 * x + 1], node[2 * x + 2]);
        }
    }

    int getmin(int a, int b, int k = 0, int l = 0, int r = -1) {
        if (r < 0) r = n;
        if (r <= a || b <= l) return INF;
        if (a <= l && r <= b) return node[k];

        int vl = getmin(a, b, 2 * k + 1, l, (l + r) / 2);
        int vr = getmin(a, b, 2 * k + 2, (l + r) / 2, r);
        return min(vl, vr);
    }
};

// Range Sum Query
struct SegmentTree {
private:
    int n;
    vector<int> node;
public:
    SegmentTree(vector<int> v) {
        int sz = (int) v.size();
        n = 1;
        while (n < sz) n *= 2;
        node.resize(2 * n - 1, 0);

        for (int i = 0; i < sz; i++) node[i + n - 1] = v[i];
        for (int i = n - 2; i >= 0; i--) node[i] = node[i * 2 + 1] + node[i * 2 + 2];
    }

    void add(int k, int val) {
        k += (n - 1);
        node[k] += val;

        while (k > 0) {
            k = (k - 1) / 2;
            node[k] = node[2 * k + 1] + node[2 * k + 2];
        }
    }

    int getsum(int a, int b, int k = 0, int l = 0, int r = -1) {
        if (r < 0) r = n;
        if (b <= l || r <= a) return 0;
        if (a <= l && r <= b) return node[k];

        int vl = getsum(a, b, 2 * k + 1, l, (l + r) / 2);
        int vr = getsum(a, b, 2 * k + 2, (l + r) / 2, r);
        return vl + vr;
    }
};

// 区間最小、区間更新
struct LazySegmentTree {
private:
    int n;
    vector<int> node, lazy;
    vector<bool> lazyFlag;

public:
    LazySegmentTree(vector<int> v) {
        int sz = (int) v.size();
        n = 1;
        while (n < sz) n *= 2;
        node.resize(2 * n - 1);
        lazy.resize(2 * n - 1, INF);
        lazyFlag.resize(2 * n - 1, false);

        for (int i = 0; i < sz; i++) node[i + n - 1] = v[i];
        for (int i = n - 2; i >= 0; i--) node[i] = min(node[i * 2 + 1], node[i * 2 + 2]);
    }

    void lazyEvaluate(int k, int l, int r) {
        if (lazyFlag[k]) {
            node[k] = lazy[k];
            if (r - l > 1) {
                lazy[k * 2 + 1] = lazy[k * 2 + 2] = lazy[k];
                lazyFlag[k * 2 + 1] = lazyFlag[k * 2 + 2] = true;
            }
            lazyFlag[k] = false;
        }
    }

    void update(int a, int b, int x, int k = 0, int l = 0, int r = -1) {
        if (r < 0) r = n;
        lazyEvaluate(k, l, r);
        if (b <= l || r <= a) return;
        if (a <= l && r <= b) {
            lazy[k] = x;
            lazyFlag[k] = true;
            lazyEvaluate(k, l, r);
        } else {
            update(a, b, x, 2 * k + 1, l, (l + r) / 2);
            update(a, b, x, 2 * k + 2, (l + r) / 2, r);
            node[k] = min(node[2 * k + 1], node[2 * k + 2]);
        }
    }

    int find(int a, int b, int k = 0, int l = 0, int r = -1) {
        if (r < 0) r = n;
        lazyEvaluate(k, l, r);
        if (b <= l || r <= a) return INF;
        if (a <= l && r <= b) return node[k];
        int vl = find(a, b, 2 * k + 1, l, (l + r) / 2);
        int vr = find(a, b, 2 * k + 2, (l + r) / 2, r);
        return min(vl, vr);
    }
};


// 区間和、区間加算
struct LazySegmentTree {
private:
    int n;
    vector<int> node, lazy;

public:
    LazySegmentTree(vector<int> v) {
        int sz = (int) v.size();
        n = 1;
        while (n < sz) n *= 2;
        node.resize(2 * n - 1);
        lazy.resize(2 * n - 1, 0);

        for (int i = 0; i < sz; i++) node[i + n - 1] = v[i];
        for (int i = n - 2; i >= 0; i--) node[i] = node[i * 2 + 1] + node[i * 2 + 2];
    }

    void eval(int k, int l, int r) {
        if (lazy[k] != 0) {
            node[k] += lazy[k];
            if (r - l > 1) {
                lazy[2 * k + 1] += lazy[k] / 2;
                lazy[2 * k + 2] += lazy[k] / 2;
            }
            lazy[k] = 0;
        }
    }

    void add(int a, int b, int x, int k = 0, int l = 0, int r = -1) {
        if (r < 0) r = n;
        eval(k, l, r);
        if (b <= l || r <= a) return;
        if (a <= l && r <= b) {
            lazy[k] += (r - l) * x;
            eval(k, l, r);
        } else {
            add(a, b, x, 2 * k + 1, l, (l + r) / 2);
            add(a, b, x, 2 * k + 2, (l + r) / 2, r);
            node[k] = node[2 * k + 1] + node[2 * k + 2];
        }
    }

    int getsum(int a, int b, int k = 0, int l = 0, int r = -1) {
        if (r < 0) r = n;
        eval(k, l, r);
        if (b <= l || r <= a) return 0;
        if (a <= l && r <= b) return node[k];
        int vl = getsum(a, b, 2 * k + 1, l, (l + r) / 2);
        int vr = getsum(a, b, 2 * k + 2, (l + r) / 2, r);
        return vl + vr;
    }
};

// 区間最大、区間加算
struct LazySegmentTree {
private:
    int n;
    vector<int> node, lazy;

public:
    LazySegmentTree(vector<int> v) {
        int sz = (int) v.size();
        n = 1;
        while (n < sz) n *= 2;
        node.resize(2 * n - 1);
        lazy.resize(2 * n - 1, 0);

        for (int i = 0; i < sz; i++) node[i + n - 1] = v[i];
        for (int i = n - 2; i >= 0; i--) node[i] = max(node[i * 2 + 1], node[i * 2 + 2]);
    }

    void eval(int k, int l, int r) {
        if (lazy[k] != 0) {
            node[k] += lazy[k];
            if (r - l > 1) {
                lazy[2 * k + 1] += lazy[k];
                lazy[2 * k + 2] += lazy[k];
            }
            lazy[k] = 0;
        }
    }

    void add(int a, int b, int x, int k = 0, int l = 0, int r = -1) {
        if (r < 0) r = n;
        eval(k, l, r);
        if (b <= l || r <= a) return;
        if (a <= l && r <= b) {
            lazy[k] += x;
            eval(k, l, r);
        } else {
            add(a, b, x, 2 * k + 1, l, (l + r) / 2);
            add(a, b, x, 2 * k + 2, (l + r) / 2, r);
            node[k] = max(node[2 * k + 1], node[2 * k + 2]);
        }
    }

    int find(int a, int b, int k = 0, int l = 0, int r = -1) {
        if (r < 0) r = n;
        eval(k, l, r);
        if (b <= l || r <= a) return -INF;
        if (a <= l && r <= b) return node[k];
        int vl = find(a, b, 2 * k + 1, l, (l + r) / 2);
        int vr = find(a, b, 2 * k + 2, (l + r) / 2, r);
        return max(vl, vr);
    }
};
