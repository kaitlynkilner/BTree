
#include <iostream>
#include <vector>
#include <map>
using namespace std;
struct Node{
    Node* children[6] = {nullptr};
    double keys[5] = {0};
    map<double, Node*> keynodes;
    int size;
    bool ifLeaf;
    string URL;
    string name;
    string cat;
    string dev;
    double price;
    
    Node(string URL, string name, string cat, string dev, double price){
        /* add other constuctor information later */
        ifLeaf = true;
        size = 0;
        this->URL = URL;
        this->name = name;
        this->cat = cat;
        this->dev = dev;
        this->price = price;
    }
    

};

class BTree{
private:
    Node* root = nullptr;
    Node* SearchName(Node* root, string name);
    Node* SearchPrice(Node* root, double price);
    Node* SearchDeveloper(Node* root, string dev);
public:
    void Insert(double key, string URL, string name, string cat, string dev, double price);
    //ind is the index of the child node
    void Split(Node* root, int ind);
    Node* HighestKey();
    Node* publicSearchName(string name);
    Node* publicSearchPrice(double price);
    Node* publicSearchDeveloper(string name);
    vector<Node*> prices;
    vector<Node*> names;
    vector<Node*> devs;
};

void BTree::Insert(double key, string URL, string name, string cat, string dev, double price){
    if(root == nullptr){
        root = new Node(URL, name, cat, dev, price);
        root->keys[0] = key;
        root->keynodes.insert({key, root});
        root->size = 1;
    }
    //automatically split root if its full
    else{
    if(root->size == 5){
        Node* newroot = new Node("", "", "", "", 0);
        //makes new root key middle of child
        newroot->keys[0] = root->keys[2];
        newroot->keynodes.insert({root->keys[2], root->keynodes[root->keys[2]]});
        newroot->size = 1;
        //did all this manually sorry (:/ I was too lazy to code actual logic, if u can think of smth better pls fix this lol
        //keys on left of middle key get added to first new child, and keys on right of middle key get add to second middle child. These new children get added to the new root nodes children array. the children of the root node get readded to the respective new children node children arrays. Also dont forget to fix the ifLeaf variables and to set root as new root
        Node* child1 = root->children[0];
        Node* child2 = root->children[1];
        Node* child3 = root->children[2];
        Node* child4 = root->children[3];
        Node* child5 = root->children[4];
        Node* child6 = root->children[5];
        Node* newchild1 = new Node("", "", "", "", 0);
        Node* newchild2 = new Node("", "", "", "", 0);
        //insert key nodes into map
        newchild1->size = 2;
        newchild2->size = 2;
        newchild1->keys[0] = root->keys[0];
        newchild1->keys[1] = root->keys[1];
        newchild1->keynodes.insert({root->keys[0], root->keynodes[root->keys[0]]});
        newchild1->keynodes.insert({root->keys[1], root->keynodes[root->keys[1]]});
        newchild2->keys[0] = root->keys[3];
        newchild2->keys[1] = root->keys[4];
        newchild2->keynodes.insert({root->keys[3], root->keynodes[root->keys[3]]});
        newchild2->keynodes.insert({root->keys[4], root->keynodes[root->keys[4]]});
        newchild1->children[0] = child1;
        newchild1->children[1] = child2;
        newchild1->children[2] = child3;
        newchild2->children[0] = child4;
        newchild2->children[1] = child5;
        newchild2->children[2] = child6;
        //for(int i = 0; i < newchild1->size+1)
        for(int i = 0; i < root->size+1; i++){
            if(root->children[i] != nullptr){
                newchild1->ifLeaf = false;
                newchild2->ifLeaf = false;
            }
        }
        newroot->children[0] = newchild1;
        newroot->children[1] = newchild2;
        newroot->ifLeaf = false;
        
        root = newroot;
        
        
    }
    //checks if root is leaf node and add keys
    if(root->ifLeaf){
        int ind = -1;
        //finds index where key needs to be inserted
        for(int i = 0; i < root->size; i++){
            if(root->keys[i] < key){
                ind = i+1;
            }
        }
        //if key needs to be inserted at end
        if(ind == -1){
            for(int i = root->size; i > 0; i--){
                root->keys[i] = root->keys[i-1];
            }
            root->keys[0] = key;
            Node* newnode = new Node(URL, name, cat, dev, price);
            root->keynodes.insert({key, newnode});
            root->size = root->size+1;
        }
        //if key needs to be inserted in middle or beginning
        else{
            for(int i = root->size; i > ind; i--){
                root->keys[i] = root->keys[i-1];
        }
            root->keys[ind] = key;
            Node* newnode = new Node(URL, name, cat, dev, price);
            root->keynodes.insert({key, newnode});
            root->size = root->size+1;
        
        }
    }
    //if root has children
    else{
        //find where key belongs in key array, find corresponding child array [i+1]. ex. if key is lower than first element, it stays at 0 and if its higher than first element its 0+1, so it belongs at index 1 in child array.
        //split
        Node* curr = root;
        while(curr->children[0]->ifLeaf != true){
            //traverses tree to find where key belongs
            int ind = -1;
            for(int i = 0; i < curr->size; i++){
                if(curr->keys[i] < key){
                    ind = i;
                }
            }
            //if a node along the path is found to be full, split it
            if(curr->children[ind+1]->size == 5){
                Split(curr, ind+1);
            }
            curr = curr->children[ind+1];
        }
        int ind = -1;
        for(int i = 0; i < curr->size; i++){
            if(curr->keys[i] < key){
                ind = i;
            }}
        //checks if a child doesn't exist there yet
        if(curr->children[ind+1] == nullptr){
            curr->children[ind+1] = new Node(URL, name, cat, dev, price);
            curr->children[ind+1]->keys[0] = key;
            curr->children[ind+1]->size = 1;
            curr->children[ind+1]->keynodes.insert({key, curr->children[ind+1]});
        }
        else{
            //checks if child node is full, splits if it is
            if(curr->children[ind+1]->size == 5){
                Split(curr, ind+1);
                for(int i = 0; i < curr->size; i++){
                    if(curr->keys[i] < key){
                        ind = i;
                    }}
            }
            if(curr->children[ind+1]->size != 5){
                int inde = -1;
                for(int i = 0; i < curr->children[ind+1]->size; i++){
                    if(curr->children[ind+1]->keys[i] < key){
                        inde = i+1;
                    }
                }
                //if it needs to be inserted at the first index
                if(inde == -1){
                    for(int i = curr->children[ind+1]->size; i > 0; i--){
                        curr->children[ind+1]->keys[i] = curr->children[ind+1]->keys[i-1];
                    }
                    curr->children[ind+1]->keys[0] = key;
                    curr->children[ind+1]->size += 1;
                    Node* newnode = new Node(URL, name, cat, dev, price);
                    curr->children[ind+1]->keynodes.insert({key, newnode});
                }
                else{
                    for(int i = curr->children[ind+1]->size; i > inde; i--){
                        curr->children[ind+1]->keys[i] = curr->children[ind+1]->keys[i-1];
                    }
                    curr->children[ind+1]->keys[inde] = key;
                    curr->children[ind+1]->size += 1;
                    Node* newnode = new Node(URL, name, cat, dev, price);
                    curr->children[ind+1]->keynodes.insert({key, newnode});
                }
            }

            
        }
    }}
}

