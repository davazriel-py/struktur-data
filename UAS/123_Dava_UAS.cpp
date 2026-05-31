#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>

using namespace std;

const int HASH_SIZE = 101; 
const char* DB_FILE    = "character_database.txt";
const char* INV_FILE   = "inventory.txt";
const char* GRAPH_FILE = "combo_graph.txt";
const char* ADMIN_PASS = "admin123";

struct Character {
    char name[64];
    char rarity[16];
    char element[32];
    int attack;
    int hp;
};

struct ListNode {
    Character data;
    ListNode* next;
    ListNode(Character c) : data(c), next(nullptr) {}
};

struct StackNode {
    Character data;
    StackNode* next;
    StackNode(Character c) : data(c), next(nullptr) {}
};

struct QueueNode {
    Character data;
    QueueNode* next;
    QueueNode(Character c) : data(c), next(nullptr) {}
};

struct Queue {
    QueueNode* front;
    QueueNode* rear;
    Queue() : front(nullptr), rear(nullptr) {}

    void enqueue(Character c) {
        QueueNode* node = new QueueNode(c);
        if (!rear) front = node;
        else rear->next = node;
        rear = node;
    }

    bool dequeue(Character& c) {
        if (!front) return false;
        c = front->data;
        QueueNode* tmp = front;
        front = front->next;
        if (!front) rear = nullptr;
        delete tmp;
        return true;
    }

    bool isEmpty() const {
        return front == nullptr;
    }
};

struct HashNode {
    Character data;
    HashNode* next;
    HashNode(Character c) : data(c), next(nullptr) {}
};

struct BSTNode {
    Character data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(Character c) : data(c), left(nullptr), right(nullptr) {}
};

struct GraphEdge {
    char target[64];
    GraphEdge* next;
    GraphEdge(const char* t) : next(nullptr) {
        strncpy(target, t, 63);
        target[63] = '\0';
    }
};

struct GraphNode {
    char name[64];
    GraphEdge* edges;
    GraphNode* next;
    GraphNode(const char* n) : edges(nullptr), next(nullptr) {
        strncpy(name, n, 63);
        name[63] = '\0';
    }
};

//linkedlist
struct LinkedList {
    ListNode* head;
    int size;
    LinkedList() : head(nullptr), size(0) {}

    void append(Character c) {
        ListNode* node = new ListNode(c);
        if (!head) { head = node; }
        else {
            ListNode* cur = head;
            while (cur->next) cur = cur->next;
            cur->next = node;
        }
        size++;
    }

    bool removeLast() {
        if (!head) return false;
        if (!head->next) {
            delete head;
            head = nullptr;
            size--;
            return true;
        }
        ListNode* cur = head;
        while (cur->next->next) cur = cur->next;
        delete cur->next;
        cur->next = nullptr;
        size--;
        return true;
    }

    void display() {
        if (!head) { cout << "  (kosong)\n"; return; }
        ListNode* cur = head;
        int i = 1;
        while (cur) {
            cout << "  " << i++ << ". " << cur->data.name
                 << " | " << cur->data.rarity
                 << " | " << cur->data.element
                 << " | ATK " << cur->data.attack
                 << " | HP " << cur->data.hp << "\n";
            cur = cur->next;
        }
    }

    void toArray(Character* arr, int& n) {
        n = 0;
        ListNode* cur = head;
        while (cur && n < 500) {
            arr[n++] = cur->data;
            cur = cur->next;
        }
    }
};

//stack
struct Stack {
    StackNode* top;
    int size;
    Stack() : top(nullptr), size(0) {}

    void push(Character c) {
        StackNode* node = new StackNode(c);
        node->next = top;
        top = node;
        size++;
    }

    bool pop(Character& c) {
        if (!top) return false;
        c = top->data;
        StackNode* tmp = top;
        top = top->next;
        delete tmp;
        size--;
        return true;
    }

    bool isEmpty() { return top == nullptr; }
};

//hashtable
struct HashTable {
    HashNode* table[HASH_SIZE];
    HashTable() {
        for (int i = 0; i < HASH_SIZE; i++) table[i] = nullptr;
    }

    int hash(const char* key) {
        int sum = 0;
        for (int i = 0; key[i] != '\0'; i++)
            sum += (int)key[i];
        return sum % HASH_SIZE;
    }

    void insert(Character c) {
        int idx = hash(c.name);
        HashNode* node = new HashNode(c);
        node->next = table[idx];
        table[idx] = node;
    }

    Character* search(const char* name) {
        int idx = hash(name);
        HashNode* cur = table[idx];
        while (cur) {
            if (strcmp(cur->data.name, name) == 0)
                return &cur->data;
            cur = cur->next;
        }
        return nullptr;
    }

    Character* searchIgnoreCase(const char* name) {
        for (int i = 0; i < HASH_SIZE; i++) {
            HashNode* cur = table[i];
            while (cur) {
                int j = 0;
                bool same = true;
                while (cur->data.name[j] != '\0' && name[j] != '\0') {
                    char a = cur->data.name[j]; if (a >= 'A' && a <= 'Z') a += 'a' - 'A';
                    char b = name[j]; if (b >= 'A' && b <= 'Z') b += 'a' - 'A';
                    if (a != b) { same = false; break; }
                    j++;
                }
                if (same && cur->data.name[j] == '\0' && name[j] == '\0')
                    return &cur->data;
                cur = cur->next;
            }
        }
        return nullptr;
    }

