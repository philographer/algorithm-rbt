/***
 ## Memo
 1. key가 회원번호인 red-black tree, value는 info
 2. key가 금액인 red-black tree (금액정보, 회원번호), value는 회원번호?
 3. key가 x좌표 y좌표 합친 red-black tree (ex. x: 100, y: 101 일때 key 100101), value는
 3. 금액변동 내역은 array 또는 vector 로 관리하면 되나?
 ***/

//
//  main.cpp
//  algorithm-subject1
//
//  Created by 유호균 on 2018. 5. 15..
//  Copyright © 2018년 유호균. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#define RED   0
#define BLACK 1
#define EMPTY_NUM -999999

using namespace std;
struct Info;
struct FoundedNode;

/************
 *** Node ***
 ************/
class Node {
public:
    Node() {
        this->key = NULL;
        this->left = NULL;
        this->right = NULL;
        this->color = RED;
        this->info = NULL;
    }
    Node(int key, Node* _left = NULL, Node* _right = NULL, Info* _info = NULL) {
        this->key = key;
        this->left = _left;
        this->right = _right;
        this->color = RED;
        this->info = _info;
    }
    ~Node(){
        left = NULL;
        right = NULL;
        parent = NULL;
        info = NULL;
    }

    Node* getLeft() {
        return left;
    }
    
    Node* getRight() {
        return right;
    }
    
    Node* getParent() {
        return parent;
    }
    
    void setParent(Node* _parent) {
        this -> parent = _parent;
    }
    
    void setRight(Node* _right) {
        right = _right;
    }
    
    void setLeft(Node* _left) {
        left = _left;
    }
    
    Node* getUncle() {
        Node* p = this->parent;
        if (!p) return new Node(-1, NULL, NULL);
        if (this->isLeftNode()) return p->right;
        else if(this->isRightNode())return p->left;
        else return NULL; // Uncle is not exist
    }
    
    bool isLeftNode() const {
        if (this -> parent) {
            return this -> parent -> left == this;
        }
        return false;
    }
    
    bool isRightNode() const {
        if (this -> parent) {
            return this -> parent -> right == this;
        }
        return false;
    }
    
    Info *GetInfo() {
        return info;
    }
    
    int GetKey() {
        return key;
    }
    
    void SetBlack() {
        this -> color = BLACK;
    }
    
    void SetRed() {
        this -> color = RED;
    }
    
    bool isRed() {
        return this -> color == RED;
    }
    
    bool isBlack() {
        return this -> color == BLACK;
    }
    
    void swapColor(Node* node) {
        int c = this -> color;
        this->color = node->color;
        node->color = c;
    }
    
    void setInfo(Info* _info) {
        info = _info;
    }
    
private:
    int key; // 회원의 키는 회원번호가 된다.
    Node* left;
    Node* right;
    Node* parent;
    int color;
    Info* info;
};

struct Pos { // 시작 좌표
    
public:
    Pos(int _x, int _y) {
        x = _x;
        y = _y;
    }
    
    int x; // x 좌표
    int y; // y 좌표
};

struct Changed { // 금액변동내역
public:
    Changed(int _type, int _howManyMoney) {
        type = _type;
        howManyMoney = _howManyMoney;
    }
    int type; // 변동 종류: 현재금액 증가, 감소를 표현한 정수 (증가: 1, 감소: 0)
    int howManyMoney; // 변동 금액 : 1 ~ 5,000,000 사이의 임의의 정수
};

