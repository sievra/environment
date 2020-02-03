#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <set>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <windows.h>

#define pb push_back

using namespace std;

struct Vertex
{
    double x, y;

    Vertex(){}

    Vertex(int xx, int yy)
    {
        x = xx;
        y = yy;
    }
};

struct edge
{
    string v;
    double w;

    edge(string vv, double ww)
    {
        v = vv;
        w = ww;
    }

    edge(string vv)
    {
        v = vv;
        w = 1;
    }

    const bool operator < (const edge &b) const
    {
        if (v != b.v)
            return v < b.v;
        return w < b.w;
    }
};

unordered_map <string, Vertex> ver; /// список вершин
unordered_map <string, multiset <edge> > in, out; /// список смежности
bool oriented, weighted; /// ориентированность, взвешенность

bool findv(string v)
{
    return ver.count(v);
}

bool finde(string u, string v)
{
    if (!weighted)
        return out[u].count(v);
    for (auto it: out[u])
        if (it.v == v)
            return true;
    return false;
}

bool addv(string v, double xx, double yy)
{
    if (ver.count(v))
    {
        cout << "Vertex \"" << v << "\" is already exist\n";
        return false;
    }
    ver[v] = Vertex(xx, yy);
    return true;
}

bool adde(string u, string v)
{
    if (!findv(u))
    {
        cout << "Vertex \"" + u + "\" doesn't exist\n";
        return false;
    }
    if (!findv(v))
    {
        cout << "Vertex \"" + v + "\" doesn't exist\n";
        return false;
    }

    out[u].insert(edge(v));
    in[v].insert(edge(u));
    if (!oriented)
    {
        out[v].insert(edge(u));
        in[u].insert(edge(v));
    }
    return true;
}

bool adde(string u, string v, double w)
{
    if (!findv(u))
    {
        cout << "Vertex \"" + u + "\" doesn't exist\n";
        return false;
    }
    if (!findv(v))
    {
        cout << "Vertex \"" + v + "\" doesn't exist\n";
        return false;
    }

    out[u].insert(edge(v, w));
    in[v].insert(edge(u, w));
    if (!oriented)
    {
        out[v].insert(edge(u, w));
        in[u].insert(edge(v, w));
    }
    return true;
}

bool deletev(string v)
{
    if (!findv(v))
    {
        cout << "Vertex \"" + v + "\" doesn't exist\n";
        return false;
    }

    for (auto it: in[v])
    {
        string u = it.v;
        it.v = v;
        out[u].erase(it);
    }
    out.erase(v);
    in.erase(v);
    ver.erase(v);
    return true;
}

bool deletee(string u, string v)
{
    if (!findv(u))
    {
        cout << "Vertex \"" + u + "\" doesn't exist\n";
        return false;
    }
    if (!findv(v))
    {
        cout << "Vertex \"" + v + "\" doesn't exist\n";
        return false;
    }
    if (!finde(u, v))
    {
        cout << "Edge \"" + u + "-" + v + "\" doesn't exist\n";
        return false;
    }
    for (auto it = out[u].begin(); it != out[u].end();)
    {
        auto it1 = *it;
        it++;
        if (it1.v == v)
        {
            out[u].erase(it1);
            in[v].erase(edge(u, it1.w));
        }
    }
    if (!oriented)
    {
        for (auto it = out[v].begin(); it != out[v].end();)
        {
            auto it1 = *it;
            it++;
            if (it1.v == u)
            {
                out[v].erase(it1);
                in[u].erase(edge(v, it1.w));
            }
        }
    }
    return true;
}

void getmatrix()
{
    freopen("matrix.txt", "r", stdin);
    cin >> oriented >> weighted;
    int n;
    double x, y;
    vector <string> v;
    string s;
    cin >> n;
    for (int i = 0; i < n; i++) /// добавление вершин
    {
        cin >> s >> x >> y;
        addv(s, x, y);
        v.push_back(s);
    }

    for (int i = 0; i < n; i++) /// матрица
        for (int j = 0; j < n; j++)
        {
            int c;
            cin >> c;
            if (!oriented && i > j)
                continue;
            if (c)
                if (!weighted)
                    adde(v[i], v[j]);
                else
                    adde(v[i], v[j], c);
        }
    fclose(stdin);
    freopen("CON", "r", stdin);
}