    void remove(const char* name) {
        int idx = hash(name);
        HashNode* cur = table[idx];
        HashNode* prev = nullptr;
        while (cur) {
            if (strcmp(cur->data.name, name) == 0) {
                if (prev) prev->next = cur->next;
                else table[idx] = cur->next;
                delete cur;
                return;
            }
            prev = cur;
            cur = cur->next;
        }
    }
};

//bst
struct BST {
    BSTNode* root;
    BST() : root(nullptr) {}

    BSTNode* insertRec(BSTNode* node, Character c) {
        if (!node) return new BSTNode(c);
        if (strcmp(c.name, node->data.name) < 0)
            node->left = insertRec(node->left, c);
        else if (strcmp(c.name, node->data.name) > 0)
            node->right = insertRec(node->right, c);
        return node;
    }

    void insert(Character c) {
        root = insertRec(root, c);
    }

    void inorder(BSTNode* node) {
        if (!node) return;
        inorder(node->left);
        cout << "  " << node->data.name
             << " | " << node->data.rarity
             << " | " << node->data.element
             << " | ATK " << node->data.attack
             << " | HP " << node->data.hp << "\n";
        inorder(node->right);
    }
};

bool equalsIgnoreCase(const char* a, const char* b);

//graph
struct Graph {
    GraphNode* head;
    Graph() : head(nullptr) {}

    GraphNode* findOrCreate(const char* name) {
        GraphNode* cur = head;
        while (cur) {
            if (strcmp(cur->name, name) == 0) return cur;
            cur = cur->next;
        }
        GraphNode* node = new GraphNode(name);
        node->next = head;
        head = node;
        return node;
    }

    void addEdge(const char* a, const char* b) {
        GraphNode* na = findOrCreate(a);
        GraphNode* nb = findOrCreate(b);

        bool exists = false;
        GraphEdge* e = na->edges;
        while (e) {
            if (strcmp(e->target, b) == 0) {
                exists = true;
                break;
            }
            e = e->next;
        }
        if (!exists) {
            GraphEdge* ea = new GraphEdge(b);
            ea->next = na->edges;
            na->edges = ea;
        }
        exists = false;
        e = nb->edges;
        while (e) {
            if (strcmp(e->target, a) == 0) {
                exists = true;
                break;
            }
            e = e->next;
        }
        if (!exists) {
            GraphEdge* eb = new GraphEdge(a);
            eb->next = nb->edges;
            nb->edges = eb;
        }
    }

    void display() {
        if (!head) { cout << "  (tidak ada relasi combo)\n"; return; }
        GraphNode* cur = head;
        while (cur) {
            cout << "  " << cur->name << " --> ";
            GraphEdge* e = cur->edges;
            if (!e) { cout << "(tidak ada relasi)\n"; }
            else {
                while (e) {
                    cout << e->target;
                    if (e->next) cout << ", ";
                    e = e->next;
                }
                cout << "\n";
            }
            cur = cur->next;
        }
    }

    void dfsRec(const char* name, char visited[][64], int& visCount) {
        for (int i = 0; i < visCount; i++)
            if (strcmp(visited[i], name) == 0) return;
        strncpy(visited[visCount], name, 63);
        visited[visCount][63] = '\0';
        visCount++;

        cout << "  [DFS] " << name << "\n";

        GraphNode* node = nullptr;
        GraphNode* cur = head;
        while (cur) {
            if (equalsIgnoreCase(cur->name, name)) { node = cur; break; }
            cur = cur->next;
        }
        if (!node) return;

        GraphEdge* e = node->edges;
        while (e) {
            dfsRec(e->target, visited, visCount);
            e = e->next;
        }
    }

    void dfsDisplayRec(const char* name, char visited[][64], int& visCount) {
        for (int i = 0; i < visCount; i++)
            if (strcmp(visited[i], name) == 0) return;
        strncpy(visited[visCount], name, 63);
        visited[visCount][63] = '\0';
        visCount++;

        cout << "  " << name << "\n";

        GraphNode* node = nullptr;
        GraphNode* cur = head;
        while (cur) {
            if (equalsIgnoreCase(cur->name, name)) { node = cur; break; }
            cur = cur->next;
        }
        if (!node) return;

        GraphEdge* e = node->edges;
        while (e) {
            dfsDisplayRec(e->target, visited, visCount);
            e = e->next;
        }
    }

    void dfsDisplay(const char* startName, char visited[][64], int& visCount) {
        bool found = false;
        GraphNode* cur = head;
        while (cur) {
            if (equalsIgnoreCase(cur->name, startName)) { found = true; break; }
            cur = cur->next;
        }
        if (!found) return;
        dfsDisplayRec(startName, visited, visCount);
    }
};

bool equalsIgnoreCase(const char* a, const char* b);

struct CharDB {
    Character chars[500];
    int count;
    CharDB() : count(0) {}

    void add(Character c) {
        if (count < 500) chars[count++] = c;
    }

    void display() {
        if (count == 0) { cout << "  (database kosong)\n"; return; }
        for (int i = 0; i < count; i++) {
            cout << "  " << i+1 << ". " << chars[i].name
                 << " | " << chars[i].rarity
                 << " | " << chars[i].element
                 << " | ATK " << chars[i].attack
                 << " | HP " << chars[i].hp << "\n";
        }
    }

    bool findByNameIgnoreCase(const char* name, Character& out) {
        for (int i = 0; i < count; i++) {
            if (equalsIgnoreCase(chars[i].name, name)) {
                out = chars[i];
                return true;
            }
        }
        return false;
    }
};


struct Stats {
    int totalSummon;
    int total5Star;
    int total4Star;
    Stats() : totalSummon(0), total5Star(0), total4Star(0) {}
};


