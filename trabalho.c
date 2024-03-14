#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Definição da estrutura de um nó da árvore AVL
struct Node {
    char name[50];
    int age;
    int height;
    struct Node *left;
    struct Node *right;
};

// Função para criar um novo nó
struct Node* createNode(char name[], int age) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    strcpy(newNode->name, name);
    newNode->age = age;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1; // Altura inicializada como 1
    return newNode;
}

// Função para obter o máximo de dois inteiros
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Função para obter a altura de um nó
int height(struct Node *n) {
    if (n == NULL)
        return 0;
    return n->height;
}

// Função para atualizar a altura de um nó
void updateHeight(struct Node *n) {
    if (n != NULL)
        n->height = 1 + max(height(n->left), height(n->right));
}

// Função para realizar uma rotação à direita em um nó
struct Node *rotateRight(struct Node *y) {
    struct Node *x = y->left;
    struct Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    // Atualiza as alturas dos nós após a rotação
    updateHeight(y);
    updateHeight(x);

    return x;
}

// Função para realizar uma rotação à esquerda em um nó
struct Node *rotateLeft(struct Node *x) {
    struct Node *y = x->right;
    struct Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    // Atualiza as alturas dos nós após a rotação
    updateHeight(x);
    updateHeight(y);

    return y;
}

// Função para obter o fator de equilíbrio de um nó
int balanceFactor(struct Node *n) {
    if (n == NULL)
        return 0;
    return height(n->left) - height(n->right);
}

// Função para inserir um nó em uma árvore AVL
struct Node* insert(struct Node* root, char name[], int age) {
    // Se a árvore estiver vazia, retorna o novo nó
    if (root == NULL) {
        return createNode(name, age);
    }

    // Insere o novo nó na subárvore esquerda se a idade for menor que a idade do nó atual
    if (age < root->age) {
        root->left = insert(root->left, name, age);
    }
    // Insere o novo nó na subárvore direita se a idade for maior que a idade do nó atual
    else if (age > root->age) {
        root->right = insert(root->right, name, age);
    }
    // Se a idade for igual, não permite duplicatas
    else {
        return root;
    }

    // Atualiza a altura do nó atual
    root->height = 1 + max(height(root->left), height(root->right));

    // Verifica o fator de equilíbrio para determinar se a árvore está desbalanceada e requer rotações
    int balance = balanceFactor(root);

    // Casos de rotação para balancear a árvore
    if (balance > 1 && age < root->left->age) {
        return rotateRight(root);
    }
    if (balance < -1 && age > root->right->age) {
        return rotateLeft(root);
    }
    if (balance > 1 && age > root->left->age) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (balance < -1 && age < root->right->age) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    // Retorna o ponteiro do nó (inalterado)
    return root;
}

// Função auxiliar para encontrar o nó com o menor valor em uma subárvore
struct Node* findMinNode(struct Node* node) {
    struct Node* current = node;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

// Função para excluir um nó da árvore AVL
struct Node* deleteNode(struct Node* root, int age) {
    // Caso base: se a árvore estiver vazia
    if (root == NULL) {
        return root;
    }
    // Se a idade a ser excluída for menor que a idade da raiz, procuramos à esquerda
    if (age < root->age) {
        root->left = deleteNode(root->left, age);
    }
    // Se a idade a ser excluída for maior que a idade da raiz, procuramos à direita
    else if (age > root->age) {
        root->right = deleteNode(root->right, age);
    }
    // Se a idade for igual à idade da raiz, encontramos o nó a ser excluído
    else {
        // Nó com apenas um filho ou nenhum filho
        if (root->left == NULL) {
            struct Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct Node* temp = root->left;
            free(root);
            return temp;
        }

        // Nó com dois filhos: obtemos o sucessor (menor nó na subárvore direita)
        struct Node* temp = findMinNode(root->right);

        // Copiamos os dados do sucessor para este nó
        strcpy(root->name, temp->name);
        root->age = temp->age;

        // Excluímos o sucessor
        root->right = deleteNode(root->right, temp->age);
    }

    // Atualiza a altura do nó atual
    root->height = 1 + max(height(root->left), height(root->right));

    // Calcula o fator de equilíbrio do nó atual
    int balance = balanceFactor(root);

    // Casos de rotação para balancear a árvore
    if (balance > 1 && balanceFactor(root->left) >= 0) {
        return rotateRight(root);
    }
    if (balance > 1 && balanceFactor(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (balance < -1 && balanceFactor(root->right) <= 0) {
        return rotateLeft(root);
    }
    if (balance < -1 && balanceFactor(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

// Função para pesquisar um valor na árvore AVL
bool search(struct Node *root, int age) {
    if (root == NULL) {
        return false;
    }

    if (age < root->age) {
        return search(root->left, age);
    } else if (age > root->age) {
        return search(root->right, age);
    } else {
        return true;
    }
}

// Função para imprimir a árvore AVL
void printTree(struct Node *root, int space) {
    if (root == NULL)
        return;

    space += 10;

    printTree(root->right, space);

    printf("\n");
    for (int i = 10; i < space; i++)
        printf(" ");
    printf("%d\n", root->age);

    printTree(root->left, space);
}

// Função auxiliar para imprimir um array
void printArray(int array[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

// Função principal
int main() {
    struct Node *root = NULL;

    // Inserindo valores de um array em uma árvore AVL
    int age[] = {15, 27, 49, 10, 8, 67, 59, 9, 13, 20};
    int numage = sizeof(age) / sizeof(age[0]);
    for (int i = 0; i < numage; i++) {
        root = insert(root, "", age[i]); // Não temos nomes para inserir, então passamos uma string vazia
    }

    int escolha;
    int valor;

    do {
        printf("\n******** ALGORITMOS E ESTRUTURA DE DADOS *********\n");
        printf("-------- ÁRVORES AVL ---------------\n");
        printf("ÁRVORE AVL DEFAULT: ");
        printArray(age, numage);
        printf("\nMenu:\n");
        printf("1. Adicionar um nó\n");
        printf("2. Verificar se um valor existe na árvore\n");
        printf("3. Remover um nó\n");
        printf("4. Mostrar a árvore\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                printf("Digite o valor a ser adicionado: ");
                scanf("%d", &valor);
                root = insert(root, "", valor); // Não temos nomes para inserir, então passamos uma string vazia
                printf("Valor %d adicionado com sucesso.\n", valor);
                break;
            case 2:
                printf("Digite o valor a ser procurado: ");
                scanf("%d", &valor);
                if (search(root, valor)) {
                    printf("O valor %d está presente na árvore.\n", valor);
                } else {
                    printf("O valor %d não está presente na árvore.\n", valor);
                }
                break;
            case 3:
                printf("Digite o valor a ser removido: ");
                scanf("%d", &valor);
                root = deleteNode(root, valor);
                printf("Valor %d removido com sucesso.\n", valor);
                break;
            case 4:
                printTree(root, 0);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (escolha != 0);

    return 0;
}