struct Info {
public:
    Info() {
        account_num = NULL;
        name = "";
        phone = "";
        position = NULL;
        grade = NULL;
        money = NULL;
        change = vector<Changed>();
    }
    void addToChange(int _moneyChange) {
        int type = -1;
        int moneyChange = _moneyChange;
        if(moneyChange > 0) {
            type = 1;
        } else {
            type = 0;
            moneyChange *= -1;
        }
        change.push_back(Changed(type, moneyChange));
    }
    int account_num; // 회원번호: 1,000,000 ~ 1,999,999 사이의 정수, (기준키, 유일함)
    string name; // 회원이름: 공백 없는 20Byte 이내의 문자열. 예) “김인하”
    string phone; // 연락처: 회원의 전화번호(공백 없는 11Byte 이내의 문자열). 예) “01012345678”
    Pos* position; // 시작 좌표 (x, y): x와 y는 각각. 0 ~ 1,000 사이의 임의의 정수.
    int grade; // 회원등급: 현재 금액에 따른 0 ~ 3 사이의 임의의 정수. (0: 일반, 1: 실버, 2: 골드, 3: 다이아), (현재금액에 대한 회원등급 - 일반: 0 , 실버: 30,000, 골드: 50,000, 다이아: 100,000)
    int money; // 현재금액: 0 ~ 100,000,000 사이의 정수
    vector<Changed> change;
    
    void setMoney(int newMoney) {
        money = newMoney;
    }
    
    void validateGrade() {
        if(money < 30000) {
            grade = 0;
        } else if(30000 <= money && money < 50000){
            grade = 1;
        } else if(50000 <= money && money < 100000){
            grade = 2;
        } else {
            grade = 3;
        }
    }
    
    
};

/********************
 *** Founded Node ***
 ********************/
struct FoundedNode {
    FoundedNode(Node* _node, int _depth) {
        node = _node;
        depth = _depth;
    }
    Node* node = NULL;
    int depth = 0;
};

/********************
 *** Inserted Node ***
 ********************/
struct InsertedNode {
    InsertedNode() {
        node = NULL;
        depth = 0;
        success = false;
    }
    InsertedNode(Node* _node, int _depth, bool _success) {
        node = _node;
        depth = _depth;
        success = _success;
    }
    Node* node = NULL;
    int depth = 0;
    bool success = false;
};
/*******************
 *** MoneyStatus ***
 *******************/
struct MoneyStatus {
    MoneyStatus() {
        money = 0;
        account_id = -99999;
    }
    
    MoneyStatus(int _money, int _account_id) {
        money = _money;
        account_id = _account_id;
    }
    
    int money = 0;
    int account_id = -99999;
    
};

vector<MoneyStatus> moneyStatus = vector<MoneyStatus>();

/**********************
 *** Red Black Tree ***
 **********************/

class RedBlackTree {
public:
    RedBlackTree() {
        Node* _nill = new Node(EMPTY_NUM, NULL, NULL);
        nill = new  Node(EMPTY_NUM, _nill, _nill);
        root = nill;
    }
    
    void _coloringAfterInsert(Node *);
    void InsertNode(Node* newNode, bool printDepth);
    Node* FindMaxNode(Node *);
    Node* _rotateLeft(Node *);
    Node* _rotateRight(Node *);
    Node* _insert(Node *, Node *, bool, int);
    Node* _insertMoney(MoneyStatus *node, MoneyStatus *newItem, bool printDepth);
    bool _isNullCheck(Node *);
    Node* GetRoot();
    FoundedNode FindNode(int, Node*, int);
private:
    Node* root;
    Node* nill;
};

void RedBlackTree::InsertNode(Node* newNode, bool printDepth=false) {
    if(_isNullCheck(root)) {
        root = newNode;
    } else {
        root = _insert(root, newNode, printDepth, 0);
        _coloringAfterInsert(newNode);
    }
    root->SetBlack();
    root->setParent(nill);
}