LinkedList inventory;
Stack undoStack;
HashTable hashTable;
BST bst;
Graph graph;
CharDB charDB;
Stats stats;
Queue summonHistory;
int summonHistorySize = 0;
const int MAX_HISTORY = 10;

void printLine(char c = '-', int len = 50) {
    for (int i = 0; i < len; i++) cout << c;
    cout << "\n";
}

void printTitle(const char* title) {
    printLine('=');
    size_t len = strlen(title);
    int pad = (50 - (int)len) / 2;
    for (int i = 0; i < pad; i++) cout << " ";
    cout << title << "\n";
    printLine('=');
}

//queue
void addToSummonHistory(Character c) {
    if (summonHistorySize >= MAX_HISTORY) {
        Character removed;
        summonHistory.dequeue(removed);
        summonHistorySize--;
    }
    summonHistory.enqueue(c);
    summonHistorySize++;
}

void clearScreen() {
    for (int i = 0; i < 3; i++) cout << "\n";
}

//queue
void menuSummonHistory() {
    clearScreen();
    printTitle("SUMMON HISTORY");
    cout << "\n  == SUMMON HISTORY ==\n";

    if (summonHistory.isEmpty()) {
        cout << "  (tidak ada history summon)\n";
    } else {
        Queue temp;
        int idx = 1;
        while (!summonHistory.isEmpty()) {
            Character c;
            summonHistory.dequeue(c);
            cout << "  " << idx++ << ". " << c.name << " | " << c.rarity << " | " << c.element << "\n";
            temp.enqueue(c);
        }
        while (!temp.isEmpty()) {
            Character c;
            temp.dequeue(c);
            summonHistory.enqueue(c);
        }
    }

    cout << "\n  Tekan ENTER untuk kembali...";
    cin.ignore(1000, '\n');
    cin.get();
}

void readLine(char* buf, int maxLen) {
    cin.getline(buf, maxLen);
}

void saveInventory() {
    ofstream f(INV_FILE);
    if (!f) return;
    ListNode* cur = inventory.head;
    while (cur) {
        f << cur->data.name << "|"
          << cur->data.rarity << "|"
          << cur->data.element << "|"
          << cur->data.attack << "|"
          << cur->data.hp << "\n";
        cur = cur->next;
    }
    f.close();
}


void saveCharDB() {
    ofstream f(DB_FILE);
    if (!f) return;
    for (int i = 0; i < charDB.count; i++) {
        f << charDB.chars[i].name << "|"
          << charDB.chars[i].rarity << "|"
          << charDB.chars[i].element << "|"
          << charDB.chars[i].attack << "|"
          << charDB.chars[i].hp << "\n";
    }
    f.close();
}

bool parseLine(const char* line, Character& c) {
    char buf[256];
    strncpy(buf, line, 255);
    buf[255] = '\0';

    char* tok = strtok(buf, "|");
    if (!tok) return false;
    strncpy(c.name, tok, 63); c.name[63] = '\0';

    tok = strtok(nullptr, "|");
    if (!tok) return false;
    strncpy(c.rarity, tok, 15); c.rarity[15] = '\0';

    tok = strtok(nullptr, "|");
    if (!tok) return false;
    strncpy(c.element, tok, 31); c.element[31] = '\0';

    tok = strtok(nullptr, "|");
    if (tok) c.attack = atoi(tok);
    else c.attack = 0;

    tok = strtok(nullptr, "|");
    if (tok) c.hp = atoi(tok);
    else c.hp = 0;

    return true;
}

void loadCharDB() {
    ifstream f(DB_FILE);
    if (!f) return;
    char line[256];
    while (f.getline(line, 255)) {
        Character c;
        if (parseLine(line, c)) {
            charDB.add(c);
            bst.insert(c);
        }
    }
    f.close();
}

void loadInventory() {
    ifstream f(INV_FILE);
    if (!f) return;
    char line[256];
    while (f.getline(line, 255)) {
        Character c;
        if (parseLine(line, c)) {
            inventory.append(c);
            hashTable.insert(c);
            undoStack.push(c);

            if (strcmp(c.rarity, "5 Star") == 0) stats.total5Star++;
            else stats.total4Star++;
            stats.totalSummon++;
        }
    }
    f.close();
}


bool parseGraphLine(const char* line, char* a, char* b) {
    char buf[256];
    strncpy(buf, line, 255);
    buf[255] = '\0';

    char* tok = strtok(buf, "|");
    if (!tok) return false;
    strncpy(a, tok, 63); a[63] = '\0';

    tok = strtok(nullptr, "|");
    if (!tok) return false;
    size_t len = strlen(tok);
    if (len > 0 && tok[len-1] == '\n') tok[len-1] = '\0';
    if (len > 1 && tok[len-2] == '\r') tok[len-2] = '\0';
    strncpy(b, tok, 63); b[63] = '\0';

    return true;
}

void saveGraph() {
    ofstream f(GRAPH_FILE);
    if (!f) return;
    GraphNode* cur = graph.head;
    while (cur) {
        GraphEdge* e = cur->edges;
        while (e) {
            if (strcmp(cur->name, e->target) < 0) {
                f << cur->name << "|" << e->target << "\n";
            }
            e = e->next;
        }
        cur = cur->next;
    }
    f.close();
}

void loadGraph() {
    ifstream f(GRAPH_FILE);
    if (!f) return;
    char line[256];
    while (f.getline(line, 255)) {
        char a[64], b[64];
        if (parseGraphLine(line, a, b)) {
            graph.addEdge(a, b);
        }
    }
    f.close();
}

