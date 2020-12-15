# pip install ploty==4.13
# pip install -U kaleido

import networkx as nx
import matplotlib.pyplot as plt

ants = 0
def read_farm():
    g = nx.Graph()

    g.add_edges_from([('s','1'), ('1','7'), ('7','5'),
                      ('5','6'), ('6','t'), ('s','2'),
                      ('2','3'), ('3','4'), ('4','5'),
                      ('1','8'), ('8','9'), ('9','10'),
                      ('10','11'),('11','t')])
    plt.subplot(121)
    fig, ax = plt.subplots()
    nx.draw(g, pos=nx.planar_layout(g), node_color='r', edge_color='b')
    fig.savefig('bar.png')
    plt.show()

    print(g.adj)

def draw_graph():
    G = nx.cubical_graph()
    plt.subplot(121)
    nx.draw(G)
    plt.subplot(122)
    fig, ax = plt.subplots()
    fig.savefig('foo.png')
    plt.show()
    nx.draw(G)
    nx.draw(G, pos=nx.circular_layout(G), node_color='r', edge_color='b')
    fig.savefig('foo.png')
    plt.show()

if __name__ == "__main__":
    read_farm()
    draw_graph()
