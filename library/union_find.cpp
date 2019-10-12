
struct UnionFind {
    vector<int> par;
    vector<int> rank;
    vector<int> vsize;
    UnionFind(int size) : par(size), rank(size), vsize(size) {
        REP(i, size) {
            par[i] = i;
            rank[i] = 0;
            vsize[i] = 1;
        }
    }
    int find(int x) {
        if (par[x] == x) {
            return x;
        } else {
            return par[x] = find(par[x]);
        }
    }
    void unite(int x, int y) {
        x = find(x), y = find(y);
        if (x == y) return;
        if (rank[x] < rank[y]) {
            par[x] = y;
        } else {
            par[y] = x;
            if (rank[x] == rank[y]) rank[x]++;
        }
        vsize[x] += vsize[y];
        vsize[y] = vsize[x];
    }
    bool same(int x, int y) { return find(x) == find(y); }
    int size(int x) { return vsize[find(x)]; }
};

struct PartiallyPersistentUnionFind {
    vector<int> data;
    vector<int> last;
    vector<vector<pair<int, int>>> add;

    PartiallyPersistentUnionFind() {}

    PartiallyPersistentUnionFind(int sz) : data(sz, -1), last(sz, 1e9), add(sz) {
        for (auto& vs : add) vs.emplace_back(-1, -1);
    }

    bool unite(int t, int x, int y) {
        x = find(t, x);
        y = find(t, y);
        if (x == y) return false;
        if (data[x] > data[y]) swap(x, y);
        data[x] += data[y];
        add[x].emplace_back(t, data[x]);
        data[y] = x;
        last[y] = t;
        return true;
    }

    int find(int t, int x) {
        if (t < last[x]) return x;
        return find(t, data[x]);
    }

    int size(int t, int x) {
        x = find(t, x);
        return -prev(lower_bound(begin(add[x]), end(add[x]), make_pair(t, 0)))->second;
    }
};