void initDefaultGraph() {
    bool isEmpty = true;
    ifstream f(GRAPH_FILE);
    if (f) {
        char line[256];
        while (f.getline(line, 255)) {
            size_t len = strlen(line);
            if (len > 0 && line[len-1] == '\r') line[len-1] = '\0';
            if (strlen(line) > 0) {
                isEmpty = false;
                break;
            }
        }
        f.close();
    }
}

void doSummon() {
    if (charDB.count == 0) {
        cout << "\n  [!] Database karakter kosong. Tambahkan via Admin Menu.\n";
        return;
    }

    Character fiveStar[100]; int fiveCount = 0;
    Character fourStar[200]; int fourCount = 0;

    for (int i = 0; i < charDB.count; i++) {
        if (strcmp(charDB.chars[i].rarity, "5 Star") == 0 && fiveCount < 100)
            fiveStar[fiveCount++] = charDB.chars[i];
        else if (strcmp(charDB.chars[i].rarity, "4 Star") == 0 && fourCount < 200)
            fourStar[fourCount++] = charDB.chars[i];
    }

    int roll = rand() % 100;
    Character picked;
    bool got5Star = false;

    if (roll < 10 && fiveCount > 0) {
        picked = fiveStar[rand() % fiveCount];
        got5Star = true;
    } else if (fourCount > 0) {
        picked = fourStar[rand() % fourCount];
    } else if (fiveCount > 0) {
        picked = fiveStar[rand() % fiveCount];
        got5Star = true;
    } else {
        cout << "\n  [!] Tidak ada character di database.\n";
        return;
    }

    cout << "\n";
    printLine('*');
    cout << "  * SUMMON *\n";
    cout << "  Swirling wishes...\n";
    printLine('*');
    cout << "\n";
    if (got5Star) {
        cout << "  *** SELAMAT! *** Kamu mendapatkan:\n";
    } else {
        cout << "  Kamu mendapatkan:\n";
    }
    cout << "  >> " << picked.name
         << " | " << picked.rarity
         << " | " << picked.element
         << " | ATK " << picked.attack
         << " | HP " << picked.hp << "\n";

    inventory.append(picked);
    undoStack.push(picked);
    hashTable.insert(picked);
    addToSummonHistory(picked);

    stats.totalSummon++;
    if (got5Star) stats.total5Star++;
    else          stats.total4Star++;

    // Simpan ke file
    saveInventory();

    cout << "\n";
}

void swapChar(Character& a, Character& b) {
    Character tmp = a;
    a = b;
    b = tmp;
}

void sortByName(Character* arr, int n) {
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++)
            if (strcmp(arr[j].name, arr[j+1].name) > 0)
                swapChar(arr[j], arr[j+1]);
}

void sortByRarity(Character* arr, int n) {
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++)
            if (strcmp(arr[j].rarity, arr[j+1].rarity) < 0)
                swapChar(arr[j], arr[j+1]);
}

void displayArray(Character* arr, int n) {
    if (n == 0) { cout << "  (kosong)\n"; return; }
    for (int i = 0; i < n; i++) {
        cout << "  " << i+1 << ". " << arr[i].name
             << " | " << arr[i].rarity
             << " | " << arr[i].element
             << " | ATK " << arr[i].attack
             << " | HP " << arr[i].hp << "\n";
    }
}

char toLowerChar(char c) {
    if (c >= 'A' && c <= 'Z') return c + ('a' - 'A');
    return c;
}

bool stringContainsIgnoreCase(const char* haystack, const char* needle) {
    if (!haystack || !needle) return false;
    size_t lenHay = strlen(haystack);
    size_t lenNeedle = strlen(needle);
    if (lenNeedle == 0 || lenHay == 0) return false;

    char lowerHay[128];
    char lowerNeedle[128];
    size_t i;

    for (i = 0; i < lenHay && i < 127; i++)
        lowerHay[i] = toLowerChar(haystack[i]);
    lowerHay[i] = '\0';

    for (i = 0; i < lenNeedle && i < 127; i++)
        lowerNeedle[i] = toLowerChar(needle[i]);
    lowerNeedle[i] = '\0';

    return strstr(lowerHay, lowerNeedle) != nullptr;
}

bool equalsIgnoreCase(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        char ca = a[i]; if (ca >= 'A' && ca <= 'Z') ca += 'a' - 'A';
        char cb = b[i]; if (cb >= 'A' && cb <= 'Z') cb += 'a' - 'A';
        if (ca != cb) return false;
        i++;
    }
    return a[i] == '\0' && b[i] == '\0';
}

bool nameAlreadyAdded(const char names[][64], int count, const char* name) {
    for (int i = 0; i < count; i++) {
        if (equalsIgnoreCase(names[i], name)) return true;
    }
    return false;
}

//sorting
void sortByAttack(Character* arr, int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j].attack < arr[j+1].attack) {
                swapChar(arr[j], arr[j+1]);
            }
        }
    }
}

//searching
void searchInventoryByKeyword(const char* keyword, Character* results, int& resultCount) {
    resultCount = 0;
    if (strlen(keyword) == 0) return;

    char addedNames[500][64];
    int addedCount = 0;

    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode* cur = hashTable.table[i];
        while (cur) {
            if (stringContainsIgnoreCase(cur->data.name, keyword)) {
                if (!nameAlreadyAdded(addedNames, addedCount, cur->data.name)) {
                    if (resultCount < 500) {
                        results[resultCount++] = cur->data;
                        if (addedCount < 500) {
                            strncpy(addedNames[addedCount], cur->data.name, 63);
                            addedNames[addedCount][63] = '\0';
                            addedCount++;
                        }
                    }
                }
            }
            cur = cur->next;
        }
    }
}

