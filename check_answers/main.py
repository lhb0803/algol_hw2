import sys


class Graph:
    def __init__(self, graph_file):
        self.vertex_dict = {}
        self.edge_dict = {}
        with open(graph_file, "r") as f:
            while True:
                line = f.readline()
                if not line: break
                line_split = line.split()

                if line_split[0] == 'v':
                    self.vertex_dict[int(line_split[1])] = int(line_split[2])

                if line_split[0] == 'e':
                    v1 = int(line_split[1])
                    if v1 in self.edge_dict.keys():
                        self.edge_dict[v1].append(int(line_split[2]))
                    else:
                        self.edge_dict[v1] = []
                        self.edge_dict[v1].append(int(line_split[2]))

    def is_neighbor(self, v1, v2):
        if v1 > v2:
            v1, v2 = v2, v1

        if v1 in self.edge_dict.keys() and v2 in self.edge_dict[v1]:
            return True
        else:
            return False

    def get_label(self, v):
        return self.vertex_dict[v]


def read_answer_file(answer_path):
    my_answer = []
    with open(answer_path, "r") as f:
        while True:
            line = f.readline()
            if not line: break
            my_embedding = []
            if line[0] != 't':
                line_split = line.split()
                for s in line_split[1:]:
                    my_embedding.append(int(s))
                if len(my_embedding) > 0:
                    my_answer.append(my_embedding)

    return my_answer


def grade_my_answer(my_answer, data_g, query_g):
    cnt = 0
    correct_cnt = 0
    for my_embedding in my_answer:
        print(f"my_embedding: {my_embedding}")
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
    return True


def main(argv):
    data_graph_path = "../data/" + argv[1]
    query_graph_path = "../query/" + argv[2]
    answer_path = "../build/main/" + argv[3]

    data_g = Graph(data_graph_path)
    query_g = Graph(query_graph_path)
    my_answer = read_answer_file(answer_path)

    grade = grade_my_answer(my_answer, data_g, query_g)
    print(f"our grade is {100 * grade:.2f} %")


if __name__ == '__main__':
    main(sys.argv)