void getedges()
{
    freopen("edges.txt", "r", stdin);
    cin >> oriented >> weighted;
    int n;
    double x, y;
    vector <string> v;
    string s;
    cin >> n;
    for (int i = 0; i < n; i++) /// добавление вершин
    {
        cin >> s >> x >> y;
        addv(s, x, y);
        v.pb(s);
    }

    int m;
    cin >> m;
    for (int i = 0; i < m; i++) /// список ребер
    {
        string u, v;
        int w;
        cin >> u >> v;
        if (!weighted)
            adde(u, v);
        else
        {
            double w;
            cin >> w;
            adde(u, v, w);
        }
    }
    fclose(stdin);
    freopen("CON", "r", stdin);
}

void getincidence()
{
    freopen("incidence.txt", "r", stdin);
    cin >> oriented >> weighted;
    int n;
    double x, y;
    vector <string> v;
    string s;
    cin >> n;
    for (int i = 0; i < n; i++) /// добавление вершин
    {
        cin >> s >> x >> y;
        addv(s, x, y);
        v.pb(s);
    }

    int m;
    cin >> m;
    for (int i = 0; i < m; i++) /// список ребер
    {
        string uu, vv;
        double ww;
        for (int j = 0; j < n; j++)
        {
            double w;
            cin >> w;
            if (w == 0)
                continue;
            if (w < 0)
                uu = v[j];
            else
            {
                vv = v[j];
                ww = w;
            }
        }
        if (!finde(uu, vv))
            adde(uu, vv, ww);
    }
    fclose(stdin);
    freopen("CON", "r", stdin);
}

void getadjacency()
{
    freopen("adjacency.txt", "r", stdin);
    cin >> oriented >> weighted;
    int n;
    double x, y;
    vector <string> v;
    string s;
    cin >> n;
    for (int i = 0; i < n; i++) /// добавление вершин
    {
        cin >> s >> x >> y;
        addv(s, x, y);
        v.pb(s);
    }

    for (int i = 0; i < n; i++) /// список ребер
    {
        int m;
        cin >> m;
        for (int j = 0; j < m; j++)
        {
            string vv;
            cin >> vv;
            if (!weighted)
            {
                if (!finde(v[i], vv))
                    adde(v[i], vv);
            }
            else
            {
                double ww;
                cin >> ww;
                if (!finde(v[i], vv))
                    adde(v[i], vv, ww);
            }
        }
    }
    fclose(stdin);
    freopen("CON", "r", stdin);
}

void imprt()
{
    cout << "Import data from:\n";
    cout << "0. none\n";
    cout << "1. adjacency matrix\n";
    cout << "2. edges list\n";
    cout << "3. incidence matrix\n";
    cout << "4. adjacency list\n";

    int a;
    cin >> a;
    switch (a)
    {
        case 1: getmatrix(); break;
        case 2: getedges(); break;
        case 3: getincidence(); break;
        case 4: getadjacency(); break;
        default: break;
    }
    cout << '\n';
}

void success()
{
    cout << "Successful!\n";
}

void addVertex()
{
    cout << "Insert name of vertex:\n";
    string name;
    cin >> name;

    cout << "Insert coordinates of vertex:\n";
    double x, y;
    cin >> x >> y;
    if (addv(name, x, y))
        success();
}

void addEdge()
{
    cout << "Insert name of vertexes:\n";
    string u, v;
    cin >> u >> v;
    if (!weighted)
    {
        if (adde(u, v))
            success();
    }
    else
    {
        cout << "Insert weight of edge:\n";
        double w;
        cin >> w;
        if (adde(u, v, w))
            success();
    }
}

void deleteVertex()
{
    cout << "Insert name of vertex:\n";
    string v;
    cin >> v;
    if (deletev(v))
        success();
}

void deleteEdge()
{
    cout << "Insert name of vertexes:\n";
    string u, v;
    cin >> u >> v;
    if (deletee(u, v))
        success();
}

void changeGraph()
{
    cout << "0. Cancel\n";
    cout << "1. Add vertex\n";
    cout << "2. Add edge\n";
    cout << "3. Delete vertex\n";
    cout << "4. Delete edge\n";

    int a;
    cin >> a;
    switch (a)
    {
        case 0: return; break;
        case 1: addVertex(); break;
        case 2: addEdge(); break;
        case 3: deleteVertex(); break;
        case 4: deleteEdge(); break;
        default: break;
    }
}

int getEdgesCount()
{
    int ans = 0;
    for (auto it: out)
        ans += it.second.size();
    if (!oriented)
        ans /= 2;
    return ans;
}

int getGraphDegree()
{
    int ans = -1;
    for (auto it: out)
        ans = max(ans, (int)it.second.size());
    return ans;
}

