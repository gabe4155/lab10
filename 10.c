#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie node structure
struct TrieNode
{
    struct TrieNode *children[26];
    int count;
};

// Trie structure
struct Trie
{	
    struct TrieNode *root;
};

// Function to create a new trie node
struct TrieNode *createNode()
{
    struct TrieNode *newNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (newNode)
    {
        newNode->count = 0;
        for (int i = 0; i < 26; i++)
            newNode->children[i] = NULL;
    }
    return newNode;
}

// Function to initialize a trie structure
struct Trie *createTrie()
{
    struct Trie *pTrie = (struct Trie *)malloc(sizeof(struct Trie));
    if (pTrie)
    {
        pTrie->root = createNode();
    }
    return pTrie;
}

// Function to insert a word into the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct TrieNode *current = pTrie->root;
    int len = strlen(word);
    for (int i = 0; i < len; i++)
    {
        int index = word[i] - 'a';
        if (!current->children[index])
            current->children[index] = createNode();
        current = current->children[index];
    }
    // Mark the last node as leaf node
    current->count++;
}

// Function to compute the number of occurrences of a word
int numberOfOccurrences(struct Trie *pTrie, char *word)
{
    struct TrieNode *current = pTrie->root;
    int len = strlen(word);
    for (int i = 0; i < len; i++)
    {
        int index = word[i] - 'a';
        if (!current->children[index])
            return 0; // Word not found
        current = current->children[index];
    }
    return current->count;
}

// Function to deallocate memory allocated for trie nodes
void deallocateTrieNodes(struct TrieNode *root)
{
    if (root)
    {
        for (int i = 0; i < 26; i++)
        {
            if (root->children[i])
                deallocateTrieNodes(root->children[i]);
        }
        free(root);
    }
}

// Function to deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (pTrie)
    {
        deallocateTrieNodes(pTrie->root);
        free(pTrie);
    }
    return NULL;
}

// Function to read the dictionary file and store words in an array
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        exit(1);
    }
    int numWords = 0;
    char word[256];
    while (fscanf(file, "%s", word) != EOF)
    {
        pInWords[numWords] = strdup(word);
        numWords++;
    }
    fclose(file);
    return numWords;
}

int main(void)
{
    char *inWords[256];
    
    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }
    
    // Create trie and insert words from the dictionary
    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Test with sample words
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    // Deallocate trie and check for errors
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");
    return 0;
}
