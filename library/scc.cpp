// 強連結成分分解
struct SCC {
    const int n;
    vector<vector<int>> G;
    vector<vector<int>> rG;
    vector<int> vs;
    vector<bool> used;
    vector<int> cmp;

    SCC(int _n) : n(_n), G(_n), rG(_n), used(_n), cmp(_n) {}

    void addEdge(int from, int to) {
        G[from].emplace_back(to);
        rG[to].emplace_back(from);
    }

    void dfs(int v) {
        used[v] = true;
        for (auto&& nxt : G[v]) {
            if (!used[nxt]) dfs(nxt);
        }
        vs.emplace_back(v);
    }

    void rdfs(int v, int k) {
        used[v] = true;
        cmp[v] = k;
        for (auto&& nxt : rG[v]) {
            if (!used[nxt]) rdfs(nxt, k);
        }
        vs.emplace_back(v);
    }

    int scc() {
        REP(v, n)
        {
            if (!used[v]) dfs(v);
        }
        used.assign(n, false);
        int k = 0;
        for (int i = vs.size() - 1; i >= 0; i--) {
            int v = vs[i];
            if (!used[v]) rdfs(v, k++);
        }
        return k;
    }

    bool same(int a, int b) {
        return cmp[a] == cmp[b];
    }

    int get(int a) {
        return cmp[a];
    }
};