pair <double, double> getRadDiam()
{
    unordered_map <string, int> vsi;
    int n = ver.size();

    int ind = 0;
    for (auto it: ver)
        vsi[it.first] = ind++;

    double ext[n][n];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i != j)
                ext[i][j] = 1e8;
            else
                ext[i][j] = 0;
    for (auto it1: out)
        for (auto it2: it1.second)
            ext[vsi[it1.first]][vsi[it2.v]] = it2.w;

    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                ext[i][j] = min(ext[i][j], ext[i][k] + ext[k][j]);

    double rad = 1e9, diam = -1e9;
    for (int i = 0; i < n; i++)
    {
        double mx = -1;
        for (int j = 0; j < n; j++)
            if (i != j)
            {
                mx = max(mx, ext[i][j]);
                diam = max(diam, ext[i][j]);
            }
        rad = min(rad, mx);
    }
    return {rad, diam};
}

pair <int, int> dfs(string v, unordered_map <string, bool> &used, bool print)
{
    if (print)
        cout << v << ' ';
    int vv = 0, ee = 0;
    used[v] = true;
    for (auto it: out[v])
        if (!used[it.v])
        {
            auto ans = dfs(it.v, used, print);
            vv += ans.first;
            ee += ans.second;
        }
    return {vv + 1, ee + out[v].size()};
}

pair <int, int> getConComp()
{
    int n = ver.size(), comp = 0, cycl = 0;
    unordered_map <string, bool> used;
    for (auto it: ver)
        used[it.first] = false;

    for (auto it: ver)
        if (!used[it.first])
        {
            auto ans = dfs(it.first, used, false);
            comp++;
            cycl += ans.second / 2 - (ans.first - 1);
        }
    return {comp, cycl};
}

void showInfo()
{
    cout << "Count of vertexes: " << ver.size() << '\n';
    cout << "Count of edges: " << getEdgesCount() << '\n';
    cout << "Graph degree: " << getGraphDegree() << '\n';
    {
        auto ext = getRadDiam();
        cout << "Radius: ";
        if (ext.first > 1e5)
            cout << "infinity";
        else
            cout << ext.first;
        cout << '\n';

        cout << "Diameter: ";
        if (ext.second > 1e5)
            cout << "infinity";
        else
            cout << ext.second;
        cout << '\n';
    }
    if (!oriented)
    {
        auto ans = getConComp();
        cout << "Count of connected components: " << ans.first << '\n';
        cout << "Cyclic number: " << ans.second << '\n';
    }
}

void isTree()
{
    auto ans = getConComp();
    if (ans.first <= 1 && ans.second == 0)
        cout << "Yes";
    else
        cout << "No";
}

void findVertex()
{
    cout << "Insert name of vertex:\n";
    string v;
    cin >> v;
    if (findv(v))
        cout << "Found";
    else
        cout << "Not found";
}

void findEdge()
{
    cout << "Insert name of vertexes:\n";
    string u, v;
    cin >> u >> v;

    if (!findv(u))
    {
        cout << "Vertex \"" + u + "\" doesn't exist\n";
        return;
    }
    if (!findv(v))
    {
        cout << "Vertex \"" + v + "\" doesn't exist\n";
        return;
    }
    if (finde(u, v))
    {
        cout << "Edge \"" + u + "-" + v + "\" is found\n";
        if (weighted)
            for (auto it: out[u])
                if (it.v == v)
                    cout << "Weight: " << it.w << '\n';
    }
    else
        cout << "Edge \"" + u + "-" + v + "\" doesn't exist\n";
}

void getVertexesList()
{
    cout << "Vertexes list:\n";
    for (auto it: ver)
    {
        cout << it.first << ' ';
        if (!oriented)
            cout << "Degree: " << out[it.first].size() << '\n';
        else
        {
            cout << "Out: " << out[it.first].size() << ' ';
            cout << "In: " << in[it.first].size() << '\n';
        }
    }
}

void getEdgesList()
{
    multiset < pair<string, edge> > s;
    cout << "Edges list:\n";
    for (auto it1: out)
        for (auto it2: out[it1.first])
        {
            if (!oriented)
                if (s.count({it1.first, edge(it2.v, it2.w)}))
                    s.erase({it1.first, edge(it2.v, it2.w)});
                else
                {
                    cout << it1.first << ' ' << it2.v << '\n';
                    s.insert({it2.v, edge(it1.first, it2.w)});
                }
            else
                cout << it1.first << ' ' << it2.v << '\n';
        }
}

