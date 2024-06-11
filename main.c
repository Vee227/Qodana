#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//structure for our nodes
struct node{
 int key;
 struct node *left;
 struct node *right;
};


//creating the node and allocating the storage
struct node* create(int value) {
  struct node* newNode = malloc(sizeof(struct node));
  newNode->key = value;
  newNode->left = NULL;
  newNode->right = NULL;

  return newNode;
}


//insert the new node on the left side of the root-node
struct node* insertLeft(struct node* root, int value) {
  root->left = create(value);
  return root->left;
}


//insert the new node on the right side of the root-node
struct node* insertRight(struct node* root, int value) {
  root->right = create(value);
  return root->right;
}


//function for calculating the height of the right and left subtrees
int h(struct node* root) {
    if (root == NULL) return 0;
    int leftHeight = h(root->left);
    int rightHeight = h(root->right);
    return (leftHeight > rightHeight) ? (leftHeight + 1) : (rightHeight + 1);
}


//function for calculation the balancefactor for every inserted node
int bal(struct node* root) {
    if (root == NULL) return 0;
    int rightHeight = h(root->right);
    int leftHeight = h(root->left);
    return (rightHeight - leftHeight);
}


//function to print the balancefactor for every node
void printBalance(struct node* root, int* counter) {
    if (root == NULL){
       return;
    }

    //Recursively go through the right subtree
    printBalance(root->right, counter);

    //Recursively go through the left subtree
    printBalance(root->left, counter);

    //Check if the current node violates the AVL rules
    printf("bal(%d) = %d", root->key, bal(root));
    if (bal(root) > 1 || bal(root) < -1) {
        printf(" (AVL violation!)");
        (*counter)++; //The counter is needed for printing at the end if the tree is an AVL tree or not
    }
    printf("\n");
}



//function to insert a new node with the given key into the tree
struct node* insert(struct node* root, int key) {
    if (root == NULL) {
        return create(key);
    }

    if (key < root->key) {
        root->left = insert(root->left, key);
    } else if (key > root->key) {
        root->right = insert(root->right, key);
    }
    //duplicate are ignored

    return root;
}


//function to find the minimum value
int findMin(struct node* root) {
    if (root == NULL) {
        printf("Empty tree!\n");
        return -1;
    }
    if (root->left == NULL) {
        return root->key;
    }
    return findMin(root->left);
}


//function to find the maximum value
int findMax(struct node* root) {
    if (root == NULL) {
        printf("Empty tree!\n");
        return -1;
    }
    if (root->right == NULL) {
        return root->key;
    }
    return findMax(root->right);
}


//function to calculate the sum of the values and the number of the nodes
void calculateSumAndCount(struct node* root, int* sum, int* count) {
    if (root == NULL) {
        return;
    }
    // Add the current value to the sum
    *sum += root->key;
    // Increase the count of nodes
    (*count)++;
    //Recursively check the left subtree and then the right subtree
    calculateSumAndCount(root->left, sum, count);
    calculateSumAndCount(root->right, sum, count);
}


// Function to calculate the average value of the binary search tree
float calculateAverage(struct node* root) {
    int sum = 0;
    int count = 0;
    calculateSumAndCount(root, &sum, &count); // Calculate sum and count recursively
    if (count == 0) {
        return 0; // Avoid division by zero
    }
    return (float)sum / count; // Calculate and return the average
}


//function to search for a specific key-value in the tree
struct node* search(struct node* root, struct node* keyNode, int* path, int depth) {
    if (root == NULL) {
        return NULL;
    }
    if (keyNode->key == root->key) {
        if (depth < 100) { //if the maximum depth of 100 is not reached yet
            path[depth] = root->key; //insert the found key on the index [depth]
        }
        return root;
    }
    if (keyNode->key < root->key) {
        if (depth < 100) {
            path[depth] = root->key;
        }
        return search(root->left, keyNode, path, depth + 1); //search for the value in the
        //left subtree because all the left values are smaller than the root-value
    }
    if (keyNode->key > root->key) {
        if(depth < 100){
            path[depth] = root->key;
        }
        return search(root->right, keyNode, path, depth + 1); //search recursively for the value in the
        //right subtree because all of the right values are greater than the root-value
    }

}


