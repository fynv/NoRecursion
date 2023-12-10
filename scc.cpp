#include <vector>
#include <stack>
#include <list>
#include <unordered_set>

using namespace std;

int scc(int N, int M, vector<int> A, vector<int> B)
{
    vector<unordered_set<int>> edge_map(N);
    vector<unordered_set<int>> edge_map_r(N);

    for (int i = 0; i < M; i++)
    {
        edge_map[A[i] - 1].insert(B[i] - 1);
        edge_map_r[B[i] - 1].insert(A[i] - 1);
    }
    
    struct State
    {
        int idx;
        unordered_set<int>::iterator it;
    };

    vector<bool> reached(N);
    list<int> order;

    for (int i = 0; i < N; i++)
    {
        if (reached[i]) continue;

        stack<State> st;
        State cur = { i, edge_map_r[i].begin() };
        reached[i] = true;

        while (true)
        {
            int j = cur.idx;
            unordered_set<int>& edges = edge_map_r[j];
            if (cur.it != edges.end())
            {
                int k = *cur.it;
                cur.it++;
                if (!reached[k])
                {
                    st.push(cur);
                    cur = { k, edge_map_r[k].begin() };
                    reached[k] = true;
                }
            }
            else
            {
                order.push_front(j);
                if (st.size() < 1) break;
                cur = st.top();
                st.pop();
            }
        }
    }

    fill(reached.begin(), reached.end(), false);

    vector<vector<int>> lst_scc;

    auto iter_order = order.begin();
    while (iter_order != order.end())
    {
        int i = *iter_order;
        iter_order++;
        if (reached[i]) continue;

        stack<State> st;
        State cur = { i, edge_map[i].begin() };
        reached[i] = true;

        vector<int> scc;
        while (true)
        {
            int j = cur.idx;
            unordered_set<int>& edges = edge_map[j];
            if (cur.it == edges.begin())
            {
                scc.push_back(j);
            }
            if (cur.it != edges.end())
            {
                int k = *cur.it;
                cur.it++;
                if (!reached[k])
                {
                    st.push(cur);
                    cur = { k, edge_map[k].begin() };
                    reached[k] = true;
                }
            }
            else
            {
                if (st.size() < 1) break;
                cur = st.top();
                st.pop();
            }
        }

        lst_scc.push_back(scc);
    }

    for (int i = 0; i < lst_scc.size(); i++)
    {
        printf("scc %d: [ ", i + 1);
        auto& scc = lst_scc[i];
        for (int j = 0; j < scc.size(); j++)
        {
            printf("%d ", scc[j] + 1);            
        }
        printf("]\n");
    }

}

int main()
{
	int N = 10;
	int M = 9;
	vector<int> A = { 3, 2, 5, 9, 10, 3, 3, 9, 4 };
	vector<int> B = { 9, 5, 7, 8, 6, 4, 5, 3, 9 };

	scc(N, M, A, B);

	return 0;
}
