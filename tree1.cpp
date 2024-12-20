#include <iostream>
#include <string>
using namespace std;

const int MAX_NODES = 100; // Gioi han so luong node trong cuon sach

// Dinh nghia cau truc node
struct Node {
    string title;  // Tieu de muc
    int pages;     // So trang muc
    int parent;    // Chi so cua muc cha (-1 neu la root)
};

// Array de luu tru cac node trong cuon sach
Node book[MAX_NODES];
int nodeCount = 0; // So luong node hien tai trong cay

// Ham tinh lai so trang cua mot node dua tren cac muc con
void updatePages(int nodeIndex) {
    if (nodeIndex < 0 || nodeIndex >= nodeCount) return;

    int totalPages = book[nodeIndex].pages; // Bat dau voi so trang cua node

    // Cong them so trang cua cac muc con
    for (int i = 0; i < nodeCount; i++) {
        if (book[i].parent == nodeIndex) {
            totalPages += book[i].pages;
        }
    }

    // Cap nhat lai so trang cho node nay
    book[nodeIndex].pages = totalPages;

    // Neu node nay co cha, cap nhat so trang cho cha
    if (book[nodeIndex].parent != -1) {
        updatePages(book[nodeIndex].parent);
    }
}

// Ham them mot node 
bool addNode(string title, int pages, int parentIndex) {
    if (nodeCount >= MAX_NODES) return false; 

    book[nodeCount] = {title, pages, parentIndex};
    nodeCount++;

    // cap nhat lai so trang
    if (parentIndex != -1) {
        updatePages(parentIndex);
    }

    return true;
}

// Ham dem so chuong
int countChapters() {
    int count = 0;
    for (int i = 0; i < nodeCount; i++) {
        if (book[i].parent == 0) { 
            count++;
        }
    }
    return count;
}

// Ham tim chuong dai nhat
int findLongestChapter() {
    int longestIndex = -1;
    int maxPages = 0;
    for (int i = 0; i < nodeCount; i++) {
        if (book[i].parent == 0 && book[i].pages > maxPages) {
            maxPages = book[i].pages;
            longestIndex = i;
        }
    }
    return longestIndex;
}

// Ham xoa mot node va cap nhat lai so trang cho các node cha
bool deleteNode(int indexToDelete) {
    if (indexToDelete < 0 || indexToDelete >= nodeCount) return false;

    int parentIndex = book[indexToDelete].parent;

    // Xoa cac node con cua cay
    for (int i = 0; i < nodeCount; i++) {
        if (book[i].parent == indexToDelete) {
            deleteNode(i);
        }
    }

    // Dich cac node phia sau len truoc
    for (int i = indexToDelete; i < nodeCount - 1; i++) {
        book[i] = book[i + 1];
        if (book[i].parent >= indexToDelete) {
            book[i].parent--;
        }
    }

    nodeCount--;

    // Cap nhat lai so trang cho cha sau khi xoa
    if (parentIndex != -1) {
        updatePages(parentIndex);
    }

    return true;
}

// Ham tim node theo tieu de
int findNode(string title) {
    for (int i = 0; i < nodeCount; i++) {
        if (book[i].title == title) return i;
    }
    return -1; // Khong tim thay
}

// Ham in danh muc cuon sach theo thu tu chuyen de
void generateNumbering(int nodeIndex, string prefix) {
    cout << prefix << " " << book[nodeIndex].title << " (" << book[nodeIndex].pages << " trang)" << endl;

    int childNumber = 1;
    for (int i = 0; i < nodeCount; i++) {
        if (book[i].parent == nodeIndex) {
            generateNumbering(i, prefix + "." + to_string(childNumber));
            childNumber++;
        }
    }
}

int main() {
    // Khoi tao root cho cuon sach
    addNode("Cuon sach", 0, -1); // Root node khong co cha (-1)

    // Them cac chuong, muc va tiểu muc
    addNode("Chuong 1", 0, 0);   // Chuong 1 duoi root
    addNode("Chuong 2", 0, 0);   // Chuong 2 duoi root
    addNode("Muc 1.1", 10, 1); // Muc 1.1 duoi Chuong 1
    addNode("Muc 1.2", 20, 1); // Muc 1.2 duoi Chuong 1
    addNode("Muc 2.1", 15, 2); // Muc 2.1 duoi Chuong 2
    addNode("Tieu muc 1.2.1", 5, 3); // Tieu muc 1.2.1 duoi Muc 1.2

    // Cap nhat lai so trang cho root
    updatePages(0);

    // In danh muc cuon sach
    cout << "Danh muc cuon sach:" << endl;
    generateNumbering(0, "1");

    // Dem so chuong
    cout << "So chuong: " << countChapters() << endl;

    // Tim chuong dai nhat
    int longestIndex = findLongestChapter();
    if (longestIndex != -1) {
        cout << "Chuong dai nhat: " << book[longestIndex].title 
             << " co " << book[longestIndex].pages << " trang." << endl;
    }

    // Tim va xoa muc
    int indexToDelete = findNode("Muc 1.1");
    
    if (indexToDelete != -1) {
        deleteNode(indexToDelete);
        cout << "Da xoa 'Muc 1.1'" << endl;
    }
    indexToDelete = findNode("Chuong 2");
    if (indexToDelete != -1){
        deleteNode(indexToDelete);
        cout << "Da xoa 'Chuong 2'"<<endl;
    }

    // Cap nhat lai so trang cho root sau khi xoa
    updatePages(0);

    // In lai danh muc sau khi xoa
    cout << "Danh muc sau khi xoa:" << endl;
    generateNumbering(0, "1");

    // Dem so chuong sau khi xoa
    cout << "So chuong sau khi xoa: " << countChapters() << endl;

    return 0;
}
