class Graph:
    vertex_list = []
    edge_list = []

    def __init__(self, graph_file):
        with open(graph_file, "r") as f:
            line_split = f.readline().split()

            if line_split[0] == 'v':
                self.vertex_list.append((int(line_split[1]), int(line_split[2])))

            if line_split[0] == 'e':
                self.edge_list.append((int(line_split[1]), int(line_split[2])))

    def is_neighbor(self, v1, v2):
        if v1 > v2:
            v1, v2 = v2, v1

        for edge in self.edge_list:
            if edge[0] == v1:
                if edge[1] == v2:
                    return True
        return False

    def get_label(self, v):
        return self.vertex_list[v][1]


def read_answer_file(answer_path):
    my_answer = []
    with open(answer_path, "r") as f:
        line = f.readline()
        my_embedding = []
        if line[0] != 't':
            line_split = line.split()
            for s in line_split[1:]:
                my_embedding.append(int(s))
            my_answer.append(my_embedding)

    return my_answer


def grade_my_answer(my_answer, data_g, query_g):
    cnt = 0
    correct_cnt = 0
    for my_embedding in my_answer:
        cnt += 1
        correct = check_embedding_right(my_embedding, data_g, query_g)
        if correct:
            correct_cnt += 1

    return correct_cnt/cnt


def check_embedding_right(my_embedding, data_g, query_g):
    # print("checking answers")
    for u, v in enumerate(my_embedding):
        if query_g.get_label(u) != data_g.get_label(v):
            # label different
            print(f"u{u} and v{v} labels are different!")
            return False

        for u_star in range(u+1, len(my_embedding)):
            v_star = my_embedding[u_star]
            if query_g.is_neighbor(u, u_star) and not data_g.is_neighbor(v, v_star):
                print(f"u{u} and u{u_star} are neighbors but, v{v} and v{v_star} are not!")
                return False


if __name__ == '__main__':
    data_graph_path = "../data/a_test.igraph"
    query_graph_path = "../query/a_test.igraph"
    answer_path = "../build/main/out.txt"

    data_g = Graph(data_graph_path)
    query_g = Graph(query_graph_path)
    my_answer = read_answer_file(answer_path)

    grade = grade_my_answer(my_answer, data_g, query_g)
    print(f"our grade is {100 * grade:.2f} %")