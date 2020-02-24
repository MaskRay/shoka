namespace strings {

struct DefaultCharRank {
  int operator[](int i) const { return s[i] - 'a'; }
  const char *s;
};

} // namespace strings

template <int N> struct PrefixTable {
  void compute(int n, const char *s) {
    z[0] = 0;
    for (int i = 1, j = 0; i < n; ++i) {
      z[i] = i < j + z[j] ? std::min(z[i - j], j + z[j] - i) : 0;
      while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
        z[i]++;
      }
      if (i + z[i] > j + z[j]) {
        j = i;
      }
    }
    z[0] = n;
  }

  int z[N];
};

template <int N, int C, typename CharRank = strings::DefaultCharRank>
struct SA {
  void compute(int _n, const char *s) {
    n = _n;
    sort(C, IdOrder{}, CharRank{s}, sa);
    int range = rank(CharRank{s}, CharRank{s});
    for (int length = 1; length < n && range < n; length <<= 1) {
      memcpy(trk, rk, sizeof(*rk) * n);
      TRank r1{n, length, trk}, r0{n, 0, trk};
      sort(range + 1, IdOrder{}, r1, tsa);
      sort(range + 1, TOrder{tsa}, r0, sa);
      range = rank(r0, r1);
    }
  }

  int sa[N], rk[N];

private:
  struct IdOrder {
    int operator[](int i) const { return i; }
  };

  struct TOrder {
    int operator[](int i) const { return sa[i]; }
    const int *sa;
  };

  struct TRank {
    int operator[](int i) const {
      return i + shift < n ? rk[i + shift] + 1 : 0;
    }
    int n, shift;
    const int *rk;
  };

  template <typename Order, typename Rank>
  void sort(int range, const Order &o, const Rank &r, int *out) {
    memset(count, 0, sizeof(*count) * range);
    for (int i = 0; i < n; ++i) {
      count[r[i]]++;
    }
    for (int i = 1; i < range; ++i) {
      count[i] += count[i - 1];
    }
    for (int i = n; i--;) {
      out[--count[r[o[i]]]] = o[i];
    }
  }

  template <typename Rank0, typename Rank1>
  int rank(const Rank0 &r0, const Rank1 &r1) {
    rk[sa[0]] = 0;
    for (int _ = 1; _ < n; ++_) {
      int i = sa[_];
      int j = sa[_ - 1];
      rk[i] = rk[j] + (r0[i] != r0[j] || r1[i] != r1[j]);
    }
    return rk[sa[n - 1]] + 1;
  }

  int n;
  int count[std::max(N, C)], tsa[N], trk[N];
};

template <int N, int C> struct SAM {
  struct Node {
    int length;
    Node *parent, *go[C];
  };

  SAM() { memset(nodes, 0, sizeof(nodes)); }

  Node *root() { return nodes; }

  void reset() {
    memset(nodes, 0, sizeof(*nodes) * (node_count));
    node_count = 1;
  }

  Node *extend(Node *p, int c) {
    Node *np = new_node(p->length + 1);
    while (p && !p->go[c]) {
      p->go[c] = np;
      p = p->parent;
    }
    if (!p) {
      np->parent = root();
    } else {
      Node *q = p->go[c];
      if (p->length + 1 == q->length) {
        np->parent = q;
      } else {
        Node *nq = new_node(p->length + 1);
        memcpy(nq->go, q->go, sizeof(q->go));
        nq->parent = q->parent;
        np->parent = q->parent = nq;
        while (p && p->go[c] == q) {
          p->go[c] = nq;
          p = p->parent;
        }
      }
    }
    return np;
  }

  int node_count = 1;
  Node nodes[N << 1];

private:
  Node *new_node(int length) { return nodes + (node_count++); }
};
