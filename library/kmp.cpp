// http://mayokoex.hatenablog.com/entry/2016/03/22/201438
// https://snuke.hatenablog.com/entry/2017/07/18/101026
// http://potetisensei.hatenablog.com/entry/2017/07/10/174908

vector<int> makeTable(const string& s) {
    int n = s.size();
    vector<int> ret(n + 1);
    ret[0] = -1;
    int j = -1;
    for (int i = 0; i < n; i++) {
        while (j >= 0 && s[i] != s[j]) j = ret[j];
        j++;
        ret[i + 1] = j;
    }
    return ret;
}

vector<int> kmp(const string& str, const string& word) {
    vector<int> table = makeTable(word), ret;
    int m = 0, i = 0, n = str.size();
    while (m + i < n) {
        if (word[i] == str[m + i]) {
            if (++i == (int) (word.size())) {
                ret.push_back(m);
                m = m + i - table[i];
                i = table[i];
            }
        } else {
            m = m + i - table[i];
            if (i > 0) i = table[i];
        }
    }
    return ret;
}