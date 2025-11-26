#include "header.h"

/*
인접행렬의 최대크기는 unsigned int만큼
거리는 unsigned int 만큼
인접행렬의 원소는 거리를 의미함
input1.txt의 거리는 1로 고정
*/

void showGraph(unsigned int** matrix, unsigned int size);
unsigned int** getMatrixfromFile(FILE* input, unsigned int* matrix_size, int mode);
int charType(int c);
void push(unsigned int* stk, unsigned int size, unsigned int value, char mode);
unsigned int pop(unsigned int* stk, char mode);
void dfs(unsigned int** matrix, unsigned int size, int* visited, unsigned int* stack);
void bfs(unsigned int** matrix, unsigned int size, int* visited, unsigned int* queue);

int main() {
    
	//input1.txt 처리
    printf("1. DFS / BFS\n\n");

    FILE* input1 = fopen("input1.txt", "r");
    if (input1 == NULL) {
        printf("Error: input1.txt.\n");
        return 1;
    }

    int graph_num = 1;
    unsigned int matrix_size;
    
    while (1) {
        unsigned int** matrix1 = NULL;
        matrix_size = 0;

		matrix1 = getMatrixfromFile(input1, &matrix_size, 1);
        if (matrix_size == 0) break;

		//방문하면 0에서 1로 바꿈
		int* visited = (int*)calloc(matrix_size, sizeof(int));

		//정점 번호 넣으면됨
		//!!오버플로 주의!!
		//구리면 바꾸거나 지워도됨
		//stack[0], queue[0]은 pop으로 나가는 값의 index
		//ex) stack = {3, 2, 3, 4}; //2, 3, 4 순서대로 push됐고 4, 3, 2 순서대로 pop됨
		//ex) queue = {4, 1, 2, 3, 4} //4, 3, 2, 1 순서대로 push됐고 4, 3, 2, 1 순서대로 pop됨
		//push, pop 함수 쓰면됨
		unsigned int* stack = (unsigned int*)calloc(matrix_size+1, sizeof(unsigned int));
		unsigned int* queue = (unsigned int*)calloc(matrix_size+1, sizeof(unsigned int));

        printf("Graph [%d]:\n", graph_num);
        showGraph(matrix1, matrix_size);

        /*
        여기에 1번 문제 작성
        matrix1에 각 그래프를 순차적으로 저장하고
        각각 출력하는 방식임
        즉 한 그래프에 대한 dfs 및 bfs 수행 후 출력하면 됨
        */

        // 그래프 탐방 수행
        printf("----------------------------\n");
        printf("깊이 우선 탐색\n");
        dfs(matrix1, matrix_size, visited, stack);
        printf("너비 우선 탐색\n");
        bfs(matrix1, matrix_size, visited, queue);
        printf("============================\n");
		
        if (matrix1) {
            for (unsigned int i = 0; i < matrix_size; i++) {
                free(*(matrix1 + i));
            }
            free(matrix1);
        }
        graph_num++;
    }

    fclose(input1);

    //input2.txt 처리
    printf("2. Dijkstra\n\n");

    FILE* input2 = fopen("input2.txt", "r");
    if (input2 == NULL) {
        printf("Error: input2.txt.\n");
        return 1;
    }

    graph_num = 1;

    while (1) {
        unsigned int** matrix2 = NULL;
        matrix_size = 0;

		matrix2 = getMatrixfromFile(input2, &matrix_size, 2);
        if (matrix_size == 0) break;

        printf("Graph [%d]:\n", graph_num);
        showGraph(matrix2, matrix_size);

        /*
        여기에 2번 문제 작성
        matrix1에 각 그래프를 순차적으로 저장하고
        각각 출력하는 방식임
        즉 한 그래프에 대한 경로 및 길이 출력하면 됨
        */
		
        if (matrix2) {
            for (unsigned int i = 0; i < matrix_size; i++) {
                free(*(matrix2 + i));
            }
            free(matrix2);
        }
        graph_num++;
    }

    fclose(input2);
    return 0;
}

void dfs(unsigned int** matrix, unsigned int size, int* visited, unsigned int* stack) {
    //  초기화
    for (unsigned int i = 0; i < size; i++) {
        visited[i] = 0;
    }
    for (unsigned int i = 0; i <= size; i++) {
        stack[i] = 0;
    }

    // 시작 정점 push
    push(stack, size + 1, 1, 's');

    unsigned int visit_count = 0;

    // DFS
    while (1) {
        unsigned int v = pop(stack, 's'); 

        // pop 결과 0이면 break
        if (v == 0) break;

        unsigned int idx = v - 1;      

        // 방문한 정점이면 comtinue
        if (visited[idx]) continue;

        // 방문 표시
        visited[idx] = 1;

        // 방문 순서
        if (visit_count == 0) {
            // 첫 번째 정점
            printf("%u", v);
        } else {
            // 그 이후 정점
            printf(" – %u", v);
        }
        visit_count++;

        // 인접 정점 push
        // 작은 번호 먼저 방문
        for (int w = (int)size; w >= 1; w--) {
            if (matrix[idx][w - 1] != 0 && !visited[w - 1]) {
                // v 와 w 가 인접, 방문하지 않은 경우
                push(stack, size + 1, (unsigned int)w, 's');
            }
        }
    }
    printf("\n");
}

void bfs(unsigned int** matrix, unsigned int size, int* visited, unsigned int* queue) {
    // 초기화
    for (unsigned int i = 0; i < size; i++) {
        visited[i] = 0;
    }
    for (unsigned int i = 0; i <= size; i++) {
        queue[i] = 0;
    }

    // 시작 정점 enqueue
    push(queue, size + 1, 1, 'q');
    visited[0] = 1;      // BFS는 enqueue 시점에 방문 표시

    unsigned int visit_count = 0;

    while (1) {
        unsigned int v = pop(queue, 'q');    // dequeue

        if (v == 0) break;

        unsigned int idx = v - 1;

        // 출력
        if (visit_count == 0) {
            printf("%u", v);
        } else {
            printf(" – %u", v);
        }
        visit_count++;

        // 인접 정점 탐색
        for (unsigned int w = 1; w <= size; w++) {
            if (matrix[idx][w - 1] != 0 && !visited[w - 1]) {
                push(queue, size + 1, w, 'q');
                visited[w - 1] = 1;   // enqueue 시 방문 체크
            }
        }
    }
    printf("\n");
}