void menuSearchCharacter() {
    clearScreen();
    printTitle("SEARCH CHARACTER");
    cout << "\n  Masukkan keyword: ";

    cin.ignore(1000, '\n');

    char keyword[64];
    readLine(keyword, 64);

    cout << "\n  == SEARCH RESULT ==\n";
    if (strlen(keyword) == 0) {
        cout << "  Character tidak ditemukan.\n";
        cout << "\n  Tekan ENTER untuk kembali...";
        cin.get();
        return;
    }

    Character* exactMatch = hashTable.searchIgnoreCase(keyword);
    if (exactMatch) {
        cout << "  Exact match found:\n";
        cout << "    " << exactMatch->name
             << " | " << exactMatch->rarity
             << " | " << exactMatch->element
             << " | ATK " << exactMatch->attack
             << " | HP " << exactMatch->hp << "\n";
    } else {
        Character matches[500];
        int matchCount = 0;
        searchInventoryByKeyword(keyword, matches, matchCount);
        if (matchCount == 0) {
            cout << "  Character tidak ditemukan.\n";
        } else {
            for (int i = 0; i < matchCount; i++) {
                cout << "  " << i+1 << ". " << matches[i].name
                     << " | " << matches[i].rarity
                     << " | " << matches[i].element
                     << " | ATK " << matches[i].attack
                     << " | HP " << matches[i].hp << "\n";
            }
        }
    }

    cout << "\n  Tekan ENTER untuk kembali...";
    cin.get();
}

void displayInventoryByDateRec(ListNode* node, int& idx) {
    if (!node) return;
    displayInventoryByDateRec(node->next, idx);
    cout << "  " << idx++ << ". " << node->data.name
         << " | " << node->data.rarity
         << " | " << node->data.element
         << " | ATK " << node->data.attack
         << " | HP " << node->data.hp << "\n";
}

void displayInventoryByDate() {
    if (!inventory.head) {
        cout << "  (inventory kosong)\n";
        return;
    }
    int idx = 1;
    displayInventoryByDateRec(inventory.head, idx);
}

void collectArchiveInOrder(BSTNode* node, Character* arr, int& n) {
    if (!node) return;
    collectArchiveInOrder(node->left, arr, n);
    if (n < 500) arr[n++] = node->data;
    collectArchiveInOrder(node->right, arr, n);
}

void displayArchiveByElement() {
    Character arr[500];
    int n = 0;
    collectArchiveInOrder(bst.root, arr, n);

    const char* elements[] = {"Pyro", "Hydro", "Electro", "Cryo", "Geo", "Anemo", "Dendro"};
    for (int g = 0; g < 7; g++) {
        const char* element = elements[g];
        bool printed = false;
        for (int i = 0; i < n; i++) {
            if (strcmp(arr[i].element, element) == 0) {
                if (!printed) {
                    cout << "\n===== " << element << " =====\n";
                    printed = true;
                }
                cout << "  " << arr[i].name
                     << " | " << arr[i].rarity
                     << " | " << arr[i].element
                     << " | ATK " << arr[i].attack
                     << " | HP " << arr[i].hp << "\n";
            }
        }
    }
}

void displayArchiveByRarity() {
    Character arr[500];
    int n = 0;
    collectArchiveInOrder(bst.root, arr, n);

    const char* rarities[] = {"5 Star", "4 Star"};
    for (int g = 0; g < 2; g++) {
        const char* rarity = rarities[g];
        bool printed = false;
        for (int i = 0; i < n; i++) {
            if (strcmp(arr[i].rarity, rarity) == 0) {
                if (!printed) {
                    cout << "\n===== " << rarity << " =====\n";
                    printed = true;
                }
                cout << "  " << arr[i].name
                     << " | " << arr[i].rarity
                     << " | " << arr[i].element
                     << " | ATK " << arr[i].attack
                     << " | HP " << arr[i].hp << "\n";
            }
        }
    }
}

void clearScreen();
void menuSummon10x();

void menuSummonSingle() {
    printTitle("SUMMON 1x");
    cout << "\n  Tekan ENTER untuk summon...\n";
    cin.ignore(1000, '\n');
    cin.get();
    doSummon();
    cout << "\n  Tekan ENTER untuk kembali...";
    cin.get();
}

void menuSummon() {
    while (true) {
        clearScreen();
        printTitle("SUMMON CHARACTER");
        cout << "\n";
        cout << "  1. Summon 1x\n";
        cout << "  2. Summon 10x\n";
        cout << "  3. Back\n";
        printLine();
        cout << "\n  Pilih submenu: ";
        
        int pilih;
        cin >> pilih;
        
        switch (pilih) {
            case 1:
                menuSummonSingle();
                break;
            case 2:
                menuSummon10x();
                break;
            case 3:
                return;
            default:
                cout << "\n  Pilihan tidak valid.\n";
                cin.ignore(1000, '\n');
                cin.get();
        }
    }
}

void menuCharacterArchive() {
    printTitle("CHARACTER ARCHIVE");

    if (!bst.root) {
        cout << "\n  Archive kosong. Tambahkan karakter ke database dahulu.\n";
        cout << "\n  Tekan ENTER untuk kembali...";
        cin.ignore(1000, '\n');
        cin.get();
        return;
    }

    cout << "\n  1. View A-Z\n";
    cout << "  2. View by Element\n";
    cout << "  3. View by Rarity\n";
    cout << "  4. Back\n";
    cout << "\n  Pilih: ";
    int pilih;
    cin >> pilih;

    cout << "\n";
    switch (pilih) {
        case 1:
            cout << "  -- Character A-Z --\n";
            bst.inorder(bst.root);
            break;
        case 2:
            cout << "  -- Character by Element --\n";
            displayArchiveByElement();
            break;
        case 3:
            cout << "  -- Character by Rarity --\n";
            displayArchiveByRarity();
            break;
        case 4:
            return;
        default:
            cout << "  Pilihan tidak valid.\n";
    }

    cout << "\n  Tekan ENTER untuk kembali...";
    cin.ignore(1000, '\n');
    cin.get();
}