void BTree::Split(Node* root, int ind){
    //this function only works under the assumption the parent node isn't full
    Node* newchild = new Node("", "", "", "", 0);
    //if parent node isn't full, then that means the "last" child node there is would be at index 4
    if(ind == 4){
        double newkey = root->children[ind]->keys[2];
        root->keys[4] = newkey;
        root->keynodes.insert({newkey,root->children[ind]->keynodes[newkey]});
        double key1 = root->children[ind]->keys[3];
        double key2 = root->children[ind]->keys[4];
        newchild->keys[0] = key1;
        newchild->keys[1] = key2;
        newchild->size = 2;
        root->size += 1;
        root->children[ind]->size = 2;
        newchild->keynodes.insert({key1 ,root->children[ind]->keynodes[key1]});
        newchild->keynodes.insert({key2 ,root->children[ind]->keynodes[key2]});
        root->children[5] = newchild;
        for(int i = 2; i < 5; i++){
            //gets rid of nodes and keys in this child that have moved
            root->children[ind]->keynodes.erase(root->children[ind]->keys[i]);
            root->children[ind]->keys[i] = 0;
        }
        if(root->children[0]->ifLeaf == false){
            //transfer children of split array
            root->children[5]->ifLeaf = false;
            int j = 0;
            for(int i = 3; i < 6; i++){
                root->children[5]->children[j] = root->children[ind]->children[i];
                root->children[ind]->children[i] = nullptr;
                j++;
            }
        }
        
    }
    else{
        //move both keys and children over to insert new key
        for(int i = root->size; i > ind; i--){
            root->keys[i] = root->keys[i-1];
        }
        for(int i = root->size+1; i > ind+1; i--){
            root->children[i] = root->children[i-1];
        }
        double key = root->children[ind]->keys[2];
        root->keys[ind] = key;
        root->keynodes.insert({key, root->children[ind]->keynodes[key]});
        root->size += 1;
        double key1 = root->children[ind]->keys[3];
        double key2 = root->children[ind]->keys[4];
        newchild->keys[0] = key1;
        newchild->keys[1] = key2;
        newchild->size = 2;
        root->children[ind]->size = 2;
        newchild->keynodes.insert({key1 ,root->children[ind]->keynodes[key1]});
        newchild->keynodes.insert({key2 ,root->children[ind]->keynodes[key2]});
        root->children[ind+1] = newchild;
        for(int i = 2; i < 5; i++){
            //gets rid of nodes and keys in this child that have moved
            root->children[ind]->keynodes.erase(root->children[ind]->keys[i]);
            root->children[ind]->keys[i] = 0;
        }
        if(root->children[0]->ifLeaf == false){
            //transfer children of split array
            root->children[ind+1]->ifLeaf = false;
            int j = 0;
            for(int i = 3; i < 6; i++){
                root->children[ind+1]->children[j] = root->children[ind]->children[i];
                root->children[ind]->children[i] = nullptr;
                j++;
            }
        }
        }

}