void dfsOrder()
{
    vector <string> v;
    unordered_map <string, bool> used;

    cout << "Choose start vertex:\n";
    int id = 0;
    for (auto it: ver)
    {
        used[it.first] = false;
        v.pb(it.first);
        cout << id++ << ". " << it.first << '\n';
    }

    int a;
    cin >> a;
    if (a >= 0 && a < v.size())
        dfs(v[a], used, true);
    else
        cout << "Incorrect input";
}

string findCycle(string v, unordered_map <string, bool> &used, edge prev)
{
    used[v] = true;
    for (auto it: out[v])
        if (!used[it.v])
        {
            string ans = findCycle(it.v, used, edge(v, it.w));
            if (ans == "exit")
                return ans;
            if (ans != "")
            {
                cout << v << ' ';
                if (ans == v)
                    return "exit";
                else
                    return ans;
            }
        }
        else
            if (it.v != prev.v || it.w != prev.w)
            {
                cout << it.v << ' ' << v << ' ';
                return it.v;
            }
    return "";
}

void cycle()
{
    unordered_map <string, bool> used;
    for (auto it: ver)
        used[it.first] = false;

    for (auto it: ver)
        if (!used[it.first])
        {
            auto ans = findCycle(it.first, used, edge("", -1));
            if (ans == "exit")
                return;
        }
}

void center()
{
    auto res = getConComp();
    if (res.first != 1)
    {
        cout << "No solution";
        return;
    }

    unordered_map <string, int> vsi;
    unordered_map <int, string> vis;
    int n = ver.size();

    int ind = 0;
    for (auto it: ver)
    {
        vis[ind] = it.first;
        vsi[it.first] = ind++;
    }

    double ext[n][n];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i != j)
                ext[i][j] = 1e8;
            else
                ext[i][j] = 0;
    for (auto it1: out)
        for (auto it2: it1.second)
            ext[vsi[it1.first]][vsi[it2.v]] = it2.w;

    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                ext[i][j] = min(ext[i][j], ext[i][k] + ext[k][j]);

    /// vertex center
    double radver = 1e9;
    int ansver = -1;
    for (int i = 0; i < n; i++)
    {
        double mx = -1;
        for (int j = 0; j < n; j++)
            if (i != j)
                mx = max(mx, ext[i][j]);
        if (mx < radver)
        {
            radver = mx;
            ansver = i;
        }
    }

    if (radver > 1e5)
    {
        cout << "No solution";
        return;
    }
    if (oriented)
    {
        cout << "Vertex " << vis[ansver] << '\n';
        cout << "Radius: " << radver;
        return;
    }

    /// edge center
    double radedge = 1e9;
    string edgeu, edgev;
    double wayu, wayv;
    for (auto it1: out)
        for (auto it2: out[it1.first])
        {
            string v = it1.first, u = it2.v;
            for (double vt = it2.w / 1000.0; vt < it2.w; vt += it2.w / 1000.0)
            {
                double mx = -1;
                for (auto it3: ver)
                {
                    string k = it3.first;
                    double d = min(vt + ext[vsi[v]][vsi[k]], it2.w - vt + ext[vsi[u]][vsi[k]]);
                    mx = max(d, mx);
                }
                if (radedge > mx)
                {
                    radedge = mx;
                    edgeu = u;
                    edgev = v;
                    wayu = vt;
                    wayv = it2.w - vt;
                }
            }
        }

    if (radver > 1e5 && radedge > 1e5)
    {
        cout << "No solution";
        return;
    }
    if (radver < radedge)
    {
        cout << "Vertex " << vis[ansver] << '\n';
        cout << "Radius: " << radver;
    }
    else
    {
        cout << "Edge \"" << edgeu << "-" << edgev << "\"\n";
        cout << "Distance to " << edgeu << ": " << wayu << '\n';
        cout << "Distance to " << edgev << ": " << wayv << '\n';
        cout << "Radius: " << radedge;
    }
}

/// matchings
unordered_map <string, int> vsi;
unordered_map <int, string> vis;
int n;
const int MAXN = 100;
vector<int> g[MAXN];
int match[MAXN], p[MAXN], base[MAXN];
bool used[MAXN], flower[MAXN];
queue <int> q;

int lca(int a, int b)
{
	bool used[MAXN] = { 0 };
	while (true)
    {
		a = base[a];
		used[a] = true;
		if (match[a] == -1)
            break;
		a = p[match[a]];
	}
	while (true)
    {
		b = base[b];
		if (used[b])
            return b;
		b = p[match[b]];
	}
}

void mark_path(int v, int b, int child)
{
	while (base[v] != b)
    {
		flower[base[v]] = flower[base[match[v]]] = true;
		p[v] = child;
		child = match[v];
		v = p[match[v]];
	}
}