Node* RedBlackTree::_insert(Node *node, Node *newItem, bool printDepth, int _curDepth) {
    if(_isNullCheck(node)) {
        node = newItem;
        if(printDepth) {
            cout << _curDepth << " " << 1;
        }
    } else if(newItem->GetKey() == node -> GetKey()) { // 삽입시에 같은 회원번호는 거절
        if(printDepth) {
            cout << _curDepth << " " << 0 << endl;
        }
        return node;
    } else if (newItem->GetKey() > node -> GetKey()) { // >
        Node* child = _insert(node->getRight(), newItem, printDepth, _curDepth+1);
        node -> setRight(child);
        child -> setParent(node);
    } else { // newItem->GetKey() < node -> GetKey() // <
        Node* child = _insert(node->getLeft(), newItem, printDepth, _curDepth+1);
        node->setLeft(child);
        child -> setParent(node);
    }
    return node;
}

/*
Node* RedBlackTree::_insertMoney(MoneyStatus *node, Node *newItem, bool printDepth) {
    if(_isNullCheck(node)) {
        node = newItem;
    }else if (newItem->GetKey() >= node -> GetKey()) { // >=
        Node* child = _insert(node->getRight(), newItem, printDepth, _curDepth+1);
        node -> setRight(child);
        child -> setParent(node);
    } else { // newItem->GetKey() < node -> GetKey()
        Node* child = _insert(node->getLeft(), newItem, printDepth, _curDepth+1);
        node->setLeft(child);
        child -> setParent(node);
    }
    return node;
}
*/

void RedBlackTree::_coloringAfterInsert(Node *x) {
    // 루트 또는 블랙이면 문제없음
    // 레드 노드는 레드인 자식을 가질 수 없음
    if (_isNullCheck(x) || _isNullCheck(x->getParent()) || x == root || !x->isRed()) return;
    Node *p = x->getParent();
    
    // 부모가 루트 노드이거나 블랙이면 상관 없음
    if(p == root || !p->isRed()) return;
    
    // 현재 삽입된 노드는 레드, 부모 노드도 레드인 상황
    
    Node *s = p -> getUncle();
    Node *pp = p -> getParent();
    
    // Case1. Double RED
    if( s != NULL && s -> isRed()) {
        p -> SetBlack();
        s-> SetBlack();
        pp -> SetRed();
        _coloringAfterInsert(pp);
    }
    
    // Case 2. Unbalanced
    else {
        bool xLeft = x -> isLeftNode();
        bool pLeft = p -> isLeftNode();
        
        // Case 2-1
        // 방향이 엇갈리는 상황
        if(xLeft ^ pLeft) {
            if (xLeft) _rotateRight(p);
            else _rotateLeft(p);
            _coloringAfterInsert(p);
        }
        // Case 2-2
        else { // 한 방향으로 감
            if (pLeft) _rotateRight(pp);
            else _rotateLeft(pp);
            p -> swapColor(pp);
            _coloringAfterInsert(x);
        }
    }
}

Node* RedBlackTree::FindMaxNode(Node *x) {
    if (_isNullCheck(x->getRight())) {
        return x;
    }
    return FindMaxNode(x->getRight());
}

Node* RedBlackTree::_rotateLeft(Node *x) {
    if(_isNullCheck(x)) return x;
    Node *right = x -> getRight();
    Node *parent = x -> getParent();
    
    bool isLeft = x -> isLeftNode(); // 부모의 왼쪽 자식 노드인지?
    
    x -> setRight(right -> getLeft()); // 1039의 오른쪽이 NULL
    if(!_isNullCheck(x -> getRight())) {
        x -> getRight() -> setParent(x); // NULL의 부모가 x
    }

    right->setLeft(x);
    right->getLeft()->setParent(right);
    
    if (x == root) {
        root = right;
        root -> setParent(nill);
    } else {
        right -> setParent(parent);
        if(isLeft) parent -> setLeft(right);
        else parent->setRight(right);
    }
    
    return right;
}