void menuCharacterCombo() {
    printTitle("CHARACTER COMBO");
    cout << "\n  1. View All Combo\n";
    cout << "  2. Explore Combo Team\n";
    cout << "\n  Pilih: ";
    int pilih;
    cin >> pilih;

    switch (pilih) {
        case 1: {
            cout << "\n  == All Combo Relationships ==\n";
            graph.display();
            break;
        }
        case 2: {
            cout << "\n  Character Name: ";

            cin.ignore(1000, '\n');

            char name[64];
            readLine(name, 64);
            
            if (strlen(name) > 0) {
                bool found = false;
                GraphNode* cur = graph.head;
                while (cur) {
                    if (equalsIgnoreCase(cur->name, name)) { found = true; break; }
                    cur = cur->next;
                }
                if (!found) {
                    cout << "\n  [!] Character tidak ditemukan di combo database.\n";
                } else {
                    if (pilih == 2) {
                        cout << "\n  -- Combo Team for " << cur->name << " --\n";
                        char visited[500][64];
                        int visCount = 0;
                        graph.dfsDisplay(cur->name, visited, visCount);
                    }
                }
            }
            break;
        }
        default:
            cout << "\n  Pilihan tidak valid.\n";
    }

    cout << "\n  Tekan ENTER untuk kembali...";
    cin.ignore(1000, '\n');
    cin.get();
}

//stack
void menuUndoSummon() {
    printTitle("UNDO LAST SUMMON");

    if (undoStack.isEmpty()) {
        cout << "\n  Stack kosong. Tidak ada summon untuk di-undo.\n";
        cout << "\n  Tekan ENTER untuk kembali...";
        cin.ignore(1000, '\n');
        cin.get();
        return;
    }

    Character last;
    undoStack.pop(last);

    cout << "\n  Menghapus: " << last.name
         << " | " << last.rarity
         << " | " << last.element
         << " | ATK " << last.attack
         << " | HP " << last.hp << "\n";

    inventory.removeLast();

    hashTable.remove(last.name);

    stats.totalSummon--;
    if (strcmp(last.rarity, "5 Star") == 0) stats.total5Star--;
    else stats.total4Star--;

    saveInventory();

    cout << "  Undo berhasil!\n";
    cout << "\n  Tekan ENTER untuk kembali...";
    cin.ignore(1000, '\n');
    cin.get();
}

void menuInventory() {
    int pilih = 0;
    while (true) {
        clearScreen();
        printTitle("INVENTORY");
        cout << "\n  1. Sort by Date\n";
        cout << "  2. Sort by Name\n";
        cout << "  3. Sort by Rarity\n";
        cout << "  4. Sort by Attack\n";
        cout << "  5. Search Character\n";
        cout << "  6. Strongest Character\n";
        cout << "  7. Back\n";
        cout << "\n  Pilih: ";
        cin >> pilih;

        switch (pilih) {
            case 1:
                clearScreen();
                printTitle("SORTED BY DATE");
                cout << "\n";
                displayInventoryByDate();
                cout << "\n  Tekan ENTER untuk kembali...";
                cin.ignore(1000, '\n');
                cin.get();
                break;
            case 2: {
                Character arr[500];
                int n;
                inventory.toArray(arr, n);
                clearScreen();
                printTitle("SORTED BY NAME");
                cout << "\n";
                if (n == 0) {
                    cout << "  Inventory kosong.\n";
                } else {
                    sortByName(arr, n);
                    displayArray(arr, n);
                }
                cout << "\n  Tekan ENTER untuk kembali...";
                cin.ignore(1000, '\n');
                cin.get();
                break;
            }
            case 3: {
                Character arr[500];
                int n;
                inventory.toArray(arr, n);
                clearScreen();
                printTitle("SORTED BY RARITY");
                cout << "\n";
                if (n == 0) {
                    cout << "  Inventory kosong.\n";
                } else {
                    sortByRarity(arr, n);
                    displayArray(arr, n);
                }
                cout << "\n  Tekan ENTER untuk kembali...";
                cin.ignore(1000, '\n');
                cin.get();
                break;
            }
            case 4: {
                Character arr[500];
                int n;
                inventory.toArray(arr, n);
                clearScreen();
                printTitle("SORTED BY ATTACK");
                cout << "\n";
                if (n == 0) {
                    cout << "  Inventory kosong.\n";
                } else {
                    sortByAttack(arr, n);
                    displayArray(arr, n);
                }
                cout << "\n  Tekan ENTER untuk kembali...";
                cin.ignore(1000, '\n');
                cin.get();
                break;
            }
            case 5:
                menuSearchCharacter();
                break;
            case 6: {
                clearScreen();
                printTitle("STRONGEST CHARACTER");
                Character best;
                bool found = false;
                ListNode* cur = inventory.head;
                while (cur) {
                    if (!found || cur->data.attack > best.attack) {
                        best = cur->data;
                        found = true;
                    }
                    cur = cur->next;
                }
                if (!found) {
                    cout << "  Inventory kosong.\n";
                } else {
                    cout << "  " << best.name
                         << " | " << best.rarity
                         << " | " << best.element
                         << " | ATK " << best.attack
                         << " | HP " << best.hp << "\n";
                }
                cout << "\n  Tekan ENTER untuk kembali...";
                cin.ignore(1000, '\n');
                cin.get();
                break;
            }
            case 7:
                return;
            default:
                cout << "\n  Pilihan tidak valid.\n";
                cin.ignore(1000, '\n');
                cin.get();
        }
    }
}

