struct LCA {
    static const int MAX_LOG_V = 21;

    vector<vector<int>> G;
    vector<vector<int>> parent;
    vector<int> depth;

    LCA(int size) {
        G = vector<vector<int>>(size);
        parent = vector<vector<int>>(MAX_LOG_V, vector<int>(size));
        depth = vector<int>(size);
    }


    void dfs(int v, int p, int d) {
        parent[0][v] = p;
        depth[v] = d;
        for (int i = 0; i < G[v].size(); i++) {
            if (G[v][i] != p) dfs(G[v][i], v, d + 1);
        }
    }

    void init(int root, int V) {
        dfs(root, -1, 0);
        for (int k = 0; k + 1 < MAX_LOG_V; k++) {
            for (int v = 0; v < V; v++) {
                if (parent[k][v] < 0)
                    parent[k + 1][v] = -1;
                else
                    parent[k + 1][v] = parent[k][parent[k][v]];
            }
        }
    }

    int get(int u, int v) {
        if (depth[u] > depth[v]) swap(u, v);
        for (int k = 0; k < MAX_LOG_V; k++) {
            if ((depth[v] - depth[u]) >> k & 1) {
                v = parent[k][v];
            }
        }
        if (u == v) return u;

        for (int k = MAX_LOG_V - 1; k >= 0; k--) {
            if (parent[k][u] != parent[k][v]) {
                u = parent[k][u];
                v = parent[k][v];
            }
        }

        return parent[0][u];
    }
};