Node* RedBlackTree::_rotateRight(Node *x) {
    if (_isNullCheck(x)) return x;
    Node *left = x -> getLeft(); // 안쓰는건가?
    Node *parent = x -> getParent();
    bool isLeft = x->isLeftNode();
    
    x -> setLeft(left->getRight()); // left의 right가 없다면?
    if(!_isNullCheck(x -> getLeft())) {
        x -> getLeft() -> setParent(left);
    }
    
    left -> setRight(x);
    left -> getRight() -> setParent(left);
    
    if ( x == root) {
        root = left;
        root -> setParent(nill);
    } else {
        left -> setParent(parent);
        if (isLeft) parent -> setLeft(left);
        else parent -> setRight(left);
    }
    
    return left;
}

bool RedBlackTree::_isNullCheck(Node *x){
    return x == NULL || x == nill;
}

Node* RedBlackTree::GetRoot() {
    return root;
}

FoundedNode RedBlackTree::FindNode(int account_num, Node* _thisRoot, int _curDepth) {
    Node* thisNode = _thisRoot;
    
    // thisNode가 null인지 검사
    if(_isNullCheck(thisNode)) {
        return FoundedNode(nill, 0);
    } else {
        // thisNode가 찾는 노드인지
        if(account_num == thisNode -> GetKey()) {
            return FoundedNode(thisNode, _curDepth);
        } else if(account_num < thisNode -> GetKey()) { // account_num < thisNode 의 key
            return FindNode(account_num, thisNode -> getLeft(), _curDepth+1);
        } else { // account_num > thisNode 의 key
            return FindNode(account_num, thisNode -> getRight(), _curDepth+1);
        }
    }
    
    return FoundedNode(thisNode, _curDepth);
}



/**
 * 1) I: 신규 회원가입
 * 2) P: 회원정보 확인: 특정 회원의 정보 출력
 * 3) A: 충전: 특정 회원의 캐쉬 충전처리
 * 4) F: 검색: 현재금액이 가장 높은 상위 5명의 정보 출력
 * 5) R: 특정 회원의 최근 금액변동내역 조회
 * 6) B: 땅 구매: 특정 회원의 땅 구매처리
 * 6) Q: 프로그램 종료
 */
enum QUERY_CASE {I=0, P, A, F, R, B, Q, ERR};

/* 기본 파일 입출력 */
bool inputFile(); // 파일 입력
bool excecuteQuery();
void insertUserFromFile(string);
void insertUserFromKeyboard();

/* 과제상 주어진 함수 SPEC */
void insertUser(int _account_num, string _name, string _phone, Pos* _position, int _grade, int _money, bool _printDepth = false); // I: 신규 회원가입
void printUser(); // P: 회원정보 확인: 특정 회원의 정보 출력
void accumulateCache(); // A: 충전: 특정 회원의 캐쉬 충전처리
void findTop5Rank(); // F: 검색: 현재금액이 가장 높은 상위 5명의 정보 출력
void recentChangePrint(); // R: 최근 조회: 특정 회원의 최근 금액변동내역 조회
void buyArea(); // B: 땅 구매: 특정 회원의 땅 구매처리
void quitProgram(); // Q: 프로그램 종료
void updateMoneyStatus(); // 상위 5명의 정보

QUERY_CASE parseCase(string);

RedBlackTree *rbt = new RedBlackTree(); // 레드블랙트리 생성 -> 전역 메모리