void menuCharacterFeatures() {
    int pilih = 0;
    while (true) {
        clearScreen();
        printTitle("CHARACTER FEATURES");
        cout << "\n";
        cout << "  1. Character Archive\n";
        cout << "  2. Character Combo\n";
        cout << "  3. Back\n";
        cout << "\n  Pilih: ";
        cin >> pilih;

        switch (pilih) {
            case 1:
                menuCharacterArchive();
                break;
            case 2:
                menuCharacterCombo();
                break;
            case 3:
                return;
            default:
                cout << "\n  Pilihan tidak valid.\n";
                cin.ignore(1000, '\n');
                cin.get();
        }
    }
}

void menuStatistics() {
    int pilih = 0;
    while (true) {
        clearScreen();
        printTitle("STATISTICS");
        cout << "\n  1. View Statistics\n";
        cout << "  2. Summon History\n";
        cout << "  3. Back\n";
        cout << "\n  Pilih: ";
        cin >> pilih;

        switch (pilih) {
            case 1: {
                clearScreen();
                printTitle("STATISTICS");
                cout << "\n";
                cout << "  Total Summon  : " << stats.totalSummon << "\n";
                cout << "  Total 5 Star  : " << stats.total5Star << "\n";
                cout << "  Total 4 Star  : " << stats.total4Star << "\n";

                if (inventory.head) {
                    int totalAtk = 0;
                    int totalHp = 0;
                    int count = 0;
                    Character best;
                    ListNode* cur = inventory.head;
                    while (cur) {
                        totalAtk += cur->data.attack;
                        totalHp += cur->data.hp;
                        if (count == 0 || cur->data.attack > best.attack) {
                            best = cur->data;
                        }
                        count++;
                        cur = cur->next;
                    }
                    cout << "  Highest ATK   : " << best.name
                         << " | ATK " << best.attack
                         << " | HP " << best.hp << "\n";
                    cout << "  Avg Attack    : " << (count > 0 ? (totalAtk / count) : 0) << "\n";
                    cout << "  Avg HP        : " << (count > 0 ? (totalHp / count) : 0) << "\n";
                } else {
                    cout << "  Highest ATK   : -\n";
                    cout << "  Avg Attack    : 0\n";
                    cout << "  Avg HP        : 0\n";
                }

                // Hitung persentase
                if (stats.totalSummon > 0) {
                    double pct5 = (double)stats.total5Star / stats.totalSummon * 100.0;
                    double pct4 = (double)stats.total4Star / stats.totalSummon * 100.0;
                    cout << "\n  Rate 5 Star   : " << (int)pct5 << "%\n";
                    cout << "  Rate 4 Star   : " << (int)pct4 << "%\n";
                }
                cout << "\n  Tekan ENTER untuk kembali...";
                cin.ignore(1000, '\n');
                cin.get();
                break;
            }
            case 2:
                menuSummonHistory();
                break;
            case 3:
                return;
            default:
                cout << "\n  Pilihan tidak valid.\n";
                cin.ignore(1000, '\n');
                cin.get();
                break;
        }
    }
}

void adminAddCharacter() {
    printTitle("ADD CHARACTER");
    char name[64], rarity[16], element[32];

    cout << "\n  Nama    : ";
    readLine(name, 64);

    cout << "  Rarity  : ";
    cout << "\n    1. 5 Star\n    2. 4 Star\n  Pilih: ";
    int r;
    cin >> r;
    if (r == 1) strncpy(rarity, "5 Star", 15);
    else        strncpy(rarity, "4 Star", 15);

    cout << "  Element : ";
    readLine(element, 32);

    Character c;
    strncpy(c.name,    name,    63);
    strncpy(c.rarity,  rarity,  15);
    strncpy(c.element, element, 31);
    c.name[63] = c.rarity[15] = c.element[31] = '\0';

    cout << "  Attack : ";
    cin >> c.attack;

    cout << "  HP     : ";
    cin >> c.hp;

    charDB.add(c);
    bst.insert(c);
    saveCharDB();

    cout << "\n  Character berhasil ditambahkan!\n";
    cout << "\n  Tekan ENTER untuk kembali...";
    cin.ignore(1000, '\n');
    cin.get();
}

void adminAddCharacterCombo() {
    printTitle("ADD CHARACTER COMBO");
    char a[64], b[64];

    cout << "\n  Character 1: ";
    readLine(a, 64);
    cout << "  Character 2: ";
    readLine(b, 64);

    if (strlen(a) == 0 || strlen(b) == 0) {
        cout << "\n  [!] Input tidak boleh kosong.\n";
    } else {
        graph.addEdge(a, b);
        saveGraph();
        cout << "\n  Combo berhasil ditambahkan dan disimpan!\n";
    }

    cout << "\n  Tekan ENTER untuk kembali...";
    cin.ignore(1000, '\n');
    cin.get();
}