//function to check if a subtree is part of the original tree
int isSubtree(struct node* root, struct node* subtree) {
    // If the subtree is empty, it is always a subtree
    if (subtree == NULL) {
        return 1;
    }

    // If the main tree is empty, there is no subtree
    if (root == NULL) {
        return 0;
    }

    // Check if the current root node matches the root node of the subtree
    if (root->key == subtree->key) {

        // If the root matches, the function checks recursively if the subtree and the main tree are the same at a path, if so the subtree is for sure a part of the main tree
        //They also match if they are both NULL
        if (isSubtree(root->left, subtree->left) && isSubtree(root->right, subtree->right)) {
            return 1;
        }
    }

    // If the current root node isn't the same as the subtree root, the function checks recursively left and right if the other values in the main tree appear in the subtree
    // Either they eventually match and both end and the function returns positive
    // Or through the first two if statements the subtree ends before the maintree and is therefore a part
    // Or the maintree ends and the subtree still has values, therefore the subtree is not a part of the maintree
    int leftSubtreeMatch = isSubtree(root->left, subtree);
    int rightSubtreeMatch = isSubtree(root->right, subtree);
    return leftSubtreeMatch || rightSubtreeMatch;
}


//function to read a file and add the values to the tree with the insert-function
struct node* readTreeFromFile(const char* filename) {
    struct node* root = NULL;
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("\nError opening the file.");
        return NULL;
    }

    int input = 0;

    while (fscanf(file, "%d", &input) != EOF) {
        root = insert(root, input);
    }

    fclose(file);
    return root;
}


//Function to free the allocated memory
void freeAll(struct node* root) {
    if (root == NULL) //if the root is NULL, return
        return;

    // Recursively free the left and right subtrees
    freeAll(root->left);
    freeAll(root->right);

    // Free the current node
    free(root);
}


int main()
{
    struct node* root = NULL;

    int avlCounter = 0;
    int choice;
    char treefilename[100];
    char keyfilename[100];


    do {
        avlCounter = 0;
        printf("\nPlease choose an action:\n");
        printf("1) Check if the tree is AVL\n");
        printf("2) Find a key in the tree\n");
        printf("3) Find a subtree in the tree\n");
        printf("4) Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the filename: ");
                scanf("%s", treefilename);
                root = readTreeFromFile(treefilename);
                 if (root == NULL) {
                    break;
                }
                printBalance(root, &avlCounter);
                if(avlCounter > 0) {
                    printf("AVL: no\n");
                } else {
                    printf("AVL: yes\n");
                }

                int min = findMin(root);
                int max = findMax(root);
                float avg = calculateAverage(root);
                printf("min: %d, max: %d, avg: %.1f\n", min, max, avg);
                break;
            case 2:
                printf("Enter the filename for the tree: ");
                scanf("%s", treefilename);

                root = readTreeFromFile(treefilename);
                if (root == NULL) {
                    break;
                }

                printf("Enter the filename for the key: ");
                scanf("%s", keyfilename);

                struct node* keyNode = readTreeFromFile(keyfilename);
                if (keyNode == NULL) {
                    freeAll(root);
                    break;
                }

                int path[100]; //to save the path to the value we search for

                struct node* result = search(root, keyNode, path, 0);

                if (result != NULL) {
                    printf("%d found ", keyNode->key);
                    int i;
                    for (i = 0; i < 100; i++) {
                        if (path[i] == 0)
                            break;
                        printf("%d", path[i]);
                        if (path[i + 1] != 0)
                            printf(", ");
                    }
                    printf("\n");
                } else {
                    printf("%d not found!\n", keyNode->key);
                }
                break;
            case 3:
                 printf("Enter the filename for the main tree: ");
                scanf("%s", treefilename);
                root = readTreeFromFile(treefilename);
                if (root == NULL) {
                    break;
                }

                printf("Enter the filename for the subtree: ");
                scanf("%s", keyfilename);
                struct node* subroot = readTreeFromFile(keyfilename);
                if (subroot == NULL) {
                    printf("Subtree not found!");
                    break;
                }

                if (isSubtree(root, subroot)) {
                    printf("Subtree found!\n");
                } else {
                    printf("Subtree not found!\n");
                }

                freeAll(subroot);
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);



    freeAll(root);
    return 0;
}