int main(int argc, const char * argv[]) {
    
//    insertUser(int _account_num, string _name, string _phone, Pos _position, int _grade, int _money)
//    insertUser(1839993, "박짱구", "01093730487", new Pos(651, 787), 0, 18000);
//    insertUser(1039826, "이진구", "01091118792", new Pos(228, 891), 0, 19000);
//    insertUser(1717492, "강백호", "01020085150", new Pos(195, 112), 0, 22000);
//    insertUser(1728484, "박짱구", "01024486501", new Pos(43, 828), 0, 13000);
//    insertUser(1555645, "홍길동", "01017650037", new Pos(732, 71), 1, 45000);
//    insertUser(1250382, "조나단", "01090121196", new Pos(614, 836), 0, 4000);
//    insertUser(1151808, "조나단", "01022519193", new Pos(961, 485), 0, 5000);
//    insertUser(1802596, "박짱구", "01046717774", new Pos(886, 730), 0, 4000);
//    insertUser(1943329, "김철수", "01053284363", new Pos(603, 103), 0, 8000);
//    insertUser(1118549, "홍길동", "01036783852", new Pos(356, 463), 1, 45000);
    
    bool res = inputFile();
    if (!res) { // 파일이 없으면 종료
        return 0;
    }
    
    // 중복 삽입 I 1839993 박짱구 01093730487 651 787
    
    bool isQuit = false;
    while (!isQuit) {
        bool res = excecuteQuery();
        if(res) isQuit = true;
    }
    
    return 0;
}


// inputFile and insertUser
bool inputFile() {
    string fileName;
    string inputStr;
    
    cout << "입력파일의 이름을 입력하세요 : ";
    cin >> fileName;
    ifstream inFile(fileName);
    
    if(inFile) {
        while(!inFile.eof()) {
            getline(inFile, inputStr);
            if(*inputStr.c_str() != '\0') { // 파일의 끝의 empty enter
                insertUserFromFile(inputStr);
            }
        }
        inFile.close();
        
        return true;
    } else {
        cout << "파일이 존재하지 않음" << endl;
        return false;
    }
    
    
}

bool excecuteQuery() {
    string userInput;
    cin >> userInput;
    QUERY_CASE queryCase = parseCase(userInput);
    
    switch (queryCase) {
        case QUERY_CASE::I:
            insertUserFromKeyboard();
            return false;
            break;
        case QUERY_CASE::P:
            printUser();
            return false;
            break;
        case QUERY_CASE::A:
            accumulateCache();
            return false;
            break;
        case QUERY_CASE::F:
            findTop5Rank();
            return false;
            break;
        case QUERY_CASE::R:
            recentChangePrint();
            return false;
            break;
        case QUERY_CASE::B:
            buyArea();
            return false;
            break;
        case QUERY_CASE::Q: // 프로그램 종료
            quitProgram();
            return true;
            break;
        default: // 프로그램 종료
            return true;
            break;
    }
}

QUERY_CASE parseCase(string _userInput) {
    if(_userInput == "I") {
        return QUERY_CASE::I;
    } else if(_userInput == "P"){
        return QUERY_CASE::P;
    } else if(_userInput == "A"){
        return QUERY_CASE::A;
    } else if(_userInput == "F"){
        return QUERY_CASE::F;
    } else if(_userInput == "R"){
        return QUERY_CASE::R;
    } else if(_userInput == "B"){
        return QUERY_CASE::B;
    } else if(_userInput == "Q"){
        return QUERY_CASE::Q;
    } else {
        return QUERY_CASE::ERR;
    }
}

void readFile(string _fileName) {
    
}

void insertUserFromFile(string _inputStr) {
    int account_num;
    string name;
    string phone;
    int x;
    int y;
    Pos* pos = new Pos(0, 0);
    int grade;
    int money;
    
    stringstream(_inputStr) >> account_num >> name >> phone >> x >> y >> grade >> money;
    pos -> x = x;
    pos -> y = y;
    
    insertUser(account_num, name, phone, pos, grade, money, true);
}

/*
 int account_num; // 회원번호: 1,000,000 ~ 1,999,999 사이의 정수, (기준키, 유일함)
 string name; // 회원이름: 공백 없는 20Byte 이내의 문자열. 예) “김인하”
 string phone; // 연락처: 회원의 전화번호(공백 없는 11Byte 이내의 문자열). 예) “01012345678”
 Pos position; // 시작 좌표 (x, y): x와 y는 각각. 0 ~ 1,000 사이의 임의의 정수.
 int grade; // 회원등급: 현재 금액에 따른 0 ~ 3 사이의 임의의 정수. (0: 일반, 1: 실버, 2: 골드, 3: 다이아), (현재금액에 대한 회원등급 - 일반: 0 , 실버: 30,000, 골드: 50,000, 다이아: 100,000)
 int money; // 현재금액: 0 ~ 100,000,000 사이의 정수
 Changed change;
 */