void adminMenu() {
    int pilih = 0;
    while (true) {
        clearScreen();
        printTitle("ADMIN MENU");
        cout << "\n";
        cout << "  1. Add Character\n";
        cout << "  2. Add Character Combo\n";
        cout << "  3. Undo Last Summon\n";
        cout << "  4. Back\n";
        cout << "\n  Pilih: ";
        cin >> pilih;

        switch (pilih) {
            case 1:
                adminAddCharacter();
                break;
            case 2:
                adminAddCharacterCombo();
                break;
            case 3:
                menuUndoSummon();
                break;
            case 4:
                return;
            default:
                cout << "\n  Pilihan tidak valid.\n";
                cin.ignore(1000, '\n');
                cin.get();
        }
    }
}

void menuAdmin() {
    char pass[64];
    cout << "\n  Masukkan Password Admin: ";
    cin.ignore(1000, '\n');
    readLine(pass, 64);

    if (strcmp(pass, ADMIN_PASS) == 0) {
        adminMenu();
    } else {
        cout << "\n  *** Access Denied ***\n";
        cout << "\n  Tekan ENTER untuk kembali...";
        cin.ignore(1000, '\n');
        cin.get();
    }
}

void showMainMenu() {
    clearScreen();
    printTitle("GENSHIN GACHA SYSTEM");
    cout << "\n";
    cout << "  1. Summon\n";
    cout << "  2. Inventory\n";
    cout << "  3. Character Features\n";
    cout << "  4. Statistics\n";
    cout << "  5. Admin Menu\n";
    cout << "  6. Exit\n";
    printLine();
    cout << "\n  Pilih menu: ";
}


int main() {
    srand((unsigned)time(nullptr));

    loadCharDB();
    loadInventory();
    loadGraph();
    initDefaultGraph();

    int pilih = 0;
    while (true) {
        showMainMenu();
        cin >> pilih;

        switch (pilih) {
            case 1:  menuSummon();             break;
            case 2:  menuInventory();          break;
            case 3:  menuCharacterFeatures();  break;
            case 4:  menuStatistics();         break;
            case 5:  menuAdmin();              break;
            case 6:
                cout << "\n  Terima kasih! Sampai jumpa, Traveler!\n\n";
                return 0;
            default:
                cout << "\n  Pilihan tidak valid.\n";
                cin.ignore(1000, '\n');
                cin.get();
        }
    }

    return 0;
}


void summonSingle(Character &picked, bool &got5Star) {
    Character fiveStar[100]; int fiveCount = 0;
    Character fourStar[200]; int fourCount = 0;

    for (int i = 0; i < charDB.count; i++) {
        if (strcmp(charDB.chars[i].rarity, "5 Star") == 0 && fiveCount < 100)
            fiveStar[fiveCount++] = charDB.chars[i];
        else if (strcmp(charDB.chars[i].rarity, "4 Star") == 0 && fourCount < 200)
            fourStar[fourCount++] = charDB.chars[i];
    }

    int roll = rand() % 100;
    got5Star = false;

    if (roll < 10 && fiveCount > 0) {
        picked = fiveStar[rand() % fiveCount];
        got5Star = true;
    } else if (fourCount > 0) {
        picked = fourStar[rand() % fourCount];
    } else if (fiveCount > 0) {
        picked = fiveStar[rand() % fiveCount];
        got5Star = true;
    } else {
        // fallback: empty char
        picked.name[0] = '\0';
        picked.rarity[0] = '\0';
        picked.element[0] = '\0';
        return;
    }

    inventory.append(picked);
    undoStack.push(picked);
    hashTable.insert(picked);
    addToSummonHistory(picked);

    stats.totalSummon++;
    if (got5Star) stats.total5Star++; else stats.total4Star++;
}

void menuSummon10x() {
    printTitle("SUMMON 10x");
    if (charDB.count == 0) {
        cout << "\n  [!] Database karakter kosong. Tambahkan karakter lewat Admin Menu.\n";
        cout << "\n  Tekan ENTER untuk kembali...";
        cin.ignore(1000, '\n');
        cin.get();
        return;
    }

    cout << "\n  Prepare 10 wishes...\n";
    cout << "  Press ENTER to begin...";
    cin.ignore(1000, '\n');
    cin.get();

    Queue results;
    cout << "\n  Summoning";
    for (int i = 0; i < 10; i++) {
        cout << "." << flush;
        Character picked; bool got5 = false;
        summonSingle(picked, got5);
        results.enqueue(picked);
    }
    cout << "\n\n  Done! Displaying results...\n\n";


    saveInventory();
    Character arr[10]; int n = 0;
    Character tmp;
    while (!results.isEmpty() && n < 10) {
        results.dequeue(tmp);
        arr[n++] = tmp;
    }

    int idx = 0;
    char cmd[8];
    auto showEntry = [&](int i){
        if (i < 0 || i >= n) return;
        cout << "\n[" << i+1 << "] ";
        if (strcmp(arr[i].rarity, "5 Star") == 0) cout << "*** ";
        cout << arr[i].name << " | " << arr[i].rarity << " | " << arr[i].element << "\n";
    };

    showEntry(idx);
    cout << "\n  Commands: n=next, a=all, q=quit\n";
    cin.ignore(1000, '\n');
    while (true) {
        cout << "  > ";
        cin.getline(cmd, 8);
        if (cmd[0] == 'n') {
            if (idx+1 < n) { idx++; showEntry(idx); }
            else cout << "  (end of results)\n";
        } else if (cmd[0] == 'a') {
            cout << "\n  -- All Results --\n";
            for (int i = 0; i < n; i++) showEntry(i);
        } else if (cmd[0] == 'q') {
            break;
        } else {
            cout << "  Unknown command. Use n/a/q.\n";
        }
    }

    cout << "\n  Returning to Main Menu...\n";
    cout << "\n  Tekan ENTER untuk kembali...";
    cin.get();
}