Node* BTree::HighestKey(){
    Node* curr = root;
    Node* biggestNode = nullptr;
    double highest = 0;
    while(curr->ifLeaf == false){
        //finds largest index where there is a child
        int ind = 0;
        for(int i = 0; i < curr->size+1; i++){
            if(curr->children[i] != nullptr){
                ind = i;
            }
        }
        if(curr->keys[ind-1] > highest){
            highest = curr->keys[ind-1];
            biggestNode = curr;
        }
        curr = curr->children[ind];
        if(curr->keys[curr->size-1] > highest){
            highest = curr->keys[curr->size-1];
            biggestNode = curr;
        }
    }
    return biggestNode->keynodes[biggestNode->keys[biggestNode->size-1]];
}

Node* BTree::SearchName(Node* root, string name)
{
    if (root == nullptr)
    {
        return nullptr;
    }

    int total = root->size+1;

    // All the children except the last
    for (int i = 0; i < total - 1; i++)
    {
        SearchName(root->children[i], name);
    }

    // Print the current node's data
    for(int i = 0; i < root->size; i++){
        if(root->keynodes[root->keys[i]]->name == name){
            names.push_back(root->keynodes[root->keys[i]]);
        }
    }
    root = SearchName(root->children[total - 1], name);

    return root;
}

Node* BTree::publicSearchName(string name){
    Node* node = SearchName(this->root, name);
    return node;
}

Node* BTree::SearchPrice(Node* root, double price)
{
    if (root == nullptr)
    {
        return nullptr;
    }

    int total = root->size+1;

    // All the children except the last
    for (int i = 0; i < total - 1; i++)
    {
        SearchPrice(root->children[i], price);
    }

    // Print the current node's data
    for(int i = 0; i < root->size; i++){
        if(root->keynodes[root->keys[i]]->price == price){
            prices.push_back(root->keynodes[root->keys[i]]);
        }
    }
    root = SearchPrice(root->children[total - 1], price);

    return root;
}

Node* BTree::publicSearchPrice(double price){
    Node* node = SearchPrice(this->root, price);
    return node;
}

Node* BTree::SearchDeveloper(Node* root, string dev)
{
    if (root == nullptr)
    {
        return nullptr;
    }

    int total = root->size+1;

    // All the children except the last
    for (int i = 0; i < total - 1; i++)
    {
        SearchDeveloper(root->children[i], dev);
    }

    // Print the current node's data
    for(int i = 0; i < root->size; i++){
        if(root->keynodes[root->keys[i]]->name == dev){
            devs.push_back(root->keynodes[root->keys[i]]);
        }
    }
    root = SearchDeveloper(root->children[total - 1], dev);

    return root;
}

Node* BTree::publicSearchDeveloper(string dev){
    Node* node = SearchDeveloper(this->root, dev);
    return node;
}