// I: 신규 회원가입: ex) I 1436848 마이클 01052558417 565 683
// 회원번호는 유일하기 때문에, 만약 기존에 동일한 회원번호가 존재한다면, 그 노드의 깊이를 출력하고, 신규 회원가입은 거절된다. 입력된 땅 좌표의 주인이 없다면, 신규 회원이 주인이 된다.
void insertUserFromKeyboard() {
    int account_num;
    string name;
    string phone;
    int x;
    int y;
    Pos* pos = new Pos(0, 0);
    
    // I 1436848 마이클 01052558417 565 683
    cin >> account_num;
    cin >> name;
    cin >> phone;
    cin >> x;
    cin >> y;
    pos -> x = x;
    pos -> y = y;
    
    // 거절구현 필요
    insertUser(account_num, name, phone, pos, 0, 0, true);
}

// I: 신규 회원가입
void insertUser(int _account_num, string _name, string _phone, Pos* _position, int _grade, int _money, bool _printDepth) {
    Info *newInfo = new Info();
    // 키보드에 의한 회원가입
    // 파일에 의한 회원가입
    
    
    
    newInfo -> account_num = _account_num;
    newInfo -> name = _name;
    newInfo -> phone = _phone;
    newInfo -> position = _position;
    newInfo -> grade = _grade;
    newInfo -> money = _money;
    
    // Right Left 없이 삽입
    Node *newNode = new Node(_account_num, NULL, NULL, newInfo);;
    rbt -> InsertNode(newNode, _printDepth);
    
// I: 신규 회원가입 질의를 나타내는 기호
// K: 회원번호
// N: 회원이름
// H: 연락처
// Ax: 시작 x 좌표
// Ay: 시작 y 좌표
    
// - 출력형식: “D S”
// D: 회원번호가 저장된 노드의 트리 내 깊이 (루트노드의 깊이는 0)
// S: 회원가입의 승인 상태를 나타내는 0 또는 1인 수 (0: 거절, 1: 승인)
}

// P: 회원정보 확인: 특정 회원의 정보 출력
/* 출력
 N: 회원이름
 H: 연락처
 G: 회원등급
 M: 회원의 현재금액
 D: 해당 회원이 저장된 노드의 트리 내 깊이
 */
void printUser() {
    int account_num;
    cin >> account_num;
    FoundedNode foundedNode = rbt -> FindNode(account_num, rbt -> GetRoot(), 0);
    Node* node = foundedNode.node;
    
    if(rbt -> _isNullCheck(node)) { // null 이 반환되면 없는 것 임.
        cout << "Not found!" << endl;
    } else {
        Info* nodeInfo = node -> GetInfo();
        cout << nodeInfo -> name << " " << nodeInfo -> phone << " " << nodeInfo -> grade << " " << nodeInfo -> money << " " << foundedNode.depth << endl;
    }
}

// A: 충전: 특정 회원의 캐쉬 충전처리 -> 등급이 바뀌어야 함
/* 입력
 A: 충전 처리 질의를 나타내는 기호
 K: 회원번호
 G: 충전 금액
 */
/* 출력
 "D R" 또는 "Not found!"
 D: 캐쉬 충전을 요청한 회원이 저장된 노드의 트리 내 깊이 (루트노드의 깊이는 0) R: 충전 후 해당 회원의 등급
 */
