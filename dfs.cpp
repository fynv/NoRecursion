#include <vector>
#include <stack>
#include <unordered_set>

using namespace std;

int dfs(int N, int M, vector<int> A, vector<int> B)
{
	vector<unordered_set<int>> edge_map(N);
	for (int i = 0; i < M; i++)
	{
		edge_map[A[i] - 1].insert(B[i] - 1);		
	}

    struct State
    {
        int idx;
        unordered_set<int>::iterator it;
    };

	vector<bool> reached(N);
    for (int i = 0; i < N; i++)
    {
        if (reached[i]) continue;

        stack<State> st;
        State cur = { i, edge_map[i].begin() };
        reached[i] = true;

        while (true)
        {
            int j = cur.idx;
            unordered_set<int>& edges = edge_map[j];
            if (cur.it == edges.begin())
            {
                printf("%d\n", j);
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
    }

}

int main()
{
	int N = 10;
	int M = 9;
	vector<int> A = { 3, 2, 5, 9, 10, 3, 3, 9, 4 };
	vector<int> B = { 9, 5, 7, 8, 6, 4, 5, 3, 9 };

	dfs(N, M, A, B);

	return 0;
}