int find_path(int root)
{
	memset (used, 0, sizeof used);
	memset (p, -1, sizeof p);
	for (int i = 0; i < n; i++)
		base[i] = i;

	used[root] = true;
	while (!q.empty())
        q.pop();
	q.push(root);
	while (!q.empty())
    {
		int v = q.front();
		q.pop();
        for (auto it: out[vis[v]])
        {
            int to = vsi[it.v];
			if (base[v] == base[to] || match[v] == to)
                continue;
			if (to == root || match[to] != -1 && p[match[to]] != -1)
			{
				int curbase = lca(v, to);
				memset (flower, 0, sizeof flower);
				mark_path(v, curbase, to);
				mark_path(to, curbase, v);
				for (int i = 0; i < n; i++)
					if (flower[base[i]])
					{
						base[i] = curbase;
						if (!used[i])
						{
							used[i] = true;
							q.push(i);
						}
					}
			}
			else
                if (p[to] == -1)
                {
                    p[to] = v;
                    if (match[to] == -1)
                        return to;
                    to = match[to];
                    used[to] = true;
                    q.push(to);
                }
		}
	}
	return -1;
}

void matchings()
{
    n = 0;
    vsi.clear();
    vis.clear();
    for (auto it: ver)
    {
        vsi[it.first] = n;
        vis[n++] = it.first;
    }

    memset (match, -1, sizeof match);
	for (int i = 0; i < n; i++)
		if (match[i] == -1)
        {
			int v = find_path(i);
			while (v != -1)
			{
				int pv = p[v],  ppv = match[pv];
				match[v] = pv;
                match[pv] = v;
				v = ppv;
			}
		}

    bool used[MAXN] = { 0 };
    cout << "Matchings:\n";
    for (int i = 0; i < n; i++)
        if (!used[i] && match[i] != -1)
        {
            cout << vis[i] << " - " << vis[match[i]] << '\n';
            used[match[i]] = true;
        }
}

void makeQuery()
{
    cout << "Query:\n";
    cout << "0. none\n";
    cout << "1. Is it a tree?\n";
    cout << "2. Find vertex\n";
    cout << "3. Find edge\n";
    cout << "4. Vertexes list\n";
    cout << "5. Edges list\n";
    cout << "6. DFS order\n";
    cout << "7. Find cycle\n";
    cout << "8. Absolute center\n";
    cout << "9. Matchings\n";

    int a;
    cin >> a;
    switch (a)
    {
        case 1: isTree(); break;
        case 2: findVertex(); break;
        case 3: findEdge(); break;
        case 4: getVertexesList(); break;
        case 5: getEdgesList(); break;
        case 6: dfsOrder(); break;
        case 7: cycle(); break;
        case 8: center(); break;
        case 9: matchings(); break;
        default: break;
    }
    cout << '\n';
}

void visualization()
{
    freopen("visual\\graph.txt", "w", stdout);
    cout << ver.size() << '\n';
    for (auto it: ver)
        cout << it.first << '\n';
    int m = 0;
    for (auto it1: out)
        m += it1.second.size();
    cout << m << '\n';
    for (auto it1: out)
        for (auto it2: out[it1.first])
            cout << it1.first << '\n' << it2.v << '\n' << it2.w << '\n';
    fclose(stdout);
    freopen("CON", "w", stdout);
    cout << "Done\n";
    //string filename = "C:\\windows\\notepad.exe";
    //string filename = "D:\\c++\\HS\\graphs\\environment\\visual\\GraphVisualization.exe";
    //system(filename.c_str());
    //WinExec(filename.c_str(), 0);
    /*
    if (FileExists(filename)) {WinExec(filename.c_str(), SW_SHOW);}
    else {ShowMessage("File не Exists");}*/
    //ShellExecute(0, "open", filename.c_str(), NULL, 0, SW_SHOW);
    //WinExec("D:\\c++\\HS\\graphs\\environment\\visual\\Graph visualization.exe", SW_SHOW);
}

void action()
{
    cout << "What do you want to do?\n";
    cout << "0. Exit\n";
    cout << "1. Change graph\n";
    cout << "2. Show info\n";
    cout << "3. Query\n";
    cout << "4. Visualization\n";

    int a;
    cin >> a;
    switch (a)
    {
        case 0: exit(0); break;
        case 1: changeGraph(); break;
        case 2: showInfo(); break;
        case 3: makeQuery(); break;
        case 4: visualization(); break;
        default: break;
    }
    cout << '\n';
}

int main()
{
    imprt();
    while (true)
        action();
    return 0;
}