void accumulateCache() {
    int account_num;
    int newMoney;
    cin >> account_num;
    cin >> newMoney;
    FoundedNode foundedNode = rbt -> FindNode(account_num, rbt -> GetRoot(), 0);
    Node* node = foundedNode.node;
    
    if(rbt -> _isNullCheck(node)) { // null 이 반환되면 없는 것 임.
        cout << "Not found!" << endl;
    } else {
        Info* nodeInfo = node -> GetInfo();
        nodeInfo -> setMoney(nodeInfo->money + newMoney); // 현재 돈 + 추가된 돈
        nodeInfo -> validateGrade(); // 등급 변경
        nodeInfo -> addToChange(newMoney);// 금액변동 내역에 추가
        cout << foundedNode.depth << " " << node -> GetInfo() -> grade << endl;
    }
}

// F: 검색: 현재금액이 가장 높은 상위 5명의 정보 출력
void findTop5Rank() {
    
}


// R: 특정 회원의 최근 금액변동내역 조회
/* 입력
 - 질의형식 : “R K L”
 R: 최근 금액변동내역 조회 질의를 나타내는 기호
 K: 회원번호
 L: 조회할 최근 변동내역의 수
 */
/*
 - 출력형식: “(조건을 만족하는 변동 금액 내역별로 한 줄에)C G” 또는 “0” 또는 “Not found!” C: 변동된 금액의 종류 (증가 : 1, 감소 : 0)
 G: 변동된 금액
 */
void recentChangePrint() {
    int account_num;
    int how_many_print;
    cin >> account_num >> how_many_print;
    
    FoundedNode foundedNode = rbt -> FindNode(account_num, rbt -> GetRoot(), 0);
    Node* node = foundedNode.node;
    if(rbt -> _isNullCheck(node)) { // null 이 반환되면 없는 것 임.
        cout << "Not found!" << endl;
    } else {
        Info* nodeInfo = node -> GetInfo();
        vector<Changed> changed = nodeInfo -> change;
        int count = 0;
        if(changed.size() == 0) {
            cout << 0 << endl;
        } else {
            for(int i = (int)changed.size()-1; i >= 0; i--) {
                cout << changed[i].type << " " << changed[i].howManyMoney << endl;
                count++;
                if(count == how_many_print) break;
            }
        }
    }
}

// B: 땅 구매: 특정 회원의 땅 구매처리
/*
 - 질의형식 : “B N Ax Ay M”
 B: 땅 구매 질의를 나타내는 기호
 N: 회원번호
 Ax: 땅의 x 좌표
 Ay: 땅의 y 좌표
 M: 거래금액
*/
/*
 - 출력형식: “F L D”
 F: 땅 구매 여부 ( 승인 : 1, 거절 : 0 ) L: 구매 요청한 회원의 현재금액
 D: 현재 땅 주인의 회원번호
 */
void buyArea() {
    int account_num;
    int xPos;
    int yPos;
    int price;
    
    cin >> account_num >> xPos >> yPos >> price;
    Pos* pos = new Pos(xPos, yPos);
}

// Q: 프로그램 종료
void quitProgram() {
    
}


// 최대 5개의 정보 출력
bool acompare(MoneyStatus lhs, MoneyStatus rhs) { return lhs.money < rhs.money; }
// 1000 1000 1000 1000 1000
void updateMoneyStatus(int account_num, int money) {
    if(moneyStatus.size() < 5) {
        moneyStatus.push_back(MoneyStatus(account_num, money));
        sort(moneyStatus.begin(), moneyStatus.end(), acompare);
    } else { // 5개 초과하면
        for(int i=0; i < 5; i++) {
            if(moneyStatus[i].money == account_num) {
                
            } else if(moneyStatus[i].money >= account_num) {
                
            }
        }
        moneyStatus.erase(moneyStatus.begin());
        moneyStatus.push_back(MoneyStatus(account_num, money));
    }
}

/*
 moneyStatus 언제 업데이트
 1. 파일에서 집어넣을때
 2. 땅을 팔고, 살때
 3. 키보드로 I(insert) 쿼리 할 때
 4. 충전 할 때
 */
